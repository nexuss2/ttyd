#include <stdio.h>

unsigned int PlatformGetTick(void);
void PlatformInputInit(void);

int main(void) {
    printf("TTYD PC runner starting\n");

    PlatformInputInit();

    printf("tick: %u\n", PlatformGetTick());
    printf("PC backend initialized\n");

    return 0;
}
