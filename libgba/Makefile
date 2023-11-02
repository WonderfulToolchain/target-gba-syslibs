# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Adrian "asie" Siekierka, 2023

WONDERFUL_TOOLCHAIN ?= /opt/wonderful
TARGET ?= gba/rom
WF_TARGET_DIR ?= $(WONDERFUL_TOOLCHAIN)/target/$(TARGET)
include $(WF_TARGET_DIR)/makedefs.mk

# Source code paths
# -----------------

INCLUDEDIRS	:= include
SOURCEDIRS	:= src
ASSETDIRS	:= assets

# Defines passed to all files
# ---------------------------

DEFINES		:=

# Libraries
# ---------

LIBS		:= -lc
LIBDIRS		:= $(WF_TARGET_DIR) $(WF_TARGET_DIR)/.. $(WONDERFUL_TOOLCHAIN)/toolchain/gcc-arm-none-eabi/arm-none-eabi/gba

# Build artifacts
# ---------------

NAME		:= gba
BUILDDIR	:= build/$(TARGET)
ARCHIVE		:= $(BUILDDIR)/lib$(NAME).a

# Verbose flag
# ------------

ifeq ($(V),1)
_V		:=
else
_V		:= @
endif

# Source files
# ------------

ifneq ($(ASSETDIRS),)
    SOURCES_WFPROCESS	:= $(shell find -L $(ASSETDIRS) -name "*.lua")
    INCLUDEDIRS		+= $(addprefix $(BUILDDIR)/,$(ASSETDIRS))
endif
SOURCES_S	:= $(shell find -L $(SOURCEDIRS) -name "*.s")
SOURCES_C	:= $(shell find -L $(SOURCEDIRS) -name "*.c")

# Compiler and linker flags
# -------------------------

WARNFLAGS	:= -Wall

INCLUDEFLAGS	:= $(foreach path,$(INCLUDEDIRS),-I$(path)) \
		   $(foreach path,$(LIBDIRS),-isystem $(path)/include)

LIBDIRSFLAGS	:= $(foreach path,$(LIBDIRS),-L$(path)/lib)

ASFLAGS		+= -x assembler-with-cpp $(DEFINES) \
		   $(INCLUDEFLAGS) -ffunction-sections -fdata-sections

CFLAGS		+= -std=gnu2x $(WARNFLAGS) $(DEFINES) \
		   $(INCLUDEFLAGS) -ffunction-sections -fdata-sections -O2 \
		   -flto -ffat-lto-objects

# Intermediate build files
# ------------------------

OBJS_ASSETS	:= $(addsuffix .o,$(addprefix $(BUILDDIR)/,$(SOURCES_WFPROCESS)))

OBJS_SOURCES	:= $(addsuffix .o,$(addprefix $(BUILDDIR)/,$(SOURCES_S))) \
		   $(addsuffix .o,$(addprefix $(BUILDDIR)/,$(SOURCES_C)))

OBJS		:= $(OBJS_ASSETS) $(OBJS_SOURCES)

DEPS		:= $(OBJS:.o=.d)

# Targets
# -------

.PHONY: all clean doc install

all: $(ARCHIVE) compile_commands.json

$(ARCHIVE): $(OBJS)
	@echo "  AR      $@"
	@$(MKDIR) -p $(@D)
	$(_V)$(AR) rcs $@ $(OBJS)

clean:
	@echo "  CLEAN"
	$(_V)$(RM) $(ARCHIVE) $(BUILDDIR) compile_commands.json

compile_commands.json: $(OBJS) | Makefile
	@echo "  MERGE   compile_commands.json"
	$(_V)$(WF)/bin/wf-compile-commands-merge $@ $(patsubst %.o,%.cc.json,$^)

doc:
	@echo "  MKDIR    build/doc"
	$(_V)$(MKDIR) -p build/doc
	@echo "  DOXYGEN"
	doxygen

install: all
	@echo "  INSTALL $(ARCHIVE)"
	$(_V)$(MKDIR) -p $(WF_TARGET_DIR)/lib
	$(_V)$(CP) -ar include $(WF_TARGET_DIR)
	$(_V)$(CP) -ar $(ARCHIVE) $(WF_TARGET_DIR)/lib

# TODO: Write install stage.

# Rules
# -----

$(BUILDDIR)/%.s.o : %.s | $(OBJS_ASSETS)
	@echo "  AS      $<"
	@$(MKDIR) -p $(@D)
	$(_V)$(CC) $(ASFLAGS) -MMD -MP -MJ $(patsubst %.o,%.cc.json,$@) -c -o $@ $<

$(BUILDDIR)/%.c.o : %.c | $(OBJS_ASSETS)
	@echo "  CC      $<"
	@$(MKDIR) -p $(@D)
	$(_V)$(CC) $(CFLAGS) -MMD -MP -MJ $(patsubst %.o,%.cc.json,$@) -c -o $@ $<

$(BUILDDIR)/%.lua.o : %.lua
	@echo "  PROCESS $<"
	@$(MKDIR) -p $(@D)
	$(_V)$(WF)/bin/wf-process -o $(BUILDDIR)/$*.c -t $(TARGET) --depfile $(BUILDDIR)/$*.lua.d --depfile-target $(BUILDDIR)/$*.lua.o $<
	$(_V)$(CC) $(CFLAGS) -MMD -MP -c -o $(BUILDDIR)/$*.lua.o $(BUILDDIR)/$*.c

# Include dependency files if they exist
# --------------------------------------

-include $(DEPS)
