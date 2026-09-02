#pragma once
#include "FilterBase.h"

#define RED_WEIGHT 0.299
#define GREEN_WEIGHT 0.587
#define BLUE_WEIGHT 0.144

class GrayScale : public FilterBase
{
private:
	bool weight;
public:
	GrayScale(bool weight);
	~GrayScale() = default;

	ip::ImageBuffer runFilter(ip::ImageBuffer image) override;

	// FilterBase을(를) 통해 상속됨
	void threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY) override;
};

