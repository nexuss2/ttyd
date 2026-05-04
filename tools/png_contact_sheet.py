import glob
import struct
import zlib

def read_png_size(path):
    with open(path, "rb") as f:
        sig = f.read(8)
        if sig != b"\x89PNG\r\n\x1a\n":
            raise ValueError(f"{path}: not png")
        length = struct.unpack(">I", f.read(4))[0]
        tag = f.read(4)
        if tag != b"IHDR":
            raise ValueError(f"{path}: missing IHDR")
        data = f.read(length)
        w, h = struct.unpack(">II", data[:8])
        return w, h

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

files = sorted(glob.glob("build/pc/aaa_00_cmpr_*.ppm"))
cell = 160
cols = 6
rows = (len(files) + cols - 1) // cols
sheet_w = cols * cell
sheet_h = rows * cell
sheet = bytearray([32] * (sheet_w * sheet_h * 3))

for idx, path in enumerate(files):
    w, h, rgb = read_ppm(path)
    col = idx % cols
    row = idx // cols
    ox = col * cell + (cell - w) // 2
    oy = row * cell + (cell - h) // 2

    for y in range(h):
        for x in range(w):
            if 0 <= ox + x < sheet_w and 0 <= oy + y < sheet_h:
                src = (y * w + x) * 3
                dst = ((oy + y) * sheet_w + (ox + x)) * 3
                sheet[dst:dst+3] = rgb[src:src+3]

write_png("build/pc/aaa_00_cmpr_sheet.png", sheet_w, sheet_h, bytes(sheet))
print("wrote build/pc/aaa_00_cmpr_sheet.png", sheet_w, sheet_h, "textures", len(files))
