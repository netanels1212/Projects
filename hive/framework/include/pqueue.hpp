/*****************************************************************************/
/*
Project: Priority queue adapter
Name: Netanel shmuel
Reviewer: ***
Date: 19.03.2024
Version: 0.2 - Private inheritance
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_PRIORITY_QUEUE_HPP
#define ILRD_HRD33_PRIORITY_QUEUE_HPP

#include <queue> //priority_queue

namespace ilrd_hrd33
{
template <typename T, typename CONTAINER = std::vector<T>, typename COMPARE = std::less<typename CONTAINER::value_type>>
class PriorityQueueAdapter : private std::priority_queue<T, CONTAINER, COMPARE>
{
public:
    explicit PriorityQueueAdapter() = default;
    ~PriorityQueueAdapter() = default;

    using std::priority_queue<T, CONTAINER, COMPARE>::value_type;
    using std::priority_queue<T, CONTAINER, COMPARE>::empty;
    using std::priority_queue<T, CONTAINER, COMPARE>::push;
    using std::priority_queue<T, CONTAINER, COMPARE>::pop;
    using std::priority_queue<T, CONTAINER, COMPARE>::size;

    const T& front() const;
};

template <typename T, typename CONTAINER, typename COMPARE>
const T& PriorityQueueAdapter<T, CONTAINER, COMPARE>::front() const
{
    return this->top();
}

} //end of namespace ilrd_hrd33


#endif //ILRD_HRD33_PRIORITY_QUEUE_HPP