# Makefile for Linux build of Parthus Host Side Stack
# (c) Copyright Parthus Technologies 2000

DEFINES = -DUSE_PTYS=1 -DDEBUG -DUSE_LINUX_SERIAL=1

CFLAGS = -g $(INCLUDE) -D_REENTRANT $(DEFINES)
CC = gcc 

OUT_DIR = $(PWD)/common-objs
TARGET_DIR = $(PWD)
TOP_DIR = $(TARGET_DIR)/../../..
TARGET_RULES = $(TARGET_DIR)/Make.rules
ARCH_DIR = $(TOP_DIR)/arch/x86-pc-linux-lib
INCLUDE = -I$(ARCH_DIR)/include/ -I$(TOP_DIR)/include -I$(TOP_DIR)/hci/include -I$(TOP_DIR)/l2cap/include -I$(TOP_DIR)/sdp/include -I$(TOP_DIR)/manager/include -I$(TOP_DIR)/api/include -I$(TOP_DIR)/rfcomm/include
export OUT_DIR TARGET_RULES CC CFLAGS CPPFLAGS TOP_DIR TARGET_DIR

all: rfcomm_iut

LIBS = $(ARCH_DIR)/libprh_bt.a -lpthread

IUT_OBJS = iut_test.o port_ent_api.o port_io.o conform_tests.o sys_data.o
clean:
	find . -name "*.[oa]" -exec rm {} \;

rfcomm_iut: $(IUT_OBJS)
	$(CC) -ggdb -o rfcomm_iut $(IUT_OBJS) $(LIBS)

