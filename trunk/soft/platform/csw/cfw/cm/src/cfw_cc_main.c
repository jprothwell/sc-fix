#include <cfw.h>
#include <sul.h>
#include "api_msg.h"
#include <ts.h>
#include <cfw_prv.h>
#include "csw_debug.h"
#include "csw_cfw_bal.h"

#if (CFW_CM_DUMP_ENABLE==0) && (CFW_CM_SRV_ENABLE==1)

#include "cfw_cc_data.h"
#include "cfw_cc_tools.h"

//this value is used to record whether the audio is on. called in CloseAudio, StartAudio and CFW_CcAoProc
//      0     :    audio off
//      1     :    audio on
//....................................................................
static UINT8 gTmpAudio = 0;



#ifdef CFW_MULTI_SIM
//this value only used in DUAL SIM project, when releasing a normal call, function will check SIM status,
//the operation is not allowed if the SIM status is abnormal.  but the EMC call needn't SIM, so when an
// EMC is ongoing, we should allow the release operation when the SIM status is abnormal
//     0    : not have EMC call
//     1    : have EMC call ongoing
//...............................................................................................................
// we may record this in the PROC data structure????
static UINT8 gEmcCallOn = 0;


//these two value are defined in shell module, used in factory test mode, auto dail test.
//      0    :  ???? unsure
//      1    :   ???  unsure
//..............................................................................................................
extern UINT32 g_TestMode_AutoResponse[];
extern UINT32 g_TestMode_AutoEMCDial[];

static UINT32 GetCallStatus ( CFW_SIM_ID nSimId );
#else
//these value are defined in shell module, used in factory test mode, auto dail test.
//      0    :  ???? unsure
//      1    :   ???  unsure
//..............................................................................................................
extern UINT32 g_TestMode_AutoResponse;
extern UINT32 g_TestMode_AutoEMCDial ;
static UINT32 GetCallStatus ( void );
#endif

//-------------------------------------------------------
// useless value list as below, maybe we should delete them directely
//...................................................................................................
//#define SIM_MAX_NUM_LEN  20 
//#define CC_AcceptWaitingCall( XP, YP, ZP )  CC_TerminateOne(XP, YP, ZP)

HAO CmMTAoProc(CFW_EV* pEvent);

//Temporarily... For bug of stack.
static void CloseAudio( void )
{

	//UINT8 nCall = 0;
	CSW_PROFILE_FUNCTION_ENTER(CloseAudio);
	
//Modify by lixp for bug 10294 at 20090107
#if 0	
#ifndef CFW_MULTI_SIM
	CFW_SetServiceId(CFW_CC_SRV_ID);
	nCall = CFW_GetAoCount();
#else
	nCall = CFW_GetAoCount(CFW_CC_SRV_ID, CFW_SIM_COUNT);
#endif
	CSW_TRACE(CFW_CC_TS_ID, TSTXT("nCall=%d, gTmpAudio=%d\n") , nCall, gTmpAudio);
	if(nCall <= 1)
	{
		if ( 1==gTmpAudio)// add by wuys 2007-05-29
		{
			gTmpAudio = 0;

			CSW_TRACE(CFW_CC_TS_ID, TSTXT("speech	CloseAudio in CSW\n") );
			CFW_BalAudCallStop();
		}
	}
#else
#ifndef CFW_MULTI_SIM

    UINT32 nAllState = GetCallStatus();
#else
       CFW_SIM_ID nSimID;
       UINT32 nAllState = 0x00;
	   
	for (nSimID = CFW_SIM_0; nSimID < CFW_SIM_COUNT; nSimID ++)
	{
            nAllState |= GetCallStatus(nSimID);
   
	}

#endif

	CSW_TRACE(CFW_CC_TS_ID, TSTXT("CloseAudio in CSW , nAllState=0x%x, gTmpAudio=%d\n"), nAllState, gTmpAudio);
    if(!(( nAllState & CC_STATE_ACTIVE ) || ( nAllState & CC_STATE_HOLD )|| ( nAllState & CC_STATE_INCOMING ) 
      || ( nAllState & CC_STATE_DIALING ) || ( nAllState & CC_STATE_ALERTLING ) || ( nAllState & CC_STATE_RELEASE )))
    {
    		if ( 1 == gTmpAudio )
    		{
    			gTmpAudio = 0;

    			CSW_TRACE(CFW_CC_TS_ID, TSTXT("speech	CloseAudio in CSW CC module\n") );

    			CFW_BalAudCallStop();

    		}		
    }

#endif
	CSW_PROFILE_FUNCTION_EXIT(CloseAudio);
	return;

}
/////////////////////////////////////////////
//add by wuys 2007-05-31
//
static void StartAudio( void )
{
    
	CSW_PROFILE_FUNCTION_ENTER(StartAudio);
	CSW_TRACE(CFW_CC_TS_ID, TSTXT("StartAudio, gTmpAudio=%d\n"), gTmpAudio );
	if ( 0==gTmpAudio)
	{
		gTmpAudio = 1;

		CSW_TRACE(CFW_CC_TS_ID, TSTXT("speech    StartAudio in CSW\n") );
		CFW_BalAudCallStart();

	}
        
    CSW_PROFILE_FUNCTION_EXIT(StartAudio);
    return;
    
}

//
// Sim Service Init
//
UINT32 CFW_CcInit(VOID)
{   
	CSW_PROFILE_FUNCTION_ENTER(CFW_CcInit);
	CSW_TRACE(CFW_CC_TS_ID, TSTXT("CFW_CcInit \n") );
#ifdef CFW_MULTI_SIM	
       CFW_SIM_ID nSimID;
	for (nSimID = CFW_SIM_0; nSimID < CFW_SIM_COUNT; nSimID ++)
	{
		CFW_CfgSetClip(TRUE, nSimID);
		CFW_CfgSetSSN(TRUE, TRUE, nSimID);
	}
#else
	CFW_CfgSetClip(TRUE);
	CFW_CfgSetSSN(TRUE, TRUE);	
#endif
	CFW_RegisterCreateAoProc(API_CC_SETUP_IND, CmMTAoProc); 
	CSW_PROFILE_FUNCTION_EXIT(CFW_CcInit);
	return ERR_SUCCESS;
}
//===================================================================================
// function add by dingmx 20080702
//  get the cause value from cause IE according TS24008 10 .5.4.11
static UINT8 S_CFW_GET_CAUSE_VALUE(UINT8 length,UINT8* pCause)
{
    UINT8 iCauseValue = 0 ;
	if (length > 1)
	{
		if (pCause[0] & 0x80)
		{
			iCauseValue = pCause[1] & 0x7F;
			CSW_TRACE(CFW_CC_TS_ID, "1,cause length %d ,Value %d,  %x %x\n",length,iCauseValue,pCause[0],pCause[1]);
		}
		else 
		{
			if (length>2)
			{
				iCauseValue = pCause[2] & 0x7F;
				CSW_TRACE(CFW_CC_TS_ID, "2,cause length %d ,Value %d , %x %x %x \n",length,iCauseValue,pCause[0],pCause[1],pCause[2]);
			}
			else
			{
				CSW_TRACE(CFW_CC_TS_ID, "3,get cause error length %d ,Value %d,%x %x \n",length,iCauseValue,pCause[0],pCause[1]);
			}
		}
	}
	else
	{
		CSW_TRACE(CFW_CC_TS_ID, "4,cause length %d ",length);
	}
	return iCauseValue;
}

//===================================================================================
// CM AO process entry.
// There are some action will done in this function:
// 1> ??
// 2> ?? 
//
PRIVATE UINT32 CFW_CcAoProc (HAO hAo,CFW_EV* pEvent)
{
	UINT32 nEvtId;
	VOID* nEvParam = NULL;
	CM_SM_INFO* proc = NULL;
	UINT32 nUTI;
	UINT32 nState = 0;
	UINT32 ret = ERR_SUCCESS;
	CFW_EV ev;
	//hameina[+] 2009.1.7 move all cfg function here
	UINT8 nCSSI=0, nCSSU=0;
	//CFW_IND_EVENT_INFO pIndInfo;
	UINT8 nCallWaiting=0;
	UINT8 nColp=0;
	UINT8 bClip = 0;
	CFW_IND_EVENT_INFO sIndInfo;
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId=CFW_SIM_END;
#endif
	CSW_PROFILE_FUNCTION_ENTER(CFW_CcAoProc);

  //Move for pEvent == NULL issue.
	if(pEvent == NULL)
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcAoProc(), pEvent is NULL!\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
		return ERR_CFW_INVALID_PARAMETER;
	}

	if((UINT32)pEvent == 0xffffffff)//from MMI
	{
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()SetProc & CC send Msg to API!\n");

		SUL_MemSet8(&ev, 0xff, SIZEOF(CFW_EV));
		ev.nParam1 = 0xff;
		pEvent = &ev;
	}
	else
		CSW_TRACE(CFW_CC_TS_ID,"CFW_CcAoProc(), RECEIVED MESSAGE ***EvtID %x\n", pEvent->nEventId);



	nEvtId = pEvent->nEventId;
	nEvParam = (VOID*)pEvent->nParam1;

	proc = CFW_GetAoUserData(hAo);
	if( !proc )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcAoProc(), get proc DATA fail!\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
		return ERR_CME_MEMORY_FAILURE;
	}

#ifdef CFW_MULTI_SIM
	CFW_GetSimID( hAo, &nSimId);
	CFW_CfgGetIndicatorEventInfo(&sIndInfo, nSimId); 
	CFW_CfgGetColp(&nColp, nSimId); 
	ret = CFW_CfgGetSSN(&nCSSI, &nCSSU, nSimId);
	ret = CFW_CfgGetCallWaiting(&nCallWaiting, nSimId);
	CFW_CfgGetClip(& bClip, nSimId);
#else
	CFW_CfgGetClip(& bClip);
	CFW_CfgGetIndicatorEventInfo(&sIndInfo); 
	ret = CFW_CfgGetSSN(&nCSSI, &nCSSU);
	CFW_CfgGetColp(&nColp); 
	ret = CFW_CfgGetCallWaiting(&nCallWaiting);

#endif


	nState = CFW_GetAoState(hAo);
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc(), AO STATE nState=0X%X, proc->nReleaseFlag=%d\n", nState, proc->nReleaseFlag);

	CFW_GetUTI(hAo, &nUTI);

	if(proc->nReleaseFlag) //continue process release all call funtion
	{
		//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 
		sIndInfo.call = 0;
#ifndef CFW_MULTI_SIM
		CFW_CfgSetIndicatorEventInfo(&sIndInfo);
#else
		CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
#endif
		proc->nReleaseFlag = FALSE;
		//modified by zhangzd for bug 1629
		proc->bMODisc = TRUE;

		if(nState == CFW_SM_CM_STATE_MTDISC)	
		{
			CSW_TRACE(CFW_CC_TS_ID, "CFW_SM_CM_STATE_MTDISC \n");        
#ifndef CFW_MULTI_SIM
			CC_Release(proc, CFW_CM_NORMAL_CLEARING);
#else
			CC_Release(proc, CFW_CM_NORMAL_CLEARING, nSimId);
#endif
		}
		else
		{
			if(proc->nHoldFunction == 0x11)
			{
				proc->nCalledFuncFlag = 4; 
				proc->nHoldFunction = 0;
			}
			else
				proc->nCalledFuncFlag = 8; //CFW_CcReleaseCall is called
#ifndef CFW_MULTI_SIM
			CC_Disc(proc, CFW_CM_NORMAL_CLEARING);
#else
			CC_Disc(proc, CFW_CM_NORMAL_CLEARING, nSimId);
#endif
			CFW_SetAoState(hAo, CFW_SM_CM_STATE_MODISC);
			//CFW_BalAudCallStop();
		}
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
		return ret;
	}
	//handle DICONNECT,RELEASE,RELEASE COMPLETE AudioOn message at first ,
	//these message can be receive in all most state.
	if(nEvtId == API_CC_DISC_IND)
	{
		//CFW_BalAudCallStop();
		// Close the audio device;
		//CloseAudio ();
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() Received Event: API_CC_DISC_IND 0x%x\n",nEvtId);
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() Received Event: nState=0x%x,CFW_SM_CM_STATE_MODISC=0x%x\n", nState,CFW_SM_CM_STATE_MODISC);
		if( (nState != CFW_SM_STATE_IDLE) && (nState != CFW_SM_CM_STATE_MODISC) \
			&& (nState != CFW_SM_CM_STATE_MTDISC))
		{
			//CFW_IND_EVENT_INFO pIndInfo;
			UINT8 iCauseValue = 0;
			api_CcDiscInd_t *p = (api_CcDiscInd_t*)nEvParam;
			//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

			sIndInfo.call = 0;

			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() not idle_modisc_mtdisc! \n");

			// [[changed by dingmx 20080702, according TS24008 10.5.4.11	
			iCauseValue = S_CFW_GET_CAUSE_VALUE(p->CauseLen, p->Cause);
			// ]]changed by dingmx end , 

			proc->nCauseVal = iCauseValue;

			//[[hameina[+] 2007-10-26 for cssu=5
			CSW_TRACE(CFW_CC_TS_ID, "for cssu=5,proc->nCallState=%d\n", proc->nCallState);
#ifndef CFW_MULTI_SIM
			CFW_CfgSetIndicatorEventInfo(&sIndInfo);
			CC_SendCrssInfoNotify(&(p->Facility));
			if(proc->nCallState == CFW_CM_STATUS_HELD)
			{
				CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 5, nUTI, 0x11);//cssu=5:call on hold has been released
				
			}
#else
			CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
			CC_SendCrssInfoNotify(&(p->Facility), nSimId);
			if(proc->nCallState == CFW_CM_STATUS_HELD)
			{
				CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 5, nUTI |(nSimId<<24), 0x11);//cssu=5:call on hold has been released

			}
			//]]hameina[+] 2007-10-26
#endif
			CC_ProcNwDisconnectMsg(hAo, proc, nEvParam);
		}
		else if(nState == CFW_SM_CM_STATE_MODISC)
		{
			//------------------------------------------------------------------------------------
			//this branch added by Hameina, 2009.3.9
			//-------------------------------------------------------------------------------------
			//reason: when user hand off the call and the net work also disconnect the call all most at the same time,
			//call state is CFW_SM_CM_STATE_MODISC, and we should deal the conflict of this MO MT disconnect
			//--------------------------------------------------------------------------------------
			//result:  1. call status should be changed to CFW_SM_CM_STATE_MTDISC.
			//		 2. send API_CC_RELEASE_REQ
			//--------------------------------------------------------------------------------------
			proc->nCallState = CFW_CM_STATUS_RELEASED;
			CFW_SetAoState(hAo, CFW_SM_CM_STATE_MTDISC);

			CSW_TRACE(CFW_CC_TS_ID, "recieve API_CC_DISC_IND, send release msg to nw directly, and changed to CFW_SM_CM_STATE_MTDISC \n");
#ifndef CFW_MULTI_SIM
			CC_Release(proc, CFW_CM_NORMAL_CLEARING);
#else
			CC_Release(proc, CFW_CM_NORMAL_CLEARING, nSimId);
#endif

		}
		else
		{
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
			return ret;
		}
	}
	else if(nEvtId == API_CC_RELEASE_IND)	
	{
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()! Received Event: API_CC_RELEASE_IND 0x%x\n",nEvtId);
        	//Modify by lixp for bug 10294 at 20090107
		//CloseAudio();

		if(nState != CFW_SM_STATE_IDLE)
		{
			//CFW_IND_EVENT_INFO pIndInfo;
			UINT8 iCauseValue = 0;
			UINT32 nAllState = 0;

			api_CcReleaseInd_t *p = (api_CcReleaseInd_t*)nEvParam;
#ifndef CFW_MULTI_SIM
			nAllState = GetCallStatus();
#else
			nAllState = GetCallStatus(nSimId);
#endif
			//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

			sIndInfo.call = 0;

			//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
			iCauseValue = S_CFW_GET_CAUSE_VALUE(p->Cause1Len, p->Cause1);
			//[[ changed by dingmx end , 

			CSW_TRACE(CFW_CC_TS_ID, "Receive RELEASE facility info 11, p->Facility.StructValid=%d\n", p->Facility.StructValid);
			//[[hameina[+] 2007-10-26 for cssu=10
			if(p->Facility.StructValid)
			{
				CSW_TRACE(CFW_CC_TS_ID, "Receive RELEASE facility info  22, p->Facility.Status=0x%x\n", p->Facility.Status);
#ifndef CFW_MULTI_SIM
				CFW_CfgSetIndicatorEventInfo(&sIndInfo);
				if(p->Facility.Status == API_CRSS_MOCALL_HAS_BEEN_FORWARDED)
					CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 2, 0xff, nUTI, 0x11);//CSSi=2:call has been forwarded
				else if(p->Facility.Status == API_CRSS_MTCALL_HAS_BEEN_FORWARDED)
					CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 10, nUTI, 0x11);//CSSu=10:call has been forwarded
#else
				CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
				if(p->Facility.Status == API_CRSS_MOCALL_HAS_BEEN_FORWARDED)
					CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 2, 0xff, nUTI |(nSimId<<24), 0x11);//CSSi=2:call has been forwarded
				else if(p->Facility.Status == API_CRSS_MTCALL_HAS_BEEN_FORWARDED)
					CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 10, nUTI |(nSimId<<24), 0x11);//CSSu=10:call has been forwarded
#endif
			}
			//]]hameina[+]2007-10-16
			CSW_TRACE(CFW_CC_TS_ID, "proc->nHoldFunction=%d\n", proc->nHoldFunction);
			//CRSS: If accept the waiting call...
			if(proc->nHoldFunction == 1)
			{
				proc->nHoldFunction = 0;
				CC_ProcNwReleaseMsg(hAo, proc, iCauseValue);
        //Modify by lixp for bug 10294 at 20090107
        CloseAudio();
#ifndef CFW_MULTI_SIM
				nAllState = GetCallStatus();
				if(!( nAllState & CC_STATE_ACTIVE ) && !( nAllState & CC_STATE_RELEASE ) &&  (nAllState & CC_STATE_WAITING)  )
				{
					CSW_TRACE(CFW_CC_TS_ID, "CC_AcceptWaitingCall trace\n");
					CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD1, 1 );
				}
				//fix the bug 3889,3891
				else if(!( nAllState & CC_STATE_ACTIVE ) && !( nAllState & CC_STATE_RELEASE ) &&  (nAllState & CC_STATE_HOLD)  )
				{
					//it is important for bug[Bug 3852] multi-party2007.01.31
					CSW_TRACE(CFW_CC_TS_ID, "CC_Retrieve trace\n");
					CC_RetrieveCall(HNULL);	
					CC_RetrieveMPTY();
				}
#else
				nAllState = GetCallStatus(nSimId);
				if(!( nAllState & CC_STATE_ACTIVE ) && !( nAllState & CC_STATE_RELEASE ) &&  (nAllState & CC_STATE_WAITING)  )
				{
					CSW_TRACE(CFW_CC_TS_ID, "CC_TerminateOne\n");
					CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD1, 1, nSimId);
				}
				//fix the bug 3889,3891
				else if(!( nAllState & CC_STATE_ACTIVE ) && !( nAllState & CC_STATE_RELEASE ) &&  (nAllState & CC_STATE_HOLD)  )
				{
					//it is important for bug[Bug 3852] multi-party2007.01.31
					CSW_TRACE(CFW_CC_TS_ID, "CC_Retrieve trace\n");
					CC_RetrieveCall(HNULL, nSimId);	
					CC_RetrieveMPTY(nSimId);
				}
#endif

			}
			else
			{
				CC_ProcNwReleaseMsg(hAo, proc, iCauseValue);
        //Modify by lixp for bug 10294 at 20090107
        CloseAudio();								
			}
		}
		else
		{
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			proc->bMODisc = FALSE;
			CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
			return ret;
		}
	}
	else if(nEvtId == API_CC_RELEASECMPLT_IND)	
	{
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() Received Event: API_CC_RELEASECMPLT_IND 0x%x\n",nState);
		if(nState != CFW_SM_STATE_IDLE)
		{            
			UINT8 iCauseValue = 0;
			api_CcReleaseCmpltInd_t *p = (api_CcReleaseCmpltInd_t*)nEvParam;

			sIndInfo.call = 0;
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()Receive RELEASE COMPLETE message\n");

			//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
			if (16 == proc->nCalledFuncFlag)
			{
				iCauseValue = proc->nCauseVal;
			}
			else
			{
				iCauseValue = S_CFW_GET_CAUSE_VALUE(p->CauseLen, p->Cause);
			}
			//]] change end

#ifndef CFW_MULTI_SIM
			CFW_CfgSetIndicatorEventInfo(&sIndInfo);
			CC_SendCrssInfoNotify(&(p->Facility));
#else
			CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
			CC_SendCrssInfoNotify(&(p->Facility), nSimId);
#endif
			CC_ProcNwReleaseCmplMsg(hAo,  proc, iCauseValue);
  		//Modify by lixp for bug 10294 at 20090107
  		CloseAudio();            

		}
		else
		{
			CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
			return ret;
		}
	}
	else if(nEvtId == API_CC_AUDIOON_IND)	
	{
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()! Received Event: API_CC_AUDIOON_IND 0x%x\n",nEvtId);
		proc->bAudioOnFlag = TRUE;
		CSW_TRACE(CFW_CC_TS_ID, "proc->nDir=%d, gTmpAudio=%d\n", proc->nDir, gTmpAudio);
		if((proc->nDir == 0)&&(0==gTmpAudio))
		{
			StartAudio();
		}
	}
	else //handle all event except disconnect,release, releasecomplete
	{
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()status machine, status= 0x%x, nEvtId=0x%x\n",nState, nEvtId);
	
		switch(nState)
		{	 
			case CFW_SM_STATE_IDLE:
			{
				if(pEvent->nParam1 == 0xff)
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()CC===>Stack send SETUP REQ API_CC_SETUP_REQ 0x%x\n",API_CC_SETUP_REQ);
#ifndef CFW_MULTI_SIM
					CFW_SendSclMessage(API_CC_SETUP_REQ, proc->pMsg);
#else
					CFW_SendSclMessage(API_CC_SETUP_REQ, proc->pMsg, nSimId);
#endif
					CFW_SetAoState(hAo, CFW_SM_CM_STATE_MOPROC);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;
				}
				if(nEvtId == API_CC_SETUP_IND)
				{
					api_CcSetupInd_t *p = (api_CcSetupInd_t*)nEvParam;
					//MMIMALLOC
					CFW_SPEECH_CALL_IND *pSpeechCallInd = (CFW_SPEECH_CALL_IND*)CSW_CC_MALLOC(SIZEOF(CFW_SPEECH_CALL_IND));

					if(pSpeechCallInd == NULL)
					{
						CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
						return ERR_NO_MORE_MEMORY;
					}                        

					SUL_ZeroMemory32(pSpeechCallInd, SIZEOF(CFW_SPEECH_CALL_IND));

					CSW_TRACE(CFW_CC_TS_ID, "Incoming call\n");

					CSW_TRACE(CFW_CC_TS_ID, "Get nCSSI=%d,nCSSU=%d\n", nCSSI, nCSSU);

					CFW_SetUTI(hAo, (UINT16)p->TI, 2);//set TI*******

					proc->nTI = p->TI;
					proc->nCallMode = CFW_CM_MODE_VOICE;
					proc->nDir = CFW_DIRECTION_MOBILE_TERMINATED;
					proc->bIsMpty = FALSE;
					pSpeechCallInd->nCLIValidity = 0xFF;

					/*If cfw get a invalid number, it will be cut...*/
					CSW_TRACE(CFW_CC_TS_ID, "CC ==> CPN length: %d, p->Facility.Status=%d\n", p->CPNLen, p->Facility.Status);

					if( p->CPNLen > TEL_NUMBER_MAX_LEN)
						p->CPNLen = TEL_NUMBER_MAX_LEN;

					if(( p->Facility.Status == API_CRSS_MTCALL_IS_FORWARDED_CALL ) && ( TRUE == p->Facility.StructValid ))
					{
						//[[hameina [mod] 2007.08.01 :forwarded call has no number, original one is RPN, but now we add a check
						if(p->CPNLen)
						{
							CSW_TRACE(CFW_CC_TS_ID, "cfw cc recieve a forwarded call 2, CPNLen=%d\n", p->CPNLen);
							proc->nAddressType = p->CallingPartyNb[0]  | 0x80; //type of address:0x81 or 0x91 or 0xA1

							pSpeechCallInd->TelNumber.nSize = CallingPartyrRaw2BCD(&(p->CallingPartyNb[0]), proc->pNumber, p->CPNLen);	 	
						}
						else if(p->RPNLen)
						{
							CSW_TRACE(CFW_CC_TS_ID, "cfw cc recieve a forwarded call 1, RPLen=%d\n", p->RPNLen);
							proc->nAddressType = p->RedirectingPartyNb[0]  | 0x80; //type of address:0x81 or 0x91 or 0xA1
							pSpeechCallInd->TelNumber.nSize = CallingPartyrRaw2BCD(&(p->RedirectingPartyNb[0]), proc->pNumber, p->RPNLen);
						}
						else
						{
							CSW_TRACE(CFW_CC_TS_ID, "cfw cc recieve a forwarded call 3, error!!!!both RPN & CPN is null\n");
						}

						//]]hameina [mod] 2007.08.01
					}
					else
					{
						proc->nAddressType = p->CallingPartyNb[0]  | 0x80; //type of address:0x81 or 0x91 or 0xA1

						pSpeechCallInd->TelNumber.nSize = CallingPartyrRaw2BCD(&(p->CallingPartyNb[0]), proc->pNumber, p->CPNLen);	 	
					}
					proc->nNumberSize = pSpeechCallInd->TelNumber.nSize;

					/*Format the invalid number...*/
					if(proc->nNumberSize > TEL_NUMBER_MAX_LEN)
					{
						proc->nNumberSize = TEL_NUMBER_MAX_LEN;
						pSpeechCallInd->TelNumber.nSize = TEL_NUMBER_MAX_LEN;
					}
#ifndef CFW_MULTI_SIM
					CFW_SetServiceId(CFW_CC_SRV_ID);
					if(CFW_GetAoCount() > 1)
#else
					if(CFW_GetAoCount(CFW_CC_SRV_ID, nSimId) > 1)
#endif
					{
						proc->nCallState = CFW_CM_STATUS_WAITING;
						
						//hameina[+] 2009.7.31 AOcount >0, this call should be a waiting call.
						pSpeechCallInd->nCode = 3; //call is waiting 
						
						if((nCallWaiting == 1) && (nCSSU == 1))
						{   
							//[[hameina[mod] 2007.9.10, out put info doesn't correct
							if(p->Facility.Status == API_CRSS_MTCALL_IS_FORWARDED_CALL)
							{
								pSpeechCallInd->nDisplayType = 6; //+CSSU +CCWA:
								pSpeechCallInd->nCode = 0; // call is a forwarded call :CSSU code
								if(nCSSI)
								{
#ifndef  CFW_MULTI_SIM
									CFW_PostNotifyEvent ( EV_CFW_CC_CRSSINFO_IND, 3, 0xff, nUTI, 0x11); //cssi=3
#else
									CFW_PostNotifyEvent ( EV_CFW_CC_CRSSINFO_IND, 3, 0xff, nUTI |(nSimId<<24), 0x11); //cssi=3
#endif
								}
							}
							else
							{
								pSpeechCallInd->nDisplayType = 5; //+CSSI +CCWA:
							}
							//]]hameina[mod] 2007.9.10, out put info doesn't correct, bug 13539
							pSpeechCallInd->TelNumber.nType =  proc->nAddressType;

							SUL_MemCopy8( pSpeechCallInd->TelNumber.nTelNumber, proc->pNumber, proc->nNumberSize);	 

						}
						else if((nCallWaiting == 1) && (nCSSU == 0))
						{
							pSpeechCallInd->nDisplayType = 4; //+CCWA:
							pSpeechCallInd->TelNumber.nType =  proc->nAddressType;
							SUL_MemCopy8( pSpeechCallInd->TelNumber.nTelNumber, proc->pNumber, proc->nNumberSize);	 
						}
						else if(nCallWaiting == 0 && (nCSSU == 0))
						{
							pSpeechCallInd->nDisplayType = 0; //no display
						}
						else if(nCallWaiting == 0 && (nCSSU == 1))
						{
							pSpeechCallInd->nDisplayType = 3; //+CSSU :
							if(p->Facility.Status == API_CRSS_MTCALL_IS_FORWARDED_CALL)
							{
								pSpeechCallInd->nCode = 0; // call is a forwarded call :CSSU code
							}
							else
								pSpeechCallInd->nCode = 0xff;
						}
#ifndef CFW_MULTI_SIM
						CC_CallConfirm(p->TI, CFW_CM_USER_BUSY);		
#else
						CC_CallConfirm(p->TI, CFW_CM_USER_BUSY, nSimId);		
#endif
					}
					else
					{
						proc->nCallState = CFW_CM_STATUS_INCOMING;
						if(bClip)
						{
							if((nCSSI == 1) && (p->Facility.Status == API_CRSS_MTCALL_IS_FORWARDED_CALL))
							{
								pSpeechCallInd->nDisplayType = 8; //+CLIP + CSSU:
								pSpeechCallInd->nCode = 0; // MT call is forwarded call :CSSU code
							}
							else
							{
								pSpeechCallInd->nDisplayType = 1;  //+CLIP:
								pSpeechCallInd->nCode = 0xff;
							}
							switch(p->CauseOfNoCLI)
							{
							case API_CAUSEOFNOCLI_REJECTBYUSER:
							case API_CAUSEOFNOCLI_INTERACTSERV:
							case API_CAUSEOFNOCLI_PAYPHONE:
								pSpeechCallInd->nCLIValidity = 0; //CLIP not provisioned
								break;
							case API_CAUSEOFNOCLI_INVALID:
								pSpeechCallInd->nCLIValidity = 1; //CLIP provisioned
								break;
							case API_CAUSEOFNOCLI_UNAVAILABLE:
								pSpeechCallInd->nCLIValidity = 3; //unknown (e.g. no network, etc.)
								break;

							default:
								pSpeechCallInd->nCLIValidity = 0xFF;
								break;

							}
							pSpeechCallInd->TelNumber.nType =  proc->nAddressType;
							SUL_MemCopy8( pSpeechCallInd->TelNumber.nTelNumber, proc->pNumber, proc->nNumberSize);	 
						}
						else
						{
							if((nCSSU == 1) && (p->Facility.Status == API_CRSS_MTCALL_IS_FORWARDED_CALL))
							{
								pSpeechCallInd->nDisplayType = 3; // + CSSU:
								pSpeechCallInd->nCode = 0; // MT call is forwarded call :CSSU code
							}
							else
							{
								pSpeechCallInd->nDisplayType = 0; //no display
								pSpeechCallInd->nCode = 0xff;

							}
						}
#ifndef CFW_MULTI_SIM
						CC_CallConfirm(p->TI, 0xFF);	
#else
						CC_CallConfirm(p->TI, 0xFF, nSimId);	
#endif

					}
#ifndef CFW_MULTI_SIM
					CC_Alert(p->TI);
#else
					CC_Alert(p->TI, nSimId);
#endif

					CFW_SetAoState(hAo, CFW_SM_CM_STATE_MTPROC);
					//CFW_GetUTI(hAo, &nUTI);
#ifndef CFW_MULTI_SIM
					if(!g_TestMode_AutoResponse)
					{
						CFW_PostNotifyEvent ( EV_CFW_CC_SPEECH_CALL_IND, (UINT32)pSpeechCallInd, SIZEOF(CFW_SPEECH_CALL_IND), nUTI, 0);
					}
					else
					{
						CFW_PostNotifyEvent ( EV_CFW_CC_SPEECH_CALL_IND, (UINT32)pSpeechCallInd, SIZEOF(CFW_SPEECH_CALL_IND), nUTI, 0);
						CFW_PostNotifyEvent ( EV_CFW_CC_SPEECH_CALL_IND, (UINT32)pSpeechCallInd, SIZEOF(CFW_SPEECH_CALL_IND), GENERATE_SHELL_UTI(), 0);

					}
#else
					if(!g_TestMode_AutoResponse[nSimId])
					{
						CFW_PostNotifyEvent ( EV_CFW_CC_SPEECH_CALL_IND, (UINT32)pSpeechCallInd, SIZEOF(CFW_SPEECH_CALL_IND), nUTI |(nSimId<<24), 0);
					}
					else
					{
						CFW_PostNotifyEvent ( EV_CFW_CC_SPEECH_CALL_IND, (UINT32)pSpeechCallInd, SIZEOF(CFW_SPEECH_CALL_IND), nUTI |(nSimId<<24), 0);
						CFW_PostNotifyEvent ( EV_CFW_CC_SPEECH_CALL_IND, (UINT32)pSpeechCallInd, SIZEOF(CFW_SPEECH_CALL_IND), GENERATE_SHELL_UTI() |(nSimId<<24), 0);

					}
#endif					
					CSW_TRACE(CFW_CC_TS_ID, "send EV_CFW_CC_SPEECH_CALL_IND to MMI\n");
				}
				else
				{
					CSW_TRACE(CFW_CC_TS_ID, "[WARNING]CFW_CcAoProc() Recieve event in IDLE state, not consumed\n");
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;

				}
				break;

			}
			case CFW_SM_CM_STATE_MOPROC:
			{
				if(pEvent->nParam1 == 0xff)
				{
					if(proc->nCurSubState == SUBSTATE_MO_PROC_DTMF)
					{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MOPROC:SUBSTATE_MO_PROC_DTMF\n");
							proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
							CC_StartDTMF(proc, *((UINT8*)(proc->pMsg)));
							CSW_TRACE(CFW_CC_TS_ID, "in MOPROC state,send START DTMF\n");
							CC_StopDTMF(proc);
#else
							CC_StartDTMF(proc, *((UINT8*)(proc->pMsg)), nSimId);
							CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state,send START DTMF\n");
							CC_StopDTMF(proc, nSimId);
#endif
							CSW_CC_FREE(proc->pMsg);
							proc->pMsg = NULL;
							CSW_TRACE(CFW_CC_TS_ID, "in MOPROC state,send STOP DTMF\n");

					}
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;

				}
				
				switch(nEvtId) 	
				{
				case API_CC_TI_IND:
				{
					api_CcTIInd_t *p = (api_CcTIInd_t*)nEvParam;
					proc->nTI = p->TI;
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() stack==>>CC API_CC_TI_IND 0x%x\n", API_CC_TI_IND);
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() IN MOPROC,receive TI is: %d\n", p->TI);
					CFW_SetUTI(hAo, (UINT16)p->TI, 2);//set TI*******
					break;
				}
				case  API_CC_CALLPROC_IND:	
				{
					UINT8 iProgressIndValue = 0;
					api_CcCallProcInd_t *p = (api_CcCallProcInd_t*)nEvParam;
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() stack==>>CC API_CC_CALLPROC_IND 0x%x\n", API_CC_CALLPROC_IND);
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() IN MOPROC,Receive CALL PROCEEDING,TI:%d\n", p->TI);

					iProgressIndValue = p->ProgressInd[1] & 0x7F;
					if (iProgressIndValue == 8)
					{
						proc->bProgressIndFlag = TRUE;
					}

					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CC==>>MMI EV_CFW_CC_CALL_INFO_IND 0x%x\n", EV_CFW_CC_CALL_INFO_IND);
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 1, proc->nIndex, nUTI, 1); //Notify MMI the call is in progress
					CSW_TRACE(CFW_CC_TS_ID, "send EV_CFW_CC_CALL_INFO_IND to MMI for OutGoing Call in progress\n");

					CC_SendCrssInfoNotify(&(p->Facility));
#else
					CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 1, proc->nIndex, nUTI |(nSimId<<24), 1); //Notify MMI the call is in progress
					CC_SendCrssInfoNotify(&(p->Facility), nSimId);
#endif
					CSW_TRACE(CFW_CC_TS_ID, "case API_CC_CALLPROC_IND call CC_SendCrssInfoNotify\n");

					break;

				}
				case API_CC_PROGRESS_IND:	
				{
					UINT8 iProgressIndValue = 0;
					api_CcProgressInd_t *p = (api_CcProgressInd_t*)nEvParam;

					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() IN MOPROC,Receive CALL PROGRESS,TI:%d\n", p->TI);
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() stack==>>CC API_CC_PROGRESS_IND 0x%x\n", API_CC_PROGRESS_IND);

					iProgressIndValue = p->ProgressInd[1] & 0x7F;
					if (iProgressIndValue == 8)
					{
						proc->bProgressIndFlag = TRUE;
					}
					break;
				}
				case API_CC_ALERT_IND:	
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() stack==>>CC API_CC_ALERT_IND 0x%x\n", API_CC_ALERT_IND);

					UINT8 iProgressIndValue = 0;
					api_CcAlertInd_t *p = (api_CcAlertInd_t*)nEvParam;
					CSW_TRACE(CFW_CC_TS_ID, "IN MOPROC,Receive ALERT,TI:%d\n", p->TI);
					
#ifndef CFW_MULTI_SIM
					CC_SendCrssInfoNotify(&(p->Facility));
#else
					CC_SendCrssInfoNotify(&(p->Facility), nSimId);
#endif
					iProgressIndValue = p->ProgressInd[1] & 0x7F;
					if (iProgressIndValue == 8)
					{
						proc->bProgressIndFlag = TRUE;
					}
					proc->nCallState = CFW_CM_STATUS_ALERTING;
					CSW_TRACE(CFW_CC_TS_ID, "case API_CC_ALERT_IND:CFW_CcAoProc() cc==>>mmi EV_CFW_CC_CALL_INFO_IND 0x%x\n", EV_CFW_CC_CALL_INFO_IND);

					StartAudio();
#ifndef CFW_MULTI_SIM
					CC_SendCrssInfoNotify(&(p->Facility));
					CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 1, proc->nIndex, nUTI, 0); //Notify MMI the called party is alerting
#else
					CC_SendCrssInfoNotify(&(p->Facility), nSimId);
					CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 1, proc->nIndex, nUTI|(nSimId<<24), 0); 
#endif
					CSW_TRACE(CFW_CC_TS_ID, "send EV_CFW_CC_CALL_INFO_IND to MMI for Sounder opend\n");

					break;                       
				}

				case API_CC_CONNECT_IND:	
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() stack==>>CC API_CC_CONNECT_IND 0x%x\n", API_CC_CONNECT_IND);
					//CFW_IND_EVENT_INFO pIndInfo;
					//UINT8 nColp;
					//CFW_CfgGetColp(&nColp); 
					//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 
					api_CcConnectInd_t *p = (api_CcConnectInd_t*)nEvParam;
					CSW_TRACE(CFW_CC_TS_ID, "the return nColp=%d\n", nColp);

					sIndInfo.call = 1;
					proc->nCallState = CFW_CM_STATUS_ACTIVE;
					proc->nCalledFuncFlag = 0;
					CSW_TRACE(CFW_CC_TS_ID, "IN MOPROC,Receive CONNECT,TI:%d\n", p->TI);

					//////////////////////// Start the audio. 
					/*
					if(gTmpAudio == 0) 
					{
					g_CSWOperateMicAndReceiver=TRUE;

					}*/
					StartAudio();
					CFW_SetAoState(hAo, CFW_SM_CM_STATE_ACTIVE);
					//??????????
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case API_CC_CONNECT_IND: cc==>>mmi EV_CFW_CC_CALL_INFO_IND 0x%x\n", EV_CFW_CC_CALL_INFO_IND);
#ifndef CFW_MULTI_SIM
					CFW_CfgSetIndicatorEventInfo(&sIndInfo);
					CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI, 0); //Notify MMI the called party accept call
					CC_SendCrssInfoNotify(&(p->Facility));
#else
					CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
					CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI |(nSimId<<24), 0); //Notify MMI the called party accept call
					CC_SendCrssInfoNotify(&(p->Facility), nSimId);
#endif

					CSW_TRACE(CFW_CC_TS_ID, "send EV_CFW_CC_CALL_INFO_IND to MMI for Sounder open\n");
					//CFW_GetUTI(hAo, &nUTI);
					CSW_TRACE(CFW_CC_TS_ID, "the connected number length=%d\n", p->CNLen);
					if((nColp == 1) && (p->CNLen > 0))
					{
						//MMIMALLOC
						CFW_TELNUMBER *pConnectedNum = (CFW_TELNUMBER*)CSW_CC_MALLOC(SIZEOF(CFW_TELNUMBER));

						if(pConnectedNum == NULL)
						{
							CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
							return ERR_NO_MORE_MEMORY;
						}                               

						SUL_ZeroMemory32(pConnectedNum, SIZEOF(CFW_TELNUMBER));
						pConnectedNum->nType = p->ConnectedNb[0]  | 0x80; //type of address:0x81 or 0x91 or 0xA1

						if( p->CNLen > TEL_NUMBER_MAX_LEN)
							p->CNLen = TEL_NUMBER_MAX_LEN;

						SUL_MemCopy8(pConnectedNum->nTelNumber, &(p->ConnectedNb[0]), p->CNLen);	 
#ifndef CFW_MULTI_SIM

						CFW_PostNotifyEvent(EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, (UINT32)pConnectedNum, 0, nUTI, 1); 
#else
						CFW_PostNotifyEvent(EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, (UINT32)pConnectedNum, 0, nUTI |(nSimId<<24), 1); 
#endif
					}
					else
					{
						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case API_CC_CONNECT_IND: cc==>>mmi EV_CFW_CC_INITIATE_SPEECH_CALL_RSP 0x%x\n", EV_CFW_CC_INITIATE_SPEECH_CALL_RSP);
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, 0, 0, nUTI, 0);
#else
						CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, 0, 0, nUTI |(nSimId<<24), 0);
#endif
					}

					break;
				}
				case API_CC_FACILITY_IND:
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() stack==>>CC API_CC_FACILITY_IND 0x%x\n", API_CC_FACILITY_IND);
					api_CcFacilityInd_t *p = (api_CcFacilityInd_t*)nEvParam;
					CSW_TRACE(CFW_CC_TS_ID, "in MOPROC state:receive FACILITY,p->Facility[0].Status:%d\n", p->Facility[0].Status);
					CSW_TRACE(CFW_CC_TS_ID, "in MOPROC state:receive FACILITY,p->Facility[0].ss_Operation:%d\n", p->Facility[0].ss_Operation);
#ifndef CFW_MULTI_SIM					
					CC_SendCrssInfoNotify(&(p->Facility[0]));
					CC_SendCrssInfoNotify(&(p->Facility[1]));
#else
					CC_SendCrssInfoNotify(&(p->Facility[0]), nSimId);
					CC_SendCrssInfoNotify(&(p->Facility[1]), nSimId);
#endif				
					break;
				}
				case API_CC_ERROR_IND:
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() stack==>>CC API_CC_ERROR_IND 0x%x\n", API_CC_ERROR_IND);

					UINT8 iCauseValue = 0;
					api_CcErrorInd_t *p = (api_CcErrorInd_t*)nEvParam;

                        		//Modify by lixp for bug 10294 at 20090107
                        		//CloseAudio();                        

					//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
					iCauseValue = S_CFW_GET_CAUSE_VALUE(p->NwkCauseLen, p->NwkCause);
					//[[ change end

					CSW_TRACE(CFW_CC_TS_ID, "in MOPROC state,CM local Error %x\n", p->Cause);

					switch(p->Cause)
					{
						case API_LOW_LAYER_FAIL:
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case: API_LOW_LAYER_FAIL \n");
						}    
						case API_NORMAL_RELEASE:
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case: API_NORMAL_RELEASE \n");
							//CFW_IND_EVENT_INFO pIndInfo;
							//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

							sIndInfo.call = 0;

							proc->nCallState = CFW_CM_STATUS_RELEASED;
							//CFW_CfgSetExtendedErrorV0(ERR_CME_NO_CONNECTION_TO_PHONE); 
#ifndef  CFW_MULTI_SIM
							CFW_CfgSetIndicatorEventInfo(&sIndInfo);
							CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI, 1); //Notify MMI the call isn't in progress
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case API_NORMAL_RELEASE:send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");

							//Release the current index
							//CFW_GetUTI(hAo, &nUTI);
							//For auto dialing..
							if(!g_TestMode_AutoEMCDial)
							{
								CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
							}
							else
							{
								CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
								CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, GENERATE_SHELL_UTI(), 0xf0);
							}
							////////////
#else
							CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
							CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI |(nSimId<<24), 1); //Notify MMI the call isn't in progress
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case API_NORMAL_RELEASE:send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");
							if(!g_TestMode_AutoEMCDial[nSimId])
							{
								CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI |(nSimId<<24), 0xf0);
							}
							else
							{
								CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI |(nSimId<<24), 0xf0);
								CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, 
									GENERATE_SHELL_UTI() |(nSimId<<24), 0xf0);
							}
#endif
							CFW_UnRegisterAO( CFW_CC_SRV_ID, hAo);

							break;
						}
						case API_CC_TIMER_EXPIRY://change state to MODISC
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case: API_LOW_LAYER_FAIL \n");
							proc->nCallState = CFW_CM_STATUS_RELEASED;
							CFW_SetAoState(hAo, CFW_SM_CM_STATE_MODISC);
							break;
						default:
							break;

					}
    			//Modify by lixp for bug 10294 at 20090107
    			CloseAudio();
					
					break;
				}
				default:
				{
					CSW_TRACE(CFW_CC_TS_ID, "[warning]CFW_CcAoProc() IN MO PROC state, recieve unknown event in this state, not consumed \n");
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;

				}
			}//switch(nEvtId) 	
			break;

			}
			case CFW_SM_CM_STATE_MODISC:
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case: CFW_SM_CM_STATE_MODISC \n");

				if(nEvtId == API_CC_ERROR_IND)
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case: CFW_SM_CM_STATE_MODISC API_CC_ERROR_IND \n");
					UINT8 iCauseValue = 0;
					api_CcErrorInd_t *p = (api_CcErrorInd_t*)nEvParam;

					//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
					iCauseValue = S_CFW_GET_CAUSE_VALUE(p->NwkCauseLen, p->NwkCause);
					// [[change end

					CSW_TRACE(CFW_CC_TS_ID, "in MODISC state,CM local Error %x\n", p->Cause);
					switch(p->Cause)
					{
						case API_LOW_LAYER_FAIL:
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case: CFW_SM_CM_STATE_MODISC API_LOW_LAYER_FAIL \n");
						case API_NORMAL_RELEASE:
						case API_CC_PROTOCOL_ERROR:
						{
							//CFW_IND_EVENT_INFO pIndInfo;
							//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

							sIndInfo.call = 0;

							proc->nCallState = CFW_CM_STATUS_RELEASED;
							//CFW_CfgSetExtendedErrorV0(ERR_CME_UNKNOWN);
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CC==>>MMI EV_CFW_CC_CALL_INFO_IND 0x%x\n", EV_CFW_CC_CALL_INFO_IND);
#ifndef  CFW_MULTI_SIM
							CFW_CfgSetIndicatorEventInfo(&sIndInfo);
							CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI, 1); //Notify MMI the call isn't in progress
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()API_CC_PROTOCOL_ERROR, send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");

							CFW_GetUTI(hAo, &nUTI);
							switch(proc->nCalledFuncFlag)
							{
								case 1:
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CC==>>MMI case 1:\n");

									CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
									break;
								case 2:
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CC==>>MMI case 2:\n");
									CFW_PostNotifyEvent ( EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);

									break;
								case 4:
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CC==>>MMI case 4:\n");
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);

									break;
								case 8:
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CC==>>MMI case 8:\n");
									CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);

									break;
								default:
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() API_CC_PROTOCOL_ERROR CC==>>MMI  default:\n");
									CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_IND, proc->nIndex, iCauseValue, nUTI, 0);

									break;

							}
#else
							CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
							CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI |(nSimId<<24), 1); //Notify MMI the call isn't in progress
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()API_CC_PROTOCOL_ERRORsend EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");

							CFW_GetUTI(hAo, &nUTI);
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CC==>>MMI case %d:\n", proc->nCalledFuncFlag);
							switch(proc->nCalledFuncFlag)
							{
								case 1:
									CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI |(nSimId<<24), 0xf0);
									break;
								case 2:
									CFW_PostNotifyEvent ( EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI |(nSimId<<24), 0xf0);
									break;
								case 4:
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI |(nSimId<<24), 0xf0);
									break;
								case 8:
									CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI |(nSimId<<24), 0xf0);
									break;
								default:
									CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_IND, proc->nIndex, iCauseValue, nUTI |(nSimId<<24), 0);
									break;
							}
#endif

							//Release the current index
							CFW_UnRegisterAO( CFW_CC_SRV_ID, hAo);
							CloseAudio();

							break;
						}

						default:
							proc->bRelHeldCallFlag = FALSE;
							break;

					}

				}
				else
				{
					CSW_TRACE(CFW_CC_TS_ID, "[warning]CFW_CcAoProc() IN MO DISC state, recieve unknown event, not consumed \n");				
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;

				}
				break;
			}
			case CFW_SM_CM_STATE_MTDISC:
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CFW_SM_CM_STATE_MTDISC \n");

				if(nEvtId == API_CC_ERROR_IND)
				{
					UINT8 iCauseValue = 0;
					api_CcErrorInd_t *p = (api_CcErrorInd_t*)nEvParam;

					//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
					iCauseValue = S_CFW_GET_CAUSE_VALUE(p->NwkCauseLen, p->NwkCause);
					//]] change end

					CSW_TRACE(CFW_CC_TS_ID, "in MTDISC state,CM local Error %x\n", p->Cause); 
					switch(p->Cause)
					{
						case API_LOW_LAYER_FAIL:
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CFW_SM_CM_STATE_MTDISC API_LOW_LAYER_FAIL\n");
						case API_NORMAL_RELEASE:
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CFW_SM_CM_STATE_MTDISC API_NORMAL_RELEASE\n");
							//CFW_IND_EVENT_INFO pIndInfo;
							//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

							sIndInfo.call = 0;

							proc->nCallState = CFW_CM_STATUS_RELEASED;
							//CFW_CfgSetExtendedErrorV0(ERR_CME_UNKNOWN);

#ifndef CFW_MULTI_SIM
							CFW_CfgSetIndicatorEventInfo(&sIndInfo);
							CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, GENERATE_URC_UTI(), 1); //Notify MMI the call isn't in progress
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case API_NORMAL_RELEASE:send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");
							//CFW_GetUTI(hAo, &nUTI);
							              //Add by lixp that speed up the hang up phone
              if( 0x11 == proc->nHoldFunction )
              {
                CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, proc->nIndex, 0, nUTI , 0x01);
              }
              else
              {
							  CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_IND, proc->nIndex, iCauseValue, nUTI, 0x0);
              }
#else
							CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
							CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, GENERATE_URC_UTI() |(nSimId <<24), 1); //Notify MMI the call isn't in progress
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case API_NORMAL_RELEASE:send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");
							//CFW_GetUTI(hAo, &nUTI);
              if( 0x11 == proc->nHoldFunction )
              {
                CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, proc->nIndex, 0, nUTI |(nSimId<<24), 0x01);
              }
              else
              {
                CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_IND, proc->nIndex, iCauseValue, nUTI |(nSimId <<24), 0x0);
              }
#endif
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() ===>> mmi CFW_SM_CM_STATE_MTDISC API_NORMAL_RELEASE\n");


              
              //Release the current index
							CFW_UnRegisterAO( CFW_CC_SRV_ID, hAo);
							CloseAudio();
							break;
						}
						default:
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_IND, proc->nIndex, iCauseValue, nUTI, 0x0);
#else
							CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_IND, proc->nIndex, iCauseValue, GENERATE_URC_UTI() |(nSimId <<24), 0x0);
#endif
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() ===>> mmi CFW_SM_CM_STATE_MTDISC EV_CFW_CC_RELEASE_CALL_IND\n");
							break;

					}

				}
				else
				{
					CSW_TRACE(CFW_CC_TS_ID, "[warning]CFW_CcAoProc() IN MT DISC state, recieve unknown event, not consumed \n");				
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;

				}
				break;

			}

			case CFW_SM_CM_STATE_MTPROC:
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: \n");
				if(pEvent->nParam1 == 0xff)
				{
					switch(proc->nCurSubState)
					{
						case SUBSTATE_MT_PROC_ATA:
							StartAudio();

							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: SUBSTATE_MT_PROC_ATA\n");
							proc->bUsedAFlag = TRUE;
							proc->nCalledFuncFlag = 2;
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()IN MTPROC, before SEND CONNECT 00 \n");
							proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
							CFW_SendSclMessage (API_CC_CONNECT_REQ, proc->pMsg);
#else
							CFW_SendSclMessage (API_CC_CONNECT_REQ, proc->pMsg, nSimId);
#endif
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()IN MTPROC, after SEND CONNECT 11 \n");
							//CFW_BalAudCallStart();
							break;
						case SUBSTATE_MT_PROC_CHLD1://accept call
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: SUBSTATE_MT_PROC_CHLD1 \n");
						case SUBSTATE_MT_PROC_CHLD2: //accept call
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: SUBSTATE_MT_PROC_CHLD2 \n");

							api_CcConnectReq_t* sOutMsg = (api_CcConnectReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_CcConnectReq_t));

							sOutMsg->TI = proc->nTI;
							sOutMsg->UserUserLen = 0;
							sOutMsg->CSALen = 0; 
							sOutMsg->Facility.StructValid = FALSE;
							proc->pMsg = sOutMsg;
							proc->nNextSubState = proc->nCurSubState;

							proc->bUsedChldFlag = TRUE;
							proc->nCalledFuncFlag = 4;
							proc->nHoldFunction = 0;
							CSW_TRACE(CFW_CC_TS_ID, "IN MTPROC, SEND CONNECT\n");
							proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
							CFW_SendSclMessage (API_CC_CONNECT_REQ, proc->pMsg);
#else
							CFW_SendSclMessage (API_CC_CONNECT_REQ, proc->pMsg, nSimId);
#endif

							break;

						case SUBSTATE_MT_PROC_CHLD0: //set UDUB to reject call
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: SUBSTATE_MT_PROC_CHLD0 \n");
							proc->bMODisc = TRUE;
							proc->nCalledFuncFlag = 4;
							proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM							
							CC_Disc(proc, CFW_CM_USER_BUSY);  // Added by Qiyan
#else
							CC_Disc(proc, CFW_CM_USER_BUSY, nSimId);  
#endif
							CSW_TRACE(CFW_CC_TS_ID, "IN MTPROC, after SEND RELEASECOMPL to reject\n");
							break;
						case SUBSTATE_MT_PROC_REJECT:
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: SUBSTATE_MT_PROC_REJECT \n");
							proc->bMODisc = TRUE;
							proc->nCalledFuncFlag = 8; //CFW_CmReleaseCall is called

							proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM							
							CC_Disc(proc, CFW_CM_USER_BUSY);  // Added by Qiyan
#else
							CC_Disc(proc, CFW_CM_USER_BUSY, nSimId);  
#endif
							CSW_TRACE(CFW_CC_TS_ID, "IN MTPROC, SEND CFW_CM_USER_BUSY to reject\n");
							break;
						default:
							break;
					}
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;                   
				}

				if(nEvtId == API_CC_CONNECTACK_IND)
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case CFW_SM_CM_STATE_MTPROC: API_CC_CONNECTACK_IND \n");
					//CFW_IND_EVENT_INFO pIndInfo;
					//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

					sIndInfo.call = 1;
					proc->nCallState = CFW_CM_STATUS_ACTIVE;
					CFW_SetAoState(hAo, CFW_SM_CM_STATE_ACTIVE);	
#ifndef CFW_MULTI_SIM
					CFW_CfgSetIndicatorEventInfo(&sIndInfo); 
					CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 1, proc->nIndex, nUTI, 1);//Notify MMI a call is in progress
#else
					CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId); 
					CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 1, proc->nIndex, GENERATE_URC_UTI()|(nSimId<<24), 1);//Notify MMI a call is in progress
#endif
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()send EV_CFW_CC_CALL_INFO_IND to MMI for InComing call in progress\n");

					//CFW_GetUTI(hAo, &nUTI);
					if(proc->bUsedAFlag)
					{
						proc->bUsedAFlag = FALSE;
						proc->nCalledFuncFlag = 0;
#ifndef CFW_MULTI_SIM							
						CFW_PostNotifyEvent ( EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP, 0, 0, nUTI, 0x0f);
#else
						CFW_PostNotifyEvent ( EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP, 0, 0, nUTI|(nSimId<<24), 0x0f);
#endif
						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()IN MTPROC, Send NOTIFY to MMI: EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP\n");

					}
					else if(proc->bUsedChldFlag)
					{
						proc->bUsedChldFlag = FALSE;
						proc->nCalledFuncFlag = 0;
#ifndef CFW_MULTI_SIM							
						CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, 0, 0, nUTI, 0x0f);
#else
						CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, 0, 0, nUTI|(nSimId<<24), 0x0f);
#endif
						CSW_TRACE(CFW_CC_TS_ID, "IN MTPROC, Send NOTIFY to MMI: EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP\n");

					}

					StartAudio();
				}
				else if(nEvtId == API_CC_ERROR_IND)
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: API_CC_ERROR_IND \n");
					UINT8 iCauseValue = 0;
					api_CcErrorInd_t *p = (api_CcErrorInd_t*)nEvParam;

					//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
					iCauseValue = S_CFW_GET_CAUSE_VALUE(p->NwkCauseLen, p->NwkCause);
					//[[ change end

					CSW_TRACE(CFW_CC_TS_ID, "in MTPROC state,CM local Error %x\n", p->Cause);                   
					switch(p->Cause)
					{
						case API_LOW_LAYER_FAIL:
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: API_LOW_LAYER_FAIL\n");
						}
						case API_NORMAL_RELEASE:
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: API_NORMAL_RELEASE\n");
							//CFW_IND_EVENT_INFO pIndInfo;
							//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

							sIndInfo.call = 0;
							proc->nCallState = CFW_CM_STATUS_RELEASED;
							//CFW_CfgSetExtendedErrorV0(ERR_CME_NO_CONNECTION_TO_PHONE); 

#ifndef CFW_MULTI_SIM
							CFW_CfgSetIndicatorEventInfo(&sIndInfo);
							CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI, 1); //Notify MMI the call isn't in progress
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() API_NORMAL_RELEASE send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");
							if(proc->bUsedAFlag)
							{
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: ==>mmi proc->bUsedAFlag\n");
								proc->bUsedAFlag = FALSE;
								CFW_PostNotifyEvent ( EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
							}
							else if(proc->bUsedChldFlag)
							{
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: ==>mmi proc->bUsedChldFlag\n");
								proc->bUsedChldFlag = FALSE;
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);

							}
              else
              {
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: bUsedAFlag[NULL] bUsedChldFlag[NULL]\n");
								proc->bUsedChldFlag = FALSE;
								//CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0x00);
          			CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_IND, proc->nIndex, iCauseValue, GENERATE_URC_UTI(), 0xf0);

              }
#else
							CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
							CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, GENERATE_URC_UTI() |(nSimId<<24), 1); //Notify MMI the call isn't in progress
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() API_NORMAL_RELEASE send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");
							if(proc->bUsedAFlag)
							{
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: ==>mmi proc->bUsedAFlag\n");
								proc->bUsedAFlag = FALSE;
								CFW_PostNotifyEvent ( EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI |(nSimId<<24), 0xf0);
							}
							else if(proc->bUsedChldFlag)
							{
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: ==>mmi proc->bUsedChldFlag\n");
								proc->bUsedChldFlag = FALSE;
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI |(nSimId<<24), 0xf0);

							}
              else
              {
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: bUsedAFlag[NULL] bUsedChldFlag[NULL]\n");
								proc->bUsedChldFlag = FALSE;
								//CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI |(nSimId<<24), 0xf0);
							  CFW_PostNotifyEvent ( EV_CFW_CC_RELEASE_CALL_IND, proc->nIndex, iCauseValue, GENERATE_URC_UTI() |(nSimId<<24) , 0x00);

              }              
#endif
							CFW_UnRegisterAO( CFW_CC_SRV_ID, hAo);
							CloseAudio();

							break;
						}
						case API_CC_TIMER_EXPIRY://change state to MODISC
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: API_CC_TIMER_EXPIRY\n");
							proc->nCallState = CFW_CM_STATUS_RELEASED;
							CFW_SetAoState(hAo, CFW_SM_CM_STATE_MODISC);
							break;
						default:
							break;

					}
				}
				else
				{
					CSW_TRACE(CFW_CC_TS_ID, "[warning]CFW_CcAoProc() case CFW_SM_CM_STATE_MTPROC: not consumed\n");
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;
				}
				break;

			}
			case CFW_SM_CM_STATE_ACTIVE:
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE:\n");
				if(pEvent->nParam1 == 0xff)
				{
					switch(proc->nCurSubState)
					{
						case SUBSTATE_ACTIVE_OR_MEETING_DTMF:
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE:SUBSTATE_ACTIVE_OR_MEETING_DTMF\n");
							proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
							CC_StartDTMF(proc, *((UINT8*)(proc->pMsg)));
							CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state,send START DTMF\n");
							CC_StopDTMF(proc);
#else
							CC_StartDTMF(proc, *((UINT8*)(proc->pMsg)), nSimId);
							CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state,send START DTMF\n");
							CC_StopDTMF(proc, nSimId);
#endif
							CSW_CC_FREE(proc->pMsg);
							proc->pMsg = NULL;
							CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state,send STOP DTMF\n");

							break;
						case SUBSTATE_ACTIVE_CHLD0://return ERROR Notify
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE:SUBSTATE_ACTIVE_CHLD0\n");
							proc->nCurSubState = proc->nNextSubState;
#ifndef  CFW_MULTI_SIM
							CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, CFW_CC_CAUSE_NOUSED_CAUSE, 
									(proc->nIndex << 16)|CFW_CC_ERR_REQUEST_IMPOSSIBLE, nUTI, 0xf0);
#else
							CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, CFW_CC_CAUSE_NOUSED_CAUSE, 
									(proc->nIndex << 16)|CFW_CC_ERR_REQUEST_IMPOSSIBLE, nUTI|(nSimId<<24), 0xf0);
#endif
							CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:CHLD0,send error NOTIFY to MMI\n");
							break;
						case SUBSTATE_ACTIVE_CHLD1://end call
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE:SUBSTATE_ACTIVE_CHLD1\n");
							proc->nCurSubState = proc->nNextSubState;
							proc->bMODisc = TRUE;
							proc->nCalledFuncFlag = 4;
#ifndef  CFW_MULTI_SIM						
							CC_Disc(proc, CFW_CM_NORMAL_CLEARING);
#else
							CC_Disc(proc, CFW_CM_NORMAL_CLEARING, nSimId);
#endif
							CFW_SetAoState(hAo, CFW_SM_CM_STATE_MODISC);
							CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:CHLD1,send DISCONNECT\n");

							break;

						case SUBSTATE_ACTIVE_CHLD2://Hold call
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: SUBSTATE_ACTIVE_CHLD2\n");
							proc->nCurSubState = proc->nNextSubState;
							proc->nCalledFuncFlag = 4;
#ifndef CFW_MULTI_SIM
							CFW_SendSclMessage (API_CC_CALLHOLD_REQ, proc->pMsg);
#else
							CFW_SendSclMessage (API_CC_CALLHOLD_REQ, proc->pMsg, nSimId);
#endif
							CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:CHLD2,send HOLD INDEX: %x TI: %x\n", proc->nIndex, ((api_CcCallHoldReq_t*)(proc->pMsg))->TI);

							break;
						case SUBSTATE_ACTIVE_CHLD3:
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: SUBSTATE_ACTIVE_CHLD3\n");
							proc->nCurSubState = proc->nNextSubState;
							proc->nCalledFuncFlag = 4;
#ifndef CFW_MULTI_SIM		
							CC_Facility(proc,  buildMPTY);
#else
							CC_Facility(proc,  buildMPTY, nSimId);
#endif
							CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:CHLD3,send BUILD MPTY\n");
							break;

						default:
							break;
					}
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;

				}
				
				switch(nEvtId)
				{
					case API_CC_CALLHOLD_CNF:
					{
						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: API_CC_CALLHOLD_CNF\n");
						//UINT8 nCSSI, nCSSU;
						//CFW_CfgGetSSN (&nCSSI, &nCSSU);

						proc->nCalledFuncFlag = 0;
						proc->nCallState = CFW_CM_STATUS_HELD;

						CFW_SetAoState(hAo, CFW_SM_CM_STATE_HELD);

						if(nCSSU)
#ifndef CFW_MULTI_SIM
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE:==>mmi EV_CFW_CC_CRSSINFO_IND\n");
							CFW_PostNotifyEvent ( EV_CFW_CC_CRSSINFO_IND , 0xff, 2, nUTI, 0x11);
						}
					//	CFW_GetUTI(hAo, &nUTI);//
						//CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
						CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
#else
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE:==>mmi EV_CFW_CC_CRSSINFO_IND\n");
							CFW_PostNotifyEvent ( EV_CFW_CC_CRSSINFO_IND , 0xff, 2, nUTI |(nSimId<<24), 0x11);
						}
						CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI |(nSimId<<24), 0);
#endif
						CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:receive HOLD CNF, SEND NOTIFY to MMI\n");

						if(proc->nHoldFunction == 0x2F)
						{
							CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:receive HOLD CNF, then check if there is a wating call\n");
#ifndef CFW_MULTI_SIM
							CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD2, 2) ;
#else
							CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD2, 2, nSimId) ;
#endif
							proc->nHoldFunction = 0;
						}

						break;
					}

					case API_CC_FACILITY_IND:
					{
						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: API_CC_FACILITY_IND\n");

						api_CcFacilityInd_t *p = (api_CcFacilityInd_t*)nEvParam;
						CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:receive BuildMPTY CNF,p->Facility[0].Status:%d\n", p->Facility[0].Status);
						CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:receive BuildMPTY CNF,p->Facility[0].ss_Operation:%d\n", p->Facility[0].ss_Operation);
#ifndef CFW_MULTI_SIM
						CC_SendCrssInfoNotify(&(p->Facility[0]));
						CC_SendCrssInfoNotify(&(p->Facility[1]));    //Added by Qiyan
#else
						CC_SendCrssInfoNotify(&(p->Facility[0]), nSimId);
						CC_SendCrssInfoNotify(&(p->Facility[1]), nSimId);    //Added by Qiyan
#endif
						proc->nCalledFuncFlag = 0;

						if(p->Facility[0].StructValid)
						{
							if((p->Facility[0].Status == API_CRSS_RETURN_RESULT) && (p->Facility[0].ss_Operation == buildMPTY))

							{
								proc->nCallState = CFW_CM_STATUS_ACTIVE;
								proc->bIsMpty = TRUE;

								CFW_SetAoState(hAo, CFW_SM_CM_STATE_INMEETING);	


#ifndef CFW_MULTI_SIM
								CC_ModifyOtherAoState(CFW_SM_CM_STATE_HELD, CFW_SM_CM_STATE_INMEETING, TRUE);
								CC_ModifyOtherAoState(CFW_SM_CM_STATE_MEETINGHELD, CFW_SM_CM_STATE_INMEETING, FALSE);
								if(nCSSU)
								{
									CFW_PostNotifyEvent ( EV_CFW_CC_CRSSINFO_IND , 0xff, 4, nUTI, 0x11);
								}
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
#else
								CC_ModifyOtherAoState(CFW_SM_CM_STATE_HELD, CFW_SM_CM_STATE_INMEETING, TRUE, nSimId);
								CC_ModifyOtherAoState(CFW_SM_CM_STATE_MEETINGHELD, CFW_SM_CM_STATE_INMEETING, FALSE, nSimId);
								if(nCSSU)
								{
									CFW_PostNotifyEvent ( EV_CFW_CC_CRSSINFO_IND , 0xff, 4, GENERATE_URC_UTI()|(nSimId<<24), 0x11);
								}
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI|(nSimId<<24), 0);
#endif
								CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:receive BuildMPTY CNF, SEND NOTIFY to MMI\n");

								/* Hold(2,0xff): ACTIVE + HOLD + WAITING */
								if( proc->nHoldFunction == 0x2)
								{
									//[[ change by dingmx 20080620,only hold MPTY OK, we can accept waiting call,
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: proc->nHoldFunction\n");
									proc->nHoldFunction = 0;  // this will be change by CC_HoldMPTY_Acceptcall
#ifndef CFW_MULTI_SIM
									CC_HoldMPTY_Acceptcall();
#else
									CC_HoldMPTY_Acceptcall(nSimId);
#endif

									//]] change end 20080620
								}

							}
							else if(p->Facility[0].Status == API_CRSS_ERROR_IND)
							{
								UINT32 param1 = 0;
								UINT32 param2 = 0;
								param1 = (p->Facility[0].ss_Code << 16)|p->Facility[0].ss_Operation;
								if(p->Facility[0].crss_ErrorInd.Cause == API_SS_REJECT_COMPONENT)
									param2 = (proc->nIndex << 16) | (p->Facility[0].crss_ErrorInd.ProblemCodeTag << 8) | p->Facility[0].crss_ErrorInd.Code;
								else
									param2 = (proc->nIndex << 16) | CFW_CRSS_ERR_RETURN_ERROR;
								//CFW_CfgSetExtendedErrorV0(ERR_CME_SS_NOT_EXECUTED);
								//CFW_GetUTI(hAo, &nUTI);
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, param1 , param2, nUTI, 0xf1);
#else
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, param1 , param2, nUTI|(nSimId<<24), 0xf1);
#endif
								CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state:receive BuildMpty ERROR, SEND NOTIFY to MMI\n");
							}
						}
						break;
					}
					case API_CC_ERROR_IND:
					{
						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: API_CC_ERROR_IND \n");

						UINT8 iCauseValue = 0;
						api_CcErrorInd_t *p = (api_CcErrorInd_t*)nEvParam;

						//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
						iCauseValue = S_CFW_GET_CAUSE_VALUE(p->NwkCauseLen, p->NwkCause);
						//[[ change end

						CSW_TRACE(CFW_CC_TS_ID, "in ACTIVE state,CM local Error %x\n", p->Cause);
						switch(p->Cause)
						{
							case API_LOW_LAYER_FAIL:
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: API_LOW_LAYER_FAIL\n");
							case API_NORMAL_RELEASE:
							{
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: API_NORMAL_RELEASE\n");

								//CFW_IND_EVENT_INFO pIndInfo;
								//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

								//pIndInfo.call = 0;
								//CFW_CfgSetIndicatorEventInfo(&pIndInfo);
								proc->nCallState = CFW_CM_STATUS_RELEASED;
								//CFW_CfgSetExtendedErrorV0(ERR_CME_PHONE_FAILURE); 
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI, 1); //Notify MMI the call isn't in progress
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()API_NORMAL_RELEASE:send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");

								//CFW_GetUTI(hAo, &nUTI);
								CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, GENERATE_URC_UTI()|(nSimId<<24), 1); //Notify MMI the call isn't in progress
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()API_NORMAL_RELEASE:send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");
								CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI|(nSimId<<24), 0xf0);
#endif
								//Release the current index
								CFW_UnRegisterAO( CFW_CC_SRV_ID, hAo);
								CloseAudio();
								break;
							}
							case API_CC_TIMER_EXPIRY:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: API_CC_TIMER_EXPIRY\n");
							case API_CALL_HOLD_REJECTED:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: API_CALL_HOLD_REJECTED\n");
							case API_CALL_RTRV_REJECTED:
								//CFW_CfgSetExtendedErrorV0(ERR_CME_SS_NOT_EXECUTED); 
								//CFW_GetUTI(hAo, &nUTI);
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: API_CALL_RTRV_REJECTED ==>>mmi\n");
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
								break;
							default:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: EV_CFW_CC_ERROR_IND ==>>mmi\n");
								CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, proc->nIndex, CFW_CC_ERR_REQUEST_IMPOSSIBLE, nUTI, 0x10);
#else
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI|(nSimId<<24), 0xf0);
								break;
							default:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: EV_CFW_CC_ERROR_IND ==>>mmi\n");
								CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, proc->nIndex, CFW_CC_ERR_REQUEST_IMPOSSIBLE, nUTI|(nSimId<<24), 0x10);
#endif
								break;

						}
						break;
					}
					default:
						CSW_TRACE(CFW_CC_TS_ID, "[warning] CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: not consumed\n");
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
						CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
						return ret;

				}


				break;
			case CFW_SM_CM_STATE_HELD:
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD: \n");
				if(pEvent->nParam1 == 0xff)
				{
					switch(proc->nCurSubState)
					{
						case SUBSTATE_HELD_CHLD0:
							proc->nCurSubState = proc->nNextSubState;
							proc->bRelHeldCallFlag = TRUE;
							proc->bMODisc = TRUE;
							proc->nCalledFuncFlag = 4;
#ifndef CFW_MULTI_SIM
							CC_Disc(proc, CFW_CM_NORMAL_CLEARING);
#else
							CC_Disc(proc, CFW_CM_NORMAL_CLEARING, nSimId);
#endif
							CFW_SetAoState(hAo, CFW_SM_CM_STATE_MODISC);
							CSW_TRACE(CFW_CC_TS_ID, "in HELD state:CHLD0,Send DISCONNECT\n");

							break;
						case SUBSTATE_HELD_CHLD1://retrieve call
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD: SUBSTATE_HELD_CHLD1\n");
						case SUBSTATE_HELD_CHLD2://retrieve call
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD: SUBSTATE_HELD_CHLD2\n");
						case SUBSTATE_HELD_CHLD2X:
							proc->nCalledFuncFlag = 4;
							proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
							CFW_SendSclMessage (API_CC_CALLRETRIEVE_REQ, proc->pMsg);
#else
							CFW_SendSclMessage (API_CC_CALLRETRIEVE_REQ, proc->pMsg, nSimId);
#endif
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD: in HELD state:CHLD1,2,2X,Send RETRIEVE ==>>stack\n");
							break;
							//case SUBSTATE_ACTIVE_CHLD1X:
							//    proc->nCurSubState = proc->nNextSubState;

							//    break;
							proc->nCurSubState = proc->nNextSubState;

						case SUBSTATE_HELD_CHLD3:
							proc->nCalledFuncFlag = 4;
							proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
							CC_Facility(proc,  buildMPTY);
#else
							CC_Facility(proc,  buildMPTY, nSimId);
#endif
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in HELD state:CHLD3,Send BuildMPTY\n");

							break;

						default:
							break;
					}
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;

				}
				switch(nEvtId)
				{
					case API_CC_CALLRETRIEVE_CNF:
					{
						//UINT8 nCSSI, nCSSU;
						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD: in HELD state:CHLD1,2,2X,Send RETRIEVE ==>>stack\n");
						//CFW_CfgGetSSN (&nCSSI, &nCSSU);

						proc->nCalledFuncFlag = 0;
						proc->nCallState = CFW_CM_STATUS_ACTIVE;

						CFW_SetAoState(hAo, CFW_SM_CM_STATE_ACTIVE);	

#ifndef CFW_MULTI_SIM
						if(nCSSU )
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD:API_CC_CALLRETRIEVE_CNF ==>>mmi\n");
							CFW_PostNotifyEvent ( EV_CFW_CC_CRSSINFO_IND , 0xff, 3, nUTI, 0x11);     
						}
						//CFW_GetUTI(hAo, &nUTI);
						CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
#else
						if(nCSSU )
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD:API_CC_CALLRETRIEVE_CNF ==>>mmi\n");

							CFW_PostNotifyEvent ( EV_CFW_CC_CRSSINFO_IND , 0xff, 3, GENERATE_URC_UTI()|(nSimId<<24), 0x11);     
						}
						CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI|(nSimId<<24), 0);
#endif
						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in HELD state:receive RETRIEVE CNF,Send NOTIFY to MMI\n");

						break;
					}

					case API_CC_FACILITY_IND:
					{
						api_CcFacilityInd_t *p = (api_CcFacilityInd_t*)nEvParam;

						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in HELD state:receive BuildMPTY CNF,p->Facility[0].Status:%d\n", p->Facility[0].Status);
						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in HELD state:receive BuildMPTY CNF,p->Facility[0].ss_Operation:%d\n", p->Facility[0].ss_Operation);

						proc->nCalledFuncFlag = 0;
#ifndef CFW_MULTI_SIM
						CC_SendCrssInfoNotify(&(p->Facility[0]));
						CC_SendCrssInfoNotify(&(p->Facility[1]));
#else
						CC_SendCrssInfoNotify(&(p->Facility[0]), nSimId);
						CC_SendCrssInfoNotify(&(p->Facility[1]), nSimId);
#endif

						if(p->Facility[0].StructValid)
						{
							if((p->Facility[0].Status == API_CRSS_RETURN_RESULT) && (p->Facility[0].ss_Operation == buildMPTY))
							{
								//UINT8 nCSSI, nCSSU;
								//CFW_CfgGetSSN (&nCSSI, &nCSSU);

								proc->nCallState = CFW_CM_STATUS_ACTIVE;
								proc->bIsMpty = TRUE;

								CFW_SetAoState(hAo, CFW_SM_CM_STATE_INMEETING);

#ifndef CFW_MULTI_SIM
								CC_ModifyOtherAoState(CFW_SM_CM_STATE_ACTIVE, CFW_SM_CM_STATE_INMEETING, TRUE);
								CC_ModifyOtherAoState(CFW_SM_CM_STATE_INMEETING, CFW_SM_CM_STATE_INMEETING, FALSE);
								if(nCSSU)
								{
									CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 4, nUTI, 0x11);//CSSU=4:call is in MPTY
								}
								//CFW_GetUTI(hAo, &nUTI);
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
#else
								CC_ModifyOtherAoState(CFW_SM_CM_STATE_ACTIVE, CFW_SM_CM_STATE_INMEETING, TRUE, nSimId);
								CC_ModifyOtherAoState(CFW_SM_CM_STATE_INMEETING, CFW_SM_CM_STATE_INMEETING, FALSE, nSimId);
								if(nCSSU)
								{
									CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 4, GENERATE_URC_UTI()|(nSimId<<24), 0x11);//CSSU=4:call is in MPTY
								}
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI|(nSimId<<24), 0);

#endif
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in HELD state:receive BuildMPTY CNF,Send NOTIFY to MMI\n");

							}
							else if(p->Facility[0].Status == API_CRSS_ERROR_IND)
							{
								UINT32 param1 = 0;
								UINT32 param2 = 0;
								param1 = (p->Facility[0].ss_Code << 16)|p->Facility[0].ss_Operation;
								if(p->Facility[0].crss_ErrorInd.Cause == API_SS_REJECT_COMPONENT)
									param2 = (proc->nIndex << 16) | (p->Facility[0].crss_ErrorInd.ProblemCodeTag << 8) | p->Facility[0].crss_ErrorInd.Code;
								else
									param2 = (proc->nIndex << 16) | CFW_CRSS_ERR_RETURN_ERROR;

#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, param1, param2, nUTI, 0xf1);
#else
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, param1, param2, nUTI|(nSimId<<24), 0xf1);
#endif
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in HELD state:receive BuildMPTY ERROR,Send NOTIFY to MMI\n");

							}
						}

						break;
					}
					break;
					case API_CC_ERROR_IND:
					{
						CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD:API_CC_ERROR_IND\n");

						UINT8 iCauseValue = 0;
						api_CcErrorInd_t *p = (api_CcErrorInd_t*)nEvParam;

						//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
						iCauseValue = S_CFW_GET_CAUSE_VALUE(p->NwkCauseLen, p->NwkCause);
						//[[ change end

						CSW_TRACE(CFW_CC_TS_ID, "in HELD state,CC local Error 0x%x\n", p->Cause);
						switch(p->Cause)
						{
							case API_LOW_LAYER_FAIL:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD:API_LOW_LAYER_FAIL\n");
							case API_NORMAL_RELEASE:
							{
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD:API_NORMAL_RELEASE\n");

								//CFW_IND_EVENT_INFO pIndInfo;
								//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

								sIndInfo.call = 0;
								proc->nCallState = CFW_CM_STATUS_RELEASED;
								// CFW_CfgSetExtendedErrorV0(ERR_CME_PHONE_FAILURE); 

#ifndef CFW_MULTI_SIM
								CFW_CfgSetIndicatorEventInfo(&sIndInfo);
								CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI, 1); //Notify MMI the call isn't in progress
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");

								//CFW_GetUTI(hAo, &nUTI);
								CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
#else
								CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
								CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, GENERATE_URC_UTI()|(nSimId<<24), 1); //Notify MMI the call isn't in progress
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");

								//CFW_GetUTI(hAo, &nUTI);
								CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI|(nSimId<<24), 0xf0);
#endif
								//Release the current index
								CFW_UnRegisterAO( CFW_CC_SRV_ID, hAo);
								CloseAudio();

								break;
							}
							case API_CC_TIMER_EXPIRY:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD:API_CC_TIMER_EXPIRY\n");
							case API_CALL_HOLD_REJECTED:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD:API_CALL_HOLD_REJECTED\n");
							case API_CALL_RTRV_REJECTED:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_HELD:API_CALL_RTRV_REJECTED\n");

								//CFW_CfgSetExtendedErrorV0(ERR_CME_SS_NOT_EXECUTED); 
								proc->nCalledFuncFlag = 0;
								//CFW_GetUTI(hAo, &nUTI);
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
#else
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI|(nSimId<<24), 0xf0);
#endif
								break;
							default:
								break;

						}
						break;
					}      
					default:
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
						CSW_TRACE(CFW_CC_TS_ID, "[warning]CFW_CcAoProc() case CFW_SM_CM_STATE_HELD:not consumed\n");
						CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
						return ret;

					}
					break;
				case CFW_SM_CM_STATE_INMEETING:
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() CFW_SM_CM_STATE_INMEETING:\n");
					if(pEvent->nParam1 == 0xff)
					{
						switch(proc->nCurSubState)
						{
							case SUBSTATE_ACTIVE_OR_MEETING_DTMF:
								proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
								CC_StartDTMF(proc, *((UINT8*)(proc->pMsg)));
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state,send START DTMF\n");
								CC_StopDTMF(proc);
#else
								CC_StartDTMF(proc, *((UINT8*)(proc->pMsg)), nSimId);
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state,send START DTMF\n");
								CC_StopDTMF(proc,nSimId);
#endif
								CSW_CC_FREE(proc->pMsg);
								proc->pMsg = NULL;
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state,send STOP DTMF\n");

								break;

							case SUBSTATE_INMEETING_CHLD0: //Notify MMI ERROR
								proc->nCurSubState = proc->nNextSubState;
								//CFW_CfgSetExtendedErrorV0(ERR_CME_UNKNOWN);
								//CFW_GetUTI(hAo, &nUTI);
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , CFW_CC_CAUSE_NOUSED_CAUSE, (proc->nIndex << 16)|CFW_CC_ERR_REQUEST_IMPOSSIBLE, nUTI, 0xf0);
#else
								CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , CFW_CC_CAUSE_NOUSED_CAUSE, 
									(proc->nIndex << 16)|CFW_CC_ERR_REQUEST_IMPOSSIBLE, nUTI|(nSimId<<24), 0xf0);
#endif
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:CHLD0,send error NOTIFY to MMI\n");                     
								break;
							case SUBSTATE_INMEETING_CHLD1:
								proc->nCurSubState = proc->nNextSubState;
								proc->bMODisc = TRUE;
								proc->nCalledFuncFlag = 4;
#ifndef CFW_MULTI_SIM
								CC_Disc(proc, CFW_CM_NORMAL_CLEARING);
#else
								CC_Disc(proc, CFW_CM_NORMAL_CLEARING, nSimId);
#endif
								CFW_SetAoState(hAo, CFW_SM_CM_STATE_MODISC);
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:CHLD1, SEND DISCONNECT\n");

								break;
								// case SUBSTATE_ACTIVE_CHLD1X:
								//     proc->nCurSubState = proc->nNextSubState;

								//     break;
							case SUBSTATE_INMEETING_CHLD2:
								proc->nCurSubState = proc->nNextSubState;
								proc->nCalledFuncFlag = 4;
#ifndef CFW_MULTI_SIM
								CC_Facility(proc, holdMPTY);
#else
								CC_Facility(proc, holdMPTY, nSimId);
#endif
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:CHLD2, SEND Hold MPTY\n");
								break;
							case SUBSTATE_INMEETING_CHLD2X:
								proc->nCurSubState = proc->nNextSubState;
								proc->nCalledFuncFlag = 4;
#ifndef CFW_MULTI_SIM								
								if(proc->bNeedSplitFlag)
								{
									CC_Facility(proc, buildMPTY);
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:CHLD2x, SEND BuildMPTY\n");
								}
								else
								{
									CC_Facility(proc, splitMPTY);
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:CHLD2x, SEND SplitMPTY\n"); 

								}
#else
								if(proc->bNeedSplitFlag)
								{
									CC_Facility(proc, buildMPTY, nSimId);
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:CHLD2x, SEND BuildMPTY\n");
								}
								else
								{
									CC_Facility(proc, splitMPTY, nSimId);
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:CHLD2x, SEND SplitMPTY\n"); 
								}
#endif

								break;

							case SUBSTATE_INMEETING_CHLD3:
								proc->nCalledFuncFlag = 4;
								proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
								CC_Facility(proc,  buildMPTY);
#else
								CC_Facility(proc,  buildMPTY, nSimId);
#endif
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:CHLD3, SEND BuildMPTY\n");

								break;

							default:
								break;
						}
						CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
						return ret;

					}
					switch(nEvtId)
					{
						case API_CC_FACILITY_IND:
						{
							api_CcFacilityInd_t *p = (api_CcFacilityInd_t*)nEvParam;
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:receive API_CC_FACILITY_IND,p->Facility[0].Status:%d\n", p->Facility[0].Status);
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:receive API_CC_FACILITY_IND,p->Facility[0].ss_Operation:%d\n", p->Facility[0].ss_Operation);

							if(p->Facility[0].StructValid)
							{
								if(p->Facility[0].Status == API_CRSS_RETURN_RESULT)
								{
									switch(p->Facility[0].ss_Operation)
									{

										case buildMPTY:
										{
											proc->nCallState = CFW_CM_STATUS_ACTIVE;
											proc->bIsMpty = TRUE;
											CFW_SetAoState(hAo, CFW_SM_CM_STATE_INMEETING);	
#ifndef CFW_MULTI_SIM
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_HELD, CFW_SM_CM_STATE_INMEETING, TRUE);
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_INMEETING, CFW_SM_CM_STATE_INMEETING, FALSE);
											if(proc->bNeedSplitFlag)
											{
												CC_Facility(proc, splitMPTY);
#else
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_HELD, CFW_SM_CM_STATE_INMEETING, TRUE, nSimId);
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_INMEETING, CFW_SM_CM_STATE_INMEETING, FALSE, nSimId);
											if(proc->bNeedSplitFlag)
											{
												CC_Facility(proc, splitMPTY, nSimId);
#endif
												proc->bNeedSplitFlag = 0;  // Added by Qiyan 
												CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:Receive BUildMPTY CNF,then send SplitMPTY\n");

											}
											else
											{
												//UINT8 nCSSI, nCSSU;
												//CFW_CfgGetSSN (&nCSSI, &nCSSU);
												proc->nCalledFuncFlag = 0;
#ifndef CFW_MULTI_SIM
												if(nCSSU)
													CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 4, nUTI, 0x11);//CSSU=4:call is in MPTY
												//CFW_GetUTI(hAo, &nUTI);
												CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, proc->nIndex, 0, nUTI, 0);   
#else
												if(nCSSU)
													CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 4, GENERATE_URC_UTI()|(nSimId<<24), 0x11);//CSSU=4:call is in MPTY
												//CFW_GetUTI(hAo, &nUTI);
												CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, proc->nIndex, 0, nUTI|(nSimId<<24), 0);   
#endif
												CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:Receive BUildMPTY CNF,Send NOTIFY to MMI\n");
												/* Hold(2,0xff): INMEETING + HOLD + WAITING */
												if( proc->nHoldFunction == 0x2)
												{
													// change by dingmx20080620,only hold MPTY OK, we can accept waiting call,
													CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc() case CFW_SM_CM_STATE_ACTIVE: proc->nHoldFunction\n");
													proc->nHoldFunction = 0;
#ifndef CFW_MULTI_SIM
													CC_HoldMPTY_Acceptcall();
#else
													CC_HoldMPTY_Acceptcall(nSimId);
#endif
													// CC_HoldMPTY();
													// CC_AcceptWaitingCall( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD2, 2) ; 
													// proc->nHoldFunction = 0;
												}

											}
											break;
										}
										case holdMPTY:
										{
											//UINT8 nCSSI, nCSSU;
											//CFW_CfgGetSSN (&nCSSI, &nCSSU);
											if(nCSSU)
											{
#ifndef CFW_MULTI_SIM
												CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 2, nUTI, 0x11);//CSSU=2:call is Held
#else
												CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 2, GENERATE_URC_UTI()|(nSimId<<24), 0x11);
#endif

												//CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 4, nUTI, 0x11);//CSSU=4:call is in MPTY hameina[-]2007.11.27
											}
											proc->nCallState = CFW_CM_STATUS_HELD;
											proc->bIsMpty = TRUE;
											proc->nCalledFuncFlag = 0;
											CFW_SetAoState(hAo, CFW_SM_CM_STATE_MEETINGHELD);	
#ifndef CFW_MULTI_SIM											
											CC_ModifyToMeetingHldState(hAo, CFW_SM_CM_STATE_INMEETING, CFW_SM_CM_STATE_MEETINGHELD);
											CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
#else
											CC_ModifyToMeetingHldState(hAo, CFW_SM_CM_STATE_INMEETING, CFW_SM_CM_STATE_MEETINGHELD, nSimId);
											CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI|(nSimId<<24), 0);
#endif
											CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:Receive HoldMPTY CNF,Send NOTIFY to MMI\n");

											if(proc->nHoldFunction == 0x2F)
											{
												CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:Receive HoldMPTY CNF, then check if there is a wating call\n");
#ifndef CFW_MULTI_SIM											
												CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD2, 2) ;
#else
												CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD2, 2, nSimId) ;
#endif
												proc->nHoldFunction = 0;
											}

										}
										break;
										case splitMPTY:
										{
											proc->nCallState = CFW_CM_STATUS_ACTIVE;
											proc->bIsMpty = FALSE;
											proc->nCalledFuncFlag = 0;
											CFW_SetAoState(hAo, CFW_SM_CM_STATE_ACTIVE);	
											//CFW_GetUTI(hAo, &nUTI);
#ifndef CFW_MULTI_SIM
											CC_ModifyToMeetingHldState(hAo, CFW_SM_CM_STATE_INMEETING, CFW_SM_CM_STATE_MEETINGHELD);
											CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
#else
											CC_ModifyToMeetingHldState(hAo, CFW_SM_CM_STATE_INMEETING, CFW_SM_CM_STATE_MEETINGHELD, nSimId);
											CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI|(nSimId<<24), 0);
#endif
											CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:Receive SplitMPTY CNF,Send NOTIFY to MMI\n");

											break;
										}
										default:
											break;

									}
								}
								else if(p->Facility[0].Status == API_CRSS_ERROR_IND)
								{
									UINT32 param1 = 0;
									UINT32 param2 = 0;
									param1 = (p->Facility[0].ss_Code << 16)|p->Facility[0].ss_Operation;
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:facility[0].Status=API_CRSS_ERROR_IND\n");
									CSW_TRACE(CFW_CC_TS_ID, "crss_ErrorInd.Cause=%d, ProblemCodeTag=%d, p->Facility[0].crss_ErrorInd.Code=%d\n", 
										p->Facility[0].crss_ErrorInd.Cause,
										p->Facility[0].crss_ErrorInd.ProblemCodeTag,
										p->Facility[0].crss_ErrorInd.Code
										);
									
									if(p->Facility[0].crss_ErrorInd.Cause == API_SS_REJECT_COMPONENT)
										param2 = (proc->nIndex << 16) | (p->Facility[0].crss_ErrorInd.ProblemCodeTag << 8) | p->Facility[0].crss_ErrorInd.Code;
									else
										param2 = (proc->nIndex << 16) | CFW_CRSS_ERR_RETURN_ERROR;
									//CFW_CfgSetExtendedErrorV0(ERR_CME_SS_NOT_EXECUTED);
									proc->nCalledFuncFlag = 0;
									//CFW_GetUTI(hAo, &nUTI);
#ifndef CFW_MULTI_SIM
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, param1, param2, nUTI, 0xf1);
#else
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, param1, param2, nUTI|(nSimId<<24), 0xf1);
#endif
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state:Send ERROR NOTIFY to MMI\n");

								}
								//[[hameina[+] 2008.12.8 send meeting notification
								else if(   API_CRSS_CALL_RETRIEVED == p->Facility[0].Status||
									API_CRSS_CALL_IS_WAITING ==p->Facility[0].Status||
									API_CRSS_CALL_ON_HOLD == p->Facility[0].Status)
								{
#ifndef CFW_MULTI_SIM
									CC_SendCrssInfoNotify(p->Facility);
#else
									CC_SendCrssInfoNotify(p->Facility, nSimId);
#endif
								}
								//]]hameina[+]2008.12.8 send meeting notification
							}

							break;
						}
						case API_CC_ERROR_IND:
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()CFW_SM_CM_STATE_INMEETING in INMEETING state:API_CC_ERROR_IND\n");
							UINT8 iCauseValue = 0;
							api_CcErrorInd_t *p = (api_CcErrorInd_t*)nEvParam;

							//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
							iCauseValue = S_CFW_GET_CAUSE_VALUE(p->NwkCauseLen, p->NwkCause);
							//[[ change end

							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in INMEETING state,CM local Error %x\n", p->Cause);
							switch(p->Cause)
							{
								case API_LOW_LAYER_FAIL:
								{
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()CFW_SM_CM_STATE_INMEETING :API_CC_ERROR_IND API_LOW_LAYER_FAIL\n");
								} 
								case API_NORMAL_RELEASE:
								{
									//CFW_IND_EVENT_INFO pIndInfo;
									//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

									sIndInfo.call = 0;
									proc->nCallState = CFW_CM_STATUS_RELEASED;
									//CFW_CfgSetExtendedErrorV0(ERR_CME_PHONE_FAILURE); 
#ifndef CFW_MULTI_SIM
									CFW_CfgSetIndicatorEventInfo(&sIndInfo);
									CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI, 1); //Notify MMI the call isn't in progress
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()11send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");

									//CFW_GetUTI(hAo, &nUTI);
									CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
#else
									CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
									CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, GENERATE_URC_UTI()|(nSimId<<24), 1); 
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()11send EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");
									CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI|(nSimId<<24), 0xf0);
#endif
									//Release the current index
									CFW_UnRegisterAO( CFW_CC_SRV_ID, hAo);
									CloseAudio();

									break;
								}
								case API_CC_TIMER_EXPIRY:
								{
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()CFW_SM_CM_STATE_INMEETING :API_CC_ERROR_IND API_CC_TIMER_EXPIRY\n");
								}
								case API_CALL_HOLD_REJECTED:
								{
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()CFW_SM_CM_STATE_INMEETING :API_CC_ERROR_IND API_CALL_HOLD_REJECTED\n");
								}
								case API_CALL_RTRV_REJECTED:
								{
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()CFW_SM_CM_STATE_INMEETING :API_CC_ERROR_IND API_CALL_RTRV_REJECTED\n");
									//CFW_CfgSetExtendedErrorV0(ERR_CME_SS_NOT_EXECUTED); 
									proc->nCalledFuncFlag = 0;
									//CFW_GetUTI(hAo, &nUTI);
#ifndef CFW_MULTI_SIM
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
#else
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI|(nSimId<<24), 0xf0);
#endif
								}
									break;
								default:
									break;

							}
							break;
						}      
						default:
							CSW_TRACE(CFW_CC_TS_ID, "[warning]CFW_CcAoProc()CFW_SM_CM_STATE_INMEETING :not consumed\n");

							CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
							CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
							return ret;
					}
					break;
				case CFW_SM_CM_STATE_MEETINGHELD:
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case CFW_SM_CM_STATE_MEETINGHELD:\n");

					if(pEvent->nParam1 == 0xff)
					{
						switch(proc->nCurSubState)
						{
							case SUBSTATE_MEETINGHLD_CHLD0:
								proc->nCurSubState = proc->nNextSubState;
								proc->bRelHeldCallFlag = TRUE;
								proc->bMODisc = TRUE;
								proc->nCalledFuncFlag = 4;
#ifndef CFW_MULTI_SIM
								CC_Disc(proc, CFW_CM_NORMAL_CLEARING);
#else
								CC_Disc(proc, CFW_CM_NORMAL_CLEARING, nSimId);
#endif
								CFW_SetAoState(hAo, CFW_SM_CM_STATE_MODISC);
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:CHLD0,Send DISCONNECT\n");
								break;
							case SUBSTATE_MEETINGHLD_CHLD1:
							{
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case CFW_SM_CM_STATE_MEETINGHELD:SUBSTATE_MEETINGHLD_CHLD1\n");
							}
							case SUBSTATE_MEETINGHLD_CHLD2:
								proc->nCalledFuncFlag = 4;
								proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
								CC_Facility(proc, retrieveMPTY);
#else
								CC_Facility(proc, retrieveMPTY, nSimId);
#endif

								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:CHLD1,2,Send retrieveMPTY INDEX: %x TI: %x \n", proc->nIndex, proc->nTI);
								break;

							case SUBSTATE_MEETINGHLD_CHLD2X:
								proc->nCalledFuncFlag = 4;
								proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
								if(proc->bNeedSplitFlag)
								{
									CC_Facility(proc, buildMPTY);
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:CHLD2x,Send BuildMPTY\n");
								}
								else
								{
									CC_Facility(proc, retrieveMPTY);
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:CHLD2x,Send RetrieveMPTY\n");

								}
#else
								if(proc->bNeedSplitFlag)
								{
									CC_Facility(proc, buildMPTY, nSimId);
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:CHLD2x,Send BuildMPTY\n");
								}
								else
								{
									CC_Facility(proc, retrieveMPTY, nSimId);
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:CHLD2x,Send RetrieveMPTY\n");

								}

#endif
								break;
							case SUBSTATE_MEETINGHLD_CHLD3:
								proc->nCalledFuncFlag = 4;
								proc->nCurSubState = proc->nNextSubState;
#ifndef CFW_MULTI_SIM
								CC_Facility(proc,  buildMPTY);
#else
								CC_Facility(proc,  buildMPTY, nSimId);
#endif
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:CHLD3,Send BuildMPTY\n");

								break;

							default:
								break;
						}
						CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
						return ret;

					}
					switch(nEvtId)
					{
						case API_CC_FACILITY_IND:
						{
							api_CcFacilityInd_t *p = (api_CcFacilityInd_t*)nEvParam;

							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:receive BuildMPTY CNF,p->Facility[0].Status:%d\n", p->Facility[0].Status);
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:receive BuildMPTY CNF,p->Facility[0].ss_Operation:%d\n", p->Facility[0].ss_Operation);

							if(p->Facility[0].StructValid)
							{                               
								if(p->Facility[0].Status == API_CRSS_RETURN_RESULT)
								{
									switch(p->Facility[0].ss_Operation)
									{

										case buildMPTY:
										{
											//UINT8 nCSSI, nCSSU;
											//CFW_CfgGetSSN (&nCSSI, &nCSSU);
											if(nCSSU)
											{
#ifndef CFW_MULTI_SIM
												CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 3, nUTI, 0x11);//CSSU=2:call is Retrieved

												CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 4, nUTI, 0x11);//CSSU=4:call is in MPTY
#else
												CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 3, GENERATE_URC_UTI()|(nSimId<<24), 0x11);//CSSU=2:call is Retrieved

												CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 4, GENERATE_URC_UTI()|(nSimId<<24), 0x11);//CSSU=4:call is in MPTY
#endif
											}
											proc->nCallState = CFW_CM_STATUS_ACTIVE;
											proc->bIsMpty = TRUE;
											CFW_SetAoState(hAo, CFW_SM_CM_STATE_INMEETING);	
#ifndef CFW_MULTI_SIM
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_ACTIVE, CFW_SM_CM_STATE_INMEETING, TRUE);
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_MEETINGHELD, CFW_SM_CM_STATE_INMEETING, FALSE);

											if(proc->bNeedSplitFlag)
											{
												CC_Facility(proc, splitMPTY);
#else
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_ACTIVE, CFW_SM_CM_STATE_INMEETING, TRUE, nSimId);
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_MEETINGHELD, CFW_SM_CM_STATE_INMEETING, FALSE, nSimId);

											if(proc->bNeedSplitFlag)
											{
												CC_Facility(proc, splitMPTY,nSimId);
#endif
												proc->bNeedSplitFlag = 0;                                                             
												CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:Receive BuildMPTY CNF,then send SplitMPTY\n");

											}
											else
											{
												//CFW_GetUTI(hAo, &nUTI);
												proc->nCalledFuncFlag = 0;
#ifndef CFW_MULTI_SIM
												CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
#else
												CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI|(nSimId<<24), 0);
#endif
												CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:Receive BuildMPTY CNF,Send NOTIFY to MMI\n");
											}
											break;
										}
										case splitMPTY:
										{
											proc->nCallState = CFW_CM_STATUS_ACTIVE;
											proc->bIsMpty = FALSE;
											proc->nCalledFuncFlag = 0;
											CFW_SetAoState(hAo, CFW_SM_CM_STATE_ACTIVE);	
#ifndef CFW_MULTI_SIM
											CC_ModifyToMeetingHldState(hAo, CFW_SM_CM_STATE_MEETINGHELD, CFW_SM_CM_STATE_MEETINGHELD);
											CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
#else
											CC_ModifyToMeetingHldState(hAo, CFW_SM_CM_STATE_MEETINGHELD, CFW_SM_CM_STATE_MEETINGHELD, nSimId);
											CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI|(nSimId<<24), 0);
#endif
											CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:Receive SplitMPTY CNF,then send NOTIFY to MMI\n");

											break;
										}
										case retrieveMPTY:
										{
											//UINT8 nCSSI, nCSSU;
											proc->nCalledFuncFlag = 0;
											//CFW_CfgGetSSN (&nCSSI, &nCSSU);
											if(nCSSU)
											{
#ifndef CFW_MULTI_SIM
												CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 3, nUTI, 0x11);//CSSU=3:call is Held
#else
												CFW_PostNotifyEvent(EV_CFW_CC_CRSSINFO_IND, 0xff, 3, GENERATE_URC_UTI()|(nSimId<<24), 0x11);//CSSU=3:call is Held
#endif
											}

											proc->nCallState = CFW_CM_STATUS_ACTIVE;
											proc->bIsMpty = TRUE;
											CFW_SetAoState(hAo, CFW_SM_CM_STATE_INMEETING);	
#ifndef CFW_MULTI_SIM
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_MEETINGHELD, CFW_SM_CM_STATE_INMEETING, FALSE);

											if(proc->bNeedRetrieve)
											{
												CC_Facility(proc, splitMPTY);
#else
											CC_ModifyOtherAoState(CFW_SM_CM_STATE_MEETINGHELD, CFW_SM_CM_STATE_INMEETING, FALSE, nSimId);

											if(proc->bNeedRetrieve)
											{
												CC_Facility(proc, splitMPTY, nSimId);
#endif
												proc->bNeedRetrieve = 0;                                                             
												CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:Receive RetrieveMPTY CNF,then send SplitMPTY\n");
											}
											else
											{
#ifndef CFW_MULTI_SIM
												CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI, 0);
#else
												CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP , proc->nIndex, 0, nUTI|(nSimId<<24), 0);
#endif
												CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state:Receive RetrieveMPTY CNF,then send NOTIFY to MMI\n");
											}

											break;
										}


										default:
											break;

									}
								}
								else if(p->Facility[0].Status == API_CRSS_ERROR_IND)
								{
									UINT32 param1 = 0;
									UINT32 param2 = 0;
									param1 = (p->Facility[0].ss_Code << 16)|p->Facility[0].ss_Operation;
									if(p->Facility[0].crss_ErrorInd.Cause == API_SS_REJECT_COMPONENT)
										param2 = (proc->nIndex << 16) | (p->Facility[0].crss_ErrorInd.ProblemCodeTag << 8) | p->Facility[0].crss_ErrorInd.Code;
									else
										param2 = (proc->nIndex << 16) | CFW_CRSS_ERR_RETURN_ERROR;
									proc->nCalledFuncFlag = 0;

#ifndef CFW_MULTI_SIM
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, param1, param2, nUTI, 0xf1);
#else
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, param1, param2, nUTI|(nSimId<<24), 0xf1);
#endif
								}
							}
							break;
						}
						case API_CC_ERROR_IND:
						{
							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case CFW_SM_CM_STATE_MEETINGHELD: API_CC_ERROR_IND:\n");

							UINT8 iCauseValue = 0;
							api_CcErrorInd_t *p = (api_CcErrorInd_t*)nEvParam;
							proc->nCalledFuncFlag = 0;

							//[[ changed by dingmx 20080702, according TS24008 10.5.4.11	
							iCauseValue = S_CFW_GET_CAUSE_VALUE(p->NwkCauseLen, p->NwkCause);
							//]] change end

							CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()in MEETINGHELD state,CM local Error %x\n", p->Cause);
							switch(p->Cause)
							{
								case API_LOW_LAYER_FAIL:
								{
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case CFW_SM_CM_STATE_MEETINGHELD: API_CC_ERROR_IND:API_LOW_LAYER_FAIL\n");
								}
								case API_NORMAL_RELEASE:
								{
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case CFW_SM_CM_STATE_MEETINGHELD: API_CC_ERROR_IND:API_NORMAL_RELEASE\n");
									//CFW_IND_EVENT_INFO pIndInfo;
									//CFW_CfgGetIndicatorEventInfo(&pIndInfo); 

									sIndInfo.call = 0;
									proc->nCallState = CFW_CM_STATUS_RELEASED;
									//CFW_CfgSetExtendedErrorV0(ERR_CME_PHONE_FAILURE); 
#ifndef CFW_MULTI_SIM
									CFW_CfgSetIndicatorEventInfo(&sIndInfo);
									CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, nUTI, 1); //Notify MMI the call isn't in progress
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()send22 EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");

									//CFW_GetUTI(hAo, &nUTI);
									CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
#else
									CFW_CfgSetIndicatorEventInfo(&sIndInfo, nSimId);
									CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, proc->nIndex, GENERATE_URC_UTI()|(nSimId<<24), 1); //Notify MMI the call isn't in progress
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()send22 EV_CFW_CC_CALL_INFO_IND to MMI for Call isn't progress\n");
									CFW_PostNotifyEvent ( EV_CFW_CC_ERROR_IND, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI|(nSimId<<24), 0xf0);
#endif
									//Release the current index
									CFW_UnRegisterAO( CFW_CC_SRV_ID, hAo);
									CloseAudio();

									break;
								}
								case API_CC_TIMER_EXPIRY:
								{
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case CFW_SM_CM_STATE_MEETINGHELD: API_CC_ERROR_IND:API_CC_TIMER_EXPIRY\n");
								}
								case API_CALL_HOLD_REJECTED:
								{
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case CFW_SM_CM_STATE_MEETINGHELD: API_CC_ERROR_IND:API_CALL_HOLD_REJECTED\n");
								}
								case API_CALL_RTRV_REJECTED:
								{
									CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAoProc()case CFW_SM_CM_STATE_MEETINGHELD: API_CC_ERROR_IND:API_CALL_RTRV_REJECTED\n");
									// CFW_CfgSetExtendedErrorV0(ERR_CME_SS_NOT_EXECUTED); 
									//CFW_GetUTI(hAo, &nUTI);
#ifndef CFW_MULTI_SIM
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI, 0xf0);
#else
									CFW_PostNotifyEvent ( EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP, iCauseValue, (proc->nIndex << 16)|p->Cause, nUTI|(nSimId<<24), 0xf0);
#endif
								}
									break;
								default:
									break;

							}
							break;
						}      
						default:
							CSW_TRACE(CFW_CC_TS_ID, "[warning]CFW_CcAoProc()case CFW_SM_CM_STATE_MEETINGHELD: :not consumed\n");
							CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
							CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
							return ret;
					}
					break;

				default:
				{
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_NOT_CONSUMED);
					CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcAoProc()default, UNKNOWN AO status\n");
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
					return ret;
				}
			}
		}
	      // the event are processed one time, and don't need to process again in this state.
	      CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);		

      CSW_PROFILE_FUNCTION_EXIT(CFW_CcAoProc);
      return(ret);
}


//
//===================================================================================
// CM Interface by MMI	
//

//
// Initiate Speech Call
//===================================================================================
UINT32 CFW_CcInitiateSpeechCall (
                                 CFW_DIALNUMBER* pDialNumber,
                                 UINT16 nUTI
#ifdef CFW_MULTI_SIM
                                 ,CFW_SIM_ID nSimId
#endif
                                 )
{
    UINT8  nCPNLen;
    UINT8  nPhoneNumLen;	
    HAO    hSm;
    CM_SM_INFO* proc=NULL;
    UINT32 ret = ERR_SUCCESS;
    api_CcSetupReq_t* sOutMsg = 0;
    UINT32 nAllState = 0;
#ifdef CFW_MULTI_SIM
	UINT8 nAOCount=0;
#endif

	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcInitiateSpeechCall() begin, numbertype=%d \n", pDialNumber->nType);
	CSW_TC_MEMBLOCK(CFW_CC_TS_ID, pDialNumber->pDialNumber, pDialNumber->nDialNumberSize, 16);
	CSW_PROFILE_FUNCTION_ENTER(CFW_CcInitiateSpeechCall);
	
#ifndef CFW_MULTI_SIM
    nAllState = GetCallStatus();
#else
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcInitiateSpeechCall(), sim=%d \n", nSimId);
	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcInitiateSpeechCall() check SIM ID failed \n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCall);
		return ret;
	}
    nAllState = GetCallStatus(nSimId);
#endif
	
    if( ( nAllState & CC_STATE_ACTIVE ) || ( nAllState & CC_STATE_WAITING )
        || ( nAllState & CC_STATE_INCOMING )|| ( nAllState & CC_STATE_DIALING )
        || ( nAllState & CC_STATE_ALERTLING )|| ( nAllState & CC_STATE_RELEASE ))
    {
       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call in wrong status. nAllState: 0x%x\n\n", nAllState);
       CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCall);
       return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
    }

#ifdef CFW_MULTI_SIM
//check if other SIM in call
      CFW_SIM_ID nSimID;
       
	for (nSimID = CFW_SIM_0; nSimID < CFW_SIM_COUNT; nSimID ++)
	{
	     if(nSimId!=nSimID )
            	     nAOCount += CFW_GetAoCount(CFW_CC_SRV_ID, nSimID);
   
	}
	if(nAOCount) 
	{
	       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call when other card in call\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCall);
		return ERR_CFW_OTHER_SIM_IN_CALL;
	}	
#endif    
    if(pDialNumber == NULL)
    {
       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call, INPUT param1 is null\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCall);
        return ERR_CFW_INVALID_PARAMETER;
    }        
    
    if(pDialNumber->nDialNumberSize > 40)
    {
       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call, INPUT call number size too long\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCall);
        return ERR_CME_DIAL_STRING_TOO_LONG;
    }
        
    if(pDialNumber->pDialNumber == NULL)
    {
       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call, INPUT call number is null\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCall);
        return ERR_CFW_INVALID_PARAMETER;
    }    	
//Frank add for adjust SS status start.
#ifdef CFW_MULTI_SIM
	if( CFW_GetAoCount(CFW_SS_SRV_ID,nSimId) > 0 )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]the SS is working now!\n");
		return ERR_CME_NO_NETWORK_SERVICE;
	}
#else
    CFW_SetServiceId(CFW_SS_SRV_ID);
    if( CFW_GetAoCount() > 0 )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]the SS is working now!\n");
		return ERR_CME_NO_NETWORK_SERVICE;
	}
#endif
//Frank add for adjust SS status end.    
    proc = (CM_SM_INFO*)CSW_CC_MALLOC(SIZEOF(CM_SM_INFO) + pDialNumber->nDialNumberSize);

    if(proc == NULL)
    {
       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call, malloc fail\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCall);
        return ERR_NO_MORE_MEMORY;
    }        
    
    SUL_ZeroMemory32(proc, SIZEOF(CM_SM_INFO) + pDialNumber->nDialNumberSize);
    
    sOutMsg = (api_CcSetupReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_CcSetupReq_t)); 
    
    
    nPhoneNumLen= pDialNumber->nDialNumberSize;
    
    // Protect the memory...
    if(nPhoneNumLen > 40)
        return ERR_CME_DIAL_STRING_TOO_LONG;

     //Called party number
    SUL_MemCopy8(&sOutMsg->CalledPartyNb[1], pDialNumber->pDialNumber, nPhoneNumLen);
     
    //called party number type
    sOutMsg->CalledPartyNb[0] = pDialNumber->nType;
    nCPNLen= 1 + nPhoneNumLen;	
    
    //in order to fill index of call
#ifndef CFW_MULTI_SIM
    proc->nIndex = CC_GetIndex();
#else
    proc->nIndex = CC_GetIndex(nSimId);
#endif
    proc->bAudioOnFlag = FALSE;
    proc->bRelHeldCallFlag = FALSE;
    proc->bUsedChldFlag = FALSE;
    proc->bUsedAFlag = FALSE;
    proc->bProgressIndFlag = FALSE;
    proc->nTI = 0x80;
    proc->nCallMode = CFW_CM_MODE_VOICE;
    proc->nCallState = CFW_CM_STATUS_DIALING;
    proc->nDir = CFW_DIRECTION_MOBILE_ORIGINATED;
    proc->bIsMpty = FALSE;
    proc->nAddressType = pDialNumber->nType;
    proc->nHoldFunction = 0;
    proc->nNumberSize = nPhoneNumLen;
    
    if(pDialNumber->pDialNumber)
        SUL_MemCopy8(proc->pNumber, pDialNumber->pDialNumber, nPhoneNumLen);
#ifdef CFW_AMR_SUPPORT
    //Beaer1
    sOutMsg->Bearer1Len = 6;
    sOutMsg->Bearer1[0] = 0x40;
    sOutMsg->Bearer1[1] = 0x04; // Speech version 3
    sOutMsg->Bearer1[2] = 0x02; // Speech version 2
    sOutMsg->Bearer1[3] = 0x00; // Speech version 1
    sOutMsg->Bearer1[4] = 0x81; // HR Speech version 1
    sOutMsg->Bearer1[5] = 0x85; // HR Speech version 3         

#else
    //Beaer1
    sOutMsg->Bearer1Len = 4;
    sOutMsg->Bearer1[0] = 0x40;
    //sOutMsg->Bearer1[1] = 0x04; // Speech version 3
    sOutMsg->Bearer1[1] = 0x02; // Speech version 2
    sOutMsg->Bearer1[2] = 0x00; // Speech version 1
    sOutMsg->Bearer1[3] = 0x81; // HR Speech version 1
    //sOutMsg->Bearer1[5] = 0x85; // HR Speech version 3         

#endif
    //Bearer2
    sOutMsg->Bearer2Len = 0;
    // Called Party Nb
    sOutMsg->CPNLen = nCPNLen;
    //subaddress
    sOutMsg->CPSALen = 0;
    //call type
    sOutMsg->CallType = API_CC_REGULAR_CALL;
	   //UserUser
    sOutMsg->UserUserLen = 0;
    
    //CLir
    switch(pDialNumber->nClir)
    {
    case 1:
        sOutMsg->CLIRSuppression = FALSE;
        sOutMsg->CLIRInvocation = TRUE;
        
        break;
    case 2:
        sOutMsg->CLIRSuppression = TRUE;
        sOutMsg->CLIRInvocation = FALSE;
        
        break;
    default:
        sOutMsg->CLIRSuppression = FALSE;
        sOutMsg->CLIRInvocation = FALSE;        
        break;        
    }
    
    
    //FacilityIE
    sOutMsg->Facility.StructValid = FALSE;
    proc->pMsg = (VOID*)sOutMsg;
    proc->nReleaseFlag = FALSE;
    proc->bMODisc = FALSE;
    proc->nCalledFuncFlag = 1; //CFW_CcInitiateSpeechCall is called
    proc->nNextSubState = SUBSTATE_IDLE;
    proc->nCurSubState = SUBSTATE_IDLE;
    proc->bNeedSplitFlag = FALSE;
    //register this MO Call instence
#ifndef CFW_MULTI_SIM
    hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc);
#else
	 hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc, nSimId);
#endif
    CSW_TRACE(CFW_CC_TS_ID, "CFW_CcInitiateSpeechCall()Register MO AO\n");
    //Set UTI for AT_APP
    CFW_SetUTI(hSm, nUTI, 0);
    CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);
    CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCall);
    CSW_TRACE(CFW_CC_TS_ID, "CFW_CcInitiateSpeechCall() end \n");
    return ret;                           
}




//
//
//
//
UINT32 CFW_CcInitiateSpeechCallEx (
                                 CFW_DIALNUMBER* pDialNumber,
                                 UINT8 *pIndex,
                                 UINT16 nUTI
#ifdef CFW_MULTI_SIM
                                 ,CFW_SIM_ID nSimId
#endif
                                 )
{
    UINT8  nCPNLen;
    UINT8  nPhoneNumLen;	
    HAO    hSm;
    CM_SM_INFO* proc=NULL;
    UINT32 ret = ERR_SUCCESS;
    api_CcSetupReq_t* sOutMsg = 0;
    UINT32 nAllState = 0;
#ifdef CFW_MULTI_SIM
	UINT8 nAOCount=0;
    
	CSW_PROFILE_FUNCTION_ENTER(CFW_CcInitiateSpeechCallEx);
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcInitiateSpeechCallEx(), sim=%d \n", nSimId);

	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcInitiateSpeechCallEx() check SIM ID failed \n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCallEx);
		return ret;
	}
	nAllState = GetCallStatus(nSimId);
#else
    CSW_PROFILE_FUNCTION_ENTER(CFW_CcInitiateSpeechCallEx);
    nAllState = GetCallStatus();
#endif
	
    *pIndex = 0;
    
    if( ( nAllState & CC_STATE_ACTIVE ) || ( nAllState & CC_STATE_WAITING )
        || ( nAllState & CC_STATE_INCOMING )|| ( nAllState & CC_STATE_DIALING )
        || ( nAllState & CC_STATE_ALERTLING )|| ( nAllState & CC_STATE_RELEASE ))
    {
       CSW_TRACE(CFW_CC_TS_ID, "Init speech call return in wrong status, nAllState=0x%x\n", nAllState);
       CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCallEx);
       return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
    }

#ifdef CFW_MULTI_SIM
//check if other SIM in call
      CFW_SIM_ID nSimID;
       
	for (nSimID = CFW_SIM_0; nSimID < CFW_SIM_COUNT; nSimID ++)
	{
	     if(nSimId!=nSimID )
            	     nAOCount += CFW_GetAoCount(CFW_CC_SRV_ID, nSimID);
   
	}
	if(nAOCount) 
	{
	       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call when other card in call\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCall);
		return ERR_CFW_OTHER_SIM_IN_CALL;
	}	
#endif
    if(pDialNumber == NULL)
    {
       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call, INPUT param1 is null\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCallEx);
        return ERR_CFW_INVALID_PARAMETER;
    }        

    if(pDialNumber->pDialNumber ==NULL)
    {
       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call, INPUT call number is null\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCallEx);
        return ERR_CFW_INVALID_PARAMETER;
    }    	
    
    if(pDialNumber->nDialNumberSize > 40)
    {
       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call, INPUT call number size too long\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCallEx);
        return ERR_CME_DIAL_STRING_TOO_LONG;
    }

//Frank add for adjust SS status start.
#ifdef CFW_MULTI_SIM
	if( CFW_GetAoCount(CFW_SS_SRV_ID,nSimId) > 0 )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]the SS is working now!\n");
		return ERR_CME_NO_NETWORK_SERVICE;
	}
#else
    CFW_SetServiceId(CFW_SS_SRV_ID);
    if( CFW_GetAoCount() > 0 )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]the SS is working now!\n");
		return ERR_CME_NO_NETWORK_SERVICE;
	}
#endif
//Frank add for adjust SS status end.
    
    proc=(CM_SM_INFO*)CSW_CC_MALLOC(SIZEOF(CM_SM_INFO) + pDialNumber->nDialNumberSize);

    if(proc == NULL)
    {
       CSW_TRACE(CFW_CC_TS_ID, "[ERROR]Init speech call, malloc fail\n");
        CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCallEx);
        return ERR_NO_MORE_MEMORY;
    }       
    
    SUL_ZeroMemory32(proc, SIZEOF(CM_SM_INFO) + pDialNumber->nDialNumberSize);

    sOutMsg = (api_CcSetupReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_CcSetupReq_t));

    
    nPhoneNumLen= pDialNumber->nDialNumberSize;

    // Protect the memory...
    if(nPhoneNumLen > 40)
        return ERR_CME_DIAL_STRING_TOO_LONG;
    
    //Called party number
    SUL_MemCopy8(&sOutMsg->CalledPartyNb[1], pDialNumber->pDialNumber, nPhoneNumLen);
    //called party number type
    sOutMsg->CalledPartyNb[0] = pDialNumber->nType;
    nCPNLen= 1 + nPhoneNumLen;	
    
    //in order to fill index of call
#ifndef CFW_MULTI_SIM
    proc->nIndex = CC_GetIndex();
#else
    proc->nIndex = CC_GetIndex(nSimId);
#endif
    proc->bAudioOnFlag = FALSE;
    proc->bRelHeldCallFlag = FALSE;
    proc->bUsedChldFlag = FALSE;
    proc->bUsedAFlag = FALSE;
    proc->bProgressIndFlag = FALSE;
    proc->nTI = 0x80;
    proc->nCallMode = CFW_CM_MODE_VOICE;
    proc->nCallState = CFW_CM_STATUS_DIALING;
    proc->nDir = CFW_DIRECTION_MOBILE_ORIGINATED;
    proc->bIsMpty = FALSE;
    proc->nAddressType = pDialNumber->nType;
    proc->nHoldFunction = 0;
    proc->nNumberSize = nPhoneNumLen;
    
    if(pDialNumber->pDialNumber)
        SUL_MemCopy8(proc->pNumber, pDialNumber->pDialNumber, nPhoneNumLen);

   //Beaer1
#ifdef CFW_AMR_SUPPORT
    sOutMsg->Bearer1Len = 6;
    sOutMsg->Bearer1[0] = 0x40;
    sOutMsg->Bearer1[1] = 0x04; // Speech version 3
    sOutMsg->Bearer1[2] = 0x02; // Speech version 2
    sOutMsg->Bearer1[3] = 0x00; // Speech version 1
    sOutMsg->Bearer1[4] = 0x01; // HR Speech version 1
    sOutMsg->Bearer1[5] = 0x85; // HR Speech version 3         

#else
    sOutMsg->Bearer1Len = 4;
    sOutMsg->Bearer1[0] = 0x40;
    //sOutMsg->Bearer1[1] = 0x04; // Speech version 3
    sOutMsg->Bearer1[1] = 0x02; // Speech version 2
    sOutMsg->Bearer1[2] = 0x00; // Speech version 1
    sOutMsg->Bearer1[3] = 0x81; // HR Speech version 1

#endif

    //Bearer2
    sOutMsg->Bearer2Len = 0;
    // Called Party Nb
    sOutMsg->CPNLen = nCPNLen;
    //subaddress
    sOutMsg->CPSALen = 0;
    //call type
    sOutMsg->CallType = API_CC_REGULAR_CALL;
	   //UserUser
    sOutMsg->UserUserLen = 0;
    
    //CLir
    switch(pDialNumber->nClir)
    {
    case 1:
        sOutMsg->CLIRSuppression = FALSE;
        sOutMsg->CLIRInvocation = TRUE;
        
        break;
    case 2:
        sOutMsg->CLIRSuppression = TRUE;
        sOutMsg->CLIRInvocation = FALSE;
        
        break;
    default:
        sOutMsg->CLIRSuppression = FALSE;
        sOutMsg->CLIRInvocation = FALSE;
        
        break;
        
    }

    
    //FacilityIE
    sOutMsg->Facility.StructValid = FALSE;
    proc->pMsg = (VOID*)sOutMsg;
    proc->nReleaseFlag = FALSE;
    proc->bMODisc = FALSE;
    proc->nCalledFuncFlag = 1; //CFW_CcInitiateSpeechCall is called
    proc->nNextSubState = SUBSTATE_IDLE;
    proc->nCurSubState = SUBSTATE_IDLE;
    proc->bNeedSplitFlag = FALSE;
    //register this MO Call instence
#ifndef CFW_MULTI_SIM
    hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc);
#else
	hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc, nSimId);
#endif
    CSW_TRACE(CFW_CC_TS_ID, "Register MO AO\n");
    //Set UTI for AT_APP
    CFW_SetUTI(hSm, nUTI, 0);
    CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);

    *pIndex = proc->nIndex;
    CSW_TRACE(CFW_CC_TS_ID, "CFW_CcInitiateSpeechCallEx() end, *pIndex=%d \n", *pIndex);	
    CSW_PROFILE_FUNCTION_EXIT(CFW_CcInitiateSpeechCallEx);
    return ret;                           
}



PRIVATE UINT32 CFW_CcReleaseProc (HAO hAo,CFW_EV* pEvent)
{
	HANDLE hSm = 0;
	UINT8 i = 0;
	UINT8 j = 0;
	CM_SM_INFO* proc=NULL; 
	//UINT32 ret = ERR_SUCCESS;
#ifdef CFW_MULTI_SIM	
	CFW_SIM_ID nSimId=CFW_SIM_END;
#endif
	CSW_PROFILE_FUNCTION_ENTER(CFW_CcReleaseProc);
	
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcReleaseProc(), BEGIN\n");
	CFW_SetServiceId(CFW_CC_SRV_ID);
	
	if((UINT32)pEvent == 0xffffffff)
	{
#ifdef CFW_MULTI_SIM	
// get SIM ID from the APP AO, hSm-->hAo
		CFW_GetSimID(hAo, &nSimId);
	    while((hSm = CFW_GetAoHandle(j, CFW_SM_STATE_READY, CFW_CC_SRV_ID, nSimId)) != HNULL)
	    {
	        j++;
	        CFW_UnRegisterAO(CFW_CC_SRV_ID, hSm);
	        CSW_TRACE(CFW_CC_TS_ID, "UnRegister AO in READY state: %d\n", (j-1));
	    }
	    
	    while(((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId) ) != HNULL) && (i < 7))

#else
		//first callback, release all the calls if any.
		while((hSm = CFW_GetAoHandle(j, CFW_SM_STATE_READY)) != HNULL)
		{
			j++;
			CFW_UnRegisterAO(CFW_CC_SRV_ID, hSm);
			CSW_TRACE(CFW_CC_TS_ID, "UnRegister AO in READY state: %d\n", (j-1));
		}

		while(((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW) ) != HNULL) && (i < 7))
#endif
		{
			i++;
			proc = (CM_SM_INFO*)CFW_GetAoUserData(hSm);  //get a CM data for hSm

			if(proc == NULL)
			{
				CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcReleaseProc() Get user data failed\n");
#ifdef CFW_MULTI_SIM				
				CFW_PostNotifyEvent(EV_CFW_CC_RELEASE_CALL_RSP, ERR_CME_MEMORY_FAILURE, 0, GENERATE_URC_UTI()|(nSimId<<24), 0x20);
#else
				CFW_PostNotifyEvent(EV_CFW_CC_RELEASE_CALL_RSP, ERR_CME_MEMORY_FAILURE, 0, GENERATE_URC_UTI(), 0x20);
#endif
				CSW_PROFILE_FUNCTION_EXIT(CFW_CcReleaseProc);
				return ERR_CME_MEMORY_FAILURE;
			}
			proc->nReleaseFlag = TRUE;


			CSW_TRACE(CFW_CC_TS_ID, "RELEASE THE %d call\n", (i-1));
			CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);
		}
		if(i == 0)
		{
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcReleaseProc()no call but call this fun CC==>>MMI EV_CFW_CC_RELEASE_CALL_RSP\n");
#ifdef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_CC_RELEASE_CALL_RSP, 0, 0, GENERATE_URC_UTI()|(nSimId<<24), 0x20);
#else
			CFW_PostNotifyEvent(EV_CFW_CC_RELEASE_CALL_RSP, 0, 0, GENERATE_URC_UTI(), 0x20);
#endif
		}

	}
	else
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcReleaseProc(), pEvent!=0xffffffff\n");
	}
	CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcReleaseProc(), END\n");	
	CSW_PROFILE_FUNCTION_EXIT(CFW_CcReleaseProc);	
	return ERR_SUCCESS;
}

//=================================================================================
//
// Release any existing calls. the original one can't release all the calls when when there are more than 4 in progress.
// the release action must be done in CSW task.
//=================================================================================
UINT32 CFW_CcReleaseCall ( 
#ifdef CFW_MULTI_SIM
		CFW_SIM_ID nSimId
#endif
		)
{
	HANDLE hSm;

	// UINT8 j = 0;
	//  CM_SM_INFO* proc=NULL; 
	UINT32 ret = ERR_SUCCESS;
	UINT32 *PCallBackData=NULL;

	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcReleaseCall() begin \n");
	CSW_PROFILE_FUNCTION_ENTER(CFW_CcReleaseCall);
	CFW_SetServiceId(CFW_CC_SRV_ID);
	PCallBackData = (UINT32*)CSW_CC_MALLOC(4);
#ifdef CFW_MULTI_SIM
	//check if other SIM in call
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcReleaseCall() SIM ID=%d \n", nSimId);
	UINT8 i = 0;

	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
		if(!(ERR_CFW_SIM_NOT_INITIATE == ret && gEmcCallOn))
		{
			CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcReleaseCall() check SIM id failed, gEmcCallOn=%d \n", gEmcCallOn);
			CSW_PROFILE_FUNCTION_EXIT(CFW_CcReleaseCall);
			return ret;
		}
		gEmcCallOn=0;
	}

	i = CFW_GetAoCount(CFW_CC_SRV_ID, nSimId);
	if(!i) 
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcReleaseCall() no call to release \n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcReleaseCall);
		return ERR_CFW_NO_CALL_INPROGRESS;
	}	
	i=0;
	hSm = CFW_RegisterAo(CFW_APP_SRV_ID, PCallBackData, CFW_CcReleaseProc, nSimId);

#else
	hSm = CFW_RegisterAo(CFW_APP_SRV_ID, PCallBackData, CFW_CcReleaseProc);


#endif

	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcReleaseCall() end \n");
	CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);

	CSW_PROFILE_FUNCTION_EXIT(CFW_CcReleaseCall);
	return(ret);
}
//=====================================================================================
//
//
//=====================================================================================
UINT32 CFW_CcAcceptSpeechCall (
#ifdef CFW_MULTI_SIM
		CFW_SIM_ID nSimId
#endif
		)
{
	HAO hSm;
	CM_SM_INFO* proc = NULL;
	UINT8 i = 0;
	UINT32 ret = ERR_SUCCESS;
	api_CcConnectReq_t* sOutMsg=NULL;

	CSW_PROFILE_FUNCTION_ENTER(CFW_CcAcceptSpeechCall);

	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAcceptSpeechCall()begin \n");
	CFW_SetServiceId(CFW_CC_SRV_ID);
#ifdef CFW_MULTI_SIM
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAcceptSpeechCall(), sim=%d \n", nSimId);
	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcAcceptSpeechCall(), check SIM ID failed \n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcAcceptSpeechCall);
		return ret;
	}

	while(((hSm = CFW_GetAoHandle(i, CFW_SM_CM_STATE_MTPROC, CFW_CC_SRV_ID, nSimId)) == HNULL) && (i < 7))
#else
	while(((hSm = CFW_GetAoHandle(i, CFW_SM_CM_STATE_MTPROC)) == HNULL) && (i < 7))
#endif
	{
		i++;
	}
	if((i == 7) || (hSm == HNULL))
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcAcceptSpeechCall(), no call in progress\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcAcceptSpeechCall);
#ifndef CFW_MULTI_SIM
		return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
#else
		return ERR_CFW_NO_CALL_INPROGRESS;
#endif
	}

	proc = (CM_SM_INFO*)CFW_GetAoUserData(hSm);
	if(proc == NULL )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcAcceptSpeechCall(), get user data failed\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcAcceptSpeechCall);
		return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
	}
	else
	{
		//[[hameina[+] 2009.8.3 this API can only accept the incoming call, waiting call will return this error
		//  bug 13542
		if(proc->nCallState != CFW_CM_STATUS_INCOMING)
		{
// when the hold/active call was released, only one call in progress with status waiting, call this function to accept this call.
// see bug 14039
			UINT8 nCnt=0;
#ifdef CFW_MULTI_SIM	
			nCnt = CFW_GetAoCount(CFW_CC_SRV_ID, nSimId);
#else
			nCnt = CFW_GetAoCount();
#endif
			  CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAcceptSpeechCall, nCnt != %d,nCallState=%d \n" , nCnt, proc->nCallState);
			if(!(proc->nCallState== CFW_CM_STATUS_WAITING && nCnt==1))
			{
				CSW_TRACE(CFW_CC_TS_ID, "[ERROR]nCallState != CFW_CM_STATUS_INCOMING\n" );
				CSW_PROFILE_FUNCTION_EXIT(CFW_CcAcceptSpeechCall);
				return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
			}
		}
		//]]hameinai[+] 2009.8.3 
		
		if( proc->nCalledFuncFlag == 2 )     //CFW_CmAcceptSpeechCall is called Added by Qiyan for...
		{

			CSW_TRACE(CFW_CC_TS_ID, "[ERROR]duplicated operation, holdmulity had been call to accept this call\n" );
			CSW_PROFILE_FUNCTION_EXIT(CFW_CcAcceptSpeechCall);
			return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
		}
		sOutMsg = (api_CcConnectReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_CcConnectReq_t));

		sOutMsg->TI = proc->nTI;
		sOutMsg->UserUserLen = 0;
		sOutMsg->CSALen = 0;
		sOutMsg->Facility.StructValid = FALSE;
		proc->pMsg = sOutMsg;
		proc->nNextSubState = proc->nCurSubState;
		proc->nCurSubState = SUBSTATE_MT_PROC_ATA;
		CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);
	}

	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcAcceptSpeechCall() end \n");
	CSW_PROFILE_FUNCTION_EXIT(CFW_CcAcceptSpeechCall);
	return(ret);
}




//=========================================================================
//
//
//
//=========================================================================
HAO CmMTAoProc(CFW_EV* pEvent)
{
    HANDLE hSm;   
    CM_SM_INFO* proc=NULL;
    UINT16  nUTI = 0;
    HAO hAo = HNULL;
    UINT8 i = 0;

    CSW_PROFILE_FUNCTION_ENTER(CmMTAoProc);
	CSW_TRACE(CFW_CC_TS_ID, "CmMTAoProc() begin\n");
#ifdef CFW_MULTI_SIM
	UINT32 ret =0;
	CFW_SIM_ID nSimId=CFW_SIM_END;
	nSimId = pEvent->nFlag;
	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_PROFILE_FUNCTION_EXIT(CmMTAoProc);
		CSW_TRACE(CFW_CC_TS_ID, "CmMTAoProc() ERROR!!!!, the incoming call with invalid SIM id\n");
		return ret;
	}
#endif
    proc=(CM_SM_INFO*)CSW_CC_MALLOC(SIZEOF(CM_SM_INFO) + 80);
    
    if(proc == NULL)
    {
	CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CmMTAoProc() MALLOC FAILED\n");
        CSW_PROFILE_FUNCTION_EXIT(CmMTAoProc);
        return ERR_NO_MORE_MEMORY;
    }        
    
    SUL_ZeroMemory32(proc, SIZEOF(CM_SM_INFO)+80);

#ifndef CFW_MULTI_SIM
/*This is a patch for conflict of MT&MO calls*/
    CFW_SetServiceId(CFW_CC_SRV_ID);
    
    while((hAo = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW) ) != HNULL)
#else
    while((hAo = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId) ) != HNULL)
#endif
    {
        UINT32 nUti = 0;

        CM_SM_INFO* pUserData = NULL;

        
        pUserData = (CM_SM_INFO*)CFW_GetAoUserData(hAo);

        CFW_GetUTI(hAo, &nUti);

        if(pUserData != NULL)
        {
            if( pUserData->nCallState == CFW_CM_STATUS_DIALING )
            {
                CSW_TRACE(CFW_CC_TS_ID, "\nCmMTAoProc().............EXCEPTION! When MT&MO conflict............\n");
#ifndef CFW_MULTI_SIM
                CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, pUserData->nIndex, nUti, 1); //Notify MMI the call isn't in progress

                CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, 0, (pUserData->nIndex << 16)|API_LOW_LAYER_FAIL, nUti, 0xf0);
#else
                CFW_PostNotifyEvent(EV_CFW_CC_CALL_INFO_IND, 0, pUserData->nIndex, nUti|(nSimId<<24), 1); //Notify MMI the call isn't in progress

                CFW_PostNotifyEvent ( EV_CFW_CC_INITIATE_SPEECH_CALL_RSP, 0, (pUserData->nIndex << 16)|API_LOW_LAYER_FAIL, nUti|(nSimId<<24), 0xf0);
#endif
                CFW_UnRegisterAO( CFW_CC_SRV_ID, hAo);

            }

        }
        i++;
    }

/*End of patch*/


    CFW_SetServiceId(CFW_CC_SRV_ID);
#ifndef CFW_MULTI_SIM
    proc->nIndex = CC_GetIndex();
#else
    proc->nIndex = CC_GetIndex(nSimId);
#endif
    proc->bAudioOnFlag = FALSE;
    proc->bRelHeldCallFlag = FALSE;
    proc->bUsedChldFlag = FALSE;
    proc->bUsedAFlag = FALSE;
    proc->bProgressIndFlag = FALSE;
    proc->nCalledFuncFlag = 0; //nothing function is called
    proc->nNextSubState = SUBSTATE_IDLE;
    proc->nCurSubState = SUBSTATE_IDLE;
    proc->bNeedSplitFlag = FALSE;
    //proc->nChldIndex = 0xff;
    proc->nReleaseFlag = FALSE;
    proc->bMODisc = FALSE;
    proc->pMsg = NULL;
    proc->nHoldFunction = 0;

    //Find a free UTI for the MT AO.
    while(nUTI <=  0xFF)
    {
        if (CFW_IsFreeUTI(nUTI, CFW_CC_SRV_ID) == ERR_CFW_UTI_IS_BUSY)
        {
            nUTI++;
        }
        else
            break;
    }

    //register this MT Call instence
#ifndef CFW_MULTI_SIM
    hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc);
#else
    hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc, nSimId);
#endif
    CFW_SetUTI( hSm, nUTI, 0 );
    
    CFW_SetAoState(hSm, CFW_SM_STATE_IDLE);
    CSW_TRACE(CFW_CC_TS_ID, "CmMTAoProc()Register MT AO\n");
    CSW_TRACE(CFW_CC_TS_ID, "CmMTAoProc() end\n");
    CSW_PROFILE_FUNCTION_EXIT(CmMTAoProc);
    return hSm;
}


//=============================================================================
//
//
//
//=============================================================================

UINT32 SRVAPI CFW_CcGetCurrentCall (
                                    CFW_CC_CURRENT_CALL_INFO CallInfo[7],
                                    UINT8* pCnt
#ifdef CFW_MULTI_SIM                                    
                                    , CFW_SIM_ID nSimId
#endif                                    
                                    )
{
    HAO hAo = HNULL;
    UINT8 i = 0;
    CM_SM_INFO* proc = NULL;
    *pCnt = 0x00;
    
    UINT8 nPhoneNumLen = 0;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CcGetCurrentCall);
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcGetCurrentCall()begin \n");


#ifdef CFW_MULTI_SIM
	UINT32 ret=0;
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcGetCurrentCall(), nSimId=%d \n", nSimId);

	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcGetCurrentCall() ret=0x%x, gEmcCallOn=%d \n",ret, gEmcCallOn);
		if(!(ERR_CFW_SIM_NOT_INITIATE == ret && gEmcCallOn))
		{
			CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcGetCurrentCall() check SIM id failed\n");
			CSW_PROFILE_FUNCTION_EXIT(CFW_CcGetCurrentCall);
			return ret;
		}
		
	}
	else
	{
		ret = CFW_GetAoCount(CFW_CC_SRV_ID, nSimId);
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcGetCurrentCall(), CFW_GetAoCountret=%d \n", ret);
		if(!ret)
		{
			CSW_PROFILE_FUNCTION_EXIT(CFW_CcGetCurrentCall);
			return ERR_CFW_NO_CALL_INPROGRESS;
		}
	}
#endif

    if((CallInfo == NULL) || (pCnt == NULL))
    {
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcGetCurrentCall(), return *pCnt =%d\n", *pCnt );
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcGetCurrentCall);
		return ERR_CFW_INVALID_PARAMETER;
    }        
#ifndef CFW_MULTI_SIM
    CFW_SetServiceId(CFW_CC_SRV_ID);
    
    *pCnt = CFW_GetAoCount();
#else
    *pCnt = CFW_GetAoCount(CFW_CC_SRV_ID, nSimId);
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcGetCurrentCall(), *pCnt =%d\n", *pCnt );

#endif
    for(i = 0; i < *pCnt; i++)
    {
#ifndef CFW_MULTI_SIM
        if((hAo = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW) ) != HNULL)
#else
        if((hAo = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId) ) != HNULL)
#endif
        {
            proc = (CM_SM_INFO*)CFW_GetAoUserData(hAo);
            if(proc != NULL)
            {
                nPhoneNumLen = proc->nNumberSize;
                CallInfo[i].idx = proc->nIndex;
                CallInfo[i].direction = proc->nDir;
                CallInfo[i].mode = proc->nCallMode;
                CallInfo[i].multiparty = proc->bIsMpty;
                CallInfo[i].status = proc->nCallState;                
                CallInfo[i].dialNumber.nType = proc->nAddressType;
                CallInfo[i].dialNumber.nSize = nPhoneNumLen;

                /*Protect MEMORY...*/
                if( nPhoneNumLen > TEL_NUMBER_MAX_LEN) 
                    nPhoneNumLen = TEL_NUMBER_MAX_LEN;
                SUL_MemCopy8(&(CallInfo[i].dialNumber.nTelNumber), proc->pNumber, nPhoneNumLen);

            }
            
        }
        
    }
	CSW_TC_MEMBLOCK(CFW_CC_TS_ID, (UINT8*)CallInfo, sizeof(CFW_CC_CURRENT_CALL_INFO)* (*pCnt), 16);
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcGetCurrentCall()end \n");
    CSW_PROFILE_FUNCTION_EXIT(CFW_CcGetCurrentCall);
    return ERR_SUCCESS;

}



//=====================================================================================
//
//
//=====================================================================================
static UINT32 GetCallStatus (
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId
#endif
	)
{
    HAO hAO;
    UINT8 i = 0;
    CM_SM_INFO *pUserData = NULL;
    UINT8 nCall = 0;
    UINT32 nAllState = 0;
#ifdef CFW_MULTI_SIM
	UINT32 ret=0;
	//CSW_TRACE(CFW_CC_TS_ID, "GetCallStatus(), sim=%d \n", nSimId);
#endif
	//CSW_TRACE(CFW_CC_TS_ID, "GetCallStatus() \n");


	CSW_PROFILE_FUNCTION_ENTER(GetCallStatus);
	CFW_SetServiceId(CFW_CC_SRV_ID);
#ifdef CFW_MULTI_SIM
	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CC_TS_ID, "GetCallStatus() check SIM id ret=0x%x\n", ret);
		if(!(ERR_CFW_SIM_NOT_INITIATE == ret && gEmcCallOn))
		{
			CSW_TRACE(CFW_CC_TS_ID, "[ERROR]GetCallStatus(), check sim id failed \n");
			CSW_PROFILE_FUNCTION_EXIT(GetCallStatus);
			return CC_STATE_NULL;
		}
	}
	nCall = CFW_GetAoCount(CFW_CC_SRV_ID, nSimId);
#else
	nCall = CFW_GetAoCount();
#endif

    if(nCall > 7)
        nCall = 7;

    for(i = 0; i < nCall; i++)
    {
#ifndef CFW_MULTI_SIM
        if((hAO = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW) ) != HNULL)
#else
        if((hAO = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId) ) != HNULL)
#endif
        {
            pUserData = (CM_SM_INFO*)CFW_GetAoUserData(hAO);
            
            if(pUserData != NULL)
            {
                switch(pUserData->nCallState) 
                {
                case CFW_CM_STATUS_DIALING:
                    nAllState |= CC_STATE_DIALING;
                	break;

                case CFW_CM_STATUS_ALERTING:
                    nAllState |= CC_STATE_ALERTLING;
                	break;
                case CFW_CM_STATUS_ACTIVE:
                    nAllState |= CC_STATE_ACTIVE;
                	break;
                
                case CFW_CM_STATUS_HELD:
                	nAllState |= CC_STATE_HOLD;
                    break;
                
                case CFW_CM_STATUS_WAITING:
                    nAllState |= CC_STATE_WAITING;
                	break;

                case CFW_CM_STATUS_INCOMING:
                    nAllState |= CC_STATE_INCOMING;
                	break;

                case CFW_CM_STATUS_RELEASED:
                    nAllState |= CC_STATE_RELEASE;
                	break;
                }

            }
            
        }
        
    }//for(;;)
	CSW_TRACE(CFW_CC_TS_ID, "GetCallStatus(), nAllState=0x%x\n", nAllState);    
    CSW_PROFILE_FUNCTION_EXIT(GetCallStatus);
    return nAllState;
}



//================================================================================
//
//
//
//================================================================================
UINT32 SRVAPI CFW_CcGetCallStatus(
#ifdef CFW_MULTI_SIM
                                                            CFW_SIM_ID nSimId
#endif
                                                          )
{
    UINT32 ret ;
    CSW_PROFILE_FUNCTION_ENTER(CFW_CcGetCallStatus);
#ifdef CFW_MULTI_SIM
	ret= GetCallStatus (nSimId );
#else
	ret= GetCallStatus ( );
#endif
	CSW_PROFILE_FUNCTION_EXIT(CFW_CcGetCallStatus);
    return ret;

}


//=====================================================================================
//
//Muti-Party 
//=====================================================================================
UINT32 CFW_CcCallHoldMultiparty (
                                 UINT8 nCmd,
                                 UINT8 nIndex
#ifdef CFW_MULTI_SIM
                                , CFW_SIM_ID nSimId
#endif
                                 )
{
	HAO hSm;
	CM_SM_INFO* proc = NULL;
	UINT32 nAllState = 0;
	UINT8 nCallNum = 0;
#ifdef CFW_MULTI_SIM
	UINT32 ret=0;
#endif
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()begin, ncmd=%d, nindex=%d\n", nCmd, nIndex);
	CSW_PROFILE_FUNCTION_ENTER(CFW_CcCallHoldMultiparty);

#ifndef CFW_MULTI_SIM
	CFW_SetServiceId(CFW_CC_SRV_ID);
	nCallNum = CFW_GetAoCount();
	if (0==nCallNum)	
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty() no call in progress\n");		
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
		return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED ;
	}
	nAllState = GetCallStatus();
#else
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()SIM ID =%d, gEmcCallOn=%d\n", nSimId, gEmcCallOn);
	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
	//emergancy call was also released by this function, return err except it may be an emergancy call
		if(!(ERR_CFW_SIM_NOT_INITIATE == ret && gEmcCallOn&& nCmd==1&&nIndex<8))
		{
			CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty()check SIM id failed\n");		
			CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
			return ret;
		}
		gEmcCallOn=0;
		
	}
	nCallNum = CFW_GetAoCount(CFW_CC_SRV_ID, nSimId);
	if (!nCallNum)	
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty() no call in progress\n");		
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
		return ERR_CFW_NO_CALL_INPROGRESS;
	}
	nAllState = GetCallStatus(nSimId);
#endif

	switch(nCmd)
	{
		case 0:  //Terminate all held calls; or set UDUB (User Determined User Busy)
		//for a waiting call,i.e. reject the waiting call.
		{
		//fix bug 12972, only have active call, do nothing
			if( nAllState == CC_STATE_ACTIVE)
			{
				CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty() case 0, only have active call\n");		
				CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
				return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
			}
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 0:\n");
#ifndef CFW_MULTI_SIM
			if( ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 0:if\n");
				CC_RejectCall();
			}
			else 
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 0:else\n");
				CC_RejectCall();
				CC_TerminateOne(CFW_SM_CM_STATE_HELD, SUBSTATE_HELD_CHLD0, 0);
				CC_TerminateMulti(CFW_SM_CM_STATE_MEETINGHELD, SUBSTATE_MEETINGHLD_CHLD0, 0);
			}
#else
			if( ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 0:if\n");
				CC_RejectCall(nSimId);
			}
			else 
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 0:else\n");
				CC_RejectCall(nSimId);
				CC_TerminateOne(CFW_SM_CM_STATE_HELD, SUBSTATE_HELD_CHLD0, 0, nSimId);
				CC_TerminateMulti(CFW_SM_CM_STATE_MEETINGHELD, SUBSTATE_MEETINGHLD_CHLD0, 0, nSimId);
			}
#endif
			break;
		}
		case 1:
		{
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 1:\n");
			UINT8 i;

			if((1 <= nIndex) && (nIndex <= 7)) //Terminate the active call nIndex (nIndex= 1-7)
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 1:if\n");
				for(i = 0; i < 7; i++)
				{
#ifndef CFW_MULTI_SIM
					if((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW)) != HNULL)
#else
					if((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId)) != HNULL)
#endif
					{
						proc= CFW_GetAoUserData(hSm);
						if(proc != NULL)
						{
							if(nIndex == proc->nIndex)
							{
								proc->nReleaseFlag = TRUE;
								proc->nHoldFunction = 0x11;
								CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);
								break;
							}

						}
					}
				}
				//hameina[+] 2009.8.3 bug 13550. invalid index, return parameter ERROR.
				if((i==7 )&& (hSm ==0))
				{
					CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty() case 1, invalid index\n");		
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
					return ERR_CME_INVALID_INDEX;
				}

			}
			else if(255== nIndex)//Terminate all active calls (if any) and accept the other call
			//(waiting call or held call)
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 1:else\n");
#ifndef CFW_MULTI_SIM
				// Hold + Wait 
				if( !( nAllState & CC_STATE_ACTIVE ) && ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 1:elseCC_AcceptWaitingCall\n");
					CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD1, 1) ;
				}
				// Active + Hold + Wait 
				else if( ( nAllState & CC_STATE_ACTIVE ) && ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
				{
					CC_TerminateOne(CFW_SM_CM_STATE_ACTIVE, SUBSTATE_ACTIVE_CHLD1, 1);
					CC_TerminateMulti(CFW_SM_CM_STATE_INMEETING, SUBSTATE_INMEETING_CHLD1, 1);
				}
				// Active + Hold/Wait 
				else if( nAllState & CC_STATE_ACTIVE )//fix the bug 3889,3891 2007.02.13
				{
					CC_TerminateOne(CFW_SM_CM_STATE_ACTIVE, SUBSTATE_ACTIVE_CHLD1, 1);
					CC_TerminateMulti(CFW_SM_CM_STATE_INMEETING, SUBSTATE_INMEETING_CHLD1, 1);
				}
				else if( nAllState & CC_STATE_HOLD)//fix the bug 3889,3891 2007.02.13, this bug is from the bug 3852 because
				//of ugly stack
				{
					CC_RetrieveCall(HNULL);	
					CC_RetrieveMPTY();
				}
				//for bug 12973, only incoming call, just do nothing
				else
				{
					CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty()case 1: only a incoming call, do nothing\n");
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
					return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED; 
				}
				
#else
				// Hold + Wait 
				if( !( nAllState & CC_STATE_ACTIVE ) && ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 1:elseCC_AcceptWaitingCall\n");
					CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD1, 1, nSimId) ;
				}
				// Active + Hold + Wait 
				else if( ( nAllState & CC_STATE_ACTIVE ) && ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
				{
					CC_TerminateOne(CFW_SM_CM_STATE_ACTIVE, SUBSTATE_ACTIVE_CHLD1, 1, nSimId);
					CC_TerminateMulti(CFW_SM_CM_STATE_INMEETING, SUBSTATE_INMEETING_CHLD1, 1, nSimId);
				}
				// Active + Hold/Wait 
				else if( nAllState & CC_STATE_ACTIVE )//fix the bug 3889,3891 2007.02.13
				{
					CC_TerminateOne(CFW_SM_CM_STATE_ACTIVE, SUBSTATE_ACTIVE_CHLD1, 1, nSimId);
					CC_TerminateMulti(CFW_SM_CM_STATE_INMEETING, SUBSTATE_INMEETING_CHLD1, 1, nSimId);
				}
				else if( nAllState & CC_STATE_HOLD)//fix the bug 3889,3891 2007.02.13, this bug is from the bug 3852 because
				//of ugly stack
				{
					CC_RetrieveCall(HNULL, nSimId);	
					CC_RetrieveMPTY(nSimId);
				}
				//for bug 12973, only incoming call, just do nothing
				else
				{
					CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty()case 1: only a incoming call, do nothing\n");
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
					return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED; 
				}
#endif
			}
			else
			{
				CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty() case 1, 222,invalid index\n");		
				CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
				return ERR_CME_INVALID_INDEX;
			}
			break;
		}
		case 2:
		{
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 2:\n");
			UINT8 i;

			if((1 <= nIndex) && (nIndex <= 7)) //Place all active calls except call nIndex(nIndex= 1-7) on hold
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 2:if\n");
				UINT32 nState;

				for(i = 0; i < 7; i++)
				{
#ifndef CFW_MULTI_SIM
					if((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW)) != HNULL)
#else
					if((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId)) != HNULL)
#endif
					{
						proc= CFW_GetAoUserData(hSm);

						if(proc != NULL)
						{
							if(nIndex == proc->nIndex)
							{
								nState = CFW_GetAoState(hSm);

								switch(nState)
								{
									case CFW_SM_CM_STATE_HELD:
										CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 2:if CFW_SM_CM_STATE_HELD:\n");
										CC_ProcHeldStateCHLD2X(hSm);
										break;

									case CFW_SM_CM_STATE_INMEETING:
										CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 2:if CFW_SM_CM_STATE_INMEETING:\n");
#ifndef CFW_MULTI_SIM
										CC_ProcMeetingStateCHLD2X(hSm, nIndex);
										break;

									case CFW_SM_CM_STATE_MEETINGHELD:
										CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 2:if CFW_SM_CM_STATE_MEETINGHELD:\n");
										CC_ProcMeetingHeldStateCHLD2X(hSm, nIndex);
#else
										CC_ProcMeetingStateCHLD2X(hSm, nIndex, nSimId);
										break;

									case CFW_SM_CM_STATE_MEETINGHELD:
										CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 2:if CFW_SM_CM_STATE_MEETINGHELD:\n");
										CC_ProcMeetingHeldStateCHLD2X(hSm, nIndex, nSimId);
#endif
										break;

									default:
										CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 2:if default:\n");
										CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
										return ERR_CFW_INVALID_PARAMETER; 
										break;   
								}
								break;
							}

						}

					}
				}
				//hameina[+] 2009.8.3 bug 13553. invalid index, return parameter ERROR.
				if((i==7 )&& (hSm ==0))
				{
					CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty() case2,111, invalid index\n");		
					CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
					return ERR_CME_INVALID_INDEX;
				}
				

			}
			else if(255==nIndex) //Place all active calls on hold (if any) and accept the other call
			//(waiting call or held call) as the active call

			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 2:else\n");
				// Hold + Wait 
				if( !( nAllState & CC_STATE_ACTIVE ) && ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
				{
					CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 2:elseCC_AcceptWaitingCall\n");
#ifndef CFW_MULTI_SIM
					CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD2, 2) ;
#else
					CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD2, 2, nSimId) ;
#endif
				}
				// Active + Hold + Wait 
				else if( ( nAllState & CC_STATE_ACTIVE ) && ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
				{
					UINT32 nState;
					UINT8 j = 0;

					if( nCallNum >=7)
					{	
						CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
						return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
					}
					//MPTY
#ifndef CFW_MULTI_SIM
					while(((hSm = CFW_GetAoHandle(j, CFW_SM_STATE_UNKONW)) != HNULL) && (j < 7))
#else
					while(((hSm = CFW_GetAoHandle(j, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId)) != HNULL) && (j < 7))
#endif
					{
						j++;

						nState = CFW_GetAoState(hSm);
						proc = CFW_GetAoUserData(hSm);
						proc->nNextSubState = proc->nCurSubState;

						if( nState == CFW_SM_CM_STATE_ACTIVE )
						{
							proc->nHoldFunction = 0x2;
							proc->nCurSubState = SUBSTATE_ACTIVE_CHLD3;
							CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_CONTINUE);
							break;
						}
						else if(nState == CFW_SM_CM_STATE_INMEETING)
						{
							proc->nHoldFunction = 0x2;
							proc->nCurSubState = SUBSTATE_INMEETING_CHLD3;
							CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_CONTINUE);
							break;
						}
					}//while

				}
				// Active + Hold/Wait 
				else
				{
#ifndef CFW_MULTI_SIM
					CC_Hold_AcceptCall();
					CC_HoldMPTY_Acceptcall();
					CC_RetrieveCall(HNULL);	
					CC_RetrieveMPTY();

#else
					CC_Hold_AcceptCall(nSimId);
					CC_HoldMPTY_Acceptcall(nSimId);
					CC_RetrieveCall(HNULL, nSimId);	
					CC_RetrieveMPTY(nSimId);
#endif
				}

			}
			else//bug 13560
			{
				CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty() case 2, invalid index\n");		
				CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
				return ERR_CME_INVALID_INDEX;
			}
			break;
		}
		case 3://Add the held call to the active calls
		{
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 3:\n");

			UINT32 nState;	   
			UINT8 i= 0;
			BOOL bActiveFlag = FALSE;
			BOOL bHeldFlag = FALSE;
#ifndef CFW_MULTI_SIM
			while(((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW)) != HNULL) && (i < 7))
#else
			while(((hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId)) != HNULL) && (i < 7))
#endif				
			{
				i++;
				nState = CFW_GetAoState(hSm);
				if((nState == CFW_SM_CM_STATE_ACTIVE) ||(nState == CFW_SM_CM_STATE_INMEETING))
					bActiveFlag = TRUE;

				if((nState == CFW_SM_CM_STATE_HELD) ||(nState == CFW_SM_CM_STATE_MEETINGHELD))
					bHeldFlag = TRUE;
			}
			if((i != 0) && (i < 7) && bActiveFlag && bHeldFlag)
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 3if:\n");
#ifndef CFW_MULTI_SIM
				if((hSm = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW)) != HNULL)
#else
				if((hSm = CFW_GetAoHandle(0, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId)) != HNULL)
#endif
				{
					proc = CFW_GetAoUserData(hSm);
					nState = CFW_GetAoState(hSm);

					if(proc != NULL)
					{
						proc->nNextSubState = proc->nCurSubState;

						switch(nState)
						{
							case CFW_SM_CM_STATE_ACTIVE:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 3if CFW_SM_CM_STATE_ACTIVE:\n");

								proc->nCurSubState = SUBSTATE_ACTIVE_CHLD3;

								break;
							case CFW_SM_CM_STATE_INMEETING:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 3if CFW_SM_CM_STATE_INMEETING:\n");

								proc->nCurSubState = SUBSTATE_INMEETING_CHLD3;
								break;
							case CFW_SM_CM_STATE_HELD:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 3if CFW_SM_CM_STATE_HELD:\n");

								proc->nCurSubState = SUBSTATE_HELD_CHLD3;

								break;
							case CFW_SM_CM_STATE_MEETINGHELD:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 3if CFW_SM_CM_STATE_MEETINGHELD:\n");

								proc->nCurSubState = SUBSTATE_MEETINGHLD_CHLD3;

								break;
							default:
								CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 3if default:\n");

								CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
								return ERR_CFW_INVALID_PARAMETER; 
								break;
						}

						CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);

					}
				}
			}
			else//bug 13561
			{
				CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcCallHoldMultiparty() case 3, meeting condition not fulfill\n");		
				CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
				return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
			}			break;
		}
		case 4:
			//Only swap, not accept the waiting call
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 4:\n");

			if( ( nAllState & CC_STATE_ACTIVE ) && ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 4if:\n");
#ifndef CFW_MULTI_SIM
				CC_HoldCall();
				CC_HoldMPTY();

				CC_RetrieveCall(HNULL);	
				CC_RetrieveMPTY();
			}
			else if( !( nAllState & CC_STATE_ACTIVE ) && ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 4:else if1\n");

				CC_RetrieveCall(HNULL);	
				CC_RetrieveMPTY();
			}
			else if( ( nAllState & CC_STATE_ACTIVE ) && !( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ))
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 4:else if2\n");

				CC_HoldCall();
				CC_HoldMPTY();
			}
#else
				CC_HoldCall(nSimId);
				CC_HoldMPTY(nSimId);

				CC_RetrieveCall(HNULL, nSimId);	
				CC_RetrieveMPTY(nSimId);
			}
			else if( !( nAllState & CC_STATE_ACTIVE ) && ( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ) )
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 4:else if1\n");

				CC_RetrieveCall(HNULL, nSimId);	
				CC_RetrieveMPTY(nSimId);
			}
			else if( ( nAllState & CC_STATE_ACTIVE ) && !( nAllState & CC_STATE_HOLD ) && ( nAllState & CC_STATE_WAITING ))
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 4:else if2\n");

				CC_HoldCall(nSimId);
				CC_HoldMPTY(nSimId);
			}
#endif
			else
			{
				CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 4:else\n");

				CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
				return ERR_CFW_INVALID_PARAMETER;
			}

			break;

		case 5:
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 5:\n");
#ifndef CFW_MULTI_SIM
			CC_HoldMPTY();
			break;    

		case 6:
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 6CC_AcceptWaitingCall:\n");

			CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD2, 2);
#else
			CC_HoldMPTY(nSimId);
			break;    

		case 6:
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()case 6CC_AcceptWaitingCall:\n");

			CC_TerminateOne( CFW_SM_CM_STATE_MTPROC, SUBSTATE_MT_PROC_CHLD2, 2, nSimId);
#endif
			break;

			default:
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()default\n");

			CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
			return ERR_CFW_INVALID_PARAMETER;
	}
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcCallHoldMultiparty()end\n");

	CSW_PROFILE_FUNCTION_EXIT(CFW_CcCallHoldMultiparty);
	return ERR_SUCCESS;

}



//
//
//
UINT32 CFW_CcEmcDial (
                                      UINT8* pDialNumber,
                                      UINT8 nDialNumberSize
#ifdef CFW_MULTI_SIM
                                      ,CFW_SIM_ID nSimId
#endif
                                 )
{
    UINT8  nCPNLen;
    UINT8  nPhoneNumLen;	
    HAO    hSm;
    CM_SM_INFO* proc=NULL;
    UINT32 ret = ERR_SUCCESS;
    api_CcSetupReq_t* sOutMsg = 0;
    UINT32 nAllState = 0;

	CSW_PROFILE_FUNCTION_ENTER(CFW_CcEmcDial);
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcEmcDial(), begin\n");
#ifndef CFW_MULTI_SIM
    nAllState = GetCallStatus();
#else
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcEmcDial(), sim=%d \n", nSimId);
//check parameter SIM ID
	if(nSimId > (CFW_SIM_COUNT -1))
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDial(), CHECK simid failed \n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDial);
		return ERR_CFW_INVALID_PARAMETER;
	}

//check if other SIM in call
//hameina[+] for UNIT TEST debug
	UINT8 nAOCount=0;
      CFW_SIM_ID nSimID;
       
	for (nSimID = CFW_SIM_0; nSimID < CFW_SIM_COUNT; nSimID ++)
	{
	     if(nSimId!=nSimID )
            	     nAOCount += CFW_GetAoCount(CFW_CC_SRV_ID, nSimID);
   
	}
	
	if(nAOCount) 
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDial(), other SIM in call\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDial);
		return ERR_CFW_OTHER_SIM_IN_CALL;
	}	

    nAllState = GetCallStatus(nSimId);
#endif
    if( ( nAllState & CC_STATE_ACTIVE ) || ( nAllState & CC_STATE_WAITING )
        || ( nAllState & CC_STATE_INCOMING )|| ( nAllState & CC_STATE_DIALING )
        || ( nAllState & CC_STATE_ALERTLING )|| ( nAllState & CC_STATE_RELEASE ))
    {
	CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDial(), call this function in wrong CALL STATUS\n");
    	CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDial);
        return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
    }    

    if(nDialNumberSize!= 2||pDialNumber==NULL)
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDial(), nDialNumberSize wrong\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDial);
		return ERR_CFW_INVALID_PARAMETER;
    }	
	//check the number, it must be 112
	if(!((pDialNumber[0] == 0x11) &&(pDialNumber[1]==0xf2)))
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDial(), invalid number\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDial);
		return ERR_CFW_INVALID_PARAMETER;
	}
//Frank add for adjust SS status start.
#ifdef CFW_MULTI_SIM
	if( CFW_GetAoCount(CFW_SS_SRV_ID,nSimId) > 0 )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]the SS is working now!\n");
		return ERR_CME_NO_NETWORK_SERVICE;
	}
#else
    CFW_SetServiceId(CFW_SS_SRV_ID);
    if( CFW_GetAoCount() > 0 )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]the SS is working now!\n");
		return ERR_CME_NO_NETWORK_SERVICE;
	}
#endif
//Frank add for adjust SS status end.

    proc=(CM_SM_INFO*)CSW_CC_MALLOC(SIZEOF(CM_SM_INFO) + nDialNumberSize);
    if(proc == NULL)
    {
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDial(), malloc failed\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDial);
		return ERR_NO_MORE_MEMORY;
    }        
    SUL_ZeroMemory32(proc, SIZEOF(CM_SM_INFO) + nDialNumberSize);
    
    sOutMsg = (api_CcSetupReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_CcSetupReq_t));
    nPhoneNumLen= nDialNumberSize;

    //protect memory...
    if(nPhoneNumLen > 40)
        return ERR_CME_DIAL_STRING_TOO_LONG;
    
    //Called party number
    SUL_MemCopy8(&sOutMsg->CalledPartyNb[1], pDialNumber, nPhoneNumLen);
    
    //called party number type
    sOutMsg->CalledPartyNb[0] = 0x81;
    nCPNLen= 1 + nPhoneNumLen;	
    
    //in order to fill index of call
    //proc->nIndex = 0xff;  //Updated by Qiyan
#ifndef CFW_MULTI_SIM
    proc->nIndex = CC_GetIndex();
#else
    proc->nIndex = CC_GetIndex(nSimId);
#endif
    proc->bAudioOnFlag = FALSE;
    proc->bRelHeldCallFlag = FALSE;
    proc->bUsedChldFlag = FALSE;
    proc->bUsedAFlag = FALSE;
    proc->bProgressIndFlag = FALSE;
    proc->nTI = 0x80;
    proc->nCallMode = CFW_CM_MODE_VOICE;
    proc->nCallState = CFW_CM_STATUS_DIALING;
    proc->nDir = CFW_DIRECTION_MOBILE_ORIGINATED;
    proc->bIsMpty = FALSE;
    proc->nAddressType = 0x81;
    proc->nHoldFunction = 0;
    proc->nNumberSize = nPhoneNumLen;


    if(pDialNumber)
        SUL_MemCopy8(proc->pNumber, pDialNumber, nPhoneNumLen);

    //Beaer1
#ifdef CFW_AMR_SUPPORT
    sOutMsg->Bearer1Len = 6;
    sOutMsg->Bearer1[0] = 0x40;
    sOutMsg->Bearer1[1] = 0x04; // Speech version 3
    sOutMsg->Bearer1[2] = 0x02; // Speech version 2
    sOutMsg->Bearer1[3] = 0x00; // Speech version 1
    sOutMsg->Bearer1[4] = 0x01; // HR Speech version 1
    sOutMsg->Bearer1[5] = 0x85; // HR Speech version 3         

#else
    sOutMsg->Bearer1Len = 4;
    sOutMsg->Bearer1[0] = 0x40;
    //sOutMsg->Bearer1[1] = 0x04; // Speech version 3
    sOutMsg->Bearer1[1] = 0x02; // Speech version 2
    sOutMsg->Bearer1[2] = 0x00; // Speech version 1
    sOutMsg->Bearer1[3] = 0x81; // HR Speech version 1

#endif

    //Bearer2
    sOutMsg->Bearer2Len = 0;
    // Called Party Nb
    sOutMsg->CPNLen = nCPNLen;
    //subaddress
    sOutMsg->CPSALen = 0;
    //call type
    sOutMsg->CallType = API_CC_EMERGENCY_CALL;
	   //UserUser
    sOutMsg->UserUserLen = 0;
    
    //CLir
    sOutMsg->CLIRSuppression = FALSE;
    sOutMsg->CLIRInvocation = FALSE;
    
    
    //FacilityIE
    sOutMsg->Facility.StructValid = FALSE;
    proc->pMsg = (VOID*)sOutMsg;
    proc->nReleaseFlag = FALSE;
    proc->bMODisc = FALSE;
    proc->nCalledFuncFlag = 1; //CFW_CcInitiateSpeechCall is called
    proc->nNextSubState = SUBSTATE_IDLE;
    proc->nCurSubState = SUBSTATE_IDLE;
    proc->bNeedSplitFlag = FALSE;
    //register this MO Call instence
#ifndef CFW_MULTI_SIM
    hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc);
#else
	hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc, nSimId);
	gEmcCallOn = 1;
#endif
    CSW_TRACE(CFW_CC_TS_ID, "Register MO AO\n");
    //Set UTI for AT_APP
    CFW_SetUTI(hSm, 0, 0);
    CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);
    
    CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDial);
    return ret;                           
}



//===============================================================================
//
//
//===============================================================================
UINT32 CFW_CcEmcDialEx (
                                      UINT8* pDialNumber,
                                      UINT8 nDialNumberSize,
                                      UINT8 *pIndex
#ifdef CFW_MULTI_SIM
                                      , CFW_SIM_ID nSimId
#endif
                                    )
{
    UINT8  nCPNLen;
    UINT8  nPhoneNumLen;	
    HAO    hSm;
    CM_SM_INFO* proc=NULL;
    UINT32 ret = ERR_SUCCESS;
    api_CcSetupReq_t* sOutMsg = 0;
    UINT32 nAllState = 0;

	CSW_PROFILE_FUNCTION_ENTER(CFW_CcEmcDialEx);
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcEmcDialEx(), begin\n");
#ifndef CFW_MULTI_SIM
    nAllState = GetCallStatus();
#else
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcEmcDialEx(), sim=%d \n", nSimId);
	if(nSimId > (CFW_SIM_COUNT -1))
	{
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDialEx);
		return ERR_CFW_INVALID_PARAMETER;
	}
//check if other SIM in call
//hameina[+] for UNIT TEST debug

	UINT8 nAOCount = 0;
      CFW_SIM_ID nSimID;
       
	for (nSimID = CFW_SIM_0; nSimID < CFW_SIM_COUNT; nSimID ++)
	{
	     if(nSimId!=nSimID )
            	     nAOCount += CFW_GetAoCount(CFW_CC_SRV_ID, nSimID);
   
	}
	if(nAOCount) 
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDialEx(), Other SIM in call\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDialEx);
		return ERR_CFW_OTHER_SIM_IN_CALL;
	}	

    nAllState = GetCallStatus(nSimId);
#endif

    *pIndex = 0;

    if( ( nAllState & CC_STATE_ACTIVE ) || ( nAllState & CC_STATE_WAITING )
        || ( nAllState & CC_STATE_INCOMING )|| ( nAllState & CC_STATE_DIALING )
        || ( nAllState & CC_STATE_ALERTLING )|| ( nAllState & CC_STATE_RELEASE ))
    {
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDialEx(), wrong call status\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDialEx);
		return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
    }    

	if(!nDialNumberSize || nDialNumberSize > 2)
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDialEx(), nDialNumberSize = 0\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDialEx);
		return ERR_CFW_INVALID_PARAMETER;
	}	
	
	//check the number, it must be 112
	if(!((pDialNumber[0] == 0x11) &&(pDialNumber[1]==0xf2)))
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDialEx(), invalid number\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDialEx);
		return ERR_CFW_INVALID_PARAMETER;
	}
//Frank add for adjust SS status start.
#ifdef CFW_MULTI_SIM
	if( CFW_GetAoCount(CFW_SS_SRV_ID,nSimId) > 0 )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]the SS is working now!\n");
		return ERR_CME_NO_NETWORK_SERVICE;
	}
#else
    CFW_SetServiceId(CFW_SS_SRV_ID);
    if( CFW_GetAoCount() > 0 )
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]the SS is working now!\n");
		return ERR_CME_NO_NETWORK_SERVICE;
	}
#endif
//Frank add for adjust SS status end.

	proc=(CM_SM_INFO*)CSW_CC_MALLOC(SIZEOF(CM_SM_INFO) + nDialNumberSize);
    if(proc == NULL)
    {
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDialEx(), malloc fail\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDialEx);
		return ERR_NO_MORE_MEMORY;
    }        
    
    SUL_ZeroMemory32(proc, SIZEOF(CM_SM_INFO) + nDialNumberSize);

    sOutMsg = (api_CcSetupReq_t*)CFW_MemAllocMessageStructure(SIZEOF(api_CcSetupReq_t));

    nPhoneNumLen= nDialNumberSize;

    if(nPhoneNumLen > 40)
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcEmcDialEx(), number too long\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDialEx);
	        return ERR_CME_DIAL_STRING_TOO_LONG;
    	}
    //Called party number
    SUL_MemCopy8(&sOutMsg->CalledPartyNb[1], pDialNumber, nPhoneNumLen);
    //called party number type
    sOutMsg->CalledPartyNb[0] = 0x81;
    nCPNLen= 1 + nPhoneNumLen;	
    
    //in order to fill index of call
    //proc->nIndex = 0xff;  //Updated by Qiyan
#ifndef CFW_MULTI_SIM
    proc->nIndex = CC_GetIndex();
#else
    proc->nIndex = CC_GetIndex(nSimId);
#endif
    proc->bAudioOnFlag = FALSE;
    proc->bRelHeldCallFlag = FALSE;
    proc->bUsedChldFlag = FALSE;
    proc->bUsedAFlag = FALSE;
    proc->bProgressIndFlag = FALSE;
    proc->nTI = 0x80;
    proc->nCallMode = CFW_CM_MODE_VOICE;
    proc->nCallState = CFW_CM_STATUS_DIALING;
    proc->nDir = CFW_DIRECTION_MOBILE_ORIGINATED;
    proc->bIsMpty = FALSE;
    proc->nAddressType = 0x81;
    proc->nHoldFunction = 0;
    proc->nNumberSize = nPhoneNumLen;
    
    if(pDialNumber)
        SUL_MemCopy8(proc->pNumber, pDialNumber, nPhoneNumLen);

#ifdef CFW_AMR_SUPPORT
    sOutMsg->Bearer1Len = 6;
    sOutMsg->Bearer1[0] = 0x40;
    sOutMsg->Bearer1[1] = 0x04; // Speech version 3
    sOutMsg->Bearer1[2] = 0x02; // Speech version 2
    sOutMsg->Bearer1[3] = 0x00; // Speech version 1
    sOutMsg->Bearer1[4] = 0x01; // HR Speech version 1
    sOutMsg->Bearer1[5] = 0x85; // HR Speech version 3         
#else
    sOutMsg->Bearer1Len = 4;
    sOutMsg->Bearer1[0] = 0x40;
    //sOutMsg->Bearer1[1] = 0x04; // Speech version 3
    sOutMsg->Bearer1[1] = 0x02; // Speech version 2
    sOutMsg->Bearer1[2] = 0x00; // Speech version 1
    sOutMsg->Bearer1[3] = 0x81; // HR Speech version 1
#endif

    
    //Bearer2
    sOutMsg->Bearer2Len = 0;
    // Called Party Nb
    sOutMsg->CPNLen = nCPNLen;
    //subaddress
    sOutMsg->CPSALen = 0;
    //call type
    sOutMsg->CallType = API_CC_EMERGENCY_CALL;
	   //UserUser
    sOutMsg->UserUserLen = 0;
    
    //CLir
    sOutMsg->CLIRSuppression = FALSE;
    sOutMsg->CLIRInvocation = FALSE;
    
    
    //FacilityIE
    sOutMsg->Facility.StructValid = FALSE;
    proc->pMsg = (VOID*)sOutMsg;
    proc->nReleaseFlag = FALSE;
    proc->bMODisc = FALSE;
    proc->nCalledFuncFlag = 1; //CFW_CcInitiateSpeechCall is called
    proc->nNextSubState = SUBSTATE_IDLE;
    proc->nCurSubState = SUBSTATE_IDLE;
    proc->bNeedSplitFlag = FALSE;
    //register this MO Call instence
#ifndef CFW_MULTI_SIM
	hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc);
#else
	hSm = CFW_RegisterAo(CFW_CC_SRV_ID, proc, CFW_CcAoProc, nSimId);
	gEmcCallOn = 1;
#endif
    //Set UTI for AT_APP
    CFW_SetUTI(hSm, 0, 0);
    CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);
    *pIndex = proc->nIndex;
    CSW_TRACE(CFW_CC_TS_ID, "CFW_CcEmcDialEx()end,*pIndex=%d \n", *pIndex);
    CSW_PROFILE_FUNCTION_EXIT(CFW_CcEmcDialEx);
    return ret;                           
}

//===============================================================================
//
//
//===============================================================================
UINT32 CFW_CcPlayTone (
		INT8 iTone 
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimId
#endif
	)
{
    HAO hSm;
    CM_SM_INFO* proc;
    UINT8 i = 0;
    UINT32 ret = ERR_SUCCESS;
    UINT8* sOutMsg = 0;
    UINT32 state = 0;

    CSW_PROFILE_FUNCTION_ENTER(CFW_CcPlayTone);
    CFW_SetServiceId(CFW_CC_SRV_ID);
    CSW_TRACE(CFW_CC_TS_ID, "CFW_CcPlayTone(),iTone=%d\n", iTone);
#ifdef CFW_MULTI_SIM
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcPlayTone(), sim id=%d \n", nSimId);
	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcPlayTone(), check sim id ret=0x%x\n", ret);
		if(!(ERR_CFW_SIM_NOT_INITIATE == ret && gEmcCallOn))
		{
			CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcPlayTone(), check sim id failed \n");
			CSW_PROFILE_FUNCTION_EXIT(CFW_CcPlayTone);
			return ret;
		}
	}
	i = CFW_GetAoCount(CFW_CC_SRV_ID, nSimId);
	if(!i)
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcPlayTone(), NO call in progress to playtone\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcPlayTone);
		return ERR_CME_OPERATION_NOT_ALLOWED;
	}
	i=0;
#endif

    if(!IsDTMFChar(iTone))
    {
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcPlayTone(), invalid input param iTone\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcPlayTone);
		return ERR_CME_INVALID_CHARACTERS_IN_DIAL_STRING;
    }
  //first, send DTMF from the active or meeting call.      
#ifndef CFW_MULTI_SIM
    while( ( hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW) ) && (i < 7) )
#else
    while( ( hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId) ) && (i < 7) )
#endif
    {
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcPlayTone(), first while i=%d\n", i);
        i++;
        state = CFW_GetAoState(hSm);
        proc = (CM_SM_INFO*)CFW_GetAoUserData(hSm);

		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcPlayTone(), first while state=0x%x\n",state);
        if( (state == CFW_SM_CM_STATE_ACTIVE) || (state == CFW_SM_CM_STATE_INMEETING) )
        {
            proc->nNextSubState = proc->nCurSubState;
            proc->nCurSubState = SUBSTATE_ACTIVE_OR_MEETING_DTMF;
            
            sOutMsg = (UINT8*)CSW_CC_MALLOC(1);
            
            *sOutMsg = iTone;
            proc->pMsg = sOutMsg;

            CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcPlayTone(), in active meeting state success end\n");
            CSW_PROFILE_FUNCTION_EXIT(CFW_CcPlayTone);
            return ret;
        }
    }


	//no active/meeting call, then send DTMF from alerting call
	i=0;
	hSm = HNULL;
#ifndef CFW_MULTI_SIM
	while(! ( hSm = CFW_GetAoHandle(i, CFW_SM_CM_STATE_MOPROC) ) && (i < 7) )
#else
	while( !( hSm = CFW_GetAoHandle(i, CFW_SM_STATE_UNKONW, CFW_CC_SRV_ID, nSimId) ) && (i < 7) )
#endif
	{
		i++;
	}
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcPlayTone(), after second while i=%d, hSm=0x%x\n", i, hSm);

	if((i<7) && (hSm!=HNULL))
	{
		CSW_TRACE(CFW_CC_TS_ID, "CFW_CcPlayTone(), after second while\n");
	        state = CFW_GetAoState(hSm);
	        proc = (CM_SM_INFO*)CFW_GetAoUserData(hSm);

	        if( proc->nCallState == CFW_CM_STATUS_ALERTING)
	        {
	            proc->nNextSubState = proc->nCurSubState;
	            proc->nCurSubState = SUBSTATE_MO_PROC_DTMF;
	            
	            sOutMsg = (UINT8*)CSW_CC_MALLOC(1);
	            
	            *sOutMsg = iTone;
	            proc->pMsg = sOutMsg;

	            CFW_SetAoProcCode(hSm, CFW_AO_PROC_CODE_MULTIPLE);
			CSW_TRACE(CFW_CC_TS_ID, "CFW_CcPlayTone(), MO PROC success end\n");
	            CSW_PROFILE_FUNCTION_EXIT(CFW_CcPlayTone);
	            return ret;
	        }
	}
			CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcPlayTone()\n");
	CSW_PROFILE_FUNCTION_EXIT(CFW_CcPlayTone);
    return ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;

}



//=================================================================================


//=================================================================================
UINT32 CFW_CcRejectCall (
#ifdef CFW_MULTI_SIM
		CFW_SIM_ID nSimId
#endif
		)
{
	HANDLE hAO;
	UINT8 i = 0;
	CM_SM_INFO *pUserData = NULL;
	UINT32 ret = ERR_SUCCESS;

	CSW_PROFILE_FUNCTION_ENTER(CFW_CcRejectCall);
	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcRejectCall() begin\n");
	CFW_SetServiceId(CFW_CC_SRV_ID);
#ifdef CFW_MULTI_SIM
	CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcRejectCall() input SIM ID=%d\n", nSimId);
	ret = CFW_CheckSimId( nSimId);
	if(ERR_SUCCESS != ret)
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcRejectCall() check SIM ID failed\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcRejectCall);
		return ret;
	}
	i=CFW_GetAoCount(CFW_CC_SRV_ID, nSimId);
	if(!i)
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcRejectCall() get AO COUNT=0\n");
		CSW_PROFILE_FUNCTION_EXIT(CFW_CcRejectCall);
		return ERR_CFW_NO_CALL_INPROGRESS;
	}
	i = 0;//hameina[+] for UINT TEST debug

	do
	{
		hAO = CFW_GetAoHandle(i, CFW_SM_CM_STATE_MTPROC, CFW_CC_SRV_ID, nSimId);
#else
	do
	{
		hAO = CFW_GetAoHandle(i, CFW_SM_CM_STATE_MTPROC);

#endif
		i++;        
	}
	while( (!hAO) && (i < 7) );
 
	if( (i <= 7) && hAO) //changyg [mod] for bug 10131
	{
	        pUserData = CFW_GetAoUserData(hAO);
	        
	        pUserData->nNextSubState = pUserData->nCurSubState;
	        pUserData->nCurSubState = SUBSTATE_MT_PROC_REJECT;

	        CFW_SetAoProcCode(hAO, CFW_AO_PROC_CODE_CONTINUE);
        
	}
	else
	{
		CSW_TRACE(CFW_CC_TS_ID, "[ERROR]CFW_CcRejectCall() no call waiting or incoming\n");
		ret = ERR_CME_OPERATION_TEMPORARILY_NOT_ALLOWED;
	}

	CSW_TRACE(CFW_CC_TS_ID, "CFW_CcRejectCall() end\n");
	CSW_PROFILE_FUNCTION_EXIT(CFW_CcRejectCall);

	return(ret);

}

#endif // ENABLE 


