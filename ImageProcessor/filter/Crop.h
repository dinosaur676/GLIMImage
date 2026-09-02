#pragma once
#include "FilterBase.h"
class Crop : public FilterBase
{
private:
	unsigned int startX, startY;
	unsigned int endX, endY;
public:
	Crop(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY);
	~Crop() = default;

	ip::ImageBuffer runFilter(ip::ImageBuffer image) override;


	// FilterBase을(를) 통해 상속됨
	void threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY) override;

};


