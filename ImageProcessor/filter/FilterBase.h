#pragma once
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

#include "../ImageBuffer.h"
#include "../Logger.h"
#include "../Util.h"

class FilterBase
{
protected:
	std::string filterName;

public:

	FilterBase(std::string name) : filterName(name) {}
	~FilterBase() {}

	virtual ip::ImageBuffer runFilter(ip::ImageBuffer image) = 0;
	virtual void threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY) = 0;
	
};

