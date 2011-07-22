#!/bin/bash



#make gen_opt 

MMICUSTOMER_DIR=$1

RESGEN_DIR=${MMICUSTOMER_DIR}/ResGenerator
RESGEN_DFT_DIR=$SOFT_WORKDIR/application/mmi_customer/ResGenerator
CUSTRES_DIR=${MMICUSTOMER_DIR}/CustResource
DEFAULT_CUSTRES_DIR=$SOFT_WORKDIR/application/mmi_customer/CustResource
export CUSTINC_DIR=${MMICUSTOMER_DIR}/CustomerInc
export PATH=$PATH:${RESGEN_DIR}
export WINEDEBUG="-all"
export CT_ERES=$2
mkdir -p $RESGEN_DIR
mkdir -p $RESGEN_DIR/res_temp
mkdir -p $CUSTINC_DIR
#rm -f $RESGEN_DIR/res_temp/*.exe

pwd
./application/CoolSimulator/GenCoolSim_dsp.sh
echo "Making Resources..."
#这里把相关目录的头文件拷贝到resgen来保证和工程的同步。
cp $SOFT_WORKDIR/platform/chip/defs/include/chip_id.h $CUSTINC_DIR
cp $SOFT_WORKDIR/application/coolmmi/mmi/include/custdataprots.h $CUSTINC_DIR
cp $SOFT_WORKDIR/application/coolmmi/mmi/include/custdatares.h $CUSTINC_DIR
cp $SOFT_WORKDIR/application/coolmmi/mmi/include/custmenures.h $CUSTINC_DIR
cp $SOFT_WORKDIR/application/coolmmi/mmi/include/fontres.h $CUSTINC_DIR
cp $SOFT_WORKDIR/application/coolmmi/mmi/include/globalmenuitems.h $CUSTINC_DIR
#cp $SOFT_WORKDIR/$MMI_CONFIG/include/mmi_features.h $CUSTINC_DIR
#cp $SOFT_WORKDIR/application/coolmmi/mmi/include/unicodedcl.h $CUSTINC_DIR
#cp $SOFT_WORKDIR/application/systeminc/mmi/include/switch.h $CUSTINC_DIR
cp $SOFT_WORKDIR/application/systeminc/mmi/include/kal_non_specific_general_types.h $CUSTINC_DIR
cp $SOFT_WORKDIR/application/coolmmi/mmi/Framework/include/oslmemory.h $CUSTINC_DIR
cp $SOFT_WORKDIR/platform/csw/include/ds.h $CUSTINC_DIR
cp $SOFT_WORKDIR/platform/csw/include/ts.h $CUSTINC_DIR
cp $SOFT_WORKDIR/platform/csw/include/drv.h $CUSTINC_DIR
cp $SOFT_WORKDIR/platform/csw/include/cos.h $CUSTINC_DIR
#cp $SOFT_WORKDIR/application/coolmmi/mmi_csdapp/GDI/include/gdi_include.h $CUSTINC_DIR


#copy all exe which we need from default folder
cp $RESGEN_DFT_DIR/copy_images.sh $RESGEN_DIR/copy_images.sh
cp $RESGEN_DFT_DIR/incp.pl $RESGEN_DIR/incp.pl
cp $RESGEN_DFT_DIR/*.exe $RESGEN_DIR/
cp $RESGEN_DFT_DIR/*.c $RESGEN_DIR/
cp $RESGEN_DFT_DIR/*.cpp $RESGEN_DIR/
cp $RESGEN_DFT_DIR/*.h $RESGEN_DIR/
cp $RESGEN_DFT_DIR/Makefile $RESGEN_DIR/
cp $RESGEN_DFT_DIR/*.map $RESGEN_DIR/
cp $RESGEN_DFT_DIR/*.dll $RESGEN_DIR/
cp $RESGEN_DFT_DIR/custom_include.txt $RESGEN_DIR/
cp $RESGEN_DFT_DIR/plmnlist.txt $RESGEN_DIR/
cp $RESGEN_DFT_DIR/ref_list.txt $RESGEN_DIR/
cp -Rf $RESGEN_DFT_DIR/compress $RESGEN_DIR/



echo "run perl............."


#cp cs_types.h。由于不能修改工程文件，所以需要在cp过程做些修改。下同。
perl $RESGEN_DIR/incp.pl "$SOFT_WORKDIR/platform/include/cs_types.h" \
										"$CUSTINC_DIR/cs_types.h" \
										10 \
										"typedef\s+unsigned\s+char\s+BOOL;" \
										3 \
										"" \
 										"typedef\s+unsigned\s+long\s+UINT32;"  \
 										3 \
 										"" \
 										"typedef\s+long\s+INT32;" \
 										3 \
 										"" \
 										"typedef\s+short\s+WCHAR;" \
 										3 \
 										"" \
 										"typedef\s+UINT32\s+HANDLE;"  \
 										3 \
 										"" \
 										"typedef\s+UINT32\*\s+PUINT32;" \
 										3 \
 										"" \
 										"typedef\s+INT32\*\s+PINT32;"  \
 										3 \
 										"" \
 										"typedef\s+int\s+ ssize_t;" \
 										3 \
 										"" \
 										"typedef\s+unsigned\s+char\s+bool;" \
 										3 \
 										"" \
 										"typedef\s+UINT8\*\s+PSTR;\s*\r\ntypedef\s+CONST\s+UINT8\*\s+PCSTR;\s*\r\ntypedef\s+UINT8\s+TCHAR;\s*\r\ntypedef\s+UINT8\*\s+PTCHAR;" \
 										3 \
 										"" 
#cp cswtype.h。
perl $RESGEN_DIR/incp.pl "$SOFT_WORKDIR/platform/csw/include/cswtype.h" \
										"$CUSTINC_DIR/cswtype.h" \
										1 \
										"typedef\s+HANDLE\s+HKEY;\s*\r\ntypedef\s+HANDLE\s+HRES;\s*\r\ntypedef\s+UINT32\s+EVID;\s*\r\ntypedef\s+UINT8\s+LANGID;\s*\r\ntypedef\s+UINT32\s+EVPARAM;\s*\r\ntypedef\s+UINT32\s+HAO;" \
										3 \
										"" 
#frameworkstruct.h
perl $RESGEN_DIR/incp.pl "$SOFT_WORKDIR/application/coolmmi/mmi/Framework/include/frameworkstruct.h" \
										"$CUSTINC_DIR/frameworkstruct.h" \
										5\
										"#include\s+\"queuegprot.h\"" \
										3 \
										"" \
										"typedef\s+struct\s*KEYBRD_MESSAGE\s+{.*}\s*KEYBRD_MESSAGE;" \
										3 \
										"" \
										"typedef\s+struct\s*{.*}\s*mmi_frm_context_struct;" \
										3 \
										"" \
										"typedef\s+U8\s+\(\*PsKeyProcessCBPtr\)\(KEYBRD_MESSAGE\s+\*\);" \
										3 \
										"" \
										"extern\s+mmi_frm_context_struct\s+g_mmi_frm_cntx;" \
										3 \
										"" \
#globalconstants.h
perl $RESGEN_DIR/incp.pl "$SOFT_WORKDIR/application/coolmmi/mmi/include/globalconstants.h" \
										"$CUSTINC_DIR/globalconstants.h" \
										1 \
										"#include\s+\"adp_key.h\"" \
										3 \
										"" 			

#oslmemory.h
perl $RESGEN_DIR/incp.pl "$SOFT_WORKDIR/application/coolmmi/mmi/include/mmi_data_types.h" \
										"$CUSTINC_DIR/mmi_data_types.h" \
										1 \
										"typedef\s+double\s+DOUBLE;.*S64;\s+typedef\s+unsigned\s+int\s+UINT;" \
										3 \
										"" 			
#stdc.h
echo -e "#include <string.h>\r\n#include \"cs_types.h\"\r\n#include \"custdatares.h\"\r\n" > rep.tmp
perl $RESGEN_DIR/incp.pl "$SOFT_WORKDIR/application/coolmmi/mmi/include/stdc.h" \
										"$CUSTINC_DIR/stdc.h" \
										1 \
										"#include\s+<string.h>" \
										4 \
										"rep.tmp"
rm rep.tmp										
#unicodedcl.h
perl $RESGEN_DIR/incp.pl "$SOFT_WORKDIR/application/coolmmi/mmi/include/unicodedcl.h" \
										"$CUSTINC_DIR/unicodedcl.h" \
										1 \
										"extern\s+" \
										3 \
										"" 	
																																									 
echo "" > $CUSTINC_DIR/mmi_trace.h
cd $RESGEN_DIR
#$RESGEN_DIR/reimp $RESGEN_DIR/mmicustomizer.lib

echo "create plmnlist"
make plmncreate.exe 
result=$?
echo $result
if [ $result != 0 ]; then
exit 1;
fi



echo "make resgenerator.exe........"
make resgenerator.exe 
result=$?
echo $result
if [ $result != 0 ]; then
exit 1;
fi


case "$(uname -s)" in 
"CYGWIN"*)
RESGENERATOR='./res_temp/resgenerator.exe' ;;
*)
RESGENERATOR='wine ./res_temp/resgenerator.exe' ;;
esac

echo "run resgenerator.exe......"
if [ ! -f ./res_temp/resgenerator.exe ]; then
echo "Can't build resgen"
elif [ "$2" = "str" ] ; then
$RESGENERATOR -s
echo "./res_temp/mtk_resgenerator.exe -s"
elif [ "$2" = "nozip" ] ;then
RESGENERATOR -g -o > ./res_temp/resgenerator.log
echo "./res_temp/resgenerator.exe -g -o > ./res_temp/resgenerator.log"
echo "Using current exist image directory"
else
$RESGENERATOR -g -x > ./res_temp/mtk_resgenerator.log
echo "./res_temp/resgenerator.exe -g -x > ./res_temp/resgenerator.log"
echo "Unzip image.zip"
fi

echo "run resgenerator.exe done"
rm -rdf *bin.lzma


#拷贝文件。最好写一个专门的拷贝函数。这里暂时先拷贝image文件。
if [ "$2" = "str" ] ; then
echo "Starting deal with strings............."
#skip updata images
else
echo "Starting copy Image files............."
if [ ! -f $CUSTRES_DIR/CustImgMap.c ]; then
echo "Can't find $CUSTRES_DIR/CustImgMap.c"
else
cp $CUSTRES_DIR/CustImgMap.c $CUSTRES_DIR/src/CustImgMap.c

#cp $CUSTRES_DIR/CustImgMap.c $DEFAULT_CUSTRES_DIR/src/CustImgMap.c
fi

if [ ! -f $CUSTRES_DIR/CustImgRes.c ]; then
echo "Can't find $CUSTRES_DIR/CustImgRes.c"
else
cp $CUSTRES_DIR/CustImgRes.c $CUSTRES_DIR/src/CustImgRes.c
#cp $CUSTRES_DIR/CustImgRes.c $DEFAULT_CUSTRES_DIR/src/CustImgRes.c

fi


if [ ! -f $CUSTRES_DIR/custimgdatahw.h ]; then
echo "Can't find $CUSTRES_DIR/custimgdatahw.h"
else
#做pack处理。因为原来的CustImgDataHW.h的某些变量命名不符合规范，所以为了编译通过在这里做特例处理
#wangzl:pack.c中的数据已经做了处理，不需要再进行本处理
#cat pack.c >> $CUSTRES_DIR/custimgdatahw.h
cp $CUSTRES_DIR//custimgdatahw.h $CUSTRES_DIR/include/custimgdatahw.h
#cp $CUSTRES_DIR//custimgdatahw.h $DEFAULT_CUSTRES_DIR/include/custimgdatahw.h

fi


if [ ! -f $CUSTRES_DIR/CustImgMapExt.c ]; then
echo "Can't find $CUSTRES_DIR/CustImgMapExt.c"
else
cp $CUSTRES_DIR/CustImgMapExt.c $CUSTRES_DIR/src/CustImgMapExt.c
#cp $CUSTRES_DIR/CustImgMapExt.c $DEFAULT_CUSTRES_DIR/src/CustImgMapExt.c

fi

if [ ! -f $CUSTRES_DIR/CustImgResExt.c ]; then
echo "Can't find $CUSTRES_DIR/CustImgResExt.c"
else
cp $CUSTRES_DIR/CustImgResExt.c $CUSTRES_DIR/src/CustImgResExt.c
#cp $CUSTRES_DIR/CustImgResExt.c $DEFAULT_CUSTRES_DIR/src/CustImgResExt.c

fi

if [ ! -f $CUSTRES_DIR/custimgdatahwext.h ]; then
echo "Can't find $CUSTRES_DIR/custimgdatahwext.h"
else
cp $CUSTRES_DIR/custimgdatahwext.h $CUSTRES_DIR/include/custimgdatahwext.h
#cp $CUSTRES_DIR/custimgdatahwext.h $DEFAULT_CUSTRES_DIR/include/custimgdatahwext.h

fi

if [ ! -f $CUSTRES_DIR/CustGameDataHW.h ]; then
echo "Can't find $CUSTRES_DIR/CustGameDataHW.h"
else
cp $CUSTRES_DIR/CustGameDataHW.h $CUSTRES_DIR/include/custgamedatahw.h
#cp $CUSTRES_DIR/CustGameDataHW.h $DEFAULT_CUSTRES_DIR/include/custgamedatahw.h

fi


fi
# End of if [ "$2" = "str" ] ; then

#menu files copy
if [ ! -f $CUSTRES_DIR/CustMenuRes.c ]; then
echo "Can't find $CUSTRES_DIR/CustMenuRes.c"
else
cp $CUSTRES_DIR/CustMenuRes.c $CUSTRES_DIR/src/CustMenuRes.c
#cp $CUSTRES_DIR/CustMenuRes.c $DEFAULT_CUSTRES_DIR/src/CustMenuRes.c
fi

#string的处理


echo "MergeStrRes CustStrList.txt ref_list.txt.................."
case "$(uname -s)" in 
"CYGWIN"*)
MergeStrRes CustStrList.txt ref_list.txt ;;
*)
wine MergeStrRes.exe CustStrList.txt ref_list.txt ;;
esac


cd $RESGEN_DIR/compress
make clean
make

cd $RESGEN_DIR

echo "make readexcel.exe..........."
make readexcel.exe 
result=$?
if [ $result != 0 ]; then
exit 1;
fi


#if [ ! -f ./res_temp/readexcel.exe ]; then
#echo "Can't build readexcel"
#else
case "$(uname -s)" in 
"CYGWIN"*)
    ./res_temp/readexcel.exe CustResList_out.txt ;;
*)
    WINEDLLOVERRIDES="msvcrt=n" WINEDLLPATH=. wine ./res_temp/readexcel.exe CustResList_out.txt ;;
esac
#fi

#str files copy
if [ ! -f $CUSTRES_DIR/CustStrRes.c ]; then
echo "Can't find $CUSTRES_DIR/CustStrRes.c"
else
cp $CUSTRES_DIR/CustStrRes.c $CUSTRES_DIR/src/CustStrRes.c
#cp $CUSTRES_DIR/CustStrRes.c $DEFAULT_CUSTRES_DIR/src/CustStrRes.c

fi
#str files copy
if [ ! -f $CUSTRES_DIR/CustStrMap.c ]; then
echo "Can't find $CUSTRES_DIR/CustStrMap.c"
else
cp $CUSTRES_DIR/CustStrMap.c $CUSTRES_DIR/src/CustStrMap.c
#cp $CUSTRES_DIR/CustStrMap.c $DEFAULT_CUSTRES_DIR/src/CustStrMap.c

fi

if [ ! -x ../compress_process.sh ];then
echo "$PWD/../compress_process.sh: Permission denied"
exit 1
fi

../compress_process.sh $1
	
echo "Resources been generated!"
#清理现场 
rm $CUSTRES_DIR/*.c
rm $CUSTRES_DIR/*.h
rm -f -r $CUSTINC_DIR
rm -f -r $RESGEN_DIR/res_temp
rm -f -r $RESGEN_DIR/obj
rm -rdf   $RESGEN_DIR/*
cd ..
#rm -f -r $RESGEN_DIR #Device or resource busy,so i can't delete it

