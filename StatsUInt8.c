/*
clear && gcc -Wall -g StatsUInt8.c -o test-StatsUInt8_forImageSeries -Dtest_StatsUInt8_forImageSeries -lm && ./test-StatsUInt8_forImageSeries
*/
    
#include "stdlib_h.h"
#include "stdint_h.h"
#include "stdio_h.h"
#include "assert_h.h"
#include "sqrtUInt16.c"

#ifndef StatsUInt8_forImageSeries_h
#define StatsUInt8_forImageSeries_h

#ifndef BUFLEN
#define BUFLEN 1024
#endif

typedef struct StatsUInt8 StatsUInt8;
struct StatsUInt8 {
    uint8_t means[3];
    uint32_t variances[3];
    uint32_t standardDeviations[3];
};

void StatsUInt8_show(StatsUInt8 stats) {
    for (size_t i = 0; i < 3; ++i) {
        //printf("channel: %zu   mean: %d   variance: %d   standardDeviation: %d\n", i, stats.means[i], stats.variances[i], stats.standardDeviations[i]);
    }
}

StatsUInt8 StatsUInt8_forImageSeries(const uint8_t* inBuf,
                                     size_t channels,
                                     size_t width,
                                     size_t fullwidth,
                                     size_t height,
                                     size_t time)
{
    assert(channels==3||channels==4);
    assert(width>0);
    assert(height>0);

    // Compute pixel means.
    uint32_t sums[] = {0, 0, 0};

    for(size_t t=0; t<time; ++t)
    {
        for(size_t y=0; y<height; ++y)
        {
            for(size_t x=0; x<width; ++x)
            {
                const size_t r = x*channels + y*channels*fullwidth + t*channels*fullwidth*height;
                //printf("(r=%u, g=%u, b=%u)\n", inBuf[r], inBuf[r+1], inBuf[r+2]);
                for(size_t z=0; z<3; ++z)
                {
                    //printf("(x=%zu, y=%zu, z=%zu)\n", x, y, x);
                    sums[z] += inBuf[r+z];
                    //printf("OK\n");
                }
                //puts("");
            }
        }
    }

    // Make empty image stats.
    StatsUInt8 stats = {0};
    for (size_t i = 0; i < 3; ++i) {
        stats.means[i] = (uint8_t)(sums[i] / (width*height*time));
    }

    // Zero sums.
    for (size_t z = 0; z < 3; ++z) {
        sums[z] = 0;
    }

    // Compute pixel variances.
    for(size_t t=0; t<time; ++t)
    {
        for(size_t y=0; y<height; ++y)
        {
            for(size_t x=0; x<width; ++x)
            {
                const size_t r = x*channels + y*channels*fullwidth + t*channels*fullwidth*height;
                for(size_t z=0; z<3; ++z)
                {
                    const uint32_t residual = inBuf[r+z] > stats.means[z] ?
                        (inBuf[r+z] - stats.means[z]) :
                        (stats.means[z] - inBuf[r+z]);

                    sums[z] += residual * residual;
                }
            }
        }
    }

    for (size_t z = 0; z < 3; ++z) {
        stats.variances[z] = sums[z] / (width*height*time);
    }

    // Compute pixel standard deviations.
    for (size_t z = 0; z < 3; ++z) {
        stats.standardDeviations[z] = sqrtUInt16(stats.variances[z]);
    }

    // Return image stats.
    return stats;
}

#ifdef test_StatsUInt8_forImageSeries

#include <assert.h>
#include "stdio_h.h"

int main() {
    const size_t channels = 4;
    const size_t width = 1;
    const size_t fullwidth = 1;
    const size_t height = 3;
    const size_t time = 1;

    uint8_t data[] = {
        0, 0, 0, 1,
        255, 255, 255, 1,
        255, 255, 255, 1,
    };

    const StatsUInt8 stats = StatsUInt8_forImageSeries(data,
                                                      channels,
                                                      width,
                                                      fullwidth,
                                                      height,
                                                      time);

    //DEBUG//printf("channel: red   mean: %d   variance: %d   standardDeviation: %d\n", stats.means[0], stats.variances[0], stats.standardDeviations[0]);
    assert(stats.means[0] == 170);
    assert(stats.variances[0] == 14450);
    assert(stats.standardDeviations[1]== 120);

    //DEBUG//printf("channel: green   mean: %d   variance: %d   standardDeviation: %d\n", stats.means[1], stats.variances[1], stats.standardDeviations[1]);
    assert(stats.means[1] == 170);
    assert(stats.variances[1] == 14450);
    assert(stats.standardDeviations[1]== 120);

    //DEBUG//printf("channel: blue   mean: %d   variance: %d   standardDeviation: %d\n", stats.means[2], stats.variances[2], stats.standardDeviations[2]);
    assert(stats.means[2] == 170);
    assert(stats.variances[2] == 14450);
    assert(stats.standardDeviations[2]== 120);

    //DEBUG//fputc('\n', stdout);

    //DEBUG//printf("width: %ld   height: %ld   time: %ld\n", width, height, time);

    //DEBUG//fputc('\n', stdout);

    //DEBUG//printf("data:\n");

    const size_t n = width * height * time;
    for (size_t i = 0; i < n * channels; i += channels) {
        //DEBUG//printf("%d %d %d %d\n", data[i], data[i + 1], data[i + 2], data[i + 3]);
    }

    return EXIT_SUCCESS;
}

#endif // test end

#endif //end header guard
