#include <stdio.h>
#include <SDL.h>
#include "platform/pc/render_sdl_pc.h"

typedef unsigned int u32;

int mapLoadPC(const char* map);
void* mapGetPCData(void);
u32 mapGetPCDataSize(void);

static short be16s(const unsigned char* p) {
    return (short)(((unsigned short)p[0] << 8) | p[1]);
}

static void get_pos(const unsigned char* data, u32 pos_base, int index, float* x, float* y, float* z) {
    const unsigned char* p = data + 0x20 + pos_base + index * 6;

    *x = be16s(p + 0) / 100.0f;
    *y = be16s(p + 2) / 100.0f;
    *z = be16s(p + 4) / 100.0f;
}

static int sx(float x) {
    return 360 + (int)(x * 8.0f);
}

static int sy(float y) {
    return 520 - (int)(y * 4.0f);
}

int main(void) {
    const unsigned char* data;
    u32 pos_base = 0x000380;
    int indices[4] = {8, 9, 2, 0};
    float x[4];
    float y[4];
    float z[4];
    int i;

    if (!mapLoadPC("aaa_00")) {
        return 1;
    }

    data = (const unsigned char*)mapGetPCData();

    for (i = 0; i < 4; i++) {
        get_pos(data, pos_base, indices[i], &x[i], &y[i], &z[i]);
        printf("v%d posIndex=%d xyz=%f %f %f screen=%d,%d\n",
            i, indices[i], x[i], y[i], z[i], sx(x[i]), sy(y[i]));
    }

    if (!PCRenderSDLInit(800, 600)) {
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    SDL_SetRenderDrawColor(PCRenderSDLGetRenderer(), 255, 255, 255, 255);

    for (i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        SDL_RenderDrawLine(PCRenderSDLGetRenderer(), sx(x[i]), sy(y[i]), sx(x[j]), sy(y[j]));
    }

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_real_mesh_wire_test.bmp");
    PCRenderSDLShutdown();

    return 0;
}
