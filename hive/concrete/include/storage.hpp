/*****************************************************************************/
/*
Project: Storage
Name: Netanel shmuel
Reviewer: ***
Date: 05.03.2024
Version: 1.0 - Initial
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_STORAGE_HPP
#define ILRD_HRD33_STORAGE_HPP

#include <cstddef> /*size_t*/
#include <memory> /*shared_ptr*/

#include "driverdata.hpp"

namespace ilrd_hrd33
{
class IStorage
{
public:
    explicit IStorage() = default;
    virtual ~IStorage() noexcept = default;
    IStorage& operator=(const IStorage&) = delete;
    IStorage(const IStorage&) = delete;
    virtual void Read(std::shared_ptr<DriverData>) const = 0;
    virtual void Write(std::shared_ptr<DriverData>) = 0;
};
}/*end of ilrd_hrd33*/

#endif /*ILRD_HRD33_STORAGE_HPP*/