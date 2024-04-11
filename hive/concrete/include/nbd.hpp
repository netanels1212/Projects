/*****************************************************************************/
/*
Project: NBD Driver Communicator
Name: Netanel shmuel
Reviewer: ***
Date: 05.03.2024
Version: 1.0 - Initial
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_NBD_Driver_Communicator_HPP
#define ILRD_HRD33_NBD_Driver_Communicator_HPP

#include <thread>

#include "driver.hpp"

namespace ilrd_hrd33
{
class NBD_Driver_Communicator : public IDriverCommunicator
{
public:
    explicit NBD_Driver_Communicator(size_t mem_size, const char *dev_file);
    explicit NBD_Driver_Communicator(size_t block_size, size_t num_of_blocks, const char *dev_file);
    ~NBD_Driver_Communicator() noexcept;
    NBD_Driver_Communicator& operator=(const NBD_Driver_Communicator&) = delete;
    NBD_Driver_Communicator(const NBD_Driver_Communicator&) = delete;
    std::shared_ptr<DriverData> ReadRequest() override;
    void SendReply(std::shared_ptr<DriverData>) override;
    int GetFD() const override;
    void Disconnect() override;  //throws system_error, TODO: change to my exception
private:
    const size_t m_mem_size;
    const size_t m_block_size;
    const size_t m_block_count;
    int m_nbd;
    int m_fds[2];
    std::thread m_thread;
};
}//end of ilrd_hrd33

#endif //ILRD_HRD33_NBD_Driver_Communicator_HPP