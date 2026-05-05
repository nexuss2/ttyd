# PC Port Milestone

Current status:
- Native PC runner builds and runs.
- PC file backend works with extracted game files.
- DVD-style file access works through PC shim.
- arcOpen-style path remapping works.
- Map files load and parse.
- Texture palettes parse.
- Several GameCube texture formats export to viewable images.
- Map texture batch export and contact sheets work.
- Message files load and can be exported/decoded from Shift-JIS to UTF-8.
- Core sound files and STM stream headers can be inspected.

Major systems smoke-tested:
- Files
- DVD manager
- ARC remapping
- Textures
- Maps
- Messages
- Sound asset discovery

Next major phase:
- Build a cleaner PC runtime layer.
- Start replacing one-off smoke tests with reusable platform APIs.
- Begin stepping toward a minimal game loop.
