#include <stdio.h>

typedef unsigned int u32;
typedef int s32;
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct DVDFileInfo {
    unsigned char unused[0x34];
    u32 length;
    void* callback;
} DVDFileInfo;

typedef struct DVDEntry DVDEntry;

unsigned int PlatformGetTick(void);
void PlatformInputInit(void);

void DVDMgrInit(void);
DVDEntry* DVDMgrOpen(const char* path, u8 priority, u16 unknown);
s32 DVDMgrRead(DVDEntry* entry, void* address, u32 size, s32 offset);
u32 DVDMgrGetLength(DVDEntry* entry);
void DVDMgrClose(DVDEntry* entry);

int main(void) {
    DVDEntry* entry;
    unsigned char buffer[64];

    printf("TTYD PC runner starting\n");

    PlatformInputInit();
    DVDMgrInit();

    printf("tick: %u\n", PlatformGetTick());

    entry = DVDMgrOpen("/rel/aaa.rel", 2, 0);
    if (!entry) {
        printf("DVDMgrOpen failed\n");
        return 1;
    }

    printf("DVD length: %u\n", DVDMgrGetLength(entry));

    if (DVDMgrRead(entry, buffer, sizeof(buffer), 0) < 0) {
        printf("DVDMgrRead failed\n");
        return 1;
    }

    printf("DVD read ok, first byte: %02x\n", buffer[0]);

    DVDMgrClose(entry);

    return 0;
}
