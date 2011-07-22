# Makefile for Linux build of Parthus Host Side Stack
# (c) Copyright Parthus Technologies 2000

DEFINES = -DUSE_PTYS=1 -DDEBUG

CFLAGS = -g -Wall -Wcast-align $(INCLUDE) -D_REENTRANT $(DEFINES)
CC = gcc 

OUT_DIR = $(PWD)/common-objs
TARGET_DIR = $(PWD)
TOP_DIR = $(TARGET_DIR)/../../../..
TARGET_RULES = $(TARGET_DIR)/Make.rules
ARCH_DIR = $(TOP_DIR)/arch/x86-pc-linux-lib
INCLUDE = -I$(ARCH_DIR)/include/ -I$(TOP_DIR)/include -I$(TOP_DIR)/hci/include -I$(TOP_DIR)/l2cap/include -I$(TOP_DIR)/sdp/include -I$(TOP_DIR)/manager/include -I$(TOP_DIR)/api/include -I$(TOP_DIR)/rfcomm/include -I$(TOP_DIR)/user
export OUT_DIR TARGET_RULES CC CFLAGS CPPFLAGS TOP_DIR TARGET_DIR

all: rfcomm_tester

OBJS = test_run.o conform_tests.o rftest_pdu_gen.o rftest_l2cap.o rftest_handle_pdu.o rftest_frames.o rftest_ctrl_frames.o
LIBS = $(ARCH_DIR)/libprh_bt.a -lpthread -lefence

clean:
	find . -name "*.[oa]" -exec rm {} \;

rfcomm_tester: $(OBJS)
	$(CC) -ggdb -o rfcomm_tester $(OBJS) $(LIBS)

