
#include <cfw_sim_prv.h>
#ifndef _CFW_SIM_PBK_H_
#define _CFW_SIM_PBK_H_




#define API_GETPBK						0
#define API_LISTPBK						1
#define API_GETSTORAGE					2


typedef struct _SIM_SM_PBK_STATE
{

	UINT8 nPreState;
	UINT8 nCurrState;
	UINT8 nNextState;
	UINT8 pad;
}SIM_SM_PBK_STATE;

typedef struct _SIM_SM_INFO_ADDPBK
{  
  //Modify by lixp at 2008-0303
  //From 30 to 38
  //Because Arabia card data length is 0x22
  //Modify by lixp at 20091225
  //From 38 to 42
  //Because BeeLine card data length is 0x28
  //From 42 to 80
  //Because Etisalat card data length is 0x2D
  UINT8 pData[80];                  // send to scl  
	UINT8 nDataSize;                  // record length
	UINT8 pNum[10];                   // find free record
	UINT8 nNumType;                   //number's type
	UINT8 nNumSize;                   //number's size
	UINT8 nNameSize;
	UINT8 nCurrentFile;               //elem file type
	UINT8 nAddRecInd;                 // index which to add record
	UINT8 nCoutOfAdd;
	UINT8 pad;
	SIM_SM_PBK_STATE nState;
}SIM_SM_INFO_ADDPBK;




// SimDeletePbkEntry's struct
typedef struct _SIM_SM_INFO_DELEPBK
{  
	UINT8 nPadFDataSize;
	UINT8 nCurrentFile;            //elem file type
	UINT8 nDeleRecInd;              // index which to add record
	UINT8 pad;
	SIM_SM_PBK_STATE nState;
}SIM_SM_INFO_DELEPBK;

// SimGetPbkEntry's struct
typedef struct _SIM_SM_INFO_GETPBK
{  
	UINT8 nRecordSize;             
	UINT8 nCurrentFile;            //elem file type
	UINT8 nCurrRecInd;  
	UINT8 pad;
	SIM_SM_PBK_STATE nState;
}SIM_SM_INFO_GETPBK;

// SimListPbkEntries's struct
typedef struct _SIM_SM_INFO_LISTPBK
{  
	UINT8 nRecordSize;
	UINT8 nCurrentFile;            //elem file type
	UINT8 nCurrRecInd;             
	UINT8 nStartRecInd;              // index which to start record
    UINT8 nEndRecInd;              // index which to end record
	UINT8 nStructNum;
	UINT32 nCurrentP;
	UINT8 pad[2];
	SIM_SM_PBK_STATE nState;
}SIM_SM_INFO_LISTPBK;

typedef struct _SIM_SM_INFO_LISTCOUNTPBK
{  
	UINT8 nRecordSize;
	UINT8 nCurrentFile;            //elem file type
	UINT8 nCurrRecInd;             
	UINT8 nStartRecInd;              // index which to start record
	UINT8 nCount;
	UINT8 nStructNum;
	UINT32 nCurrentP;
	UINT8 pad[2];
	SIM_SM_PBK_STATE nState;
}SIM_SM_INFO_LISTCOUNTPBK;

// SimGetPbkStrorageInfo's struct
typedef struct _SIM_SM_INFO_STORAGEINFO
{  
	UINT8 nCurrentFile;            //elem file type
	UINT8 nDataSize;               //record's size 
	UINT8 nTotalSize[2];  
	SIM_SM_PBK_STATE nState;
}SIM_SM_INFO_STORAGEINFO;

typedef struct _SIM_SM_INFO_STORAGE
{                  
	UINT8 nCurrentFile;            //elem file type
	UINT8 nNotUsedIndex; 
	UINT8 nCurrentIndex;
	UINT16 nTotalIndex;               //record's size 
	UINT8 nDataSize;               //record's size
	UINT8 nTotalSize[2]; 
	SIM_SM_PBK_STATE nState;
}SIM_SM_INFO_STORAGE;

typedef struct _SIM_SM_INFO_INIT
{                  
	SIM_SM_PBK_STATE nState;	
}SIM_SM_INFO_INIT;

UINT32 CFW_SimPBKAddProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimPBKDelProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimPBKEntryProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimPBKListEntryProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimPBKListCountEntryProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimPBKStorageProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimPBKStorageInfoProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimInitProc (HAO hAo,CFW_EV* pEvent);

UINT32 CFW_SimPBKListCount_TimeOut();



#define CFW_SIM_REMAINLENGTH			14

#define CFW_SIM_PBK_IDLE             		0
#define CFW_SIM_PBK_READELEMENT             1
#define CFW_SIM_PBK_READRECORD       		2
#define CFW_SIM_PBK_SEEK					3
#define CFW_SIM_PBK_UPDATERECORD 			4
#define CFW_SIM_PBK_READBINARY				5
#define CFW_SIM_PBK_RESET					6


#endif // _CFW_SIM_PBK_H_

