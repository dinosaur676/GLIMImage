#include "Blur.h"
#include <iostream>

Blur::Blur(bool isBlurKerner) : isBlurKerner(isBlurKerner), FilterBase("Blur")
{

}

ip::ImageBuffer Blur::runFilter(ip::ImageBuffer image)
{
    auto start = std::chrono::high_resolution_clock::now();

    int width = image.width();
    int height = image.height();
    int rowStride = image.rowStride();
    int length = width * ip::ImageBuffer::CHANNELS * height;
    int* kerner = this->isBlurKerner ? this->blurKernel : this->sharpenKerner;

    ip::ImageBuffer outputImage(width, height);
    std::vector<std::uint8_t> buffer(length);


    // BMP는 기본적으로 bottom-up: 파일의 첫 행이 이미지의 마지막 행.

    std::thread t[5];

    for (int i = 0; i < 5; ++i) {
        int startX = width / 5 * i;
        int endX = width / 5 * (i + 1);

        int startY = 1;
        int endY = height - 1;

        startX = startX < 1 ? 1 : startX;
        endX = endX > width - 1 ? width - 1: endX;

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

void Blur::threadProcess(std::vector<uint8_t>& buffer, ip::ImageBuffer& outputImage, ip::ImageBuffer& image, int startX, int endX, int startY, int endY)
{

    int* kerner = this->isBlurKerner ? this->blurKernel : this->sharpenKerner;
    int rowStride = image.rowStride();

    int kernerSum = 0;
    for (int i = 0; i < 9; ++i) {
        kernerSum += kerner[i];
    }

    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {

            int sum[3] = { 0, 0, 0 };


            for (int kx = 0; kx < 3; ++kx) {
                for (int ky = 0; ky < 3; ++ky) {

                    int nx = (x + (kx - 1)) * ip::ImageBuffer::CHANNELS;
                    int ny = (y + (ky - 1)) * rowStride;

                    int index = nx + ny;
                    int kernerValue = kerner[ky * 3 + kx];

                    for (int color = 0; color < 3; ++color) {

                        int RGB = image.data()[index + color];
                        sum[color] += RGB * kernerValue;
                    }
                }
            }


            for (int color = 0; color < ip::ImageBuffer::CHANNELS; ++color) {
                int index = x * ip::ImageBuffer::CHANNELS + y * rowStride + color;
                buffer[index] = util::clamp((sum[color] / kernerSum), 0, 255);
            }
        }
    }
}
