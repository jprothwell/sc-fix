#!/bin/sh
#
#  $Id: build_api.sh 1532 2010-08-23 05:47:34Z huazeng $
#

DIRNAMES='hci l2cap manager sdp rfcomm' 
INPUTFILE=bthost_api.h
TEMPFILE=bthost_api_temp.h

echo "Building primitive headers..."

if !([ -f $INPUTFILE ]) 
then
	echo "$INPUTFILE does not exist - is the current directory correct??"
	exit
fi

if [ -f $TEMPFILE ] 
then
	echo "$TEMPFILE exists - pervious run was incomplete - manual intervention required!!"
	exit
fi

touch $TEMPFILE

if !([ -f $TEMPFILE ]) 
then
	echo "cannot create file in current directory!!"
	exit
fi

grep "\*@\*" $INPUTFILE > $TEMPFILE 2> /dev/null

echo " " >> $TEMPFILE
echo " " >> $TEMPFILE

echo $DIRNAMES
echo $INPUTFILE 

for i in $DIRNAMES ; do
	echo Reading $i headers...
	echo " " >> $TEMPFILE
	echo " " >> $TEMPFILE
	echo "/* " >> $TEMPFILE
	echo "     Primitives from $i " >> $TEMPFILE
	echo "*/ " >> $TEMPFILE
 	grep -h "SERVICEPRIMITIVE_API" ../$i/*.h ../$i/include/*.h >> $TEMPFILE 2> /dev/null
done

rm $INPUTFILE
mv $TEMPFILE $INPUTFILE 

echo "Primitive header complete"
