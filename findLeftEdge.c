/*
clear; gcc -Wall -g findLeftEdge.c -o test-findLeftEdge -Dtest_findLeftEdge -lm && ./test-findLeftEdge > outfile.txt
*/

#ifndef findLeftEdge_h
#define findLeftEdge_h

#include "stdlib_h.h"
#include "stdint_h.h"
#include "sqrtUInt16.c"
#include "StatsUInt8.c"
#include "pixelDiffersSignificantly.c"
#include "create_test_image_with_vertical_cinematic_bars.c"

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
size_t findLeftEdge(const uint8_t* inBuf,
                    size_t channels,
                    size_t samplewidth,
                    size_t width,
                    size_t height,
                    size_t time)
{
    const StatsUInt8 stats = StatsUInt8_forImageSeries(inBuf,
                                                       channels,
                                                       samplewidth,
                                                       width, //full width
                                                       height,
                                                       time);

    //puts("Stats:");
    //StatsUInt8_show(stats);

    const size_t xstride = channels;
    const size_t ystride = width * channels;
    const size_t tstride = height * width * channels;

    for (size_t t = 0; t < time; ++t) {
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width / 2; ++x) {
                const size_t r = x * xstride + y * ystride + t * tstride;

                //printf("(%d %d %d)\n", inBuf[r], inBuf[r+1], inBuf[r+2]);

                if (pixelDiffersSignificantly(inBuf[r], inBuf[r+1], inBuf[r+2], stats)) {
                    //printf("Differs from mean at (x, y, t) = (%zu, %zu, %zu)\n", x, y, t);
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
    const size_t channels=4;
    const size_t samplewidth=1;
    const size_t width = 4;
    const size_t height = 4;
    const size_t time = 1;
    const uint8_t data[(4 * 4 * 1) * 4] = {
        0, 0, 0, 255,     0,   0,   0, 255,     0,   0,   0, 255,   0, 0, 0, 255,

        0, 0, 0, 255,   255, 255, 255, 255,   255, 255, 255, 255,   0, 0, 0, 255,

        0, 0, 0, 255,   255, 255, 255, 255,   255, 255, 255, 255,   0, 0, 0, 255,

        0, 0, 0, 255,     0,   0,   0, 255,     0,   0,   0, 255,   0, 0, 0, 255,
    };
    size_t leftEdgePos = findLeftEdge(data, channels, samplewidth, width, height, time);
    //DEBUG//printf("leftEdgePos = %zu\n", leftEdgePos);
    //DEBUG//printf("[%d] %s %s", 0, "???", "It runs.");
    size_t expected = 1;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[0] FAIL It finds an edge.");
        //printf("expected=%zu not equal to actual=%zu\n", expected, actual);
        return 1;
    }
    puts("[0] PASS It finds an edge.");
    return 0;
}

int test1() {
    const size_t channels=4;
    const size_t samplewidth=2;
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
    size_t leftEdgePos = findLeftEdge(data, channels, samplewidth, width, height, time);
    //DEBUG//printf("leftEdgePos = %zu\n", leftEdgePos);
    //DEBUG//printf("[%d] %s %s", 0, "???", "It runs.");
    size_t expected = 2;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[1] FAIL It finds and edge (2).");
        //printf("expected=%zu not equal to actual=%zu\n", expected, actual);
        return 1;
    }
    puts("[1] PASS It finds an edge (2).");
    return 0;
}

int test2() {
    const size_t channels=4;
    const size_t samplewidth=2;
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
    size_t leftEdgePos = findLeftEdge(data, channels, samplewidth, width, height, time);
    //DEBUG//printf("leftEdgePos = %zu\n", leftEdgePos);
    //DEBUG//printf("[%d] %s %s", 0, "???", "It runs.");
    size_t expected = 2;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[2] FAIL It sees a very different pixel as an edge.");
        //printf("expected=%zu not equal to actual=%zu\n", expected, actual);
        return 1;
    }
    puts("[2] PASS It sees a very different pixel as an edge.");
    return 0;
}

int test3() {
    const size_t channels=4;
    const size_t samplewidth=2;
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
    size_t leftEdgePos = findLeftEdge(data, channels, samplewidth, width, height, time);
    size_t expected = 0;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[3] FAIL It returns index 0 if no edge is found.");
        //printf("expected=%zu not equal to actual=%zu\n", expected, actual);
        return 1;
    }
    puts("[3] PASS It returns index 0 if no edge is found.");
    return 0;
}

int test4() {
    const size_t channels = 4;
    const size_t samplewidth=2;
    const size_t width = 6;
    const size_t height = 6;
    const size_t time = 1;
    const uint8_t data[(6 * 6 * 1) * 4] = {
          0,   0,   0, 254,  254, 254, 254, 254,  128, 128, 128, 254,  0, 0, 0, 254,  254, 254, 254, 254,  0, 0, 0, 254,

        254, 254, 254, 254,    0,   0,   0, 254,  128, 128, 128, 254,  0, 0, 0, 254,    0,   0,   0, 254,  0, 0, 0, 254,
                                                                    
          0,   0,   0, 254,  254, 254, 254, 254,  128, 128, 128, 254,  0, 0, 0, 254,  254, 254, 254, 254,  0, 0, 0, 254,

        254, 254, 254, 254,    0,   0,   0, 254,  128, 128, 128, 254,  0, 0, 0, 254,  254, 254, 254, 254,  0, 0, 0, 254,

          0,   0,   0, 254,  254, 254, 254, 254,  128, 128, 128, 254,  0, 0, 0, 254,    0,   0,   0, 254,  0, 0, 0, 254,

        254, 254, 254, 254,    0,   0,   0, 254,  128, 128, 128, 254,  0, 0, 0, 254,  254, 254, 254, 254,  0, 0, 0, 254,
    };
    size_t leftEdgePos = findLeftEdge(data, channels, samplewidth, width, height, time);
    size_t expected = 0;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[4] FAIL It doesn't see pixels as edges if their brightness is not statistically significant.");
        printf("expected=%zu not equal to actual=%zu\n", expected, actual);
        return 1;
    }
    puts("[4] PASS It doesn't see pixels as edges if their brightness is not statistically significant.");
    return 0;
}

int test5() {
    const size_t channels = 4;
    const size_t samplewidth=2;
    const size_t width = 6;
    const size_t height = 6;
    const size_t time = 1;
    const uint8_t data[(6 * 6 * 1) * 4] = {
          0,   0,   0, 255,  255, 255, 255, 255,  128, 128, 128, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        255, 255, 255, 255,    0,   0,   0, 255,  128, 128, 128, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,
                                                                    
          0,   0,   0, 255,  255, 255, 255, 255,  128, 128, 128, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

        255, 255, 255, 255,    0,   0,   0, 255,  128, 128, 128, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,

          0,   0,   0, 255,  255, 255, 255, 255,  128, 128, 128, 255,  0, 0, 0, 255,    0,   0,   0, 255,  0, 0, 0, 255,

        255, 255, 255, 255,    0,   0,   0, 255,  128, 128, 128, 255,  0, 0, 0, 255,  255, 255, 255, 255,  0, 0, 0, 255,
    };
    size_t leftEdgePos = findLeftEdge(data, channels, samplewidth, width, height, time);
    size_t expected = 1;
    size_t actual = leftEdgePos;
    if (expected != actual) {
        puts("[5] FAIL It computes the mean with integer division, where 255 / 2 = 127.");
        printf("expected=%zu not equal to actual=%zu\n", expected, actual);
        return 1;
    }
    puts("[5] PASS It computes the mean with integer division, where 255 / 2 = 127.");
    return 0;
}


int test6() {
    const size_t channels=3,
                 samplewidth=10,
                 width=60,
                 height=96,
                 barwidth=12,
                 time=1;

    uint8_t* cwhData = create_test_image_with_vertical_cinematic_bars(channels,
                                                                      width,
                                                                      height,
                                                                      barwidth);

    //print_2d_pixel_array_uint8(channels, width, height, cwhData, " ", "\t", "\n");

    save_image_stb_uint8(cwhData, channels, width, height, "bars");

    size_t leftEdgePos = findLeftEdge(cwhData, channels, samplewidth, width, height, time);

    size_t expected = barwidth;
    size_t actual = leftEdgePos;

    if (expected != actual) {
        puts("[6] FAIL It finds the left edge of a test image with black vertical cinematic bars and a white image.");
        //printf("expected=%zu not equal to actual=%zu\n", expected, actual);
        return 1;
    }

    puts("[6] PASS It finds the left edge of a test image with black vertical cinematic bars and a white image.");

    return 0;
}

int main() {
    test0();
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    return 0;
}

#endif // end test

#endif // end header guard
