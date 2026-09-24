# ソフトクリームファン 🍦

甘くて、涼しくて、音がなる。
ソフトクリームの形をした、CuteeWorksのオリジナル電子工作作品です。

9V角電池で動く小さなファンに、マイコンによるPWM制御と音楽再生を組み合わせました。
電源を入れると音楽が流れ、モーターが回転します。

## できること

- ソフトクリーム型の筐体でファンが回る
- UIAPduinoでモーターをPWM制御
- 電源投入時に音楽を再生
- カスタム基板に部品を差し込んで組み立てられる
- 電子工作ワークショップで組み立て体験ができる

## 使用している部品

- UIAPduino（CH32V003F4）
- 9V角電池
- MINI360 降圧コンバーター（9V → 5V）
- DCモーター
- IRLU3410 MOSFET
- PAM8302A オーディオアンプ
- スピーカー
- スイッチ
- ソフトクリーム型自作基板

## 回路の概要

9V角電池からモーターを動かし、MINI360で5Vに降圧してUIAPduinoとオーディオアンプに電源を供給します。
モーターはMOSFETを介してUIAPduinoからPWM制御します。

```text
9V角電池
├─ モーター
└─ MINI360（9V → 5V）
   ├─ UIAPduino
   └─ PAM8302A → スピーカー
```



## 動作の流れ

1. 電源を入れる
2. 音楽が流れる
3. UIAPduinoがモーターをPWM制御する
4. ソフトクリームのようにファンが回る



## 開発環境

- Arduino IDE
- UIAPduino用CH32V00xボードパッケージ



## フォルダー構成

```text
images/
       　作品・展示・ワークショップの写真
　       チラシや配布資料の画像・PDF

music/   音楽

pcb/
         基板製造用ガーバーデータ
         回路図・基板設計資料

enclosure/   ソフトクリーム型筐体
```



## 安全上の注意

⚠️ 電池をショートさせないでください。発熱・発火の危険があります。

⚠️ UIAPduinoを回路に接続したまま、電池をつないだ状態でUSBを接続しないでください。必ず電池と回路を外してからUSBを接続してください。

⚠️ MINI360の出力電圧を確認してから部品を接続してください。目安は4.8〜5.2Vです。

⚠️ 回転中のモーターやファンに指や髪の毛を近づけないでください。

## CuteeWorks

電子工作を、もっと可愛く、もっと楽しく。

- Facebook: [https://www.facebook.com/jyoshidarake](https://www.facebook.com/jyoshidarake)
- Instagram: [https://www.instagram.com/jyoshidarake](https://www.instagram.com/jyoshidarake)

