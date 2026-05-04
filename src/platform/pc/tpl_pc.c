#include <stdio.h>

typedef unsigned int u32;
typedef unsigned short u16;

static u32 be32(const unsigned char* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}

static u16 be16(const unsigned char* p) {
    return ((u16)p[0] << 8) | (u16)p[1];
}

void TPLInspect(const char* label, const void* data, u32 size) {
    const unsigned char* p;
    u32 version;
    u32 count;
    u32 descriptorOffset;
    u32 i;

    p = (const unsigned char*)data;

    if (!data || size < 12) {
        printf("TPLInspect %s failed: too small\n", label);
        return;
    }

    version = be32(p + 0);
    count = be32(p + 4);
    descriptorOffset = be32(p + 8);

    printf("TPL %s version=%08x descriptors=%u descriptorOffset=%u\n",
           label, version, count, descriptorOffset);

    if (version != 0x0020AF30) {
        printf("TPL %s unexpected version\n", label);
        return;
    }

    for (i = 0; i < count && i < 8; i++) {
        u32 desc = descriptorOffset + i * 8;
        u32 texHeaderOffset;
        u32 clutHeaderOffset;

        if (desc + 8 > size) {
            printf("  descriptor[%u] outside\n", i);
            break;
        }

        texHeaderOffset = be32(p + desc);
        clutHeaderOffset = be32(p + desc + 4);

        printf("  descriptor[%u] texHeader=%u clutHeader=%u", i, texHeaderOffset, clutHeaderOffset);

        if (texHeaderOffset && texHeaderOffset + 0x24 <= size) {
            const unsigned char* th = p + texHeaderOffset;
            u16 height = be16(th + 0);
            u16 width = be16(th + 2);
            u32 format = be32(th + 4);
            u32 dataOffset = be32(th + 8);
            u32 wrapS = be32(th + 12);
            u32 wrapT = be32(th + 16);

            printf(" width=%u height=%u format=%u dataOffset=%u wrapS=%u wrapT=%u",
                   width, height, format, dataOffset, wrapS, wrapT);
        }

        printf("\n");
    }
}
