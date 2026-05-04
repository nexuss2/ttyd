#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static const char* remap_path(const char* filename) {
    if (strcmp(filename, "effect.tpl") == 0) return "/e/jp/effect.tpl";
    if (strcmp(filename, "font/msgWindow.tpl") == 0) return "/f/msgWindow.tpl";
    if (strcmp(filename, "env.tpl") == 0) return "/env.tpl";
    if (strcmp(filename, "fade.tpl") == 0) return "/fade.tpl";
    return filename;
}

void arcInit(void) {
}

void arcEntry(s32 id, void* data, u32 size) {
    (void)id;
    (void)data;
    (void)size;
}

void* arcOpen(const char* filename, void** addr, u32* length) {
    const char* path;
    DVDFileInfo info;
    void* data;

    path = remap_path(filename);

    if (!PlatformFileExists(path)) {
        return NULL;
    }

    if (!PlatformFileOpen(path, &info)) {
        return NULL;
    }

    data = malloc(info.length);
    if (!data) {
        return NULL;
    }

    if (PlatformFileRead(path, &info, data, info.length, 0) < 0) {
        free(data);
        return NULL;
    }

    if (addr) {
        *addr = data;
    }

    if (length) {
        *length = info.length;
    }

    return data;
}
