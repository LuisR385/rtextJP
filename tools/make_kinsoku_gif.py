"""Combine the two real RTextJP captures into a small looping documentation GIF."""

from __future__ import annotations

import argparse
from pathlib import Path

from PIL import Image


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("asset_directory", type=Path)
    args = parser.parse_args()
    directory = args.asset_directory.resolve()
    paths = [directory / "kinsoku-on.png", directory / "kinsoku-off.png"]
    frames = [Image.open(path).convert("P", palette=Image.Palette.ADAPTIVE) for path in paths]
    output = directory / "kinsoku-toggle.gif"
    frames[0].save(output, save_all=True, append_images=frames[1:], duration=1300,
                   loop=0, optimize=True, disposal=2)
    print(f"updated {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
