# Third-party notices

RTextJP itself is licensed under the [MIT License](LICENSE).

## raylib

RTextJP is an unofficial helper library built on top of raylib and is not part of the official
raylib distribution. The source repository does not need to contain a copy of raylib. When
`RTEXTJP_FETCH_RAYLIB=ON` is selected, CMake downloads the official raylib `6.0` tag from:

- <https://github.com/raysan5/raylib>

raylib is distributed under the zlib/libpng license. Its license is available in the official
repository and is copied into Windows demo archives created by this project.

## Noto Sans JP

The source repository does not commit `examples/resources/japanese.ttf`. A local copy can be
placed there for running examples or producing a demo archive. When the local font is Noto Sans
JP, its accompanying license and upstream documentation are kept in:

- `examples/resources/OFL.txt`
- `examples/resources/README.txt`

Noto Sans JP is distributed under the SIL Open Font License 1.1. The optional Windows demo archive
copies the font license and documentation alongside the font. Do not replace the font without also
reviewing and including the replacement font's redistribution terms.
