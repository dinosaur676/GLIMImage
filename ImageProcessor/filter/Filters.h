#pragma once
#include <queue>

#include "./FilterBase.h"
#include "./GrayScale.h"
#include "./Thresholding.h"
#include "./Blur.h"
#include "./Crop.h"
#include "./Resize.h"
#include "./Flip.h"
#include "./BrightContrast.h"
#include "./Histogram.h"

#include "../Util.h"
#include "../CommandLineParser.h"

class Filters
{
	
private:
	std::queue<FilterBase*> filterQueue;

    Filters() = default;
    ~Filters() = default;

public:
    static Filters& getInstance();

    void addFilter(FilterBase* filter);
    void addFilter(std::string filters);

    ip::ImageBuffer runFilter(ip::ImageBuffer image);

    Filters(const Filters&) = delete;
    Filters& operator=(const Filters&) = delete;
};

