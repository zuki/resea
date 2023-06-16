# hdaudio (実験的)
Intel HDオーディオコントローラ＆スピーカー用のデバイスドライバです。

現在のところ、このドライバは実験的な実装であり、ハードコーディングされた
サウンドデータを繰り返し演奏するだけです。

## ビルド
演奏するwavファイル (44.1kHz, 16-bits, 2 channels) が必要です。
`wav2c.py` を実行して `sound_data.h` を生成してください。

```
$ cd servers/experimental/hdaudio
$ ./wav2c.py <your_wav_file>
```

## QEMU上でのテスト
`HD_AUDIO`によりIntel HDオーディオデバイスを
有効にしてください。:

```
$ make run HD_AUDIO=y
```

## 参考資料
- [ATA PIO Mode - OSDev Wiki](https://wiki.osdev.org/ATA_PIO_Mode)

## ソースコード
[servers/drivers/blk/ide](https://github.com/zuki/resea/tree/master/servers/drivers/blk/ide)
