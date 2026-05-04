#include <stdio.h>
#include <string.h>

typedef int BOOL;
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct DVDFileInfo {
    unsigned char unused[0x34];
    u32 length;
    void* callback;
} DVDFileInfo;

typedef void (*DVDCallback)(s32 result, DVDFileInfo* fileInfo);

typedef struct DVDEntry {
    char name[64];
    DVDFileInfo info;
    void* address;
    s32 bytesLeft;
    s32 offset;
    s32 position;
    DVDCallback callback;
    u16 status;
    u16 priority;
    u16 unknown;
    u8 pad_96[2];
} DVDEntry;

#define DVDEntryCount 0x100
#define DVDMGR_INUSE 0x8000
#define DVDMGR_FINISHED 0x0002

static DVDEntry dvdq[DVDEntryCount];

BOOL PlatformFileExists(const char* path);
BOOL PlatformFileOpen(const char* path, DVDFileInfo* info);
s32 PlatformFileRead(const char* path, DVDFileInfo* info, void* address, u32 size, s32 offset);
void PlatformFileClose(DVDFileInfo* info);

void DVDMgrInit(void) {
    memset(dvdq, 0, sizeof(dvdq));
}

DVDEntry* DVDMgrOpen(const char* path, u8 priority, u16 unknown) {
    DVDEntry* entry;
    int i;

    if (!PlatformFileExists(path)) {
        return NULL;
    }

    for (i = 0; i < DVDEntryCount; i++) {
        entry = &dvdq[i];
        if (!(entry->status & DVDMGR_INUSE)) break;
    }

    if (i >= DVDEntryCount) return NULL;

    memset(entry, 0, sizeof(DVDEntry));
    strcpy(entry->name, path);
    entry->priority = priority;
    entry->unknown = unknown;
    entry->status = DVDMGR_INUSE;

    if (!PlatformFileOpen(entry->name, &entry->info)) {
        memset(entry, 0, sizeof(DVDEntry));
        return NULL;
    }

    return entry;
}

s32 DVDMgrRead(DVDEntry* entry, void* address, u32 size, s32 offset) {
    s32 result = PlatformFileRead(entry->name, &entry->info, address, size, offset);
    entry->status |= DVDMGR_FINISHED;
    return result;
}

u32 DVDMgrGetLength(DVDEntry* entry) {
    return entry->info.length;
}

void DVDMgrClose(DVDEntry* entry) {
    PlatformFileClose(&entry->info);
    memset(entry, 0, sizeof(DVDEntry));
}
