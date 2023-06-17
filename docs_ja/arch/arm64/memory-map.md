# メモリマップ (arm64)

```
LMA_OFFSET = 0xffff000000000000;
/* 物理アドレスの上限: 1GB - 76MB (GPU) = 948 MB */
// PHYS_TOP = 0x000000003B400000;
MEMORY {
    boot_ram (rwx): ORIGIN = 0x0000000000080000, LENGTH = 0x0001000
    ram (rwx):      ORIGIN = 0xffff000000081000, LENGTH = 0x2fff000
    lma_ram (rwx):  ORIGIN = 0x0000000000081000, LENGTH = 0x2fff000
}
```

## カーネル空間

- `kernel/arch/arm64/machines/raspi3/kernel.ld`

| アドレス                     | サイズ   | 説明                                     |
|------------------------------|----------|------------------------------------------|
| `0x0000_0000_0008_0000 - 0x0000_0000_0008_0fff`  | 4 KiB    | ブートコード             |
| `0x0000_0000_0008_1000 - 0x0000_0000_0307_ffff`  | 47 MiB   | カーネルイメージとデータ（物理アドレス） |
| `0xffff_0000_0008_1000 - 0xffff_0000_0307_ffff`  | 47 MiB   | カーネルイメージとデータ（仮想アドレス） |

## ユーザ仮想アドレス空間

- `libs/resea/arch/arm64/user.ld`
- vmやramdiskで大きなファイルを埋め込むため.rodata領域が大きい

| アドレス                                        | サイズ       | 説明                                                                  |
|-------------------------------------------------|--------------|-----------------------------------------------------------------------|
| `0x0010_0000 - 0x007f_efff`                     | 7 MiB     | .text, .rodata, .data                                                    |
| `0x0080_0000 - 0x0081_ffff`                     | 128 KiB   | .heap |
| `0x0082_0000 - 0x02ff_efff`                     |  44 MiB   | .bss  |
| `0x02ff_f000 - 0x02ff_ffff`                     | 4 KiB        | cmdline (so-called command line arguments)                            |
| `0x0300_0000 - 0x09ff_ffff`                     | 112 MiB       | free vaddr                                                         |


## メモリ定数（リンクアドレス: 物理アドレス）

|   変数名       |    アドレス   | 説明 |
|:------------------:|:-----------|:-----|
| __kernel_image | 0x0000_0000_0080_0000  | カーネルイメージの先頭  |
| __kernel_page_table | LMA(ALIGN(__bss_end, 4096)) | カーネルページテーブルの先頭 |
| __boot_stack_base | __kernel_pge_table + 0x4000  | 各CPU用のブートスタック領域の先頭  |
| __kernel_image_end | ALIGN(_boot_stack_base + (0x1000 * 16), 4096)  | カーネルイメージの終端 |

## ページテーブルの構成

|   オフセット       |    テーブル   |
|:------------------:|:-----------|
|   0x0000-0x1000   |   Level-0                           |
|   0x1000-0x2000   |   Level-1                           |
|   0x2000-0x3000   |   Level-2 (0x3000_0000-0x4000_0000) : ノーマルメモリ |
|   0x3000-0x4000   |   Level-2 (0x4000_0000-0x5000_0000) : デバイスアドレス(MMIO) |


### kernel/arch/arm64/machines/raspi3/boot.S

- カーネル用のストレートページング

```
    adrp x4, __kernel_page_table

    // レベル-0のテーブル（1要素）
    mov  x0, x4
    add  x1, x4, #0x00001000 // レベル1テーブルのベースアドレス.
    ldr  x5, =0x00000403     // テーブル記述子 (AF=1)
    orr  x1, x1, x5
    str  x1, [x0]

    // レベル-1のテーブル (2要素)
    add  x0, x4, #0x1000
    add  x1, x4, #0x2000     // レベル2テーブルのベースアドレス
    ldr  x5, =0x00000b03     // テーブル記述子 (nG=1, SH=11)
    orr  x1, x1, x5
    str  x1, [x0]

    add  x0, x0, #8
    add  x1, x4, #0x3000     // レベル2テーブルのベースアドレス
    ldr  x5, =0x00000b03     // テーブル記述子 (nG=1, SH=11)
    orr  x1, x1, x5
    str  x1, [x0]

    // レベル-2のテーブル: 0x0000_0000-0x3eff_ffff (1008MiB): 504要素: ノーマルメモリ
    add  x0, x4, #0x2000
    ldr  x1, =0x00000401    // ブロック記述子 (AF=1)
    ldr  x2, =504           // = 1008 (MiB) / 2 (MiB): 1ブロック2MiB
1:
    str  x1, [x0], 0x8
    add  x1, x1, #0x200000  // 1ブロック 2MiB
    sub  x2, x2, 1
    cbnz x2, 1b

    // Level-2テーブル: 0x3f00_0000-0x4100_0000 (32MiB): 16要素 MMIO
    ldr  x5, =0xffffffe3        // AttrIndxをクリア (the index in MAIR_EL1).
    and  x1, x1, x5
    orr  x1, x1, #0x00000004    // AttrIndxを1にセット (MMIO)
    ldr  x2, =16                // 16ブロック
2:
    str  x1, [x0], 0x8
    add  x1, x1, #0x200000      // 1ブロック 2MB
    sub  x2, x2, 1
    cbnz x2, 2b

enable_mmu:
    // ページングの構成.
    ldr  x0, =0x000000ff // bit[0:7]  = 0xff : ノーマルメモリ、キャッシュ可能
                         // bit[8:15] = 0x00 : デバイス-nGnRnE メモリ
    msr  mair_el1, x0

    /* 4KBページなら、0xb5103510 (xv6) とするべきだが変更するとストールする */
    ldr  x0, =0xb5103510 // TTBR0/1 共に4KiB ページ、リージョンサイズは 2^48.
    msr  tcr_el1, x0

    adrp x0, __kernel_page_table
    msr  ttbr1_el1, x0
    msr  ttbr0_el1, x0
```
