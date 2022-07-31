#include <stdio.h>   // printf, fflush
#include <unistd.h>  // sleep

int main() {
    printf("Hello, World!\n");

    for (int loop = 0; loop < 4; ++loop) {
        for (int each = 0; each < 4; ++each) {
            printf("\rloading%.*s   \b\b\b", each, "...");
            fflush(stdout);  // force printing as no newline in output
            sleep(1);
        }
    }
    printf("\n");

    return 0;
}
