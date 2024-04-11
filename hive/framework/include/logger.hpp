/*****************************************************************************/
/*
Project: Logger
Name: HRD33
Reviewer: ***
Date: 17.03.2024
Version: 0.1 - Initial
*/
/*****************************************************************************/

#ifndef ILRD_HRD33_LOGGER_HPP
#define ILRD_HRD33_LOGGER_HPP

#include <fstream>  //ofstream 
#include <thread>   //thread 
#include <string>   //string
#include <iostream> //cerr

#include "singleton.hpp"
#include "waitable_queue.hpp"

namespace ilrd_hrd33
{
class Logger;

#define LOG_I(msg) Singleton<Logger>::GetInstance()->Log(Logger::LogLevel::INFO,msg);
#define LOG_E(msg) Singleton<Logger>::GetInstance()->Log(Logger::LogLevel::ERROR,msg);
#define LOG_W(msg) Singleton<Logger>::GetInstance()->Log(Logger::LogLevel::WARNING,msg);
#define LOG_D(msg) Singleton<Logger>::GetInstance()->Log(Logger::LogLevel::DEBUG,msg); 
#define LOG_I_L(msg,line) Singleton<Logger>::GetInstance()->Log(Logger::LogLevel::INFO,msg,line); 
#define LOG_E_L(msg,line) Singleton<Logger>::GetInstance()->Log(Logger::LogLevel::ERROR,msg,line); 
#define LOG_W_L(msg,line) Singleton<Logger>::GetInstance()->Log(Logger::LogLevel::WARNING,msg,line); 
#define LOG_D_L(msg,line) Singleton<Logger>::GetInstance()->Log(Logger::LogLevel::DEBUG,msg,line); 

#define LOG_SETDEBUG() Singleton<Logger>::GetInstance()->SetLevel(Logger::LogLevel::DEBUG);
#define LOG_SETERROR() Singleton<Logger>::GetInstance()->SetLevel(Logger::LogLevel::ERROR);
#define LOG_SETINFO() Singleton<Logger>::GetInstance()->SetLevel(Logger::LogLevel::INFO);
#define LOG_SETWARNING() Singleton<Logger>::GetInstance()->SetLevel(Logger::LogLevel::WARNING);

#define LOG_CERR()  Singleton<Logger>::GetInstance()->SetOutput(std::cerr);
#define LOG_FILE(f_name)  Singleton<Logger>::GetInstance()->SetOutput(f_name);
#define LOG_STOP() Singleton<Logger>::GetInstance()->Stop();

class LoggerException : public std::exception
{
public:
    LoggerException(const std::string &msg);
    ~LoggerException() = default;
    LoggerException(const LoggerException &other_) = default;
    LoggerException &operator=(const LoggerException &other_) = default;
    const char *what() const noexcept override;

private:
    std::string m_message;
};

class Logger
{

public:
    enum class LogLevel
    {
        DEBUG = 0,
        INFO,
        WARNING,
        ERROR
    };

    
    void Log(LogLevel level, const std::string &msg);
    void Log(LogLevel level, const std::string &msg, size_t line);
    void SetOutput(std::ostream &output_stream = std::cerr); //throws LoggerException
    void SetOutput(const std::string &f_name); //throws LoggerException
    void SetLevel(LogLevel level);
    void Stop(); //throws LoggerException
                //Cannot start logger after it stopped

    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger &operator=(Logger &&) = delete;

private:
    explicit Logger();
    ~Logger() noexcept;
    friend class Singleton<Logger>;

    WaitableQueue<std::queue<std::string>> m_queue;
    void LogFromQueue(); //thread func

    bool m_running;
    std::thread m_thread;
    LogLevel m_level;
    std::string m_f_name;
    std::ofstream m_log_file;
    std::ostream m_buffer;
};
} //end of namespace ilrd_hrd33

#endif //ILRD_HRD33_LOGGER_HPP