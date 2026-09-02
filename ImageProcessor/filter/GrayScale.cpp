#include "GrayScale.h"

GrayScale::GrayScale(bool weight) :  FilterBase("GrayScale"), weight(weight)
{
    
}

ip::ImageBuffer GrayScale::runFilter(ip::ImageBuffer image)
{

    auto start = std::chrono::high_resolution_clock::now();

    int width = image.width();
    int height = image.height();
    int length = width * ip::ImageBuffer::CHANNELS * height;
    int rowStride = image.rowStride();


    ip::ImageBuffer outputImage(width, height);
    std::vector<std::uint8_t> buffer(length);
    uint8_t* originBuffer = image.data();

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

void GrayScale::threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY)
{
    int width = image.width();
    int height = image.height();
    int rowStride = image.rowStride();

    double weightArray[3] = { RED_WEIGHT, GREEN_WEIGHT, BLUE_WEIGHT };

    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY;
            ++y) {

            double gray = 0.0;
            for (int color = 0; color < ip::ImageBuffer::CHANNELS; ++color) {

                int index = x * ip::ImageBuffer::CHANNELS + y * rowStride + color;

                if (weight) {
                    gray += (double)image.data()[index] * weightArray[color];
                }
                else {
                    gray += (double)image.data()[index] / (double)ip::ImageBuffer::CHANNELS;
                }
            }

            for (int color = 0; color < ip::ImageBuffer::CHANNELS; ++color) {
                int index = x * ip::ImageBuffer::CHANNELS + y * rowStride + color;
                buffer[index] = (int)(gray);
            }
        }
    }
}
