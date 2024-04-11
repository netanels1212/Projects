#include <iostream>

#include "dispatcher.hpp"

using namespace ilrd_hrd33;

struct Message 
{
    int data;
};

class Subscriber 
{
public:
    void handleMessage(const Message &msg) 
    {
        std::cout << "Received message with data: " << msg.data << std::endl;
    }

    void handleDeath() 
    {
        std::cout << "Subscriber is unsubscribing" << std::endl;
    }
};

int main() 
{
    Subscriber subscriber1, subscriber2, subscriber3;

    CallBack<Message, Subscriber> callback1(&subscriber1, &Subscriber::handleMessage, &Subscriber::handleDeath);
    CallBack<Message, Subscriber> callback2(&subscriber2, &Subscriber::handleMessage, &Subscriber::handleDeath);
    Dispatcher<Message> dispatcher;
    Message msg;
    {
        CallBack<Message, Subscriber> callback3(&subscriber3, &Subscriber::handleMessage, &Subscriber::handleDeath);

        dispatcher.Subscribe(&callback1);
        dispatcher.Subscribe(&callback2);
        dispatcher.Subscribe(&callback3);

        msg.data = 42;

        dispatcher.NotifyAll(msg);

        msg.data = 100;
    }
    dispatcher.NotifyAll(msg);
    return 0;
}
