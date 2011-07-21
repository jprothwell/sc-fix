#include "stdio.h"
#include "qq_sym.h"
#include "jmetoc_geo_sym.h"
#include "msn_sym.h"


int main(int argc,char *argv[])
{
	FILE *f=NULL;
	int dataLen=sizeof(QQ_SYM);
	f = fopen("./QQ_SYM.bin","w+");
	fwrite(QQ_SYM,1,dataLen,f);
	fclose(f);
#ifdef _MMI_SUPPORT_JME_GEO_	

	dataLen=sizeof(JMETOC_GEO_SYM);
	f = fopen("./GEO_SYM.bin","w+");
	fwrite(JMETOC_GEO_SYM,1,dataLen,f);
	fclose(f);
	#endif
#ifdef _MMI_SUPPORT_JME_MSN_	
	dataLen=sizeof(MSN_SYM);
	f = fopen("./MSN_SYM.bin","w+");
	fwrite(MSN_SYM,1,dataLen,f);
	fclose(f);
#endif
	return 0;
}
