# 最初の10分

このページでは、raylibを使ったことがあるC言語初心者が、最初の日本語を表示するまでを説明します。

## 1. ファイルを置く

自分のプロジェクトに `include/rtextjp.h` をコピーします。raylibの `raylib.h` にもコンパイラから
到達できるようにしてください。`rtextjp.h` 自体にはraylib本体を含んでいないため、リンク時には
通常どおりraylibライブラリが必要です。

ソースファイルはUTF-8で保存してください。Visual C++をCコンパイラとして使う場合は `/utf-8` を
指定します。このリポジトリのCMake設定は自動で指定します。

## 2. 実装を1回だけ有効にする

小さなプログラムなら、`main.c` の先頭を次のようにします。

```c
#define RTEXTJP_IMPLEMENTATION
#include "rtextjp.h"
```

複数の `.c` ファイルがある場合、`RTEXTJP_IMPLEMENTATION` を書くのは1ファイルだけです。
2ファイル以上で定義すると、リンク時に「同じ関数が複数ある」というエラーになります。

## 3. フォントと文字を用意する

日本語グリフを含むTTFまたはOTFを用意します。次のように、画面で使う文字列を連結して渡します。

```c
const char *title = "冒険のはじまり";
const char *button = "ゲーム開始";
const char *allCharacters = "冒険のはじまりゲーム開始";

Font font = RTextJPLoadFontFromText("japanese.ttf", 48, allCharacters);
if (!IsFontValid(font)) {
    // パス、形式、ファイル破損などを確認する
}
```

文字列をそのまま隣に並べて連結しても構いません。

```c
const char *allCharacters =
    "冒険のはじまり"
    "ゲーム開始"
    "Options";
```

同じ文字はRTextJPが自動で除きます。ASCIIは自動追加されるため、英数字をすべて列挙する必要は
ありません。`baseSize` は32〜64程度から始めると分かりやすく、あとで表示時に拡大・縮小できます。

### 入力文字を事前に決められない場合

自由入力などでは、JIS X 0208の範囲を列挙型で選べます。第1水準・第2水準は漢字だけなので、
普通の日本語文には非漢字集合も組み合わせます。

```c
RTextJPCharacterSet set = (RTextJPCharacterSet)(
    RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI |
    RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1);

Font font = RTextJPLoadFontFromCharacterSet("japanese.ttf", 32, set, "髙島");
```

固定UIでは、上の大きな集合より `RTextJPLoadFontFromText()` の方が軽量です。詳しい文字数と
選択基準は [日本語文字集合プリセット](character_sets_ja.md) を参照してください。

## 4. 1行を描く

```c
RTextJPStyle style = RTextJPStyleDefault(font, 32.0f, BLACK);
RTextJPDraw(title, (Vector2){ 40.0f, 40.0f }, style);
```

`RTextJPStyleDefault()` で安全な初期値を作ってから、一部だけ変更してください。構造体の全メンバーを
自分で埋める必要はありません。

## 5. 矩形へ折り返す

```c
Rectangle area = { 40.0f, 100.0f, 520.0f, 220.0f };
RTextJPStyle style = RTextJPStyleDefault(font, 28.0f, DARKGRAY);

style.alignment = RTEXTJP_ALIGN_LEFT;
style.verticalAlignment = RTEXTJP_VALIGN_TOP;
RTextJPDrawBox("日本語は空白がなくても、幅に合わせて折り返せます。", area, style);
```

標準では次が有効です。

- `wrap = true`: 矩形幅で折り返す
- `kinsoku = true`: 開き括弧を行末、句読点を行頭に置かない
- `clip = true`: 矩形外を描画しない

外側ですでに `BeginScissorMode()` を使っている場合、入れ子を避けるため `clip = false` にします。

## 6. 終了時に解放する

フォントはウィンドウを閉じる前に解放します。

```c
UnloadFont(font);
CloseWindow();
```

## うまく表示されないとき

### 四角や疑問符になる

フォントにそのグリフがないか、`RTextJPLoadFontFromText()` の第3引数へ文字を含めていません。
表示する全テキストをまとめた文字列を見直してください。

### 文字化けする

ソースをUTF-8で保存し、コンパイラの入力・実行文字セットもUTF-8にします。Windowsの古い
コードページで保存した日本語リテラルはUTF-8として解釈できません。

### `RTEXTJP_IMPLEMENTATION` に関するリンクエラー

- 未解決シンボル: どの `.c` にも実装マクロがない
- 多重定義: 複数の `.c` で実装マクロを定義している

### 入力した漢字だけ表示されない

入力される可能性がある文字をフォント作成時に含める必要があります。自由入力で全漢字を扱う
場合は、アプリ側で対象文字集合を定める、フォントアトラスを作り直す、または動的グリフキャッシュを
持つ別の仕組みが必要です。RTextJPのJISプリセットは対象範囲を広げられますが、フォント自体にない字形を
作ることはできず、巨大な全CJKアトラスも自動生成しません。
