# TTYD PC Port Filesystem Seam

Central file:
- src/manager/dvdmgr.c

Most game systems call:
- DVDMgrOpen()
- DVDMgrRead()
- DVDMgrReadAsync()
- DVDMgrClose()
- DVDMgrGetLength()

Current GameCube backend:
- DVDOpen()
- DVDRead()
- OSYieldThread()
- DVD callbacks

PC port target:
- keep game code calling DVDMgr*
- replace internals with PlatformFile* when PLATFORM_PC is enabled

Future PC backend ideas:
- map game paths to extracted orig/G8MJ01/files/
- use fopen/fread/fseek or stdio
- make async initially behave synchronously, then call callback immediately
