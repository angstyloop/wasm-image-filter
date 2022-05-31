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

/*
// TODO rewrite for uint8
void save_image_stb(image im, const char *name)
{
    char buff[256];
    sprintf(buff, "%s.jpg", name);
    unsigned char *data = calloc(im.w*im.h*im.c, sizeof(char));
    int i,k;
    for(k = 0; k < im.c; ++k){
        for(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned char) roundf((255*im.data[i + k*im.w*im.h]));
        }
    }
    //int success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    int success = stbi_write_jpg(buff, im.w, im.h, im.c, data, 100);
    free(data);
    if(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}
*/

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
    uint8_t* cwhData = stbi_load(filename, wptr, hptr, cptr, 0);

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
    char* filename="./a.jpg";
    int c=0, w=0, h=0;
    uint8_t* cwhData = load_image_stb_uint8(filename, &c, &w, &h);
    // Print channels, width, and height, formatted with 4 spaces after each
    // comma.
    printf("c=%d,%*c,w=%d,%*c,h=%d\n", c, 4, ' ', w, 4, ' ', h);

    for(int i=0; i<c; ++i)
    {
        for(int j=0; j<w; ++j)
        {
            for(int k=0; k<h; ++k)
            {
                const int index = i + j*c + k*c*w;
                printf("%u ", cwhData[index]);
            }
            printf("%*c", 4, ' '); // print 4 spaces
        }
        puts(""); // print newline
    }

    // Clean up.
    free(cwhData);

    puts("[0] PASS It runs.");

    return EXIT_SUCCESS;
}
#endif

#endif // end load_image_h
