# PC Port Next Steps

## Done

- PC file backend
- PC DVD manager shim
- PC arcOpen remapping
- PC runtime health check
- Map loading and parsing
- Texture palette parsing
- Texture export for I4, I8, IA4, RGB565 and CMPR
- Map texture batch export and contact sheets
- Message file parsing and Shift-JIS to UTF-8 export
- Sound asset discovery and STM header inspection

## Next

1. Build a cleaner platform API layer.
2. Reduce noisy debug output in map and texture tools.
3. Add real command-line tools for:
   - map info export
   - texture export
   - message export
   - STM inspection
4. Start a minimal game loop shell:
   - init platform
   - init file/DVD/ARC
   - load one map
   - load one message file
   - tick loop
5. Later:
   - renderer/window backend
   - input backend
   - audio playback
   - language branch experiments
