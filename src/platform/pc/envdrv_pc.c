#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

static void* s_env_texture;
static u32 s_env_texture_length;
static int s_env_loaded;

void envInit(void) {
    s_env_texture = 0;
    s_env_texture_length = 0;
    s_env_loaded = 0;
}

void envTexSetup(void) {
    s_env_texture = arcOpen("env.tpl", 0, &s_env_texture_length);

    if (s_env_texture) {
        s_env_loaded = 1;
        printf("envTexSetup ok: env.tpl length=%u first=%02x\n",
               s_env_texture_length,
               *(unsigned char*)s_env_texture);
    } else {
        s_env_loaded = 0;
        printf("envTexSetup failed\n");
    }
}

int envIsLoaded(void) {
    return s_env_loaded;
}
