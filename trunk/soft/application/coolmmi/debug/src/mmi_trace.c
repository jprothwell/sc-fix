/******************************************************************************************
			Defines some trace switches and functions for debug trace purpose.
*******************************************************************************************/

#include "stdio.h"
#include "string.h"
#include "ts.h"
#include "mmi_trace.h"
#include "cos.h"
#ifdef MMI_ON_WIN32 /*zhuoxz,2009-6-18*/
#include "mmi_data_types.h"
#endif
/*
Below are the trace switches. You can change it's value to control the output of any kind
of trace information.
**/
/////////////////////////////////////////
// Trace level for force output
 

/////////////////////////////////////////
// Trace switch for MMI more than one module related
PUBLIC BOOL g_sw_MAINENTRY = MMI_TRACE_LEVEL_1;
PUBLIC BOOL g_sw_DEBUG = MMI_TRACE_LEVEL_1;
PUBLIC BOOL g_sw_DUALSIM = MMI_TRACE_LEVEL_1;
PUBLIC BOOL g_sw_ADP_COMMON = MMI_TRACE_LEVEL_1;
PUBLIC BOOL g_sw_MMS = MMI_TRACE_LEVEL_1;
PUBLIC BOOL g_sw_WAP = MMI_TRACE_LEVEL_1;




/////////////////////////////////////////
// Trace level for system related
PUBLIC BOOL g_sw_ADP_POWERON = MMI_TRACE_LEVEL_2;
PUBLIC BOOL g_sw_ADP_POWEROFF = MMI_TRACE_LEVEL_2;
PUBLIC BOOL g_sw_FRAMEWORK = MMI_TRACE_LEVEL_2;
PUBLIC BOOL g_sw_POWERON = MMI_TRACE_LEVEL_2;
PUBLIC BOOL g_sw_POWEROFF = MMI_TRACE_LEVEL_2;
PUBLIC BOOL g_sw_SYSTEM = MMI_TRACE_LEVEL_2;
PUBLIC BOOL g_sw_PM = MMI_TRACE_LEVEL_2;
PUBLIC BOOL g_sw_trace_check = MMI_TRACE_LEVEL_2;  


/////////////////////////////////////////
// Trace level for network related
PUBLIC BOOL g_sw_ADP_NW = MMI_TRACE_LEVEL_3;
PUBLIC BOOL g_sw_NW = MMI_TRACE_LEVEL_3;
PUBLIC BOOL g_sw_ADP_GPRS = MMI_TRACE_LEVEL_3;






/////////////////////////////////////////
// Trace level for SIM related
PUBLIC BOOL g_sw_ADP_SIM = MMI_TRACE_LEVEL_4;
PUBLIC BOOL g_sw_ADP_SAT = MMI_TRACE_LEVEL_4;
PUBLIC BOOL g_sw_SAT = MMI_TRACE_LEVEL_4;
PUBLIC BOOL g_sw_ADP_SMU = MMI_TRACE_LEVEL_4;
PUBLIC BOOL g_sw_SMU = MMI_TRACE_LEVEL_4;

/////////////////////////////////////////
// Trace level for PHB related
PUBLIC BOOL g_sw_ADP_PHB = MMI_TRACE_LEVEL_5;
PUBLIC BOOL g_sw_PHB = MMI_TRACE_LEVEL_5;
PUBLIC BOOL g_sw_Ebr = MMI_TRACE_LEVEL_5;

/////////////////////////////////////////
// Trace level for CC related
PUBLIC BOOL g_sw_ADP_CC = MMI_TRACE_LEVEL_6;
PUBLIC BOOL g_sw_CC = MMI_TRACE_LEVEL_6;
PUBLIC BOOL g_sw_ADP_SS = MMI_TRACE_LEVEL_6;
PUBLIC BOOL g_sw_SS = MMI_TRACE_LEVEL_6;
PUBLIC BOOL g_sw_ADP_CALLLOG = MMI_TRACE_LEVEL_6;
PUBLIC BOOL g_sw_CALLLOG = MMI_TRACE_LEVEL_6;

/////////////////////////////////////////
// Trace level for SMS related
PUBLIC BOOL g_sw_ADP_SMS = MMI_TRACE_LEVEL_7;
PUBLIC BOOL g_sw_SMS = MMI_TRACE_LEVEL_7;

/////////////////////////////////////////
// Trace level for GDI/GUI related
PUBLIC BOOL g_sw_GDI = MMI_TRACE_LEVEL_8;
PUBLIC BOOL g_sw_GUI = MMI_TRACE_LEVEL_8;





/////////////////////////////////////////
// Trace level for Touch Panel and IM related
PUBLIC BOOL g_sw_TPL = MMI_TRACE_LEVEL_9;
PUBLIC BOOL g_sw_HandW = MMI_TRACE_LEVEL_9;
PUBLIC BOOL g_sw_IM = MMI_TRACE_LEVEL_9;

/////////////////////////////////////////
// Trace level for multimedia related
PUBLIC BOOL g_sw_MP3 = MMI_TRACE_LEVEL_10;
PUBLIC BOOL g_sw_MP4 = MMI_TRACE_LEVEL_10;
PUBLIC BOOL g_sw_Vidrec = MMI_TRACE_LEVEL_10;
PUBLIC BOOL g_sw_SREC = MMI_TRACE_LEVEL_10;
PUBLIC BOOL g_sw_MED_Vid = MMI_TRACE_LEVEL_10;
PUBLIC BOOL g_sw_MED_Img = MMI_TRACE_LEVEL_10;
PUBLIC BOOL g_sw_CAMERA = MMI_TRACE_LEVEL_10;
PUBLIC BOOL g_sw_MED = MMI_TRACE_LEVEL_10;
PUBLIC BOOL g_sw_CAM = MMI_TRACE_LEVEL_10;
PUBLIC BOOL g_sw_VdoRec = MMI_TRACE_LEVEL_10;







/////////////////////////////////////////
// Trace level for Audio related
PUBLIC BOOL g_sw_ADP_AUD = MMI_TRACE_LEVEL_11;
PUBLIC BOOL g_sw_MED_Aud = MMI_TRACE_LEVEL_11;
PUBLIC BOOL g_sw_AUDIO = MMI_TRACE_LEVEL_11;
PUBLIC BOOL g_sw_MIDI = MMI_TRACE_LEVEL_11;


/////////////////////////////////////////
// Trace level for Organizer related
PUBLIC BOOL g_sw_CALCULATOR = MMI_TRACE_LEVEL_12;
PUBLIC BOOL g_sw_ORGANIZER= MMI_TRACE_LEVEL_12;
PUBLIC BOOL g_sw_notepad = MMI_TRACE_LEVEL_12;
PUBLIC BOOL g_sw_ALARM= MMI_TRACE_LEVEL_12;
PUBLIC BOOL g_sw_GAME= MMI_TRACE_LEVEL_12;


/////////////////////////////////////////
// Trace level for Settings related
PUBLIC BOOL g_sw_SETTING = MMI_TRACE_LEVEL_13;
PUBLIC BOOL g_sw_WpSsPwon = MMI_TRACE_LEVEL_13;


/////////////////////////////////////////
// Trace level for common related
PUBLIC BOOL g_sw_IDLE = MMI_TRACE_LEVEL_14;
PUBLIC BOOL g_sw_LCD = MMI_TRACE_LEVEL_14;

/////////////////////////////////////////
// Trace level for FileSystem related
PUBLIC BOOL g_sw_NVRAM = MMI_TRACE_LEVEL_15;
PUBLIC BOOL g_sw_FMGR = MMI_TRACE_LEVEL_15;
PUBLIC BOOL g_sw_FMR = MMI_TRACE_LEVEL_15;


/////////////////////////////////////////
// Trace level for Others related
PUBLIC BOOL g_sw_ADP_BT = MMI_TRACE_LEVEL_16;//Renwy add for BT debug 2008/06/16
PUBLIC BOOL g_sw_BT = MMI_TRACE_LEVEL_1;//Renwy add for BT debug 2008/06/16
PUBLIC BOOL g_sw_SP = MMI_TRACE_LEVEL_16;
PUBLIC BOOL g_sw_DataMgr = MMI_TRACE_LEVEL_16;
















// add by zhuoxz,2009-3-24
#ifdef MMI_ON_WIN32
void dbg_TraceOpen(BOOL bTrace)
{
  g_sw_ADP_COMMON = bTrace;
  g_sw_ADP_POWERON = bTrace;
  g_sw_ADP_POWEROFF = bTrace;
  g_sw_ADP_CC = bTrace;
  g_sw_ADP_SMS = bTrace;
  g_sw_ADP_PHB = bTrace;
  g_sw_ADP_SIM = bTrace;
  g_sw_ADP_SS = bTrace;
  g_sw_ADP_CALLLOG = bTrace;
  g_sw_ADP_SAT = bTrace;
  g_sw_ADP_SMU = bTrace;
  g_sw_ADP_NW = bTrace;
  g_sw_ADP_AUD = bTrace;
  g_sw_ADP_BT = bTrace;//Renwy add for BT debug 2008/06/16

  g_sw_CC = bTrace;
  g_sw_SMS = bTrace;
  g_sw_PHB = bTrace;
  g_sw_SS = bTrace;
  g_sw_CALLLOG = bTrace;
  g_sw_SAT = bTrace;
  g_sw_SMU = bTrace;
  g_sw_NW = bTrace;
  g_sw_SP = bTrace;

  g_sw_GDI = bTrace;
  g_sw_GUI = bTrace;
  g_sw_FRAMEWORK = bTrace;
  g_sw_SETTING = bTrace;
  g_sw_CALCULATOR = bTrace;
  g_sw_POWERON = bTrace;
  g_sw_POWEROFF = bTrace;
  g_sw_IDLE = bTrace;
  g_sw_PM = bTrace;
  g_sw_IM = bTrace;
  g_sw_ORGANIZER= bTrace;
  g_sw_NVRAM = bTrace;
  g_sw_CAMERA = bTrace;
  g_sw_MED = bTrace;
  g_sw_MED_Aud = bTrace;
  g_sw_MED_Vid = bTrace;
  g_sw_MED_Img = bTrace;
  g_sw_BT = bTrace;//Renwy add for BT debug 2008/06/16
  g_sw_Vidrec = bTrace;//changed by zhangl

  g_sw_DUALSIM = bTrace;

/////////////////////////////////////////
// Trace switch for Platform Layer

  g_sw_MIDI = bTrace;
  g_sw_LCD = bTrace;
  g_sw_AUDIO = bTrace;
  g_sw_SYSTEM = bTrace;
  g_sw_FMGR = bTrace;
  g_sw_CAM = bTrace;
  g_sw_MP3 = bTrace;
  g_sw_MP4 = bTrace;
  g_sw_TPL = bTrace;
  g_sw_HandW = bTrace;
  g_sw_SREC = bTrace;//chenhe,20070628
  g_sw_FMR = bTrace;

  g_sw_VdoRec = bTrace;
  g_sw_Ebr = bTrace;
  g_sw_WpSsPwon = bTrace;
  g_sw_DataMgr = bTrace;
  g_sw_notepad = bTrace;
  g_sw_ALARM= bTrace;//gdm 090605
  g_sw_GAME= bTrace;//kecx added on 090817
}
#endif /* #ifdef MMI_ON_WIN32 */

#if 0
//add by wuzc for replace cos_sleep 
void MMI_Sleep(UINT32 nMillisecond)
{	

	UINT32 i;
	//10000 may be equal to 0.77 ms in jade xcpu
	//UINT32 cnt = nMillisecond*13000;
	//20800 may be equal to 1 ms in Greenstone xcpu, modified by kecx on 20090728
	UINT32 cnt = nMillisecond*20800;
	for(i = 0; i < cnt; i++)
		asm volatile ("nop");
	
}
#endif //0
void mmi_trace_hex(BOOL flag, int size, void *data)
{
    unsigned char buf[116];
    unsigned char temp[16];
    unsigned char *pStart;
    int left;
    int len;
    int i;
    
    //mmi_trace(flag, "\r\nExcute func : mmi_trace_hex()");
    
    if (data == NULL || size <= 0)
    {
        return;
    }
   
    sprintf(buf, "size = %d, buffer address = %X", size, (int)data);
    mmi_trace(flag, buf);

    pStart = (unsigned char *)data;
    left = size;

    while(left > 0)
    {
        len = left;
        if(len > 16)
        {
            len = 16;
        }
        buf[0] = 0;
        for(i = 0; i < len; i++)
        {
            sprintf(temp, "0X%02X, ", *(pStart + i));
            strcat(buf, temp);
        }
        pStart += len;
        left -= len;

		COS_Sleep(10); /* sleep 10ms to avoid the trace losing */
		
        mmi_trace(flag, "%s", buf);
    }

    buf[0] = '\r';
    buf[1] = '\n';
    buf[2] = '\0';
    mmi_trace(flag, buf);
}

//add by elisa for add msg trace on 2009-6-2
#ifdef MMI_MESSAGE_TRACE

#include "stack_common.h"
#include "stack_msgs.h"

#define 	MMIMesLong    	70
#define		ModeNameLength	16
#define 	NumSapFile     	9
#define  	L4A_LENTH  		(MSG_ID_L4A_CODE_END + 2)-MSG_ID_L4A_CODE_BEGIN
#define  	MMIAPI_LENTH	(MSG_ID_MMIAPI_CODE_END +2) -MSG_ID_MMIAPI_CODE_BEGIN
#define 	MED_LENTH		(MSG_ID_MED_CODE_END +2) - MSG_ID_MED_CODE_BEGIN
#define		WAP_LENGTH		(MSG_ID_WAP_CODE_END +2) -MSG_ID_WAP_CODE_BEGIN
#define   	J2ME_LENTH		(MSG_ID_MMI_PHB_JAVA_SEARCH_ENTRY_RSP + 2) -MSG_ID_J2ME_CODE_BEGIN
#define		FMT_LENGTH		(MSG_ID_FMT_MSG_CODE_END+2) - MSG_ID_FMT_MSG_CODE_BEGIN
#define		DRV_LENGTH		(MSG_ID_TP_PEN_DOWN_IND +2) - DRIVER_MSG_CODE_BEGIN
#define		NVRAM_LENGTH	(MSG_ID_NVRAM_CODE_END + 2) - MSG_ID_NVRAM_CODE_BEGIN

typedef struct{
	UINT32   MAPBegin;			
	UINT32   MesIDBegin;
	UINT32   MesIDEnd;

}MMIMEGLookUpStruct;

#define ADD_MAG(name)   #name

static char module_type_Name[][ModeNameLength]=
{
   ADD_MAG(MOD_NIL),
   ADD_MAG(MOD_NVRAM),
  // MOD_ADP ,
   ADD_MAG(MOD_L4C&MOD_ADP),
   ADD_MAG(MOD_L4C_2),   
   ADD_MAG(MOD_MED),
   ADD_MAG(MOD_MED_V),
   ADD_MAG(MOD_MMI),
   ADD_MAG(MOD_FMT),
   ADD_MAG(MOD_TP_TASK),
   ADD_MAG(MOD_TRANSPORT),
   ADD_MAG(MOD_J2ME),
   ADD_MAG(MOD_L1SP),
   ADD_MAG(MOD_WAP),
   ADD_MAG(MOD_JASYN),
   ADD_MAG(MOD_UEM),
   ADD_MAG(MOD_AS),
   ADD_MAG(MOD_VISUAL_HISR),
   ADD_MAG(MOD_JPEG_HISR),
   ADD_MAG(MOD_TST_READER),
   ADD_MAG(TOTAL_MODULES)	
};

static char MEGIDName[][MMIMesLong]=
{



  ADD_MAG(MSG_ID_INVALID_TYPE = 0),


#if defined(CSD_TARGET) || defined (CSD_KAL_MNT)

#ifndef __L1_STANDALONE__

   #include "mmi_sap.h"
   ADD_MAG(MSG_ID_MMI_CODE_CHECK_POINT),


   #include "mmiapi_sap.h"
   ADD_MAG(MSG_ID_MMIAPI_CODE_CHECK_POINT),

   #include "med_sap.h"
   ADD_MAG(MSG_ID_MED_CODE_CHECK_POINT),

   #include "wap_sap.h"  
   ADD_MAG(MSG_ID_WAP_CODE_CHECK_POINT),

   #include "j2me_sap.h" 
   ADD_MAG(MSG_ID_J2ME_CODE_CHECK_POINT),


#if ( !defined(FMT_NOT_PRESENT) && !defined(__L1_STANDALONE__) && !defined(__MAUI_BASIC__) )
   #include "fmt_sap.h"
   ADD_MAG(MSG_ID_FMT_CODE_CHECK_POINT),
#endif /* !FMT_NOT_PRESENT */

#endif /* !__L1_STANDALONE__ */

   #include "drv_sap.h"
   ADD_MAG(MSG_ID_DRV_CODE_CHECK_POINT),

   #include "nvram_sap.h"
   ADD_MAG(MSG_ID_NVRAM_CODE_CHECK_POINT),
   
#endif /* CSD_TARGET or CSD_KAL_MNT */

   ADD_MAG(MSG_ID_END)



};
#undef ADD_MAG



static MMIMEGLookUpStruct    sap[NumSapFile] ={
		{0,	0,	0},	//	return "MSG_ID_INVALID_TYPE";
		{1,	MSG_ID_L4A_CODE_BEGIN,		MSG_ID_L4A_CODE_END 	+ 1 },   //else if(nEvId >= MSG_ID_L4A_CODE_BEGIN && nEvId <= (MSG_ID_L4A_CODE_END + 1))
		{1 +L4A_LENTH ,  MSG_ID_MMIAPI_CODE_BEGIN,	MSG_ID_MMIAPI_CODE_END+ 1}, //else if(nEvId >= MSG_ID_MMIAPI_CODE_BEGIN&& nEvId <= (MSG_ID_MMIAPI_CODE_END +1))
		{1 +L4A_LENTH +  MMIAPI_LENTH , MSG_ID_MED_CODE_BEGIN,	MSG_ID_MED_CODE_END +1}, 	//else if(nEvId >= MSG_ID_MED_CODE_BEGIN&& nEvId <= (MSG_ID_MED_CODE_END +1))
		{1 +L4A_LENTH +  MMIAPI_LENTH +	MED_LENTH ,	MSG_ID_WAP_CODE_BEGIN,	MSG_ID_WAP_CODE_END +1},	//else if(nEvId >= MSG_ID_WAP_CODE_BEGIN&& nEvId <= (MSG_ID_WAP_CODE_END +1))
		{1 +L4A_LENTH +  MMIAPI_LENTH +	MED_LENTH +	WAP_LENGTH ,	MSG_ID_J2ME_CODE_BEGIN,		MSG_ID_MMI_PHB_JAVA_SEARCH_ENTRY_RSP + 1},	//else if(nEvId >= MSG_ID_J2ME_CODE_BEGIN&& nEvId <= (MSG_ID_MMI_PHB_JAVA_SEARCH_ENTRY_RSP + 1))
		{1 +L4A_LENTH +  MMIAPI_LENTH +	MED_LENTH +	WAP_LENGTH +	J2ME_LENTH ,	MSG_ID_FMT_MSG_CODE_BEGIN,	MSG_ID_FMT_MSG_CODE_END+1},	//else if(nEvId >= MSG_ID_FMT_MSG_CODE_BEGIN&& nEvId <= (MSG_ID_FMT_MSG_CODE_END+1))
		{1 +L4A_LENTH +  MMIAPI_LENTH +	MED_LENTH +	WAP_LENGTH +	J2ME_LENTH +	FMT_LENGTH	,	DRIVER_MSG_CODE_BEGIN,	MSG_ID_TP_PEN_DOWN_IND +1},	//else if(nEvId >= DRIVER_MSG_CODE_BEGIN&& nEvId <= (MSG_ID_TP_PEN_DOWN_IND +1))
		{1 +L4A_LENTH +  MMIAPI_LENTH +	MED_LENTH +	WAP_LENGTH +	J2ME_LENTH +	FMT_LENGTH	+	DRV_LENGTH , MSG_ID_NVRAM_CODE_BEGIN,	MSG_ID_NVRAM_CODE_END +1	}
};


PCSTR GetMEGName(UINT32 nEvId)
{
	uint32  i ;
	for(i = 0;  i < NumSapFile;i++)
	{
		if (nEvId >=sap[i].MesIDBegin && nEvId <= sap[i].MesIDEnd)
		{
			mmi_trace(g_sw_trace_check, TSTXT("##### fun:GetMEGName : Message begin:%s [ID : %d], this sap file begin:%d, current [ID: %d]\n"),
	                 	MEGIDName[(sap[i].MAPBegin)], sap[i].MesIDBegin, sap[i].MAPBegin, nEvId );
			return MEGIDName[nEvId-sap[i].MesIDBegin  + sap[i].MAPBegin];
		}
	}
	return "unknow message id name";
}


PCSTR GetModName(UINT32 modID)
{
    int k = (int)((sizeof(module_type_Name))/ModeNameLength);

	if(modID>=0 && modID< k)
	{
		return module_type_Name[modID];
	}
	else
	{
		return "unknow Modle id and name";

	}
}

#define  CheckMessage	ADD_MAG(MSG_ID_MMI_CC_SET_MAX_ACM_REQ),\
						ADD_MAG(MSG_ID_MMI_CODE_CHECK_POINT),\
						ADD_MAG(MSG_ID_MED_STARTUP_CNF),\
						ADD_MAG(MSG_ID_MEDIA_IMG_RESIZE_REQ),\
						ADD_MAG(MSG_ID_WPS_STATUS_QUERY_RSP),\
						ADD_MAG(MSG_ID_WAP_CODE_CHECK_POINT),\
						ADD_MAG(MSG_ID_FMT_CODE_CHECK_POINT),\
						ADD_MAG(MSG_ID_NVRAM_CODE_CHECK_POINT),\
						ADD_MAG(MSG_ID_MMI_EQ_CLOCK_TICK_IND),\
						ADD_MAG(MSG_ID_MMI_NW_ATTACH_IND)
						
void MMIMessageNameCheck()
{
	int ret =  0;
	int i,j = 0;
	char MesNameCheck[][MMIMesLong]=
		{	
			#define ADD_MAG(name)  #name
			CheckMessage
			#undef ADD_MAG

		};

	int msg_id_check[]=
		{
			#define ADD_MAG(name)  name
			CheckMessage
			#undef ADD_MAG
		};


	int k = (int)((sizeof(MesNameCheck))/MMIMesLong);
	
	mmi_trace(g_sw_trace_check, TSTXT("########## elisa MMI_MESSAGE trace check code begin: k = %d \n"),k);
	 
   	for (i = 0; i < k; i++ )
	{	
		 ret = strcmp(MesNameCheck[i], GetMEGName(msg_id_check[i]));
		 if (ret !=0)  
	 	 {     
	 		j++;
	 		mmi_trace(g_sw_trace_check, TSTXT("####### elisa error!!!!!!!!!ret = %d    there are some things error with message id , do not take it for reference\n"), ret);
			mmi_trace(g_sw_trace_check,TSTXT("###### message id :%d   right message: %s:, returned message: %s "),msg_id_check[i], MesNameCheck[i],GetMEGName(msg_id_check[i]));
	 	 }
		 
	}
	if (j ==0)    
	{
		mmi_trace(g_sw_trace_check,TSTXT("###### elisa MMi_MESSAGE_TRACE check passed! "));
	}
	mmi_trace(g_sw_trace_check, TSTXT("############ elisa MMI_MESSAGE trace check code end: \n"));

	return ;
}




#endif


#ifdef MMI_MEMLEAK_CHK

#define MAX_MEMLEAK_CHECK_NUMBER 120
#define MAX_FUNCTION_NAME_LENGTH 80
typedef struct{
UINT32 allocatedFlag;
UINT8 funtion[MAX_FUNCTION_NAME_LENGTH];
UINT32 line;
UINT32 address;
UINT32 size;
}MEM_ALLOCATED_INFO;

MEM_ALLOCATED_INFO mmiMemLeakChk[MAX_MEMLEAK_CHECK_NUMBER];

void addMemMalloc(char *function,int line,void * addr,UINT32 size)
{
	INT32 vi=0;
	while(vi<MAX_MEMLEAK_CHECK_NUMBER)
	{
		if(FALSE ==mmiMemLeakChk[vi].allocatedFlag)
		{
			mmiMemLeakChk[vi].allocatedFlag = TRUE;
			mmiMemLeakChk[vi].line = line;
			mmiMemLeakChk[vi].address = (UINT32)addr;
			mmiMemLeakChk[vi].size = size;
			strcpy(mmiMemLeakChk[vi].funtion,function);
			break;
		}
		vi++;
	}
	if(vi>=MAX_MEMLEAK_CHECK_NUMBER)
	{
		mmi_trace(1,"addMemMalloc,TOO MANY MALLOC IS USING!!!");
	}
}

void delMemMalloc(void *addr)
{
	INT32 vi=0;
	INT32 searched = 0;
	while(vi<MAX_MEMLEAK_CHECK_NUMBER)
	{
		if( (TRUE ==mmiMemLeakChk[vi].allocatedFlag) &&((UINT32)addr == mmiMemLeakChk[vi].address)&& (FALSE ==mmiMemLeakChk[vi+1].allocatedFlag))
		{
			mmiMemLeakChk[vi].allocatedFlag = FALSE;
			mmiMemLeakChk[vi].line = 0;
			mmiMemLeakChk[vi].address = 0;
			mmiMemLeakChk[vi].size = 0;
			memset(mmiMemLeakChk[vi].funtion,0x0,sizeof(mmiMemLeakChk[vi].funtion));
			searched = 1;
			break;
		}
		else if( (TRUE ==mmiMemLeakChk[vi].allocatedFlag) &&((UINT32)addr == mmiMemLeakChk[vi].address)&& (TRUE ==mmiMemLeakChk[vi+1].allocatedFlag))
		{
			mmiMemLeakChk[vi].allocatedFlag = TRUE;
			mmiMemLeakChk[vi].line = mmiMemLeakChk[vi+1].line;
			mmiMemLeakChk[vi].address =mmiMemLeakChk[vi+1].address ;
			mmiMemLeakChk[vi].size =mmiMemLeakChk[vi+1].size ;
			strcpy(mmiMemLeakChk[vi].funtion,mmiMemLeakChk[vi+1].funtion);
			searched = 1;
		}
		else if(searched)
		{
			if(TRUE ==mmiMemLeakChk[vi+1].allocatedFlag )
			{
				mmiMemLeakChk[vi].allocatedFlag = TRUE;
				mmiMemLeakChk[vi].line = mmiMemLeakChk[vi+1].line;
				mmiMemLeakChk[vi].address =mmiMemLeakChk[vi+1].address ;
				mmiMemLeakChk[vi].size =mmiMemLeakChk[vi+1].size ;				
				strcpy(mmiMemLeakChk[vi].funtion,mmiMemLeakChk[vi+1].funtion);
			}
			else
			{
				mmiMemLeakChk[vi].allocatedFlag = FALSE;
				mmiMemLeakChk[vi].line = 0;
				mmiMemLeakChk[vi].address = 0;
				mmiMemLeakChk[vi].size = 0;
				memset(mmiMemLeakChk[vi].funtion,0x0,sizeof(mmiMemLeakChk[vi].funtion));
			}
		}
		vi++;	
	}

}
void mmi_memleakCHK(void)
{
	INT32 vi=0;
	mmi_trace(1,"mmi_memleakCHK,START:#######################");

	while(vi<MAX_MEMLEAK_CHECK_NUMBER)
	{
		if(TRUE ==mmiMemLeakChk[vi].allocatedFlag)
		{
			mmi_trace(1,"%d: MEMORY LEAK MAY BE IN FUCTION :%s,LINE:%d,size is %d",vi,mmiMemLeakChk[vi].funtion,mmiMemLeakChk[vi].line,mmiMemLeakChk[vi].size);
		}
		vi++;
	}
	mmi_trace(1,"mmi_memleakCHK,END:#########################");

}
#endif 
void *MMI_Malloc(char *file,char *function,int line,UINT32 size)
{
	u32  sxr_CallerAdd;
	void *tp = NULL;
	COS_GET_RA(&sxr_CallerAdd);
	tp = COS_MALLOC(size);

#ifdef MMI_MEMLEAK_CHK	
	addMemMalloc(function,line,tp,size);
	mmi_trace(1,"chenheMalloc,file is %s,function is %s,line is %d,size is %d,tp is %p",file,function,line,size,tp);
#endif	
       if(tp)
   	{
   	  COS_UpdateMemCallAdd(tp,sxr_CallerAdd);
   	}
	return tp;
}

void MMI_Free(void *addr)
{
	COS_FREE(addr);
#ifdef MMI_MEMLEAK_CHK	
	delMemMalloc(addr);
#endif
	
}

