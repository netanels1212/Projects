#include <iostream> //std::cerr std:cout std::endl
#include <dlfcn.h>  //dlopen dlerror RTLD_LAZY

#include "dll_loader.hpp"
#include "dir_monitor.hpp"
#include "logger.hpp"

namespace ilrd_hrd33
{
    DllLoader::DllLoader(Dispatcher<std::filesystem::path> *dispatcher_) : m_load_commands(this, &DllLoader::Load)
    {
        dispatcher_->Subscribe(&m_load_commands);
    }

    DllLoader::~DllLoader()noexcept
    {
        size_t size = m_library_handlers.size();
        
        while(size)
        {
            int error = dlclose(m_library_handlers.back());
            LOG_E("Dlclose failed with error number " + std::to_string(error));
            m_library_handlers.pop_back();
            --size;
        }
    }

    void DllLoader::Load(const std::filesystem::path &msg_)
    {
        LOG_I_L(msg_, __LINE__);

        void *handle = dlopen(msg_.c_str(), RTLD_LAZY);
        m_library_handlers.push_back(handle);

        if (!handle)
        {
            std::string str = "Failed to open library: ";
            str += dlerror();

            LOG_E_L(str, __LINE__);

            return;
        }
    }

}
