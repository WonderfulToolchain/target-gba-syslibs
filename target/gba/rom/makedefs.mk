WF_TARGET := gba
WF_SUBTARGET := rom

include $(dir $(lastword $(MAKEFILE_LIST)))../makedefs-common.mk

WF_TARGET_DIR ?= $(WF)/target/gba/rom
WF_CRT0 := $(WF_TARGET_DIR)/lib/crt0.o
