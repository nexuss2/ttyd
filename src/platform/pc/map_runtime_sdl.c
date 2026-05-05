#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "platform/pc/render_sdl_pc.h"
#include "platform/pc/map_runtime_sdl.h"

typedef unsigned int u32;

int mapLoadPC(const char* map);
void* mapGetPCData(void);
u32 mapGetPCDataSize(void);

typedef struct PCMapWorldLine {
    float x0;
    float y0;
    float z0;
    float x1;
    float y1;
    float z1;
} PCMapWorldLine;

typedef struct PCMapTriangle {
    u32 material;
    u32 texture_hash;
    float x0;
    float y0;
    float z0;
    float x1;
    float y1;
    float z1;
    float x2;
    float y2;
    float z2;
} PCMapTriangle;

struct PCMapRuntime {
    const unsigned char* data;
    u32 size;
    u32 root_joint;
    PCMapWorldLine* lines;
    int line_count;
    int line_capacity;
    PCMapTriangle* triangles;
    int triangle_count;
    int triangle_capacity;
    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;
};

static float s_view_offset_x = 450.0f;
static float s_view_offset_y = 620.0f;
static float s_view_scale = 1.0f;

static u32 be32(const unsigned char* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3];
}

static unsigned short be16u(const unsigned char* p) {
    return (unsigned short)(((unsigned short)p[0] << 8) | p[1]);
}

static short be16s(const unsigned char* p) {
    return (short)(((unsigned short)p[0] << 8) | p[1]);
}

static float bef32(const unsigned char* p) {
    union {
        u32 u;
        float f;
    } v;

    v.u = be32(p);
    return v.f;
}

static void get_pos(const unsigned char* data, u32 pos_base, int index, float tx, float ty, float tz, float* x, float* y, float* z) {
    const unsigned char* p = data + 0x20 + pos_base + index * 6;

    *x = be16s(p + 0) / 100.0f + tx;
    *y = be16s(p + 2) / 100.0f + ty;
    *z = be16s(p + 4) / 100.0f + tz;
}

static int project_x(float x, float z) {
    return (int)(s_view_offset_x + ((x * 5.0f) + (z * 1.5f)) * s_view_scale);
}

static const char* map_str_at(PCMapRuntime* map, u32 off) {
    if (!map || off == 0 || 0x20 + off >= map->size) {
        return "";
    }

    return (const char*)(map->data + 0x20 + off);
}

static u32 hash_texture_name_string(const char* s) {
    u32 h = 2166136261u;

    while (s && *s) {
        h ^= (unsigned char)*s;
        h *= 16777619u;
        s++;
    }

    return h;
}

static u32 material_texture_hash(PCMapRuntime* map, u32 material) {
    u32 ref;
    u32 texture_record;
    u32 texture_name_off;
    const char* texture_name;

    if (!map || !material || 0x20 + material + 0x10 > map->size) {
        return 0;
    }

    ref = be32(map->data + 0x20 + material + 0x0c);

    if (!ref || 0x20 + ref + 4 > map->size) {
        return 0;
    }

    texture_record = be32(map->data + 0x20 + ref);

    if (!texture_record || 0x20 + texture_record + 4 > map->size) {
        return 0;
    }

    texture_name_off = be32(map->data + 0x20 + texture_record);
    texture_name = map_str_at(map, texture_name_off);

    return hash_texture_name_string(texture_name);
}

static unsigned char mat_r(u32 material) {
    return (unsigned char)(80 + ((material >> 1) & 127));
}

static unsigned char mat_g(u32 material) {
    return (unsigned char)(80 + ((material >> 4) & 127));
}

static unsigned char mat_b(u32 material) {
    return (unsigned char)(80 + ((material >> 7) & 127));
}

static int project_y(float y, float z) {
    return (int)(s_view_offset_y + ((-y * 3.0f) + (z * 1.0f)) * s_view_scale);
}

static void add_world_line(PCMapRuntime* map, float x0, float y0, float z0, float x1, float y1, float z1) {
    PCMapWorldLine* next;

    if (map->line_count >= map->line_capacity) {
        int new_capacity = map->line_capacity ? map->line_capacity * 2 : 4096;
        next = (PCMapWorldLine*)realloc(map->lines, sizeof(PCMapWorldLine) * new_capacity);

        if (!next) {
            return;
        }

        map->lines = next;
        map->line_capacity = new_capacity;
    }

    if (map->line_count == 0) {
        map->min_x = map->max_x = x0;
        map->min_y = map->max_y = y0;
        map->min_z = map->max_z = z0;
    }

    if (x0 < map->min_x) map->min_x = x0;
    if (x1 < map->min_x) map->min_x = x1;
    if (y0 < map->min_y) map->min_y = y0;
    if (y1 < map->min_y) map->min_y = y1;
    if (z0 < map->min_z) map->min_z = z0;
    if (z1 < map->min_z) map->min_z = z1;

    if (x0 > map->max_x) map->max_x = x0;
    if (x1 > map->max_x) map->max_x = x1;
    if (y0 > map->max_y) map->max_y = y0;
    if (y1 > map->max_y) map->max_y = y1;
    if (z0 > map->max_z) map->max_z = z0;
    if (z1 > map->max_z) map->max_z = z1;

    map->lines[map->line_count].x0 = x0;
    map->lines[map->line_count].y0 = y0;
    map->lines[map->line_count].z0 = z0;
    map->lines[map->line_count].x1 = x1;
    map->lines[map->line_count].y1 = y1;
    map->lines[map->line_count].z1 = z1;
    map->line_count++;
}


static void add_triangle(PCMapRuntime* map, u32 material, u32 texture_hash, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) {
    PCMapTriangle* next;

    if (map->triangle_count >= map->triangle_capacity) {
        int new_capacity = map->triangle_capacity ? map->triangle_capacity * 2 : 4096;
        next = (PCMapTriangle*)realloc(map->triangles, sizeof(PCMapTriangle) * new_capacity);

        if (!next) {
            return;
        }

        map->triangles = next;
        map->triangle_capacity = new_capacity;
    }

    map->triangles[map->triangle_count].material = material;
    map->triangles[map->triangle_count].texture_hash = texture_hash;
    map->triangles[map->triangle_count].x0 = x0;
    map->triangles[map->triangle_count].y0 = y0;
    map->triangles[map->triangle_count].z0 = z0;
    map->triangles[map->triangle_count].x1 = x1;
    map->triangles[map->triangle_count].y1 = y1;
    map->triangles[map->triangle_count].z1 = z1;
    map->triangles[map->triangle_count].x2 = x2;
    map->triangles[map->triangle_count].y2 = y2;
    map->triangles[map->triangle_count].z2 = z2;
    map->triangle_count++;
}

static void cache_display_list(PCMapRuntime* map, u32 material, u32 mesh, u32 pos_base, int dl_index, float tx, float ty, float tz) {
    u32 dl = be32(map->data + 0x20 + mesh + 0x10 + dl_index * 8);
    u32 dl_len = be32(map->data + 0x20 + mesh + 0x14 + dl_index * 8);
    const unsigned char* p;
    unsigned char cmd;
    int count;
    float x[128];
    float y[128];
    float z[128];
    int i;

    if (dl == 0 || dl_len == 0 || 0x20 + dl + dl_len > map->size) {
        return;
    }

    p = map->data + 0x20 + dl;
    cmd = p[0];
    count = be16u(p + 1);

    if (cmd != 0x98 || count <= 0 || count > 128) {
        return;
    }

    p += 3;

    for (i = 0; i < count; i++) {
        int pos_index = be16u(p + 0);
        get_pos(map->data, pos_base, pos_index, tx, ty, tz, &x[i], &y[i], &z[i]);
        p += 10;
    }

    for (i = 0; i < count; i++) {
        int k = (i + 1) % count;
        add_world_line(map, x[i], y[i], z[i], x[k], y[k], z[k]);
    }

    {
        u32 texture_hash = material_texture_hash(map, material);

        for (i = 1; i + 1 < count; i++) {
            add_triangle(map, material, texture_hash, x[0], y[0], z[0], x[i], y[i], z[i], x[i + 1], y[i + 1], z[i + 1]);
        }
    }
}

static void cache_mesh(PCMapRuntime* map, u32 material, u32 mesh, float tx, float ty, float tz) {
    u32 vcd;
    u32 pos_base;
    u32 display_list_count;
    u32 i;

    if (!mesh || 0x20 + mesh + 0x10 > map->size) {
        return;
    }

    vcd = be32(map->data + 0x20 + mesh + 0x0c);
    if (!vcd || 0x20 + vcd + 4 > map->size) {
        return;
    }

    pos_base = be32(map->data + 0x20 + vcd + 0x00);
    display_list_count = be32(map->data + 0x20 + mesh + 0x04);

    if (display_list_count > 256) {
        return;
    }

    for (i = 0; i < display_list_count; i++) {
        cache_display_list(map, material, mesh, pos_base, i, tx, ty, tz);
    }
}

static void cache_joint_tree(PCMapRuntime* map, u32 joint, float parent_tx, float parent_ty, float parent_tz) {
    const unsigned char* j;
    u32 child;
    u32 next;
    u32 part_count;
    float tx;
    float ty;
    float tz;
    u32 i;

    if (!map || joint == 0 || 0x20 + joint + 0x68 > map->size) {
        return;
    }

    j = map->data + 0x20 + joint;

    child = be32(j + 0x0c);
    next = be32(j + 0x10);
    part_count = be32(j + 0x5c);

    tx = parent_tx + bef32(j + 0x30);
    ty = parent_ty + bef32(j + 0x34);
    tz = parent_tz + bef32(j + 0x38);

    if (part_count > 0 && part_count < 64) {
        for (i = 0; i < part_count; i++) {
            u32 part = 0x60 + i * 8;
            u32 material = be32(j + part + 0);
            u32 mesh = be32(j + part + 4);
            cache_mesh(map, material, mesh, tx, ty, tz);
        }
    }

    cache_joint_tree(map, child, tx, ty, tz);
    cache_joint_tree(map, next, parent_tx, parent_ty, parent_tz);
}

PCMapRuntime* PCMapRuntimeLoad(const char* map_name) {
    PCMapRuntime* out;

    if (!mapLoadPC(map_name)) {
        return 0;
    }

    out = (PCMapRuntime*)calloc(1, sizeof(PCMapRuntime));
    if (!out) {
        return 0;
    }

    out->data = (const unsigned char*)mapGetPCData();
    out->size = mapGetPCDataSize();
    out->root_joint = be32(out->data + 0x20 + 4);

    cache_joint_tree(out, out->root_joint, 0.0f, 0.0f, 0.0f);

    printf("loaded real map runtime %s root=%06x size=%u cachedWorldLines=%d cachedTriangles=%d\n",
        map_name,
        out->root_joint,
        out->size,
        out->line_count,
        out->triangle_count);

    printf("real map bounds x=[%.2f, %.2f] y=[%.2f, %.2f] z=[%.2f, %.2f]\n",
        out->min_x, out->max_x,
        out->min_y, out->max_y,
        out->min_z, out->max_z);

    return out;
}

void PCMapRuntimeSetView(float offset_x, float offset_y, float scale) {
    s_view_offset_x = offset_x;
    s_view_offset_y = offset_y;
    s_view_scale = scale;
}

void PCMapRuntimeUseAutoView(PCMapRuntime* map, int width, int height) {
    float min_px;
    float max_px;
    float min_py;
    float max_py;
    float w;
    float h;
    float scale_x;
    float scale_y;
    float scale;

    if (!map || map->line_count == 0) {
        return;
    }

    min_px = (map->min_x * 5.0f) + (map->min_z * 1.5f);
    max_px = (map->max_x * 5.0f) + (map->max_z * 1.5f);
    min_py = (-map->max_y * 3.0f) + (map->min_z * 1.0f);
    max_py = (-map->min_y * 3.0f) + (map->max_z * 1.0f);

    w = max_px - min_px;
    h = max_py - min_py;

    if (w < 1.0f) {
        w = 1.0f;
    }

    if (h < 1.0f) {
        h = 1.0f;
    }

    scale_x = (float)(width - 80) / w;
    scale_y = (float)(height - 80) / h;
    scale = scale_x < scale_y ? scale_x : scale_y;

    s_view_scale = scale;
    s_view_offset_x = ((float)width * 0.5f) - ((min_px + max_px) * 0.5f * scale);
    s_view_offset_y = ((float)height * 0.5f) - ((min_py + max_py) * 0.5f * scale);

    printf("auto view offset=(%.2f, %.2f) scale=%.4f projectedSize=(%.2f, %.2f)\n",
        s_view_offset_x,
        s_view_offset_y,
        s_view_scale,
        w,
        h);
}

void PCMapRuntimeDrawWire(PCMapRuntime* map) {
    int i;

    if (!map) {
        return;
    }

    SDL_SetRenderDrawColor(PCRenderSDLGetRenderer(), 255, 255, 255, 255);

    for (i = 0; i < map->line_count; i++) {
        SDL_RenderDrawLine(
            PCRenderSDLGetRenderer(),
            project_x(map->lines[i].x0, map->lines[i].z0),
            project_y(map->lines[i].y0, map->lines[i].z0),
            project_x(map->lines[i].x1, map->lines[i].z1),
            project_y(map->lines[i].y1, map->lines[i].z1)
        );
    }
}

void PCMapRuntimeDrawFilled(PCMapRuntime* map) {
    int i;

    if (!map) {
        return;
    }

    SDL_SetRenderDrawColor(PCRenderSDLGetRenderer(), 90, 90, 90, 255);

    for (i = 0; i < map->triangle_count; i++) {
        SDL_Vertex verts[3];

        verts[0].position.x = (float)project_x(map->triangles[i].x0, map->triangles[i].z0);
        verts[0].position.y = (float)project_y(map->triangles[i].y0, map->triangles[i].z0);
        verts[0].color.r = mat_r(map->triangles[i].texture_hash);
        verts[0].color.g = mat_g(map->triangles[i].texture_hash);
        verts[0].color.b = mat_b(map->triangles[i].texture_hash);
        verts[0].color.a = 255;

        verts[1].position.x = (float)project_x(map->triangles[i].x1, map->triangles[i].z1);
        verts[1].position.y = (float)project_y(map->triangles[i].y1, map->triangles[i].z1);
        verts[1].color.r = mat_r(map->triangles[i].texture_hash);
        verts[1].color.g = mat_g(map->triangles[i].texture_hash);
        verts[1].color.b = mat_b(map->triangles[i].texture_hash);
        verts[1].color.a = 255;

        verts[2].position.x = (float)project_x(map->triangles[i].x2, map->triangles[i].z2);
        verts[2].position.y = (float)project_y(map->triangles[i].y2, map->triangles[i].z2);
        verts[2].color.r = mat_r(map->triangles[i].texture_hash);
        verts[2].color.g = mat_g(map->triangles[i].texture_hash);
        verts[2].color.b = mat_b(map->triangles[i].texture_hash);
        verts[2].color.a = 255;

        SDL_RenderGeometry(PCRenderSDLGetRenderer(), 0, verts, 3, 0, 0);
    }
}

void PCMapRuntimeDestroy(PCMapRuntime* map) {
    if (!map) {
        return;
    }

    free(map->lines);
    free(map->triangles);
    free(map);
}
