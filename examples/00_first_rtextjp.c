#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
int main(void)
{
    InitWindow(800, 450, "RTextJP example");

    const char *text =
        "こんにちは、raylib！\n"
        "日本語を自然に折り返して表示します。\n"
        "こんなかんじで！"              ;

    Font font = RTextJPLoadFontFromText(
        "resources/japanese.ttf",
        48,
        text
    );

    RTextJPStyle style = RTextJPStyleDefault(font, 32.0f, BLACK);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        RTextJPDrawBox(
            text,
            (Rectangle){ 40, 40, 720, 370 },
            style
        );

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
