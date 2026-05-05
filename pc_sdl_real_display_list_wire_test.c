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

int main(void) {
    const unsigned char* data;
    u32 mesh = 0x01a2a0;
    u32 pos_base = 0x000380;
    u32 dl;
    u32 dl_len;
    const unsigned char* p;
    unsigned char cmd;
    int count;
    float x[64];
    float y[64];
    float z[64];
    int i;

    if (!mapLoadPC("aaa_00")) {
        return 1;
    }

    data = (const unsigned char*)mapGetPCData();

    dl = be32(data + 0x20 + mesh + 0x10);
    dl_len = be32(data + 0x20 + mesh + 0x14);

    printf("real mesh=%06x displayList=%06x len=%u\n", mesh, dl, dl_len);

    p = data + 0x20 + dl;
    cmd = p[0];
    count = be16u(p + 1);

    printf("GX cmd=%02x vertexCount=%d\n", cmd, count);

    if (cmd != 0x98 || count <= 0 || count > 64) {
        printf("unsupported display list\n");
        return 1;
    }

    p += 3;

    for (i = 0; i < count; i++) {
        int pos_index = be16u(p + 0);
        int normal_index = be16u(p + 2);
        int color_index = be16u(p + 4);
        int uv0_index = be16u(p + 6);
        int uv1_index = be16u(p + 8);

        get_pos(data, pos_base, pos_index, &x[i], &y[i], &z[i]);

        printf("v%d pos=%d normal=%d color=%d uv0=%d uv1=%d xyz=%f,%f,%f screen=%d,%d\n",
            i,
            pos_index,
            normal_index,
            color_index,
            uv0_index,
            uv1_index,
            x[i], y[i], z[i],
            sx(x[i], z[i]),
            sy(y[i], z[i]));

        p += 10;
    }

    if (!PCRenderSDLInit(800, 600)) {
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    SDL_SetRenderDrawColor(PCRenderSDLGetRenderer(), 255, 255, 255, 255);

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

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_real_display_list_wire_test.bmp");
    PCRenderSDLShutdown();

    return 0;
}
