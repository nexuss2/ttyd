#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_runtime_sdl.h"

typedef unsigned int u32;

int mapLoadPC(const char* map);
void* mapGetPCData(void);
u32 mapGetPCDataSize(void);

typedef struct PCMapLine {
    int x0;
    int y0;
    int x1;
    int y1;
} PCMapLine;

struct PCMapRuntime {
    const unsigned char* data;
    u32 size;
    u32 root_joint;
    PCMapLine* lines;
    int line_count;
    int line_capacity;
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

static float bef32(const unsigned char* p) {
    union {
        u32 u;
        float f;
    } v;

    v.u = be32(p);
    return v.f;
}

static void get_pos(const unsigned char* data, u32 pos_base, int index, float tx, float ty, float tz, float* x, float* y, float* z) {
    const unsigned char* p = data + 0x20 + pos_base + index * 6;

    *x = be16s(p + 0) / 100.0f + tx;
    *y = be16s(p + 2) / 100.0f + ty;
    *z = be16s(p + 4) / 100.0f + tz;
}

static int sx(float x, float z) {
    return 450 + (int)(x * 5.0f) + (int)(z * 1.5f);
}

static int sy(float y, float z) {
    return 620 - (int)(y * 3.0f) + (int)(z * 1.0f);
}

static void add_line(PCMapRuntime* map, int x0, int y0, int x1, int y1) {
    PCMapLine* next;

    if (map->line_count >= map->line_capacity) {
        int new_capacity = map->line_capacity ? map->line_capacity * 2 : 4096;
        next = (PCMapLine*)realloc(map->lines, sizeof(PCMapLine) * new_capacity);

        if (!next) {
            return;
        }

        map->lines = next;
        map->line_capacity = new_capacity;
    }

    map->lines[map->line_count].x0 = x0;
    map->lines[map->line_count].y0 = y0;
    map->lines[map->line_count].x1 = x1;
    map->lines[map->line_count].y1 = y1;
    map->lines[map->line_count].y1 = y1;
    map->line_count++;
}

static void cache_display_list(PCMapRuntime* map, u32 mesh, u32 pos_base, int dl_index, float tx, float ty, float tz) {
    u32 dl = be32(map->data + 0x20 + mesh + 0x10 + dl_index * 8);
    u32 dl_len = be32(map->data + 0x20 + mesh + 0x14 + dl_index * 8);
    const unsigned char* p;
    unsigned char cmd;
    int count;
    float x[128];
    float y[128];
    float z[128];
    int i;

    if (dl == 0 || dl_len == 0 || 0x20 + dl + dl_len > map->size) {
        return;
    }

    p = map->data + 0x20 + dl;
    cmd = p[0];
    count = be16u(p + 1);

    if (cmd != 0x98 || count <= 0 || count > 128) {
        return;
    }

    p += 3;

    for (i = 0; i < count; i++) {
        int pos_index = be16u(p + 0);
        get_pos(map->data, pos_base, pos_index, tx, ty, tz, &x[i], &y[i], &z[i]);
        p += 10;
    }

    for (i = 0; i < count; i++) {
        int k = (i + 1) % count;
        add_line(map, sx(x[i], z[i]), sy(y[i], z[i]), sx(x[k], z[k]), sy(y[k], z[k]));
    }
}

static void cache_mesh(PCMapRuntime* map, u32 mesh, float tx, float ty, float tz) {
    u32 vcd;
    u32 pos_base;
    u32 display_list_count;
    u32 i;

    if (!mesh || 0x20 + mesh + 0x10 > map->size) {
        return;
    }

    vcd = be32(map->data + 0x20 + mesh + 0x0c);
    if (!vcd || 0x20 + vcd + 4 > map->size) {
        return;
    }

    pos_base = be32(map->data + 0x20 + vcd + 0x00);
    display_list_count = be32(map->data + 0x20 + mesh + 0x04);

    if (display_list_count > 256) {
        return;
    }

    for (i = 0; i < display_list_count; i++) {
        cache_display_list(map, mesh, pos_base, i, tx, ty, tz);
    }
}

static void cache_joint_tree(PCMapRuntime* map, u32 joint, float parent_tx, float parent_ty, float parent_tz) {
    const unsigned char* j;
    u32 child;
    u32 next;
    u32 part_count;
    float tx;
    float ty;
    float tz;
    u32 i;

    if (!map || joint == 0 || 0x20 + joint + 0x68 > map->size) {
        return;
    }

    j = map->data + 0x20 + joint;

    child = be32(j + 0x0c);
    next = be32(j + 0x10);
    part_count = be32(j + 0x5c);

    tx = parent_tx + bef32(j + 0x30);
    ty = parent_ty + bef32(j + 0x34);
    tz = parent_tz + bef32(j + 0x38);

    if (part_count > 0 && part_count < 64) {
        for (i = 0; i < part_count; i++) {
            u32 part = 0x60 + i * 8;
            u32 mesh = be32(j + part + 4);
            cache_mesh(map, mesh, tx, ty, tz);
        }
    }

    cache_joint_tree(map, child, tx, ty, tz);
    cache_joint_tree(map, next, parent_tx, parent_ty, parent_tz);
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

    cache_joint_tree(out, out->root_joint, 0.0f, 0.0f, 0.0f);

    printf("loaded real map runtime %s root=%06x size=%u cachedLines=%d with joint translations\n",
        map_name,
        out->root_joint,
        out->size,
        out->line_count);

    return out;
}

void PCMapRuntimeDrawWire(PCMapRuntime* map) {
    int i;

    if (!map) {
        return;
    }

    SDL_SetRenderDrawColor(PCRenderSDLGetRenderer(), 255, 255, 255, 255);

    for (i = 0; i < map->line_count; i++) {
        SDL_RenderDrawLine(
            PCRenderSDLGetRenderer(),
            map->lines[i].x0,
            map->lines[i].y0,
            map->lines[i].x1,
            map->lines[i].y1
        );
    }
}

void PCMapRuntimeDestroy(PCMapRuntime* map) {
    if (!map) {
        return;
    }

    free(map->lines);
    free(map);
}
