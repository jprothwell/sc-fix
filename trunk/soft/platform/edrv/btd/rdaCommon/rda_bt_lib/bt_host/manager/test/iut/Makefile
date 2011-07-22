CFLAGS = -g  $(INCLUDES) -DEBMK_TEST=1
CC = gcc 


TOP_DIR = $(PWD)/../../../
ARCH_DIR = $(TOP_DIR)/arch/x86-pc-linux-lib
INCLUDES = -I$(TOP_DIR)/user/
LIBS = $(ARCH_DIR)/libprh_bt.a -lpthread

IUT_OBJS = iut_gap.o gap_iut_conform_tests.o

all: iut_gap

%.o: %.c
	gcc -c $(INCLUDES) $(CFLAGS) $< -o $@


iut_gap: $(IUT_OBJS)
	$(CC) $(CFLAGS) -ggdb -o iut_gap $(IUT_OBJS) $(LIBS)

clean:
	rm -f *.o