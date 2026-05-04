#include <stdio.h>

typedef unsigned int u32;
typedef int s32;

typedef struct DVDFileInfo {
    unsigned char unused[0x34];
    u32 length;
    void* callback;
} DVDFileInfo;

int PlatformFileExists(const char* path);
int PlatformFileOpen(const char* path, DVDFileInfo* info);
s32 PlatformFileRead(const char* path, DVDFileInfo* info, void* address, u32 size, s32 offset);
void PlatformFileClose(DVDFileInfo* info);
unsigned int PlatformGetTick(void);

int main() {
    DVDFileInfo info;
    char buffer[64];

    printf("tick: %u\n", PlatformGetTick());

    if (!PlatformFileExists("/rel/aaa.rel")) {
        printf("file not found\n");
        return 0;
    }

    if (!PlatformFileOpen("/rel/aaa.rel", &info)) {
        printf("open failed\n");
        return 1;
    }

    PlatformFileRead("/rel/aaa.rel", &info, buffer, sizeof(buffer), 0);
    printf("read ok, length: %u, first byte: %02x\n", info.length, (unsigned char)buffer[0]);

    PlatformFileClose(&info);
    return 0;
}
