WF := $(WONDERFUL_TOOLCHAIN)
CC := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-gcc
CXX := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-g++
AR := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-gcc-ar
AS := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-as
LD := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-ld
NM := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-nm
OBJCOPY := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-objcopy
RANLIB := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-ranlib
CP := cp
INSTALL := install
MKDIR := mkdir
RM := rm -rf

GBAFIX := $(WF)/bin/wf-gbatool fix
ROMLINK := $(WF)/bin/wf-gbatool link $(WF_SUBTARGET)

WF_EXTLIB_DIR := $(WF)/target/$(WF_TARGET)/external

WF_ARCH_CFLAGS := -specs=$(WF)/target/$(WF_TARGET)/$(WF_SUBTARGET)/gcc.specs
WF_ARCH_ASFLAGS :=
WF_ARCH_LDFLAGS := -specs=$(WF)/target/$(WF_TARGET)/$(WF_SUBTARGET)/gcc.specs
WF_ARCH_LIBDIRS :=
