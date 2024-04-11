/*****************************************************************************/
/*
Project: Counted Resource
Name: Netanel shmuel
Reviewer: ***
Date: 25.02.2024
Version: 1.0 - Initial
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_DETAILS_HPP
#define ILRD_HRD33_DETAILS_HPP

#include <cstddef>
#include "utils_cpp.hpp"

namespace details
{
template <typename T>
class CountedResource
{
public:
    static CountedResource* Create(T* t_);
    T* GetPtr();
    void IncrementCounter();
    void DecrementCounter();
private:
    explicit CountedResource(T *ptr_);
    ~CountedResource() NOEXCEPT;
    size_t m_counter;
    T* m_ptr;

    CountedResource(CountedResource &other_);//Do not implement
    CountedResource &operator=(const CountedResource &other_); //Do not implement
};

template <typename T>
CountedResource<T>* CountedResource<T>::Create(T* t_)
{
    return new CountedResource(t_);
}

template <typename T>
T* CountedResource<T>::GetPtr()
{
    return m_ptr;
}

template <typename T>
void CountedResource<T>::IncrementCounter()
{
    ++m_counter;
}

template <typename T>
void CountedResource<T>::DecrementCounter()
{
    --m_counter;

    if(0 == m_counter)
    {
        delete(m_ptr);
        delete this;
    }
}

template <typename T>
CountedResource<T>::CountedResource(T *ptr_) : m_counter(1), m_ptr(ptr_)
{
}

template <typename T>
CountedResource<T>::~CountedResource() NOEXCEPT
{
}
}//end of details

#endif //ILRD_HRD33_DETAILS_HPP
