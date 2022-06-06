//#include <stdio.h>
//#include <stdlib.h>
//#include "emscripten.h"

#include "findLeftEdge.c"

#ifndef size_t
typedef unsigned long size_t;
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

uint8_t clamp(int value, int scale) {
    if (!scale) return 255;
    value /= scale;
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (uint8_t)(value);
}

void applyEdgeKernelToPixel(uint8_t* inBuf, uint8_t* outBuf, int xPos, int yPos, size_t width, size_t height) {

    const size_t channels = 4;

    const int xStride = channels;
    const int yStride = channels * width;

    const int kernel[10] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1,
        1
    };

    int sum = 0;
    int kindex = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int k = -1; k <= 1; ++k) {
            const int x = xPos + k;
            const int y = yPos + i;
            if (0 <= x && x < width && 0 <= y && y < height) {
                int index = x * xStride + y * yStride;
                sum += (int)inBuf[index] * (int)kernel[kindex];
            }
            ++kindex;
        }
    }

    int index = xPos * xStride + yPos * yStride;

    uint8_t clampedSum = clamp(sum, kernel[kindex]);

    outBuf[index] = clampedSum;
    outBuf[index+1] = clampedSum;
    outBuf[index+2] = clampedSum;
    outBuf[index + 3] = 255;
}

void applyEdgeKernelToImage(uint8_t* inBuf, uint8_t* outBuf, size_t width, size_t height) {
    for (int x = 0; x < (int)width; ++x)
        for (int y = 0; y < (int)height; ++y)
            applyEdgeKernelToPixel(inBuf, outBuf, x, y, width, height);
}

/** Get the left edge of an image that sits between two vertical cinematic bars.
 * See findLeftEdge.
 */
void getLeftEdgePos(uint8_t* inBuf,
                    size_t width,
                    size_t height,
                    size_t time)
{
    size_t samplewidth = 10;
    size_t channels = 4;
    return findLeftEdge(inBuf, channels, samplewidth, width, height, time);
}
