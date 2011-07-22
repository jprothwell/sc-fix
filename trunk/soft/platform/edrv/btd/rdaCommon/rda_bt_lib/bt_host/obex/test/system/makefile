CCFlags = -g -gp -Wall -pedantic
TOP = ../../..
LDFlags = -L. -L$(TOP)/arch/x86-pc-linux-lib
IFlags = -I$(TOP)/include -I../../include -I$(TOP)/arch/x86-pc-linux-lib/include -I$(TOP)/rfcomm/include -I$(TOP)/manager/include
OBJECTS = obex_unit_tests.o test_obex_server_interface.o test_obex_utils.o test_obex_tal.c
LIBS = -lprh_bt -lpthread 

%.o: %.c
	gcc -c $(CCFlags) $(IFlags) $< -o $@

all: client server
#all : server

clean :
	rm -rf *.o ; cd $(TOP)/arch/x86-pc-linux-lib ; gmake clean

libs :
	cd $(TOP)/arch/x86-pc-linux-lib ; tcsh -c 'gmake'
	echo "Finished creating libprh_bt.a" 

client : libs test_client.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) test_client.o $(LIBS) -o $@

server : libs test_server.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) test_server.o $(LIBS) -o $@

