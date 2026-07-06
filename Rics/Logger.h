#pragma once
#include <string>

class Logger
{
public:
    Logger() {}
    ~Logger() {}
    
    void init(std::string module);
    
    void trace(std::string message);
    void debug(std::string message);
    void info(std::string message);
    void warn(std::string message);
    void error(std::string message);
    
private:
    std::string loggerName;
};
