#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
int TPLExportFirstI8(const char* out_path, const void* data, u32 size);

int main(void) {
    u32 length = 0;
    void* data = arcOpen("fade.tpl", 0, &length);

    if (!data) {
        printf("failed to load fade.tpl\n");
        return 1;
    }

    if (!TPLExportFirstI8("build/pc/fade_i8.ppm", data, length)) {
        printf("I8 export failed\n");
        return 1;
    }

    return 0;
}
