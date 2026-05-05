import glob
import struct
import zlib
import sys

def read_ppm(path):
    with open(path, "rb") as f:
        if f.readline().strip() != b"P6":
            raise ValueError(f"{path}: not P6")
        line = f.readline().strip()
        while line.startswith(b"#"):
            line = f.readline().strip()
        w, h = map(int, line.split())
        if int(f.readline().strip()) != 255:
            raise ValueError(f"{path}: unsupported max value")
        return w, h, f.read()

def chunk(tag, data):
    return struct.pack(">I", len(data)) + tag + data + struct.pack(">I", zlib.crc32(tag + data) & 0xffffffff)

def write_png(path, w, h, rgb):
    raw = b"".join(b"\x00" + rgb[y*w*3:(y+1)*w*3] for y in range(h))
    data = b"\x89PNG\r\n\x1a\n"
    data += chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
    data += chunk(b"IDAT", zlib.compress(raw))
    data += chunk(b"IEND", b"")
    with open(path, "wb") as f:
        f.write(data)

prefix = sys.argv[1] if len(sys.argv) > 1 else "aaa_00"
files = sorted(glob.glob(f"build/pc/{prefix}_*.ppm"))

cell = 160
cols = 6
rows = (len(files) + cols - 1) // cols
sheet_w = cols * cell
sheet_h = max(1, rows) * cell
sheet = bytearray([32] * (sheet_w * sheet_h * 3))

for idx, path in enumerate(files):
    w, h, rgb = read_ppm(path)
    col = idx % cols
    row = idx // cols
    ox = col * cell + max(0, (cell - w) // 2)
    oy = row * cell + max(0, (cell - h) // 2)

    for y in range(min(h, cell)):
        for x in range(min(w, cell)):
            src = (y * w + x) * 3
            dst = ((oy + y) * sheet_w + (ox + x)) * 3
            if 0 <= dst < len(sheet) - 2:
                sheet[dst:dst+3] = rgb[src:src+3]

out = f"build/pc/{prefix}_sheet.png"
write_png(out, sheet_w, sheet_h, bytes(sheet))
print("wrote", out, "textures", len(files))
