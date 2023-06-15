PHDRS {
    text PT_LOAD;
    data PT_LOAD;
    bss PT_LOAD;
}

SECTIONS {
    . = 0x00100000;

    .text :{
        *(.bootelf_header);
        *(.text*)
    } :text

    /* raddiskではrodata領域にディスクが作成される */
    .rodata :{
        *(.rodata*)
    } :text

    /* :dataプログラムヘッダの先頭 */
    .data : ALIGN(0x1000) {
        *(.data*)

        . = ALIGN(16);
        __stack = .;
        . += 0x4000;
        __stack_end = .;
    } :data

    /* この'.'の設定は:bssプログラムヘッダの先頭を設定     */
    /* ramdiskでrodataに大きな領域が作成され、デフォルト値 */
    /* では領域が交差してエラー。                          */

    __cmdline = 0x02fff000;

    . = 0x00800000;
    __zeroed_pages = .;

    .bss : ALIGN(0x1000) {
        __heap = .;
        . += 0x20000;
        __heap_end = .;

        __bss = .;
        *(.bss*);
        __bss_end = .;
    } :bss

    . = 0x03000000;
    __zeroed_pages_end = .;
    __straight_mapping = .;

    __free_vaddr = .;
    . = 0x0a000000;
    __free_vaddr_end = .;

    /DISCARD/ :{
        *(.ARM.exidx)
    }
}
