#include <stdlib.h>
#include <stdio.h>
#include "platform/pc/render_sdl_pc.h"

typedef unsigned int u32;

typedef struct PCTextureRuntime {
    PCSDLTexture* texture;
    int width;
    int height;
    int index;
} PCTextureRuntime;

int TPLDecodeCMPRAtIndexToRGB(const void* data, u32 size, int index, unsigned char** out_pixels, int* out_width, int* out_height);

PCTextureRuntime* PCTextureRuntimeCreateFromTPLCMPR(const void* tpl, u32 size, int index) {
    unsigned char* pixels = 0;
    int width = 0;
    int height = 0;
    PCTextureRuntime* out;

    if (!TPLDecodeCMPRAtIndexToRGB(tpl, size, index, &pixels, &width, &height)) {
        return 0;
    }

    out = (PCTextureRuntime*)malloc(sizeof(PCTextureRuntime));
    if (!out) {
        free(pixels);
        return 0;
    }

    out->texture = PCRenderSDLCreateTextureRGB(pixels, width, height);
    free(pixels);

    if (!out->texture) {
        free(out);
        return 0;
    }

    out->width = width;
    out->height = height;
    out->index = index;

    printf("runtime texture %d created %dx%d\n", index, width, height);
    return out;
}

void PCTextureRuntimeDraw(PCTextureRuntime* texture, int x, int y, int draw_width, int draw_height) {
    if (!texture) {
        return;
    }

    PCRenderSDLDrawTexture(texture->texture, x, y, draw_width, draw_height);
}

void PCTextureRuntimeDestroy(PCTextureRuntime* texture) {
    if (!texture) {
        return;
    }

    PCRenderSDLDestroyTexture(texture->texture);
    free(texture);
}
