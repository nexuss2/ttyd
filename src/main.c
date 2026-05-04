#include "main.h"

#include "mario/mariost.h"
#include "sdk/DEMOInit.h"

#ifdef PLATFORM_PC
OSTick PlatformGetTick(void);
#endif

extern GlobalWork* gp;

int main(void) {
    OSTick base_tick;

    marioStInit();
    marioStMain();
    while (1) {
#ifdef PLATFORM_PC
        base_tick = PlatformGetTick();
#else
        base_tick = OSGetTick();
#endif
        DEMOBeforeRender();
        marioStDisp();
        marioStMain();
#ifdef PLATFORM_PC
        gp->mDeltaGame = PlatformGetTick() - base_tick;
#else
        gp->mDeltaGame = OSGetTick() - base_tick;
#endif
        DEMODoneRender();
#ifdef PLATFORM_PC
        gp->mDeltaRender = PlatformGetTick() - base_tick;
#else
        gp->mDeltaRender = OSGetTick() - base_tick;
#endif
    }

    return 0;
}
