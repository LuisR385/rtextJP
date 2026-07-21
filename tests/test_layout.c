#if defined(NDEBUG)
    #undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>

#define RTEXTJP_STATIC
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"

static Font MakeMonospaceFont(GlyphInfo *glyph, Rectangle *rec)
{
    Font font = { 0 };
    glyph->value = '?';
    glyph->advanceX = 10;
    rec->width = 10.0f;
    font.baseSize = 10;
    font.glyphCount = 1;
    font.glyphs = glyph;
    font.recs = rec;
    return font;
}

static void TestJapaneseWrap(void)
{
    GlyphInfo glyph = { 0 };
    Rectangle rec = { 0 };
    Font font = MakeMonospaceFont(&glyph, &rec);
    RTextJPStyle style = RTextJPStyleDefault(font, 10.0f, BLACK);
    RTextJPMetrics metrics;

    style.spacing = 0.0f;
    style.lineSpacing = 2.0f;
    metrics = RTextJPMeasure("日本語", 20.0f, style);
    assert(metrics.lineCount == 2);
    assert(metrics.codepointCount == 3);
    assert(metrics.size.x == 20.0f);
    assert(metrics.size.y == 22.0f);
}

static void TestEnglishWrapTrimsSpace(void)
{
    GlyphInfo glyph = { 0 };
    Rectangle rec = { 0 };
    Font font = MakeMonospaceFont(&glyph, &rec);
    RTextJPStyle style = RTextJPStyleDefault(font, 10.0f, BLACK);
    RTextJPMetrics metrics;

    style.spacing = 0.0f;
    metrics = RTextJPMeasure("one two", 35.0f, style);
    assert(metrics.lineCount == 2);
    assert(metrics.size.x == 30.0f);
    assert(metrics.codepointCount == 7);

    metrics = RTextJPMeasure("one   two", 45.0f, style);
    assert(metrics.lineCount == 2);
    assert(metrics.size.x == 30.0f);
    assert(metrics.codepointCount == 9);
}

static void TestKinsoku(void)
{
    GlyphInfo glyph = { 0 };
    Rectangle rec = { 0 };
    Font font = MakeMonospaceFont(&glyph, &rec);
    RTextJPStyle style = RTextJPStyleDefault(font, 10.0f, BLACK);
    RTextJPMetrics metrics;
    RTextJPLineInternal firstLine;

    style.spacing = 0.0f;
    firstLine = rtextjp_next_line("あい。", 20.0f, style);
    assert(firstLine.codepointCount == 1);
    metrics = RTextJPMeasure("あい。", 20.0f, style);
    assert(metrics.lineCount == 2);
    assert(metrics.size.x == 20.0f);

    style.kinsoku = false;
    firstLine = rtextjp_next_line("あい。", 20.0f, style);
    assert(firstLine.codepointCount == 2);
    metrics = RTextJPMeasure("あい。", 20.0f, style);
    assert(metrics.lineCount == 2);
    assert(metrics.size.x == 20.0f);
}

static void TestExplicitNewlineAndVerticalMeasure(void)
{
    GlyphInfo glyph = { 0 };
    Rectangle rec = { 0 };
    Font font = MakeMonospaceFont(&glyph, &rec);
    RTextJPStyle style = RTextJPStyleDefault(font, 10.0f, BLACK);
    RTextJPMetrics metrics;

    style.spacing = 0.0f;
    style.lineSpacing = 2.0f;
    metrics = RTextJPMeasure("日\n", 100.0f, style);
    assert(metrics.lineCount == 2);
    assert(metrics.codepointCount == 2);
    assert(metrics.size.y == 22.0f);

    metrics = RTextJPMeasureVertical("日本語", 22.0f, style);
    assert(metrics.lineCount == 2);
    assert(metrics.codepointCount == 3);
    assert(metrics.size.x == 20.0f);
    assert(metrics.size.y == 22.0f);
}

int main(void)
{
    TestJapaneseWrap();
    TestEnglishWrapTrimsSpace();
    TestKinsoku();
    TestExplicitNewlineAndVerticalMeasure();
    puts("RTextJP layout tests passed");
    return 0;
}
