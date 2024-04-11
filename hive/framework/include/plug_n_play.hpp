/*****************************************************************************/
/*
Project: PlugNPlay
Name: Netanel shmuel
Reviewer: ***
Date: 3.04.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_PLUG_N_PLAY_HPP
#define ILRD_HRD33_PLUG_N_PLAY_HPP

#include "dir_monitor.hpp"
#include "dll_loader.hpp"

namespace ilrd_hrd33
{
class PlugNPlay
{
    public:
        explicit PlugNPlay(const std::filesystem::path &dir_);
        ~PlugNPlay() = default;

        PlugNPlay(const PlugNPlay &other_) = delete;
        PlugNPlay& operator=(const PlugNPlay &other_) = delete;
    private:
        DirMonitor m_monitor;
        DllLoader m_loader;
};

} // end of ilrd_hrd33

#endif // ILRD_HRD33_PLUG_N_PLAY_HPP

