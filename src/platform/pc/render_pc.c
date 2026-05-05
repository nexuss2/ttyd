#include <stdio.h>

static int s_width;
static int s_height;
static int s_frame;

int PCRenderInit(int width, int height) {
    s_width = width;
    s_height = height;
    s_frame = 0;

    printf("PC render init %dx%d\n", s_width, s_height);
    return 1;
}

void PCRenderBeginFrame(void) {
    printf("[render] begin frame %d\n", s_frame);
}

void PCRenderClear(void) {
    printf("[render] clear\n");
}

void PCRenderEndFrame(void) {
    printf("[render] end frame %d\n", s_frame);
    s_frame++;
}

void PCRenderShutdown(void) {
    printf("PC render shutdown after %d frames\n", s_frame);
}
