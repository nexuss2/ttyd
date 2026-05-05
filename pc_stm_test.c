#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32;
typedef unsigned short u16;

typedef struct DVDFileInfo {
    unsigned char unused[0x34];
    u32 length;
    void* callback;
} DVDFileInfo;

int PlatformFileOpen(const char* path, DVDFileInfo* info);
int PlatformFileRead(const char* path, DVDFileInfo* info, void* address, u32 size, int offset);

static u32 be32(const unsigned char* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}

static u16 be16(const unsigned char* p) {
    return ((u16)p[0] << 8) | (u16)p[1];
}

static void inspect_stm(const char* path) {
    DVDFileInfo info;
    unsigned char header[128];

    if (!PlatformFileOpen(path, &info)) {
        printf("open failed: %s\n", path);
        return;
    }

    if (PlatformFileRead(path, &info, header, sizeof(header), 0) < 0) {
        printf("read failed: %s\n", path);
        return;
    }

    printf("STM %s length=%u\n", path, info.length);
    printf("  h00=%04x h02=%04x\n", be16(header + 0), be16(header + 2));
    printf("  h04=%u h08=%u h0c=%u\n", be32(header + 4), be32(header + 8), be32(header + 12));
    printf("  h10=%u h14=%u\n", be32(header + 16), be32(header + 20));
    printf("  h40=%u h44=%u h48=%u h4c=%u\n", be32(header + 0x40), be32(header + 0x44), be32(header + 0x48), be32(header + 0x4c));
    printf("  h50=%u h54=%u\n", be32(header + 0x50), be32(header + 0x54));
}

int main(void) {
    inspect_stm("./sound/stream/sys_opn1_32k.stm");
    inspect_stm("./sound/stream/btl_win1_32k.stm");
    return 0;
}
