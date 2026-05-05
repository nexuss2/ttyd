#include <stdlib.h>
#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLDecodeCMPRAtIndexToRGB(const void* data, u32 size, int index, unsigned char** out_pixels, int* out_width, int* out_height);

int main(void) {
    u32 length = 0;
    void* tpl;
    int i;

    tpl = arcOpen("./m/aaa_00/t", 0, &length);
    if (!tpl) {
        printf("failed to load map texture palette\n");
        return 1;
    }

    if (!PCRenderSDLInit(960, 640)) {
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();

    for (i = 0; i < 31; i++) {
        unsigned char* pixels = 0;
        int width = 0;
        int height = 0;
        int col = i % 6;
        int row = i / 6;
        int x = 32 + col * 150;
        int y = 32 + row * 110;

        if (TPLDecodeCMPRAtIndexToRGB(tpl, length, i, &pixels, &width, &height)) {
            printf("draw texture %d %dx%d at %d,%d\n", i, width, height, x, y);
            PCRenderSDLDrawRGBScaled(pixels, width, height, x, y, 96, 96);
            free(pixels);
        } else {
            printf("skip texture %d\n", i);
        }
    }

    PCRenderSDLEndFrame();
    PCRenderSDLSaveBMP("build/pc/sdl_cmpr_sheet_test.bmp");
    PCRenderSDLShutdown();

    return 0;
}
