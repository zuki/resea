# ユニットテスト

`libs/unittest`は（`libs`にある）ライブラリと（`servers`にある）ユーザ空間
アプリケーションのための非常にプリミティブなユニットテストフレームワークを
提供します。少し複雑な関数を書いている場合に役に立ちます。

このフレームワークは魅力的な特徴と機能を持っています。

- フレームワークはテストを開発環境（macOSなど）で実行する普通のユーザ空間
  アプリケーションにコンパイルします。
- テストプログラムはネイティブアプリケーションなので超早く、LLDBなどの
  好みのデバッグツールを使用することができます。
- [Undefined Behavior Sanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html) と [Address Santizer](https://clang.llvm.org/docs/AddressSanitizer.html)が
  デフォルトで有効になっています。

## 補足説明

- テスト環境からは（メッセージパッシングなどの）システムコールを使うことはできません
- Reseaアプリケーションの`main()`は呼び出されません。

## テストを書く

```c
#include <unittest.h>

int add(int a, int b) {
    return a + b;
}

TEST("1 + 1 equals to 2") {
    TEST_EXPECT_EQ(add(1, 1), 2);
}
```

### マクロ

- `TEST("description")`: ユニットテスト関数の定義にはこのマクロを使用します。
- `TEST_EXPECT_EQ(a, b)`: `a == b` であるかチェックする。
- `TEST_EXPECT_NE(a, b)`: `a != b` であるかチェックする。
- `TEST_EXPECT_LT(a, b)`: `a < b` であるかチェックする。
- `TEST_EXPECT_LE(a, b)`: `a <= b` であるかチェックする。
- `TEST_EXPECT_GT(a, b)`: `a > b` であるかチェックする。
- `TEST_EXPECT_GE(a, b)`: `a >= b` であるかチェックする。

## テストを実行する

```
$ make unittest TARGET=servers/apps/test
```
