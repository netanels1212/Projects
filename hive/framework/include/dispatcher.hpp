/*****************************************************************************/
/*
Project: Dispatcher
Name: Netanel shmuel
Reviewer: Yan
Date: 24.03.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_DISPATCHER_HPP
#define ILRD_HRD33_DISPATCHER_HPP

#include <cassert>        //assert
#include <unordered_set> //unordered_set
#include <functional>    //function

namespace ilrd_hrd33
{

template <typename MSG>
class Dispatcher;

template <typename MSG> //Should be copyable
class CallBackBase
{
public:
    explicit CallBackBase();
    virtual ~CallBackBase() noexcept;
    CallBackBase(const CallBackBase &) = delete;
    CallBackBase &operator=(const CallBackBase &) = delete;

    void SetDispatcher(Dispatcher<MSG> *dispatcher_);

private:
    virtual void Notify(const MSG &msg_) = 0;
    virtual void NotifyDeathExt();
    void NotifyDeath();
    friend Dispatcher<MSG>;
    Dispatcher<MSG> *m_dispatcher;
};

template <typename MSG, class SUB>
class CallBack : public CallBackBase<MSG>
{
public:
    typedef void (SUB::*NotifyPtr)(const MSG& msg_);
    typedef void (SUB::*DeathPtr)(void);

    explicit CallBack(SUB *instance_, NotifyPtr notify_func, DeathPtr notify_death_func = nullptr);
    virtual ~CallBack() noexcept = default;
    CallBack(const CallBack &) = delete;
    CallBack &operator=(const CallBack &) = delete;

private:
    void NotifyDeathExt() override;
    void Notify(const MSG &msg_) override;
    NotifyPtr m_notify;
    DeathPtr m_notify_death_func;
    SUB *m_instance;
};

template <typename MSG>
class Dispatcher
{
public:
    Dispatcher() = default;
    ~Dispatcher() noexcept;
    Dispatcher(const Dispatcher &) = delete;
    Dispatcher &operator=(const Dispatcher &) = delete;

    void Subscribe(CallBackBase<MSG> *call_);
    void NotifyAll(const MSG &msg_);

private:
    std::unordered_set<CallBackBase<MSG> *> m_callbacks;
    void Unsubscribe(CallBackBase<MSG> *call_);
    friend CallBackBase<MSG>;
};

template <typename MSG>
CallBackBase<MSG>::CallBackBase() : m_dispatcher(nullptr){}

template <typename MSG>
CallBackBase<MSG>::~CallBackBase() noexcept
{
    if(m_dispatcher != nullptr)
    {
        m_dispatcher->Unsubscribe(this);
    }
}

template <typename MSG>
void CallBackBase<MSG>::SetDispatcher(Dispatcher<MSG> *dispatcher_)
{
    assert(nullptr != dispatcher_); //TODO: remove this line [?]

    m_dispatcher = dispatcher_;
}

template <typename MSG>
void CallBackBase<MSG>::NotifyDeathExt(){}

template <typename MSG>
void CallBackBase<MSG>::NotifyDeath()
{
    m_dispatcher = nullptr; //TODO: use SetDispatcher, and send her nullptr
    NotifyDeathExt();
}

template <typename MSG, class SUB>
CallBack<MSG,SUB>::CallBack(SUB *instance_, NotifyPtr notify_func,
                            DeathPtr notify_death_func) : m_instance(instance_),
                                                          m_notify(notify_func),
                                                          m_notify_death_func(notify_death_func)
{
    assert(nullptr != instance_);
    assert(nullptr != notify_func);
}

template <typename MSG, class SUB>
void  CallBack<MSG,SUB>::NotifyDeathExt()
{
    if (nullptr !=  m_notify_death_func)
    {
        (m_instance->*m_notify_death_func)();
    }
}

template <typename MSG, class SUB>
void CallBack<MSG,SUB>::Notify(const MSG &msg_) 
{
    if (nullptr != m_notify) //TODO: no need to check it, it not be nullptr because we check it in the Ctor [?]
    {
        (m_instance->*m_notify)(msg_);
    }
}

template <typename MSG>
Dispatcher<MSG>::~Dispatcher() noexcept
{
    for (auto callback : m_callbacks)
    {
        callback->NotifyDeath(); //TODO: if you do unsubscribe it will be invalid iterator [?] - copy it, and operate on the copy. or add boolean value for each callback to represent his existing
    }
}

template <typename MSG>
void Dispatcher<MSG>::Subscribe(CallBackBase<MSG> *call_)
{
    assert(nullptr != call_);

    m_callbacks.insert(call_);
    call_->SetDispatcher(this);
}

template <typename MSG>
void Dispatcher<MSG>::NotifyAll(const MSG &msg_)
{
    for(auto callback : m_callbacks)
    {
        callback->Notify(msg_); //TODO: it maybe do unsubscribe and it will cause invalid iterator [?]
    }
}

template <typename MSG>
void Dispatcher<MSG>::Unsubscribe(CallBackBase<MSG> *call_)
{
    assert(nullptr != call_);

    m_callbacks.erase(call_); //TODO: it maybe do unsubscribe and it will cause invalid iterator [?]
    //TODO: add SetDispatcher(nullptr)
}
} // end of namespace ilrd_hrd33
#endif