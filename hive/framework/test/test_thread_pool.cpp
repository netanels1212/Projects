/* #include <iostream>
#include <thread>
#include <vector>

 #include "thread_pool.hpp"
#include "task.hpp"

using namespace ilrd_hrd33;

// Define a simple task class for testing
class TestTask : public ITask
{
public:
    explicit TestTask(int id) : m_id(id) {}
    ~TestTask() {}

    void Execute() override
    {
        std::cout << "Task " << m_id << " is executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task " << m_id << " finished\n";
    }

private:
    int m_id;
};

int main()
{
    // Create a thread pool with 3 threads
    ThreadPool threadPool(3);

    // Add tasks with different priorities to the thread pool
    for (int i = 0; i < 10; ++i)
    {
        if (i % 2 == 0)
        {
            threadPool.AddTask(std::make_shared<TestTask>(i), ThreadPool::Priority::LOW);
        }
        else
        {
            threadPool.AddTask(std::make_shared<TestTask>(i), ThreadPool::Priority::HIGH);
        }
    }

    // Pause the thread pool momentarily to ensure all tasks are added
    threadPool.Pause();

    threadPool.Run();
    
    // Resume the thread pool to start task execution
    threadPool.Resume();

    // Wait for tasks to complete
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Adjust the sleep duration as needed

    // Stop the thread pool
    threadPool.Stop(std::chrono::seconds(5)); // Set a timeout for stopping threads

    return 0;
} */


/* #include <iostream>
#include "thread_pool.hpp"
#include "task.hpp"
#include <unistd.h>

using namespace ilrd_hrd33;

// Mock Task implementation for testing purposes
class MockTask : public ITask
{
public:
    virtual void Execute() override
    {
        executed = true;
        std::cout << "Here" << std::endl;
    }

    bool executed = false;
};

void Func(ThreadPool *threadPool)
{
    auto task1 = std::make_shared<MockTask>();
    auto task2 = std::make_shared<MockTask>();
    while (true)
    {
        threadPool->AddTask(task1, ThreadPool::Priority::LOW);
        threadPool->AddTask(task2, ThreadPool::Priority::HIGH);
    }
}
int main()
{
    // Test 1: Adding tasks and executing them
    
        ThreadPool threadPool(2);
        threadPool.Run();
        std::thread th(Func, &threadPool);

        threadPool.Pause();
        sleep(2);
        std::cout << "Paused" << std::endl;
        sleep(2);
        std::cout << "Resume" << std::endl;
        threadPool.Resume();
        // Wait for tasks to execute (assuming they execute asynchronously)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        threadPool.Stop(std::chrono::milliseconds(100));

        th.join();
    
    

    return 0;
} */

#include "thread_pool.hpp"
#include "task.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <unistd.h>

using namespace ilrd_hrd33;

// Define a simple task class for testing
class TestTask : public ITask
{
public:
    explicit TestTask(int id) : m_id(id) {}

    void Execute() override
    {
        std::cout << "Task " << m_id << " is executing\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Task " << m_id << " finished\n";
    }

private:
    int m_id;
};

class TestTaskLoop : public ITask
{
public:
    explicit TestTaskLoop(int id) : m_id(id) {}

    void Execute() override
    {
        while (1)
        {
            std::cout << "in loop" << std::endl;
            sleep(1);
        }
        
    }

private:
    int m_id;
};

int main()
{
    ThreadPool threadPool(3);

    for (int i = 0; i < 10; ++i)
    {
        if(i  == 0)
        {
            threadPool.AddTask(std::make_shared<TestTaskLoop>(i), ThreadPool::Priority::HIGH);
        }
        else  
        {
            threadPool.AddTask(std::make_shared<TestTask>(i), ThreadPool::Priority::MEDIUM);
        }
    }

    threadPool.Run();

    //std::this_thread::sleep_for(std::chrono::seconds(2));

    threadPool.SetThreadNum(1);

    //std::cout << "here" << std::endl;

    for (int i = 10; i < 15; ++i)
    {
        threadPool.AddTask(std::make_shared<TestTask>(i), ThreadPool::Priority::LOW);
    }
    sleep(2);
    threadPool.SetThreadNum(3);
    for (int i = 15; i < 20; ++i)
    {
        threadPool.AddTask(std::make_shared<TestTask>(i), ThreadPool::Priority::LOW);
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));

    threadPool.Stop(std::chrono::seconds(3));

    return 0;
}