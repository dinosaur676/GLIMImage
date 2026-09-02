#include "Logger.h"

Logger Logger::instance;

std::string Logger::getCurrentTime()
{
	time_t timer;
	struct tm* t;
	timer = time(nullptr);
	t = localtime(&timer);

	std::string year = std::to_string(t->tm_year + 1900);
	std::string mon = std::to_string(t->tm_mon + 1);
	std::string day = std::to_string(t->tm_mday);
	std::string hour = std::to_string(t->tm_hour);
	std::string min = std::to_string(t->tm_min);
	std::string sec = std::to_string(t->tm_sec);


	return year + "/" + mon + "/" + day + "  " + hour + ":" + min + ":" + sec;
}

void Logger::init(std::string filePath, bool isTimePrint)
{
	if (instance.initialized)
		return;

	instance.filePath = filePath;
	instance.isTimePrint = isTimePrint;

	instance.file.open(filePath, std::ios::out | std::ios::app);
	if (!instance.file.is_open()) {
		std::cout << "file Open Error: "
			<< filePath
			<< std::endl;

		return;
	}

	instance.initialized = true;

}

Logger& Logger::getInstance()
{
	if (!instance.initialized)
		init("./log.txt", true);

	return instance;
}

void Logger::LogError(std::string name, std::string message)
{
	std::string currentTime = getInstance().isTimePrint ? getCurrentTime() : "";

	getInstance().file << "[" << currentTime << "][ERROR] " << name << " : " << message << std::endl;
}

void Logger::LogInfo(std::string name, std::string message)
{
	std::string currentTime = getInstance().isTimePrint ? getCurrentTime() : "";

	getInstance().file << "[" << currentTime << "][INFO] " << name << " : " << message << std::endl;
}
