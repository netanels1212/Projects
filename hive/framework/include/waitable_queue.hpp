/*****************************************************************************/
/*
Project: Waitable queue
Name: Netanel shmuel
Reviewer: Yan
Date: 17.03.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_WAITABLE_QUEUE_HPP
#define ILRD_HRD33_WAITABLE_QUEUE_HPP

#include <queue> //queue
#include <mutex> //mutex
#include <condition_variable> //condition_variable

namespace ilrd_hrd33
{
template <typename CONTAINER>/* 1) CONTAINER MUST SUPPORT THOSE OPERATIONS:
                                    front()
                                    push()
                                    pop() 
                                    
                                2) need to be resizeable
                                3) container should have value_type */
class WaitableQueue //MessageType needs to be copyable and assignable
{
public:
    enum class PopStatus
    {
        SUCCESS = 0,
        TIMEOUT
    };

    using MessageType = typename CONTAINER::value_type;

    explicit WaitableQueue() = default;
    ~WaitableQueue() noexcept = default;
    void Push(const MessageType& msg_);
    void Pop(MessageType* msg_);
    PopStatus Pop(MessageType* msg_, std::chrono::milliseconds mili_sec_); //TODO: if the user want to input seconds, he need to enter big number, so do it Template function, and he dont need al the trmplate type generations
    bool IsEmpty() const;

    WaitableQueue(const WaitableQueue&) = delete;
    WaitableQueue(WaitableQueue&&) = delete; //TODO: allow this
    WaitableQueue& operator=(const WaitableQueue&) = delete;
    WaitableQueue& operator=(WaitableQueue&&) = delete; //TODO: allow this

private:
    CONTAINER m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_condition;
};

template <typename CONTAINER>
void WaitableQueue<CONTAINER>::Push(const MessageType& msg_)
{
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(msg_);
    }

    m_condition.notify_one();
}

template <typename CONTAINER>
void WaitableQueue<CONTAINER>::Pop(MessageType* msg_)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    while (m_queue.empty())
    {
        m_condition.wait(lock);
    }

    *msg_ = m_queue.front(); //TODO: check if the assignment failed - if failed don't do pop_front
    m_queue.pop();
}

template <typename CONTAINER>
typename WaitableQueue<CONTAINER>::PopStatus WaitableQueue<CONTAINER>::Pop(MessageType* msg_, std::chrono::milliseconds mili_sec_)
{
    std::unique_lock<std::mutex> lock(m_mutex); //TODO: do lock to specific time and then move on. and in the wait_for calc the new time and wait this time

    if (m_queue.empty())
    {
        if (m_condition.wait_for(lock, mili_sec_) == std::cv_status::timeout)
        {
            return WaitableQueue<CONTAINER>::PopStatus::TIMEOUT;
        }
    }

    *msg_ = m_queue.front();
    m_queue.pop();

    return WaitableQueue<CONTAINER>::PopStatus::SUCCESS;
}

template <typename CONTAINER>
bool WaitableQueue<CONTAINER>::IsEmpty() const
{
    std::unique_lock<std::mutex> lock(m_mutex);
    
    return m_queue.empty();
}
} //end of namespace ilrd_hrd33

#endif //ILRD_HRD33_WAITABLE_QUEUE_HPP