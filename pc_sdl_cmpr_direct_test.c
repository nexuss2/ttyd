#include <stdlib.h>
#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLDecodeFirstCMPRToRGB(const void* data, u32 size, unsigned char** out_pixels, int* out_width, int* out_height);

int main(void) {
    u32 length = 0;
    void* tpl;
    unsigned char* pixels = 0;
    int width = 0;
    int height = 0;

    tpl = arcOpen("./m/aaa_00/t", 0, &length);
    if (!tpl) {
        printf("failed to load map texture palette\n");
        return 1;
    }

    if (!TPLDecodeFirstCMPRToRGB(tpl, length, &pixels, &width, &height)) {
        printf("failed to decode CMPR\n");
        return 1;
    }

    if (!PCRenderSDLInit(640, 480)) {
        free(pixels);
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();
    PCRenderSDLDrawRGB(pixels, width, height, 288, 208);
    PCRenderSDLEndFrame();

    PCRenderSDLSaveBMP("build/pc/sdl_cmpr_direct_test.bmp");

    PCRenderSDLShutdown();
    free(pixels);

    return 0;
}
