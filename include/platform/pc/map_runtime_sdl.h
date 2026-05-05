#pragma once

typedef struct PCMapRuntime PCMapRuntime;

PCMapRuntime* PCMapRuntimeLoad(const char* map);
void PCMapRuntimeSetView(float offset_x, float offset_y, float scale);
void PCMapRuntimeDrawWire(PCMapRuntime* map);
void PCMapRuntimeDestroy(PCMapRuntime* map);
