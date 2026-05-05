# Audio Notes

Sound assets discovered:
- sound/proj/pmario.pool
- sound/proj/pmario.proj
- sound/proj/pmario.sdir
- sound/proj/pmario.samp
- sound/proj/pmario.slib
- sound/proj/pmario.stbl
- sound/proj/pmario.etbl
- 146 streamed .stm files in sound/stream

Confirmed:
- PC file backend opens core sound project files.
- PC file backend opens STM stream files.
- STM header inspector reads structured big-endian fields.
- 0x00007d00 appears in STM headers and likely represents 32000 Hz.

Not done yet:
- STM decoding/playback.
- MusyX project parsing.
- SFX/sample playback.
