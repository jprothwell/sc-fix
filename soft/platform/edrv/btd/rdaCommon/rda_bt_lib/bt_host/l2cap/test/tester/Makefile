CFLAGS = -g  $(INCLUDES) -DDEBUG
CC = gcc 


TOP_DIR = $(PWD)/../../../../bt_host
ARCH_DIR = $(TOP_DIR)/arch/x86-pc-linux-lib
INCLUDES = -I./  -I$(TOP_DIR)/user/ -I$(TOP_DIR)/manager/include/ -I$(TOP_DIR)/hci/include/ -I$(TOP_DIR)/sdp/include/ -I$(TOP_DIR)/l2cap/include/ -I$(TOP_DIR)/rfcomm/ -I$(TOP_DIR)/include/ -I$(ARCH_DIR)/include/
 
LIBS = $(ARCH_DIR)/libprh_bt.a -lpthread

TESTER_OBJS = tester_l2cap.o l2cap_lower_tester.o

all: tester_l2cap

%.o: %.c
	gcc -c $(INCLUDES) $(CFLAGS) $< -o $@


tester_l2cap: $(TESTER_OBJS)
	$(CC) $(CFLAGS) -ggdb -o tester_l2cap $(TESTER_OBJS) $(LIBS)

clean:
	rm -f *.o

