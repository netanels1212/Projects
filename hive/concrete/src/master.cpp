#include "task.hpp"
#include "request_engine.hpp"
#include "driver.hpp"
#include "storage.hpp"
#include "ramstorage.hpp"
#include "nbd.hpp"

using namespace ilrd_hrd33;

template<typename KEY, typename ARGS>
class NBDGateway: public IGateway<KEY, ARGS>
{
    public:
        explicit NBDGateway(std::shared_ptr<NBD_Driver_Communicator> nbd_):m_nbd(nbd_) {}
        ~NBDGateway()noexcept = default;
        int GetFd() const override {return m_nbd->GetFD();}
        std::pair<KEY, ARGS> Read() override
        {
            std::shared_ptr<DriverData>req = m_nbd->ReadRequest();
            return std::make_pair(static_cast<TYPE>(req->m_type), req);
        }
    private:
        std::shared_ptr<NBD_Driver_Communicator> m_nbd;
};

class ThreadSafeReplyTask : public ITask
{
public:
    ThreadSafeReplyTask(std::shared_ptr<RAM_Storage> ram_, std::shared_ptr<IDriverCommunicator> driver_
                                        ,std::shared_ptr<DriverData> request_)
        : m_storage(ram_), m_request(request_), m_driver(driver_){}
    
    virtual void Execute() override 
    {
        m_lock.lock();
        m_driver->SendReply(m_request);
        m_lock.unlock();
    }
    static std::mutex m_lock;
protected:
    std::shared_ptr<RAM_Storage> m_storage;
    std::shared_ptr<DriverData> m_request;
    std::shared_ptr<IDriverCommunicator> m_driver;
};

class ReadTask : public ThreadSafeReplyTask
{
public:
    ReadTask(std::shared_ptr<RAM_Storage> ram_, std::shared_ptr<IDriverCommunicator> driver_
                                        , std::shared_ptr<DriverData> request_)
        : ThreadSafeReplyTask(ram_, driver_, request_) {}

    virtual void Execute() override 
    {
        m_storage->Read(m_request);
        ThreadSafeReplyTask::Execute();
    }
};

class WriteTask : public ThreadSafeReplyTask
{
public:
    WriteTask(std::shared_ptr<RAM_Storage> ram_, std::shared_ptr<IDriverCommunicator> driver_
                                        ,std::shared_ptr<DriverData> request_)
        : ThreadSafeReplyTask(ram_, driver_, request_) {}

    virtual void Execute() override 
    {
        m_storage->Write(m_request);
        ThreadSafeReplyTask::Execute();
    }
};

class DisconnectTask : public ThreadSafeReplyTask
{
public:
    DisconnectTask(std::shared_ptr<RAM_Storage> ram_, std::shared_ptr<IDriverCommunicator> driver_
                                        , RequestEngine<TYPE, std::shared_ptr<DriverData>> &engine,
                                         std::shared_ptr<DriverData> request_)
        : ThreadSafeReplyTask(ram_, driver_, request_), m_engine(engine) {}

    virtual void Execute() override 
    {
        m_driver->Disconnect();
        ThreadSafeReplyTask::Execute();
        m_engine.Stop();
    }

    private:
    RequestEngine<TYPE, std::shared_ptr<DriverData>> &m_engine;
};

class FlushTask : public ThreadSafeReplyTask
{
public:
    FlushTask(std::shared_ptr<RAM_Storage> ram_, std::shared_ptr<IDriverCommunicator> driver_
                                        ,std::shared_ptr<DriverData> request_)
        : ThreadSafeReplyTask(ram_, driver_, request_) {}

    virtual void Execute() override 
    {
        ThreadSafeReplyTask::Execute();
    }
};

class TrimTask : public ThreadSafeReplyTask
{
public:
    TrimTask(std::shared_ptr<RAM_Storage> ram_, std::shared_ptr<IDriverCommunicator> driver_
                                        ,std::shared_ptr<DriverData> request_)
        : ThreadSafeReplyTask(ram_, driver_, request_) {}

    virtual void Execute() override 
    {
        ThreadSafeReplyTask::Execute();
    }
};

std::mutex ThreadSafeReplyTask::m_lock;

int main(int argc, char**argv)
{
    (void)argc;
    Singleton<Logger>::GetInstance()->SetOutput(std::cerr);    
    LOG_SETDEBUG();
    std::shared_ptr<RAM_Storage> rs = std::make_shared<RAM_Storage>(134217728);
    std::shared_ptr<NBD_Driver_Communicator> nbd = std::make_shared<NBD_Driver_Communicator>(134217728,argv[1]);
    
    // Create an instance of RequestEngine
    RequestEngine<TYPE, std::shared_ptr<DriverData>> engine("log.txt", 4);

    // Add the gateway
    std::shared_ptr<NBDGateway<TYPE, std::shared_ptr<DriverData>>> gateway = 
                                std::make_shared<NBDGateway<TYPE, std::shared_ptr<DriverData>>>(nbd);
    engine.AddGateWay(gateway);

    engine.ConfigTask([rs, nbd](std::shared_ptr<DriverData> arg) -> std::shared_ptr<ITask>
    {
        return std::make_shared<ReadTask>(rs, nbd, arg);
    }, TYPE::READ);
    engine.ConfigTask([rs, nbd](std::shared_ptr<DriverData> arg) -> std::shared_ptr<ITask>
    {
        return std::make_shared<WriteTask>(rs, nbd, arg);
    }, TYPE::WRITE);
    engine.ConfigTask([rs, nbd, &engine](std::shared_ptr<DriverData> arg) -> std::shared_ptr<ITask>
    {
        return std::make_shared<DisconnectTask>(rs, nbd, engine, arg);
    }, TYPE::DISC);
    engine.ConfigTask([rs, nbd](std::shared_ptr<DriverData> arg) -> std::shared_ptr<ITask>
    {
        return std::make_shared<FlushTask>(rs, nbd, arg);
    }, TYPE::FLUSH);
    engine.ConfigTask([rs, nbd](std::shared_ptr<DriverData> arg) -> std::shared_ptr<ITask>
    {
        return std::make_shared<TrimTask>(rs, nbd, arg);
    }, TYPE::TRIM);

    engine.Run();
    
    Singleton<Logger>::GetInstance()->Stop();    
    sleep(1);
    return 0;

}

/* #include <cassert> // assert
#include <iostream> // cout
#include <unistd.h>

#include "nbd.hpp"
#include "ramstorage.hpp"
#include "driverdata.hpp"
#include "logger.hpp"

using namespace ilrd_hrd33;

int main(int argc, char const *argv[])
{
    assert(2 == argc);

    LOG_SETDEBUG();
    LOG_FILE("logger.txt");

    volatile bool is_nbd_connected = true;
    
    RAM_Storage rs(100000000);
    NBD_Driver_Communicator nbd(100000000, argv[1]);

    while(is_nbd_connected)
    {
        std::shared_ptr<DriverData> request = nbd.ReadRequest();
        
        switch (request->m_type)
        {
        case TYPE::READ:
            rs.Read(request);
            break;

        case TYPE::WRITE:
            rs.Write(request);
            break;
        
        case TYPE::DISC:
            nbd.Disconnect();
            is_nbd_connected = false;
            break;
            
        default:
            break;
        }

        nbd.SendReply(request);
    }

    return 0;
}
 */