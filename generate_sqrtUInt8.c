#include <stdlib.h>

int main() {
    for (int i=0, j=0; i<256; ++i) {
        if (i >= (j+1)*(j+1)) {
            ++j;
        }
        printf("[%d] = %d\n", i, j);
    }
    return 0;
}
