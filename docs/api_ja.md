# APIの選び方

## 最初に使う3つ

### `RTextJPLoadFontFromText()`

日本語フォントを、実際に使う文字だけで読み込みます。返された `Font` はraylibの通常のフォントと
同じため、必要なら `DrawTextEx()` でも使えます。

### `RTextJPLoadFontFromCharacterSet()`

JIS X 0208のかな、非漢字、第1水準、第2水準を列挙型で選び、任意のUTF-8文字列を追加します。
固定UIには `FromText()`、入力範囲を事前に列挙できない画面には `FromCharacterSet()` という使い分けです。

```c
RTextJPCharacterSet set = (RTextJPCharacterSet)(
    RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI |
    RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1);
Font font = RTextJPLoadFontFromCharacterSet(path, 32, set, "髙島");
```

`RTextJPCharacterSetCodepointCount()` でアトラス作成前の要求数を確認できます。戻り値はフォントが
実際に持つグリフ数ではありません。詳しくは [日本語文字集合プリセット](character_sets_ja.md) を
参照してください。

### `RTextJPStyleDefault()`

描画スタイルの初期値を作ります。`fontSize` は表示時の大きさで、フォント読み込み時の `baseSize`
とは別に変更できます。

### `RTextJPDrawBox()`

ほとんどの文章表示にはこの関数を使います。矩形幅で自動折り返しし、揃え、禁則、クリップを
まとめて処理します。

## 描画関数の使い分け

| やりたいこと | 関数 |
|---|---|
| 座標から短い文字列を描く | `RTextJPDraw()` |
| 説明文をパネル内へ収める | `RTextJPDrawBox()` |
| 描画前に必要サイズを知る | `RTextJPMeasure()` |
| 簡易縦書きを描く | `RTextJPDrawVertical()` |
| 縦書きの列数とサイズを知る | `RTextJPMeasureVertical()` |
| 1行入力欄を描く | `RTextJPDrawInputBox()` |

`RTextJPDraw()` は明示的な改行だけを処理し、自動折り返しや揃えは行いません。揃えたい場合は
`RTextJPDrawBox()` に幅のある矩形を渡してください。

## 揃え

```c
style.alignment = RTEXTJP_ALIGN_CENTER;
style.verticalAlignment = RTEXTJP_VALIGN_MIDDLE;
```

横方向は `LEFT`、`CENTER`、`RIGHT`、縦方向は `TOP`、`MIDDLE`、`BOTTOM` です。
文字列全体の高さが矩形より大きい場合でも揃え計算は行われ、その後 `clip` によって切り抜かれます。

## 折り返しと禁則

日本語・CJK文字の前後は、空白がなくても折り返し候補になります。ラテン文字は空白を優先し、
単語が矩形より長い場合だけ文字単位で分割します。

`kinsoku = true` では代表的な次の文字を扱います。

- 行末に置かない: `（「『【` などの開き括弧
- 行頭に置かない: `、。！？）」』】`、小書き仮名、長音記号など

これは読みやすいゲームUIを目的にした簡易処理です。ルビ、分離禁止、ぶら下げ、約物の詰め、
OpenTypeの高度な組版は行いません。

## UTF-8補助

- `RTextJPUtf8Decode()`: 先頭1文字を読む
- `RTextJPUtf8Encode()`: 1コードポイントを書き出す
- `RTextJPUtf8IsValid()`: 文字列全体を検証する
- `RTextJPUtf8CodepointCount()`: バイト数ではなく文字数を数える
- `RTextJPUtf8ByteIndex()`: n文字目のバイト位置を得る

Cの `strlen()` はバイト数を返します。日本語の「日本」はUTF-8で6バイトですが、RTextJPの
コードポイント数は2です。書記素クラスタまではまとめないため、結合文字や絵文字ZWJシーケンスは
複数コードポイントとして数えます。

## 入力欄

```c
char buffer[256] = "";
RTextJPInput input;
RTextJPInputInit(&input, buffer, sizeof(buffer));

// クリックなどで切り替える
input.active = true;

// 毎フレーム
if (RTextJPInputUpdate(&input)) {
    // bufferが変更された
}
RTextJPDrawInputBox(&input, bounds, textStyle,
                  RTextJPInputStyleDefault(), "名前を入力");
```

バッファは入力状態より長く生存させてください。スタック上で宣言する場合は、同じ関数のゲームループ
全体で使えば問題ありません。容量の単位は文字数ではなくバイト数です。日本語は通常1文字3バイト、
一部の文字や絵文字は4バイトです。

`RTextJPInputInsert()` は、OS固有のIME連携などで確定済みUTF-8文字列を受け取った場合にも使えます。

## メモリ方針

描画、測定、入力更新はヒープ確保しません。フォント読み込み関数と
`RTextJPCharacterSetCodepointCount()` は重複文字を除く一時配列を確保し、結果を得た直後に解放します。
組み込み環境などで置き換える場合、実装マクロより前に次を定義できます。

```c
#define RTEXTJP_MALLOC(size) my_alloc(size)
#define RTEXTJP_FREE(pointer) my_free(pointer)
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
```
