#include <stdlib.h>
#include <string.h>

void* __memAlloc(int heap, unsigned int size) {
    (void)heap;
    return malloc(size);
}

void __memFree(int heap, void* ptr) {
    (void)heap;
    free(ptr);
}

void* __memClear(void* ptr, unsigned int size) {
    return memset(ptr, 0, size);
}
