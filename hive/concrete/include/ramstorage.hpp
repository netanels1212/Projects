/*****************************************************************************/
/*
Project: RAM Storage
Name: Netanel shmuel
Reviewer: ***
Date: 05.03.2024
Version: 0.1 - Initial
         0.2 - Modified members and Constructor
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_RAMSTORAGE_HPP
#define ILRD_HRD33_RAMSTORAGE_HPP

#include <memory> //shared_ptr

#include "storage.hpp"

namespace ilrd_hrd33
{
class RAM_Storage : public IStorage
{
public:
    explicit RAM_Storage(size_t mem_size_);
    ~RAM_Storage() noexcept;
    RAM_Storage& operator=(const RAM_Storage&) = delete;
    RAM_Storage(const RAM_Storage&) = delete;
    void Read(std::shared_ptr<DriverData>) const override;
    void Write(std::shared_ptr<DriverData>) override;
private:
    const size_t m_mem_size;
    unsigned char* m_storage;
};
} //end of ilrd_hrd33

#endif //ILRD_HRD33_RAMSTORAGE_HPP