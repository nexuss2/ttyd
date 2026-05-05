#include <stdlib.h>
#include <stdio.h>
#include "platform/pc/texture_runtime_sdl.h"

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

typedef struct PCMapTextureSet {
    PCTextureRuntime* textures[64];
    int count;
} PCMapTextureSet;

PCMapTextureSet* PCMapTextureSetLoad(const char* map) {
    char path[128];
    u32 length = 0;
    void* tpl;
    PCMapTextureSet* set;
    int i;

    sprintf(path, "./m/%s/t", map);

    tpl = arcOpen(path, 0, &length);
    if (!tpl) {
        printf("failed to load map texture palette %s\n", path);
        return 0;
    }

    set = (PCMapTextureSet*)calloc(1, sizeof(PCMapTextureSet));
    if (!set) {
        return 0;
    }

    for (i = 0; i < 64; i++) {
        set->textures[i] = PCTextureRuntimeCreateFromTPLCMPR(tpl, length, i);
        if (set->textures[i]) {
            set->count++;
        }
    }

    printf("map texture set %s loaded %d textures\n", map, set->count);
    return set;
}

void PCMapTextureSetDrawSheet(PCMapTextureSet* set, int x, int y) {
    int i;

    if (!set) {
        return;
    }

    for (i = 0; i < 64; i++) {
        int col = i % 6;
        int row = i / 6;

        if (set->textures[i]) {
            PCTextureRuntimeDraw(set->textures[i], x + col * 150, y + row * 110, 96, 96);
        }
    }
}

void PCMapTextureSetDestroy(PCMapTextureSet* set) {
    int i;

    if (!set) {
        return;
    }

    for (i = 0; i < 64; i++) {
        PCTextureRuntimeDestroy(set->textures[i]);
    }

    free(set);
}
