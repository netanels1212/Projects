#include <iostream>

#include "example_plugin.hpp"
#include "factory.hpp"
#include "singleton.hpp"
#include "task.hpp"
#include "driverdata.hpp"

namespace ilrd_hrd33
{
    static void __attribute__((constructor)) registerExamplePlugin()
    {
        std::cout << "Registering ExamplePlugin..." << std::endl;
        auto factory = Singleton<Factory<ITask, TYPE, std::shared_ptr<DriverData>>>::GetInstance();
        factory->AddOrReplace(TYPE::DISC, [](std::shared_ptr<DriverData> dr_) -> std::shared_ptr<ITask>
    {
        return std::make_shared<DisconnectTask>();
    });
    }

}
