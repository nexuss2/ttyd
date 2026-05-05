#include <stdlib.h>
#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLDecodeCMPRAtIndexToRGB(const void* data, u32 size, int index, unsigned char** out_pixels, int* out_width, int* out_height);

int main(void) {
    u32 length = 0;
    void* tpl;
    unsigned char* pixels = 0;
    int width = 0;
    int height = 0;
    PCSDLTexture* texture;

    tpl = arcOpen("./m/aaa_00/t", 0, &length);
    if (!tpl) {
        printf("failed to load map texture palette\n");
        return 1;
    }

    if (!TPLDecodeCMPRAtIndexToRGB(tpl, length, 0, &pixels, &width, &height)) {
        printf("failed to decode CMPR\n");
        return 1;
    }

    if (!PCRenderSDLInit(640, 480)) {
        free(pixels);
        return 1;
    }

    texture = PCRenderSDLCreateTextureRGB(pixels, width, height);
    free(pixels);

    if (!texture) {
        PCRenderSDLShutdown();
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();
    PCRenderSDLDrawTexture(texture, 256, 176, 128, 128);
    PCRenderSDLEndFrame();

    PCRenderSDLSaveBMP("build/pc/sdl_texture_cache_test.bmp");

    PCRenderSDLDestroyTexture(texture);
    PCRenderSDLShutdown();

    return 0;
}
