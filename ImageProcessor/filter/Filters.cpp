#include "Filters.h"

Filters& Filters::getInstance()
{
    static Filters instance;
    return instance;
}

void Filters::addFilter(FilterBase* filter)
{
    getInstance().filterQueue.push(filter);
}

void Filters::addFilter(std::string filters)
{
    bool isInBright = false;
    std::vector<std::string> filterSplit = util::stringSplit(filters, "/");

    for (int i = 0; i < filterSplit.size(); ++i) {
        if (filterSplit[i].find("gray") != std::string::npos) {
            if (filterSplit[i].find("weight") != std::string::npos) {
                addFilter(new GrayScale(true));
            }
            else {
                addFilter(new GrayScale(false));
            }
        }
        else if (filterSplit[i].find("threshold") != std::string::npos) {
            std::vector<std::string> args = util::stringSplit(filterSplit[i], ":");
            addFilter(new Thresholding(std::stoi(args[1])));
        }
        else if (filterSplit[i].find("blur") != std::string::npos) {
           
            bool isFind = false;;

            for (int filterIndex = 0; filterIndex < filterSplit.size(); ++filterIndex) {
                if (filterSplit[filterIndex].find("sharpen") == std::string::npos)
                    continue;

                std::string sharpenValue = util::stringSplit(filterSplit[filterIndex], ":")[1];

                if(sharpenValue == "1")
                    addFilter(new Blur(false));
                else
                    addFilter(new Blur(true));

                isFind = true;
                break;
            }

            if(!isFind)
                addFilter(new Blur(true));
        }
        else if (filterSplit[i].find("crop") != std::string::npos) {
            std::string argValue = util::stringSplit(filterSplit[i], ":")[1];
            std::vector<std::string> posList = util::stringSplit(argValue, ",");
            addFilter(new Crop(std::stoi(posList[0]), std::stoi(posList[1]), std::stoi(posList[2]), std::stoi(posList[3])));
        }
        else if (filterSplit[i].find("resize") != std::string::npos) {
            std::string argValue = util::stringSplit(filterSplit[i], ":")[1];
            std::vector<std::string> posList = util::stringSplit(argValue, ",");
            addFilter(new Resize(std::stoi(posList[0]), std::stoi(posList[1])));
        }
        else if (filterSplit[i].find("flip") != std::string::npos) {
            std::string argValue = util::stringSplit(filterSplit[i], ":")[1];
            std::vector<std::string> flipList = util::stringSplit(argValue, ",");
            addFilter(new Flip(std::stoi(flipList[1]), std::stoi(flipList[0])));
        }
        else if (filterSplit[i].find("histogram") != std::string::npos) {
            addFilter(new Histogram());
        }
        else if (filterSplit[i].find("bright") != std::string::npos || filterSplit[i].find("contrast") != std::string::npos) {
            
            if (isInBright)
                continue;

            double contrast = 1.0;
            unsigned int bright = 0;

            for (int filterIndex = 0; filterIndex < filterSplit.size(); ++filterIndex) {
                if (filterSplit[filterIndex].find("bright") != std::string::npos)
                {
                    std::string value = util::stringSplit(filterSplit[filterIndex], ":")[1];
                    bright = std::stoi(value);
                }
                else if (filterSplit[filterIndex].find("contrast") != std::string::npos)
                {
                    std::string value = util::stringSplit(filterSplit[filterIndex], ":")[1];
                    contrast = std::stod(value);
                }
            }

            isInBright = true;

            addFilter(new BrightContrast(bright, contrast));
        }
    }
}

ip::ImageBuffer Filters::runFilter(ip::ImageBuffer image)
{
    ip::ImageBuffer outputImage = image;

    while (filterQueue.size() > 0) {
        FilterBase* filter = filterQueue.front();

        outputImage = filter->runFilter(outputImage);

        filterQueue.pop();
        delete filter;
    }


    return outputImage;
}
