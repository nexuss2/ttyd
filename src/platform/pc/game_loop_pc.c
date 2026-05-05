#include <stdio.h>

unsigned int PlatformGetTick(void);
void PlatformInputInit(void);
void DVDMgrInit(void);

void mapInit(void);
int mapLoadPC(const char* map);

int msgLoadPC(const char* filename);

int PCGameLoopRun(void) {
    unsigned int start;
    unsigned int now;
    int frame;

    printf("TTYD PC minimal game loop starting\n");

    PlatformInputInit();
    DVDMgrInit();

    mapInit();

    if (!mapLoadPC("aaa_00")) {
        printf("[FAIL] map load aaa_00\n");
        return 1;
    }

    if (!msgLoadPC("hei_00")) {
        printf("[FAIL] message load hei_00\n");
        return 1;
    }

    start = PlatformGetTick();

    for (frame = 0; frame < 10; frame++) {
        now = PlatformGetTick();
        printf("frame %d tick=%u elapsed=%u\n", frame, now, now - start);
    }

    printf("TTYD PC minimal game loop complete\n");

    return 0;
}
