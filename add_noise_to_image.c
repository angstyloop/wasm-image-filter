#include<stdint.h>
#include <time.h>
#include <stdlib.h>

#include "load_image_uint8.c"

void add_noise_to_image(uint8_t* data, int x0, int y0, int c, int w, int h, uint8_t a)
{
    srand(time(NULL));

    for(int y=0; y<h; ++y)
    {
        for(int x=0; x<w; ++x)
        {
            for(int z=0; z<3; ++z)
            {
                // index for WHC order (x has longest stride, and z (channel)
                // has shortest).
                const i = z + x*c + y*c*w;

                // random
                uint16_t r = rand();

                // use leftmost bit as parity
                uint16_t p = r & 256;

                //modulo remaining bits by amplitude
                uint8_t d = (uint8_t)(r/2) % a;

                // for positive parity add the delta
                if(p>0)
                { 
                    data[i]+=d;
                }

                // for negative parity subtract the delta, clamping at zero
                else
                { 
                    if(data[i]>d)
                    {
                        data[i]-=d;
                    }
                    else
                    {
                        data[i]=0;
                    }
                }
            }
        }
    }
}

int main() {
    int x0=0, y0=0; // origin (x0, y0)
    int c = 3; // channels
    int w = 0; // width
    int h = 0; // height
    float a = 10; // amplitude
    uint8_t* data = load_image_stb_uint8("in.jpg", &c, &w, &h);
    add_noise_to_image(data, i, x0, y0, c, w, h, a);
    save_image_stb_uint8(data, c, w, h, "out")
    free(data);
    return 0;
}
