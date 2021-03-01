#include <stdio.h>
#include <stdlib.h>
#include "emscripten.h"

/* The emscripten compiler, emcc , actually has an option, `-s ALLOW_MEMORY_GROWTH=1` , which I believe makes this bit here unnecessary. */

/* Since there is no main function, this is considered a "standalone wasm build", and you will need to also use the `--no-entry` option. */

// unused - just forces the compiler to allocate enough memory for the module
//unsigned char buffer[640*480*4*6] = {0};

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

// don't need this - importing stdlib now
//#ifndef size_t
//typedef unsigned int size_t;
//#endif

uint8_t clamp(int value, int scale) {
    if (!scale) return 255;
    value /= scale;
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (uint8_t)(value);
}

void apply_edge_kernel_to_pixel(uint8_t* buffer_in, uint8_t* buffer_out, int xpos, int ypos, size_t width, size_t height) {

    // rgba: red, blue, green, alpha
    const size_t channels = 4;

    // buffer_in has image data stored in HWC format: 
    //  index = xpos * xstride + ypos * ystride + cpos * cstride
    //    where cpos = 0, since we are convolving the red channel only
    //    and where cstride = channels
    //    so index = x*xstride + y*ystride
    const int xstride = channels;
    const int ystride = channels * width;
    /*const int cstride = 1;*/

    // We're convolving on red channel values only.
    /*const int c = 0;*/

    // 3x3 edge kernel, and scale factor
    const int kernel[10] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1,
        1
    };

    int sum = 0;

    // keep a separate index for the kernel instead of using mod in the loop
    int kindex = 0;

    // loop through 3x3 submatrix
    for (int i = -1; i <= 1; ++i) {
        for (int k = -1; k <= 1; ++k) {
            int x_ = xpos + k;
            int y_ = ypos + i;
            // check bounds
            if (0 <= x_ && x_ < width && 0 <= y_ && y_ < height) {
                // calculate the index in the HWC formatted array
                int index = x_ * xstride + y_ * ystride;
                // calculate its contribution to the sum
                sum += (int)buffer_in[index] * (int)kernel[kindex];
            }
            ++kindex;
        }
    }

    // calculate the index in the HWC formatted array for the the pixel at xpos, ypos
    int index = xpos * xstride + ypos * ystride;

    // scale by the scale factor that is the last element of kernel[], and clamp to the range [0,256) ( i.e. [0,255] )
    uint8_t clampedSum = clamp(sum, kernel[kindex]);

    // assign the same value to all the color channels - namely, the sum computed from the red channels in 3x3 pixel matrix
    // centered at (x,y)
    buffer_out[index] = buffer_out[index+1] = buffer_out[index+2] = clampedSum;

    // assign max opacity to alpha channel
    buffer_out[index + 3] = 255;
}

void apply_edge_kernel_to_image(uint8_t* buffer_in, uint8_t* buffer_out, size_t width, size_t height) {
    for (int x = 0; x < (int)width; ++x)
        for (int y = 0; y < (int)height; ++y)
            apply_edge_kernel_to_pixel(buffer_in, buffer_out, x, y, width, height);
}
