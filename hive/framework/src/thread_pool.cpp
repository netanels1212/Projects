#include <future> //promise, future

#include "thread_pool.hpp"
#include "task.hpp"

namespace ilrd_hrd33
{
const int MAX_PRIO = static_cast<int>(ThreadPool::Priority::HIGH) + 1; //TODO: if we need this line - do m_priority as int and avoid cast
class Semaphore //TODO: do it in other .hpp file (it can be reuseable in other projects)
{
public:
    explicit Semaphore(int count = 0) : m_count(count) {} //TODO: not inline (in all functions)

    void Post() 
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        ++m_count;
        m_condition.notify_one();
    }

    void Wait() 
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this] { return m_count > 0; });
        --m_count;
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    int m_count; //TODO: do it atomic
};

class PauseTask : public ITask 
{
public:
    explicit PauseTask() = default;
    virtual void Execute() override;

    static Semaphore m_semaphore; //TODO: not static (because if we want multiple thread pools), do it as a member in the thread pool
};

void PauseTask::Execute()
{
    m_semaphore.Wait();
}

Semaphore PauseTask::m_semaphore;

class EndTask : public ITask 
{
public:
    explicit EndTask() = default;
    virtual void Execute() override {}
};


struct TaskWithPriority 
{
public:
    TaskWithPriority() {}
    TaskWithPriority(std::shared_ptr<ITask> task_, ThreadPool::Priority m_priority_);

    bool operator<(const TaskWithPriority& other) const;

    std::shared_ptr<ITask> GetTask() const; //TODO: called it Execute()

private:
    std::shared_ptr<ITask> m_task;
    ThreadPool::Priority m_priority; 
};

TaskWithPriority::TaskWithPriority(std::shared_ptr<ITask> task_, ThreadPool::Priority m_priority_) : m_task(task_), m_priority(m_priority_) {}

bool TaskWithPriority::operator<(const TaskWithPriority& other) const
{
    return (static_cast<int>(m_priority) < static_cast<int>(other.m_priority));
}

std::shared_ptr<ITask> TaskWithPriority::GetTask() const
{
    return m_task;
}
//TODO: use hardware_concurrency to know how much threads to create (is ahe return 0 - throw an execption)   [?]
ThreadPool::ThreadPool(size_t threads_num) : m_threads_num(threads_num), m_pause(false), m_stop(false) {}

ThreadPool::~ThreadPool() noexcept
{
    if(!m_stop)
    {
        Stop(std::chrono::milliseconds(10));
    }
}

void ThreadPool::Run()
{
    for (size_t i = 0; i < m_threads_num; ++i) 
    {
        m_promises.emplace_back();
        m_futures.emplace_back(m_promises.back().get_future());

        //TODO: try to do it with lambda
        m_threads.emplace_back(std::bind(&ThreadPool::AddThread, this, std::move(m_promises.back())));
    }
}

void ThreadPool::AddThread(std::promise<void>& joinPromise)
{
    TaskWithPriority task; //TODO: need to be noexcept [?]

    while (!m_stop)
    {
        if(!m_pause)
        {
            m_tasks_queue.Pop(&task);
            task.GetTask()->Execute(); 
        }
    }

    joinPromise.set_value();
}

void ThreadPool::AddTask(std::shared_ptr<ITask> task, Priority prio)
{
    m_tasks_queue.Push(TaskWithPriority(task, prio));
}

void ThreadPool::SetThreadNum(size_t new_size)
{
    size_t vector_size = m_threads.size();
    
    if(new_size < m_threads_num)
    {
        for(size_t i = 0; i < vector_size - new_size; ++i)
        {
            auto task = std::make_shared<PauseTask>();
            AddTask(task, static_cast<ThreadPool::Priority>(4));
        }
    }
    else
    {
        for(size_t i = m_threads_num; i < new_size && i < vector_size; ++i)
        {
            PauseTask::m_semaphore.Post();
        }

        for(int i = new_size - vector_size; i > 0; --i)
        {
            m_promises.emplace_back(); //TODO: it need to be reuseable in other function. and there is a bug?
            m_futures.emplace_back(m_promises.back().get_future());
            m_threads.emplace_back(std::bind(&ThreadPool::AddThread, this, std::move(m_promises.back())));
        }
    }
    m_threads_num = new_size;
}

void ThreadPool::Pause()
{
    m_pause.store(true);
}

void ThreadPool::Resume()
{
    m_pause.store(false);
    Run(); //TODO: need it?
}

void ThreadPool::Stop(std::chrono::milliseconds timeout)
{
    m_stop.store(true);

    for (size_t i = m_threads_num; i < m_threads.size(); ++i)
    {
        PauseTask::m_semaphore.Post();
    }

    for (size_t i = 0; i < m_threads.size(); ++i)
    {
        auto task = std::make_shared<EndTask>();
        AddTask(task, static_cast<ThreadPool::Priority>(MAX_PRIO));
    }

    auto start = std::chrono::steady_clock::now();
    std::chrono::milliseconds remainingTime = std::chrono::milliseconds(timeout);

    for (size_t i = 0; i < m_threads.size(); ++i)
    {

        auto &thread = m_threads[i];
        auto &future = m_futures[i];

        if (thread.joinable())
        {
            auto status = future.wait_for(std::chrono::milliseconds(remainingTime));
            auto elapsed = std::chrono::steady_clock::now() - start;
            remainingTime = timeout > elapsed ? timeout - std::chrono::duration_cast<std::chrono::milliseconds>(elapsed) : std::chrono::milliseconds(0);

            if (status == std::future_status::timeout)
            {
                thread.detach();
                //TODO: use the Logger to log the execution
            }

            else
            {
                thread.join();
            }
        }
    }
}

} //end of namespace ilrd_hrd33