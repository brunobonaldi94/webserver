#include "Logger.hpp"

Logger::Logger()
{
}

Logger::~Logger()
{
}

LogInfos Logger::getColorAndType(LogTypeEnum type)
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

void Logger::debug(const std::string& message, LogTypeEnum type, std::string where)
{
    LogInfos logInfos = Logger::getColorAndType(type);
    std::cout << logInfos.logColor
              << "[" 
              << logInfos.logType
              << "] " 
              << where
              << "`"
              << message 
              << "`"
              << RESET << std::endl;
}

void Logger::log(const std::string& message, LogTypeEnum type)
{

    LogInfos logInfos = Logger::getColorAndType(type);
    std::string dateTime = Logger::GetDateTime();
    std::cout << logInfos.logColor
              << "[" 
              << logInfos.logType
              << dateTime
              << " - ] " 
              << message << " " 
              << RESET << std::endl;
}

LogInfos::LogInfos(std::string logColor, std::string logType)
{
    this->logColor = logColor;
    this->logType = logType;
}