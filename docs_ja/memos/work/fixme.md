# 要検討

## `libs/common/arch/arm64/arch_types.h`

- `trap_frame_t`型の定義は必要か

## `kernel/arch/arm64`, `kernel/arch/arm64/machines/raspi3`

## `libs/resea/arch/arm64/`

### `arch/syscall.h`

- aarch64ではSYSCALL_NRはx0ではなく、x8で渡す
  - これはlinuxがそうしているだけでxv6(musl)はそれに合わせているだけだった。
    reseaのhandle_syscall()はSYSCALL_NRを第1引数で受けているのでx0で問題なし。

### `kernel/boot.c#mpmain()`

- cpuvars[]はどこで設定している?
    - IDLE_TASKのマクロ定義が疑問だったがidle_taskは実体で定義されているので
      `cpuvars[NUM_CPUS_MAX]`と宣言された時点で作成されるので問題ない

    ```c
    # kernel/arch/arm64/arch.h
    static inline struct cpuvar *get_cpuvar(void) {
        return arm64_get_cpuvar();
    }

    # kernel/arch/arm64/mp.c
    static struct cpuvar cpuvars[NUM_CPUS_MAX];
    struct cpuvar *arm64_get_cpuvar(void) {
        return &cpuvars[mp_self()];
    }

    # kernel/task.h
    #define IDLE_TASK (&get_cpuvar()->idle_task)    // idle_taskはポインタではなく実体
    error_t err = task_create(IDLE_TASK, "(idle)", 0, NULL, 0);
    struct cpuvar {
        struct arch_cpuvar arch;
        struct task *current_task;
        struct task idle_task;
    };

    ```

### `kernel/arch/arm64/init.c#L71-72`

- bit[0] = 1: カウンタを有効にする
- bit[1] = 1: カウンタをリセットする

```
ARM64_MSR(pmcr_el0, 0b1ull);   // Reset counters.   # 0b110ull
ARM64_MSR(pmcr_el0, 0b11ull);  // Enable counters.  # ob1ull
```
