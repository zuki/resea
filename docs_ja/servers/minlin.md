# MinLin - Linuxプログラムのためのバイナリ互換レイア（実験的）

MinLinは2つの要素でできています。MinLinサーバとMinLin Linuxディストリビューションです。

MinLinサーバは、Resea上でLinuxバイナリを*修正なし*でネイティブに動作させるための
ABI互換性（特にLinuxシステムコール）の提供を担当します。

MinLin Linuxはこの互換レイヤをテストするための最小限のLinuxディストリ
ビューションです。もちろん、Linuxカーネル上でも動作します。

## ソースコード

[servers/minlin](https://github.com/zuki/resea/tree/master/servers/minlin)
