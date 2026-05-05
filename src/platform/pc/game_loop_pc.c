#include <stdio.h>

unsigned int PlatformGetTick(void);
void PlatformInputInit(void);
void DVDMgrInit(void);

void mapInit(void);
int mapLoadPC(const char* map);

int msgLoadPC(const char* filename);

int PCRenderInit(int width, int height);
void PCRenderBeginFrame(void);
void PCRenderClear(void);
void PCRenderEndFrame(void);
void PCRenderShutdown(void);

int PCGameLoopRunWithContent(const char* map, const char* msg) {
    unsigned int start;
    unsigned int now;
    int frame;

    printf("TTYD PC minimal game loop starting\n");
    printf("map=%s msg=%s\n", map, msg);

    PlatformInputInit();
    DVDMgrInit();

    mapInit();

    if (!mapLoadPC(map)) {
        printf("[FAIL] map load %s\n", map);
        return 1;
    }

    if (!msgLoadPC(msg)) {
        printf("[FAIL] message load %s\n", msg);
        return 1;
    }

    if (!PCRenderInit(640, 480)) {
        printf("[FAIL] render init\n");
        return 1;
    }

    start = PlatformGetTick();

    for (frame = 0; frame < 10; frame++) {
        now = PlatformGetTick();
        printf("frame %d tick=%u elapsed=%u\n", frame, now, now - start);

        PCRenderBeginFrame();
        PCRenderClear();
        PCRenderEndFrame();
    }

    PCRenderShutdown();

    printf("TTYD PC minimal game loop complete\n");

    return 0;
}

int PCGameLoopRun(void) {
    return PCGameLoopRunWithContent("aaa_00", "hei_00");
}
