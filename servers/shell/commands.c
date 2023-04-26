/** @file commands.c */
#include "commands.h"
#include "fs.h"
#include "http.h"
#include <resea/cmdline.h>
#include <resea/ipc.h>
#include <resea/malloc.h>
#include <resea/printf.h>
#include <resea/syscall.h>
#include <string.h>

/** @ingroup shell
 * @brief fs_readコマンドの実行関数
 * @param argc 引数の数
 * @param argv 引数配列のポインタ
 */
static void fs_read_command(int argc, char **argv) {
    if (argc < 2) {
        WARN("fs_read: too few arguments");
        return;
    }

    fs_read(argv[1]);
}

/** @ingroup shell
 * @brief fs_writeコマンドの実行関数
 * @param argc 引数の数
 * @param argv 引数配列のポインタ
 */
static void fs_write_command(int argc, char **argv) {
    if (argc < 2) {
        WARN("fs_write: too few arguments");
        return;
    }

    fs_write(argv[1], (uint8_t *) argv[2], strlen(argv[2]));
}

/** @ingroup shell
 * @brief http_getコマンドの実行関数
 * @param argc 引数の数
 * @param argv 引数配列のポインタ
 */
static void http_get_command(int argc, char **argv) {
    if (argc < 2) {
        WARN("http_get: too few arguments");
        return;
    }

    http_get(argv[1]);
}

/** @ingroup shell
 * @brief psコマンドの実行関数
 * @param argc 引数の数
 * @param argv 引数配列のポインタ
 */
static void ps_command(__unused int argc, __unused char **argv) {
    kdebug("ps");
}

/** @ingroup shell
 * @brief qコマンドの実行関数
 * @param argc 引数の数
 * @param argv 引数配列のポインタ
 */
static void quit_command(__unused int argc, __unused char **argv) {
    kdebug("q");
}

/** @ingroup shell
 * @brief helpコマンドの実行関数
 * @param argc 引数の数
 * @param argv 引数配列のポインタ
 */
static void help_command(__unused int argc, __unused char **argv) {
    INFO("help              -  Print this message.");
    INFO("<task> cmdline... -  Launch a task.");
    INFO("ps                -  List tasks.");
    INFO("q                 -  Halt the computer.");
    INFO("fs-read path      -  Read a file.");
    INFO("fs-write path str -  Write a string into a file.");
    INFO("http-get url      -  Peform a HTTP GET request.");
}

/** @ingroup shell
 * @var commans
 * @brief シェル内部コマンドの配列
 */
struct command commands[] = {
    {.name = "help", .run = help_command},
    {.name = "ps", .run = ps_command},
    {.name = "q", .run = quit_command},
    {.name = "fs-read", .run = fs_read_command},
    {.name = "fs-write", .run = fs_write_command},
    {.name = "http-get", .run = http_get_command},
    {.name = NULL, .run = NULL},
};
