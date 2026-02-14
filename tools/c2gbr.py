#!/usr/bin/env python3
"""
Convert BackgroundTileSet.c (Game Boy 2bpp hex) to GBTD .gbr file format.

Usage: python3 tools/c2gbr.py sprites/BackgroundTileSet.c sprites/BackgroundTileSet.gbr
"""

import sys
import struct
import re


def parse_c_hex_array(filepath):
    """Extract hex bytes from a C array file."""
    with open(filepath, "r") as f:
        content = f.read()

    # Find all hex values (0xNN)
    hex_values = re.findall(r"0x([0-9A-Fa-f]{2})", content)
    return bytes(int(h, 16) for h in hex_values)


def twobpp_to_pixels(tile_data_2bpp):
    """Convert 2bpp Game Boy tile data to 1-byte-per-pixel format.

    In 2bpp: each row is 2 bytes (low, high). For pixel X (0=leftmost):
      color = ((high >> (7-X)) & 1) << 1 | ((low >> (7-X)) & 1)

    In .gbr: each pixel is 1 byte (0-3), stored row by row, left to right.
    """
    pixels = bytearray()
    num_tiles = len(tile_data_2bpp) // 16

    for t in range(num_tiles):
        offset = t * 16
        for row in range(8):
            low = tile_data_2bpp[offset + row * 2]
            high = tile_data_2bpp[offset + row * 2 + 1]
            for x in range(8):
                bit = 7 - x
                color = ((high >> bit) & 1) << 1 | ((low >> bit) & 1)
                pixels.append(color)

    return bytes(pixels)


def make_string(s, length):
    """Create a null-padded fixed-length string."""
    encoded = s.encode("ascii")[:length]
    return encoded + b"\x00" * (length - len(encoded))


def write_record(f, record_type, record_id, data):
    """Write a .gbr record: type(2) + id(2) + length(4) + data."""
    f.write(struct.pack("<HHI", record_type, record_id, len(data)))
    f.write(data)


def create_gbr(tile_pixels, num_tiles, output_path, tileset_name="BackgroundTileSet"):
    """Create a GBTD v2.2 .gbr file."""
    with open(output_path, "wb") as f:
        # File header
        f.write(b"GBO0")

        # Record 0: Producer (type 0x01)
        producer = bytearray()
        producer += make_string("Gameboy Tile Designer", 30)
        producer += make_string("2.2", 10)
        producer += make_string("Home: www.casema.net/~hpmulder", 80)
        write_record(f, 0x01, 0, bytes(producer))

        # Record 1: Tile Data (type 0x02)
        tile_data = bytearray()
        tile_data += make_string(tileset_name, 30)
        tile_data += struct.pack("<HH", 8, 8)         # width, height
        tile_data += struct.pack("<H", num_tiles)       # tile count
        tile_data += bytes([0x00, 0x01, 0x02, 0x03])   # color set
        tile_data += tile_pixels                         # 1 byte per pixel
        write_record(f, 0x02, 1, bytes(tile_data))

        # Record 2: Tile Settings (type 0x03) - 19 bytes for v2.2
        tile_settings = bytearray()
        tile_settings += struct.pack("<H", 1)     # tile_id (references record 1)
        tile_settings += bytes([1])                # simple = true
        tile_settings += bytes([0])                # flags
        tile_settings += bytes([0])                # left_color
        tile_settings += bytes([3])                # right_color
        tile_settings += struct.pack("<HH", 8, 8) # split_width, split_height
        tile_settings += bytes([0])                # split_order (horizontal)
        tile_settings += bytes([1])                # color_set (Game Boy)
        tile_settings += struct.pack("<HHH", 0, 0, 0)  # bookmarks
        tile_settings += bytes([0])                # auto_update
        write_record(f, 0x03, 2, bytes(tile_settings))

        # Record 3: Tile Export (type 0x04) - 195 bytes for v2.2
        tile_export = bytearray()
        tile_export += struct.pack("<H", 1)                  # tile_id
        tile_export += make_string(tileset_name + ".c", 128) # file_name
        tile_export += bytes([3])                             # file_type (GBDK C)
        tile_export += make_string("TILES", 20)              # section_name
        tile_export += make_string(tileset_name, 20)         # label_name
        tile_export += bytes([0])                             # bank
        tile_export += bytes([1])                             # tile_array
        tile_export += bytes([0])                             # format (GB 4-color)
        tile_export += bytes([0])                             # counter
        tile_export += struct.pack("<HH", 0, num_tiles - 1)  # from, upto
        tile_export += bytes([0])                             # compression (none)
        tile_export += bytes([0])                             # include_colors
        tile_export += bytes([0])                             # sgb_palettes
        tile_export += bytes([0])                             # gbc_palettes
        tile_export += bytes([0])                             # make_meta_tiles
        tile_export += struct.pack("<I", 0)                   # meta_offset
        tile_export += bytes([0])                             # meta_counter
        tile_export += bytes([0])                             # split
        tile_export += struct.pack("<I", 0)                   # block_size
        tile_export += bytes([0])                             # sel_tab
        write_record(f, 0x04, 3, bytes(tile_export))

        # Record 4: Tile Import (type 0x05) - 143 bytes for v2.2
        tile_import = bytearray()
        tile_import += struct.pack("<H", 1)                  # tile_id
        tile_import += make_string("", 128)                  # file_name
        tile_import += bytes([0])                             # file_type
        tile_import += struct.pack("<HHH", 0, 0, 0)         # from, to, count
        tile_import += bytes([0])                             # color_conversion
        tile_import += struct.pack("<I", 0)                   # first_byte
        tile_import += bytes([0])                             # binary_file_type
        write_record(f, 0x05, 4, bytes(tile_import))

        # Record 5: Palettes (type 0x0D)
        palettes = bytearray()
        palettes += struct.pack("<H", 1)       # id
        palettes += struct.pack("<H", 8)       # count (8 CGB palettes)
        # 8 palettes x 4 colors x 4 bytes = 128 bytes
        # Default DMG-like palette: black, dark gray, light gray, white
        dmg_colors = [
            (0x00, 0x00, 0x00, 0x00),  # Color 0: white     -> 0,0,0 (lightest)
            (0x60, 0x60, 0x60, 0x00),  # Color 1: light gray
            (0xC0, 0xC0, 0xC0, 0x00),  # Color 2: dark gray
            (0xF8, 0xF8, 0xF8, 0x00),  # Color 3: black     -> F8,F8,F8 (darkest)
        ]
        for pal in range(8):
            for color in dmg_colors:
                palettes += bytes(color)
        palettes += struct.pack("<H", 4)       # sgb_count
        for pal in range(8):  # SGB palettes (8 slots)
            for color in dmg_colors:
                palettes += bytes(color)
        write_record(f, 0x0D, 5, bytes(palettes))

        # Record 6: Tile Pal (type 0x0E)
        tile_pal = bytearray()
        tile_pal += struct.pack("<H", 1)           # id
        tile_pal += struct.pack("<H", num_tiles)   # count
        for _ in range(num_tiles):
            tile_pal += struct.pack("<I", 0)       # CGB palette 0 for each tile
        tile_pal += struct.pack("<H", num_tiles)   # sgb_count
        for _ in range(num_tiles):
            tile_pal += struct.pack("<I", 0)       # SGB palette 0 for each tile
        write_record(f, 0x0E, 6, bytes(tile_pal))


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <input.c> <output.gbr>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    # Parse the C hex array
    raw_2bpp = parse_c_hex_array(input_path)
    num_tiles = len(raw_2bpp) // 16
    print(f"Found {num_tiles} tiles ({len(raw_2bpp)} bytes of 2bpp data)")

    # Convert 2bpp to 1-byte-per-pixel
    pixels = twobpp_to_pixels(raw_2bpp)
    print(f"Expanded to {len(pixels)} bytes (1 byte per pixel)")

    # Generate .gbr file
    create_gbr(pixels, num_tiles, output_path)
    print(f"Written: {output_path}")


if __name__ == "__main__":
    main()
