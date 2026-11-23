# Arduino 目覚まし時計

Arduino Uno と RTC モジュール（RX8130CE）を使ったデジタル時計プロジェクトです。
16×2 LCD に現在時刻を表示し、4つのボタンで時刻設定ができます。

## ハードウェア構成

| 部品 | 型番・仕様 |
|------|-----------|
| マイコンボード | Arduino Uno |
| RTC モジュール | Epson RX8130CE |
| LCD | 16×2 キャラクタ LCD（I2C アドレス: 0x27） |
| ボタン | タクトスイッチ × 4 |

### ピン配置

| ピン番号 | 用途 |
|---------|------|
| 7 | + ボタン（値を増やす） |
| 8 | − ボタン（値を減らす） |
| 9 | ENTER ボタン（次の項目へ進む / 保存） |
| 10 | BACK ボタン（前の項目へ戻る） |

RTC・LCD はいずれも I2C（SDA/SCL）接続です。

## 使用ライブラリ

- [ArtronShop_RX8130CE](https://github.com/ArtronShop/ArtronShop_RX8130CE) — RTC モジュール制御
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) — I2C LCD 制御
- Wire（Arduino 標準）

## 開発環境

- [PlatformIO](https://platformio.org/)
- プラットフォーム: `atmelavr`
- ボード: `uno`
- フレームワーク: `arduino`

## 機能

### 時刻表示

起動・設定完了後、LCD に以下の形式で現在時刻を表示します。

```
2026/11/21
00:00:00
```

### 時刻設定

電源投入時に時刻設定モードに入ります。以下の順番で年・月・日・時・分・秒を設定します。

| ステップ | 表示 | 設定範囲 |
|---------|------|---------|
| 1 | SET YEAR | 2026 〜 2099 |
| 2 | SET MONTH | 1 〜 12 |
| 3 | SET DAY | 1 〜 31 |
| 4 | SET HOUR | 0 〜 23 |
| 5 | SET MINUTE | 0 〜 59 |
| 6 | SET SECOND | 0 〜 59 |
| 7 | CONFIRM TIME | ENTER で RTC に保存 |

#### ボタン操作

| ボタン | 動作 |
|--------|------|
| `+` | 表示中の値を +1 する |
| `−` | 表示中の値を -1 する |
| `ENTER` | 次の設定項目へ進む（確認画面では RTC に保存） |
| `BACK` | 前の設定項目へ戻る |

値は上限・下限で折り返します（例: 23 の次は 0）。

## ビルドと書き込み

```bash
# ビルド
pio run

# 書き込み
pio run --target upload

# シリアルモニタ
pio device monitor --baud 9600
```

## ディレクトリ構成

```
.
├── src/
│   └── LCDtest.cpp   # メインソースコード
├── include/          # ヘッダファイル置き場（現在未使用）
├── lib/              # プロジェクトローカルライブラリ置き場
├── test/             # テストコード置き場
└── platformio.ini    # PlatformIO 設定ファイル
```
