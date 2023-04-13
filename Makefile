# Default values for build system.
export V         ?=
export VERSION   ?= v0.9.0
export BUILD_DIR ?= build
ifeq ($(shell uname), Darwin)
LLVM_PREFIX ?= /usr/local/opt/llvm/bin/
GRUB_PREFIX ?= /usr/local/opt/i386-elf-grub/bin/i386-elf-
endif

# The default build target.
.PHONY: default
default: build

# Disable builtin implicit rules and variables.
MAKEFLAGS += --no-builtin-rules --no-builtin-variables
.SUFFIXES:

# Enable verbose output if $(V) is set.
ifeq ($(V),)
.SILENT:
endif

# Determine if we need to load ".config".
non_config_targets := defconfig olddefconfig menuconfig mergeconfig website book clean unittest
load_config := y
ifeq ($(filter-out $(non_config_targets), $(MAKECMDGOALS)),)
load_config :=
endif
# The default target (build) needs ".config".
ifeq ($(MAKECMDGOALS),)
load_config := y
endif

# Include other makefiles.
ifeq ($(load_config), y)
ifeq ($(wildcard .config),)
$(error .config does not exist (run 'make menuconfig' first))
endif

include .config

export ARCH  := $(patsubst "%",%,$(CONFIG_ARCH))
export MACHINE := $(patsubst "%",%,$(CONFIG_MACHINE))
boot_task_name := $(patsubst "%",%,$(CONFIG_BOOT_TASK))
boot_elf     := $(BUILD_DIR)/$(boot_task_name).elf
kernel_image := $(BUILD_DIR)/resea.elf
bootfs_bin   := $(BUILD_DIR)/bootfs.bin
builtin_libs := common resea unittest
all_servers  := $(shell tools/scan-servers-dir.py --names)
all_libs     := $(shell tools/scan-libs-dir.py --names)
servers      := \
	$(sort $(foreach server, $(all_servers), \
		$(if $(value $(shell echo CONFIG_$(server)_SERVER | \
			tr  '[:lower:]' '[:upper:]')), $(server),)))
autostarts   := \
	$(sort $(foreach server, $(all_servers), \
		$(if $(filter-out m, $(value $(shell echo CONFIG_$(server)_SERVER | \
			tr  '[:lower:]' '[:upper:]'))), $(server),)))
bootfs_files  := $(foreach name, $(servers), $(BUILD_DIR)/$(name).elf)
all_autogen_files := $(BUILD_DIR)/include/config.h $(BUILD_DIR)/include/idl.h

# Visits the soruce directory recursively and fills $(cflags), $(objs) and $(libs).
# $(1): The target source dir.
# $(2): The build dir.
define visit-subdir
$(eval objs-y :=)
$(eval autogen-files-y :=)
$(eval libs-y :=)
$(eval build_dir := $(2)/$(1))
$(eval subdirs-y :=)
$(eval cflags-y :=)
$(eval global-cflags-y :=)
$(eval global-includes-y :=)
$(eval third-party-build :=)
$(eval include $(1)/build.mk)
$(eval build_mks += $(1)/build.mk)
$(eval objs     += $(addprefix $(2)/$(1)/, $(objs-y)))
$(eval all_objs += $(addprefix $(2)/$(1)/, $(objs-y)))
$(eval autogen-files += $(autogen-files-y))
$(eval libs += $(libs-y))
$(eval cflags += $(cflags-y))
$(eval CFLAGS += $(global-cflags-y))
$(eval INCLUDES += $(global-includes-y))
$(eval $(foreach subdir, $(subdirs-y), \
	$(eval $(call visit-subdir,$(1)/$(subdir),$(2)))))
endef

# Enumerate kernel object files.
all_objs :=
objs :=
$(eval $(call visit-subdir,kernel,$(BUILD_DIR)/kernel))
$(eval $(call visit-subdir,libs/common,$(BUILD_DIR)/kernel))
kernel_objs := $(objs)

endif # ifeq ($(load_config), y)

CC         := $(LLVM_PREFIX)clang$(LLVM_SUFFIX)
LD         := $(LLVM_PREFIX)ld.lld$(LLVM_SUFFIX)
AR         := $(LLVM_PREFIX)llvm-ar$(LLVM_SUFFIX)
NM         := $(LLVM_PREFIX)llvm-nm$(LLVM_SUFFIX)
OBJCOPY    := $(LLVM_PREFIX)llvm-objcopy$(LLVM_SUFFIX)
CLANG_TIDY := $(LLVM_PREFIX)clang-tidy$(LLVM_SUFFIX)
PROGRESS   := printf "  \\033[1;96m%8s\\033[0m  \\033[1;m%s\\033[0m\\n"
PYTHON3    ?= python3
DOCKER     ?= docker
CARGO      ?= cargo
SPARSE     ?= sparse

GIT_REVISION  := $(shell git rev-parse --short HEAD)

CFLAGS += -g3 -std=c11 -ffreestanding -fno-builtin -nostdlib -nostdinc
CFLAGS += -fstack-size-section
CFLAGS += -DVERSION='"$(VERSION)"' -DGIT_REVISION='"$(GIT_REVISION)"'
CFLAGS += -DBOOTELF_PATH='"$(boot_elf)"'
CFLAGS += -DBOOTFS_PATH='"$(bootfs_bin)"'
CFLAGS += -DAUTOSTARTS='"$(autostarts)"'

WERRORS += -Wall -Wextra
WERRORS += -Werror=implicit-function-declaration
WERRORS += -Werror=int-conversion
WERRORS += -Werror=incompatible-pointer-types
WERRORS += -Werror=shift-count-overflow
WERRORS += -Werror=switch
WERRORS += -Werror=return-type
WERRORS += -Werror=pointer-integer-compare
WERRORS += -Werror=tautological-constant-out-of-range-compare
WERRORS += -Wno-unused-parameter

INCLUDES += -Ilibs/common/include
INCLUDES += -Ilibs/common/arch/$(ARCH)
INCLUDES += -I$(BUILD_DIR)/include
INCLUDES += -Ikernel/arch/$(ARCH)/machines/$(MACHINE)/include

KERNEL_CFLAGS += $(WERRORS)

CARGOFLAGS +=-Z build-std=core,alloc --quiet
RUSTFLAGS += -C lto -Z emit-stack-sizes -Z external-macro-backtrace

ifdef CONFIG_BUILD_RELEASE
RUST_BUILD := release
CFLAGS += -flto
else
RUST_BUILD := debug
SANITIZER += -fsanitize=undefined
RUSTFLAGS += -C debug_assertions=no
endif

CFLAGS += $(if $(CONFIG_OPT_LEVEL_0), -O0)
CFLAGS += $(if $(CONFIG_OPT_LEVEL_2), -O2)
CFLAGS += $(if $(CONFIG_OPT_LEVEL_3), -O3)
CFLAGS += $(if $(CONFIG_OPT_LEVEL_S), -Os)
RUSTFLAGS += $(if $(CONFIG_OPT_LEVEL_0), -C opt-level=0)
RUSTFLAGS += $(if $(CONFIG_OPT_LEVEL_2), -C opt-level=2)
RUSTFLAGS += $(if $(CONFIG_OPT_LEVEL_3), -C opt-level=3)
RUSTFLAGS += $(if $(CONFIG_OPT_LEVEL_S), -C opt-level=s)

# Disable sparse(1), a C source code analyzer if $(C) is not set.
ifeq ($(C),)
# `:` is a valid command: it do nothing and always exits with 0.
SPARSE := :
endif

#
#  Build Commands
#
.PHONY: build
build: $(kernel_image) $(BUILD_DIR)/compile_commands.json

.PHONY: lint
lint: $(BUILD_DIR)/compile_commands.json
	find kernel servers libs -name "*.c" | xargs $(CLANG_TIDY) -p $(BUILD_DIR)

EXPECTED ?= Passed all tests
.PHONY: test
test: $(kernel_image)
	$(PROGRESS) "TEST" 'EXPECTED="$(EXPECTED)"'
	./tools/run-and-check.py $(RUNCHECKFLAGS) "$(EXPECTED)" -- $(MAKE) run

unittest:
	if [ "$(TARGET)" = "" ]; then \
		echo "*** TARGET is not set"; \
		echo "*** Example: make unittest TARGET=servers/apps/test"; \
		exit 1; \
	fi
	$(MAKE) -f libs/unittest/unittest.mk test

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: defconfig
defconfig: $(BUILD_DIR)/Kconfig.autogen
	$(PROGRESS) "CONFIG"
	./tools/config.py --defconfig

.PHONY: olddefconfig
olddefconfig: $(BUILD_DIR)/Kconfig.autogen
	$(PROGRESS) "CONFIG"
	./tools/config.py --olddefconfig

.PHONY: mergeconfig
mergeconfig: $(BUILD_DIR)/Kconfig.autogen
	$(PROGRESS) "CONFIG"
	./tools/merge-config.py --outfile=.config $(CONFIG_FILES)

.PHONY: menuconfig
menuconfig: $(BUILD_DIR)/Kconfig.autogen
	./tools/config.py --menuconfig

.PHONY: website
website:
	$(PROGRESS) "GEN" $(BUILD_DIR)/website
	mkdir -p $(BUILD_DIR)/website
	cp docs/top_page.html $(BUILD_DIR)/website/index.html
	mdbook build -d $(BUILD_DIR)/website/docs
	./tools/genidl.py --lang html --idl interface.idl -o $(BUILD_DIR)/website/interfaces.html

.PHONY: book
book:
	mdbook serve

.PHONY: rustdoc
rustdoc:
	$(PROGRESS) "CARGO" $(BUILD_DIR)/rustdoc
	PROGRAM_NAME= cargo doc \
		--workspace \
		--manifest-path libs/resea/rust/Cargo.toml \
		--target-dir $(BUILD_DIR)/rustdoc

## Makefile中の変数を見るために使用
.PHONY: echovars
echovars:
	echo $(kernel_objs)

#
#  Build Rules
#
kernel_ld := kernel/arch/$(ARCH)/$(if $(MACHINE),machines/$(MACHINE)/,)kernel.ld
$(kernel_image): $(kernel_objs) $(BUILD_DIR)/kernel/__name__.o $(kernel_ld) \
		tools/nm2symbols.py tools/embed-symbols.py Makefile
	$(PROGRESS) "LD" $@
	$(LD) $(LDFLAGS) --script=$(kernel_ld) -Map $(@:.elf=.map) -o $@.tmp \
		$(kernel_objs) $(BUILD_DIR)/kernel/__name__.o
	$(PROGRESS) "GEN" $(@:.elf=.symbols)
	$(NM) $@.tmp | ./tools/nm2symbols.py > $(@:.elf=.symbols)
	$(PROGRESS) "SYMBOLS" $@
	./tools/embed-symbols.py $(@:.elf=.symbols) $@.tmp
	cp $@.tmp $@

$(bootfs_bin): $(bootfs_files) tools/mkbootfs.py
	$(PROGRESS) "MKBOOTFS" $@
	$(PYTHON3) tools/mkbootfs.py -o $@ $(bootfs_files)

$(BUILD_DIR)/kernel/%.o: %.c Makefile
	$(PROGRESS) "CC" $<
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER) $(KERNEL_CFLAGS) $(INCLUDES) -Ikernel -Ikernel/arch/$(ARCH) -DKERNEL \
		-c -o $@ $< -MD -MF $(@:.o=.deps) -MJ $(@:.o=.json)
	$(SPARSE) $(CFLAGS) $(SANITIZER) $(INCLUDES) -Ikernel -Ikernel/arch/$(ARCH) -DKERNEL $<


$(BUILD_DIR)/kernel/%.o: %.S Makefile $(boot_elf)
	$(PROGRESS) "CC" $<
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER) $(KERNEL_CFLAGS) $(INCLUDES) -Ikernel -Ikernel/arch/$(ARCH) -DKERNEL \
		-c -o $@ $< -MD -MF $(@:.o=.deps) -MJ $(@:.o=.json)

$(BUILD_DIR)/kernel/__name__.c:
	$(PROGRESS) "GEN" $@
	mkdir -p $(@D)
	echo "const char *__program_name(void) { return \"kernel\"; }" > $(@)

$(BUILD_DIR)/kernel/__name__.o: $(BUILD_DIR)/kernel/__name__.c
	$(PROGRESS) "CC" $<
	$(CC) $(CFLAGS) $(SANITIZER) $(KERNEL_CFLAGS) $(INCLUDES) -DKERNEL -c -o $@ $(@:.o=.c)

#
#  Userland build rules
#
$(BUILD_DIR)/%.o: %.c Makefile
	$(PROGRESS) "CC" $<
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER) $(INCLUDES) -c -o $@ $< -MD -MF $(@:.o=.deps) -MJ $(@:.o=.json)
	$(SPARSE) $(CFLAGS) $(SANITIZER) $(INCLUDES) $<

$(BUILD_DIR)/%.o: %.S Makefile
	$(PROGRESS) "CC" $<
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER) $(INCLUDES) -c -o $@ $< -MD -MF $(@:.o=.deps) -MJ $(@:.o=.json)

#
#  Auto-generated files.
#
$(BUILD_DIR)/Kconfig.autogen: tools/genkconfig.py \
	$(wildcard servers/*/build.mk servers/*/*/build.mk  servers/*/*/*/build.mk)
	$(PROGRESS) "GEN" $@
	mkdir -p $(@D)
	./tools/genkconfig.py -o $@

$(BUILD_DIR)/include/config.h: .config $(BUILD_DIR)/Kconfig.autogen tools/config.py
	$(PROGRESS) "GEN" $@
	mkdir -p $(@D)
	./tools/config.py --genconfig $@

$(BUILD_DIR)/include/idl.h: tools/genidl.py $(wildcard *.idl */*.idl */*/*.idl)
	$(PROGRESS) "GEN" $@
	mkdir -p $(@D)
	./tools/genidl.py --lang c --idl interface.idl -o $@

libs/resea/rust/idl/mod.rs: tools/genidl.py $(wildcard *.idl */*.idl */*/*.idl)
	$(PROGRESS) "GEN" $@
	mkdir -p $(@D)
	./tools/genidl.py --lang rust --idl interface.idl -o $(@D)

# JSON compilation database.
# https://clang.llvm.org/docs/JSONCompilationDatabase.html
$(BUILD_DIR)/compile_commands.json: $(kernel_objs)
	$(PROGRESS) "GEN" $(BUILD_DIR)/compile_commands.json
	$(PYTHON3) tools/merge-compile-commands.py \
		-o $(BUILD_DIR)/compile_commands.json \
		$(shell find $(BUILD_DIR) -type f -name "*.json")

#
#  Libs build rules
#
define lib-build-rule
$(eval dir := $(shell tools/scan-libs-dir.py --dir $(1)))
$(eval outfile := $(BUILD_DIR)/libs/$(notdir $(1)).lib.o)
$(eval outfile_a := $(BUILD_DIR)/libs/$(notdir $(1)).a)
$(eval name :=)
$(eval objs :=)
$(eval libs :=)
$(eval cflags :=)
$(eval build_mks :=)
$(eval autogen-files :=)
$(eval third-party-build :=)
$(eval $(call visit-subdir,$(dir),$(BUILD_DIR)))
$(eval lib_o := $(foreach lib, $(libs), $(BUILD_DIR)/libs/$(notdir $(lib)).lib.o))
$(eval $(outfile): objs := $(objs))
$(eval $(outfile): lib_o := $(lib_o))
$(eval $(outfile_a): lib_o := $(lib_o))
$(eval all_autogen_files += $(autogen-files))
$(eval $(outfile): $(objs) $(build_mks) $(all_autogen_files) $(lib_o))
$(eval $(outfile_a): $(outfile) $(build_mks) $(all_autogen_files) $(lib_o))
$(eval $(objs): CFLAGS += $(cflags))
$(eval $(objs): CFLAGS += $(if $(third-party-build),, $(WERRORS)))
$(eval $(objs): INCLUDES $(if $(third-party-build), :=, += $(INCLUDES)))
endef

$(foreach lib, $(all_libs), \
	$(eval $(call lib-build-rule,$(lib))))

$(BUILD_DIR)/libs/%.lib.o:
	$(PROGRESS) "LD" $(@)
	$(LD) -r -o $(@) $(objs)

$(BUILD_DIR)/libs/%.a:
	$(PROGRESS) "AR" $(@)
	$(AR) crs $(@) $(@:.a=.lib.o) $(lib_o)

#
#  Server build rules
#
define server-build-rule
$(eval server_dir := $(shell tools/scan-servers-dir.py --dir $(1)))
$(eval name :=)
$(eval objs :=)
$(eval libs := $(builtin_libs))
$(eval cflags :=)
$(eval build_mks :=)
$(eval autogen-files :=)
$(eval rust :=)
$(eval objs += $(BUILD_DIR)/$(1)/__name__.o)
$(eval $(call visit-subdir,$(server_dir),$(BUILD_DIR)))
$(eval objs += $(foreach lib, $(libs), $(BUILD_DIR)/libs/$(notdir $(lib)).a))
$(eval $(BUILD_DIR)/$(1)/__name__.c: name := $(name))
$(eval $(BUILD_DIR)/$(1).elf: name := $(name))
$(eval $(BUILD_DIR)/$(1).debug.elf: name := $(name))
$(eval $(BUILD_DIR)/$(1).debug.elf: server_dir := $(server_dir))
$(eval $(BUILD_DIR)/$(1).debug.elf: objs := $(objs))
$(eval $(BUILD_DIR)/$(1).debug.elf: objs += $(if $(rust), $(BUILD_DIR)/rust/$(name).a))
$(eval $(BUILD_DIR)/$(1).debug.elf: $(objs) $(if $(rust), $(BUILD_DIR)/rust/$(name).a) $(build_mks) $(all_autogen_files))
$(eval $(objs): CFLAGS += $(cflags))
$(foreach lib, $(all_libs),
	$(eval $(objs): INCLUDES += -Ilibs/$(lib)/include))
endef
$(foreach server, $(boot_task_name) $(servers), \
	$(eval $(call server-build-rule,$(server))))

%/__name__.c:
	$(PROGRESS) "GEN" $@
	mkdir -p $(@D)
	echo "#include <types.h>" > $(@)
	echo "const char *__program_name(void) { return \"$(name)\"; }" >> $(@)

%/__name__.o: %/__name__.c
	$(PROGRESS) "CC" $<
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER) $(INCLUDES) -c -o $(@) $<

$(BUILD_DIR)/%.debug.elf: tools/nm2symbols.py \
		tools/embed-symbols.py libs/resea/arch/$(ARCH)/user.ld Makefile
	$(PROGRESS) "LD" $(@)
	$(LD) $(LDFLAGS) --script=libs/resea/arch/$(ARCH)/user.ld \
		-Map $(@:.debug.elf=.map) -o $(@).tmp $(objs)
	$(NM) $(@).tmp | ./tools/nm2symbols.py > $(BUILD_DIR)/$(name).symbols
	$(PROGRESS) "SYMBOLS" $(BUILD_DIR)/$(name).debug.elf
	./tools/embed-symbols.py $(BUILD_DIR)/$(name).symbols $(@).tmp
	mv $(@).tmp $@

$(BUILD_DIR)/%.elf: $(BUILD_DIR)/%.debug.elf ./tools/embed-bootelf-header.py
	$(PROGRESS) "STRIP" $@
	$(OBJCOPY) --strip-all-gnu --strip-debug $< $@
	./tools/embed-bootelf-header.py --name=$(name) $(@)

FORCE:
$(BUILD_DIR)/rust/%.a: FORCE
	$(PROGRESS) "CARGO" $(server_dir)
	mkdir -p $(BUILD_DIR)/rust/$(name)
	PROGRAM_NAME="$(name)" \
		$(CARGO) +nightly build \
			--manifest-path $(server_dir)/Cargo.toml \
			--target libs/resea/rust/arch/$(ARCH)/$(ARCH).json \
			--target-dir $(BUILD_DIR)/rust/$(name) \
			$(CARGOFLAGS)
	cp $(BUILD_DIR)/rust/$(name)/$(ARCH)/$(RUST_BUILD)/lib$(name).a $@
	cat $(BUILD_DIR)/rust/$(name)/$(ARCH)/$(RUST_BUILD)/lib$(name).d \
		| ./tools/textedit.py -r ' ' ' \\\n    ' \
		| ./tools/textedit.py -r \
			'$(BUILD_DIR)/rust/$(name)/$(ARCH)/$(RUST_BUILD)/lib' \
			'$(BUILD_DIR)/rust/' \
		| ./tools/textedit.py -r \
			'$(PWD)/' \
			'' \
		| ./tools/textedit.py -f "(.a:|$(server_dir)/)" \
		> $(BUILD_DIR)/rust/$(name).deps

$(all_objs): $(all_autogen_files)

# Build dependencies generated by clang and Cargo.
-include $(shell find $(BUILD_DIR) -name "*.deps" 2>/dev/null)
