/*
clear; gcc -Wall -g pixelDiffersSignificantly.c -o test-pixelDiffersSignificantly -Dtest_pixelDiffersSignificantly && ./test-pixelDiffersSignificantly
*/

#ifndef pixelsDifferSignificantly_h
#define pixelsDifferSignificantly_h

#include "StatsUInt8.c"

/** Return 1 (true) if at least one channel differs from the mean for that
 * channel more than the standard deviation for that channel, according to
 * `stats`. Return 0 (false) if no channel differs from the mean for that
 * channel more than the standard deviation for that channel.
 */
int pixelDiffersSignificantly(uint8_t r, uint8_t g, uint8_t b, StatsUInt8 stats) {
    const uint8_t rdiff = r > stats.means[0] ?
        r - stats.means[0] :
        stats.means[0] - r;

    const uint8_t gdiff = g > stats.means[1] ? 
        g - stats.means[1] :
        stats.means[1] - g;

    const uint8_t bdiff = b > stats.means[2] ?
        b - stats.means[2] :
        stats.means[2] - b;

    if (rdiff > stats.standardDeviations[0]
            || gdiff > stats.standardDeviations[1]
            || bdiff > stats.standardDeviations[2]) {
        // differs significantly in at least one channel
        return 1;
    }

    // does not differ significantly in any channel

    return 0;
}

#ifdef test_pixelDiffersSignificantly

#include "assert_h.h"
#include "stdio_h.h"

int main() {
    const StatsUInt8 stats = {0};
    assert(!pixelDiffersSignificantly(0, 0, 0, stats));
    assert(pixelDiffersSignificantly(1, 0, 0, stats));
    assert(pixelDiffersSignificantly(0, 1, 0, stats));
    assert(pixelDiffersSignificantly(0, 0, 1, stats));
    printf("[%d] %s %s", 0, "PASS", "It runs.");
}

#endif // end test

#endif // end header guard
