/*
gcc -g -Wall sqrtUInt8.c -o test-sqrtUInt8 -Dtest_sqrtUInt8 -lm && ./test-sqrtUInt8
*/

#ifndef sqrtUInt8_h
#define sqrtUInt8_h

#include "stdint_h.h"

#define sqrtUInt8_lookup /**/\
(const uint8_t [256]){       \
    [0] = 0,                 \
    [1] = 1,                 \
    [2] = 1,                 \
    [3] = 1,                 \
    [4] = 2,                 \
    [5] = 2,                 \
    [6] = 2,                 \
    [7] = 2,                 \
    [8] = 2,                 \
    [9] = 3,                 \
    [10] = 3,                \
    [11] = 3,                \
    [12] = 3,                \
    [13] = 3,                \
    [14] = 3,                \
    [15] = 3,                \
    [16] = 4,                \
    [17] = 4,                \
    [18] = 4,                \
    [19] = 4,                \
    [20] = 4,                \
    [21] = 4,                \
    [22] = 4,                \
    [23] = 4,                \
    [24] = 4,                \
    [25] = 5,                \
    [26] = 5,                \
    [27] = 5,                \
    [28] = 5,                \
    [29] = 5,                \
    [30] = 5,                \
    [31] = 5,                \
    [32] = 5,                \
    [33] = 5,                \
    [34] = 5,                \
    [35] = 5,                \
    [36] = 6,                \
    [37] = 6,                \
    [38] = 6,                \
    [39] = 6,                \
    [40] = 6,                \
    [41] = 6,                \
    [42] = 6,                \
    [43] = 6,                \
    [44] = 6,                \
    [45] = 6,                \
    [46] = 6,                \
    [47] = 6,                \
    [48] = 6,                \
    [49] = 7,                \
    [50] = 7,                \
    [51] = 7,                \
    [52] = 7,                \
    [53] = 7,                \
    [54] = 7,                \
    [55] = 7,                \
    [56] = 7,                \
    [57] = 7,                \
    [58] = 7,                \
    [59] = 7,                \
    [60] = 7,                \
    [61] = 7,                \
    [62] = 7,                \
    [63] = 7,                \
    [64] = 8,                \
    [65] = 8,                \
    [66] = 8,                \
    [67] = 8,                \
    [68] = 8,                \
    [69] = 8,                \
    [70] = 8,                \
    [71] = 8,                \
    [72] = 8,                \
    [73] = 8,                \
    [74] = 8,                \
    [75] = 8,                \
    [76] = 8,                \
    [77] = 8,                \
    [78] = 8,                \
    [79] = 8,                \
    [80] = 8,                \
    [81] = 9,                \
    [82] = 9,                \
    [83] = 9,                \
    [84] = 9,                \
    [85] = 9,                \
    [86] = 9,                \
    [87] = 9,                \
    [88] = 9,                \
    [89] = 9,                \
    [90] = 9,                \
    [91] = 9,                \
    [92] = 9,                \
    [93] = 9,                \
    [94] = 9,                \
    [95] = 9,                \
    [96] = 9,                \
    [97] = 9,                \
    [98] = 9,                \
    [99] = 9,                \
    [100] = 10,              \
    [101] = 10,              \
    [102] = 10,              \
    [103] = 10,              \
    [104] = 10,              \
    [105] = 10,              \
    [106] = 10,              \
    [107] = 10,              \
    [108] = 10,              \
    [109] = 10,              \
    [110] = 10,              \
    [111] = 10,              \
    [112] = 10,              \
    [113] = 10,              \
    [114] = 10,              \
    [115] = 10,              \
    [116] = 10,              \
    [117] = 10,              \
    [118] = 10,              \
    [119] = 10,              \
    [120] = 10,              \
    [121] = 11,              \
    [122] = 11,              \
    [123] = 11,              \
    [124] = 11,              \
    [125] = 11,              \
    [126] = 11,              \
    [127] = 11,              \
    [128] = 11,              \
    [129] = 11,              \
    [130] = 11,              \
    [131] = 11,              \
    [132] = 11,              \
    [133] = 11,              \
    [134] = 11,              \
    [135] = 11,              \
    [136] = 11,              \
    [137] = 11,              \
    [138] = 11,              \
    [139] = 11,              \
    [140] = 11,              \
    [141] = 11,              \
    [142] = 11,              \
    [143] = 11,              \
    [144] = 12,              \
    [145] = 12,              \
    [146] = 12,              \
    [147] = 12,              \
    [148] = 12,              \
    [149] = 12,              \
    [150] = 12,              \
    [151] = 12,              \
    [152] = 12,              \
    [153] = 12,              \
    [154] = 12,              \
    [155] = 12,              \
    [156] = 12,              \
    [157] = 12,              \
    [158] = 12,              \
    [159] = 12,              \
    [160] = 12,              \
    [161] = 12,              \
    [162] = 12,              \
    [163] = 12,              \
    [164] = 12,              \
    [165] = 12,              \
    [166] = 12,              \
    [167] = 12,              \
    [168] = 12,              \
    [169] = 13,              \
    [170] = 13,              \
    [171] = 13,              \
    [172] = 13,              \
    [173] = 13,              \
    [174] = 13,              \
    [175] = 13,              \
    [176] = 13,              \
    [177] = 13,              \
    [178] = 13,              \
    [179] = 13,              \
    [180] = 13,              \
    [181] = 13,              \
    [182] = 13,              \
    [183] = 13,              \
    [184] = 13,              \
    [185] = 13,              \
    [186] = 13,              \
    [187] = 13,              \
    [188] = 13,              \
    [189] = 13,              \
    [190] = 13,              \
    [191] = 13,              \
    [192] = 13,              \
    [193] = 13,              \
    [194] = 13,              \
    [195] = 13,              \
    [196] = 14,              \
    [197] = 14,              \
    [198] = 14,              \
    [199] = 14,              \
    [200] = 14,              \
    [201] = 14,              \
    [202] = 14,              \
    [203] = 14,              \
    [204] = 14,              \
    [205] = 14,              \
    [206] = 14,              \
    [207] = 14,              \
    [208] = 14,              \
    [209] = 14,              \
    [210] = 14,              \
    [211] = 14,              \
    [212] = 14,              \
    [213] = 14,              \
    [214] = 14,              \
    [215] = 14,              \
    [216] = 14,              \
    [217] = 14,              \
    [218] = 14,              \
    [219] = 14,              \
    [220] = 14,              \
    [221] = 14,              \
    [222] = 14,              \
    [223] = 14,              \
    [224] = 14,              \
    [225] = 15,              \
    [226] = 15,              \
    [227] = 15,              \
    [228] = 15,              \
    [229] = 15,              \
    [230] = 15,              \
    [231] = 15,              \
    [232] = 15,              \
    [233] = 15,              \
    [234] = 15,              \
    [235] = 15,              \
    [236] = 15,              \
    [237] = 15,              \
    [238] = 15,              \
    [239] = 15,              \
    [240] = 15,              \
    [241] = 15,              \
    [242] = 15,              \
    [243] = 15,              \
    [244] = 15,              \
    [245] = 15,              \
    [246] = 15,              \
    [247] = 15,              \
    [248] = 15,              \
    [249] = 15,              \
    [250] = 15,              \
    [251] = 15,              \
    [252] = 15,              \
    [253] = 15,              \
    [254] = 15,              \
    [255] = 15,              \
}

uint8_t sqrtUInt8(uint8_t i) {
    return sqrtUInt8_lookup[i];
}

#ifdef test_sqrtUInt8

#include <math.h>
#include <assert.h>
#include "stdio_h.h"

int main() {
    for (int i=0; i<256; ++i) {
        printf("sqrtUInt8(%d) = %d\n", i, sqrtUInt8(i));
        assert((float)sqrtUInt8(i) <= sqrt((float)i));
    }
    const int pass = 1;
    printf("\n[%d] %s %s", 0, pass ? "PASS" : "FAIL",
            "It satisfies sqrtUInt8(i) <= sqrt(i) for all i in [0,255].\n");
    return 0;
}
#endif // end test

#endif // end header guard