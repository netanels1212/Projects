#include "logger.hpp"
#include "singleton.hpp"

using namespace ilrd_hrd33;

int main()
{
    Logger *logger = Singleton<Logger>::GetInstance();

    logger->SetOutput(std::cerr);
    //logger->SetOutput("test_logging.txt"); 
    logger->SetLevel(Logger::LogLevel::DEBUG);
    logger->Log(Logger::LogLevel::INFO,"Hello");
    logger->Log(Logger::LogLevel::DEBUG,"dasdas");
    logger->Log(Logger::LogLevel::WARNING,"Ma kore Kapara");
    logger->Log(Logger::LogLevel::ERROR,"WaitQueue",__LINE__);
    
    logger->Stop();
    return 0;
}