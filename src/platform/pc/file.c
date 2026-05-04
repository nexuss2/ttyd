#include <dolphin/dvd.h>
#include <dolphin/types.h>

BOOL PlatformFileExists(const char* path) {
    return FALSE;
}

BOOL PlatformFileOpen(const char* path, DVDFileInfo* info) {
    return FALSE;
}

s32 PlatformFileRead(DVDFileInfo* info, void* address, u32 size, s32 offset) {
    return -1;
}

void PlatformFileClose(DVDFileInfo* info) {
}
