#include <stdio.h>

typedef unsigned int u32;

void* arcOpen(const char* filename, void** addr, u32* length);

static void test_arc(const char* name) {
    u32 length = 0;
    void* data = arcOpen(name, 0, &length);

    if (!data) {
        printf("arcOpen failed: %s\n", name);
        return;
    }

    printf("arcOpen ok: %s length=%u first=%02x\n", name, length, *(unsigned char*)data);
}

int main(void) {
    test_arc("effect.tpl");
    test_arc("env.tpl");
    test_arc("fade.tpl");
    test_arc("font/msgWindow.tpl");
    return 0;
}
