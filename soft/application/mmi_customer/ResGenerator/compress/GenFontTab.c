#include "stdio.h"

typedef unsigned char U8;
typedef unsigned int U32;
#include "l_w1_ch_gb2312.h"


void main(int argc,char *argv[])
{
	FILE *f=NULL;
	int dataLen=sizeof(GB14x14_Data);
	f = fopen("./GB14x14_Data.bin","w+");
	fwrite(GB14x14_Data,1,dataLen,f);
	fclose(f);
}
