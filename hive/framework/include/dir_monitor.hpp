/*****************************************************************************/
/*
Project: dir_monitor
Name: HRD33
Reviewer: ***
Date: 1.04.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_DIR_MONITOR_HPP
#define ILRD_HRD33_DIR_MONITOR_HPP

#include <filesystem>    // std::filesystem::path

#include "dispatcher.hpp"
#include "thread_pool.hpp"

namespace ilrd_hrd33
{

class DirMonitor
{
public:
    explicit DirMonitor(const std::filesystem::path &dir_);
    ~DirMonitor() noexcept;
    void RunMonitor();
    Dispatcher<std::filesystem::path> &GetDispatcher();

    DirMonitor(const DirMonitor &other_) = delete;
    DirMonitor &operator=(const DirMonitor &other_) = delete;

private:
    Dispatcher<std::filesystem::path> m_file_change_dispatcher;
    std::filesystem::path m_dir;
    int m_inotify_fd;
    int m_watch_descriptor;
    ThreadPool m_tp;
};

} // end of ilrd_hrd33

#endif // ILRD_HRD33_DIR_MONITOR_HPP
