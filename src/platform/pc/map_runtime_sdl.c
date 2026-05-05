#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_runtime_sdl.h"

typedef unsigned int u32;

int mapLoadPC(const char* map);
void* mapGetPCData(void);
u32 mapGetPCDataSize(void);

typedef struct PCMapDrawPart {
    u32 material;
    u32 mesh;
} PCMapDrawPart;

struct PCMapRuntime {
    const unsigned char* data;
    u32 size;
    u32 root_joint;
    PCMapDrawPart* parts;
    int part_count;
    int part_capacity;
};

static u32 be32(const unsigned char* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}

static unsigned short be16u(const unsigned char* p) {
    return (unsigned short)(((unsigned short)p[0] << 8) | p[1]);
}

static short be16s(const unsigned char* p) {
    return (short)(((unsigned short)p[0] << 8) | p[1]);
}

static void get_pos(const unsigned char* data, u32 pos_base, int index, float* x, float* y, float* z) {
    const unsigned char* p = data + 0x20 + pos_base + index * 6;

    *x = be16s(p + 0) / 100.0f;
    *y = be16s(p + 2) / 100.0f;
    *z = be16s(p + 4) / 100.0f;
}

static int sx(float x, float z) {
    return 450 + (int)(x * 5.0f) + (int)(z * 1.5f);
}

static int sy(float y, float z) {
    return 620 - (int)(y * 3.0f) + (int)(z * 1.0f);
}

static void add_part(PCMapRuntime* map, u32 material, u32 mesh) {
    PCMapDrawPart* next;

    if (map->part_count >= map->part_capacity) {
        int new_capacity = map->part_capacity ? map->part_capacity * 2 : 256;
        next = (PCMapDrawPart*)realloc(map->parts, sizeof(PCMapDrawPart) * new_capacity);

        if (!next) {
            return;
        }

        map->parts = next;
        map->part_capacity = new_capacity;
    }

    map->parts[map->part_count].material = material;
    map->parts[map->part_count].mesh = mesh;
    map->part_count++;
}

static void build_part_cache_from_joint(PCMapRuntime* map, u32 joint) {
    const unsigned char* j;
    u32 child;
    u32 next;
    u32 part_count;
    u32 i;

    if (!map || joint == 0 || 0x20 + joint + 0x68 > map->size) {
        return;
    }

    j = map->data + 0x20 + joint;
    child = be32(j + 0x0c);
    next = be32(j + 0x10);
    part_count = be32(j + 0x5c);

    if (part_count > 0 && part_count < 64) {
        for (i = 0; i < part_count; i++) {
            u32 part = 0x60 + i * 8;
            u32 material = be32(j + part + 0);
            u32 mesh = be32(j + part + 4);

            if (mesh) {
                add_part(map, material, mesh);
            }
        }
    }

    build_part_cache_from_joint(map, child);
    build_part_cache_from_joint(map, next);
}

static void draw_display_list(const unsigned char* data, u32 mesh, u32 pos_base, int dl_index) {
    u32 dl = be32(data + 0x20 + mesh + 0x10 + dl_index * 8);
    u32 dl_len = be32(data + 0x20 + mesh + 0x14 + dl_index * 8);
    const unsigned char* p = data + 0x20 + dl;
    unsigned char cmd = p[0];
    int count = be16u(p + 1);
    float x[128];
    float y[128];
    float z[128];
    int i;

    if (dl == 0 || dl_len == 0 || cmd != 0x98 || count <= 0 || count > 128) {
        return;
    }

    p += 3;

    for (i = 0; i < count; i++) {
        int pos_index = be16u(p + 0);
        get_pos(data, pos_base, pos_index, &x[i], &y[i], &z[i]);
        p += 10;
    }

    for (i = 0; i < count; i++) {
        int k = (i + 1) % count;
        SDL_RenderDrawLine(PCRenderSDLGetRenderer(), sx(x[i], z[i]), sy(y[i], z[i]), sx(x[k], z[k]), sy(y[k], z[k]));
    }
}

static void draw_mesh(const unsigned char* data, u32 mesh) {
    u32 vcd = be32(data + 0x20 + mesh + 0x0c);
    u32 pos_base = be32(data + 0x20 + vcd + 0x00);
    u32 display_list_count = be32(data + 0x20 + mesh + 0x04);
    u32 i;

    if (display_list_count > 256) {
        return;
    }

    for (i = 0; i < display_list_count; i++) {
        draw_display_list(data, mesh, pos_base, i);
    }
}

PCMapRuntime* PCMapRuntimeLoad(const char* map_name) {
    PCMapRuntime* out;

    if (!mapLoadPC(map_name)) {
        return 0;
    }

    out = (PCMapRuntime*)calloc(1, sizeof(PCMapRuntime));
    if (!out) {
        return 0;
    }

    out->data = (const unsigned char*)mapGetPCData();
    out->size = mapGetPCDataSize();
    out->root_joint = be32(out->data + 0x20 + 4);

    build_part_cache_from_joint(out, out->root_joint);

    printf("loaded real map runtime %s root=%06x size=%u cachedParts=%d\n",
        map_name,
        out->root_joint,
        out->size,
        out->part_count);

    return out;
}

void PCMapRuntimeDrawWire(PCMapRuntime* map) {
    int i;

    if (!map) {
        return;
    }

    SDL_SetRenderDrawColor(PCRenderSDLGetRenderer(), 255, 255, 255, 255);

    for (i = 0; i < map->part_count; i++) {
        draw_mesh(map->data, map->parts[i].mesh);
    }
}

void PCMapRuntimeDestroy(PCMapRuntime* map) {
    if (!map) {
        return;
    }

    free(map->parts);
    free(map);
}
