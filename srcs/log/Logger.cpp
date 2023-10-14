#include "Logger.hpp"

Logger::Logger()
{
}

Logger::~Logger()
{
}

LogInfos Logger::GetColorAndType(LogTypeEnum type)
{
    std::string logType;
    std::string logColor;
    switch (type)
    {
        case SUCCESS:
            logType = "SUCCESS";
            logColor = GREEN;
            break;
        case INFO:
            logType = "INFO";
            logColor = BLUE;
            break;
        case WARNING:
            logType = "WARNING";
            logColor = YELLOW;
            break;
        case ERROR:
            logType = "ERROR";
            logColor = RED;
            break;
    }
    return LogInfos(logColor, logType);
}

std::string Logger::GetDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d-%m-%Y %X", &tstruct);
    return buf;
}

std::ostream* Logger::SetStream(LogTypeEnum type)
{
    if (type == ERROR)
       return &std::cerr;
    return &std::cout;
}

void Logger::Debug(const std::string& where, LogTypeEnum type,const std::string& message)
{
    if (!DEBUG)
        return ;
    LogInfos logInfos = Logger::GetColorAndType(type);
    std::ostream *stream = Logger::SetStream(type);
    *stream 
            << logInfos.logColor
            << "[" 
            << logInfos.logType
            << "] "
            << CYAN
            << where
            << logInfos.logColor
            << "`"
            << message 
            << "`"
            << RESET 
            << std::endl;
}

void Logger::Log(LogTypeEnum type, const std::string& message)
{

    LogInfos logInfos = Logger::GetColorAndType(type);
    std::string dateTime = Logger::GetDateTime();
    std::ostream *stream = Logger::SetStream(type);
    *stream 
            << logInfos.logColor
            << dateTime
            << " ["  
            << logInfos.logType
            << "]: " 
            << message << " " 
            << RESET << std::endl;
}

void Logger::PrintMessage(LogTypeEnum type, const std::string& message)
{
    LogInfos logInfos = Logger::GetColorAndType(type);
    std::ostream *stream = Logger::SetStream(type);
    *stream 
            << logInfos.logColor
            << "[" 
            << logInfos.logType
            << "] "
            << message 
            << RESET 
            << std::endl;
}

LogInfos::LogInfos(std::string logColor, std::string logType)
{
    this->logColor = logColor;
    this->logType = logType;
}

