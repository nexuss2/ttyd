#include <stdio.h>

typedef int BOOL;
typedef int s32;
typedef unsigned int u32;

typedef struct DVDFileInfo {
    unsigned char unused[0x34];
    u32 length;
    void* callback;
} DVDFileInfo;

#define TRUE 1
#define FALSE 0
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
    info->callback = 0;
    fclose(f);

    return TRUE;
}

s32 PlatformFileRead(const char* path, DVDFileInfo* info, void* address, u32 size, s32 offset) {
    FILE* f;
    char pc_path[512];
    (void)info;

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
    (void)info;
}
