# Makefile for Linux build of Parthus Host Side Stack
# (c) Copyright Parthus Technologies 2000

DEFINES = -DUSE_PTYS=1 -DDEBUG

CFLAGS = -g $(INCLUDE) -D_REENTRANT $(DEFINES)
CC = gcc 

OUT_DIR = $(PWD)/common-objs
TARGET_DIR = $(PWD)
TOP_DIR = $(TARGET_DIR)/../../
TARGET_RULES = $(TARGET_DIR)/Make.rules
ARCH_DIR = $(TOP_DIR)/arch/x86-pc-linux-lib
INCLUDE = -I$(ARCH_DIR)/include/ -I$(TOP_DIR)/include -I$(TOP_DIR)/hci/include -I$(TOP_DIR)/l2cap/include -I$(TOP_DIR)/sdp/include -I$(TOP_DIR)/manager/include -I$(TOP_DIR)/api/include -I$(TOP_DIR)/rfcomm/include
export OUT_DIR TARGET_RULES CC CFLAGS CPPFLAGS TOP_DIR TARGET_DIR

all: rfcomm_shell

LIBS = -lpthread -ltcl8.0 -ltk8.0 -ltix

clean:
	find . -name "*.[oa]" -exec rm {} \;

rfcomm_shell: test_main.o 
	$(CC) -ggdb -o rfcomm_shell test_main.o $(LIBS)

