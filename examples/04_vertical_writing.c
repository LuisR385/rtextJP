/*
 * 04: ゲームUI向けの簡易縦書き
 */
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
#include "example_common.h"

int main(int argc, char **argv)
{
    const char *fontPath = NULL;
    const char *rightText = "春はあけぼの。\nやうやう白くなりゆく山ぎは。";
    const char *leftText = "簡易縦書き\nゲームの手紙や演出向け";
    const char *glyphs = "春はあけぼの。やうやう白くなりゆく山ぎは。簡易縦書きゲームの手紙や演出向け右から左";
    Rectangle rightArea = { 80.0f, 90.0f, 350.0f, 430.0f };
    Rectangle leftArea = { 520.0f, 90.0f, 350.0f, 430.0f };
    Font font;
    RTextJPStyle style;

    InitWindow(950, 600, "RTextJP - 04 vertical writing");
    SetTargetFPS(60);
    fontPath = ExampleFindJapaneseFont(argc, argv);

    if (fontPath == NULL) {
        ExampleShowStartupError("No supported font path was found.");
        CloseWindow();
        return 1;
    }
    font = RTextJPLoadFontFromText(fontPath, 46, glyphs);
    if (!IsFontValid(font)) {
        ExampleShowStartupError(fontPath);
        CloseWindow();
        return 1;
    }

    style = RTextJPStyleDefault(font, 31.0f, (Color){ 54, 45, 38, 255 });
    style.spacing = 12.0f;      /* 列と列の間隔 */
    style.lineSpacing = 8.0f;   /* 縦方向の文字間隔 */

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){ 242, 235, 219, 255 });
        DrawText("RIGHT TO LEFT", 80, 45, 18, BROWN);
        DrawText("LEFT TO RIGHT", 520, 45, 18, BROWN);
        DrawRectangleLinesEx(rightArea, 1.0f, (Color){ 180, 160, 130, 255 });
        DrawRectangleLinesEx(leftArea, 1.0f, (Color){ 180, 160, 130, 255 });
        RTextJPDrawVertical(rightText, rightArea, style, RTEXTJP_VERTICAL_RIGHT_TO_LEFT);
        RTextJPDrawVertical(leftText, leftArea, style, RTEXTJP_VERTICAL_LEFT_TO_RIGHT);
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
