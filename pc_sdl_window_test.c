#include <stdio.h>

int PCRenderSDLInit(int width, int height);
void PCRenderSDLBeginFrame(void);
void PCRenderSDLClear(void);
void PCRenderSDLEndFrame(void);
int PCRenderSDLPollQuit(void);
void PCRenderSDLShutdown(void);

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
        PCRenderSDLEndFrame();
    }

    PCRenderSDLShutdown();

    printf("SDL window test complete\n");
    return 0;
}
