#if defined(NDEBUG)
    #undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define RTEXTJP_STATIC
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"

static void TestDecodeAndEncode(void)
{
    int bytes = 0;
    char encoded[5];

    assert(RTextJPUtf8Decode("日", &bytes) == 0x65E5);
    assert(bytes == 3);
    assert(RTextJPUtf8Decode("😀", &bytes) == 0x1F600);
    assert(bytes == 4);

    assert(RTextJPUtf8Encode(0x65E5, encoded) == 3);
    assert(strcmp(encoded, "日") == 0);
    assert(RTextJPUtf8Encode(0x1F600, encoded) == 4);
    assert(strcmp(encoded, "😀") == 0);
    assert(RTextJPUtf8Encode(0xD800, encoded) == 0);
    assert(RTextJPUtf8Encode(0x110000, encoded) == 0);

    assert(RTextJPUtf8Encode(0x7F, encoded) == 1);
    assert(RTextJPUtf8Encode(0x80, encoded) == 2);
    assert(RTextJPUtf8Encode(0x7FF, encoded) == 2);
    assert(RTextJPUtf8Encode(0x800, encoded) == 3);
    assert(RTextJPUtf8Encode(0xFFFF, encoded) == 3);
    assert(RTextJPUtf8Encode(0x10000, encoded) == 4);
    assert(RTextJPUtf8Encode(0x10FFFF, encoded) == 4);
}

static void AssertTruncatedSequence(const unsigned char *sequence)
{
    const char *text = (const char *)(const void *)sequence;
    int bytes = 0;
    bool valid = true;

    assert(rtextjp_decode_checked(text, &bytes, &valid) == 0xFFFD);
    assert(bytes == 1);
    assert(!valid);
    assert(!RTextJPUtf8IsValid(text));
}

static void TestTruncatedSequences(void)
{
    const unsigned char truncated2_1[] = { 0xC2u, 0x00u };
    const unsigned char truncated3_1[] = { 0xE3u, 0x00u };
    const unsigned char truncated3_2[] = { 0xE3u, 0x81u, 0x00u };
    const unsigned char truncated4_1[] = { 0xF0u, 0x00u };
    const unsigned char truncated4_2[] = { 0xF0u, 0x90u, 0x00u };
    const unsigned char truncated4_3[] = { 0xF0u, 0x90u, 0x80u, 0x00u };

    AssertTruncatedSequence(truncated2_1);
    AssertTruncatedSequence(truncated3_1);
    AssertTruncatedSequence(truncated3_2);
    AssertTruncatedSequence(truncated4_1);
    AssertTruncatedSequence(truncated4_2);
    AssertTruncatedSequence(truncated4_3);
}

static void TestValidationAndIndex(void)
{
    const unsigned char invalidBytes[] = { 0xC0u, 0xAFu, 0x00u };
    const unsigned char truncated[] = { 0xE3u, 0x81u, 0x00u };
    const unsigned char surrogate[] = { 0xEDu, 0xA0u, 0x80u, 0x00u };
    const unsigned char tooLarge[] = { 0xF4u, 0x90u, 0x80u, 0x80u, 0x00u };
    const char *invalid = (const char *)(const void *)invalidBytes;
    int bytes = 0;

    assert(RTextJPUtf8IsValid("日本A😀"));
    assert(!RTextJPUtf8IsValid(invalid));
    assert(!RTextJPUtf8IsValid((const char *)(const void *)truncated));
    assert(!RTextJPUtf8IsValid((const char *)(const void *)surrogate));
    assert(!RTextJPUtf8IsValid((const char *)(const void *)tooLarge));
    assert(!RTextJPUtf8IsValid(NULL));
    assert(RTextJPUtf8Decode(invalid, &bytes) == 0xFFFD);
    assert(bytes == 1);
    assert(RTextJPUtf8CodepointCount("日本A😀") == 4);
    assert(RTextJPUtf8CodepointCount(invalid) == 2);
    assert(RTextJPUtf8ByteIndex("日本A", 0) == 0);
    assert(RTextJPUtf8ByteIndex("日本A", 1) == 3);
    assert(RTextJPUtf8ByteIndex("日本A", 2) == 6);
    assert(RTextJPUtf8ByteIndex("日本A", 99) == 7);
}

static void TestFixedInputBuffer(void)
{
    char storage[16] = "日";
    char small[5] = "";
    char truncated[5] = "A日";
    char invalidStorage[] = "A\xC0" "B";
    const char invalidText[] = "A\xC0" "B";
    RTextJPInput input;
    RTextJPInput limited;
    RTextJPInput truncatedInput;
    RTextJPInput invalidInput;

    RTextJPInputInit(&input, storage, (int)sizeof(storage));
    assert(input.length == 3);
    assert(input.cursor == 3);
    assert(RTextJPInputInsert(&input, "本A"));
    assert(strcmp(storage, "日本A") == 0);
    assert(input.length == 7);

    assert(RTextJPInputSetText(&input, "山田"));
    assert(strcmp(storage, "山田") == 0);

    RTextJPInputInit(&limited, small, (int)sizeof(small));
    assert(!RTextJPInputSetText(&limited, "日本"));
    assert(strcmp(small, "日") == 0);
    assert(RTextJPUtf8IsValid(small));

    /* A capacity cut through a multibyte character must preserve the valid prefix. */
    RTextJPInputInit(&truncatedInput, truncated, 4);
    assert(strcmp(truncated, "A") == 0);
    assert(truncatedInput.length == 1);
    assert(truncatedInput.cursor == 1);

    RTextJPInputInit(&invalidInput, invalidStorage, (int)sizeof(invalidStorage));
    assert(strcmp(invalidStorage, "A") == 0);
    assert(invalidInput.length == 1);
    assert(!RTextJPInputSetText(&invalidInput, invalidText));
    assert(strcmp(invalidStorage, "AB") == 0);
}

int main(void)
{
    TestDecodeAndEncode();
    TestTruncatedSequences();
    TestValidationAndIndex();
    TestFixedInputBuffer();
    puts("RTextJP UTF-8 tests passed");
    return 0;
}
