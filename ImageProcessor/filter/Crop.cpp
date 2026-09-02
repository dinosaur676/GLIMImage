#include "Crop.h"

Crop::Crop(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY) 
	: startX(startX), startY(startY), endX(endX), endY(endY), FilterBase("Crop")
{}

ip::ImageBuffer Crop::runFilter(ip::ImageBuffer image)
{
    auto start = std::chrono::high_resolution_clock::now();

    int width = image.width();
    int height = image.height();
    int rowStride = image.rowStride();
    int length = width * ip::ImageBuffer::CHANNELS * height;

    
    int cropWidth = endX - startX;
    int cropHeight = endY - startY;
    int cropStride = cropWidth * ip::ImageBuffer::CHANNELS;
    int cropLength = cropStride * cropHeight;

    ip::ImageBuffer outputImage(cropWidth, cropHeight);
    std::vector<std::uint8_t> buffer(cropLength);

    std::thread t[5];

    for (int i = 0; i < 5; ++i) {
        int _startX = this->startX + cropWidth / 5 * i;
        int _endX = this->startX + cropWidth / 5 * (i + 1);

        int _startY = this->startY;
        int _endY = this->endY;

        _endX = _endX > this->startX + cropWidth ? this->startX + cropWidth : _endX;

        t[i] = std::thread([this, &buffer, &outputImage, &image, _startX, _endX, _startY, _endY]() {
            this->threadProcess(buffer, outputImage, image, _startX, _endX, _startY, _endY);
            }
        );
    }


    for (int i = 0; i < 5; ++i) {
        t[i].join();
    }


    std::memcpy(outputImage.data(), buffer.data(), cropLength);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        );


    LOGGER.LogInfo(filterName, "처리 시간(us) " + std::to_string(duration.count()));

    return outputImage;
}

void Crop::threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY)
{
    int cropWidth = outputImage.width();
    int cropHeight = outputImage.height();
    int cropStride = outputImage.width() * ip::ImageBuffer::CHANNELS;
    int rowStride = image.rowStride();

    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            int nx = x * ip::ImageBuffer::CHANNELS;
            int ny = y * rowStride;

            for (int color = 0; color < ip::ImageBuffer::CHANNELS; ++color) {
                int originIndex = nx + ny + color;
                int bufferIndex = (x - this->startX) * ip::ImageBuffer::CHANNELS + (y - this->startY) * cropStride + color;
                buffer[bufferIndex] = image.data()[originIndex];
            }
        }
    }
}
