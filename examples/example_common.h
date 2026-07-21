#ifndef RTEXTJP_EXAMPLE_COMMON_H
#define RTEXTJP_EXAMPLE_COMMON_H

/*
 * example共通のフォント探索とエラー画面。
 *
 * ダブルクリックではカレントディレクトリが一定しないため、実行ファイルの場所、
 * リポジトリ、環境変数、Windows標準フォントの順に探します。
 */
#include <stdio.h>
#include <stdlib.h>

static const char *ExampleTryApplicationRelative(const char *relativePath)
{
    static char fullPath[1024];
    const char *applicationDirectory = GetApplicationDirectory();

    if ((applicationDirectory == NULL) || (relativePath == NULL)) return NULL;
    if (snprintf(fullPath, sizeof(fullPath), "%s/%s", applicationDirectory, relativePath) < 0) {
        return NULL;
    }
    fullPath[sizeof(fullPath) - 1] = '\0';
    return FileExists(fullPath) ? fullPath : NULL;
}

static const char *ExampleEnvironmentFontPath(void)
{
#if defined(_MSC_VER)
    static char copiedPath[1024];
    char *allocatedPath = NULL;
    size_t length = 0;

    if ((_dupenv_s(&allocatedPath, &length, "RTEXTJP_FONT_PATH") != 0) ||
        (allocatedPath == NULL)) return NULL;
    if (snprintf(copiedPath, sizeof(copiedPath), "%s", allocatedPath) < 0) {
        free(allocatedPath);
        return NULL;
    }
    copiedPath[sizeof(copiedPath) - 1] = '\0';
    free(allocatedPath);
    return copiedPath;
#else
    return getenv("RTEXTJP_FONT_PATH");
#endif
}

static const char *ExampleFindJapaneseFont(int argc, char **argv)
{
    const char *environmentPath = ExampleEnvironmentFontPath();
    const char *found = NULL;
    int i = 0;
    static const char *workingDirectoryCandidates[] = {
        "examples/resources/japanese.ttf",
        "examples/resources/japanese.otf",
        "resources/japanese.ttf",
        "resources/japanese.otf",
        "japanese.ttf",
        "japanese.otf",
        NULL
    };
    static const char *systemFontCandidates[] = {
#if defined(_WIN32)
        "C:/Windows/Fonts/NotoSansJP-VF.ttf",
        "C:/Windows/Fonts/YuGothM.ttc",
        "C:/Windows/Fonts/meiryo.ttc",
        "C:/Windows/Fonts/msgothic.ttc",
#elif defined(__APPLE__)
        "/System/Library/Fonts/ヒラギノ角ゴシック W3.ttc",
#else
        "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
        "/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc",
#endif
        NULL
    };
    static const char *applicationRelativeCandidates[] = {
        "japanese.ttf",
        "japanese.otf",
        "resources/japanese.ttf",
        "resources/japanese.otf",
        "../examples/resources/japanese.ttf",
        "../examples/resources/japanese.otf",
        "../../examples/resources/japanese.ttf",
        "../../examples/resources/japanese.otf",
        "../../../examples/resources/japanese.ttf",
        "../../../examples/resources/japanese.otf",
        NULL
    };

    if ((argc >= 2) && (argv != NULL) && (argv[1] != NULL) && FileExists(argv[1])) {
        return argv[1];
    }
    if ((environmentPath != NULL) && FileExists(environmentPath)) return environmentPath;

    for (i = 0; workingDirectoryCandidates[i] != NULL; i++) {
        if (FileExists(workingDirectoryCandidates[i])) return workingDirectoryCandidates[i];
    }
    for (i = 0; applicationRelativeCandidates[i] != NULL; i++) {
        found = ExampleTryApplicationRelative(applicationRelativeCandidates[i]);
        if (found != NULL) return found;
    }
    for (i = 0; systemFontCandidates[i] != NULL; i++) {
        if (FileExists(systemFontCandidates[i])) return systemFontCandidates[i];
    }
    return NULL;
}

static void ExampleShowStartupError(const char *detail)
{
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){ 245, 247, 250, 255 });
        DrawText("RTextJP example could not start", 48, 54, 30, MAROON);
        DrawText("A Japanese TTF/OTF font could not be loaded.", 48, 112, 20, DARKGRAY);
        DrawText("Put a font here:", 48, 162, 20, DARKGRAY);
        DrawText("examples/resources/japanese.ttf", 70, 198, 22, DARKBLUE);
        DrawText("or set RTEXTJP_FONT_PATH / pass the font path as argument 1.",
                 48, 244, 18, GRAY);
        if (detail != NULL) DrawText(detail, 48, 292, 16, MAROON);
        DrawText("Close this window after checking the message.", 48, 354, 18, GRAY);
        EndDrawing();
    }
}

#endif /* RTEXTJP_EXAMPLE_COMMON_H */
