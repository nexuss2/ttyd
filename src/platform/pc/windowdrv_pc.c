#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

static void* s_window_texture;
static u32 s_window_texture_length;
static int s_window_loaded;

void windowInit(void) {
    s_window_texture = 0;
    s_window_texture_length = 0;
    s_window_loaded = 0;
}

void windowTexSetup(void) {
    s_window_texture = arcOpen("font/msgWindow.tpl", 0, &s_window_texture_length);

    if (s_window_texture) {
        s_window_loaded = 1;
        printf("windowTexSetup ok: font/msgWindow.tpl length=%u first=%02x\n",
               s_window_texture_length,
               *(unsigned char*)s_window_texture);
    } else {
        s_window_loaded = 0;
        printf("windowTexSetup failed\n");
    }
}

int windowIsLoaded(void) {
    return s_window_loaded;
}
