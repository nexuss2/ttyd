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
    unsigned char* out;
    u32 tile_x;
    u32 tile_y;
    u32 y;
    u32 x;
    const unsigned char* cursor;

    out = (unsigned char*)calloc(width * height * 3, 1);
    if (!out) {
        return;
    }

    cursor = src;

    for (tile_y = 0; tile_y < height; tile_y += 8) {
        for (tile_x = 0; tile_x < width; tile_x += 8) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x += 2) {
                    unsigned char b = *cursor++;
                    unsigned char hi = b >> 4;
                    unsigned char lo = b & 0x0F;
                    u32 px0 = tile_x + x;
                    u32 px1 = tile_x + x + 1;
                    u32 py = tile_y + y;

                    if (px0 < width && py < height) {
                        unsigned char v = hi * 17;
                        unsigned char* dst = out + ((py * width + px0) * 3);
                        dst[0] = v;
                        dst[1] = v;
                        dst[2] = v;
                    }

                    if (px1 < width && py < height) {
                        unsigned char v = lo * 17;
                        unsigned char* dst = out + ((py * width + px1) * 3);
                        dst[0] = v;
                        dst[1] = v;
                        dst[2] = v;
                    }
                }
            }
        }
    }

    f = fopen(out_path, "wb");
    if (!f) {
        free(out);
        printf("failed to write %s\n", out_path);
        return;
    }

    fprintf(f, "P6\n%u %u\n255\n", width, height);
    fwrite(out, 1, width * height * 3, f);
    fclose(f);
    free(out);

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

static void rgb565_to_rgb(unsigned short c, unsigned char* rgb) {
    unsigned char r = (c >> 11) & 0x1F;
    unsigned char g = (c >> 5) & 0x3F;
    unsigned char b = c & 0x1F;

    rgb[0] = (r << 3) | (r >> 2);
    rgb[1] = (g << 2) | (g >> 4);
    rgb[2] = (b << 3) | (b >> 2);
}

static void decode_cmpr_block(const unsigned char* src, unsigned char* out, u32 width, u32 height, u32 bx, u32 by) {
    unsigned short c0 = ((unsigned short)src[0] << 8) | src[1];
    unsigned short c1 = ((unsigned short)src[2] << 8) | src[3];
    unsigned int bits = ((unsigned int)src[4] << 24) | ((unsigned int)src[5] << 16) | ((unsigned int)src[6] << 8) | src[7];
    unsigned char colors[4][3];
    u32 x;
    u32 y;

    rgb565_to_rgb(c0, colors[0]);
    rgb565_to_rgb(c1, colors[1]);

    if (c0 > c1) {
        colors[2][0] = (2 * colors[0][0] + colors[1][0]) / 3;
        colors[2][1] = (2 * colors[0][1] + colors[1][1]) / 3;
        colors[2][2] = (2 * colors[0][2] + colors[1][2]) / 3;
        colors[3][0] = (colors[0][0] + 2 * colors[1][0]) / 3;
        colors[3][1] = (colors[0][1] + 2 * colors[1][1]) / 3;
        colors[3][2] = (colors[0][2] + 2 * colors[1][2]) / 3;
    } else {
        colors[2][0] = (colors[0][0] + colors[1][0]) / 2;
        colors[2][1] = (colors[0][1] + colors[1][1]) / 2;
        colors[2][2] = (colors[0][2] + colors[1][2]) / 2;
        colors[3][0] = 0;
        colors[3][1] = 0;
        colors[3][2] = 0;
    }

    for (y = 0; y < 4; y++) {
        for (x = 0; x < 4; x++) {
            u32 idx = (bits >> (30 - 2 * (y * 4 + x))) & 3;
            u32 px = bx + x;
            u32 py = by + y;

            if (px < width && py < height) {
                unsigned char* dst = out + ((py * width + px) * 3);
                dst[0] = colors[idx][0];
                dst[1] = colors[idx][1];
                dst[2] = colors[idx][2];
            }
        }
    }
}

static void write_ppm_rgb(const char* out_path, const unsigned char* rgb, u32 width, u32 height) {
    FILE* f = fopen(out_path, "wb");

    if (!f) {
        printf("failed to write %s\n", out_path);
        return;
    }

    fprintf(f, "P6\n%u %u\n255\n", width, height);
    fwrite(rgb, 1, width * height * 3, f);
    fclose(f);

    printf("wrote %s\n", out_path);
}

int TPLExportFirstCMPR(const char* out_path, const void* data, u32 size) {
    const unsigned char* p = (const unsigned char*)data;
    u32 version;
    u32 count;
    u32 descriptorOffset;
    u32 i;

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
        unsigned char* out;
        const unsigned char* src;
        u32 bx;
        u32 by;

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

        if (format != 14 || dataOffset >= size) {
            continue;
        }

        printf("exporting descriptor %u CMPR width=%u height=%u dataOffset=%u\n", i, width, height, dataOffset);

        out = (unsigned char*)calloc(width * height * 3, 1);
        if (!out) {
            return 0;
        }

        src = p + dataOffset;

        for (by = 0; by < height; by += 8) {
            for (bx = 0; bx < width; bx += 8) {
                decode_cmpr_block(src + 0, out, width, height, bx + 0, by + 0);
                decode_cmpr_block(src + 8, out, width, height, bx + 4, by + 0);
                decode_cmpr_block(src + 16, out, width, height, bx + 0, by + 4);
                decode_cmpr_block(src + 24, out, width, height, bx + 4, by + 4);
                src += 32;
            }
        }

        write_ppm_rgb(out_path, out, width, height);
        free(out);
        return 1;
    }

    printf("no CMPR texture found\n");
    return 0;
}

int TPLExportAllCMPR(const char* out_dir, const char* prefix, const void* data, u32 size) {
    const unsigned char* p = (const unsigned char*)data;
    u32 version;
    u32 count;
    u32 descriptorOffset;
    u32 i;
    int exported = 0;

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
        unsigned char* out;
        const unsigned char* src;
        u32 bx;
        u32 by;
        char out_path[256];

        if (desc + 8 > size) {
            break;
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

        if (format != 14 || dataOffset >= size) {
            continue;
        }

        out = (unsigned char*)calloc(width * height * 3, 1);
        if (!out) {
            continue;
        }

        src = p + dataOffset;

        for (by = 0; by < height; by += 8) {
            for (bx = 0; bx < width; bx += 8) {
                decode_cmpr_block(src + 0, out, width, height, bx + 0, by + 0);
                decode_cmpr_block(src + 8, out, width, height, bx + 4, by + 0);
                decode_cmpr_block(src + 16, out, width, height, bx + 0, by + 4);
                decode_cmpr_block(src + 24, out, width, height, bx + 4, by + 4);
                src += 32;
            }
        }

        sprintf(out_path, "%s/%s_%03u.ppm", out_dir, prefix, i);
        write_ppm_rgb(out_path, out, width, height);
        free(out);
        exported++;
    }

    printf("exported %d CMPR textures from %s\n", exported, prefix);
    return exported;
}

static void write_ppm_rgb565_tiled(const char* out_path, const unsigned char* src, u32 width, u32 height) {
    unsigned char* out;
    const unsigned char* cursor;
    u32 tile_x;
    u32 tile_y;
    u32 x;
    u32 y;

    out = (unsigned char*)calloc(width * height * 3, 1);
    if (!out) {
        return;
    }

    cursor = src;

    for (tile_y = 0; tile_y < height; tile_y += 4) {
        for (tile_x = 0; tile_x < width; tile_x += 4) {
            for (y = 0; y < 4; y++) {
                for (x = 0; x < 4; x++) {
                    unsigned short c = ((unsigned short)cursor[0] << 8) | cursor[1];
                    unsigned char rgb[3];
                    u32 px = tile_x + x;
                    u32 py = tile_y + y;

                    cursor += 2;
                    rgb565_to_rgb(c, rgb);

                    if (px < width && py < height) {
                        unsigned char* dst = out + ((py * width + px) * 3);
                        dst[0] = rgb[0];
                        dst[1] = rgb[1];
                        dst[2] = rgb[2];
                    }
                }
            }
        }
    }

    write_ppm_rgb(out_path, out, width, height);
    free(out);
}

int TPLExportFirstRGB565(const char* out_path, const void* data, u32 size) {
    const unsigned char* p = (const unsigned char*)data;
    u32 version;
    u32 count;
    u32 descriptorOffset;
    u32 i;

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

        if (format == 5 && dataOffset < size) {
            printf("exporting descriptor %u RGB565 width=%u height=%u dataOffset=%u\n", i, width, height, dataOffset);
            write_ppm_rgb565_tiled(out_path, p + dataOffset, width, height);
            return 1;
        }
    }

    printf("no RGB565 texture found\n");
    return 0;
}
