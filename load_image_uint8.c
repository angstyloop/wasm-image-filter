/*
clear && gcc -Wall -g -o test-load-image-uint8 -Dtest_load_image_uint8 load_image_uint8.c -lm && ./test-load-image-uint8
*/

#ifndef load_image_h
#define load_image_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/**
 * @param cwhData - The pixel data as an array of 8 bit unsigned integers, in
 *                  CWH format.
 *
 * @param name - The desired name of the JPG file, without an extension.
 *
 * @param c - The number of channels.
 *
 * @param w - The width in pixels.
 * 
 * @param h - The height in pixels.
 */
void save_image_stb_uint8(uint8_t* cwhData, int c, int w, int h, const char *name)
{
    char namebuf[256];
    sprintf(namebuf, "%s.jpg", name);
    //int success = stbi_write_png(namebuf, w, h, c, data, w*c);
    int success = stbi_write_jpg(namebuf, c, w, h, cwhData, 100);
    free(cwhData);
    if(!success){
        fprintf(stderr, "Failed to write image %s\n", namebuf);
    }
}

/** Use STB to load an image from a JPEG file and put it in an RGBA array of
 * 8-bit unsigned integers in CWH format (linear rgb). This "flattens" the
 * image pixel data while respecting the orientation. In this format, the
 * channel index has the shortest stride (1), and the height index has the
 * longest stride (width*channels).
 *
 * @param filename - Name of the file to load
 *
 * @param cptr - Pointer to the number of channels, to be set by the
 * function.
 *
 * @param wptr - Pointer to the image width in pixels, to be set by the
 * function
 *
 * @param hptr - Pointer to the image height in pixels, to be set by the
 * function.
 *
 * @return An unsigned 8-bit integer array of pixels in linear RGBA order.
 */
uint8_t* load_image_stb_uint8(
        char* filename,
        int* cptr,
        int* wptr,
        int* hptr)
{
    uint8_t* cwhData = stbi_load(filename, cptr, wptr, hptr, 0);

    if(!cwhData){
        fprintf(stderr, "Cannot load image \"%s\"STB Reason: %s\n",
                filename, stbi_failure_reason());
        exit(EXIT_FAILURE);
    }

    return cwhData;
}

#ifdef test_load_image_uint8
#include<assert.h>
int main()
{
    // Load image.

    char* filename="./a.jpg";
    int c=0, w=0, h=0;
    uint8_t* cwhData = load_image_stb_uint8(filename, &c, &w, &h);
    // Print channels, width, and height, formatted with 4 spaces after each
    // comma.
    printf("c=%d,%*cw=%d,%*ch=%d\n", c, 4, ' ', w, 4, ' ', h);

    for(int i=0; i<c; ++i)
    {
        for(int j=0; j<w; ++j)
        {
            for(int k=0; k<h; ++k)
            {
                const int index = i + j*c + k*c*w;
                //printf("%u ", cwhData[index]);
            }
            //printf("%*c", 4, ' '); // print 4 spaces
        }
        //puts(""); // print newline
    }

    // Save image.
    save_image_stb_uint8(cwhData, c, w, h, "b");

    // Load the image just saved.
    uint8_t* cwhData2 = load_image_stb_uint8("./b.jpg", &c, &w, &h);

    // Assert the final image matches the original pixel by pixel.
    for (int i=0; i<c*w*h; ++i)
    {
        assert(cwhData[i] == cwhData2[i]);
    }

    // Clean up.
    free(cwhData);
    free(cwhData2);

    puts("[0] PASS It runs.");

    return EXIT_SUCCESS;
}
#endif

#endif // end load_image_h
