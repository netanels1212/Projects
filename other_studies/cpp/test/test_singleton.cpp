#include <iostream>
#include <thread>
#include <vector>

#include "singleton.hpp"

class MyClass 
{
public:
    friend class ilrd_hrd33::Singleton<MyClass>;
    
private:
    MyClass() 
    {
        std::cout << "MyClass instance created" << std::endl;
    }
    ~MyClass() 
    {
        std::cout << "MyClass instance destroyed" << std::endl;
    }
};

void createInstances() 
{
    auto instance = ilrd_hrd33::Singleton<MyClass>::GetInstance();
    // Perform operations with the instance if needed
}

int main() 
{
    constexpr int num_threads = 5;
    std::vector<std::thread> threads;

    // Create threads
    for (int i = 0; i < num_threads; ++i) 
    {
        threads.emplace_back(createInstances);
    }

    // Join threads
    for (auto& thread : threads) 
    {
        thread.join();
    }

    return 0;
}
