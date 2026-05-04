#include <stdio.h>

unsigned int PlatformGetTick(void);
void PlatformInputInit(void);

void DVDMgrInit(void);

void fadeInit(void);
void fadeTexSetup(void);
int fadeIsLoaded(void);

void envInit(void);
void envTexSetup(void);
int envIsLoaded(void);

int main(void) {
    printf("TTYD PC runner starting\n");

    PlatformInputInit();
    DVDMgrInit();

    printf("tick: %u\n", PlatformGetTick());

    fadeInit();
    fadeTexSetup();
    printf("fade loaded: %d\n", fadeIsLoaded());

    envInit();
    envTexSetup();
    printf("env loaded: %d\n", envIsLoaded());

    return 0;
}
