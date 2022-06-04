/*
clear && gcc -Wall -g -o test-create-test-image-with-vertical-cinematic-bars -Dtest_create_test_image_with_vertical_cinematic_bars create_test_image_with_vertical_cinematic_bars.c -lm && ./test-create-test-image-with-vertical-cinematic-bars > outfile.txt
*/

#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<assert.h>

#include "load_image_uint8.c"

/** Create a test image with vertical cinematic bars. The main color of the
 * image is white (0 0 0 255). The color of the cinematic bars is black
 * (255 255 255 255). The cinematic bars start on the left and right edges.
 *
 * @param width - The image width in pixels.
 * @param height - The image height in pixels.
 * @param barwidth - The width of the cinematic bar in pixels.
 *
 * @return A pointer to the image data, a dynamically allocated array of uint8_t
 * that must be freed. The pixel data is in CWH format. 
 */

uint8_t* create_test_image_with_vertical_cinematic_bars(size_t channels,
                                                        size_t width,
                                                        size_t height,
                                                        size_t barwidth)
{
    assert(channels==3 || channels==4);

    uint8_t* a = calloc(channels*width*height, sizeof(uint8_t));

    const int alpha = (channels==4)?1:0;

    for (size_t y=0; y<height; ++y)
    {
        for(size_t x=0; x<width; ++x)
        {
            const size_t r = x*channels + y*width*channels;
            for(size_t z=0; z<3; ++z) //loop over rgb channels
            {
                a[r+z] = (x<barwidth || x>width-barwidth-1) ? 0 : 255;
            }
            if(alpha){ //alpha channel (opacity), if present
                a[r+3] = 255; //opacity
            }
        }
    }

    return a;
}

/** Print @a an array of uint8_t (8-bit unsigned integers), of size @n. Print
 * the separator @s in between each value. Instead of a separator, print a
 * newline after the final value.
 *
 * @param n - Size of the array.
 * @param a - The array of uint8_t (8-bit unsigned integers).
 * @param s - The separator string.
 */
void print_array_uint8(size_t n, uint8_t* a, const char s[1]) {
    for (size_t i=0; i<n-1; ++i)
        printf("%u%s", a[i], s);
    printf("%u\n", a[n-1]);
}


/** Print @a a 2-d array of 8-bit unsigned integer pixel channel values. 
 *
 * @param w - The width of the array in pixels.
 * @param h - The height of the array in pixels.
 * @param a - An array of size 4*w*h.
 * @param s1 - Separator between channel values of a single pixel.
 * @param s2 - Separator between columns of pixels.
 * @param s3 - Separator between rows of pixels.
 * 
 */
void print_2d_pixel_array_uint8(size_t c,
                                size_t w,
                                size_t h,
                                uint8_t* a,
                                const char s1[1],
                                const char s2[1],
                                const char s3[1])
{
    assert(w>0);
    assert(h>0);
    assert(c==3 || c==4);

    const int alpha = (c==4)?1:0;

    for(size_t y=0; y<h; ++y)
    {
        for(size_t x=0; x<w; ++x)
        {
            const size_t r = x*c + y*c*w;
            for(size_t z=0; z<3; ++z)
            {
                printf("%u", a[r+z]);
                printf("%s", s1);
            }
            if(alpha){
                printf("%u", a[r+3]);
            }
            printf("%s", s2);
        }
        printf("%s", s3);
    }
}

// TEST

#ifdef test_create_test_image_with_vertical_cinematic_bars

int main(){
    int channels=3,
        width=100,
        height=100,
        barwidth=30;

    uint8_t* cwhData = create_test_image_with_vertical_cinematic_bars(channels,
                                                                      width,
                                                                      height,
                                                                      barwidth);

    //print_array_uint8(4*width*height, data, " ").
    //print_2d_pixel_array_uint8(channels, width, height, cwhData, " ", "\t", "\n");

    // Use pixel data to create image.
    save_image_stb_uint8(cwhData, channels, width, height, "bars");

    // Load that image back into data. 
    int channels2=0, width2=0, height2=0;
    uint8_t* cwhData2 = load_image_stb_uint8("bars.jpg", &channels2, &width2, &height2);

    //printf("channels2=%d, width2=%d, height2=%d\n", channels2, width2, height2);

    //print_2d_pixel_array_uint8(channels2, width2, height2, cwhData2, " ", "\t", "\n");

    // Clean up.
    free(cwhData);
    free(cwhData2);
}

#endif // end test_create_test_image_with_vertical_cinematic_bars
