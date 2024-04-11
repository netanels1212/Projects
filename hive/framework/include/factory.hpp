/*****************************************************************************/
/*
Project: Factory
Name: Netanel shmuel
Reviewer: Shahar
Date: 14.03.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_FACTORY_HPP
#define ILRD_HRD33_FACTORY_HPP

#include <unordered_map> //unordered_map
#include <functional>    //function
#include <string>        //string
#include <memory>        //shared_ptr

namespace ilrd_hrd33
{
class FactoryException : public std::exception
{
public:
    explicit FactoryException(const std::string &msg);
    ~FactoryException() = default;
    FactoryException(const FactoryException &other_) = default;
    FactoryException &operator=(const FactoryException &other_) = default;

    const char *what() const noexcept override;

private:
    std::string m_message;
};

template <typename BASE, typename KEY, typename... ARGS>
class Factory
{
public:
    explicit Factory() = default;
    ~Factory() noexcept = default;
    void AddOrReplace(const KEY &key, std::function<std::shared_ptr<BASE>(ARGS...)> ctor);
    std::shared_ptr<BASE> Create(const KEY &key, ARGS... args); // throws FactoryException

private:
    Factory(const Factory &) = delete;
    Factory(Factory &&) = delete;
    Factory &operator=(const Factory &) = delete;
    Factory &operator=(Factory &&) = delete;
    std::unordered_map<KEY, std::function<std::shared_ptr<BASE>(ARGS...)>> m_ctors;
};

FactoryException::FactoryException(const std::string &msg) : m_message(msg) {}

const char *FactoryException::what() const noexcept
{
    return m_message.c_str();
}

template <typename BASE, typename KEY, typename... ARGS>
void Factory<BASE, KEY, ARGS...>::AddOrReplace(const KEY &key, std::function<std::shared_ptr<BASE>(ARGS...)> ctor)
{
    m_ctors[key] = ctor;
}

template <typename BASE, typename KEY, typename... ARGS>
std::shared_ptr<BASE> Factory<BASE, KEY, ARGS...>::Create(const KEY &key, ARGS... args)
{
    auto it = m_ctors.find(key);
    if (it == m_ctors.end())
    {
        throw FactoryException("No constructor found for this key"); //throw FactoryException("No constructor found for key: " + key);
    }

    return (it->second)(args...);
}
} //end of namespace ilrd_hrd33
#endif // ILRD_HRD33_FACTORY_HPP