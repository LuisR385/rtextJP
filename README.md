# RTextJP

<p align="center">
  <strong>English</strong> · <a href="README_ja.md">日本語</a>
</p>
<p align="center">
  <img alt="Language: C99" src="https://img.shields.io/badge/language-C99-A8B9CC?style=flat-square&logo=c&logoColor=white">
  <img alt="C++ compatible header" src="https://img.shields.io/badge/C%2B%2B-header%20compatible-00599C?style=flat-square&logo=cplusplus&logoColor=white">
  <img alt="Single header" src="https://img.shields.io/badge/distribution-single--header-6E40C9?style=flat-square">
  <a href="https://github.com/raysan5/raylib"><img alt="raylib 6.0 or later" src="https://img.shields.io/badge/raylib-6.0%2B-000000?style=flat-square&logo=raylib&logoColor=white"></a>
  <img alt="Platforms: Windows, Linux, macOS" src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-2F80ED?style=flat-square">
  <img alt="Build: CMake 3.22 or later" src="https://img.shields.io/badge/CMake-3.22%2B-064F8C?style=flat-square&logo=cmake&logoColor=white">
  <a href="LICENSE"><img alt="License: MIT" src="https://img.shields.io/badge/license-MIT-green?style=flat-square"></a>
</p>

RTextJP is an unofficial C99 single-header helper for drawing Japanese-friendly UTF-8 text with
[raylib](https://www.raylib.com/). It adds lightweight Japanese/English wrapping, kinsoku rules,
alignment, simple vertical layout, Japanese font character-set presets, and fixed-buffer UTF-8
input helpers.

This project is not part of the official raylib distribution.

![Real RTextJP output showing wrapping, alignment, vertical text, and input](docs/assets/rtextjp-overview.png)

## Features

- UTF-8 decoding, encoding, validation, codepoint counting, and byte-index conversion
- Font loading from the exact text used by an application
- JIS X 0208 kana, non-kanji, level-1, and level-2 character-set presets
- Japanese character wrapping and English word-boundary wrapping
- Lightweight line-start and line-end kinsoku handling
- Horizontal and vertical alignment inside rectangles
- Simple right-to-left or left-to-right vertical columns for game UI
- Fixed-capacity single-line UTF-8 input state
- C99 implementation with a C++-compatible declaration section
- Tests for UTF-8, layout, character sets, ASan, and UBSan

Kinsoku behavior can be toggled without changing the text or box width:

![Kinsoku enabled and disabled](docs/assets/kinsoku-toggle.gif)

## Supported environments

| Area | Support |
|---|---|
| Implementation language | C99 |
| Consumer language | C; C++ can include and call the C API |
| raylib | 6.0 or later |
| Build system | CMake 3.22 or later, or direct compiler integration |
| Platforms | Windows, GNU/Linux, and macOS where raylib 6.0 is available |
| Source encoding | UTF-8 |

The Windows build is verified with MSVC and Clang. The implementation has no platform-specific
drawing backend; platform behavior follows raylib. The example font finder contains candidates for
Windows, macOS, and common GNU/Linux Noto font locations.

## Minimal use

Add `include/rtextjp.h` to a project that already uses raylib. In exactly one C translation unit:

```c
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"

int main(void)
{
    const char *message = "こんにちは、raylib!";

    InitWindow(800, 450, "RTextJP example");
    Font font = RTextJPLoadFontFromText("japanese.ttf", 48, message);
    RTextJPStyle style = RTextJPStyleDefault(font, 32.0f, BLACK);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        RTextJPDraw(message, (Vector2){ 40.0f, 40.0f }, style);
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
```

Other translation units include the header without defining `RTEXTJP_IMPLEMENTATION`.

## CMake development environments

RTextJP does not require a committed raylib binary. The shared development preset enables the
pinned raylib `6.0` FetchContent fallback, while CMake still prefers an installed raylib package.
The first fallback configure requires Git and network access; later runs use CMake's cached copy.

### Visual Studio 2022 / MSVC development

The Visual Studio presets require CMake and the Visual Studio 2022 C++ workload. They use the
64-bit MSVC toolchain and do not require Ninja or LLVM. Configure once, then choose a configuration
through a build preset:

```sh
cmake --preset vs2022
cmake --build --preset vs2022-debug
```

For an optimized build:

```sh
cmake --build --preset vs2022-release
```

The `vs2022` configure preset enables the examples and writes the Visual Studio solution to
`build-vs2022/`. It deliberately does not set `CMAKE_BUILD_TYPE`, because Visual Studio is a
multi-config generator. `START_HERE.bat` remains available as the menu-driven Windows launcher.

### Visual Studio 2022 / MSVC tests

The test preset uses a separate build tree with examples disabled and tests enabled:

```sh
cmake --preset vs2022-tests
cmake --build --preset vs2022-tests-debug
ctest --preset vs2022-tests-debug
```

### Ninja, clangd, and Zed completion

Visual Studio generators do not produce `compile_commands.json`. Developers who have Ninja and a
C compiler available can generate one in a dedicated directory:

```sh
cmake --preset clangd
```

On Windows, an **x64 Native Tools Command Prompt for VS 2022** makes MSVC available to Ninja. The
preset does not hard-code `CMAKE_C_COMPILER`, `CMAKE_MAKE_PROGRAM`, an LLVM installation, or any
machine-specific path. It enables examples and tests, while leaving the optional docs gallery off.

The command creates `build-clangd/compile_commands.json`; building that tree is not required for
completion. The repository `.clangd` points to it with a relative path, and Zed's C support uses
clangd. Re-run the configure command after changing sources, CMake options, or toolchains. If Zed
was already open, run `editor: restart language server` once from the command palette.

A compilation database contains absolute paths from the machine that generated it. Therefore
`build-clangd/` and a root `compile_commands.json` are ignored and must not be committed.

### Local compiler and Ninja paths

`CMakeUserPresets.json` is ignored by Git and is the appropriate place for machine-specific
overrides. If Ninja or a compiler is not on `PATH`, create a local preset such as `clangd-local`
that inherits `clangd`, set `CMAKE_MAKE_PROGRAM` and/or `CMAKE_C_COMPILER` there, and run that local
preset. Do not add those paths to `CMakePresets.json` or `.clangd`.

### Other generators and sanitizers

On other platforms, select the usual local generator or let CMake select its default:

```sh
cmake -S . -B build -DRTEXTJP_BUILD_EXAMPLES=ON -DRTEXTJP_FETCH_RAYLIB=ON
cmake --build build
```

ASan and UBSan remain available with Clang or GCC:

```sh
cmake -S . -B build-sanitize -DCMAKE_C_COMPILER=clang -DRTEXTJP_BUILD_EXAMPLES=OFF -DRTEXTJP_BUILD_TESTS=ON -DRTEXTJP_FETCH_RAYLIB=ON -DRTEXTJP_ENABLE_SANITIZERS=ON
cmake --build build-sanitize
ctest --test-dir build-sanitize --output-on-failure
```

## Japanese fonts

raylib's default font does not contain Japanese glyphs. Pass a TTF/OTF path and list all text that
must be available in the font atlas:

```c
const char *screenText =
    "設定を保存しますか？"
    "はい"
    "いいえ"
    "Settings are saved.";

Font font = RTextJPLoadFontFromText("japanese.ttf", 48, screenText);
```

For text that cannot be known in advance, use a Japanese character-set preset and add custom
characters separately:

```c
RTextJPCharacterSet set = (RTextJPCharacterSet)(
    RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI |
    RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1);

Font font = RTextJPLoadFontFromCharacterSet("japanese.ttf", 32, set, "髙島");
```

The source repository intentionally ignores `examples/resources/japanese.ttf`. For local examples,
place a redistributable Japanese font there, set `RTEXTJP_FONT_PATH`, or pass the font path as the
first command-line argument. The included `OFL.txt` and `README.txt` describe the optional local
Noto Sans JP copy.

## Windows demo archive

A maintainer can produce the separate release asset described in the Japanese documentation. It
contains one ready-to-run example, the raylib DLL, the local Japanese font, and all relevant license
files without committing those binaries to the source repository.

```text
RTextJP-demo-windows.zip
├── rtextjp_01_hello_japanese.exe
├── raylib.dll
├── japanese.ttf
├── README.md
├── README_ja.md
├── README-font.txt
├── LICENSE-RTextJP.txt
├── LICENSE-raylib.txt
└── LICENSE-font.txt
```

Create it from a Windows developer shell after placing Noto Sans JP at
`examples/resources/japanese.ttf`:

```text
cmake -S . -B build-demo -G "NMake Makefiles" -DRTEXTJP_BUILD_EXAMPLES=ON -DRTEXTJP_FETCH_RAYLIB=ON -DRTEXTJP_BUILD_DEMO_PACKAGE=ON -DBUILD_SHARED_LIBS=ON
cmake --build build-demo --target rtextjp_demo_archive
```

The archive is written to `build-demo/packages/RTextJP-demo-windows.zip`.

## Examples

| Source | Demonstrates |
|---|---|
| `01_hello_japanese.c` | Minimal setup, font loading, basic drawing |
| `02_text_box.c` | Japanese/English wrapping and kinsoku |
| `03_alignment.c` | Horizontal/vertical alignment and measurement |
| `04_vertical_writing.c` | Simple vertical columns |
| `05_text_input.c` | Fixed-buffer UTF-8 input |
| `06_utf8_tools.c` | UTF-8 inspection GUI |
| `07_character_sets.c` | JIS preset selection and custom characters |

## Documentation

- [Japanese README](README_ja.md)
- [Japanese quick start](docs/quickstart_ja.md)
- [Japanese API guide](docs/api_ja.md)
- [Japanese character-set guide](docs/character_sets_ja.md)
- [Japanese gallery and capture instructions](docs/gallery_ja.md)
- [Japanese design notes](docs/design_ja.md)
- [Japanese input and IME notes](docs/ime_ja.md)

Public declarations also contain short English Doxygen descriptions.

## Current limitations

- Kinsoku support is intentionally lightweight and is not a full JIS X 4051 typesetting engine.
- Vertical text uses grid placement and does not apply OpenType `vert`/`vrt2` substitutions.
- IME composition and candidate windows are not drawn; committed UTF-8 can be inserted.
- Character-set presets request glyphs from a font but cannot create glyphs absent from that font.
- A very large character set increases atlas creation time and VRAM usage.
- Nested raylib scissor modes are not supported; set `style.clip = false` inside an existing one.

## License

RTextJP is licensed under the [MIT License](LICENSE), copyright © 2026 LuisR385.

raylib and optional fonts retain their own licenses. See [Third-party notices](THIRD_PARTY_NOTICES.md)
and the license files shipped with a demo archive.
