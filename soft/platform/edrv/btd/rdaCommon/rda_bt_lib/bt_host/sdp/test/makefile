CCFlags = -g -gp -Wall -pedantic
LDFlags = -L. -L../../arch/x86-pc-linux-lib
IFlags = -I../../include -I../include -I../../l2cap/include -I../../manager/include -I../../hci/include -I../../api/include -I../../arch/x86-pc-linux-lib/include -I../../user
OBJECTS = ..
LIBS = -lprh_bt -lpthread 

%.o: %.c
	gcc -c $(CCFlags) $(IFlags) $< -o $@

all: sdp_test server_sdp

sdp_test : libs test_sdp.o 
	gcc $(CCFlags) $(IFlags) $(LDFlags) test_sdp.o $(LIBS) -o $@ 

server_sdp : libs server_sdp.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) server_sdp.o $(LIBS) -o $@	

clean :
	rm -rf *.o ../*.o

libs :
	cd ../../arch/x86-pc-linux-lib ; tcsh -c 'gmake'
	echo "Finished creating libprh_bt.a" 

ecos : libs ecos.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) ecos.o $(LIBS) -o $@

tci : libs tci_lower.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) tci_lower.o $(LIBS) -o $@

unit : unit_1 unit_2 


unit_1 : libs unit_1.o cunit.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) unit_1.o cunit.o $(LIBS) -o $@

unit_2 : libs unit_2.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) unit_2.o $(LIBS) -o $@


examples : libs ex6.o ex5.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) ex5.o $(LIBS) -o ex5 
	gcc $(CCFlags) $(IFlags) $(LDFlags) ex6.o $(LIBS) -o ex6 
