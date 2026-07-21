/*
 * 03: 横揃え・縦揃えとRTextJPMeasure()
 */
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
#include "example_common.h"

static void DrawPanel(Rectangle panel, const char *text, RTextJPStyle style, const char *caption)
{
    RTextJPMetrics metrics = RTextJPMeasure(text, panel.width - 24.0f, style);
    Rectangle textArea = { panel.x + 12.0f, panel.y + 12.0f,
                           panel.width - 24.0f, panel.height - 24.0f };

    DrawRectangleRounded(panel, 0.05f, 8, (Color){ 250, 250, 252, 255 });
    DrawRectangleRoundedLinesEx(panel, 0.05f, 8, 2.0f, (Color){ 195, 200, 210, 255 });
    RTextJPDrawBox(text, textArea, style);
    DrawText(TextFormat("%s / %.0f x %.0f / %d lines", caption,
                        metrics.size.x, metrics.size.y, metrics.lineCount),
             (int)panel.x, (int)(panel.y + panel.height + 8.0f), 16, GRAY);
}

int main(int argc, char **argv)
{
    const char *fontPath = NULL;
    const char *text = "揃え方を変えても、同じ文章と同じ矩形を使えます。\nAlignment works for English, too.";
    const char *glyphs = "揃え方を変えても同じ文章と矩形を使えます。左中央右上下Alignment works for English, too.";
    Font font;
    RTextJPStyle style;

    InitWindow(1050, 560, "RTextJP - 03 alignment");
    SetTargetFPS(60);
    fontPath = ExampleFindJapaneseFont(argc, argv);

    if (fontPath == NULL) {
        ExampleShowStartupError("No supported font path was found.");
        CloseWindow();
        return 1;
    }
    font = RTextJPLoadFontFromText(fontPath, 42, glyphs);
    if (!IsFontValid(font)) {
        ExampleShowStartupError(fontPath);
        CloseWindow();
        return 1;
    }

    style = RTextJPStyleDefault(font, 24.0f, (Color){ 42, 50, 65, 255 });
    style.lineSpacing = 8.0f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){ 238, 242, 247, 255 });
        DrawText("Horizontal + vertical alignment", 40, 28, 26, DARKBLUE);

        style.alignment = RTEXTJP_ALIGN_LEFT;
        style.verticalAlignment = RTEXTJP_VALIGN_TOP;
        DrawPanel((Rectangle){ 40.0f, 90.0f, 300.0f, 350.0f }, text, style, "LEFT / TOP");

        style.alignment = RTEXTJP_ALIGN_CENTER;
        style.verticalAlignment = RTEXTJP_VALIGN_MIDDLE;
        DrawPanel((Rectangle){ 375.0f, 90.0f, 300.0f, 350.0f }, text, style, "CENTER / MIDDLE");

        style.alignment = RTEXTJP_ALIGN_RIGHT;
        style.verticalAlignment = RTEXTJP_VALIGN_BOTTOM;
        DrawPanel((Rectangle){ 710.0f, 90.0f, 300.0f, 350.0f }, text, style, "RIGHT / BOTTOM");

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
