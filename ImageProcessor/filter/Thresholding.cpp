#include "Thresholding.h"

#include <iostream>

Thresholding::Thresholding(unsigned int threshold) : FilterBase("Threshold")
{
	this->threshold = threshold;
}

ip::ImageBuffer Thresholding::runFilter(ip::ImageBuffer image)
{
    auto start = std::chrono::high_resolution_clock::now();
    int width = image.width();
    int height = image.height();
    int rowStride = image.rowStride();
    int length = width * ip::ImageBuffer::CHANNELS * height;

    ip::ImageBuffer outputImage(width, height);
    std::vector<std::uint8_t> buffer(length);

    GrayScale grayScale = GrayScale(true);
    ip::ImageBuffer originBuffer = grayScale.runFilter(image);


    std::thread t[5];

    for (int i = 0; i < 5; ++i) {
        int startX = width / 5 * i;
        int endX = width / 5 * (i + 1);

        int startY = 0;
        int endY = height;

        endX = endX > width ? width : endX;

        t[i] = std::thread([this, &buffer, &outputImage, &image, startX, endX, startY, endY]() {
            this->threadProcess(buffer, outputImage, image, startX, endX, startY, endY);
            }
        );
    }


    for (int i = 0; i < 5; ++i) {
        t[i].join();
    }

    std::memcpy(outputImage.data(), buffer.data(), length);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        );


    LOGGER.LogInfo(filterName, "처리 시간(us) " + std::to_string(duration.count()));

    return outputImage;
}

void Thresholding::threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY)
{
    int rowStride = image.rowStride();


    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {

            for (int color = 0; color < ip::ImageBuffer::CHANNELS; ++color) {
                int index = x * ip::ImageBuffer::CHANNELS + y * rowStride + color;
                buffer[index] = image.data()[index] >= this->threshold ? 255 : 0;
            }
        }
    }
}
