/*****************************************************************************/
/*
Project: Driver Communicator
Name: Netanel shmuel
Reviewer: ***
Date: 05.03.2024
Version: 1.0 - Initial
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_DRIVER_HPP
#define ILRD_HRD33_DRIVER_HPP

#include <cstddef> /*size_t*/
#include <memory> /*shared_ptr*/

#include "driverdata.hpp"

namespace ilrd_hrd33
{
class IDriverCommunicator
{
public:
    explicit IDriverCommunicator() = default;
    virtual ~IDriverCommunicator() noexcept = default;
    IDriverCommunicator& operator=(const IDriverCommunicator&) = delete;
    IDriverCommunicator(const IDriverCommunicator&) = delete;
    virtual std::shared_ptr<DriverData> ReadRequest() = 0;
    virtual void SendReply(std::shared_ptr<DriverData>) = 0;
    virtual int GetFD() const = 0;
    virtual void Disconnect() = 0; /*throws system_error, TODO: change to my exception*/
};
}/*end of ilrd_hrd33*/

#endif /*ILRD_HRD33_DRIVER_HPP*/