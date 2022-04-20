# ブートシーケンス

1. ブートローダ（GRUBなど）がカーネルイメージをロードします。
2. アーキテクチャ固有のブートコードがCPUと必須のペリフェラルを初期化します。
3. カーネルがデバッグやメモリ、プロセス、スレッドなどのサブシステムを
   初期化します（kernel/1. boot.c）。
4. カーネルがbootfsから最初のユーザランドプロセスを作成します。
5. この最初のユーザランドプロセス（通常は`vm`）がbootfsから各種サーバを
   生成します。

## 1. カーネルイメージのロード

1. kernel8.imgが0x80000にコピーされ0x80000から実行開始
2. kernel8.imgはresea.elfをbinary形式に変換したもの
3. resea.elfの先頭は`kernel/arch/arm64/machines/raspi3/boot.S`
4. したがって、boot.Sの実行

### resea.elfの冒頭

```
Disassembly of section .boot:

0000000000080000 <__kernel_image>:
   80000:   58000940    ldr x0, 80128 <halt+0xc>        // リテラルプールで解決
   80004:   d5181000    msr sctlr_el1, x0
   80008:   d2b00000    mov x0, #0x80000000             // 直値movで解決    #2147483648
   8000c:   d51c1100    msr hcr_el2, x0
   80010:   d28038a0    mov x0, #0x1c5                  // #453
   80014:   d51c4000    msr spsr_el2, x0
   80018:   10000060    adr x0, 80024 <el1_mode>
   8001c:   d51c4020    msr elr_el2, x0
   80020:   d69f03e0    eret

0000000000080024 <el1_mode>:
   80024:   d53800a1    mrs x1, mpidr_el1
   80028:   92400421    and x1, x1, #0x3
   8002c:   58000822    ldr x2, 80130 <halt+0x14>
   80030:   d2820003    mov x3, #0x1000                 // #4096
   80034:   91400442    add x2, x2, #0x1, lsl #12
   80038:   9b010864    madd    x4, x3, x1, x2
   8003c:   9100009f    mov sp, x4
   80040:   b4000041    cbz x1, 80048 <bsp_boot>
...
000000000008011c <halt>:
   8011c:   d503207f    wfi
   80120:   17ffffff    b   8011c <halt>
   80124:   d503201f    nop
   80128:   30d00800    .word   0x30d00800              // sctlr_el1への設定値
   8012c:   00000000    .word   0x00000000
   80130:   0066d000    .word   0x0066d000
   80134:   00000000    .word   0x00000000
   80138:   ffffffe3    .word   0xffffffe3
   8013c:   00000000    .word   0x00000000
   80140:   35103510    .word   0x35103510
   80144:   00000000    .word   0x00000000
   80148:   30d00801    .word   0x30d00801
   8014c:   00000000    .word   0x00000000
   80150:   0008a9a8    .word   0x0008a9a8
   80154:   ffff0000    .word   0xffff0000
```

## 2 arm64固有のブートコード: kernel/arch/arm64/machines/raspi3/boot.S

1. boot: el2の設定をしてel1 (2)へ
2. el1_mode: 各cpu用のspをセットして、bsp(cpu0)なら(3), ap(cpu1-3)は(4)へ
3. bsp_boot: paging tableの構築して(4)へ
4. enable_mmu: mmuを有効にして(5)へ
5. mmu_enabled: spをupperアドレスへ移動してarm64_init()を呼び出す

**注**: `LDR Rd, =const`は疑似命令で、constantがmov命令一つで実行できれば
`MOV Rd, #const`に、できなければリテラルプールを作成してPC相対として
アクセスする。詳細は[ドキュメント](https://developer.arm.com/documentation/dui0473/j/writing-arm-assembly-language/load-32-bit-immediate-values-to-a-register-using-ldr-rd---const)
を参照（[日本語の解説](https://nw.tsuda.ac.jp/lec/arm64/book1/ch05/)も）


### 2.2 kernel/arch/arm64/init.c#arm64_init()

1. 例外ベクタの登録
2. apは以下を実行、bspは(3)へ
   1. lock(): 何もせずreturn (FIXME)    (kernel/arch/arm64/mp.c)
   2. mpinit(): irqをマスクしてmpmain()を呼び出し (NORETRUN)
3. cpuvar[], bssの0クリア
4. arm64_peripherals_init(): uart, watchdog, timerの初期化
5. lock(): big lock
6. データキャッシュ、命令キャッシュの有効化
7. ベンチマーク用の設定
8. マシン固有のbootinfoの設定 (FIXEME)
9. kmain(&bootinfo)を呼び出し (NORETURN)

## 3. カーネルの初期化処理

### 3.1 kmain(): kernel/boot.c

1. task_init()
   1. runqueuの初期化
   2. tasks[]の初期化
   3. irq_owners[]の初期化
2. mp_start() # arch/arm64/mp.c
   1.  machine_mp_start(): ap (cpu1-3) の起動 # arch/arm64/machines/raspi3/mp.c
3. boot elf headerの読み込みとbootinfoの設定
   1. locate_bootelf_header(): boot elfの検索: header=vm.elf[0x10000]
   2. memcpy(&bootelf->bootinfo, bootinfo, sizeof(*bootinfo))
4. 最初のユーザランドタスクの作成 => (4)項
   1. task_lookup_unchecked(1) => &tasks[0] (tid=1)
   2. task_create(): vm server task
5. map_bootelf(): boot elfを仮想メモリにマップ
6. mpmain()の呼び出し

## 3.2 mpmain(): kernel/boot.c

1. task_create(): IDLE_TASK (tid=0) の作成
2. CURRENT = IDLE_TASK
3. arch_idele(): 無限ループ
