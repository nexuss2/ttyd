#include <time.h>

typedef unsigned int OSTick;

OSTick PlatformGetTick(void) {
    return (OSTick)clock();
}
