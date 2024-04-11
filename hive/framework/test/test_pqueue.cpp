#include <iostream>
#include "pqueue.hpp"

using namespace ilrd_hrd33;

int main()
{
    PriorityQueueAdapter<int> pq;

    // Push some elements into the priority queue
    pq.push(10);
    pq.push(30);
    pq.push(20);
    pq.push(5);
    pq.push(5);
    pq.push(40);

    std::cout << "Priority Queue size: " << pq.size() << std::endl;

    // Print elements from the priority queue
    std::cout << "Priority Queue elements: ";
    while (!pq.empty())
    {
        std::cout << pq.front() << " ";
        pq.pop();
    }
    std::cout << std::endl;

    return 0;
}