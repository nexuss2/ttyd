#include <dolphin/pad.h>

void PlatformInputInit(void) {
}

void PlatformInputRead(PADStatus* pads) {
    int i;

    for (i = 0; i < 4; i++) {
        pads[i].button = 0;
        pads[i].stickX = 0;
        pads[i].stickY = 0;
        pads[i].substickX = 0;
        pads[i].substickY = 0;
        pads[i].triggerLeft = 0;
        pads[i].triggerRight = 0;
        pads[i].analogA = 0;
        pads[i].analogB = 0;
        pads[i].err = 0;
    }
}

void PlatformInputReset(unsigned int resetMask) {
}
