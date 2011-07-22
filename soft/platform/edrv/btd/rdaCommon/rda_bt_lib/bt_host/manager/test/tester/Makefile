CFLAGS = -g  $(INCLUDES)
CC = gcc 


TOP_DIR = $(PWD)/../../../
ARCH_DIR = $(TOP_DIR)/arch/x86-pc-linux-lib
INCLUDES = -I$(TOP_DIR)/user/ -I$(TOP_DIR)/manager/include/ -I$(TOP_DIR)/hci/include/ -I$(TOP_DIR)/sdp/include/ -I$(TOP_DIR)/l2cap/include/ -I$(TOP_DIR)/rfcomm/ -I$(TOP_DIR)/include/ -I$(ARCH_DIR)/include/
 
LIBS = $(ARCH_DIR)/libprh_bt.a -lpthread

TESTER_OBJS = tester_gap.o gap_tester_conform_tests.o

all: tester_gap

%.o: %.c
	gcc -c $(INCLUDES) $(CFLAGS) $< -o $@


tester_gap: $(TESTER_OBJS)
	$(CC) $(CFLAGS) -ggdb -o tester_gap $(TESTER_OBJS) $(LIBS)

clean:
	rm -f *.o




