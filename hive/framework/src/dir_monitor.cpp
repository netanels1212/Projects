#include <sys/inotify.h> //inotify
#include <unistd.h>      //close
#include <iostream>      //iostuff

#include "dir_monitor.hpp"
#include "task.hpp"
#include "logger.hpp"

namespace ilrd_hrd33
{
    void ProcessEvents(char *buffer, ssize_t bytesRead, Dispatcher<std::filesystem::path> &dispatcher, std::filesystem::path file_path);

class RunMonitorTask: public ITask
{
    public:
        explicit RunMonitorTask(int inotify_fd_, Dispatcher<std::filesystem::path> &dispatcher_,
        std::filesystem::path dir_):m_inotify_fd(inotify_fd_), m_dispatcher(dispatcher_), m_dir(dir_){}

        ~RunMonitorTask()noexcept = default;
        
        void Execute() override
        {
            const size_t bufferSize = BUFSIZ;
            char buffer[bufferSize];

            while (true) 
            {
                ssize_t bytesRead = read(m_inotify_fd, buffer, bufferSize);
                if (bytesRead == -1) 
                {
                    throw std::runtime_error("Error reading from inotify fd");
                }

                ProcessEvents(buffer, bytesRead, m_dispatcher, m_dir);
            }
        }

    private:
        int m_inotify_fd;
        Dispatcher<std::filesystem::path> &m_dispatcher;
        std::filesystem::path m_dir;
};

    DirMonitor::DirMonitor(const std::filesystem::path &dir_) : m_dir(dir_), m_tp(1)
    {
        m_tp.Run();
        m_inotify_fd = inotify_init();
        if (-1 == m_inotify_fd)
        {
            LOG_E_L("Failed to initialize inotify",__LINE__);
            throw std::runtime_error("Failed to initialize inotify");
        }
        m_watch_descriptor = inotify_add_watch(m_inotify_fd, dir_.c_str(), IN_CLOSE_WRITE| IN_MOVED_TO);
        if (-1 == m_watch_descriptor)
        {
            LOG_E_L("Failed to add watch",__LINE__);
            close(m_inotify_fd);
            throw std::runtime_error("Failed to add watch");
        }
    }

    DirMonitor::~DirMonitor() noexcept
    {
        m_tp.Stop(std::chrono::milliseconds(10));

        try
        {
            if (-1 != m_inotify_fd)
            {
                inotify_rm_watch(m_inotify_fd, m_watch_descriptor);
            }
            if (-1 != m_inotify_fd)
            {
                close(m_inotify_fd);
            }
        }
        catch (const std::exception &e)
        {
            LOG_E(e.what());
        }
    }

    void DirMonitor::RunMonitor()
    {
        m_tp.AddTask(std::make_shared<RunMonitorTask>(m_inotify_fd, m_file_change_dispatcher, m_dir), ThreadPool::Priority::HIGH);
    }

    Dispatcher<std::filesystem::path> &DirMonitor::GetDispatcher()
    {
        return m_file_change_dispatcher;
    }

void ProcessEvents(char* buffer, ssize_t bytesRead, Dispatcher<std::filesystem::path> &dispatcher, std::filesystem::path file_path) 
{
    char* ptr = buffer;
    while (ptr < buffer + bytesRead) 
    {
        struct inotify_event* event = reinterpret_cast<struct inotify_event*>(ptr);
        if (event->len > 0) 
        {
            std::string filename = event->name;

            if (filename != "." && filename != ".." && filename.find(".swp") == std::string::npos) 
            {
                file_path = file_path / filename;

                if (event->mask & IN_CLOSE_WRITE ||event->mask & IN_MOVED_TO) 
                {
                    dispatcher.NotifyAll(file_path);
                }
            }
        }
        ptr += sizeof(struct inotify_event) + event->len;
    }
}

}
