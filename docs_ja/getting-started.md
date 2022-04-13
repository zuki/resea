# 始めよう

## 依存システム

- LLVM/Clang/LLD バージョン8.x以降
- Python バージョン3.7以降
- QEMU
- Bochs *(オプション)*
- GRUBとxorriso *(オプション)*
- Docker *(オプション: Linux ABIはパッケージのビルドにDocterを使用します)*

### macOS

```
$ brew install llvm python3 qemu bochs i386-elf-grub xorriso mtools sparse
$ brew cask install gcc-arm-embedded
$ pip3 install -r tools/requirements.txt
```

### Ubuntu 20.04

```
$ apt install llvm lld clang qemu bochs grub2 xorriso mtools \
    python3 python3-dev python3-pip python3-setuptools gcc-arm-none-eabi \
    clang-format clang-tidy sparse
$ pip3 install --user -r tools/requirements.txt
$ cargo install cargo-xbuild
```

### Rust

Rustアプリケーションを作成したい場合は、Rustツールチェーンをインストールしてください。

1. [rustup](https://rustup.rs/)のインストール
2. rust-srcコンポートの追加: `rustup component add rust-src --toolchain nightly`.

## Reseaのビルド

```
$ make build               # カーネル実行ファイルのビルド
$ make iso                 # ISOイメージのビルド
$ make build BUILD=release # カーネル実行ファイルのビルド（リリースビルド）
$ make build V=1           # 冗長なコマンド出力を伴うビルド
$ make clean               # 生成されたファイルの削除
```
## Reseaの実行

```
$ make run GUI=1     # QEMUでの実行
$ make run           # QEMU -nographicでの実行
$ make bochs         # Bochsでの実行
```
