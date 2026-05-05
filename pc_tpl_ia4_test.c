#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLExportFirstIA4(const char* out_path, const void* data, u32 size);

int main(void) {
    u32 length = 0;
    void* data = arcOpen("effect.tpl", 0, &length);

    if (!data) {
        printf("failed to load effect.tpl\n");
        return 1;
    }

    if (!TPLExportFirstIA4("build/pc/effect_ia4.ppm", data, length)) {
        printf("IA4 export failed\n");
        return 1;
    }

    return 0;
}
