#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32;
typedef unsigned short u16;

static u32 be32(const unsigned char* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}

static u16 be16(const unsigned char* p) {
    return ((u16)p[0] << 8) | (u16)p[1];
}

static void write_ppm_gray_i4(const char* out_path, const unsigned char* src, u32 width, u32 height) {
    FILE* f;
    u32 x;
    u32 y;

    f = fopen(out_path, "wb");
    if (!f) {
        printf("failed to write %s\n", out_path);
        return;
    }

    fprintf(f, "P6\n%u %u\n255\n", width, height);

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            u32 pixel_index = y * width + x;
            unsigned char b = src[pixel_index / 2];
            unsigned char nibble;
            unsigned char v;
            unsigned char rgb[3];

            if ((pixel_index & 1) == 0) {
                nibble = b >> 4;
            } else {
                nibble = b & 0x0F;
            }

            v = (unsigned char)(nibble * 17);
            rgb[0] = v;
            rgb[1] = v;
            rgb[2] = v;
            fwrite(rgb, 1, 3, f);
        }
    }

    fclose(f);
    printf("wrote %s\n", out_path);
}

int TPLExportFirstI4(const char* out_path, const void* data, u32 size) {
    const unsigned char* p;
    u32 version;
    u32 count;
    u32 descriptorOffset;
    u32 i;

    p = (const unsigned char*)data;

    if (!data || size < 12) {
        return 0;
    }

    version = be32(p + 0);
    count = be32(p + 4);
    descriptorOffset = be32(p + 8);

    if (version != 0x0020AF30) {
        return 0;
    }

    for (i = 0; i < count; i++) {
        u32 desc = descriptorOffset + i * 8;
        u32 texHeaderOffset;
        const unsigned char* th;
        u32 format;
        u32 dataOffset;
        u16 height;
        u16 width;

        if (desc + 8 > size) {
            return 0;
        }

        texHeaderOffset = be32(p + desc);
        if (!texHeaderOffset || texHeaderOffset + 0x24 > size) {
            continue;
        }

        th = p + texHeaderOffset;
        height = be16(th + 0);
        width = be16(th + 2);
        format = be32(th + 4);
        dataOffset = be32(th + 8);

        if (format == 0 && dataOffset < size) {
            printf("exporting descriptor %u I4 width=%u height=%u dataOffset=%u\n",
                   i, width, height, dataOffset);
            write_ppm_gray_i4(out_path, p + dataOffset, width, height);
            return 1;
        }
    }

    printf("no I4 texture found\n");
    return 0;
}
