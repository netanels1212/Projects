/*****************************************************************************/
/*
Project: Driver Data
Name: Netanel shmuel
Reviewer: ***
Date: 05.03.2024
Version: 1.0 - Initial
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_DRIVERDATA_HPP
#define ILRD_HRD33_DRIVERDATA_HPP

#include <cstddef> // size_t

#include <vector>

namespace ilrd_hrd33
{
enum class TYPE{READ, WRITE, DISC, FLUSH, TRIM};
enum class ERROR{SUCCESS = 0, ERROR};

struct DriverData
{
    DriverData() = default;
    ~DriverData() = default;
    DriverData& operator=(const DriverData&) = delete;
    DriverData(const DriverData&) = delete;
    size_t m_len;
    size_t m_from;
    char m_handle[8];
    std::vector<unsigned char> m_buffer;
    TYPE m_type;
    ERROR m_error;
};
} //end of namespace ilrd_hrd33
#endif //ILRD_HRD33_DRIVERDATA_HPP