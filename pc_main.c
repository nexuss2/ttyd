#include <stdio.h>

unsigned int PlatformGetTick(void);
void PlatformInputInit(void);

void DVDMgrInit(void);
void fadeInit(void);
void fadeTexSetup(void);
int fadeIsLoaded(void);

int main(void) {
    printf("TTYD PC runner starting\n");

    PlatformInputInit();
    DVDMgrInit();

    printf("tick: %u\n", PlatformGetTick());

    fadeInit();
    fadeTexSetup();

    printf("fade loaded: %d\n", fadeIsLoaded());

    return 0;
}
