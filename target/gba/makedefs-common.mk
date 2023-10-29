WF := $(WONDERFUL_TOOLCHAIN)
CC := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-gcc
AR := $(WF)/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-ar
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

WF_ARCH_CFLAGS := -mcpu=arm7tdmi -mtune=arm7tdmi -ffreestanding
WF_ARCH_ASFLAGS :=
WF_ARCH_LDFLAGS :=
WF_ARCH_LIBDIRS := $(WF)/target/gba $(WF)/toolchain/gcc-arm-none-eabi/arm-none-eabi/gba
