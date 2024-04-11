/*****************************************************************************/
/*
Project: RequestEngine
Name: Netanel shmuel
Reviewer: ***
Date: 27.03.2024
Version: 0.1
*/
/*****************************************************************************/
#ifndef ILRD_HRD33_REQUESTENGINE_HPP
#define ILRD_HRD33_REQUESTENGINE_HPP

#include <sys/epoll.h> // epoll
#include <filesystem>  // filesystem
#include <functional>
#include <vector>
#include <atomic>
#include <unistd.h>
#include <signal.h>

#include "logger.hpp"
#include "thread_pool.hpp"
#include "factory.hpp"
#include "driver.hpp"
#include "storage.hpp"
#include "driverdata.hpp"

namespace ilrd_hrd33
{

    template <typename KEY, typename ARGS>
    class IGateway
    {
    public:
        explicit IGateway() = default;
        virtual ~IGateway() noexcept = default;
        virtual int GetFd() const = 0;
        virtual std::pair<KEY, ARGS> Read() = 0;
    };

    template <typename KEY, typename ARGS>
    class RequestEngine
    {
    public:
        explicit RequestEngine(const std::filesystem::path &plug_dir_path_, size_t thread_count_);
        ~RequestEngine() noexcept;
        void ConfigTask(std::function<std::shared_ptr<ITask>(ARGS)>, const KEY &key_);
        void AddGateWay(std::shared_ptr<IGateway<KEY, ARGS>> gateway_);
        void Run(); // blocking
        void Stop();
        void SetThreads(size_t thread_num_);
        void ConfigLogger(const std::filesystem::path &log_path_, Logger::LogLevel sev);

        RequestEngine(const RequestEngine &frame_) = delete;
        RequestEngine &operator=(const RequestEngine &frame_) = delete;

    private:
        Factory<ITask, KEY, ARGS> m_factory;
        std::vector<std::shared_ptr<IGateway<KEY, ARGS>>> m_gateways;
        ThreadPool m_tp;
        std::atomic<bool> m_stop;
    };

    template <typename KEY, typename ARGS>
    RequestEngine<KEY, ARGS>::RequestEngine(const std::filesystem::path &plug_dir_path_, size_t thread_count_) : m_tp(thread_count_), m_stop(false) {}

    template <typename KEY, typename ARGS>
    RequestEngine<KEY, ARGS>::~RequestEngine() noexcept {}

    template <typename KEY, typename ARGS>
    void RequestEngine<KEY, ARGS>::ConfigTask(std::function<std::shared_ptr<ITask>(ARGS)> task_, const KEY &key_)
    {
        m_factory.AddOrReplace(key_, task_);
    }

    template <typename KEY, typename ARGS>
    void RequestEngine<KEY, ARGS>::AddGateWay(std::shared_ptr<IGateway<KEY, ARGS>> gateway_)
    {
        m_gateways.push_back(gateway_);
    }
    template <typename KEY, typename ARGS>
    void RequestEngine<KEY, ARGS>::Run()
    {
        int epoll_fd = epoll_create1(0);

        for (auto gateway : m_gateways)
        {
            int fd = gateway->GetFd();
            struct epoll_event event;
            event.events = EPOLLIN;
            event.data.fd = fd;
            if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1)
            {
                LOG_E("epoll addinf fd failed");
                return;
            }
        }

        struct epoll_event events[m_gateways.size()];

        m_tp.Run();

        while (!m_stop)
        {
            int num_events = epoll_wait(epoll_fd, events, m_gateways.size(), -1);
            if (num_events == -1 && errno != EINTR)
            {
                
                LOG_D("Error in epoll");
                
                break;
            }
            else
            {
                if (errno == EINTR)
                {
                    if (signal(SIGINT, SIG_IGN))
                    {
                        LOG_D("Disconnect");
                    }
                }
                for (int i = 0; i < num_events; ++i)
                {
                    if (events[i].events & EPOLLIN)
                    {
                        int fd = events[i].data.fd;
                        for (auto gateway : m_gateways)
                        {
                            if (fd == gateway->GetFd())
                            {
                                std::pair<KEY, ARGS> pair = gateway->Read();
                                std::shared_ptr<ITask> task = m_factory.Create(pair.first, pair.second);
                                m_tp.AddTask(task, ThreadPool::Priority::HIGH);
                                break;
                            }
                        }
                    }
                }
            }
        }

        close(epoll_fd);
    }

    template <typename KEY, typename ARGS>
    void RequestEngine<KEY, ARGS>::Stop()
    {
        m_stop.store(true);
        m_tp.Stop(std::chrono::milliseconds(10));
    }

    template <typename KEY, typename ARGS>
    void RequestEngine<KEY, ARGS>::SetThreads(size_t thread_num_)
    {
        m_tp.SetThreadNum(thread_num_);
    }

    template <typename KEY, typename ARGS>
    void RequestEngine<KEY, ARGS>::ConfigLogger(const std::filesystem::path &log_path_, Logger::LogLevel sev)
    {
        LOG_FILE(log_path_);
        Singleton<Logger>::GetInstance()->SetLevel(sev);
    }

} // end of namespace ilrd_hrd33

#endif //end of ILRD_HRD33_REQUESTENGINE_HPP
