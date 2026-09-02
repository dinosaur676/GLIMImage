#pragma once
#include "FilterBase.h"
class Flip : public FilterBase
{
private:
	bool horizon, vertical;
public:
	Flip(bool horizon, bool vertical);
	~Flip() = default;

	ip::ImageBuffer runFilter(ip::ImageBuffer image) override;

	// FilterBase을(를) 통해 상속됨
	void threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY) override;
};

