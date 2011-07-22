CCFlags = -g -Wall -pedantic
LDFlags = -L. -L../../arch/x86-pc-linux-lib
IFlags = -I../../include -I../include -I../../arch/x86-pc-linux-lib/include
LIBS = -lprh_bt -lpthread

%.o: %.c
	gcc -c $(CCFlags) $(IFlags) $< -o $@

all: goep_header_test goep_object_test goep_pdu_test

goep_header_test : libs ../goep_header_encoder.o goep_header_test.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) goep_header_test.o $(LIBS) ../goep_header_encoder.o -o $@

goep_object_test : libs ../goep_obex_object.o goep_object_test.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) goep_object_test.o $(LIBS) ../goep_obex_object.o -o $@


goep_pdu_test : libs ../goep_pdu_encoder.o ../goep_obex_object.o goep_pdu_test.o ../goep_header_encoder.o
	gcc $(CCFlags) $(IFlags) $(LDFlags) goep_pdu_test.o $(LIBS) ../goep_pdu_encoder.o ../goep_obex_object.o ../goep_header_encoder.o -o $@


clean :
	rm -rf *.o ../*.o

libs :
	cd ../../arch/x86-pc-linux-lib ; tcsh -c 'gmake'
	echo "Finished creating libprh_bt.a "
