#include <stdio.h>
#include <SDL.h>
#include "platform/pc/render_sdl_pc.h"

typedef unsigned int u32;

int mapLoadPC(const char* map);
void* mapGetPCData(void);
u32 mapGetPCDataSize(void);

static u32 be32(const unsigned char* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}

static unsigned short be16u(const unsigned char* p) {
    return (unsigned short)(((unsigned short)p[0] << 8) | p[1]);
}

static short be16s(const unsigned char* p) {
    return (short)(((unsigned short)p[0] << 8) | p[1]);
}

static const char* str_at(const unsigned char* data, u32 size, u32 target) {
    if (target == 0 || 0x20 + target >= size) {
        return "<null>";
    }

    return (const char*)(data + 0x20 + target);
}

static void get_pos(const unsigned char* data, u32 pos_base, int index, float* x, float* y, float* z) {
    const unsigned char* p = data + 0x20 + pos_base + index * 6;

    *x = be16s(p + 0) / 100.0f;
    *y = be16s(p + 2) / 100.0f;
    *z = be16s(p + 4) / 100.0f;
}

static int sx(float x, float z) {
    return 400 + (int)(x * 5.0f) + (int)(z * 1.5f);
}

static int sy(float y, float z) {
    return 540 - (int)(y * 3.0f) + (int)(z * 1.0f);
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

static void draw_mesh(const unsigned char* data, u32 size, u32 material, u32 mesh, const char* name) {
    u32 vcd = be32(data + 0x20 + mesh + 0x0c);
    u32 pos_base = be32(data + 0x20 + vcd + 0x00);
    u32 display_list_count = be32(data + 0x20 + mesh + 0x04);
    u32 i;

    printf("draw real mesh name=%s material=%06x mesh=%06x posBase=%06x displayLists=%u\n",
        name, material, mesh, pos_base, display_list_count);

    for (i = 0; i < display_list_count; i++) {
        draw_display_list(data, mesh, pos_base, i);
    }
}

static void walk_and_draw(const unsigned char* data, u32 size, u32 joint, int* drawn) {
    const unsigned char* j;
    u32 name_ptr;
    u32 child;
    u32 next;
    u32 part_count;
    u32 i;

    if (*drawn >= 20 || joint == 0 || 0x20 + joint + 0x68 > size) {
        return;
    }

    j = data + 0x20 + joint;
    name_ptr = be32(j + 0x00);
    child = be32(j + 0x0c);
    next = be32(j + 0x10);
    part_count = be32(j + 0x5c);

    if (part_count > 0) {
        for (i = 0; i < part_count && *drawn < 20; i++) {
            u32 part = 0x60 + i * 8;
            u32 material = be32(j + part + 0);
            u32 mesh = be32(j + part + 4);

            draw_mesh(data, size, material, mesh, str_at(data, size, name_ptr));
            (*drawn)++;
        }
    }

    walk_and_draw(data, size, child, drawn);
    walk_and_draw(data, size, next, drawn);
}

int main(void) {
    const unsigned char* data;
    u32 size;
    u32 root_joint;
    int drawn = 0;

    if (!mapLoadPC("aaa_00")) {
        return 1;
    }

    data = (const unsigned char*)mapGetPCData();
    size = mapGetPCDataSize();
    root_joint = be32(data + 0x20 + 4);

    if (!PCRenderSDLInit(900, 700)) {
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    SDL_SetRenderDrawColor(PCRenderSDLGetRenderer(), 255, 255, 255, 255);
    walk_and_draw(data, size, root_joint, &drawn);

    printf("drawn real mesh parts=%d\n", drawn);

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_real_20_meshes_wire_test.bmp");
    PCRenderSDLShutdown();

    return 0;
}
