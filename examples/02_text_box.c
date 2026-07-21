/*
 * 02: 日本語・英語の折り返しと簡易禁則処理
 */
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
#include "example_common.h"

int main(int argc, char **argv)
{
    const char *fontPath = NULL;
    const char *paragraph =
        "「日本語の文章」は、単語の間に空白がなくても自然に折り返す必要があります。"
        "句読点（、。）や閉じ括弧が行頭へ来ないように、RTextJPは簡易的な禁則処理を行います。\n\n"
        "English words prefer wrapping at spaces. AWordThatIsLongerThanTheBox is split only when needed.";
    const char *glyphs =
        "禁則処理 ON OFF Kキーで切り替え"
        "日本語の文章は単語の間に空白がなくても自然に折り返す必要があります。"
        "句読点や閉じ括弧が行頭へ来ないように、RTextJPは簡易的な禁則処理を行います。"
        "English words prefer wrapping at spaces. AWordThatIsLongerThanTheBox is split only when needed.";
    Rectangle box = { 70.0f, 100.0f, 760.0f, 430.0f };
    Font font;
    RTextJPStyle style;
    bool kinsoku = true;

    InitWindow(900, 600, "RTextJP - 02 text box");
    SetTargetFPS(60);
    fontPath = ExampleFindJapaneseFont(argc, argv);

    if (fontPath == NULL) {
        ExampleShowStartupError("No supported font path was found.");
        CloseWindow();
        return 1;
    }
    font = RTextJPLoadFontFromText(fontPath, 44, glyphs);
    if (!IsFontValid(font)) {
        ExampleShowStartupError(fontPath);
        CloseWindow();
        return 1;
    }

    style = RTextJPStyleDefault(font, 27.0f, (Color){ 45, 48, 55, 255 });
    style.lineSpacing = 10.0f;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_K)) kinsoku = !kinsoku;
        style.kinsoku = kinsoku;

        BeginDrawing();
        ClearBackground((Color){ 243, 240, 232, 255 });
        DrawText(kinsoku ? "KINSOKU: ON  (press K)" : "KINSOKU: OFF (press K)",
                 70, 40, 20, kinsoku ? DARKGREEN : MAROON);
        DrawRectangleRounded(box, 0.03f, 8, (Color){ 255, 255, 255, 255 });
        DrawRectangleRoundedLinesEx(box, 0.03f, 8, 2.0f, (Color){ 190, 180, 165, 255 });
        RTextJPDrawBox(paragraph, (Rectangle){ box.x + 24.0f, box.y + 22.0f,
                                            box.width - 48.0f, box.height - 44.0f }, style);
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
