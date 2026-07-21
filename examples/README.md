# example集の読み方

## まず動かす

Windowsでは、リポジトリのルートにある `START_HERE.bat` をダブルクリックし、表示されたメニューで
`1` を入力してEnterを押してください。ビルドエラーや起動エラーが起きても画面は自動で閉じません。

すべてC99の独立したサンプルです。各ファイルが `RTEXTJP_IMPLEMENTATION` を定義しているため、
1ファイルずつ実行できます。自分の複数ファイル構成へコピーするときは、実装マクロを1か所だけに
戻してください。

## 準備

Windowsでは、リポジトリ内またはWindows標準の日本語フォントを自動検出します。
自分でフォントを指定する場合は、日本語を含むTTFまたはOTFを次の名前で置きます。

```text
examples/resources/japanese.ttf
```

別の場所に置いた場合は、実行時の第1引数にパスを渡せます。

```text
rtextjp_02_text_box D:/fonts/MyJapaneseFont.otf
```

フォントを同梱していない理由と選び方は [resources/README.md](resources/README.md) を参照してください。

## おすすめの順番

### 1. `01_hello_japanese.c`

最初はここだけ読んでください。次の4段階が分かります。

1. 実装マクロを定義する
2. ウィンドウ初期化後に日本語フォントを読み込む
3. 標準スタイルを作る
4. 終了前にフォントを解放する

### 2. `02_text_box.c`

長い日本語と英語を矩形内へ表示します。`K` キーで禁則処理を切り替えられるため、右端付近の
括弧や句読点の動きを比較できます。

### 3. `03_alignment.c`

3つの矩形へ左・中央・右揃えで表示します。`RTextJPMeasure()` の結果も画面に描き、レイアウト前に
必要サイズを取得する使い方を示します。

### 4. `04_vertical_writing.c`

日本語で一般的な右から左へ進む縦書きと、左から右へ進む配置を比較します。これは本格的な
出版組版ではなく、ゲーム画面向けの簡易配置です。

### 5. `05_text_input.c`

固定バッファ、クリックによるアクティブ切り替え、UTF-8入力、貼り付けを扱います。未知の漢字を
入力する場合にフォント文字集合も必要になる点をコメントで説明しています。

### 6. `06_utf8_tools.c`

ほかのexampleと同じくウィンドウを開くGUI例です。`strlen()` との違い、コードポイント、バイト数、
UTF-8検証を表形式で確認できます。

### 7. `07_character_sets.c`

数字キー `1`〜`5` で、カスタムのみ、かな、第1水準、第2水準、JIS X 0208全体を切り替えます。
各プリセットの要求グリフ数と、実際に作成された `Font.glyphCount` を同じ画面で比較できます。
プリセットを広げると再構築に時間がかかることも体験できるため、固定UIでは必要文字だけを渡す
方式が軽量であることも分かります。

`rtextjp_test_utf8.exe`、`rtextjp_test_layout.exe`、`rtextjp_test_character_sets.exe` は内部の自動検証
プログラムで、exampleではありません。
通常のビルドでは生成されず、`RTEXTJP_BUILD_TESTS=ON` を明示した場合だけ `build/tests/` に出力されます。

## CMake

リポジトリのルートで次を実行します。

```text
cmake -S . -B build -DRTEXTJP_BUILD_EXAMPLES=ON -DRTEXTJP_FETCH_RAYLIB=ON
cmake --build build
```

インストール済みのraylib 6.0を使う場合は `RTEXTJP_FETCH_RAYLIB=ON` を省略できます。取得は明示的に
有効化した場合だけ行われ、公式raylibの固定タグ `6.0` を使用します。

実行ファイルの場所は利用したCMakeジェネレーターによって `build/` または `build/Debug/` などに
なります。実行時の作業ディレクトリが違う場合は、フォントの絶対パスを第1引数で渡してください。
