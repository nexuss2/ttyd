# Runtime and Render Notes

Current status:
- PC runtime health check exists.
- PC game loop shell exists.
- Game loop can load selectable map/message content.
- Terminal render shell exists.
- Render flow currently supports:
  - init
  - begin frame
  - clear
  - end frame
  - shutdown

This is not real rendering yet. It is a safe render-loop bridge before adding SDL/OpenGL or another window backend.

Next:
- Keep terminal render shell as the fallback backend.
- Later add a real window backend when SDL2/OpenGL is available.
- Avoid full GX rendering until the render backend is stable.
