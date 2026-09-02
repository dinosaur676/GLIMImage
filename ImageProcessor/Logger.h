#pragma once
#include <fstream>
#include <ctime>
#include <string>
#include <iostream>

#include "Exceptions.h"

#define LOGGER Logger::getInstance()

class Logger
{
private:
    Logger() = default;
    ~Logger() = default;

    static Logger instance;

    bool initialized = false;
    bool isTimePrint;
    std::string filePath;
    std::ofstream file;

    std::string getCurrentTime();

public:
    static void init(std::string filePath, bool isTimePrint);
    static Logger& getInstance();

    void LogError(std::string name, std::string message);
    void LogInfo(std::string name, std::string message);


    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};