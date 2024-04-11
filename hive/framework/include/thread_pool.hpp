/*****************************************************************************/
/*
Project: Thread Pool
Name: Netanel shmuel
Reviewer: Yan
Date: 19.03.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_THREAD_POOL_HPP
#define ILRD_HRD33_THREAD_POOL_HPP

#include <thread> //thread
#include <functional> //std::function
#include <atomic> //atomic bool
#include <future> //Future, promise

#include "waitable_queue.hpp"
#include "pqueue.hpp"
#include "task.hpp"

namespace ilrd_hrd33
{
struct TaskWithPriority;

class ThreadPool
{
using pqueue = PriorityQueueAdapter<TaskWithPriority>;

public:
    enum class Priority
    {
        LOW = 0,
        MEDIUM,
        HIGH
    };

    explicit ThreadPool(size_t threads_num);
    ~ThreadPool() noexcept;
    void Run();
    void AddTask(std::shared_ptr<ITask> task, Priority prio);
    void SetThreadNum(size_t new_size);
    void Pause();
    void Resume();
    void Stop(std::chrono::milliseconds);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool &operator=(const ThreadPool&) = delete;

private:
    std::vector<std::thread> m_threads;
    WaitableQueue<pqueue> m_tasks_queue;
    std::atomic<bool> m_pause; //TODO: change the name to m_is_pause
    std::atomic<bool> m_stop; //TODO: change the name to m_is_stop
    size_t m_threads_num; //TODO: more decriptive name - m_running_threads
    std::vector<std::promise<void>> m_promises; //TODO: no need it [?]
    std::vector<std::future<void>> m_futures;
    void AddThread(std::promise<void>& joinPromise);
};
} //end of namespace ilrd_hrd33

#endif //ILRD_HRD33_THREAD_POOL_HPP
