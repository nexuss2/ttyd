#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

static void* s_effect_texture;
static u32 s_effect_texture_length;
static int s_effect_loaded;

void effInit(void) {
    s_effect_texture = 0;
    s_effect_texture_length = 0;
    s_effect_loaded = 0;
}

void effTexSetup(void) {
    s_effect_texture = arcOpen("effect.tpl", 0, &s_effect_texture_length);

    if (s_effect_texture) {
        s_effect_loaded = 1;
        printf("effTexSetup ok: effect.tpl length=%u first=%02x\n",
               s_effect_texture_length,
               *(unsigned char*)s_effect_texture);
    } else {
        s_effect_loaded = 0;
        printf("effTexSetup failed\n");
    }
}

int effIsLoaded(void) {
    return s_effect_loaded;
}
