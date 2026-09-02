#include "Histogram.h"

Histogram::Histogram() : FilterBase("Histogram")
{}

ip::ImageBuffer Histogram::runFilter(ip::ImageBuffer image)
{

    auto start = std::chrono::high_resolution_clock::now();
    int width = image.width();
    int height = image.height();
    int rowStride = image.rowStride();

    int histogram[3][256] = {};
    std::string label[3] = {"BLUE", "GREEN", "RED"};

    const std::uint8_t* data = image.data();
    

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int index = x * ip::ImageBuffer::CHANNELS + y * rowStride;
               
            for (int color = 0; color < 3; ++color) {
                ++histogram[color][data[index + color]];
            }
        }
    }

    int maxValue[3] = { 0, 0, 0 };

    for (int i = 0; i < 256; ++i)
    {
        for (int color = 0; color < 3; ++color) {
            maxValue[color] = maxValue[color] < histogram[color][i] ? histogram[color][i] : maxValue[color];
        }
       
    }

    std::cout << "===== Histogram =====\n";

    for (int color = 0; color < ip::ImageBuffer::CHANNELS; ++color) {
        std::cout << "\n[" << label[color] << "]\n";
        for (int i = 0; i < 256; ++i)
        {
            int barLength = (double)histogram[color][i] / (double)(maxValue[color]) * 20;
                
            std::cout << i << " | ";

            for (int j = 0; j < barLength; ++j)
            {
                std::cout << '#';
            }

            std::cout
                << " (" << histogram[color][i] << ")\n";
        }
    }



    

    std::cout << "===============================\n";

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        );


    LOGGER.LogInfo(filterName, "처리 시간(us) " + std::to_string(duration.count()));

    return image;
}

void Histogram::threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY)
{}

