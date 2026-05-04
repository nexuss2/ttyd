# TTYD PC Port Input Plan

Current input entry point:
- src/system.c
- makeKey()
- calls DEMOPadRead()
- reads DemoPad[i]

Game internal state:
- gp->dirs
- gp->dirsNew
- gp->dirsRepeat
- gp->button
- gp->buttonNew
- gp->buttonRepeat
- gp->buttonUp
- gp->stickX
- gp->stickY
- gp->substickX
- gp->substickY
- gp->triggerLeft
- gp->triggerRight

PC port target:
- replace DEMOPadRead() backend
- keep makeKey() logic mostly unchanged
- feed DemoPad[] from SDL keyboard/controller input

First abstraction:
PlatformInput_Update()
PlatformInput_GetPadStatus(chan)
PlatformInput_SetRumble(chan, state)

Next build step:
- Do not add src/platform/pc/input.c to the normal GameCube decomp build yet.
- It should only compile when PLATFORM_PC is enabled.
- Future PC build needs a separate config/target, not the matching G8MJ01 build.
