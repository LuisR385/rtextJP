/*
 * 06: UTF-8の検証とコードポイント単位の読み取り（GUI例）
 *
 * 1〜5と同じく、ダブルクリックでウィンドウを開いたまま確認できます。
 */
#include <stdio.h>
#include <string.h>

#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
#include "example_common.h"

int main(int argc, char **argv)
{
    const char *text = "日本語 + English + 😀";
    const char *glyphs =
        "UTF解析ビューア日本語として有効ですバイト数文字数"
        "日本語 + English + 😀";
    const char *fontPath = NULL;
    Font font;
    RTextJPStyle titleStyle;
    RTextJPStyle textStyle;

    InitWindow(900, 680, "RTextJP - 06 UTF-8 tools");
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

    titleStyle = RTextJPStyleDefault(font, 30.0f, (Color){ 35, 60, 95, 255 });
    textStyle = RTextJPStyleDefault(font, 25.0f, (Color){ 35, 40, 50, 255 });

    while (!WindowShouldClose()) {
        const char *cursor = text;
        int index = 0;
        float rowY = 290.0f;

        BeginDrawing();
        ClearBackground((Color){ 241, 245, 250, 255 });

        RTextJPDraw("UTF-8解析ビューア", (Vector2){ 48.0f, 35.0f }, titleStyle);
        DrawText("This window uses the UTF-8 helper API in RTextJP.", 50, 82, 18, GRAY);

        DrawRectangleRounded((Rectangle){ 48.0f, 120.0f, 804.0f, 112.0f },
                             0.08f, 8, WHITE);
        DrawRectangleRoundedLinesEx((Rectangle){ 48.0f, 120.0f, 804.0f, 112.0f },
                                    0.08f, 8, 2.0f, (Color){ 185, 198, 215, 255 });
        RTextJPDraw(text, (Vector2){ 70.0f, 142.0f }, textStyle);
        RTextJPDraw(RTextJPUtf8IsValid(text) ? "UTF-8として有効です" : "UTF-8として不正です",
                  (Vector2){ 70.0f, 190.0f },
                  RTextJPStyleDefault(font, 18.0f, DARKGREEN));
        DrawText(TextFormat("strlen: %u bytes    RTextJP: %d codepoints",
                            (unsigned int)strlen(text), RTextJPUtf8CodepointCount(text)),
                 430, 194, 18, DARKGRAY);

        DrawText("INDEX", 55, 255, 17, GRAY);
        DrawText("CODEPOINT", 145, 255, 17, GRAY);
        DrawText("BYTES", 300, 255, 17, GRAY);
        DrawText("GLYPH", 410, 255, 17, GRAY);

        while ((*cursor != '\0') && (rowY < 635.0f)) {
            int bytes = 0;
            int codepoint = RTextJPUtf8Decode(cursor, &bytes);
            Color rowColor = ((index % 2) == 0) ? WHITE : (Color){ 232, 238, 246, 255 };

            DrawRectangle(48, (int)(rowY - 4.0f), 804, 25, rowColor);
            DrawText(TextFormat("%02d", index), 62, (int)rowY, 16, DARKGRAY);
            DrawText(TextFormat("U+%04X", codepoint), 145, (int)rowY, 16, DARKBLUE);
            DrawText(TextFormat("%d", bytes), 315, (int)rowY, 16, DARKGRAY);
            DrawTextCodepoint(font, codepoint, (Vector2){ 430.0f, rowY - 5.0f },
                              22.0f, (Color){ 35, 40, 50, 255 });

            cursor += bytes;
            index++;
            rowY += 21.0f;
        }

        DrawText("Close with ESC or the window X button.", 590, 650, 16, GRAY);
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
