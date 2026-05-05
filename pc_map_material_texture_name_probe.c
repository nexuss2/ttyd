#include <stdio.h>

typedef unsigned int u32;

int mapLoadPC(const char* map);
void* mapGetPCData(void);
u32 mapGetPCDataSize(void);

static u32 be32(const unsigned char* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}

static const char* str_at(const unsigned char* data, u32 size, u32 off) {
    if (off == 0 || 0x20 + off >= size) {
        return "";
    }

    return (const char*)(data + 0x20 + off);
}

static void print_material_texture(const unsigned char* data, u32 size, u32 material) {
    u32 material_name_off = be32(data + 0x20 + material + 0x00);
    u32 ref = be32(data + 0x20 + material + 0x0c);
    u32 texture_record = 0;
    u32 texture_name_off = 0;

    if (ref && 0x20 + ref + 4 <= size) {
        texture_record = be32(data + 0x20 + ref);
    }

    if (texture_record && 0x20 + texture_record + 4 <= size) {
        texture_name_off = be32(data + 0x20 + texture_record);
    }

    printf("material=%06x name=%s ref=%06x textureRecord=%06x texture=%s\n",
        material,
        str_at(data, size, material_name_off),
        ref,
        texture_record,
        str_at(data, size, texture_name_off));
}

int main(void) {
    const unsigned char* data;
    u32 size;
    u32 materials[] = {
        0x0190fc,
        0x0181e4,
        0x0182f8,
        0x019660,
        0x019888,
        0x019774,
        0x018a84,
        0x01999c,
        0x01a128
    };
    int i;

    if (!mapLoadPC("aaa_00")) {
        return 1;
    }

    data = (const unsigned char*)mapGetPCData();
    size = mapGetPCDataSize();

    for (i = 0; i < (int)(sizeof(materials) / sizeof(materials[0])); i++) {
        print_material_texture(data, size, materials[i]);
    }

    return 0;
}
