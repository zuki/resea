objs-y += trap.o init.o vm.o mp.o task.o switch.o interrupt.o
objs-y += usercopy.o
subdirs-y += machines/$(MACHINE)

QEMU  ?= qemu-system-aarch64

CFLAGS += --target=aarch64-none-eabi -mcmodel=large
CFLAGS += -mgeneral-regs-only
LDFLAGS += -z max-page-size=4096

QEMUFLAGS += -M raspi3 -cpu cortex-a53 -serial mon:stdio -semihosting -d guest_errors,unimp
QEMUFLAGS +=
QEMUFLAGS += $(if $(GUI),,-nographic)
QEMUFLAGS += $(if $(GDB),-S -s,)

ifneq ($(E1000),)
QEMUFLAGS += -netdev user,id=net0,hostfwd=tcp:127.0.0.1:1234-:80
QEMUFLAGS += -object filter-dump,id=fiter0,netdev=net0,file=e1000.pcap
QEMUFLAGS += -device e1000,netdev=net0,mac=52:54:00:12:34:56
endif

ifneq ($(VIRTIO_NET),)
ifneq ($(VIRTIO_MODERN),)
QEMUFLAGS += -device virtio-net,netdev=net1,packed=on
else
QEMUFLAGS += -device virtio-net,netdev=net1,disable-legacy=off,disable-modern=on
endif
QEMUFLAGS += -object filter-dump,id=fiter1,netdev=net1,file=virtio.pcap
QEMUFLAGS += -netdev user,id=net1,hostfwd=tcp:127.0.0.1:1234-:80
endif

ifneq ($(VIRTIO_GPU),)
QEMUFLAGS +=-vga none -device virtio-gpu,disable-legacy=off,disable-modern=on
endif

.PHONY: run
run: $(BUILD_DIR)/kernel8.img
	$(PROGRESS) "RUN" $<
	$(QEMU) $(QEMUFLAGS) -kernel $< build/ide.img
