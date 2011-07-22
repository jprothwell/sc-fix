CFLAGS = -g  $(INCLUDES) -DDEBUG
CC = gcc 


TOP_DIR = $(PWD)/../../../../bt_host
ARCH_DIR = $(TOP_DIR)/arch/x86-pc-linux-lib
INCLUDES =  -I./  -I$(TOP_DIR)/user/ -I$(TOP_DIR)/manager/include/ -I$(TOP_DIR)/hci/include/ -I$(TOP_DIR)/sdp/include/ -I$(TOP_DIR)/l2cap/include/ -I$(TOP_DIR)/rfcomm/ -I$(TOP_DIR)/include/ -I$(ARCH_DIR)/include/
LIBS = $(ARCH_DIR)/libprh_bt.a -lpthread

IUT_OBJS = iut_l2cap.o l2cap_iut_test.o 

all: iut_l2cap

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@


iut_l2cap: $(IUT_OBJS)
	$(CC) $(CFLAGS) -ggdb -o iut_l2cap $(IUT_OBJS) $(LIBS)

clean:
	rm -f *.o
