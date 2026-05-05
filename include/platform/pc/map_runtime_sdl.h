#pragma once
#include "platform/pc/map_texture_runtime_sdl.h"

typedef struct PCMapRuntime PCMapRuntime;

PCMapRuntime* PCMapRuntimeLoad(const char* map);
void PCMapRuntimeSetView(float offset_x, float offset_y, float scale);
void PCMapRuntimeUseAutoView(PCMapRuntime* map, int width, int height);
void PCMapRuntimeDrawWire(PCMapRuntime* map);
void PCMapRuntimeDrawFilled(PCMapRuntime* map, PCMapTextureSet* textures);
void PCMapRuntimeDestroy(PCMapRuntime* map);
