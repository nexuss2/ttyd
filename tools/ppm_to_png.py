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

patterns = sys.argv[1:] or ["build/pc/*.ppm"]

for pattern in patterns:
    for ppm in glob.glob(pattern):
        png = ppm.rsplit(".", 1)[0] + ".png"
        w, h, rgb = read_ppm(ppm)
        write_png(png, w, h, rgb)
        print("wrote", png)
