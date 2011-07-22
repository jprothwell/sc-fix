#!/bin/bash

MMICUSTOMER_DIR=$1

RESGEN_DIR=${MMICUSTOMER_DIR}/ResGenerator
CUSTRES_DIR=${MMICUSTOMER_DIR}/CustResource

case "$(uname -s)" in 
"CYGWIN"*)
LZMA="${RESGEN_DIR}/compress/lzma/lzma.exe";;
*)
LZMA="wine ${RESGEN_DIR}/compress/lzma/lzma.exe";;
esac

cd $CUSTRES_DIR
for bin in `ls StrRes_?.bin`
do
	hsuffix="_huffman.bin"
	htsuffix="_huffman.tab"
	lsuffix="_lzma.bin"
	ltsuffix="_lzma.tab"
	prefix=`echo $bin |awk -F"." '{print $1}'`
	$RESGEN_DIR/compress/huffman/huffcode -c -i $bin -o $prefix$hsuffix
	cp $bin _tmp$bin
	$LZMA -f -v -9 _tmp$bin
	mv _tmp$bin.lzma $prefix$lsuffix
	$RESGEN_DIR/compress/bin2array $prefix$hsuffix $prefix$htsuffix
	$RESGEN_DIR/compress/bin2array $prefix$lsuffix $prefix$ltsuffix
done

cp -f *.tab ./include/
rm -rdf *.bin *.lzma *.tab
