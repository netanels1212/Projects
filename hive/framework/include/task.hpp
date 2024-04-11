/*****************************************************************************/
/*
Project: Thread Pool
Name: Netanel shmuel
Reviewer: Yan
Date: 19.03.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef TASK_HPP
#define TASK_HPP

namespace ilrd_hrd33
{

// Interface for a Task
//Add private function ptr and priority
class ITask 
{
public:
    ITask() = default;

    virtual ~ITask() = default;

    virtual void Execute() = 0;
};

} //end of namespace ilrd_hrd33

#endif //COMMAND_HPP