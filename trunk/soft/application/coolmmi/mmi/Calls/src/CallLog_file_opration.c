/*****************************************************************************
*  Copyright Statement:
*  --------------------

*
*****************************************************************************/
//  implementation of the Call Mgmt Incoming Handling.
//
//////////////////////////////////////////////////////////////////////
 
#if 0
#ifdef MMI_ON_WIN32
//#include "stdafx.h"
#include "string.h"
#include "stdlib.h"
#include "calllog_file_opration.h"
#include "stdio.h"
//#include "callmgmtstruct.h"
//#include "memallocator.h"
#else
#include "stdc.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "debuginitdef.h"
#include "wgui.h"
#include "globalstruct.h"
#include "mmi_data_types.h"
#include "wgui_categories.h"
#include "historygprot.h"
#include "idleappprot.h"
#include "custdatares.h"
#include "protocolevents.h"
#include "eventsgprot.h"
#include "datetimetype.h"
#include "thrdosinterface.h"
#include "osthreadinterfacegprot.h"
#include "custmenures.h"
#include "miscfunctions.h"
//jl #include "maintaskgexdcl.h"
#include "queuegprot.h"
#include "taskinit.h"
#include "voicememodef.h"
#include "voicememotype.h"
#include "voicememoprot.h"
#include "settingprofile.h"
#include "gpioinc.h"
#include "alarmdef.h"
#include "calllog_file_opration.h"

#ifdef MMI_ON_HARDWARE_P
   #include "mmi_msg_struct.h"
   #include "l4dr.h"
   #include "custom_util.h"
#endif
#endif

#define TOT_SIZE_OF_DIALED_LIST  MAX_PHB_LN_ENTRY
#define TOT_SIZE_OF_MISSED_LIST  MAX_PHB_LN_ENTRY
#define TOT_SIZE_OF_RECVD_LIST   MAX_PHB_LN_ENTRY
#define TOT_SIZE_OF_CALLS_LIST   MAX_PHB_LN_ENTRY

void CallHistoryWriteToFile(kal_uint16 FileType,char Isupdate) ;  
void CallHistoryReadFromFile(kal_uint16 FileType);


mmi_phb_set_last_number_rsp_struct *gSetRspStruct;
mmi_phb_del_last_number_rsp_struct *gDelRspEntries;      
mmi_phb_get_last_number_rsp_struct *gGetRspEntries;
kal_uint8 DialedCallCount =0;
kal_uint8 RecvdCallCount=0;
kal_uint8 MissedCallCount=0;
FILE *fpCallHistory;
void * gCallHistoryBuffer = NULL;
mmi_phb_get_last_number_rsp_struct  phb_ln_save_old[3];    /*用来保存已拨，未接，已接记录*/
//mmi_cc_call_ring_ind_struct *gIncomingCallIndStruct;
/*初始化用来保存已拨，未接，已接记 录的全局变量*/
void Initialization_phb_ln(void)
{
	int i;
	for(i=0;i<3;i++)
      {
	    memset(&phb_ln_save_old[i],0,sizeof(mmi_phb_get_last_number_rsp_struct));
	    phb_ln_save_old[i].type=i;
	    
      }
}
/*将response设置到全局变量中去*/
void SetBufferForCallHistory( void * dataBuf )
{
	gCallHistoryBuffer = dataBuf ;
}
/* 返回set_last_number_rsp*/
void * SetLastNoRsp(mmi_phb_set_last_number_rsp_struct* SetRspStruct)
{
       char Isupdate=1;
	mmi_phb_set_last_number_req_struct *setLastNoReq = (mmi_phb_set_last_number_req_struct *)gCallHistoryBuffer;
       gSetRspStruct=SetRspStruct;
     
	gSetRspStruct->result.flag=0;
	gSetRspStruct->result.cause=0;

	gSetRspStruct->list[0].tel.type=setLastNoReq->entry.tel.type;
	//No_list set in the end for code optimization

	gSetRspStruct->list[0].tel.length=setLastNoReq->entry.tel.length;
	if(strlen(setLastNoReq->entry.tel.number)!=0)
		strcpy(gSetRspStruct->list[0].tel.number,setLastNoReq->entry.tel.number);
	else
		strcpy(gSetRspStruct->list[0].tel.number,"NULL");
	gSetRspStruct->list[0].alpha_id.name_length=setLastNoReq->entry.alpha_id.name_length;
	gSetRspStruct->list[0].alpha_id.name_dcs=setLastNoReq->entry.alpha_id.name_dcs;
	if(strlen(setLastNoReq->entry.alpha_id.name)!=0)
		strcpy(gSetRspStruct->list[0].alpha_id.name,setLastNoReq->entry.alpha_id.name);
	else
		strcpy(gSetRspStruct->list[0].alpha_id.name,"NULL");
	gSetRspStruct->list[0].time.rtc_sec=setLastNoReq->entry.time.rtc_sec;
	gSetRspStruct->list[0].time.rtc_min=setLastNoReq->entry.time.rtc_min;
	gSetRspStruct->list[0].time.rtc_hour=setLastNoReq->entry.time.rtc_hour;
	gSetRspStruct->list[0].time.rtc_day=setLastNoReq->entry.time.rtc_day;
	gSetRspStruct->list[0].time.rtc_mon=setLastNoReq->entry.time.rtc_mon;
	gSetRspStruct->list[0].time.rtc_wday=setLastNoReq->entry.time.rtc_wday;
	gSetRspStruct->list[0].time.rtc_year=setLastNoReq->entry.time.rtc_year;
	gSetRspStruct->list[0].count= setLastNoReq->entry.count;
	//gSetRspStruct->list[0].count= 1; //setLastNoReq->entry.count;

	if(setLastNoReq->type==0)
	{
              if (!Is_update_ln_number(setLastNoReq->index,setLastNoReq->type,gSetRspStruct))
              {
		         ++DialedCallCount;
		         Isupdate=0;
              }
		gSetRspStruct->no_list=DialedCallCount;
		CallHistoryWriteToFile(0,Isupdate);
	
	}else if(setLastNoReq->type==1)
	{	

               if (!Is_update_ln_number(setLastNoReq->index,setLastNoReq->type,gSetRspStruct))
               {
		           ++MissedCallCount;
		           Isupdate=0;
               }
		gSetRspStruct->no_list=MissedCallCount;
		CallHistoryWriteToFile(1,Isupdate);

	}else if(setLastNoReq->type==2)
	{
              if (!Is_update_ln_number(setLastNoReq->index,setLastNoReq->type,gSetRspStruct))
             {
		        ++RecvdCallCount;
		         Isupdate=0;
              }
		gSetRspStruct->no_list=RecvdCallCount;
		CallHistoryWriteToFile(2,Isupdate);
		
	}
	
	return (void *)gSetRspStruct;
}
int GetCurrentCount(int type)  /*通过type返回当前对应type 的记录条数*/
{
     int count=0;
	
	if(type==0)
       {
                   count=DialedCallCount;
       }else if(type==1)
       {
                    count=MissedCallCount;
       }else if(type==2)
       {
                    count=RecvdCallCount;
       }
       return count;
}
/* 返回get_last_number_rsp*/
void * GetLastNoRsp(mmi_phb_get_last_number_rsp_struct* GetRspEntries)
{
	mmi_phb_get_last_number_req_struct *getLastNoReq = (mmi_phb_get_last_number_req_struct *)gCallHistoryBuffer;
	gGetRspEntries=GetRspEntries; 

	if(getLastNoReq->type==0)
	{
		CallHistoryReadFromFile(0);
		DialedCallCount = gGetRspEntries->no_list;
	}else if(getLastNoReq->type==1)
	{
		CallHistoryReadFromFile(1);
		MissedCallCount = gGetRspEntries->no_list;
	}else if(getLastNoReq->type==2)
	{
		CallHistoryReadFromFile(2);
		RecvdCallCount = gGetRspEntries->no_list;
	}
	gGetRspEntries->more_data = 0;
	return (void *)gGetRspEntries;
}
/*删除一条记录*/
/*方法:在全局变量里过滤掉index的记录*/
void DeleteLastNoRsp(int FileType,int index)  
{
   int i,count;
   S8 str[200];
   int len;
   if(FileType==0)
   {
        count=DialedCallCount;
   		fpCallHistory =FS_Open("DialedCallHistory.txt", "w"); //FS_Open("DialedCallHistory.txt", "a+");
   }else if(FileType==1)
   {
        count=MissedCallCount;
   		fpCallHistory = FS_Open("MissedCallHistory.txt", "w");//FS_Open("MissedCallHistory.txt", "a+");
   }else if(FileType==2)
   {
        count=RecvdCallCount;
   		fpCallHistory =FS_Open("RecvdCallHistory.txt", "w");//FS_Open("RecvdCallHistory.txt", "a+");
   }
    for(i=0;i<count;i++)
    {
            if(index==i)
            	  continue;
            if(phb_ln_save_old[FileType].list[i].tel.number[0]=='\0')
            	 strcpy(gSetRspStruct->list[0].tel.number,"NULL");  
            if(phb_ln_save_old[FileType].list[i].alpha_id.name[0]=='\0')   
               strcpy(phb_ln_save_old[FileType].list[i].alpha_id.name,"NULL");
            
           sprintf(str, "%d,%d,%d,%d,%d,%s,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d\n",
       		phb_ln_save_old[FileType].result.flag,
       		phb_ln_save_old[FileType].result.cause,
       		phb_ln_save_old[FileType].no_list,
       		phb_ln_save_old[FileType].list[i].tel.type,
       		phb_ln_save_old[FileType].list[i].tel.length,
       		phb_ln_save_old[FileType].list[i].tel.number,
       		phb_ln_save_old[FileType].list[i].alpha_id.name_length,
       		phb_ln_save_old[FileType].list[i].alpha_id.name_dcs,
       		phb_ln_save_old[FileType].list[i].alpha_id.name,
       		phb_ln_save_old[FileType].list[i].time.rtc_sec,
       		phb_ln_save_old[FileType].list[i].time.rtc_min,
       		phb_ln_save_old[FileType].list[i].time.rtc_hour,
       		phb_ln_save_old[FileType].list[i].time.rtc_day,
       		phb_ln_save_old[FileType].list[i].time.rtc_mon,
       		phb_ln_save_old[FileType].list[i].time.rtc_wday,
       		phb_ln_save_old[FileType].list[i].time.rtc_year,
       		phb_ln_save_old[FileType].list[i].count);
       	//fputs(str, fpCallHistory);
       	#ifdef MMI_ON_WIN32
       	    fwrite(str,strlen(str),1,fpCallHistory);
             #else
                 FS_Write(fpCallHistory,str,strlen(str),&len);
             #endif//
     }
 	FS_Close(fpCallHistory);	
}
/*删除记录的入口，通过对no_data做判断是删一条还是全删*/	
void * DeleteAllCallsRsp(mmi_phb_del_last_number_rsp_struct * DelRspEntries)  
{
	mmi_phb_del_last_number_req_struct *delNoReq = (mmi_phb_del_last_number_req_struct*)gCallHistoryBuffer;
	gDelRspEntries=DelRspEntries;
	//printf(" ****DeleteAllCallsRsp******* " );
     
	//printf(" ****DeleteAllCallsRsp******* " );

	if(delNoReq->type==0)
	{
		if (delNoReq->no_data == 1)
		{
                     DeleteLastNoRsp(delNoReq->type,delNoReq->index);
			DialedCallCount--;
		}
		else
		{
			fpCallHistory =FS_Open("DialedCallHistory.txt", "w"); //FS_Open("DialedCallHistory.txt", "w");
			DialedCallCount = 0;
		      FS_Close(fpCallHistory);	
		}
	}else if(delNoReq->type==1)
	{
		if (delNoReq->no_data == 1)
		{ 
                     DeleteLastNoRsp(delNoReq->type,delNoReq->index );
			MissedCallCount--;
		}
		else
		{
			fpCallHistory = FS_Open("MissedCallHistory.txt", "w");//FS_Open("MissedCallHistory.txt", "w");
			MissedCallCount = 0;
			FS_Close(fpCallHistory);	
		}
	}else if(delNoReq->type==2)
	{
		if (delNoReq->no_data == 1)
		{
                     DeleteLastNoRsp(delNoReq->type,delNoReq->index);
			RecvdCallCount--;
		}
		else
		{
			fpCallHistory = FS_Open("RecvdCallHistory.txt", "w");//FS_Open("RecvdCallHistory.txt", "w");
			RecvdCallCount = 0;
			FS_Close(fpCallHistory);	
		}
	}
	return (void *)gDelRspEntries;
}

int find_oldest_calllog(int type, int count)   /*找到当前calllog中时间最前的记录*/
{
  int i;
  int oldestindex=0;
  MYTIME oldesttime,currenttime,diff;
  oldesttime.nYear=phb_ln_save_old[type].list[0].time.rtc_year;
  oldesttime.nMonth=phb_ln_save_old[type].list[0].time.rtc_mon;
  oldesttime.nDay=phb_ln_save_old[type].list[0].time.rtc_day;
  oldesttime.nHour=phb_ln_save_old[type].list[0].time.rtc_hour;
  oldesttime.nMin=phb_ln_save_old[type].list[0].time.rtc_min;
  oldesttime.nSec=phb_ln_save_old[type].list[0].time.rtc_sec;
  for(i=1;i<count;i++)
  {    
       currenttime.nYear=phb_ln_save_old[type].list[i].time.rtc_year;
       currenttime.nMonth=phb_ln_save_old[type].list[i].time.rtc_mon;
       currenttime.nDay=phb_ln_save_old[type].list[i].time.rtc_day;
       currenttime.nHour=phb_ln_save_old[type].list[i].time.rtc_hour;
       currenttime.nMin=phb_ln_save_old[type].list[i].time.rtc_min;
       currenttime.nSec=phb_ln_save_old[type].list[i].time.rtc_sec;
       if( CompareTime(currenttime,oldesttime,&diff)==TIME_LESS)
      	{
      	  memcpy(&oldesttime,&currenttime,sizeof(MYTIME));
      	  oldestindex=i;
       }
       
   }
  return oldestindex;
}
/*判断是否更新一条，重复号码，以及到达最大数量。并更新全局变量*/
int Is_update_ln_number(int index,int type,mmi_phb_set_last_number_rsp_struct * set_last_number_rsp)
{
	int i,count;
	int tempcount;
	int oldestindex=0;
       count=GetCurrentCount(type);

        if(index!=0xff&&(index<count))     /*更新指定index 的calllog*/
	{
              memcpy(&phb_ln_save_old[type].list[index],&set_last_number_rsp->list[0],sizeof(l4c_phb_ln_entry_struct) );
	       return 1;
	}
        else                                            /*更新重复号码*/
	{
	    for(i=0;i<count;i++)
           {
                // if(phb_ln_save_old[type].list[i].tel.number[0]=='\0')
                //     strcpy(phb_ln_save_old[type].list[i].tel.number,"NULL");  
                 if(strcmp(phb_ln_save_old[type].list[i].tel.number,set_last_number_rsp->list[0].tel.number)==0)
                 {
                      tempcount=phb_ln_save_old[type].list[i].count+1;//phb_ln_save_old[type].list[i].count= phb_ln_save_old[type].list[i].count+1;
                      memcpy(&phb_ln_save_old[type].list[i],&set_last_number_rsp->list[0],sizeof(l4c_phb_ln_entry_struct) );
                      phb_ln_save_old[type].list[i].count=tempcount;
                      return 1;
                 }
           }
	 }
      
       if (count==TOT_SIZE_OF_DIALED_LIST)   /*到达最大数量,覆盖最老的记录 */
      	{
      	     oldestindex=find_oldest_calllog(type,count);
            memcpy(&phb_ln_save_old[type].list[oldestindex],&set_last_number_rsp->list[0],sizeof(l4c_phb_ln_entry_struct) );
	     return 1;
      	     
      	}
       

	return 0;
}

void CallHistoryWriteToFile(kal_uint16 FileType,char Isupdate)  /*将calllog写入文件并存入全局变量中*/
{
	S8 str[200];
	int i,count;
	int len;
	if (Isupdate)    /*如果只是更新或已经到达最大数量则只要将全局变量中的记录存入文件即可*/
	{     
	    #ifndef debug_test 
	      if(FileType==0)
             {
                           count=DialedCallCount;
       			fpCallHistory =FS_Open("DialedCallHistory.txt", "w"); //FS_Open("DialedCallHistory.txt", "a+");
       	}else if(FileType==1)
       	{
                             count=MissedCallCount;
       			fpCallHistory = FS_Open("MissedCallHistory.txt", "w");//FS_Open("MissedCallHistory.txt", "a+");
       	}else if(FileType==2)
       	{
                           count=RecvdCallCount;
       			fpCallHistory =FS_Open("RecvdCallHistory.txt", "w");//FS_Open("RecvdCallHistory.txt", "a+");
       	}
       	
       	if(fpCallHistory == NULL) {
       		exit(0);
       	}
             // for(i=0;i<TOT_SIZE_OF_DIALED_LIST;i++)
             for(i=0;i<count;i++)
             {
                     if(phb_ln_save_old[FileType].list[i].tel.number[0]=='\0')
                     	 strcpy(gSetRspStruct->list[0].tel.number,"NULL");  
                    if(phb_ln_save_old[FileType].list[i].alpha_id.name[0]=='\0')
                         
                        strcpy(phb_ln_save_old[FileType].list[i].alpha_id.name,"NULL");
                  
                  
       		sprintf(str, "%d,%d,%d,%d,%d,%s,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d\n",
       		phb_ln_save_old[FileType].result.flag,
       		phb_ln_save_old[FileType].result.cause,
       		phb_ln_save_old[FileType].no_list,
       		phb_ln_save_old[FileType].list[i].tel.type,
       		phb_ln_save_old[FileType].list[i].tel.length,
       		phb_ln_save_old[FileType].list[i].tel.number,
       		phb_ln_save_old[FileType].list[i].alpha_id.name_length,
       		phb_ln_save_old[FileType].list[i].alpha_id.name_dcs,
       		phb_ln_save_old[FileType].list[i].alpha_id.name,
       		phb_ln_save_old[FileType].list[i].time.rtc_sec,
       		phb_ln_save_old[FileType].list[i].time.rtc_min,
       		phb_ln_save_old[FileType].list[i].time.rtc_hour,
       		phb_ln_save_old[FileType].list[i].time.rtc_day,
       		phb_ln_save_old[FileType].list[i].time.rtc_mon,
       		phb_ln_save_old[FileType].list[i].time.rtc_wday,
       		phb_ln_save_old[FileType].list[i].time.rtc_year,
       		phb_ln_save_old[FileType].list[i].count);
              	//fputs(str, fpCallHistory);
       	       FS_Write(fpCallHistory,str,strlen(str),&len);
             	}
       	FS_Close(fpCallHistory);
         #endif
	}
	else       /*如果增加一条记录则直接以追加的方式写入文件*/
       {
              #ifdef debug_test 
       	    count=GetCurrentCount(FileType);
       	    memcpy(&phb_ln_save_old[FileType].list[count-1],&gSetRspStruct->list[0],sizeof(l4c_phb_ln_entry_struct) );
       	#else
       	if(FileType==0)
       	{
       			fpCallHistory =FS_Open("DialedCallHistory.txt", "a+");
       	}else if(FileType==1)
       	{
       			fpCallHistory = FS_Open("MissedCallHistory.txt", "a+");
       	}else if(FileType==2)
       	{
       			fpCallHistory =FS_Open("RecvdCallHistory.txt", "a+");
       	}
       	
       	if(fpCallHistory == NULL) {
       		exit(0);
       	}
       	sprintf(str, "%d,%d,%d,%d,%d,%s,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d\n",
       	gSetRspStruct->result.flag,
       	gSetRspStruct->result.cause,
       	gSetRspStruct->no_list,
       	gSetRspStruct->list[0].tel.type,
       	gSetRspStruct->list[0].tel.length,
       	gSetRspStruct->list[0].tel.number,
       	gSetRspStruct->list[0].alpha_id.name_length,
       	gSetRspStruct->list[0].alpha_id.name_dcs,
       	gSetRspStruct->list[0].alpha_id.name,
       	gSetRspStruct->list[0].time.rtc_sec,
       	gSetRspStruct->list[0].time.rtc_min,
       	gSetRspStruct->list[0].time.rtc_hour,
       	gSetRspStruct->list[0].time.rtc_day,
       	gSetRspStruct->list[0].time.rtc_mon,
       	gSetRspStruct->list[0].time.rtc_wday,
       	gSetRspStruct->list[0].time.rtc_year,
       	gSetRspStruct->list[0].count);      	
       	//fputs(str, fpCallHistory);
       	FS_Write(fpCallHistory,str,strlen(str),&len);
       	FS_Close(fpCallHistory);
       	#endif
	}
}
int psi_fgets(S8 *str,int len,FILE *file)
{
    int count=0;
    char currentchar[2];
    int read_len;
	do
	{
	      
		if( FS_Read(file,currentchar,1,&read_len))
			*(str++)=currentchar[0];
		else
			return 0;
		count++;
	}
	while(currentchar[0]!='\n'&&(count<len));
      *str='\0';
      return 1;
}
void CallHistoryReadFromFile(kal_uint16 FileType) 
{
	S8 str[100];
	S8 *token;
	S8 index = 0;
       #ifdef debug_test 
       int count=0;
       int i;
       	count=GetCurrentCount(FileType);
       	 gGetRspEntries->result.flag =0;
    		gGetRspEntries->result.cause = 0;
       	for(i=0;i<count;i++)
       	{
           	  //  gGetRspEntries->result.flag =0;
    		//	gGetRspEntries->result.cause = 0;
    			gGetRspEntries->list[index].tel.type = phb_ln_save_old[FileType].list[index].tel.type;
    	 		gGetRspEntries->list[index].tel.length =  phb_ln_save_old[FileType].list[index].tel.length ;
    			if(!strcmp(phb_ln_save_old[FileType].list[index].tel.number,"NULL"))
    			{
    				gGetRspEntries->list[index].tel.number[0] ='\0';
    			}
    			else
    			{
    				strcpy((S8 *)gGetRspEntries->list[index].tel.number, phb_ln_save_old[FileType].list[index].tel.number);
    			}
    	 		gGetRspEntries->list[index].alpha_id.name_length =phb_ln_save_old[FileType].list[index].alpha_id.name_length;			
    	 		gGetRspEntries->list[index].alpha_id.name_dcs = phb_ln_save_old[FileType].list[index].alpha_id.name_dcs;
    			if(!strcmp(phb_ln_save_old[FileType].list[index].alpha_id.name,"NULL"))
    				gGetRspEntries->list[index].alpha_id.name[0] ='\0';
    			else
    				strcpy((S8 *)gGetRspEntries->list[index].alpha_id.name, phb_ln_save_old[FileType].list[index].alpha_id.name);
    	 		gGetRspEntries->list[index].time.rtc_sec =phb_ln_save_old[FileType].list[index].time.rtc_sec;
    	 		gGetRspEntries->list[index].time.rtc_min =phb_ln_save_old[FileType].list[index].time.rtc_min;
    	 		gGetRspEntries->list[index].time.rtc_hour = phb_ln_save_old[FileType].list[index].time.rtc_hour;
    	 		gGetRspEntries->list[index].time.rtc_day = phb_ln_save_old[FileType].list[index].time.rtc_day;
    	 		gGetRspEntries->list[index].time.rtc_mon = phb_ln_save_old[FileType].list[index].time.rtc_mon;
    	 		gGetRspEntries->list[index].time.rtc_wday =phb_ln_save_old[FileType].list[index].time.rtc_wday;
    	 		gGetRspEntries->list[index].time.rtc_year = phb_ln_save_old[FileType].list[index].time.rtc_year;
    	 		gGetRspEntries->list[index].count = phb_ln_save_old[FileType].list[index].count;
    			index++;
		}
		gGetRspEntries->no_list = index;
		phb_ln_save_old[FileType].no_list= index;
         
       	if(FileType==0)
		{
			gGetRspEntries->type=0;
		}
		else if(FileType==1)
		{
			gGetRspEntries->type=1;
		}
		else if(FileType==2)
		{
			gGetRspEntries->type=2;
		}
     #else
	if(FileType==0)
	{
		fpCallHistory =FS_Open("DialedCallHistory.txt", "r");
	}else if(FileType==1)
	{
		fpCallHistory =FS_Open("MissedCallHistory.txt", "r");
	}else if(FileType==2)
	{
		fpCallHistory = FS_Open("RecvdCallHistory.txt", "r");
	}

	if(fpCallHistory == NULL) 
	{
		gGetRspEntries->result.flag = 0;
		gGetRspEntries->result.cause = 0;
		gGetRspEntries->no_list = index;
		if(FileType==0)
		{
			gGetRspEntries->type=0;
		}
		else if(FileType==1)
		{
			gGetRspEntries->type=1;
		}
		else if(FileType==2)
		{
			gGetRspEntries->type=2;
		}
		return;
	}
	else
	{
	         while(psi_fgets(str,91,fpCallHistory))  /*91 为一条记录的大小(89+2)结尾为'\n'*/		     
	         {
			str[strlen(str) - 1]='\0';
			token = strtok(str, ",");
			gGetRspEntries->result.flag = atoi(token);
			token = strtok(NULL, ",");
			gGetRspEntries->result.cause = atoi(token);
			token = strtok(NULL, ",");
	 		//gSetGetRspEntries->no_list = atoi(token);
			token = strtok(NULL, ",");
			gGetRspEntries->list[index].tel.type = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].tel.length = atoi(token);
			token = strtok(NULL, ",");
			if(!strcmp(token,"NULL"))
			{
				gGetRspEntries->list[index].tel.number[0] ='\0';
			}
			else
			{
				strcpy((S8 *)gGetRspEntries->list[index].tel.number, token);
			}
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].alpha_id.name_length = atoi(token);			
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].alpha_id.name_dcs = atoi(token);
			token = strtok(NULL, ",");
			if(!strcmp(token,"NULL"))
				gGetRspEntries->list[index].alpha_id.name[0] ='\0';
			else
				strcpy((S8 *)gGetRspEntries->list[index].alpha_id.name, token);
		
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_sec = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_min = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_hour = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_day = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_mon = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_wday = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].time.rtc_year = atoi(token);
			token = strtok(NULL, ",");
	 		gGetRspEntries->list[index].count = atoi(token);
	 		memcpy(&phb_ln_save_old[FileType].list[index],&gGetRspEntries->list[index],sizeof(l4c_phb_ln_entry_struct) );
			index++;
		}
		gGetRspEntries->no_list = index;
		phb_ln_save_old[FileType].no_list= index;
		if(FileType==0)
		{
			gGetRspEntries->type=0;
		}
		else if(FileType==1)
		{
			gGetRspEntries->type=1;
		}
		else if(FileType==2)
		{
			gGetRspEntries->type=2;
		}
		#ifdef MMI_ON_WIN32
		fclose(fpCallHistory);     
		#else
		FS_Close(fpCallHistory);
		#endif
	}
     #endif
}
/*
void * IncomingCallInd(void)
{
	static kal_uint16 callId=1;

	//printf(" ****IncomingCallInd******* " );
	
	//gIncomingCallIndStruct = (MMI_CC_CALL_RING_IND_STRUCT *)
	//	CMemAllocator::MyMallocDataPtr(sizeof(MMI_CC_CALL_RING_IND_STRUCT));

	gIncomingCallIndStruct->ref_count=0;
	gIncomingCallIndStruct->msg_len=200;
	gIncomingCallIndStruct->call_id=(kal_uint8)callId;
	gIncomingCallIndStruct->num.type=0;
	gIncomingCallIndStruct->num.length=6;
	strcpy(gIncomingCallIndStruct->num.number,"9810098100");
	gIncomingCallIndStruct->sub_addr.type=0;
	gIncomingCallIndStruct->sub_addr.length=0;
	strcpy(gIncomingCallIndStruct->sub_addr.number,"\0");
	strcpy(gIncomingCallIndStruct->name,"Shweta");
	gIncomingCallIndStruct->auto_answer=0;
	
	callId++;
	return (void *)gIncomingCallIndStruct;
}

*/
#endif
 
