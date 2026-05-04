#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);
void TPLInspect(const char* label, const void* data, u32 size);

static void inspect(const char* name) {
    u32 length = 0;
    void* data = arcOpen(name, 0, &length);

    if (!data) {
        printf("TPL load failed: %s\n", name);
        return;
    }

    TPLInspect(name, data, length);
}

int main(void) {
    inspect("fade.tpl");
    inspect("env.tpl");
    inspect("font/msgWindow.tpl");
    inspect("effect.tpl");
    inspect("./m/aaa_00/t");
    return 0;
}
