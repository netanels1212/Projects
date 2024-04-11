/*****************************************************************************/
/*
Project: dir_monitor
Name: HRD33
Reviewer: ***
Date: 2.04.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_DLL_LOADER_HPP
#define ILRD_HRD33_DLL_LOADER_HPP

#include <filesystem> // std::filesystem::path

#include "dispatcher.hpp"
#include "dir_monitor.hpp"

namespace ilrd_hrd33
{
    class DllLoader
    {

    public:
        explicit DllLoader(Dispatcher<std::filesystem::path> *dispatcher_);
        ~DllLoader() noexcept;

        void Load(const std::filesystem::path &msg_);

        DllLoader(const DllLoader &other_) = delete;
        DllLoader &operator=(const DllLoader &other_) = delete;

    private:
        CallBack<std::filesystem::path, DllLoader> m_load_commands;
        std::vector<void *> m_library_handlers;
    }; // end of ilrd_hrd33
}
#endif // ILRD_HRD33_DLL_LOADER_HPP