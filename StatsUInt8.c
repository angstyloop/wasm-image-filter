/*
gcc -Wall -g StatsUInt8.c -o test-StatsUInt8_forImageSeries -Dtest_StatsUInt8_forImageSeries -lm && ./test-StatsUInt8_forImageSeries
*/
    
#include "stdlib_h.h"
#include "stdint_h.h"
#include "stdio_h.h"
#include "sqrtUInt16.c"

#ifndef StatsUInt8_forImageSeries_h
#define StatsUInt8_forImageSeries_h

#ifndef BUFLEN
#define BUFLEN 1024
#endif

typedef struct StatsUInt8 StatsUInt8;
struct StatsUInt8 {
    uint8_t means[3];
    uint16_t variances[3];
    uint8_t standardDeviations[3];
};

void StatsUInt8_show(StatsUInt8 stats) {
    for (size_t i = 0; i < 3; ++i) {
        printf("channel: %zu   mean: %d   variance: %d   standardDeviation: %d\n", i, stats.means[i], stats.variances[i], stats.standardDeviations[i]);
    }
}

StatsUInt8 StatsUInt8_forImageSeries(const uint8_t* inBuf, size_t width, size_t height, size_t time) {
    // Compute pixel means.
    uint16_t sums[] = {0, 0, 0};

    const size_t n = width * height * time;

    for (size_t i = 0; i < n * 4; i += 4) {
        sums[0] += inBuf[i];
        sums[1] += inBuf[i + 1];
        sums[2] += inBuf[i + 2];
    }

    // Make empty image stats.
    StatsUInt8 stats = {0};
    for (size_t i = 0; i < 3; ++i) {
        stats.means[i] = (uint8_t)(sums[i] / n);
    }

    // Compute pixel variances.
    for (size_t j = 0; j < 3; ++j) {
        sums[j] = 0;
    }
    for (size_t i = 0; i < n * 4; i += 4) {
        for (size_t j = 0; j < 3; ++j) {
            const uint16_t residual = inBuf[i + j] > stats.means[i + j] ?
                (inBuf[i + j] - stats.means[j]) :
                (stats.means[j] - inBuf[i + j]);

            sums[j] += residual * residual;
        }
    }
    for (size_t j = 0; j < 3; ++j) {
        stats.variances[j] = sums[j] / n;
    }

    // Compute pixel standard deviations.
    for (size_t j = 0; j < 3; ++j) {
        stats.standardDeviations[j] = sqrtUInt16(stats.variances[j]);
    }

    // Return image stats.
    return stats;
}

#ifdef test_StatsUInt8_forImageSeries

#include <assert.h>
#include "stdio_h.h"

int main() {
    const size_t width = 1;
    const size_t height = 3;
    const size_t time = 1;

    const size_t n = width * height * time;

    uint8_t data[] = {
        0, 0, 0, 1,
        255, 255, 255, 1,
        255, 255, 255, 1,
    };

    const StatsUInt8 stats = StatsUInt8_forImageSeries(data, width, height, time);

    printf("channel: red   mean: %d   variance: %d   standardDeviation: %d\n",
            stats.means[0], stats.variances[0], stats.standardDeviations[0]);
    assert(stats.means[0] == 170);
    assert(stats.variances[0] == 14450);
    assert(stats.standardDeviations[1]== 120);

    printf("channel: green   mean: %d   variance: %d   standardDeviation: %d\n",
            stats.means[1], stats.variances[1], stats.standardDeviations[1]);
    assert(stats.means[1] == 170);
    assert(stats.variances[1] == 14450);
    assert(stats.standardDeviations[1]== 120);

    printf("channel: blue   mean: %d   variance: %d   standardDeviation: %d\n",
            stats.means[2], stats.variances[2], stats.standardDeviations[2]);
    assert(stats.means[2] == 170);
    assert(stats.variances[2] == 14450);
    assert(stats.standardDeviations[2]== 120);

    fputc('\n', stdout);

    printf("width: %ld   height: %ld   time: %ld\n", width, height, time);

    fputc('\n', stdout);

    printf("data:\n");

    for (size_t i = 0; i < n * 4; i += 4) {
        printf("%d %d %d %d\n", data[i], data[i + 1], data[i + 2], data[i + 3]);
    }

    return EXIT_SUCCESS;
}

#endif // test end

#endif //end header guard
