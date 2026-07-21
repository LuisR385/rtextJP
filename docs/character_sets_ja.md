# 日本語文字集合プリセット

## 先に結論

画面に出す文章が決まっているなら、従来の `RTextJPLoadFontFromText()` が最も軽量です。名前入力や
外部データなど、実行前に全文字を列挙できない場合だけ `RTextJPCharacterSet` を使って範囲を広げます。

文字集合を広げるほど、フォントアトラスの作成時間、メモリ、VRAM使用量が増えます。迷った場合は
「カスタムのみ」から始め、足りない範囲だけ追加してください。

![文字集合プリセットごとの要求グリフ数](assets/character-sets.png)

## 選べる値

表示可能ASCII 95文字と全角空白 `U+3000` は、どの選択でも自動追加されます。次の「実際の要求数」は
その自動追加と重複除去を反映した値です。

| 列挙値 | プリセット本体 | 実際の要求数 | 主な用途 |
|---|---:|---:|---|
| `RTEXTJP_CHARACTER_SET_CUSTOM` | 0文字 | 96 | 表示文を追加文字列へすべて列挙する |
| `RTEXTJP_CHARACTER_SET_JIS_X_0208_KANA` | 169文字 | 265 | 平仮名・片仮名の自由入力 |
| `RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI` | 524文字 | 619 | 記号、英数字、かな、ギリシャ・キリル文字など |
| `RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1` | 2,965文字 | 3,061 | 第1水準漢字だけを追加する |
| `RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_2` | 3,390文字 | 3,486 | 第2水準漢字だけを追加する |
| `RTEXTJP_CHARACTER_SET_JIS_X_0208` | 6,879文字 | 6,974 | 非漢字・第1水準・第2水準をまとめて要求する |

`LEVEL_1` と `LEVEL_2` は漢字だけです。かなや句読点まで含む普通の日本語テキストには、
`NON_KANJI` と必要な漢字水準を組み合わせます。

## 組み合わせる

列挙値はビットフラグです。CではビットORの結果を `RTextJPCharacterSet` へキャストします。

```c
RTextJPCharacterSet set = (RTextJPCharacterSet)(
    RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI |
    RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1);

const char *custom = "髙島";  // プリセット外の固有名詞も追加できる
int requested = RTextJPCharacterSetCodepointCount(set, custom); // 3,585
Font font = RTextJPLoadFontFromCharacterSet("japanese.ttf", 32, set, custom);
```

同じコードポイントは自動で1個にまとめられます。`custom` がプリセット内の文字を含んでも、
アトラスへ重複登録されません。改行・タブ・不正なUTF-8は追加文字から除外されます。

## アプリ独自の列挙型を作る

画面側へJISの詳細を漏らしたくない場合は、アプリ用の分かりやすい列挙型へまとめられます。

```c
typedef enum GameFontProfile {
    GAME_FONT_MENU,
    GAME_FONT_NAME_INPUT,
    GAME_FONT_ARCHIVE
} GameFontProfile;

static RTextJPCharacterSet GameFontCharacterSet(GameFontProfile profile)
{
    switch (profile) {
        case GAME_FONT_NAME_INPUT:
            return (RTextJPCharacterSet)(RTEXTJP_CHARACTER_SET_JIS_X_0208_NON_KANJI |
                                       RTEXTJP_CHARACTER_SET_JIS_X_0208_LEVEL_1);
        case GAME_FONT_ARCHIVE:
            return RTEXTJP_CHARACTER_SET_JIS_X_0208;
        case GAME_FONT_MENU:
        default:
            return RTEXTJP_CHARACTER_SET_CUSTOM;
    }
}
```

この形なら、後から「名前入力だけ第2水準も許す」といった変更を1か所で行えます。

## 要求数と実際のグリフ数は別

`RTextJPCharacterSetCodepointCount()` はRTextJPがフォントへ要求する重複なしコードポイント数です。
フォント自体に文字がなければ、raylibはそのグリフをアトラスへ追加できません。

```c
int requested = RTextJPCharacterSetCodepointCount(set, custom);
Font font = RTextJPLoadFontFromCharacterSet(path, 32, set, custom);

TraceLog(LOG_INFO, "requested=%d, font provided=%d", requested, font.glyphCount);
```

ギャラリー生成時に使った Noto Sans JP では、JIS全体とカスタム文字「髙」を合わせて6,975文字を
要求し、6,974グリフが読み込まれました。不足していたのは `U+2252`（`≒`）です。この結果は
使用フォントによって変わります。プリセットは、フォントにない字形を新しく作る機能ではありません。

## JISプリセットと文字コード

プリセットは「JIS X 0208に対応するUnicodeコードポイントの集合」です。RTextJPの公開文字列は引き続き
すべてUTF-8であり、Shift_JISやEUC-JPのバイト列を受け付けるようになるわけではありません。

生成データはJISの区点位置をUnicodeへ写し、第1水準2,965字、第2水準3,390字、非漢字524字、
合計6,879字になることを生成時と自動テストの両方で確認しています。再生成スクリプトは
[`tools/generate_jis_tables.py`](../tools/generate_jis_tables.py) です。

## 失敗しにくい選び方

1. 固定UIは `RTextJPLoadFontFromText()` を使う。
2. かな入力だけなら `KANA` と固有名詞を組み合わせる。
3. 一般的な日本語入力なら `NON_KANJI | LEVEL_1` から試す。
4. 稀な漢字が必要と分かってから `LEVEL_2` を足す。
5. 全JISを選ぶ場合は `baseSize` を小さめにし、実機で作成時間とテクスチャサイズを確認する。