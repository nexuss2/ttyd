# TTYD PC Port Input Seam

Game input flow:
- src/system.c -> makeKey()
- makeKey() calls DEMOPadRead()
- makeKey() reads DemoPad[i]
- game code reads gp->button, gp->buttonNew, gp->buttonRepeat, gp->stickX, gp->stickY

Current backend:
- libs/dolsdk2004/src/demo/DEMOPad.c
- DEMOPadRead()
- uses PADRead(), PADClamp(), PADReset()

PC port target:
- keep makeKey()
- keep DEMOPadCopy()
- replace PADRead/PADClamp/PADReset path with PC input backend

Useful button values:
- LEFT  = 0x0001
- RIGHT = 0x0002
- DOWN  = 0x0004
- UP    = 0x0008
- A     = 0x0100
- B     = 0x0200
- X     = 0x0400
- Y     = 0x0800
- START = 0x1000

First PC mapping:
- Enter -> START
- Z -> A
- X -> B
- A -> X
- S -> Y
- Arrow keys -> D-pad and/or stickX/stickY
