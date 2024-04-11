/*****************************************************************************/
/*
Project: Singleton
Name: Netanel shmuel
Reviewer: Shahar
Date: 13.03.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_SINGLETON_HPP
#define ILRD_HRD33_SINGLETON_HPP

#include <atomic> //atomic
#include <mutex>  //mutex
#include <cstdlib> // atexit

namespace ilrd_hrd33
{
    template <typename T> /* 1) Must have default ctor on private
                             2) Must not have cctor
                             3) Singleton must be friend
                          */
    class Singleton
    {
    public:
        static T *GetInstance();
    private:
        explicit Singleton() = delete;
        ~Singleton() = delete;
        Singleton(const Singleton<T> &) = delete;
        Singleton(Singleton<T> &&) = delete;
        Singleton &operator=(const Singleton &) = delete;
        Singleton &operator=(Singleton &&) = delete;
        static void DestroyInstance();

        static std::atomic<T *> s_instance;
        static std::mutex s_mutex;
    };

    template <typename T>
    std::atomic<T *> Singleton<T>::s_instance(nullptr);

    template <typename T>
    std::mutex Singleton<T>::s_mutex;

    template <typename T>
    T *Singleton<T>::GetInstance()
    {
        T *temp = s_instance.load(std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_acquire);

        if (temp == nullptr)
        {
            std::lock_guard<std::mutex> lock(s_mutex);
            temp = s_instance.load(std::memory_order_relaxed);

            if (temp == nullptr)
            {
                temp = new T;
                std::atomic_thread_fence(std::memory_order_release);
                s_instance.store(temp, std::memory_order_relaxed);

                atexit(DestroyInstance);
            }
        }

        return temp;
    }

    template<typename T>
    void Singleton<T>::DestroyInstance()
    {
        T *temp = s_instance.load(std::memory_order_relaxed);

        if (temp != nullptr)
        {
            delete temp;
            
            s_instance.store(nullptr, std::memory_order_relaxed);
        }
    }
}

#endif /*ILRD_HRD33_SINGLETON_HPP*/
