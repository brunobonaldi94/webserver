
#pragma once

#include <iostream>
#include <iomanip>
#include <time.h>
#include "defines.hpp"

class Ostream;

#define RESET       "\033[0m"
#define BLACK       "\033[30m"              /* Black */
#define RED         "\033[31m"              /* Red */
#define GREEN       "\033[32m"              /* Green */
#define YELLOW      "\033[33m"              /* Yellow */
#define BLUE        "\033[34m"              /* Blue */
#define MAGENTA     "\033[35m"              /* Magenta */
#define CYAN        "\033[36m"              /* Cyan */
#define WHITE       "\033[37m"              /* White */
#define BOLDBLACK   "\033[1m\033[30m"       /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"       /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"       /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"       /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"       /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"       /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"       /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"       /* Bold White */

typedef enum
{
    SUCCESS,
    INFO,
    WARNING,
    ERROR
} LogTypeEnum;

struct LogInfos
{
    LogInfos(std::string logColor, std::string logType);
    std::string logColor;
    std::string logType;
};

class Logger
{
private:
    Logger();
    ~Logger();
    static LogInfos GetColorAndType(LogTypeEnum type);
    static std::ostream* SetStream(LogTypeEnum type);
    static std::string GetDateTime();

public:
    static void Log(LogTypeEnum type, const std::string& message);
    static void Debug(const std::string& where, LogTypeEnum type,const std::string& message);
    static void PrintMessage(LogTypeEnum type, const std::string& message);
};