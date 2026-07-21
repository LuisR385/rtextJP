/*
 * 07: 日本語文字集合プリセットとカスタム文字の組み合わせ
 */
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
#include "example_common.h"

typedef struct CharacterSetChoice {
    const char *key;
    const char *name;
    const char *description;
    RTextJPCharacterSet characterSet;
} CharacterSetChoice;

static const CharacterSetChoice choices[] = {
    { "1", "カスタムのみ", "ゲーム内で実際に使う文字だけ。最も軽量です。",
      RTEXTJP_CHARACTER_SET_CUSTOM },
    { "2", "かな", "JIS X 0208の平仮名・片仮名。名前入力などに。",
      RTEXTJP_CHARACTER_SET_JIS_X_0208_KANA },
    { "3", "第1水準", "よく使われる漢字2,965字。かなは別に追加します。",
      RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1 },
    { "4", "第2水準", "第2水準漢字3,390字。必要な場合だけ選びます。",
      RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_2 },
    { "5", "JIS X 0208 全体", "非漢字・第1水準・第2水準。作成時間とVRAMに注意。",
      RTEXTJP_CHARACTER_SET_JIS_X_0208 }
};

static Font LoadChoice(const char *fontPath, int choice, const char *customCharacters)
{
    return RTextJPLoadFontFromCharacterSet(fontPath, 32, choices[choice].characterSet,
                                         customCharacters);
}

int main(int argc, char **argv)
{
    const char *fontPath = NULL;
    const char *uiCharacters =
        "日本語文字集合プリセット数字キーで読み込みカスタムのみゲーム内で実際に使う文字だけ最も軽量です"
        "かな平仮名片仮名名前入力などに第1水準よく使われる漢字字かなは別に追加します"
        "第2水準必要な場合だけ選びます全体非漢字作成時間とに注意選択中実際のグリフ数"
        "独自追加勇者髙島組み合わせ可能";
    const char *sample = "独自追加: 勇者・髙島";
    Font font = { 0 };
    RTextJPStyle titleStyle;
    RTextJPStyle bodyStyle;
    int selected = 0;
    int i = 0;
    int counts[5] = { 0 };

    InitWindow(1080, 720, "RTextJP - 07 character sets");
    SetTargetFPS(60);
    fontPath = ExampleFindJapaneseFont(argc, argv);

    if (fontPath == NULL) {
        ExampleShowStartupError("No supported font path was found.");
        CloseWindow();
        return 1;
    }
    font = LoadChoice(fontPath, selected, uiCharacters);
    if (!IsFontValid(font)) {
        ExampleShowStartupError(fontPath);
        CloseWindow();
        return 1;
    }

    titleStyle = RTextJPStyleDefault(font, 30.0f, (Color){ 28, 47, 74, 255 });
    bodyStyle = RTextJPStyleDefault(font, 20.0f, (Color){ 45, 52, 64, 255 });
    titleStyle.clip = false;
    bodyStyle.clip = false;
    for (i = 0; i < 5; i++) {
        counts[i] = RTextJPCharacterSetCodepointCount(choices[i].characterSet, NULL);
    }

    while (!WindowShouldClose()) {
        int requested = -1;
        for (i = 0; i < 5; i++) {
            if (IsKeyPressed(KEY_ONE + i)) requested = i;
        }
        if ((requested >= 0) && (requested != selected)) {
            Font replacement = LoadChoice(fontPath, requested, uiCharacters);
            if (IsFontValid(replacement)) {
                UnloadFont(font);
                font = replacement;
                selected = requested;
                titleStyle.font = font;
                bodyStyle.font = font;
            }
        }

        BeginDrawing();
        ClearBackground((Color){ 242, 246, 251, 255 });
        RTextJPDraw("日本語文字集合プリセット", (Vector2){ 42.0f, 28.0f }, titleStyle);
        DrawText("Press 1-5 to rebuild the font atlas", 44, 70, 18, GRAY);

        for (i = 0; i < 5; i++) {
            Rectangle card = { 42.0f, 110.0f + 91.0f*(float)i, 996.0f, 76.0f };
            Color fill = (i == selected) ? (Color){ 222, 237, 255, 255 }
                                         : (Color){ 255, 255, 255, 255 };
            Color border = (i == selected) ? (Color){ 44, 114, 190, 255 }
                                           : (Color){ 205, 214, 225, 255 };
            DrawRectangleRounded(card, 0.08f, 8, fill);
            DrawRectangleRoundedLinesEx(card, 0.08f, 8, 2.0f, border);
            DrawText(choices[i].key, 60, (int)card.y + 24, 22, DARKBLUE);
            RTextJPDraw(choices[i].name, (Vector2){ 100.0f, card.y + 13.0f }, bodyStyle);
            RTextJPDraw(choices[i].description, (Vector2){ 270.0f, card.y + 13.0f }, bodyStyle);
            DrawText(TextFormat("%d glyphs", counts[i]), 835, (int)card.y + 28, 18, GRAY);
        }

        DrawRectangleRounded((Rectangle){ 42.0f, 584.0f, 996.0f, 100.0f }, 0.05f, 8,
                             (Color){ 30, 42, 60, 255 });
        bodyStyle.color = RAYWHITE;
        RTextJPDraw(sample, (Vector2){ 66.0f, 604.0f }, bodyStyle);
        DrawText(TextFormat("selected atlas: %d glyphs (custom text is always added)",
                            font.glyphCount), 66, 650, 17, (Color){ 180, 198, 220, 255 });
        bodyStyle.color = (Color){ 45, 52, 64, 255 };
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
