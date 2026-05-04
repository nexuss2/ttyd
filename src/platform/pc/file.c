#include <dolphin/dvd.h>
#include <dolphin/types.h>
#include <stdio.h>
#include <string.h>

#define PC_GAME_FILES_ROOT "orig/G8MJ01/files"

static void make_pc_path(char* out, const char* path) {
    if (path[0] == '/') {
        sprintf(out, "%s%s", PC_GAME_FILES_ROOT, path);
    } else {
        sprintf(out, "%s/%s", PC_GAME_FILES_ROOT, path);
    }
}

BOOL PlatformFileExists(const char* path) {
    FILE* f;
    char pc_path[512];

    make_pc_path(pc_path, path);
    f = fopen(pc_path, "rb");
    if (!f) {
        return FALSE;
    }

    fclose(f);
    return TRUE;
}

BOOL PlatformFileOpen(const char* path, DVDFileInfo* info) {
    FILE* f;
    char pc_path[512];

    make_pc_path(pc_path, path);
    f = fopen(pc_path, "rb");
    if (!f) {
        return FALSE;
    }

    fseek(f, 0, SEEK_END);
    info->length = (u32)ftell(f);
    info->startAddr = 0;
    info->callback = NULL;
    fclose(f);

    return TRUE;
}

s32 PlatformFileRead(const char* path, DVDFileInfo* info, void* address, u32 size, s32 offset) {
    FILE* f;
    char pc_path[512];

    make_pc_path(pc_path, path);

    f = fopen(pc_path, "rb");
    if (!f) {
        return -1;
    }

    fseek(f, offset, SEEK_SET);
    fread(address, 1, size, f);
    fclose(f);

    return size;
}

void PlatformFileClose(DVDFileInfo* info) {
}
