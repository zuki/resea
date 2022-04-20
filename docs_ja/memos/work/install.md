# Resea インストール

```
$ git clone -o upstream https://github.com/nuta/resea.git
Cloning into 'resea'...
remote: Enumerating objects: 15400, done.
remote: Counting objects: 100% (829/829), done.
remote: Compressing objects: 100% (471/471), done.
remote: Total 15400 (delta 485), reused 611 (delta 313), pack-reused 14571
Receiving objects: 100% (15400/15400), 3.60 MiB | 8.44 MiB/s, done.
Resolving deltas: 100% (10174/10174), done.
vagrant@ubuntu-bionic:~$ ls
backup        dyn_back  proc.c      test                  xv6-armv8-xuyipei
bash_profile  lfspkgs   Public      Videos                xv6-fudan
Desktop       LLD       resea       work                  xv6-OS-for-arm-v8
develop       mikan     source      xv6-armv8
Documents     Music     strace.log  xv6-armv8-hakula
Downloads     Pictures  Templates   xv6-armv8-hatsunehan
$ cd resea
$ make menuconfig
       GEN  build/Kconfig.autogen
Using default symbol values (no '.config')
Configuration saved to '.config'
$ make
       GEN  build/include/config.h
       GEN  build/include/idl.h
  DOWNLOAD  https://download.sourceforge.net/sourceforge/libpng/zlib-1.2.11.tar.xz
  DOWNLOAD  https://cairographics.org/snapshots/cairo-1.17.4.tar.xz
  DOWNLOAD  https://cairographics.org/releases/pixman-0.40.0.tar.gz
  DOWNLOAD  https://download.sourceforge.net/sourceforge/libpng/libpng-1.6.37.tar.xz
  DOWNLOAD  https://download.savannah.gnu.org/releases/freetype/freetype-2.11.0.tar.xz
  DOWNLOAD  https://sourceware.org/pub/newlib/newlib-4.1.0.tar.gz
       GEN  build/libs/libc/targ-include
        CC  kernel/boot.c
        CC  kernel/task.c
        CC  kernel/ipc.c
        CC  kernel/syscall.c
        CC  kernel/printk.c
        CC  kernel/kdebug.c
        CC  kernel/arch/x64/task.c
        CC  kernel/arch/x64/vm.c
        CC  kernel/arch/x64/serial.c
       GEN  build/vm/__name__.c
        CC  build/vm/__name__.c
        CC  servers/vm/main.c
        CC  servers/vm/task.c
        CC  servers/vm/ool.c
        CC  servers/vm/page_alloc.c
        CC  servers/vm/page_fault.c
        CC  servers/vm/bootfs.c
       GEN  build/datetime/__name__.c
        CC  build/datetime/__name__.c
        CC  servers/experimental/datetime/main.c
        CC  libs/common/string.c
        CC  libs/common/vprintf.c
        CC  libs/common/ubsan.c
        CC  libs/common/bitmap.c
        CC  libs/common/arch/x64/memcpy.S
        LD  build/libs/common.lib.o
        AR  build/libs/common.a
        CC  libs/resea/init.c
        CC  libs/resea/printf.c
        CC  libs/resea/malloc.c
        CC  libs/resea/handle.c
        CC  libs/resea/async.c
        CC  libs/resea/task.c
        CC  libs/resea/syscall.c
        CC  libs/resea/ipc.c
        CC  libs/resea/timer.c
        CC  libs/resea/cmdline.c
        CC  libs/resea/datetime.c
        CC  libs/resea/arch/x64/start.S
        LD  build/libs/resea.lib.o
        AR  build/libs/resea.a
        CC  libs/unittest/dummy.c
        LD  build/libs/unittest.lib.o
        AR  build/libs/unittest.a
        LD  build/datetime.debug.elf
   SYMBOLS  build/datetime.debug.elf
     STRIP  build/datetime.elf
       GEN  build/dm/__name__.c
        CC  build/dm/__name__.c
        CC  servers/dm/main.c
        CC  servers/dm/pci.c
        CC  libs/driver/dma.c
        CC  libs/driver/io.c
        CC  libs/driver/irq.c
        LD  build/libs/driver.lib.o
        AR  build/libs/driver.a
        LD  build/dm.debug.elf
   SYMBOLS  build/dm.debug.elf
     STRIP  build/dm.elf
       GEN  build/e1000/__name__.c
        CC  build/e1000/__name__.c
        CC  servers/drivers/net/e1000/main.c
        CC  servers/drivers/net/e1000/e1000.c
        LD  build/e1000.debug.elf
   SYMBOLS  build/e1000.debug.elf
     STRIP  build/e1000.elf
       GEN  build/fatfs/__name__.c
        CC  build/fatfs/__name__.c
        CC  servers/fs/fatfs/main.c
        CC  servers/fs/fatfs/fat.c
        LD  build/fatfs.debug.elf
   SYMBOLS  build/fatfs.debug.elf
     STRIP  build/fatfs.elf
       GEN  build/hello/__name__.c
        CC  build/hello/__name__.c
        CC  servers/apps/hello/main.c
        LD  build/hello.debug.elf
   SYMBOLS  build/hello.debug.elf
     STRIP  build/hello.elf
       GEN  build/ide/__name__.c
        CC  build/ide/__name__.c
        CC  servers/drivers/blk/ide/main.c
       GEN  build/ide.img
4098+0 records in
4098+0 records out
4196352 bytes (4.2 MB, 4.0 MiB) copied, 0.00950865 s, 441 MB/s
       GEN  build/servers/drivers/blk/ide/disk.o
        LD  build/ide.debug.elf
   SYMBOLS  build/ide.debug.elf
     STRIP  build/ide.elf
       GEN  build/ramdisk/__name__.c
        CC  build/ramdisk/__name__.c
        CC  servers/drivers/blk/ramdisk/main.c
       GEN  build/ramdisk.img
4098+0 records in
4098+0 records out
4196352 bytes (4.2 MB, 4.0 MiB) copied, 0.00961032 s, 437 MB/s
       GEN  build/servers/drivers/blk/ramdisk/disk.o
        LD  build/ramdisk.debug.elf
   SYMBOLS  build/ramdisk.debug.elf
     STRIP  build/ramdisk.elf
       GEN  build/random/__name__.c
        CC  build/random/__name__.c
        CC  servers/random/main.c
        LD  build/random.debug.elf
   SYMBOLS  build/random.debug.elf
     STRIP  build/random.elf
       GEN  build/rtc/__name__.c
        CC  build/rtc/__name__.c
        CC  servers/drivers/rtc/rtc/main.c
        LD  build/rtc.debug.elf
   SYMBOLS  build/rtc.debug.elf
     STRIP  build/rtc.elf
       GEN  build/shell/__name__.c
        CC  build/shell/__name__.c
        CC  servers/shell/main.c
        CC  servers/shell/commands.c
        CC  servers/shell/http.c
        CC  servers/shell/fs.c
        LD  build/shell.debug.elf
   SYMBOLS  build/shell.debug.elf
     STRIP  build/shell.elf
       GEN  build/tcpip/__name__.c
        CC  build/tcpip/__name__.c
        CC  servers/tcpip/main.c
        CC  servers/tcpip/arp.c
        CC  servers/tcpip/device.c
        CC  servers/tcpip/dhcp.c
        CC  servers/tcpip/ethernet.c
        CC  servers/tcpip/ipv4.c
        CC  servers/tcpip/mbuf.c
        CC  servers/tcpip/tcp.c
        CC  servers/tcpip/udp.c
        CC  servers/tcpip/stats.c
        CC  servers/tcpip/icmp.c
        CC  servers/tcpip/dns.c
        LD  build/tcpip.debug.elf
   SYMBOLS  build/tcpip.debug.elf
     STRIP  build/tcpip.elf
       GEN  build/virtio_gpu/__name__.c
        CC  build/virtio_gpu/__name__.c
        CC  servers/drivers/gpu/virtio_gpu/main.c
        CC  servers/drivers/gpu/virtio_gpu/cairo_demo.c
servers/drivers/gpu/virtio_gpu/cairo_demo.c:159:13: warning: format specifies type 'int' but the argument has type 'long' [-Wformat]
            ((long)
            ^~~~~~~
servers/drivers/gpu/virtio_gpu/cairo_demo.c:180:17: warning: initializing 'uint8_t *' (aka 'unsigned char *') with an expression of type 'char []' converts between pointers to integer types with different sign [-Wpointer-sign]
                _binary_servers_drivers_gpu_virtio_gpu_wallpaper_png_start,
                ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
2 warnings generated.
  DOWNLOAD  roboto-android.zip
curl: (23) Failed writing body (0 != 1369)
make: *** [servers/drivers/gpu/virtio_gpu/build.mk:9: build/servers/drivers/gpu/virtio_gpu/roboto_font.o] Error 23
```

## servers/dirvers/gpu/virtio/gpu/build.mkを修正

```
$ vi servers/dirvers/gpu/virtio_gpu/build.mk

:%s/$(build_dir)/$(BUILD_DIR)/servers/dirvers/gpu/virtio_gpu/
:w
:q

$ git diff
diff --git a/servers/drivers/gpu/virtio_gpu/build.mk b/servers/drivers/gpu/virtio_gpu/build.mk
index 1eaf81e..0e7aa71 100644
--- a/servers/drivers/gpu/virtio_gpu/build.mk
+++ b/servers/drivers/gpu/virtio_gpu/build.mk
@@ -3,14 +3,14 @@ description := A virtio-gpu GPU driver
 objs-y := main.o  cairo_demo.o roboto_font.o wallpaper.o
 libs-y := virtio driver third_party/cairo

-$(build_dir)/cairo_demo.o: CFLAGS += $(LIBC_CFLAGS) $(CAIRO_CFLAGS)
-$(build_dir)/roboto_font.o:
+$(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/cairo_demo.o: CFLAGS += $(LIBC_CFLAGS) $(CAIRO_CFLAGS)
+$(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/roboto_font.o:
        $(PROGRESS) DOWNLOAD roboto-android.zip
-       curl -sSL -o $(build_dir)/roboto.zip https://github.com/googlefonts/roboto/releases/download/v2.138/roboto-android.zip
-       cd $(build_dir) && unzip -nq roboto.zip
+       curl -sSL -o $(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/roboto.zip https://github.com/googlefonts/roboto/releases/download/v2.138/roboto-android.zip
+       cd $(BUILD_DIR)/servers/drivers/gpu/virtio_gpu && unzip -nq roboto.zip
        $(PROGRESS) OBJCOPY $@
-       $(OBJCOPY) -Ibinary -Oelf64-x86-64 $(build_dir)/Roboto-Regular.ttf $@
+       $(OBJCOPY) -Ibinary -Oelf64-x86-64 $(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/Roboto-Regular.ttf $@

-$(build_dir)/wallpaper.o: servers/drivers/gpu/virtio_gpu/wallpaper.png
+$(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/wallpaper.o: servers/drivers/gpu/virtio_gpu/wallpaper.png
        $(PROGRESS) OBJCOPY $@
        $(OBJCOPY) -Ibinary -Oelf64-x86-64 $< $@
```

```
$ make
       GEN  build/Kconfig.autogen
       GEN  build/include/config.h
  DOWNLOAD  roboto-android.zip
   OBJCOPY  build/servers/drivers/gpu/virtio_gpu/roboto_font.o
   OBJCOPY  build/servers/drivers/gpu/virtio_gpu/wallpaper.o
        CC  libs/virtio/virtio.c
        CC  libs/virtio/virtio_modern.c
        CC  libs/virtio/virtio_legacy.c
        LD  build/libs/virtio.lib.o
        AR  build/libs/virtio.a
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-analysis-surface.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-arc.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-array.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-atomic.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-base64-stream.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-base85-stream.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-bentley-ottmann-rectangular.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-bentley-ottmann-rectilinear.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-bentley-ottmann.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-botor-scan-converter.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-boxes-intersect.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-boxes.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-cache.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-clip-boxes.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-clip-polygon.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-clip-region.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-clip-surface.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-clip-tor-scan-converter.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-clip.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-color.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-composite-rectangles.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-compositor.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-contour.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-damage.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-debug.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-default-context.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-device.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-error.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-fallback-compositor.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-fixed.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-font-face-twin-data.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-font-face-twin.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-font-face.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-font-options.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-freed-pool.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-freelist.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-gstate.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-hash.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-hull.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-image-compositor.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-image-info.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-image-source.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-image-surface.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-line.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-lzw.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-mask-compositor.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-matrix.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-mempool.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-mesh-pattern-rasterizer.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-misc.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-mono-scan-converter.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-mutex.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-no-compositor.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-observer.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-output-stream.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-paginated-surface.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path-bounds.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path-fill.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path-fixed.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path-in-fill.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path-stroke-boxes.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path-stroke-polygon.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path-stroke-traps.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path-stroke-tristrip.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path-stroke.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-path.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-pattern.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-pen.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-polygon-intersect.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-polygon-reduce.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-polygon.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-raster-source-pattern.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-recording-surface.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-rectangle.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-rectangular-scan-converter.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-region.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-rtree.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-scaled-font.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-shape-mask-compositor.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-slope.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-spans-compositor.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-spans.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-spline.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-stroke-dash.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-stroke-style.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-surface-clipper.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-surface-fallback.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-surface-observer.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-surface-offset.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-surface-snapshot.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-surface-subsurface.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-surface-wrapper.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-surface.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-tor-scan-converter.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-tor22-scan-converter.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-toy-font-face.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-traps-compositor.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-traps.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-tristrip.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-unicode.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-user-font.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-wideint.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-time.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-ft-font.c
        CC  libs/third_party/cairo/cairo-1.17.4/src/cairo-png.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-access.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-access-accessors.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-bits-image.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-combine32.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-combine-float.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-conical-gradient.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-filter.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-x86.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-mips.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-arm.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-ppc.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-edge.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-edge-accessors.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-fast-path.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-glyph.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-general.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-gradient-walker.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-image.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-implementation.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-linear-gradient.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-matrix.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-noop.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-radial-gradient.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-region16.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-region32.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-solid-fill.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-timer.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-trap.c
        CC  libs/third_party/cairo/pixman-0.40.0/pixman/pixman-utils.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngpread.c
        CC  libs/third_party/cairo/libpng-1.6.37/png.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngerror.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngmem.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngget.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngrtran.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngwrite.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngtrans.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngwio.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngset.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngwtran.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngrio.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngread.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngrutil.c
        CC  libs/third_party/cairo/libpng-1.6.37/pngwutil.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftcid.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftdebug.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftbbox.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftinit.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftmm.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftgasp.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftbase.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftbitmap.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftotval.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftbdf.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/fttype1.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftsynth.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftfstype.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftpatent.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftpfr.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftgxval.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/type1/type1.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/truetype/truetype.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftglyph.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/cid/type1cid.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftwinfnt.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/base/ftstroke.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/cff/cff.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/type42/type42.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/pfr/pfr.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/pcf/pcf.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/winfonts/winfnt.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/pshinter/pshinter.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/bdf/bdf.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/raster/raster.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/sfnt/sfnt.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/smooth/smooth.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/autofit/autofit.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/cache/ftcache.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/sdf/sdf.c
In file included from libs/third_party/cairo/freetype-2.11.0/src/sdf/sdf.c:26:
libs/third_party/cairo/freetype-2.11.0/src/sdf/ftsdf.c:933:5: warning: variable 'cbox' is used uninitialized whenever switch default is taken [-Wsometimes-uninitialized]
    default:
    ^~~~~~~
libs/third_party/cairo/freetype-2.11.0/src/sdf/ftsdf.c:937:12: note: uninitialized use occurs here
    return cbox;
           ^~~~
libs/third_party/cairo/freetype-2.11.0/src/sdf/ftsdf.c:849:5: note: variable 'cbox' is declared here
    FT_CBox  cbox;
    ^
1 warning generated.
        CC  libs/third_party/cairo/freetype-2.11.0/src/gzip/ftgzip.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/lzw/ftlzw.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/bzip2/ftbzip2.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/psaux/psaux.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/psnames/psnames.c
        CC  libs/third_party/cairo/freetype-2.11.0/src/dlg/dlgwrap.c
libs/third_party/cairo/freetype-2.11.0/src/dlg/dlgwrap.c:21:9: warning: '_XOPEN_SOURCE' macro redefined [-Wmacro-redefined]
#define _XOPEN_SOURCE
        ^
<command line>:22:9: note: previous definition is here
#define _XOPEN_SOURCE 700
        ^
libs/third_party/cairo/freetype-2.11.0/src/dlg/dlgwrap.c:22:9: warning: '_POSIX_C_SOURCE' macro redefined [-Wmacro-redefined]
#define _POSIX_C_SOURCE 200809L
        ^
<command line>:23:9: note: previous definition is here
#define _POSIX_C_SOURCE 200809
        ^
2 warnings generated.
        CC  libs/third_party/cairo/freetype-2.11.0/builds/unix/ftsystem.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/dummy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_add.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_add_sep.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_append.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_count.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_create.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_create_sep.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_delete.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_extract.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_insert.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_next.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_replace.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/argz_stringify.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/buf_findstr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/envz_entry.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/envz_get.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/envz_add.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/envz_remove.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/envz_merge.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/argz/envz_strip.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/__adjust.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/__atexit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/__call_atexit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/__exp10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/__ten_mu.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/_Exit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/abort.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/abs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/aligned_alloc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/assert.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/atexit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/atof.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/atoff.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/atoi.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/atol.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/calloc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/div.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/dtoa.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/dtoastub.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/environ.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/envlock.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/eprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/exit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/gdtoa-gethex.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/gdtoa-hexnan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/getenv.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/getenv_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/imaxabs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/imaxdiv.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/itoa.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/labs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/ldiv.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/ldtoa.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/malloc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mblen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mblen_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mbstowcs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mbstowcs_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mbtowc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mbtowc_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mlock.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mprec.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mstats.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/on_exit_args.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/quick_exit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/rand.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/rand_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/random.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/realloc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/reallocarray.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/reallocf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/sb_charsets.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtod.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtoimax.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtol.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtoul.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtoumax.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/utoa.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstod.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstoimax.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstol.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstoul.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstoumax.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstombs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstombs_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wctomb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wctomb_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtodg.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtold.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtorx.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstold.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/arc4random.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/arc4random_uniform.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/cxa_atexit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/cxa_finalize.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/drand48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/ecvtbuf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/efgcvt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/erand48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/jrand48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/lcong48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/lrand48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mrand48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/msize.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mtrim.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/nrand48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/rand48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/seed48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/srand48.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtoll.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtoll_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtoull.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/strtoull_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstoll.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstoll_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstoull.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcstoull_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/atoll.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/llabs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/lldiv.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/a64l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/btowc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/getopt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/getsubopt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/l64a.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/malign.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mbrlen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mbrtowc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mbsinit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mbsnrtowcs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mbsrtowcs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/on_exit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/valloc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcrtomb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcsnrtombs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wcsrtombs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/wctob.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/putenv.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/putenv_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/setenv.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/setenv_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/rpmatch.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/system.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdlib/mallocr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/ctype_.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isalnum.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isalpha.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iscntrl.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isdigit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/islower.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isupper.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isprint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/ispunct.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isspace.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isxdigit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/tolower.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/toupper.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/categories.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isalnum_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isalpha_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isascii.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isascii_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isblank.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isblank_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iscntrl_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isdigit_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/islower_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isupper_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isprint_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/ispunct_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isspace_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswalnum.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswalnum_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswalpha.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswalpha_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswblank.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswblank_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswcntrl.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswcntrl_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswctype.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswctype_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswdigit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswdigit_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswgraph.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswgraph_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswlower.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswlower_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswprint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswprint_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswpunct.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswpunct_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswspace.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswspace_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswupper.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswupper_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswxdigit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/iswxdigit_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/isxdigit_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/jp2uc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/toascii.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/toascii_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/tolower_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/toupper_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/towctrans.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/towctrans_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/towlower.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/towlower_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/towupper.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/towupper_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/wctrans.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/wctrans_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/wctype.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ctype/wctype_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/bsearch.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/ndbm.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/qsort.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/hash.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/hash_bigkey.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/hash_buf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/hash_func.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/hash_log2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/hash_page.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/hcreate.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/hcreate_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/tdelete.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/tdestroy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/tfind.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/tsearch.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/twalk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/bsd_qsort_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/search/qsort_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fiprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fiscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/iprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/iscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/siprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/siscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/sniprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vdiprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfwprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/viprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/viscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vsiprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vsiscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vsniprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/clearerr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fclose.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fdopen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/feof.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/ferror.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fflush.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fgetc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fgetpos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fgets.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fileno.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/findfp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/flags.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fopen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fputc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fputs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fread.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/freopen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fseek.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fsetpos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/ftell.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fvwrite.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fwalk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fwrite.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getchar.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getc_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getchar_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getdelim.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getline.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/gets.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/makebuf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/perror.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/printf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/putc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/putchar.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/putc_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/putchar_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/puts.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/refill.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/remove.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/rename.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/rewind.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/rget.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/scanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/sccl.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/setbuf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/setbuffer.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/setlinebuf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/setvbuf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/snprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/sprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/sscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/stdio.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/tmpfile.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/tmpnam.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/ungetc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vdprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vsnprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vsprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vsscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/wbuf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/wsetup.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/asiprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vasiprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/asprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fcloseall.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fseeko.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/ftello.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getw.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/mktemp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/putw.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vasprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/asniprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/diprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vasniprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/asnprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/clearerr_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/dprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/feof_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/ferror_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fflush_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fgetc_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fgets_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fgetwc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fgetwc_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fgetws.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fgetws_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fileno_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fmemopen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fopencookie.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fpurge.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fputc_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fputs_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fputwc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fputwc_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fputws.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fputws_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fread_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fsetlocking.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/funopen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fwide.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fwprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fwrite_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/fwscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getwc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getwc_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getwchar.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/getwchar_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/open_memstream.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/putwc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/putwc_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/putwchar.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/putwchar_u.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/stdio_ext.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/swprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/swscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/ungetwc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vasnprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vswprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vswscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vwprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vwscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/wprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/wscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfwprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfwprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfwprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfwscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfwscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfwscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/stdio/vfwscanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/bcopy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/bzero.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/explicit_bzero.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/ffsl.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/ffsll.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/fls.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/flsl.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/flsll.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/index.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/memchr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/memcmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/memcpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/memmove.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/memset.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/rindex.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strcasecmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strcat.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strchr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strcmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strcoll.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strcpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strcspn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strdup.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strdup_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strerror.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strerror_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strlcat.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strlcpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strlen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strlwr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strncasecmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strncat.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strncmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strncpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strnlen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strnstr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strpbrk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strrchr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strsep.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strsignal.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strspn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strtok.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strtok_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strupr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strxfrm.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strstr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/swab.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/timingsafe_bcmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/timingsafe_memcmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/u_strerr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcscat.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcschr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcscmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcscoll.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcscpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcscspn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcslcat.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcslcpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcslen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsncat.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsncmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsncpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsnlen.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcspbrk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsrchr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsspn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsstr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcstok.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcswidth.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsxfrm.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcwidth.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wmemchr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wmemcmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wmemcpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wmemmove.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wmemset.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/xpg_strerror_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/bcmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/memccpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/mempcpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/stpcpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/stpncpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strndup.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strcasestr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strchrnul.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strndup_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcpcpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcpncpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsdup.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/gnu_basename.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/memmem.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/memrchr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/rawmemchr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strcasecmp_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strcoll_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strncasecmp_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strverscmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/strxfrm_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcscasecmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcscasecmp_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcscoll_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsncasecmp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsncasecmp_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wcsxfrm_l.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/string/wmempcpy.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/signal/psignal.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/signal/raise.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/signal/signal.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/asctime.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/asctime_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/clock.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/ctime.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/ctime_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/difftime.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/gettzinfo.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/gmtime.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/gmtime_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/lcltime.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/lcltime_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/mktime.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/month_lengths.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/strftime.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/strptime.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/time.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/tzcalc_limits.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/tzlock.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/tzset.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/tzset_r.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/tzvars.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/time/wcsftime.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/locale.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/localeconv.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/duplocale.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/freelocale.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/lctype.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/lmessages.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/lnumeric.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/lmonetary.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/newlocale.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/nl_langinfo.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/timelocal.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/locale/uselocale.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/closer.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/reent.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/impure.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/fcntlr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/fstatr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/getreent.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/gettimeofdayr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/isattyr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/linkr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/lseekr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/mkdirr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/openr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/readr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/renamer.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/signalr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/signgam.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/sbrkr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/statr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/timesr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/unlinkr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/writer.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/reent/execr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/errno/errno.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/misc/__dprintf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/misc/unctrl.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/misc/ffs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/misc/init.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/misc/fini.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/chk_fail.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/stack_protector.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/memcpy_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/memmove_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/mempcpy_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/memset_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/stpcpy_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/stpncpy_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/strcat_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/strcpy_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/strncat_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/strncpy_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/gets_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/snprintf_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/sprintf_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/vsnprintf_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/ssp/vsprintf_chk.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/k_standard.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/k_rem_pio2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/k_cos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/k_sin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/k_tan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_acos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_acosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_asin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_atan2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_atanh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_cosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_exp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_fmod.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_tgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_hypot.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_j0.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_j1.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_jn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/er_lgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_log.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_log10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_pow.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_rem_pio2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_remainder.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_scalb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_sinh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/e_sqrt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_acos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_acosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_asin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_atan2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_atanh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_cosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_exp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_fmod.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_gamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wr_gamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_hypot.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_j0.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_j1.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_jn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_lgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wr_lgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_log.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_log10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_pow.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_remainder.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_scalb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_sinh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_sqrt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_sincos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_drem.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_asinh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_atan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_ceil.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_cos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_erf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_fabs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_floor.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_frexp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_ldexp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_signif.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_sin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_tan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/s_tanh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_exp2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/w_tgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/kf_rem_pio2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/kf_cos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/kf_sin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/kf_tan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_acos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_acosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_asin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_atan2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_atanh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_cosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_exp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_fmod.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_tgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_hypot.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_j0.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_j1.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_jn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/erf_lgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_log.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_log10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_pow.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_rem_pio2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_remainder.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_scalb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_sinh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/ef_sqrt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_acos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_acosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_asin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_atan2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_atanh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_cosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_exp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_fmod.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_gamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wrf_gamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_hypot.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_j0.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_j1.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_jn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_lgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wrf_lgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_log.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_log10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_pow.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_remainder.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_scalb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_sinh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_sqrt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_sincos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_drem.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_asinh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_atan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_ceil.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_cos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_erf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_fabs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_floor.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_frexp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_ldexp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_signif.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_sin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_tan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/sf_tanh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_exp2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_tgamma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/wf_log2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/math/el_hypot.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_finite.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_copysign.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_modf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_scalbn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_cbrt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_exp10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_expm1.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_ilogb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_infinity.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_isinf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_isinfd.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_isnan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_isnand.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_log1p.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_nan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_nextafter.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_pow10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_rint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_logb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_log2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_fdim.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_fma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_fmax.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_fmin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_fpclassify.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_lrint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_llrint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_lround.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_llround.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_nearbyint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_remquo.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_round.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_scalbln.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_signbit.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/s_trunc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/exp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/exp2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/exp_data.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/math_err.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/log.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/log_data.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/log2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/log2_data.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/pow.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/pow_log_data.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_finite.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_copysign.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_modf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_scalbn.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_cbrt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_exp10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_expm1.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_ilogb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_infinity.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_isinf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_isinff.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_isnan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_isnanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_log1p.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_nan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_nextafter.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_pow10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_rint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_logb.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_fdim.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_fma.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_fmax.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_fmin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_fpclassify.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_lrint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_llrint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_lround.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_llround.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_nearbyint.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_remquo.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_round.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_scalbln.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_trunc.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_exp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_exp2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_exp2_data.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_log.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_log_data.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_log2.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_log2_data.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_pow_log2_data.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sf_pow.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sinf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/cosf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sincosf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/sincosf_data.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/common/math_errf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cabs.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cacos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cacosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/carg.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/casin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/casinh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/catan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/catanh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/ccos.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/ccosh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cephes_subr.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cexp.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cimag.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/clog.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/clog10.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/conj.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cpow.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cproj.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/creal.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/csin.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/csinh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/csqrt.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/ctan.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/ctanh.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cabsf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/casinf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/ccosf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cimagf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cprojf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/csqrtf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cacosf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/casinhf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/ccoshf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/clogf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/clog10f.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/crealf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/ctanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cacoshf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/catanf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cephes_subrf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/conjf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/csinf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/ctanhf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cargf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/catanhf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cexpf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/cpowf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/complex/csinhf.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libm/machine/x86_64/fenv.c
        CC  libs/third_party/libc/newlib-4.1.0/newlib/libc/machine/x86_64/setjmp.S
        LD  build/libs/third_party/libc/newlib_all.o
   OBJCOPY  build/libs/third_party/libc/newlib_all.o
        CC  libs/third_party/libc/newlib_syscalls.c
        LD  build/libs/libc.lib.o
        CC  libs/third_party/zlib/zlib-1.2.11/adler32.c
        CC  libs/third_party/zlib/zlib-1.2.11/crc32.c
        CC  libs/third_party/zlib/zlib-1.2.11/deflate.c
        CC  libs/third_party/zlib/zlib-1.2.11/infback.c
        CC  libs/third_party/zlib/zlib-1.2.11/inffast.c
        CC  libs/third_party/zlib/zlib-1.2.11/inflate.c
        CC  libs/third_party/zlib/zlib-1.2.11/inftrees.c
        CC  libs/third_party/zlib/zlib-1.2.11/trees.c
        CC  libs/third_party/zlib/zlib-1.2.11/zutil.c
        CC  libs/third_party/zlib/zlib-1.2.11/compress.c
        CC  libs/third_party/zlib/zlib-1.2.11/uncompr.c
        CC  libs/third_party/zlib/zlib-1.2.11/gzclose.c
        CC  libs/third_party/zlib/zlib-1.2.11/gzlib.c
libs/third_party/zlib/zlib-1.2.11/gzlib.c:252:9: warning: implicit declaration of function 'lseek' is invalid in C99 [-Wimplicit-function-declaration]
        LSEEK(state->fd, 0, SEEK_END);  /* so gzoffset() is correct */
        ^
libs/third_party/zlib/zlib-1.2.11/gzlib.c:14:17: note: expanded from macro 'LSEEK'
#  define LSEEK lseek
                ^
libs/third_party/zlib/zlib-1.2.11/gzlib.c:258:24: warning: implicit declaration of function 'lseek' is invalid in C99 [-Wimplicit-function-declaration]
        state->start = LSEEK(state->fd, 0, SEEK_CUR);
                       ^
libs/third_party/zlib/zlib-1.2.11/gzlib.c:14:17: note: expanded from macro 'LSEEK'
#  define LSEEK lseek
                ^
libs/third_party/zlib/zlib-1.2.11/gzlib.c:359:9: warning: implicit declaration of function 'lseek' is invalid in C99 [-Wimplicit-function-declaration]
    if (LSEEK(state->fd, state->start, SEEK_SET) == -1)
        ^
libs/third_party/zlib/zlib-1.2.11/gzlib.c:14:17: note: expanded from macro 'LSEEK'
#  define LSEEK lseek
                ^
libs/third_party/zlib/zlib-1.2.11/gzlib.c:400:15: warning: implicit declaration of function 'lseek' is invalid in C99 [-Wimplicit-function-declaration]
        ret = LSEEK(state->fd, offset - state->x.have, SEEK_CUR);
              ^
libs/third_party/zlib/zlib-1.2.11/gzlib.c:14:17: note: expanded from macro 'LSEEK'
#  define LSEEK lseek
                ^
libs/third_party/zlib/zlib-1.2.11/gzlib.c:496:14: warning: implicit declaration of function 'lseek' is invalid in C99 [-Wimplicit-function-declaration]
    offset = LSEEK(state->fd, 0, SEEK_CUR);
             ^
libs/third_party/zlib/zlib-1.2.11/gzlib.c:14:17: note: expanded from macro 'LSEEK'
#  define LSEEK lseek
                ^
5 warnings generated.
        CC  libs/third_party/zlib/zlib-1.2.11/gzread.c
libs/third_party/zlib/zlib-1.2.11/gzread.c:35:15: warning: implicit declaration of function 'read' is invalid in C99 [-Wimplicit-function-declaration]
        ret = read(state->fd, buf + *have, get);
              ^
libs/third_party/zlib/zlib-1.2.11/gzread.c:651:11: warning: implicit declaration of function 'close' is invalid in C99 [-Wimplicit-function-declaration]
    ret = close(state->fd);
          ^
2 warnings generated.
        CC  libs/third_party/zlib/zlib-1.2.11/gzwrite.c
libs/third_party/zlib/zlib-1.2.11/gzwrite.c:89:20: warning: implicit declaration of function 'write' is invalid in C99 [-Wimplicit-function-declaration]
            writ = write(state->fd, strm->next_in, put);
                   ^
libs/third_party/zlib/zlib-1.2.11/gzwrite.c:110:24: warning: implicit declaration of function 'write' is invalid in C99 [-Wimplicit-function-declaration]
                writ = write(state->fd, state->x.next, put);
                       ^
libs/third_party/zlib/zlib-1.2.11/gzwrite.c:661:9: warning: implicit declaration of function 'close' is invalid in C99 [-Wimplicit-function-declaration]
    if (close(state->fd) == -1)
        ^
3 warnings generated.
        LD  build/libs/zlib.lib.o
        LD  build/libs/cairo.lib.o
        AR  build/libs/cairo.a
        LD  build/virtio_gpu.debug.elf
   SYMBOLS  build/virtio_gpu.debug.elf
too many symbols: max=512, actual=6693 (hint: increase NUM_SYMBOLS config)
     STRIP  build/virtio_gpu.elf
       GEN  build/virtio_net/__name__.c
        CC  build/virtio_net/__name__.c
        CC  servers/drivers/net/virtio_net/main.c
        LD  build/virtio_net.debug.elf
   SYMBOLS  build/virtio_net.debug.elf
     STRIP  build/virtio_net.elf
  MKBOOTFS  build/bootfs.bin
        CC  servers/vm/bootfs_image.S
        CC  servers/vm/shm.c
        CC  libs/elf/dummy.c
        LD  build/libs/elf.lib.o
        AR  build/libs/elf.a
        LD  build/vm.debug.elf
   SYMBOLS  build/vm.debug.elf
     STRIP  build/vm.elf
        CC  kernel/arch/x64/boot.S
        CC  kernel/arch/x64/init.c
        CC  kernel/arch/x64/interrupt.c
        CC  kernel/arch/x64/trap.S
        CC  kernel/arch/x64/mp.c
        CC  kernel/arch/x64/screen.c
        CC  libs/common/string.c
        CC  libs/common/vprintf.c
        CC  libs/common/ubsan.c
        CC  libs/common/bitmap.c
        CC  libs/common/arch/x64/memcpy.S
       GEN  build/kernel/__name__.c
        CC  build/kernel/__name__.c
        LD  build/resea.elf
       GEN  build/resea.symbols
   SYMBOLS  build/resea.elf
       GEN  build/compile_commands.json
```

## 実行

```
$ make run
       GEN  build/include/config.h
       RUN  build/resea.elf
WARNING: Image format was not specified for 'build/ide.img' and probing guessed raw.
         Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
         Specify the 'raw' format explicitly to remove the restrictions.
qemu-system-x86_64: warning: TCG doesn't support requested feature: CPUID.01H:ECX.x2apic [bit 21]
qemu-system-x86_64: warning: TCG doesn't support requested feature: CPUID.01H:ECX.tsc-deadline [bit 24]
qemu-system-x86_64: warning: TCG doesn't support requested feature: CPUID.01H:ECX.avx [bit 28]
qemu-system-x86_64: warning: TCG doesn't support requested feature: CPUID.01H:ECX.f16c [bit 29]
Invalid access at addr 0xFEBB0000, size 1, region '(null)', reason: rejected
Invalid access at addr 0x0, size 1, region '(null)', reason: rejected
Invalid access at addr 0xFEBB0001, size 1, region '(null)', reason: rejected
Invalid access at addr 0x1, size 1, region '(null)', reason: rejected
Invalid access at addr 0xFEBB0002, size 1, region '(null)', reason: rejected
Invalid access at addr 0x2, size 1, region '(null)', reason: rejected
... 以下 0xFEBB00F3, 0xF3まで1バイトずつ繰り返す
Invalid access at addr 0xFEBB00F3, size 1, region '(null)', reason: rejected
Invalid access at addr 0xF3, size 1, region '(null)', reason: rejected


SeaBIOS (version 1.13.0-1ubuntu1.1)


iPXE (http://ipxe.org) 00:03.0 CA00 PCI2.10 PnP PMM+1FF8C8B0+1FECC8B0 CA00



Booting from ROM..

Booting Resea v0.9.0 (ac85038)...
[kernel] new task #1: vm (pager=(null))
[kernel] boot ELF: entry=00000000010168f8
[kernel] boot ELF: 0000000001000000 -> 0000000000116000 (14732KiB)
[kernel] boot ELF: 0000000003000000 -> 0000000000f79000 (36KiB)
[kernel] boot ELF: 0000000004000000 -> 0000000000000000 (137292KiB, zeroed)
[kernel] new task #0: (idle) (pager=(null))
[kernel] Booted CPU #0
[vm] starting...
[vm] available RAM region #3: 0000000010000000-0000000020fc5000 (271MiB)
[vm] launching datetime...
[kernel] new task #2: datetime (pager=vm)
[vm] launching dm...
[kernel] new task #3: dm (pager=vm)
[vm] launching e1000...
[kernel] new task #4: e1000 (pager=vm)
[vm] launching fatfs...
[kernel] new task #5: fatfs (pager=vm)
[vm] launching ide...
[kernel] new task #6: ide (pager=vm)
[vm] launching ramdisk...
[kernel] new task #7: ramdisk (pager=vm)
[vm] launching random...
[kernel] new task #8: random (pager=vm)
[vm] launching rtc...
[kernel] new task #9: rtc (pager=vm)
[vm] launching shell...
[kernel] new task #10: shell (pager=vm)
[vm] launching tcpip...
[kernel] new task #11: tcpip (pager=vm)
[vm] launching virtio_gpu...
[kernel] new task #12: virtio_gpu (pager=vm)
[vm] launching virtio_net...
[kernel] new task #13: virtio_net (pager=vm)
[vm] ready
[kernel] IPC: page_fault: datetime -> vm
[kernel] IPC: page_fault_reply: vm -> datetime
[kernel] IPC: page_fault: dm -> vm
[kernel] IPC: page_fault_reply: vm -> dm
[kernel] IPC: page_fault: e1000 -> vm
[kernel] IPC: page_fault_reply: vm -> e1000
[kernel] IPC: page_fault: fatfs -> vm
[kernel] IPC: page_fault_reply: vm -> fatfs
[kernel] IPC: page_fault: ide -> vm
[kernel] IPC: page_fault_reply: vm -> ide
[kernel] IPC: page_fault: ramdisk -> vm
[kernel] IPC: page_fault_reply: vm -> ramdisk
[kernel] IPC: page_fault: random -> vm
[kernel] IPC: page_fault_reply: vm -> random
[kernel] IPC: page_fault: rtc -> vm
[kernel] IPC: page_fault_reply: vm -> rtc
[kernel] IPC: page_fault: shell -> vm
[kernel] IPC: page_fault_reply: vm -> shell
[kernel] IPC: page_fault: tcpip -> vm
[kernel] IPC: page_fault_reply: vm -> tcpip
[kernel] IPC: page_fault: virtio_gpu -> vm
[kernel] IPC: page_fault_reply: vm -> virtio_gpu
[kernel] IPC: page_fault: virtio_net -> vm
[kernel] IPC: page_fault_reply: vm -> virtio_net

... 以下をはさみながら以上のエラーを繰り返し

[datetime] starting...
[dm] starting...
[fatfs] starting...
[random] starting...
[shell] starting...
[e1000] starting...
[virtio_net] starting...
shell>
[ramdisk] ready
[rtc] ready
[dm] ready
[ide] ready
[datetime] ready
[fatfs] Files ---------------------------------------------
[kernel] enabled IRQ: task=e1000, vector=11
[fatfs] /HELLO   TXT
[fatfs] ---------------------------------------------------
[virtio_net] libs/virtio/virtio_legacy.c:267: unexpected error (Not Found)
[virtio_net] WARN: Backtrace:
[fatfs] ready
[virtio_net] WARN:     #0: 000000000101513b virtio_legacy_find_device()+0x2db
[virtio_net] WARN:     #1: 000000000100082f main()+0x5f
[virtio_net] WARN:     #2: 0000000001004d17 resea_init()+0x57
[virtio_net] WARN:     #3: 000000000100cfeb halt()+0x0
[kernel] WARN: Exception #13

[kernel] RIP = 000000000100cfeb CS  = 000000000000002b  RFL = 0000000000000206
[kernel] SS  = 0000000000000023 RSP = 00000000041ffc48  RBP = 00000000041ffd80
[kernel] RAX = 000000000100cfeb RBX = 0000000001004f30  RCX = 00000000010095ba
[kernel] RDX = 0000000000000042 RSI = 0000000001017980  RDI = 0000000000000006
[kernel] R8  = 0000000001017990 R9  = 0000000000000000  R10 = 0000000000000000
[kernel] R11 = 0000000000000246 R12 = 0000000000000001  R13 = 0000000000001001
[kernel] R14 = 0000000001021627 R15 = 000000000c200000  ERR = 0000000000000032

[vm] WARN: virtio_net: exception occurred, killing the task...
kernel] destroying virtio_net...

[e1000] initialized the device
[e1000] MAC address = 52:54:00:12:34:56

[tcpip] starting...
[tcpip] ready
[tcpip] registered new net device 'net1'
[e1000] ready

virtio_gpu] WARN:     #0: 000000000101565b virtio_legacy_find_device()+0x2db
[kernel] IPC: page_fault: virtio_gpu -> vm
[kernel] IPC: page_fault_reply: vm -> virtio_gpu
[virtio_gpu] WARN:     #1: 00000000010002e6 main()+0x36
[virtio_gpu] WARN:     #2: 0000000001005237 resea_init()+0x57
[virtio_gpu] WARN:     #3: 000000000100d50b halt()+0x0
[kernel] WARN: Exception #13

[kernel] RIP = 000000000100d50b CS  = 000000000000002b  RFL = 0000000000000206
[kernel] SS  = 0000000000000023 RSP = 00000000041ffd08  RBP = 00000000041ffe40
[kernel] RAX = 000000000100d50b RBX = 0000000001005450  RCX = 0000000001009ada
[kernel] RDX = 0000000000000042 RSI = 00000000012c1a70  RDI = 0000000000000006
[kernel] R8  = 00000000012c1a80 R9  = 0000000000000000  R10 = 0000000000000000
[kernel] R11 = 0000000000000246 R12 = 0000000000000010  R13 = 0000000000001010
[kernel] R14 = 000000000130eecc R15 = 000000000c200008  ERR = 0000000000000032
[kernel] IPC: exception: virtio_gpu -> vm
[vm] WARN: virtio_gpu: exception occurred, killing the task...
[kernel] destroying virtio_gpu...
```

## 設定変更

- CONFIG_BUILD_RELEASE=y
- CONFIG_VIRTIO_GPU_SERVER をオフ
- CONFIG_VIRTIO_NET_SERVE をオフ
- CONFIG_TRACE_IPC をオフ


```
$ make run
       RUN  build/resea.elf
WARNING: Image format was not specified for 'build/ide.img' and probing guessed
         Automatically detecting the format is dangerous for raw images, write o
         Specify the 'raw' format explicitly to remove the restrictions.
qemu-system-x86_64: warning: TCG doesn't support requested feature: CPUID.01H:EC
qemu-system-x86_64: warning: TCG doesn't support requested feature: CPUID.01H:EC
qemu-system-x86_64: warning: TCG doesn't support requested feature: CPUID.01H:EC
qemu-system-x86_64: warning: TCG doesn't support requested feature: CPUID.01H:EC
Invalid access at addr 0xFEBB0000, size 1, region '(null)', reason: rejected
Invalid access at addr 0x0, size 1, region '(null)', reason: rejected
...

SeaBIOS (version 1.13.0-1ubuntu1.1)


iPXE (http://ipxe.org) 00:03.0 CA00 PCI2.10 PnP PMM+1FF8C8B0+1FECC8B0 CA00



Booting from ROM..

Booting Resea v0.9.0 (ac85038)...
[kernel] Booted CPU #0
[vm] ready
shell>
[random] ready
[dm] ready
[datetime] ready
[fatfs] Files ---------------------------------------------
[fatfs] /HELLO   TXT
[fatfs] ---------------------------------------------------
[e1000] initialized the device
[e1000] MAC address = 52:54:00:12:34:56
[tcpip] ready
[tcpip] registered new net device 'net1'
[e1000] ready
[tcpip] WARN: retrying DHCP discover...
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
ls
[shell] WARN: failed to launch 'ls': Not Found
shell> hello
shell> [hello] Hello, World!
[vm] hello: terminated its execution

shell> test
shell> [test] starting integrated tests...
[vm] WARN: test still waiting for a missing service 'shm_test_server', did you o
shell> shm_test_server
[shell] WARN: failed to launch 'shm_test_server': Not Found
shell> test_server
[shell] WARN: failed to launch 'test_server': Not Found
shell> shm_test
shell> [test] Passed all tests!				// Reseaが終了
vagrant@ubuntu-bionic:~/resea$
```

## arm68でmake

```
$ make clean
$ make
...
       GEN  build/servers/drivers/blk/ide/disk.o
build/servers/drivers/blk/ide/disk.S:1:15: error: invalid alignment value
.data; .align 4096; .global __image, __image_end; __image: .incbin "build/ide.im
              ^
make: *** [servers/drivers/blk/ide/build.mk:13: build/servers/drivers/blk/ide/di

$ cat disk.S
.data; .align 4096; .global __image, __image_end; __image: .incbin "build/ide.img"; __image_end:

$ vi servers/drivers/blk/ide/build.mk
$ vi servers/drivers/blk/ramdisk/build.mk
s/align/baling/

$ make
...
       GEN  build/ide.img
4098+0 records in
4098+0 records out
4196352 bytes (4.2 MB, 4.0 MiB) copied, 0.00986078 s, 426 MB/s
       GEN  build/servers/drivers/blk/ide/disk.o
        LD  build/ide.debug.elf
ld.lld: error: section .data virtual address range overlaps with .bss
>>> .data range is [0x10E000, 0x5127FF]
>>> .bss range is [0x300000, 0x3203A7]

ld.lld: error: section .data load address range overlaps with .bss
>>> .data range is [0x10E000, 0x5127FF]
>>> .bss range is [0x300000, 0x3203A7]
make: *** [Makefile:399: build/ide.debug.elf] Error 1

$ vi libs/resea/arch/arm64/user.ld
s/. = 0x00300000/. = 0x03000000/

$ make
       GEN  build/Kconfig.autogen
       GEN  build/include/config.h
       GEN  build/servers/drivers/blk/ramdisk/disk.o
        LD  build/ramdisk.debug.elf
   SYMBOLS  build/ramdisk.debug.elf
     STRIP  build/ramdisk.elf
       GEN  build/random/__name__.c
        CC  build/random/__name__.c
        CC  servers/random/main.c
        LD  build/random.debug.elf
   SYMBOLS  build/random.debug.elf
     STRIP  build/random.elf
       GEN  build/rtc/__name__.c
        CC  build/rtc/__name__.c
        CC  servers/drivers/rtc/rtc/main.c
        LD  build/rtc.debug.elf
   SYMBOLS  build/rtc.debug.elf
     STRIP  build/rtc.elf
       GEN  build/shell/__name__.c
        CC  build/shell/__name__.c
        CC  servers/shell/main.c
servers/shell/main.c:142:27: error: use of undeclared identifier 'CONSOLE_IRQ'
    ASSERT_OK(irq_acquire(CONSOLE_IRQ));
                          ^
servers/shell/main.c:142:27: error: use of undeclared identifier 'CONSOLE_IRQ'
2 errors generated.
make: *** [Makefile:278: build/servers/shell/main.o] Error 1

$ vi servers/shell/main.c
+#ifdef ARCH_X64
     ASSERT_OK(irq_acquire(CONSOLE_IRQ));
+#endif

$ make
...
   SYMBOLS  build/resea.elf
       GEN  build/compile_commands.json

$ make run
       GEN  build/include/config.h
   OBJCOPY  build/kernel8.img
       RUN  build/kernel8.img
WARNING: Image format was not specified for 'build/ide.img' and probing guessed raw.
         Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
         Specify the 'raw' format explicitly to remove the restrictions.
qemu-system-aarch64: -drive if=ide,file=build/ide.img: machine type does not support if=ide,bus=0,unit=0
make: *** [kernel/arch/arm64/build.mk:18: run] Error 1
```

qemu-system-aarch64はideをサポートしていない。なにか別の方法を考える。

## x86_64

```
$ make
...
       GEN  build/ide.img
4098+0 records in
4098+0 records out
4196352 bytes (4.2 MB, 4.0 MiB) copied, 0.00950345 s, 442 MB/s
       GEN  build/servers/drivers/blk/ide/disk.o
        LD  build/ide.debug.elf
...

$ cat build/servers/drivers/blk/ide/disk.S
.data; .align 4096; .global __image, __image_end; __image: .incbin "build/ide.img"; __image_end:

$ readelf -SW build/servers/drivers/blk/ide/disk.o
There are 6 section headers, starting at offset 0x4018d0:

Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
  [ 1] .strtab           STRTAB          0000000000000000 401890 00003d 00      0   0  1
  [ 2] .text             PROGBITS        0000000000000000 000040 000000 00  AX  0   0  4
  [ 3] .data             PROGBITS        0000000000000000 001000 400800 00  WA  0   0 4096
  [ 4] .debug_line       PROGBITS        0000000000000000 401800 000046 00      0   0  1
  [ 5] .symtab           SYMTAB          0000000000000000 401848 000048 18      1   1  8
```
