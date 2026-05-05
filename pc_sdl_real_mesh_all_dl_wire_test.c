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

static void get_pos(const unsigned char* data, u32 pos_base, int index, float* x, float* y, float* z) {
    const unsigned char* p = data + 0x20 + pos_base + index * 6;

    *x = be16s(p + 0) / 100.0f;
    *y = be16s(p + 2) / 100.0f;
    *z = be16s(p + 4) / 100.0f;
}

static int sx(float x, float z) {
    return 400 + (int)(x * 8.0f) + (int)(z * 2.0f);
}

static int sy(float y, float z) {
    return 520 - (int)(y * 4.0f) + (int)(z * 1.0f);
}

static void draw_display_list(const unsigned char* data, u32 mesh, u32 pos_base, int dl_index) {
    u32 dl = be32(data + 0x20 + mesh + 0x10 + dl_index * 8);
    u32 dl_len = be32(data + 0x20 + mesh + 0x14 + dl_index * 8);
    const unsigned char* p = data + 0x20 + dl;
    unsigned char cmd = p[0];
    int count = be16u(p + 1);
    float x[64];
    float y[64];
    float z[64];
    int i;

    printf("DL%d ptr=%06x len=%u cmd=%02x count=%d\n", dl_index, dl, dl_len, cmd, count);

    if (cmd != 0x98 || count <= 0 || count > 64) {
        printf("skip unsupported display list %d\n", dl_index);
        return;
    }

    p += 3;

    for (i = 0; i < count; i++) {
        int pos_index = be16u(p + 0);
        int normal_index = be16u(p + 2);
        int color_index = be16u(p + 4);
        int uv0_index = be16u(p + 6);
        int uv1_index = be16u(p + 8);

        get_pos(data, pos_base, pos_index, &x[i], &y[i], &z[i]);

        printf("  v%d pos=%d normal=%d color=%d uv0=%d uv1=%d xyz=%f,%f,%f\n",
            i, pos_index, normal_index, color_index, uv0_index, uv1_index, x[i], y[i], z[i]);

        p += 10;
    }

    for (i = 0; i < count; i++) {
        int j = (i + 1) % count;
        SDL_RenderDrawLine(
            PCRenderSDLGetRenderer(),
            sx(x[i], z[i]),
            sy(y[i], z[i]),
            sx(x[j], z[j]),
            sy(y[j], z[j])
        );
    }
}

int main(void) {
    const unsigned char* data;
    u32 mesh = 0x01a2a0;
    u32 pos_base = 0x000380;
    u32 display_list_count;
    u32 i;

    if (!mapLoadPC("aaa_00")) {
        return 1;
    }

    data = (const unsigned char*)mapGetPCData();
    display_list_count = be32(data + 0x20 + mesh + 0x04);

    printf("REAL MAP aaa_00 mesh=%06x displayListCount=%u\n", mesh, display_list_count);

    if (!PCRenderSDLInit(800, 600)) {
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    SDL_SetRenderDrawColor(PCRenderSDLGetRenderer(), 255, 255, 255, 255);

    for (i = 0; i < display_list_count; i++) {
        draw_display_list(data, mesh, pos_base, i);
    }

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_real_mesh_all_dl_wire_test.bmp");
    PCRenderSDLShutdown();

    return 0;
}
