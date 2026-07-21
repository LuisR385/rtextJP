/*
 * 01: 最小の日本語表示
 *
 * ビルドした実行ファイルの第1引数へ日本語TTF/OTFのパスを渡すこともできます。
 */
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
#include "example_common.h"

int main(int argc, char **argv)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const char *message = "こんにちは、raylib!";
    const char *fontPath = NULL;
    Font font;
    RTextJPStyle style;

    InitWindow(screenWidth, screenHeight, "RTextJP - 01 hello japanese");
    SetTargetFPS(60);
    fontPath = ExampleFindJapaneseFont(argc, argv);

    if (fontPath == NULL) {
        TraceLog(LOG_ERROR, "Japanese font not found");
        ExampleShowStartupError("No supported font path was found.");
        CloseWindow();
        return 1;
    }

    /* messageにある日本語と、表示可能ASCIIだけをフォントへ読み込みます。 */
    font = RTextJPLoadFontFromText(fontPath, 48, message);
    if (!IsFontValid(font)) {
        TraceLog(LOG_ERROR, "Could not load font: %s", fontPath);
        ExampleShowStartupError(fontPath);
        CloseWindow();
        return 1;
    }

    style = RTextJPStyleDefault(font, 40.0f, (Color){ 35, 45, 60, 255 });

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){ 245, 247, 250, 255 });

        RTextJPDraw(message, (Vector2){ 70.0f, 170.0f }, style);
        DrawText("The Japanese line above is drawn by RTextJP.", 72, 235, 18, GRAY);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
