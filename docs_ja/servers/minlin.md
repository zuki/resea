# MinLin - Linuxプログラムのためのバイナリ互換レイア（実験的）

MinLinは2つあります。MinLinサーバとMinLin Linuxディストリビューションです。

MinLinサーバは、Resea上でLinuxバイナリを*修正なし*でネイティブに動作させるための
ABI互換性（特にLinuxシステムコール）の提供を担当します。

MinLin Linuxはこの互換性レイヤーをテストするための最小限のLinuxディストリ
ビューションです。もちろん、Linuxカーネル上でも動作します。

## ソースコード

[servers/minlin](https://github.com/nuta/resea/tree/master/servers/minlin)
