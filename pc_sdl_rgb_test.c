#include "platform/pc/render_sdl_pc.h"

static unsigned char pixels[64 * 64 * 3];

int main(void) {
    int x;
    int y;

    for (y = 0; y < 64; y++) {
        for (x = 0; x < 64; x++) {
            int i = (y * 64 + x) * 3;
            int on = ((x / 8) + (y / 8)) & 1;

            pixels[i + 0] = on ? 220 : 40;
            pixels[i + 1] = on ? 180 : 40;
            pixels[i + 2] = on ? 80 : 40;
        }
    }

    if (!PCRenderSDLInit(640, 480)) {
        return 1;
    }

    PCRenderSDLBeginFrame();
    PCRenderSDLClear();
    PCRenderSDLDrawRGB(pixels, 64, 64, 288, 208);
    PCRenderSDLEndFrame();

    PCRenderSDLSaveBMP("build/pc/sdl_rgb_test.bmp");
    PCRenderSDLShutdown();

    return 0;
}
