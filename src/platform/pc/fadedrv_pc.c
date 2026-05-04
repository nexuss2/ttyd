#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

static void* s_fade_texture;
static u32 s_fade_texture_length;
static int s_fade_loaded;

void fadeInit(void) {
    s_fade_texture = 0;
    s_fade_texture_length = 0;
    s_fade_loaded = 0;
}

void fadeTexSetup(void) {
    s_fade_texture = arcOpen("fade.tpl", 0, &s_fade_texture_length);

    if (s_fade_texture) {
        s_fade_loaded = 1;
        printf("fadeTexSetup ok: fade.tpl length=%u first=%02x\n",
               s_fade_texture_length,
               *(unsigned char*)s_fade_texture);
    } else {
        s_fade_loaded = 0;
        printf("fadeTexSetup failed\n");
    }
}

int fadeIsLoaded(void) {
    return s_fade_loaded;
}
