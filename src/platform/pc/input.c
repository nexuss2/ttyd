typedef unsigned short u16;
typedef signed char s8;

typedef struct PADStatus {
    u16 button;
    s8 stickX;
    s8 stickY;
    s8 substickX;
    s8 substickY;
    unsigned char triggerLeft;
    unsigned char triggerRight;
    unsigned char analogA;
    unsigned char analogB;
    signed char err;
} PADStatus;

#define PAD_BUTTON_A 0x0100

static u16 s_buttons;
static s8 s_stick_x;
static s8 s_stick_y;

void PlatformInputInit(void) {
    s_buttons = 0;
    s_stick_x = 0;
    s_stick_y = 0;
}

void PlatformInputRead(PADStatus* pads) {
    int i;

    s_buttons = PAD_BUTTON_A;
    s_stick_x = 0;
    s_stick_y = 80;

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

    pads[0].button = s_buttons;
    pads[0].stickX = s_stick_x;
    pads[0].stickY = s_stick_y;
}

void PlatformInputReset(unsigned int resetMask) {
    (void)resetMask;
}
