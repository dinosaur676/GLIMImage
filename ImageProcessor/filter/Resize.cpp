#include "Resize.h"

Resize::Resize(unsigned int width, unsigned int height) : width(width), height(height), FilterBase("Resize")
{}

ip::ImageBuffer Resize::runFilter(ip::ImageBuffer image)
{
    auto start = std::chrono::high_resolution_clock::now();
    int width = image.width();
    int height = image.height();
    int rowStride = image.rowStride();

    int resizeLength = this->width * ip::ImageBuffer::CHANNELS * this->height;

    ip::ImageBuffer outputImage(this->width, this->height);
    std::vector<std::uint8_t> buffer(resizeLength);

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


    std::memcpy(outputImage.data(), buffer.data(), resizeLength);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end - start
        );


    LOGGER.LogInfo(filterName, "처리 시간(us) " + std::to_string(duration.count()));

    return outputImage;
}

void Resize::threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY)
{
    int resizeWidth = outputImage.width();
    int resizeHeight = outputImage.height();

    int originWidth = image.width();
    int originHeight = image.height();
    int rowStride = image.rowStride();


    int resizeStride = resizeWidth * ip::ImageBuffer::CHANNELS;

    for (int x = 0; x < resizeWidth; ++x) {
        for (int y = 0; y < outputImage.height(); ++y) {

            int nx = x * originWidth / resizeWidth * ip::ImageBuffer::CHANNELS;
            int ny = y * originHeight / resizeHeight * rowStride;

            for (int color = 0; color < ip::ImageBuffer::CHANNELS; ++color) {
                int originIndex = nx + ny + color;
                int bufferIndex = x * ip::ImageBuffer::CHANNELS + y * resizeStride + color;
                buffer[bufferIndex] = image.data()[originIndex];
            }
        }
    }

       
}