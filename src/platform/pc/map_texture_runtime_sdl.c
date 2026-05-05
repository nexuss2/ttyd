#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "platform/pc/texture_runtime_sdl.h"

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int mapLoadPC(const char* map);
int mapWriteTextureNamesPC(const char* map, const char* out_path);

typedef struct PCMapTextureSet {
    PCTextureRuntime* textures[64];
    char names[64][64];
    int count;
} PCMapTextureSet;

static int load_names(PCMapTextureSet* set, const char* map) {
    char temp_path[256];
    FILE* f;
    int index;
    char name[64];

    sprintf(temp_path, "build/pc/%s_runtime_texture_names.txt", map);

    if (!mapLoadPC(map)) {
        return 0;
    }

    if (!mapWriteTextureNamesPC(map, temp_path)) {
        return 0;
    }

    f = fopen(temp_path, "r");
    if (!f) {
        return 0;
    }

    while (fscanf(f, "%d %63s", &index, name) == 2) {
        if (index >= 0 && index < 64) {
            strncpy(set->names[index], name, sizeof(set->names[index]) - 1);
            set->names[index][sizeof(set->names[index]) - 1] = 0;
        }
    }

    fclose(f);
    return 1;
}

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

    load_names(set, map);

    for (i = 0; i < 64; i++) {
        set->textures[i] = PCTextureRuntimeCreateFromTPLCMPR(tpl, length, i);
        if (set->textures[i]) {
            set->count++;
        }
    }

    printf("map texture set %s loaded %d textures\n", map, set->count);
    return set;
}

int PCMapTextureSetFindIndex(PCMapTextureSet* set, const char* name) {
    int i;

    if (!set || !name) {
        return -1;
    }

    for (i = 0; i < 64; i++) {
        if (set->names[i][0] && strcmp(set->names[i], name) == 0) {
            return i;
        }
    }

    return -1;
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

void PCMapTextureSetDrawNamed(PCMapTextureSet* set, const char* name, int x, int y, int draw_width, int draw_height) {
    int index;

    if (!set || !name) {
        return;
    }

    index = PCMapTextureSetFindIndex(set, name);
    if (index < 0 || index >= 64 || !set->textures[index]) {
        printf("texture name not found: %s\n", name);
        return;
    }

    PCTextureRuntimeDraw(set->textures[index], x, y, draw_width, draw_height);
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
