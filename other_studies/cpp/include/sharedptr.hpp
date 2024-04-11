/*****************************************************************************/
/*
Project: Shared Pointer
Name: Netanel shmuel
Reviewer: ***
Date: 25.02.2024
Version: 1.0 - Initial
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_SHARED_HPP
#define ILRD_HRD33_SHARED_HPP

#include <cstddef>
#include "shared_details.hpp"

namespace ilrd_hrd33
{
template <typename S>
class SharedPointer;

template<typename T> //template expects pointers
class SharedPointer
{
public:
    explicit SharedPointer(T *ptr_ = NULL); 

    template <typename S>
    SharedPointer(const SharedPointer<S>& o_);

    SharedPointer(const SharedPointer &other_);
    SharedPointer &operator=(const SharedPointer &other_);

    T& operator*() const;
    T* operator->() const;
    ~SharedPointer() throw();

private:
    template <typename>
    friend class SharedPointer;

    details::CountedResource<T> *m_cr;
};

using namespace details;

template <typename T>
SharedPointer <T>::SharedPointer(T *ptr_) : m_cr(CountedResource<T>::Create(ptr_))
{
}

template <typename T>
template <typename S>
SharedPointer<T>::SharedPointer(const SharedPointer<S>& o_) : m_cr(o_.m_cr)
{
    m_cr->IncrementCounter();
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer &other_) : m_cr(other_.m_cr)
{
    m_cr->IncrementCounter();
}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer &other_)
{
    other_.m_cr->IncrementCounter(); //avoid self assignment
    m_cr->DecrementCounter();
    m_cr = other_.m_cr;

    return *this;
}

template <typename T>
T& SharedPointer<T>::operator*() const
{
    return *(m_cr->GetPtr());
}

template <typename T>
T* SharedPointer<T>::operator->() const
{
    return m_cr->GetPtr();
}

template <typename T>
SharedPointer<T>::~SharedPointer() throw()
{
    m_cr->DecrementCounter();
}
} // end of ilrd_hrd33

#endif /*ILRD_HRD33_SHARED_HPP*/