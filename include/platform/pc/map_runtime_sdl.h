#pragma once

typedef struct PCMapRuntime PCMapRuntime;

PCMapRuntime* PCMapRuntimeLoad(const char* map);
void PCMapRuntimeDrawWire(PCMapRuntime* map);
void PCMapRuntimeDestroy(PCMapRuntime* map);
