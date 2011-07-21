include ${SOFT_WORKDIR}/env/compilation/mmi_compilevars.mk

CFLAGS=-O2 -Wall
#CFLAGS=-g -Wall -Werror
CC=gcc
AR=ar

ifneq "$(shell uname | grep -c CYGWIN)" "0"
	LZMA=./lzma/lzma.exe
else
	LZMA=wine ./lzma/lzma.exe
endif

ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_240_320"
QQ_INCLUDE=../../../../../coolmmi/mmi/jmetoc/240_320/include
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_176_220"
QQ_INCLUDE=../../../../../coolmmi/mmi/jmetoc/176_220/include
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_220_176"
QQ_INCLUDE=../../../../../coolmmi/mmi/jmetoc/220_176/include
endif
ifeq "$(strip ${MMI_PARAM_LCDSIZE})" "LCDSIZE_128_160"
QQ_INCLUDE=../../../../../coolmmi/mmi/jmetoc/128_160/include
endif

FONT_INCLUDE=../../CustResource/include 

all:	bin2array GenQqsymTab GenFontTab huffcode #lzma
	./GenQqsymTab
	./GenFontTab
ifneq "$(findstring _MMI_SUPPORT_JME_QQ_, ${MMI_EXPORT_FLAG})" "" 
	./huffman/huffcode -i QQ_SYM.bin -o QQ_SYM_huffman.bin
	$(LZMA) -f -v -9 QQ_SYM.bin
	./bin2array QQ_SYM_huffman.bin QQ_SYM_huffman.tab
	./bin2array QQ_SYM.bin.lzma QQ_SYM_lzma.tab
	cp -f QQ_SYM*.tab $(QQ_INCLUDE)
endif
	
	
ifneq "$(findstring _MMI_SUPPORT_JME_GEO_, ${MMI_EXPORT_FLAG})" "" 
	./huffman/huffcode -i GEO_SYM.bin -o GEO_SYM_huffman.bin
	$(LZMA) -f -v -9 GEO_SYM.bin
	./bin2array GEO_SYM_huffman.bin GEO_SYM_huffman.tab
	./bin2array GEO_SYM.bin.lzma GEO_SYM_lzma.tab
	cp -f GEO_SYM*.tab $(QQ_INCLUDE)
endif
ifneq "$(findstring _MMI_SUPPORT_JME_MSN_, ${MMI_EXPORT_FLAG})" "" 
	./huffman/huffcode -i MSN_SYM.bin -o MSN_SYM_huffman.bin
	$(LZMA) -f -v -9 MSN_SYM.bin
	./bin2array MSN_SYM_huffman.bin MSN_SYM_huffman.tab	
	./bin2array MSN_SYM.bin.lzma MSN_SYM_lzma.tab
	cp -f MSN_SYM*.tab $(QQ_INCLUDE)
endif
	./huffman/huffcode -i GB14x14_Data.bin -o GB14x14_Data_huffman.bin
	$(LZMA) -f -v -9 GB14x14_Data.bin
	./bin2array GB14x14_Data_huffman.bin GB14x14_Data_huffman.tab

	./bin2array GB14x14_Data.bin.lzma GB14x14_Data_lzma.tab
	cp -f GB14x14_Data*.tab $(FONT_INCLUDE)

huffcode:
	make -C huffman

#lzma:
#	make -C lzma

bin2array : bin2array.c
	$(CC) $(CFLAGS) -o $@ $^

GenQqsymTab: GenQqsymTab.c
	$(CC) $(CFLAGS) -o $@ $^ -I$(QQ_INCLUDE)

GenFontTab: GenFontTab.c
	$(CC) $(CFLAGS) -o $@ $^ -I$(FONT_INCLUDE)


clean:
	make -C huffman clean
	$(RM) *.o *~ core *.bin *.tab *.lzma GenQqsymTab GenQqsymTab.exe GenFontTab GenFontTab.exe bin2array bin2array.exe
