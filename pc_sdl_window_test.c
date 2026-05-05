#include <stdio.h>

int PCRenderSDLInit(int width, int height);
void PCRenderSDLBeginFrame(void);
void PCRenderSDLClear(void);
void PCRenderSDLEndFrame(void);
int PCRenderSDLPollQuit(void);
void PCRenderSDLShutdown(void);
int PCRenderSDLSaveBMP(const char* path);
int PCRenderSDLDrawPPM(const char* path, int x, int y);

int main(void) {
    int frame;

    if (!PCRenderSDLInit(640, 480)) {
        return 1;
    }

    for (frame = 0; frame < 300; frame++) {
        if (PCRenderSDLPollQuit()) {
            break;
        }

        PCRenderSDLBeginFrame();
        PCRenderSDLClear();
        PCRenderSDLDrawPPM("build/pc/aaa_00_cmpr.ppm", 288, 208);
        PCRenderSDLEndFrame();

        if (frame == 0) {
            PCRenderSDLSaveBMP("build/pc/sdl_window_test.bmp");
        }
    }

    PCRenderSDLShutdown();

    printf("SDL window test complete\n");
    return 0;
}
