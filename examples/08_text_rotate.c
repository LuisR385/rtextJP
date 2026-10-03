#include "raylib.h"

#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "RTextJP DrawEx Test");
    SetTargetFPS(60);

    // JIS X 0208:
    // 非漢字 + 第1水準 + 第2水準
    Font font = RTextJPLoadFontFromCharacterSet(
        "resources/japanese.ttf",
        48,
        RTEXTJP_CHARACTER_SET_JIS_X_0208,
        NULL
    );

    //回転を考慮して品質を向上させる
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR); 

    if (!IsFontValid(font))
    {
        TraceLog(LOG_ERROR, "Failed to load Japanese font.");
        CloseWindow();
        return 1;
    }

    RTextJPStyle style =
        RTextJPStyleDefault(font, 48.0f, WHITE);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground((Color) { 30, 30, 30, 255 });

        // =========================================================
        // rotation = 0
        // =========================================================

        Vector2 position0 = { 300.0f, 150.0f };
        Vector2 origin0 = position0;

        DrawCircleV(origin0, 5.0f, RED);

        RTextJPDraw("RTextJP 日本語 TEST NORMAL Draw API", (Vector2){300.f,70.f}, style);

        RTextJPDrawEx(
            "RTextJP 日本語 TEST",
            position0,
            origin0,
            0.0f,
            48.0f,
            2.0f,
            style
        );

        // =========================================================
        // rotation = 15
        // =========================================================

        Vector2 position15 = { 300.0f, 300.0f };
        Vector2 origin15 = position15;

        DrawCircleV(origin15, 5.0f, RED);

        RTextJPDrawEx(
            "RTextJP 日本語 TEST",
            position15,
            origin15,
            15.0f,
            48.0f,
            2.0f,
            style
        );

        // =========================================================
        // rotation = 45
        // =========================================================

        Vector2 position45 = { 300.0f, 500.0f };
        Vector2 origin45 = position45;

        DrawCircleV(origin45, 5.0f, RED);

        RTextJPDrawEx(
            "RTextJP 日本語 TEST",
            position45,
            origin45,
            45.0f,
            48.0f,
            2.0f,
            style
        );

        DrawText(
            "Red point = rotation origin",
            20,
            20,
            20,
            LIGHTGRAY
        );

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}