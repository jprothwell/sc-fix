#ifndef _LOCALDATA_H
#define _LOCALDATA_H
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

typedef struct __LD_GPData  LDGP;
struct __LD_GPData // structure of LD data
{
	// integer data
	int32_t intData[MAX_INTEGER_RECORD];
	// byte data
	int32_t szDataLen[MAX_STRING_RECORD];		// length
	byte_t  szData[MAX_STRING_RECORD][MAX_STRING_RECORD_LENGTH];	// data
#ifdef SUPPORT_BLOCK_SP_SMS
	byte_t  CHPData[sizeof(SP_LIST)];
#endif
// 	int32_t GPLen[MAX_GP_RECORD + MAX_CACHED_UIP_NUM];		// length of GP data
// 	byte_t  GPData[MAX_GP_RECORD + MAX_CACHED_UIP_NUM][MAX_GP_RECORD_LENGTH];	// data of GP
	
};


typedef struct __LD  LD;  // main structure of LOCALDATA

struct __LD
{
	ILD  Vtbl;	// first element

	WEPAppData * pWep;

	LDGP ldGP;

// 	int32_t   soundDelay;	// sound delay(ms), 0 mean NO off control

#ifdef SUPPORT_FTP
	WCHAR_t  clientPath[MAX_FILENAME_LEN];
	WCHAR_t  clientFile[MAX_FILENAME_LEN];
	int32_t reTransferPoint;
	byte_t  selectedType;	// 0: Null; 1-File; 2-Dir
	int32_t fileLen ;
#endif
};

#endif 

