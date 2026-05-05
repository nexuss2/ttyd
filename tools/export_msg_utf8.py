from pathlib import Path
import sys

if len(sys.argv) < 2:
    print("usage: python3 tools/export_msg_utf8.py msg_name_without_txt")
    print("example: python3 tools/export_msg_utf8.py hei_00")
    raise SystemExit(1)

name = sys.argv[1]
src = Path(f"orig/G8MJ01/files/msg/{name}.txt")
dst = Path(f"build/pc/{name}_utf8.txt")

if not src.exists():
    print(f"missing {src}")
    raise SystemExit(1)

data = src.read_bytes()
text = data.decode("shift_jis", errors="replace")
dst.write_text(text, encoding="utf-8")

print(f"wrote {dst}")
