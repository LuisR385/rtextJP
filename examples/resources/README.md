# 日本語フォントを置く場所

このディレクトリへ、日本語グリフを含むTTFまたはOTFを `japanese.ttf` という名前で置くと、
exampleを引数なしで実行できます。拡張子を変えたくない場合は、実行時に元のパスを渡してください。

```text
rtextjp_01_hello_japanese C:/path/to/font.otf
```

RTextJPのソースリポジトリはフォント本体をコミットしません。`japanese.ttf` と
`japanese.otf` は `.gitignore` 対象です。フォントごとに再配布条件、ゲームへの組み込み条件、
クレジット要件が異なるため、プロジェクトで利用可能なライセンスを持つ日本語フォントを
選んでください。

Noto Sans JPをローカルへ置く場合は、このディレクトリの `OFL.txt` と `README.txt` を一緒に
保持してください。WindowsデモZIPの作成ターゲットは、フォント本体とこれらの文書をまとめて
配布物へコピーします。

exampleは `RTextJPLoadFontFromText()` に表示文字を渡すため、フォントファイル全体のグリフをGPUへ
読み込むわけではありません。ただし、元フォント自体に存在しない文字は表示できません。
