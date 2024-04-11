#include <chrono> //chrono_literals
#include <ctime> //localtime, time
#include <iomanip> //put_time
#include <sstream> //stringstream

#include "logger.hpp"

namespace ilrd_hrd33
{
LoggerException::LoggerException(const std::string &msg_) : m_message(msg_) {}

const char *LoggerException::what() const noexcept
{
    return m_message.c_str();
}

Logger::Logger(): m_running(true), m_level(LogLevel::DEBUG), m_buffer(nullptr)
{
    m_thread = std::thread(&Logger::LogFromQueue, this);
}

Logger::~Logger() noexcept
{
    Stop();

    if(m_thread.joinable())
    {
        m_thread.join();
    }
}

void Logger::Log(LogLevel level, const std::string &msg)
{
    Log(level, msg, 0);
}

void Logger::Log(LogLevel level, const std::string &msg, size_t line)
{
    if (!m_running)
    {
        throw LoggerException("Logger is not running");
    }

    if (level < m_level)
    {
        return;
    }

    std::stringstream ss;
    auto now_time = std::chrono::system_clock::now();
    auto now_time_as_timet = std::chrono::system_clock::to_time_t(now_time);
    auto now_as_tm = std::localtime(&now_time_as_timet);
    ss << "[" << std::put_time(now_as_tm, "%Y-%m-%d %H:%M:%S") << "] ";

    switch (level)
    {
    case LogLevel::DEBUG:
        ss << "[DEBUG] ";
        break;
    case LogLevel::INFO:
        ss << "[INFO] ";
        break;
    case LogLevel::WARNING:
        ss << "[WARNING] ";
        break;
    case LogLevel::ERROR:
        ss << "[ERROR] ";
        break;
    default:
        break;
    }

    ss << msg;

    if (0 != line)
    {
        ss << " (Line: " << line << ")";
    }

    ss << std::endl;

    //now that the Log message are ready - we write it to m_queue
    m_queue.Push(ss.str());
}

void Logger::SetOutput(std::ostream &output_stream) //my TODO: throw execption
{
/*     if (!m_log_file.is_open())
    {
        m_log_file.close();
    } */

    m_buffer.rdbuf(output_stream.rdbuf());
}

void Logger::SetOutput(const std::string &f_name)
{
    m_f_name = f_name;
    m_log_file.open(m_f_name, std::ios::app); //app for write in the end of the stream

    if (!m_log_file.is_open())
    {
        throw LoggerException("Failed to open log file: " + m_f_name);
    }

    m_buffer.rdbuf(m_log_file.rdbuf());
}

void Logger::SetLevel(LogLevel level)
{
    m_level = level;
}

void Logger::Stop()
{
    if (m_running)
    {
        m_running = false;
    }

    if(m_thread.joinable())
    {
        m_thread.join();
    }

    if (m_log_file.is_open())
    {
        m_log_file.close();
    }
}

void Logger::LogFromQueue()
{
    while (m_running || !m_queue.IsEmpty())
    {
        std::string msg;
        m_queue.Pop(&msg);
        if (m_log_file.is_open())
        {
            m_log_file << msg;
        }

        else
        {
            m_buffer << msg;
        }
        
    }
}
} //end of namespace ilrd_hrd33