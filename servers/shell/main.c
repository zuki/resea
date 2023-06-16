#include "commands.h"
#include <driver/irq.h>
#include <resea/ipc.h>
#include <resea/malloc.h>
#include <resea/printf.h>
#include <resea/syscall.h>
#include <string.h>

#define BOLD             "\e[1;34m"  // Bold.
#define PRINTF(fmt, ...) printf(BOLD fmt SGR_RESET, ##__VA_ARGS__)

static void skip_whitespaces(char **cmdline) {
    while (**cmdline == ' ') {
        (*cmdline)++;
    }
}

/** @ingroup shell
 * @brief コマンドライン入力文字列をパースしてargv[]に設定する
 * @param comdline コマンドライン入力文字列
 * @param argv 設定するargv[]へのポインタ
 * @param argv_max argv[]の最大インデックス
 * @param argv[]の要素数
*/
static int parse(char *cmdline, char **argv, int argv_max) {
    // 前方の空白を削除して、NULLなら終了
    skip_whitespaces(&cmdline);
    if (*cmdline == '\0') {
        return 0;
    }
    // コマンドラインからargv[]を設定する
    int argc = 1;
    argv[0] = cmdline;
    while (*cmdline != '\0') {
        if (*cmdline == ' ') {
            *cmdline++ = '\0';
            skip_whitespaces(&cmdline);
            argv[argc] = cmdline;
            argc++;
            if (argc == argv_max - 1) {
                break;
            }
        } else {
            cmdline++;
        }
    }

    argv[argc] = NULL;
    return argc;
}

/** @ingroup shell
 * @brief argv[]からコマンドラインを作成してvmに起動を依頼する.
 * @param argc 引数の数
 * @param argv[] 引数配列
 */
static void launch_task(int argc, char **argv) {
    // argvからコマンドラインを構築する
    // 作成するコマンドラインの長さを計算する
    size_t len = 0;
    for (int i = 0; i < argc; i++) {
        len += strlen(argv[i]) + 1 /* +1は空白か'\0' */;
    }
    // コマンドライン用のメモリを割り当て、argv[]を空白を挟んでコピーする
    char *name_and_cmdline = malloc(len);
    size_t offset = 0;
    for (int i = 0; i < argc; i++) {
        strncpy2(&name_and_cmdline[offset], argv[i], len - offset);
        offset += strlen(argv[i]);
        name_and_cmdline[offset++] = ' ';
    }
    name_and_cmdline[offset - 1] = '\0';
    // メッセージを作成してipc_call()
    struct message m;
    m.type = TASK_LAUNCH_MSG;
    m.task_launch.name_and_cmdline = name_and_cmdline;
    error_t err = ipc_call(VM_TASK, &m);
    // コマンドライン用のメモリを開放して終了
    //  vm/main.cの処理関数内でもfree()しているがfree()は引数がNULLならそのままreturnするのでOK
    free(name_and_cmdline);
    if (err != OK) {
        WARN("failed to launch '%s': %s", argv[0], err2str(err));
    }
}

static void run(char *cmdline) {
    // コマンドライン文字列をパース
    int argv_max = 8;
    char **argv = malloc(sizeof(char *) * argv_max);
    int argc = parse(cmdline, argv, argv_max);
    if (!argc) {
        return;
    }
    // シェル内部コマンドの場合はコマンド関数を実行する
    for (int i = 0; commands[i].name != NULL; i++) {
        if (!strcmp(commands[i].name, argv[0])) {
            commands[i].run(argc, argv);
            return;
        }
    }

    // 内部コマンドでなければvmにタスクの起動を依頼する
    launch_task(argc, argv);
}

static void prompt(const char *input) {
    PRINTF("\e[1K\rshell> %s", input);
    printf_flush();
}

static void read_input(void) {
    char buf[256];
    static char cmdline[512];
    static int cmdline_index = 0;

    OOPS_OK(sys_console_read(buf, sizeof(buf)));
    for (size_t i = 0; i < sizeof(buf) && buf[i] != '\0'; i++) {
        PRINTF("%c", buf[i]);
        switch (buf[i]) {
            case '\n':
            case '\r':
                cmdline[cmdline_index] = '\0';
                printf("\e[0m");
                printf_flush();
                run(cmdline);
                cmdline_index = 0;
                prompt("");
                break;
            // Escape sequence.
            case 0x1b:
                // TODO: Parse and handle the escape sequence.
                break;
            // Backspace.
            case 0x7f:
                if (cmdline_index > 0) {
                    printf("\b");
                    cmdline_index--;
                    cmdline[cmdline_index] = '\0';
                    prompt(cmdline);
                }
                break;
            default:
                if (cmdline_index == sizeof(cmdline) - 1) {
                    WARN("too long command");
                    cmdline_index = 0;
                    prompt("");
                } else {
                    cmdline[cmdline_index] = buf[i];
                    cmdline_index++;
                }
        }
    }
    printf_flush();
}

void main(void) {
    TRACE("starting...");

    ASSERT_OK(irq_acquire(CONSOLE_IRQ));

    // The mainloop: receive and handle messages.
    prompt("");
    while (true) {
        struct message m;
        error_t err = ipc_recv(IPC_ANY, &m);
        ASSERT_OK(err);

        switch (m.type) {
            case NOTIFICATIONS_MSG:
                if (m.notifications.data & NOTIFY_IRQ) {
                    read_input();
                }
                break;
            default:
                discard_unknown_message(&m);
        }
    }
}
