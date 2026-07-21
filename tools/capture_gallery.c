/* Generate documentation screenshots from actual RTextJP/raylib rendering. */
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"

#include <stdio.h>

static void DrawPanel(Rectangle bounds)
{
    DrawRectangleRounded(bounds, 0.04f, 8, (Color){ 255, 255, 255, 255 });
    DrawRectangleRoundedLinesEx(bounds, 0.04f, 8, 2.0f, (Color){ 207, 216, 228, 255 });
}

static void SaveCurrentFrame(const char *fileName)
{
    Image image = LoadImageFromScreen();
    if (IsImageValid(image)) {
        ExportImage(image, fileName);
        UnloadImage(image);
    }
}

static void DrawOverview(Font font, const char *fileName)
{
    RTextJPStyle title = RTextJPStyleDefault(font, 28.0f, (Color){ 26, 45, 72, 255 });
    RTextJPStyle body = RTextJPStyleDefault(font, 22.0f, (Color){ 46, 53, 64, 255 });
    RTextJPStyle small = RTextJPStyleDefault(font, 18.0f, (Color){ 60, 68, 82, 255 });
    RTextJPInput input;
    RTextJPInputStyle inputStyle = RTextJPInputStyleDefault();
    char inputBuffer[64] = "山田太郎";
    Rectangle panel1 = { 36.0f, 104.0f, 548.0f, 220.0f };
    Rectangle panel2 = { 616.0f, 104.0f, 548.0f, 220.0f };
    Rectangle panel3 = { 36.0f, 352.0f, 548.0f, 286.0f };
    Rectangle panel4 = { 616.0f, 352.0f, 548.0f, 286.0f };
    int i = 0;

    title.clip = false;
    body.lineSpacing = 8.0f;
    small.clip = false;
    RTextJPInputInit(&input, inputBuffer, (int)sizeof(inputBuffer));

    BeginDrawing();
    ClearBackground((Color){ 240, 245, 251, 255 });
    DrawText("RTextJP", 38, 24, 34, (Color){ 28, 91, 157, 255 });
    DrawText("Japanese-friendly UTF-8 text helpers for raylib", 150, 34, 21,
             (Color){ 82, 95, 112, 255 });
    DrawText("ACTUAL OUTPUT", 995, 34, 16, (Color){ 38, 133, 91, 255 });

    DrawPanel(panel1);
    DrawText("WRAP + KINSOKU", 56, 124, 16, (Color){ 38, 112, 184, 255 });
    RTextJPDrawBox("日本語は空白がなくても幅に合わせて折り返します。句読点（、。）も読みやすい位置に保ちます。\nEnglish wraps at word boundaries.",
                 (Rectangle){ 56.0f, 157.0f, 508.0f, 145.0f }, body);

    DrawPanel(panel2);
    DrawText("ALIGNMENT", 636, 124, 16, (Color){ 38, 112, 184, 255 });
    for (i = 0; i < 3; i++) {
        Rectangle box = { 636.0f + 169.0f*(float)i, 158.0f, 154.0f, 138.0f };
        const char *labels[] = { "左揃え", "中央揃え", "右揃え" };
        DrawRectangleRounded(box, 0.08f, 6, (Color){ 245, 248, 252, 255 });
        small.alignment = (RTextJPAlign)i;
        small.verticalAlignment = (RTextJPVerticalAlign)i;
        RTextJPDrawBox(labels[i], (Rectangle){ box.x + 9.0f, box.y + 9.0f,
                                             box.width - 18.0f, box.height - 18.0f }, small);
    }

    DrawPanel(panel3);
    DrawText("VERTICAL", 56, 372, 16, (Color){ 38, 112, 184, 255 });
    RTextJPDrawVertical("春風や\n堤長うして\n家遠し", (Rectangle){ 76.0f, 411.0f, 190.0f, 190.0f },
                      title, RTEXTJP_VERTICAL_RIGHT_TO_LEFT);
    RTextJPDraw("右から左へ進む簡易縦書き", (Vector2){ 286.0f, 430.0f }, small);
    RTextJPDraw("ゲームの手紙や演出向け", (Vector2){ 286.0f, 468.0f }, small);
    DrawText("grid layout / no OpenType vert", 286, 516, 16, GRAY);

    DrawPanel(panel4);
    DrawText("UTF-8 INPUT", 636, 372, 16, (Color){ 38, 112, 184, 255 });
    RTextJPDraw("プレイヤー名", (Vector2){ 646.0f, 415.0f }, small);
    input.active = true;
    RTextJPDrawInputBox(&input, (Rectangle){ 646.0f, 451.0f, 488.0f, 64.0f }, body,
                      inputStyle, "名前を入力");
    DrawText("4 codepoints / 12 UTF-8 bytes", 648, 532, 17, GRAY);
    RTextJPDraw("固定長バッファを文字境界で安全に編集", (Vector2){ 646.0f, 567.0f }, small);
    if (fileName != NULL) SaveCurrentFrame(fileName);
    EndDrawing();
}

static void DrawCharacterSets(Font font, int requestedGlyphCount, int providedGlyphCount,
                              const char *fileName)
{
    const RTextJPCharacterSet sets[] = {
        RTEXTJP_CHARACTER_SET_CUSTOM,
        RTEXTJP_CHARACTER_SET_JIS_X_0208_KANA,
        RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1,
        RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_2,
        RTEXTJP_CHARACTER_SET_JIS_X_0208
    };
    const char *names[] = { "カスタム", "かな", "第1水準", "第2水準", "JIS X 0208 全体" };
    const char *notes[] = {
        "実際に使う文字だけ",
        "平仮名・片仮名",
        "漢字 2,965字",
        "漢字 3,390字",
        "非漢字 + 第1 + 第2"
    };
    RTextJPStyle title = RTextJPStyleDefault(font, 30.0f, (Color){ 27, 46, 75, 255 });
    RTextJPStyle body = RTextJPStyleDefault(font, 21.0f, (Color){ 48, 55, 68, 255 });
    int i = 0;

    title.clip = false;
    body.clip = false;
    BeginDrawing();
    ClearBackground((Color){ 243, 247, 252, 255 });
    RTextJPDraw("必要な文字だけ、列挙型で選ぶ", (Vector2){ 42.0f, 30.0f }, title);
    DrawText("RTextJPCharacterSet presets + optional custom UTF-8", 44, 76, 18, GRAY);

    for (i = 0; i < 5; i++) {
        Rectangle card = { 42.0f, 116.0f + 85.0f*(float)i, 1116.0f, 68.0f };
        int count = RTextJPCharacterSetCodepointCount(sets[i], NULL);
        DrawPanel(card);
        DrawText(TextFormat("%d", i + 1), 62, (int)card.y + 22, 20,
                 (Color){ 38, 112, 184, 255 });
        RTextJPDraw(names[i], (Vector2){ 112.0f, card.y + 17.0f }, body);
        RTextJPDraw(notes[i], (Vector2){ 365.0f, card.y + 17.0f }, body);
        DrawText(TextFormat("%d glyphs", count), 930, (int)card.y + 24, 18,
                 (Color){ 82, 95, 112, 255 });
    }

    DrawRectangleRounded((Rectangle){ 42.0f, 561.0f, 1116.0f, 78.0f }, 0.05f, 8,
                         (Color){ 29, 43, 62, 255 });
    body.color = RAYWHITE;
    RTextJPDraw("どのプリセットにも、固有名詞・記号を追加できます: 髙島",
              (Vector2){ 66.0f, 578.0f }, body);
    DrawText(TextFormat("runtime: requested %d / font provided %d",
                        requestedGlyphCount, providedGlyphCount),
             790, 611, 15, (Color){ 170, 192, 218, 255 });
    if (fileName != NULL) SaveCurrentFrame(fileName);
    EndDrawing();
}

static void DrawKinsoku(Font font, bool enabled, const char *fileName)
{
    RTextJPStyle title = RTextJPStyleDefault(font, 31.0f, (Color){ 27, 46, 75, 255 });
    RTextJPStyle text = RTextJPStyleDefault(font, 44.0f, (Color){ 40, 47, 59, 255 });
    RTextJPStyle caption = RTextJPStyleDefault(font, 23.0f, (Color){ 58, 68, 84, 255 });
    const char *sample = "あい。うえ、かき）くけ「こさ";
    float width = RTextJPMeasure("あい", 0.0f, text).size.x;
    Rectangle box = { 476.0f, 153.0f, width + 4.0f, 390.0f };
    int row = 0;

    title.clip = false;
    text.kinsoku = enabled;
    text.lineSpacing = 8.0f;
    caption.lineSpacing = 7.0f;
    BeginDrawing();
    ClearBackground((Color){ 243, 247, 252, 255 });
    RTextJPDraw("禁則処理を切り替える", (Vector2){ 42.0f, 30.0f }, title);
    DrawText("The same text and the same width", 44, 76, 18, GRAY);
    DrawRectangleRounded((Rectangle){ 42.0f, 112.0f, 1116.0f, 492.0f }, 0.03f, 8,
                         (Color){ 255, 255, 255, 255 });
    for (row = 0; row < 7; row++) {
        DrawRectangle((int)box.x, (int)(box.y + (text.fontSize + text.lineSpacing)*(float)row),
                      (int)box.width, (int)text.fontSize,
                      (row % 2 == 0) ? (Color){ 235, 242, 250, 255 }
                                     : (Color){ 246, 249, 252, 255 });
    }
    DrawRectangleLinesEx(box, 2.0f, (Color){ 93, 122, 153, 255 });
    RTextJPDrawBox(sample, box, text);

    DrawText(enabled ? "KINSOKU  ON" : "KINSOKU  OFF", 735, 204, 30,
             enabled ? (Color){ 31, 137, 91, 255 } : (Color){ 188, 75, 65, 255 });
    RTextJPDrawBox(enabled ? "句読点を行頭に置かないため、直前の文字も次の行へ送ります。"
                         : "幅だけで折り返すため、句読点が行頭に残る場合があります。",
                 (Rectangle){ 735.0f, 265.0f, 370.0f, 105.0f }, caption);
    DrawText(enabled ? "Readable Japanese line breaks" : "Mechanical line breaks",
             738, 390, 18, GRAY);
    DrawText("style.kinsoku =", 738, 451, 18, (Color){ 75, 87, 103, 255 });
    DrawText(enabled ? "true" : "false", 918, 451, 18,
             enabled ? DARKGREEN : MAROON);
    if (fileName != NULL) SaveCurrentFrame(fileName);
    EndDrawing();
}

int main(int argc, char **argv)
{
    const char *glyphs =
        "日本語空白幅合わせ折り返します句読点読みやすい位置保ち左揃え中央右春風堤長うして家遠し"
        "からへ進む簡易縦書きゲーム手紙演出向けプレイヤー名山田太郎名前入力固定長バッファ文字境界安全編集"
        "必要な文字だけ列挙型で選ぶカスタムかな第1水準第2全体実際使う平仮名片仮名漢字非"
        "どのプリセットにも固有名詞記号絵文字追加できます髙島禁則処理切り替える"
        "同じ文章矩形句読点を行頭に置かないため直前の文字も次の行へ送ります幅だけで折り返す残る場合があります"
        "あいうえかきくけこさ（）「」、。";
    char overviewPath[1024];
    char setsPath[1024];
    char onPath[1024];
    char offPath[1024];
    Font font = { 0 };
    Font fullFont = { 0 };
    int expectedFullCount = 0;
    int fullGlyphCount = 0;
    int level1Index = 0;
    int level2Index = 0;
    int requestedCount = 0;
    int missingCount = 0;
    int *requestedCodepoints = NULL;
    int i = 0;

    if (argc < 3) {
        fprintf(stderr, "usage: rtextjp_capture_gallery FONT OUTPUT_DIRECTORY\n");
        return 2;
    }
    snprintf(overviewPath, sizeof(overviewPath), "%s/rtextjp-overview.png", argv[2]);
    snprintf(setsPath, sizeof(setsPath), "%s/character-sets.png", argv[2]);
    snprintf(onPath, sizeof(onPath), "%s/kinsoku-on.png", argv[2]);
    snprintf(offPath, sizeof(offPath), "%s/kinsoku-off.png", argv[2]);

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_HIDDEN | FLAG_MSAA_4X_HINT);
    InitWindow(1200, 675, "RTextJP documentation capture");
    if (!IsWindowReady()) return 3;

    font = RTextJPLoadFontFromText(argv[1], 48, glyphs);
    expectedFullCount = RTextJPCharacterSetCodepointCount(RTEXTJP_CHARACTER_SET_JIS_X_0208, "髙");
    fullFont = RTextJPLoadFontFromCharacterSet(argv[1], 20, RTEXTJP_CHARACTER_SET_JIS_X_0208, "髙");
    if (!IsFontValid(font) || !IsFontValid(fullFont)) {
        fprintf(stderr, "font loading failed\n");
        CloseWindow();
        return 4;
    }
    fullGlyphCount = fullFont.glyphCount;
    level1Index = GetGlyphIndex(fullFont, 0x4E9C);
    level2Index = GetGlyphIndex(fullFont, 0x5F0C);
    requestedCodepoints = rtextjp_build_character_set(RTEXTJP_CHARACTER_SET_JIS_X_0208,
                                                      "髙", &requestedCount);
    if (requestedCodepoints == NULL) {
        UnloadFont(fullFont);
        UnloadFont(font);
        CloseWindow();
        return 5;
    }
    for (i = 0; i < requestedCount; i++) {
        int glyphIndex = GetGlyphIndex(fullFont, requestedCodepoints[i]);
        if (fullFont.glyphs[glyphIndex].value != requestedCodepoints[i]) {
            fprintf(stderr, "font missing U+%04X\n", requestedCodepoints[i]);
            missingCount++;
        }
    }
    RTEXTJP_FREE(requestedCodepoints);
    if ((requestedCount != expectedFullCount) ||
        (fullGlyphCount != (requestedCount - missingCount)) ||
        (fullFont.glyphs[level1Index].value != 0x4E9C) ||
        (fullFont.glyphs[level2Index].value != 0x5F0C)) {
        fprintf(stderr, "JIS runtime check failed: requested=%d provided=%d missing=%d\n",
                requestedCount, fullGlyphCount, missingCount);
        UnloadFont(fullFont);
        UnloadFont(font);
        CloseWindow();
        return 5;
    }
    UnloadFont(fullFont);

    DrawOverview(font, NULL);
    DrawOverview(font, overviewPath);
    DrawCharacterSets(font, expectedFullCount, fullGlyphCount, NULL);
    DrawCharacterSets(font, expectedFullCount, fullGlyphCount, setsPath);
    DrawKinsoku(font, true, NULL);
    DrawKinsoku(font, true, onPath);
    DrawKinsoku(font, false, NULL);
    DrawKinsoku(font, false, offPath);

    UnloadFont(font);
    CloseWindow();
    if (!FileExists(overviewPath) || !FileExists(setsPath) ||
        !FileExists(onPath) || !FileExists(offPath)) return 6;
    printf("runtime JIS check passed: requested=%d provided=%d missing=%d\n",
           expectedFullCount, fullGlyphCount, missingCount);
    return 0;
}
