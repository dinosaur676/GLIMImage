#pragma once
#include "FilterBase.h"
#include "../Util.h"

class Blur : public FilterBase
{
private:
	bool isBlurKerner;

	int blurKernel[9] = { 
		1, 1, 1,
		1, 1, 1, 
		1, 1, 1 };

	int sharpenKerner[9] = { 
		0, -1, 0,
		-1, 5, -1, 
		0, -1, 0 };

public:
	Blur(bool isBlurKerner);
	~Blur() = default;

	ip::ImageBuffer runFilter(ip::ImageBuffer image) override;

	// FilterBase을(를) 통해 상속됨
	void threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY) override;
};

