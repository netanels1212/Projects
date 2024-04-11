#include <iostream> //cout

#include "ramstorage.hpp"

namespace details
{
    static unsigned char *AllocStorage(size_t memory_size_)
    {
        unsigned char *ret = new unsigned char[memory_size_];

        std::cout << ret[100] << std::endl;

        return ret;
    }
} //end of namespace details

namespace ilrd_hrd33
{
RAM_Storage::RAM_Storage(size_t memory_size_) : m_mem_size(memory_size_),
                                                m_storage(details::AllocStorage(memory_size_)) {}

RAM_Storage::~RAM_Storage() noexcept
{
    delete[] m_storage;
    m_storage = nullptr;
}

void RAM_Storage::Read(std::shared_ptr<DriverData> data_) const
{
    data_->m_buffer.assign(m_storage + data_->m_from, m_storage + data_->m_from + data_->m_len);
}

void RAM_Storage::Write(std::shared_ptr<DriverData> data_)
{
    std::copy(data_->m_buffer.begin(), data_->m_buffer.end(), m_storage + data_->m_from);
}
} //end of namespace ilrd_hrd33
