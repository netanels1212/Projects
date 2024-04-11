#include <csignal>      // sigfillset, pthread_sigmask 
#include <sys/socket.h> // socketpair 
#include <stdexcept>    // runtime_error 
#include <sys/ioctl.h>  // ioctl 
#include <fcntl.h>      // open, RDWR 
#include <linux/nbd.h>  // NBD_SET_BLKSIZE, NBD_SET_SIZE, NBD_SET_SIZE_BLOCKS
#include <thread>       // thread 
#include <netinet/in.h> // htonl 
#include <iostream>     // cout

#include "nbd.hpp"
#include "fproject_utils.hpp"
#include "logger.hpp"

#include "task.hpp"

namespace details
{
using namespace ilrd_hrd33;

int g_nbd_fd;

void CreateSocketPair(int *sp_)
{
    if(-1 == socketpair(AF_UNIX, SOCK_STREAM, 0, sp_))
    {
        LOG_E("Failed to create socket pair");
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error("Failed to create socket pair");
    }
}

void OpenNBD(const char* nbd_name_, int *nbd_) 
{
    *nbd_ = open(nbd_name_, O_RDWR);

    if(-1 == *nbd_) 
    {
        LOG_E("Failed to open nbd_name_");
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error("Failed to open NBD device");
    }

    g_nbd_fd = *nbd_;
}

void SetNbdOptions(size_t mem_size_, size_t block_size_, size_t num_of_blocks_, int nbd_)
{
    if(block_size_) 
    {
        if(-1 == ioctl(nbd_, NBD_SET_BLKSIZE, block_size_))
        {
            LOG_E("Failed to ioctl - NBD_SET_BLKSIZE");
            LOG_E_L("nbd.cpp", __LINE__);
            throw std::runtime_error("ioctl NBD_SET_BLKSIZE failed");
        }
    }

    if(mem_size_) 
    {
        if(-1 == ioctl(nbd_, NBD_SET_SIZE, mem_size_))
        {
            LOG_E("Failed to ioctl - NBD_SET_SIZE");
            LOG_E_L("nbd.cpp", __LINE__);
            throw std::runtime_error("ioctl NBD_SET_SIZE failed");
        }
    }

    if(num_of_blocks_) 
    {
        if(-1 == ioctl(nbd_, NBD_SET_SIZE_BLOCKS, num_of_blocks_))
        {
            LOG_E("Failed to ioctl - NBD_SET_SIZE_BLOCKS");
            LOG_E_L("nbd.cpp", __LINE__);
            throw std::runtime_error("ioctl NBD_SET_SIZE_BLOCKS failed");
        }
    }

    if(-1 == ioctl(nbd_, NBD_CLEAR_SOCK))
    {
        LOG_E("Failed to ioctl - NBD_CLEAR_SOCK");
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error("ioctl NBD_CLEAR_SOCK failed");
    }
}

void SetupNBD(int server_fd_, int nbd_)
{
    sigset_t sigset;
    int flags;

    if (0 != sigfillset(&sigset) || 0 != pthread_sigmask(SIG_SETMASK, &sigset, NULL))
    {
        LOG_E("Failed to sig mask");
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error("Failed to mask signals");
    }

    if (-1 == ioctl(nbd_, NBD_SET_SOCK, server_fd_))
    {
        LOG_E("Failed to ioctl - NBD_SET_SOCK");
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error("ioctl NBD_SET_SOCK failed");
    }

#if defined NBD_SET_FLAGS
        flags = 0;
#if defined NBD_FLAG_SEND_TRIM
        flags |= NBD_FLAG_SEND_TRIM;
#endif
#if defined NBD_FLAG_SEND_FLUSH
        flags |= NBD_FLAG_SEND_FLUSH;
#endif
    if (flags != 0 && ioctl(nbd_, NBD_SET_FLAGS, flags) == -1)
    {
        LOG_E("Failed to ioctl - NBD_SET_FLAGS");
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error("ioctl NBD_SET_FLAGS failed");
    }
#endif
    if (-1 == ioctl(nbd_, NBD_DO_IT))
    {
        LOG_E("Failed to ioctl - NBD_DO_IT");
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error("ioctl NBD_DO_IT failed");
    }

    if (-1 == ioctl(nbd_, NBD_CLEAR_QUE))
    {
        LOG_E("Failed to ioctl - NBD_CLEAR_QUE");
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error("ioctl NBD_CLEAR_QUE failed");
    }

    if (-1 == ioctl(nbd_, NBD_CLEAR_SOCK))
    {
        LOG_E("Failed to ioctl - NBD_CLEAR_SOCK");
        LOG_E_L("nbd.cpp", __LINE__);       
        throw std::runtime_error("ioctl NBD_CLEAR_SOCK failed");
    }
}

void NbdDisconnectHandler(int signo_)
{
    if (-1 == ioctl(g_nbd_fd, NBD_DISCONNECT))
    {
        LOG_E("Failed to ioctl - NBD_DISCONNECT");
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error("ioctl NBD_DISCONNECT failed");
    }

    else
    {
        g_nbd_fd = -1;
    }
}

static int SetHandler(int sig_, const struct sigaction *act_)
{
    struct sigaction oact;
    int ret = sigaction(sig_, act_, &oact);

    if (0 == ret && oact.sa_handler != SIG_DFL)
    {
        LOG_E("Failed to sigaction - SIG_DFL");
        LOG_E_L("nbd.cpp", __LINE__);    
    }

    return ret;
}

void SetupDisconnection(int m_nbd_)
{
    (void)m_nbd_;
    struct sigaction act;

    act.sa_handler = NbdDisconnectHandler;
    act.sa_flags = SA_RESTART;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);
    sigaddset(&act.sa_mask, SIGTERM);

    SetHandler(SIGINT, &act);
    SetHandler(SIGTERM, &act);
}

static int WriteAll(int fd_, char *buf_, size_t count_)
{
    int bytes_written = 0;

    while (0 < count_)
    {
        bytes_written = write(fd_, buf_, count_);
        buf_ += bytes_written;
        count_ -= bytes_written;
    }
    LOG_D("Writing all succeded");

    return 0;
}

static int ReadAll(int fd_, char *buf_, size_t count_)
{
    int bytes_read = 0;

    while (0 < count_)
    {
        bytes_read = read(fd_, buf_, count_);
        buf_ += bytes_read;
        count_ -= bytes_read;
    }
    LOG_D("Reading all succeded");

    return 0;
}

void InitConnection(int *fd_, int *nbd_,
                    size_t mem_size_, size_t block_size_, size_t num_of_blocks_, const char* dev_file_)
{
    try
    {
        CreateSocketPair(fd_);
        LOG_D("Socket pair creation succeded");
        OpenNBD(dev_file_, nbd_);
        LOG_D("Opening NBD succeded");
        SetNbdOptions(mem_size_, block_size_, num_of_blocks_, *nbd_);
        LOG_D("Set options to NBD succeded");
    }
    catch (const std::runtime_error &e)
    {
        LOG_E(e.what());
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error(e.what());
    }
}
}//end of namespace details

namespace ilrd_hrd33
{
using namespace details;

NBD_Driver_Communicator::NBD_Driver_Communicator(size_t mem_size_, const char* dev_file_):
                         m_mem_size(mem_size_), m_block_size(0), m_block_count(0)
{
    try
    {
        details::InitConnection(m_fds, &m_nbd, mem_size_, m_block_size, m_block_count, dev_file_);
        LOG_D("Init connection successed");
        LOG_D_L("nbd.cpp", __LINE__);

        m_thread = std::thread(details::SetupNBD, m_fds[1], m_nbd);
        LOG_D("NBD Thread is up");

        details::SetupDisconnection(m_nbd);
        LOG_D("Setup Disconnection successed");
    }
    catch (const std::runtime_error &e)
    {
        LOG_E(e.what());
        LOG_E_L("nbd.cpp", __LINE__);
        throw std::runtime_error(e.what());
    }
}

NBD_Driver_Communicator::NBD_Driver_Communicator(size_t block_size_, size_t num_of_blocks_, const char* dev_file_):
                                                 NBD_Driver_Communicator(block_size_ * num_of_blocks_, dev_file_) {}

NBD_Driver_Communicator::~NBD_Driver_Communicator() noexcept 
{
    m_thread.join();
    LOG_D("NBD thread joined");
}

std::shared_ptr<DriverData> NBD_Driver_Communicator::ReadRequest()
{
    std::shared_ptr<DriverData> ret(new DriverData);
    struct nbd_request request;
    ssize_t bytes_read;

    bytes_read = read(m_fds[0], &request, sizeof(request));

    if (-1 == bytes_read)
    {
        LOG_I("Failed to read from socket");
        LOG_I_L("nbd.cpp", __LINE__);
        ret->m_type = TYPE::DISC;

        return ret;
    }

    unsigned char *temp;
    ret->m_error = ERROR::SUCCESS;
    ret->m_len = ntohl(request.len);
    ret->m_from = ntohll(request.from);
    std::copy(request.handle, request.handle + 8, ret->m_handle);
    LOG_D("Copied request handle");

    switch (ntohl(request.type))
    {
    case NBD_CMD_READ:
        std::cout << "Request for read of size " << ntohl(request.len) << std::endl;
        std::cout << "Got NBD_CMD_READ" << std::endl;
        ret->m_type = TYPE::READ;
        LOG_D("Request type: NBD_CMD_READ");
        return ret;

    case NBD_CMD_WRITE:
        std::cout << "Request for write of size " << ntohl(request.len) << std::endl;
        std::cout << "Got NBD_CMD_WRITE" << std::endl;
        temp = new unsigned char[ret->m_len];
        ret->m_type = TYPE::WRITE;
        ReadAll(m_fds[0], (char *)temp, ret->m_len);
        ret->m_buffer.clear();
        ret->m_buffer.assign(temp, temp + ret->m_len);
        delete[] temp;
        LOG_D("Request type: NBD_CMD_WRITE");
        return ret;

    case NBD_CMD_DISC:
        std::cout << "Got NBD_CMD_DISC" << std::endl;
        ret->m_type = TYPE::DISC;
        LOG_D("Request type: NBD_CMD_DISC");
        return ret;

    case NBD_CMD_FLUSH:
        std::cout << "Got NBD_CMD_FLUSH" << std::endl;
        ret->m_type = TYPE::FLUSH;
        LOG_D("Request type: NBD_CMD_FLUSH");
        return ret;

    case NBD_CMD_TRIM:
        std::cout << "Got NBD_CMD_TRIM" << std::endl;
        ret->m_type = TYPE::TRIM;
        LOG_D("Request type: NBD_CMD_TRIM");
        return ret;

    default:
        LOG_D("Request type: unknown");
        break;
    }

    return ret;
}

void NBD_Driver_Communicator::SendReply(std::shared_ptr<DriverData> request_)
{
    struct nbd_reply reply;
    size_t len = request_->m_buffer.size();
    unsigned char *temp = new unsigned char[len];
    std::copy(request_->m_handle, request_->m_handle + 8, reply.handle);
    LOG_D("Copied reply handle");

    reply.magic = htonl(NBD_REPLY_MAGIC);
    reply.error = htonl(0);

    switch (request_->m_type)
    {
    case TYPE::READ:
        WriteAll(m_fds[0], (char *)&reply, sizeof(struct nbd_reply));
        std::copy(request_->m_buffer.begin(), request_->m_buffer.end(), temp);
        request_->m_buffer.clear();
        WriteAll(m_fds[0], (char *)temp, len);
        break;

    case TYPE::WRITE:
    case TYPE::FLUSH:
    case TYPE::TRIM:
    case TYPE::DISC:
        WriteAll(GetFD(), (char *)&reply, sizeof(struct nbd_reply));
        break;

    default:
        break;
    }

    delete[] temp;
}

int NBD_Driver_Communicator::GetFD() const
{
    return m_fds[0];
}

void NBD_Driver_Communicator::Disconnect()
{
    if (g_nbd_fd != -1)
    {
        if (-1 == ioctl(g_nbd_fd, NBD_DISCONNECT))
        {
            LOG_E("Failed to ioctl - NBD_DISCONNECT");
            LOG_E_L("nbd.cpp", __LINE__);
            throw std::runtime_error("ioctl NBD_DISCONNECT failed");
        }
    }

    close(m_fds[1]);
    LOG_E("closed nbd socket");
}
} //end of namespace ilrd_hrd33



