/*
clear; gcc -Wall -g findLeftEdge.c -o test-findLeftEdge -Dtest_findLeftEdge && ./test-findLeftEdge
*/

#ifndef findLeftEdge_h
#define findLeftEdge_h

#include "stdlib_h.h"
#include "stdint_h.h"
#include "sqrtUInt16.c"
#include "StatsUInt8.c"
#include "pixelDiffersSignificantly.c"

/** Minimum bar width in pixels
 */
#define MIN_BAR_WIDTH 1

// Not used.
//uint8_t grayUInt8(uint8_t r, uint8_t g, uint8_t b) {
//    return (uint8_t)( ( 6*(uint16_t)r + 3*(uint16_t)g + (uint16_t)b ) / 10 );
//}

/** Detect the location of an edge under very specific conditions, without
 * averaging or convolving over the entire image.
 *
 * Given the constraints described below, the edge can be found by simply
 * finding the pixel mean and standard deviation over small region within the
 * bar, and then returning the x-position of the first pixel that has at least
 * one significantly different channel.
 *
 * Return 0 if no edge is found.
 *
 * CONSTRAINTS
 *
 * - it's a screen share video
 *
 * - there are sidebars in the video that have high contrast to the actual
 *   video, but which may contain noise (normally eliminated with a 2-d
 *   smoothing kernel)
 *
 * - there is a minimum sidebar width, so there is a smallest sidebar that can
 *   be used to get an average and error bars for the dark pixels
 *
 * - there is an initial time period during which the video can be observed
 *   in order for the important quantities to be determined.
 *
 * - only one edge, say the left edge, needs to be located. The dynamic
 *   content is always in the center.
 *
 * @param inBuf a CWHT pixel array of unsigned integers between 0 and 255.
 *
 *              i.e. RGBARGBARGBA might be a 3x3 px image at T=0
 *                   RGBARGBARGBA
 *                   RGBARGBARGBA
 *
 * @param width The height of the video.
 *
 * @param height The height of the video.
 *
 * @param time The number of time steps (animation frames).
 * 
 * This function assumes only "good" inputs are passed.
 *
 * This function avoids floating point computations.
 */
size_t findLeftEdge(const uint8_t* inBuf, size_t width, size_t height, size_t time) {

    const StatsUInt8 stats = StatsUInt8_forImageSeries(inBuf, MIN_BAR_WIDTH, height, time);

    //DEBUG//puts("Stats:");
    //DEBUG//StatsUInt8_show(stats);

    const size_t xstride = 4;
    const size_t ystride = width * 4;
    const size_t tstride = height * width * 4;

    for (size_t t = 0; t < time; ++t) {
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width / 2; ++x) {
                const size_t i = x * xstride + y * ystride + t * tstride;

                //DEBUG//printf("(%d %d %d) ", inBuf[i], inBuf[i+1], inBuf[i+2]);

                if (pixelDiffersSignificantly(inBuf[i], inBuf[i + 1], inBuf[i + 2], stats)) {
                    //DEBUG//printf("Differs from mean at (x, y, t) = (%zu, %zu, %zu)", x, y, t);
                    return x;
                }
            }
            //DEBUG//puts("");
        }
        //DEBUG//puts("");
    }
    //DEBUF//puts("");
    return 0;
}

#ifdef test_findLeftEdge

#include "stdio_h.h"

int test0() {
    const size_t width = 4;
    const size_t height = 4;
    const size_t time = 1;
    const uint8_t data[(4 * 4 * 1) * 4] = {
        0, 0, 0, 255,     0,   0,   0, 255,     0,   0,   0, 255,   0, 0, 0, 255,

        0, 0, 0, 255,   255, 255, 255, 255,   255, 255, 255, 255,   0, 0, 0, 255,

        0, 0, 0, 255,   255, 255, 255, 255,   255, 255, 255, 255,   0, 0, 0, 255,

        0, 0, 0, 255,     0,   0,   0, 255,     0,   0,   0, 255,   0, 0, 0, 255,
    };
    size_t leftEdgePos = findLeftEdge(data, width, height, time);
    //DEBUG//printf("leftEdgePos = %zu\n", leftEdgePos);
    //DEBUG//printf("[%d] %s %s", 0, "???", "It runs.");
    size_t expected = 1;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[0] FAIL It finds an edge.");
        printf("expected=%zu not equal to actual=%zu", expected, actual);
        return 1;
    }
    puts("[0] PASS It finds an edge.");
    return 0;
}

int test1() {
    const size_t width = 6;
    const size_t height = 6;
    const size_t time = 2;
    const uint8_t data[(6 * 6 * 2) * 4] = {
        0, 0, 0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,
                                                                    
        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,
    };
    size_t leftEdgePos = findLeftEdge(data, width, height, time);
    //DEBUG//printf("leftEdgePos = %zu\n", leftEdgePos);
    //DEBUG//printf("[%d] %s %s", 0, "???", "It runs.");
    size_t expected = 2;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[1] FAIL It finds and edge (2).");
        printf("expected=%zu not equal to actual=%zu", expected, actual);
        return 1;
    }
    puts("[1] PASS It finds an edge (2).");
    return 0;
}

int test2() {
    const size_t width = 6;
    const size_t height = 6;
    const size_t time = 2;
    const uint8_t data[(6 * 6 * 2) * 4] = {
        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,
                                                                    
        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,
    };
    size_t leftEdgePos = findLeftEdge(data, width, height, time);
    //DEBUG//printf("leftEdgePos = %zu\n", leftEdgePos);
    //DEBUG//printf("[%d] %s %s", 0, "???", "It runs.");
    size_t expected = 2;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[2] FAIL It sees a very different pixel as an edge.");
        printf("expected=%zu not equal to actual=%zu", expected, actual);
        return 1;
    }
    puts("[2] PASS It sees a very different pixel as an edge.");
    return 0;
}

int test3() {
    const size_t width = 6;
    const size_t height = 6;
    const size_t time = 2;
    const uint8_t data[(6 * 6 * 2) * 4] = {
        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,
                                                                    
        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,

        0, 0, 0, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,
    };
    size_t leftEdgePos = findLeftEdge(data, width, height, time);
    size_t expected = 0;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[3] FAIL It returns index 0 if no edge is found.");
        printf("expected=%zu not equal to actual=%zu", expected, actual);
        return 1;
    }
    puts("[3] PASS It returns index 0 if no edge is found.");
    return 0;
}

int test4() {
    const size_t width = 6;
    const size_t height = 6;
    const size_t time = 2;
    const uint8_t data[(6 * 6 * 2) * 4] = {
          0,   0,   0, 255,  255, 255, 255, 255,  128, 128, 128, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        255, 255, 255, 255,    0,   0,   0, 255,  128, 128, 128, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,
                                                                    
          0,   0,   0, 255,  255, 255, 255, 255,  128, 128, 128, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        255, 255, 255, 255,    0,   0,   0, 255,  128, 128, 128, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

          0,   0,   0, 255,  255, 255, 255, 255,  128, 128, 128, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,

        255, 255, 255, 255,    0,   0,   0, 255,  128, 128, 128, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,
    };
    size_t leftEdgePos = findLeftEdge(data, width, height, time);
    size_t expected = 0;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[3] FAIL It doesn't see pixels as edges if their brightness is not statistically significant.");
        printf("expected=%zu not equal to actual=%zu", expected, actual);
        return 1;
    }
    puts("[3] PASS It doesn't see pixels as edges if their brightness is not statistically significant.");
    return 0;
}


int main() {
    test0();
    test1();
    test2();
    test3();
    test4();
    return 0;
}

#endif // end test

#endif // end header guard
