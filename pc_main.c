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

void windowInit(void);
void windowTexSetup(void);
int windowIsLoaded(void);

void effInit(void);
void effTexSetup(void);
int effIsLoaded(void);

void mapInit(void);
int mapLoadPC(const char* map);

int msgLoadPC(const char* filename);

static void print_result(const char* name, int ok) {
    printf("[%s] %s\n", ok ? "OK" : "FAIL", name);
}

int main(void) {
    printf("TTYD PC runner starting\n");
    printf("tick: %u\n", PlatformGetTick());

    PlatformInputInit();
    DVDMgrInit();

    fadeInit();
    fadeTexSetup();
    print_result("fade texture", fadeIsLoaded());

    envInit();
    envTexSetup();
    print_result("env texture", envIsLoaded());

    windowInit();
    windowTexSetup();
    print_result("window texture", windowIsLoaded());

    effInit();
    effTexSetup();
    print_result("effect texture", effIsLoaded());

    mapInit();
    print_result("map aaa_00", mapLoadPC("aaa_00"));

    print_result("message hei_00", msgLoadPC("hei_00"));

    printf("PC runtime health check complete\n");

    return 0;
}
