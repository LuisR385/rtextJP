#if defined(NDEBUG)
    #undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>

#define RTEXTJP_STATIC
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"

static void TestPresetCounts(void)
{
    assert(RTextJPCharacterSetCodepointCount(RTEXTJP_CHARACTER_SET_CUSTOM, NULL) == 96);
    assert(RTextJPCharacterSetCodepointCount(RTEXTJP_CHARACTER_SET_CUSTOM, "日本日本") == 98);
    assert(RTextJPCharacterSetCodepointCount(RTEXTJP_CHARACTER_SET_JIS_X_0208_KANA, NULL) == 265);
    assert(RTextJPCharacterSetCodepointCount(RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI, NULL) == 619);
    assert(RTextJPCharacterSetCodepointCount(RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1, NULL) == 3061);
    assert(RTextJPCharacterSetCodepointCount(RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_2, NULL) == 3486);
    assert(RTextJPCharacterSetCodepointCount(RTEXTJP_CHARACTER_SET_JIS_X_0208, NULL) == 6974);

    /* Kana is already part of the non-kanji set and must not be counted twice. */
    assert(RTextJPCharacterSetCodepointCount(
        (RTextJPCharacterSet)(RTEXTJP_CHARACTER_SET_JIS_X_0208_KANA |
                            RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI), NULL) == 619);

    /* An app-specific character can always be added on top of a preset. */
    assert(RTextJPCharacterSetCodepointCount(RTEXTJP_CHARACTER_SET_JIS_X_0208, "😀") == 6975);
    assert(RTextJPCharacterSetCodepointCount(
        (RTextJPCharacterSet)(RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI |
                            RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1), "髙島") == 3585);
}

static void TestPresetContents(void)
{
    int count = 0;
    int *codepoints = rtextjp_build_character_set(RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1,
                                                 NULL, &count);
    assert(codepoints != NULL);
    assert(rtextjp_codepoint_exists(codepoints, count, 0x4E9C)); /* 亜: level 1 */
    assert(!rtextjp_codepoint_exists(codepoints, count, 0x5F0C)); /* 弌: level 2 */
    RTEXTJP_FREE(codepoints);

    codepoints = rtextjp_build_character_set(RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_2,
                                            NULL, &count);
    assert(codepoints != NULL);
    assert(rtextjp_codepoint_exists(codepoints, count, 0x5F0C));
    assert(!rtextjp_codepoint_exists(codepoints, count, 0x4E9C));
    RTEXTJP_FREE(codepoints);

    codepoints = rtextjp_build_character_set(RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI,
                                            NULL, &count);
    assert(codepoints != NULL);
    assert(rtextjp_codepoint_exists(codepoints, count, 0x2252)); /* ≒ */
    RTEXTJP_FREE(codepoints);
}

int main(void)
{
    TestPresetCounts();
    TestPresetContents();
    puts("RTextJP character-set tests passed");
    return 0;
}
