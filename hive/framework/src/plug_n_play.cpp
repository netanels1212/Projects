#include "plug_n_play.hpp"

namespace ilrd_hrd33
{
PlugNPlay::PlugNPlay(const std::filesystem::path &dir_):m_monitor(DirMonitor(dir_)), 
                                                        m_loader(&m_monitor.GetDispatcher())
{
    m_monitor.RunMonitor();
}
}

