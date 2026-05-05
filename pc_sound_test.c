#include <stdio.h>

typedef unsigned int u32;

typedef struct DVDFileInfo {
    unsigned char unused[0x34];
    u32 length;
    void* callback;
} DVDFileInfo;

int PlatformFileExists(const char* path);
int PlatformFileOpen(const char* path, DVDFileInfo* info);

static void test_sound_file(const char* path) {
    DVDFileInfo info;

    if (!PlatformFileExists(path)) {
        printf("missing: %s\n", path);
        return;
    }

    if (!PlatformFileOpen(path, &info)) {
        printf("open failed: %s\n", path);
        return;
    }

    printf("ok: %s length=%u\n", path, info.length);
}

int main(void) {
    test_sound_file("./sound/proj/pmario.pool");
    test_sound_file("./sound/proj/pmario.proj");
    test_sound_file("./sound/proj/pmario.sdir");
    test_sound_file("./sound/proj/pmario.samp");
    test_sound_file("./sound/proj/pmario.slib");
    test_sound_file("./sound/proj/pmario.stbl");
    test_sound_file("./sound/proj/pmario.etbl");
    test_sound_file("./sound/stream/sys_opn1_32k.stm");
    test_sound_file("./sound/stream/btl_win1_32k.stm");
    return 0;
}
