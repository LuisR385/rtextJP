/*
 * 05: 固定長UTF-8バッファの1行入力欄
 *
 * フォントはglyphsに含めた文字だけを持ちます。自由な漢字入力には、アプリの用途に合った
 * より広い文字集合または動的フォント管理が必要です。
 */
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
#include "example_common.h"

int main(int argc, char **argv)
{
    const char *fontPath = NULL;
    const char *glyphs =
        "名前を入力してくださいクリックで選択中の文字数バイト"
        "山田太郎佐藤鈴木高橋伊藤渡辺"
        "あいうえおかきくけこさしすせそたちつてとなにぬねの"
        "はひふへほまみむめもやゆよらりるれろわをん"
        "アイウエオカキクケコサシスセソタチツテトナニヌネノ"
        "ハヒフヘホマミムメモヤユヨラリルレロワヲン";
    char buffer[256] = "山田太郎";
    RTextJPInput input;
    Rectangle field = { 120.0f, 210.0f, 660.0f, 64.0f };
    Font font;
    RTextJPStyle textStyle;
    RTextJPInputStyle inputStyle;

    InitWindow(900, 500, "RTextJP - 05 text input");
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

    RTextJPInputInit(&input, buffer, (int)sizeof(buffer));
    input.active = true;
    textStyle = RTextJPStyleDefault(font, 28.0f, (Color){ 30, 35, 45, 255 });
    inputStyle = RTextJPInputStyleDefault();

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            input.active = CheckCollisionPointRec(GetMousePosition(), field);
        }
        (void)RTextJPInputUpdate(&input);

        BeginDrawing();
        ClearBackground((Color){ 240, 244, 249, 255 });
        RTextJPDraw("名前を入力してください", (Vector2){ 120.0f, 145.0f }, textStyle);
        RTextJPDrawInputBox(&input, field, textStyle, inputStyle, "名前を入力");
        DrawText("Click the field. Arrow/Home/End, Backspace/Delete and Ctrl+V are supported.",
                 120, 300, 17, GRAY);
        DrawText(TextFormat("%d codepoints / %d bytes",
                            RTextJPUtf8CodepointCount(input.text), input.length),
                 120, 335, 18, DARKGRAY);
        DrawText("IME preedit/candidate UI depends on the raylib platform backend.",
                 120, 375, 17, MAROON);
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
