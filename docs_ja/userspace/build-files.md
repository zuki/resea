# ファイルのビルド

サーバのディレクトリに`build.mk`を追加します。

```mk
# build.mk

# サーバ名。/[a-zA-Z_][a-zA-Z0-9_]*/ でなければならない
name := ps2_keyboard
# 簡単な説明
description := PS/2 Keyboard Driver
# オブジェクトファイル
objs-y += main.o
# 依存ライブラリ
libs-y += driver
```

ビルド構成に追加したい場合は同じディレクトリに`Kconfig`ファイルを追加します。

```
menu "ps2_keyboard - PS/2 Keyboard Driver"
    # PS2_KEYBOARD_SERVER is set if this `ps2_keyboard` is enabled in the config.
	depends on PS2_KEYBOARD_SERVER

    config PRINT_PERIODICALLY
        bool "Print a message every second"
endmenu
```

詳しくは[Kconfig Language](https://www.kernel.org/doc/html/latest/kbuild/kconfig-language.html)を参照してください。
