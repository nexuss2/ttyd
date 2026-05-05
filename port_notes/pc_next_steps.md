# PC Port Next Steps

## Done

- PC file backend
- PC DVD manager shim
- PC arcOpen remapping
- PC runtime health check
- PC game loop shell
- PC terminal render shell
- PC platform umbrella header
- Map loading and parsing
- Texture palette parsing
- Texture export for I4, I8, IA4, RGB565 and CMPR
- Map texture batch export and contact sheets
- Message file parsing and Shift-JIS to UTF-8 export
- Sound asset discovery and STM header inspection

## Next

1. Keep the PC layer organised with headers and reusable APIs.
2. Reduce debug noise in parsers.
3. Build a real window backend when SDL/OpenGL is available.
4. Start a minimal renderer:
   - open window
   - clear frame
   - draw one exported texture
   - then later draw map texture quads
5. Keep language/ROM hack experiments on a separate branch.
