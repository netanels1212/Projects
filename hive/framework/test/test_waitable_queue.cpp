#include <iostream> //cout
#include <thread> //thread

#include "waitable_queue.hpp"

using namespace ilrd_hrd33;

void ProducerThread(WaitableQueue<std::deque<int>>& queue)
{
    for (int i = 0; i < 10; ++i) 
    {
        queue.Push(i + 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void ConsumerThread(WaitableQueue<std::deque<int>>& queue)
{
    for (int i = 0; i < 10; ++i) 
    {
        int msg;

        queue.Pop(&msg);
        std::cout << "Received: " << msg << std::endl;
    }
}

int main()
{
    WaitableQueue<std::deque<int>> queue;

    // Producer thread
    std::thread producer(ProducerThread, std::ref(queue));

    // Consumer thread
    std::thread consumer(ConsumerThread, std::ref(queue));

    producer.join();
    consumer.join();

    std::cout << "Test for IsEmpty: " << queue.IsEmpty() << std::endl;

    // Test Pop with milliseconds
    int msg;

    ilrd_hrd33::WaitableQueue<std::deque<int>>::PopStatus status = queue.Pop(&msg, std::chrono::milliseconds(1000));
    if (status == ilrd_hrd33::WaitableQueue<std::deque<int>>::PopStatus::SUCCESS)
    {
        std::cout << "Pop successful after 1 second. Received: " << msg << std::endl;
    } 
    
    else
    {
        std::cout << "Pop timed out after 1 second." << std::endl;
    }

    return 0;
}