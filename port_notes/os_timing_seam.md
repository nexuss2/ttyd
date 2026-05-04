# TTYD PC Port OS / Timing Seam

Important calls found:
- OSGetTick()
- OSGetTime()
- OSTicksToSeconds()
- OSTicksToMilliseconds()
- OSYieldThread()
- OSCreateThread()
- OSResumeThread()
- OSCancelThread()

Best first target:
- main.c uses OSGetTick() for frame delta timing
- dvdmgr.c uses thread/yield behaviour
- mariost.c has DVD check thread and idle timeout logic

PC port target:
- keep game timing logic
- replace OS time/thread backend later with PlatformTime / PlatformThread
