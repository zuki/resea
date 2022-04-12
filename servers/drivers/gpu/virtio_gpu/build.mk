name := virtio_gpu
description := A virtio-gpu GPU driver
objs-y := main.o  cairo_demo.o roboto_font.o wallpaper.o
libs-y := virtio driver third_party/cairo

$(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/cairo_demo.o: CFLAGS += $(LIBC_CFLAGS) $(CAIRO_CFLAGS)
$(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/roboto_font.o:
	$(PROGRESS) DOWNLOAD roboto-android.zip
	curl -sSL -o $(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/roboto.zip https://github.com/googlefonts/roboto/releases/download/v2.138/roboto-android.zip
	cd $(BUILD_DIR)/servers/drivers/gpu/virtio_gpu && unzip -nq roboto.zip
	$(PROGRESS) OBJCOPY $@
	$(OBJCOPY) -Ibinary -Oelf64-x86-64 $(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/Roboto-Regular.ttf $@

$(BUILD_DIR)/servers/drivers/gpu/virtio_gpu/wallpaper.o: servers/drivers/gpu/virtio_gpu/wallpaper.png
	$(PROGRESS) OBJCOPY $@
	$(OBJCOPY) -Ibinary -Oelf64-x86-64 $< $@
