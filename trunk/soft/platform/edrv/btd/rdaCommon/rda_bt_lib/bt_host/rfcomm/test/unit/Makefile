DEFINES = -DPRH_HOST_UNIT_TEST=1
CFLAGS = -g -Wall -pedantic $(DEFINES)
TOP = ../../..
LDFLAGS = -L. -L$(TOP)/arch/x86-pc-linux-lib
INCLUDE = -I$(TOP)/include -I../../include -I$(TOP)/arch/x86-pc-linux-lib/include -I$(TOP)/user

HARNESS_OBJS = hu_harness.o
UNIT_TEST_OBJS = rfcomm_unit_conntest.o rfcomm_unit_l2cap_stub.o cunit.o 
RFTEST_OBJS = rftest_handle_pdu.o rftest_frames.o rftest_ctrl_frames.o rftest_pdu_gen.o

OBJECTS = $(UNIT_TEST_OBJS) $(RFTEST_OBJS) $(HARNESS_OBJS)

LIBS = -lprh_bt -lpthread 

export DEFINES

%.o: %.c
	gcc -c $(CFLAGS) $(INCLUDE) $< -o $@

all: unit_tests

clean :
	rm -rf *.o ; cd $(TOP)/arch/x86-pc-linux-lib ; gmake clean

libs :
	cd $(TOP)/arch/x86-pc-linux-lib ; tcsh -c 'gmake'
	echo "Finished creating libprh_bt.a" 


unit_tests : libs $(OBJECTS)#  cunit.o 
	gcc $(CFLAGS) $(INCLUDE) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@
