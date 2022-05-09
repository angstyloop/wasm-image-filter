#include <stdio.h>

int main() {
    for (int i=0, j=0; i<65536; ++i) {
        if (i >= (j+1)*(j+1)) {
            ++j;
        }
        printf("[%d] = %d\n", i, j);
    }
    return 0;
}
