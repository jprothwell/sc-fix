#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "smsapi.h"
#include "cfw_prv.h"
#include "zyxphone.h"
#include "zyxsys.h"
#include "zyxstring.h"
#include "zyxextern.h"
#include "zyxstd.h"

typedef struct sms_data{
	int 	times;
	char 	num[20];
	U16 	msg[128];
	ZyxSMSCB cb;
}sms_data,* psms_data;

#define SMS_DATA_ARRAY_MAX	(10)
static sms_data sms_data_array[SMS_DATA_ARRAY_MAX];
static int sms_data_init = 0;
static int sms_data_timer = -1;
static int sms_data_sended = 0;
static psms_data sms_data_cur = 0;
static ZyxSMSCB sms_callback = NULL;
 
int Zyx_get_usable_sim()
{
#if 0
   if(CFW_CheckSimId(CFW_SIM_0) == ERR_SUCCESS)
    {
        ZyxUsableSim =  CFW_SIM_0;
    }
    else if(CFW_CheckSimId(CFW_SIM_1) == ERR_SUCCESS)
    {
        ZyxUsableSim =  CFW_SIM_1;
    }
    else
    {
        ZyxUsableSim =  CFW_SIM_END;
    }
#endif
    ZyxTrace("zyx use sim%d",ZyxUsableSim);
    return ZyxUsableSim;
}

int ZyxSendSmsOne(char* num,U16* msg, ZyxSMSCB cb);

#ifdef __MMI_DUAL_SIM__
BOOL ZyxSmsSlaveCheck(void *inMsg,void *data)
{
	return 0;
}
#endif

BOOL ZyxSmsCheck(void *inMsg,void *data)
{
	return 0;
}

static void ZyxSendSmsTimer(int time_id)
{
	int i;

	for (i = 0; i < SMS_DATA_ARRAY_MAX; i ++){
		sms_data_cur = &sms_data_array[i];
		if (sms_data_cur->times > 0){
			ZyxTrace("zyx sms send one %s %s remain %d", 
				sms_data_cur->msg, sms_data_cur->num, sms_data_cur->times);
 			ZyxSendSmsOne(sms_data_cur->num, sms_data_cur->msg, sms_callback);
 			break;
		}
	}
}

static void ZyxSendMsgcb(void *number, module_type mod, U16 result)
{
	int i;

    if(result == MMI_FRM_SMS_OK)
    {
		ZyxTrace("zyx sms send ok");
    	if (ZyxCheckCallback((void*)sms_data_cur->cb)){
        	sms_data_cur->cb(number, 1);
        }
        sms_data_cur->times --;
    }
    else
    {
		ZyxTrace("zyx sms send failed");
    	if (ZyxCheckCallback((void*)sms_data_cur->cb)){
        	sms_data_cur->cb(number, 0);
      	}
    }

    sms_data_sended ++;

	//防止出错后，无节制发送
    if (sms_data_sended > 100){
		ZyxTrace("zyx sms send too much");
		return ;
    }

	for (i = 0; i < SMS_DATA_ARRAY_MAX; i ++){
		sms_data_cur = &sms_data_array[i];
		if (sms_data_cur->times){
			ZyxTrace("zyx sms send continue %s %s remain %d", 
				sms_data_cur->msg, sms_data_cur->num, sms_data_cur->times);
			sms_data_timer = ZyxStartTimer(60*1000,ZyxSendSmsTimer);
			return ;
		}
	}
	
	sms_data_timer = -1;
	ZyxTrace("zyx sms send finish");
}

int ZyxSendSmsOne(char* num,U16* msg, ZyxSMSCB cb)
{
#if 1
    U8 length = 0;
    int usable_sim = ZyxUsableSim;
	mmi_frm_sms_send_struct sendData;
	
    if (usable_sim == CFW_SIM_END)
        return -1;

    if (mmi_frm_sms_check_action_pending())
    {
    	ZyxTrace("zyx sms pending");
    	//return -1;
    }    

    ZyxTrace("zyx sms send num=%s,msg=%s",num,msg);
#ifdef __MMI_DUAL_SIM__
    if ( usable_sim == 1)
    {
        MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
        MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
    }
    else if ( usable_sim == 2)
    {
        MTPNP_AD_Free_Channel(MTPNP_AD_SMS_CHANNEL);
        MTPNP_AD_Set_Channel(MTPNP_AD_SMS_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
    }	
#endif

    sms_callback = cb;

    memset((S8*) &sendData, 0, sizeof(mmi_frm_sms_send_struct));
    sendData.dcs_class = SMSAL_CLASS1;
    sendData.sendrequire |= MMI_FRM_SMS_CLASS;
    sendData.sendcheck |= MMI_FRM_SMS_SCR;
    memcpy((S8*) & (sendData.number[length]), (S8*) num,  strlen(num));

    sendData.dcs = SMSAL_UCS2_DCS;
    sendData.stringlength = mmi_ucs2strlen((S8*)msg);
    sendData.string = (S8 *)OslMalloc((sendData.stringlength + 1) * ENCODING_LENGTH);
    memset((S8*) sendData.string, 0, (sendData.stringlength + 1) * ENCODING_LENGTH);
    ZyxUCSStrcpy((U16*)sendData.string, (U16 *)msg);

   	mmi_frm_sms_send_sms(ZyxSendMsgcb, MOD_MMI, &sendData);    
#else
	ZyxSendMsgcb(num,0, 0);
#endif
    return 0;
}

int ZyxSendSms(char* num,char* msg, int nTimes,ZyxSMSCB cb)
{
	int i;
 
	ZyxTrace("sms %s %s %d", num, msg, nTimes);
 
	if (sms_data_init == 0){
		sms_data_init ++;
		for (i = 0; i < SMS_DATA_ARRAY_MAX; i ++){
			sms_data_cur = &sms_data_array[i];
			sms_data_cur->times = 0;
		}
	}

	for (i = 0; i < SMS_DATA_ARRAY_MAX; i ++){
		sms_data_cur = &sms_data_array[i];
		if (sms_data_cur->times == 0){
			break;
		}
	}

	if (i == SMS_DATA_ARRAY_MAX){
		return -1;
	}

	sms_data_cur->times = nTimes;
	sms_data_cur->cb = cb;
	Zyx_strcpy(sms_data_cur->num, num);
	if (msg[0] && msg[1])
	{
		U16 umsg[128];
		ZyxGBToUCS((char*)msg, umsg, Zyx_strlen((char*)msg));
		ZyxUCSStrncpy(sms_data_cur->msg, umsg, 128);
		//Zyx_strncpy((char*)sms_data_cur->msg, (char*)msg, 128);
	}
	else
	{
		ZyxUCSStrncpy(sms_data_cur->msg, (U16*)msg, 128);
	}

	if (sms_data_timer >= 0){
		ZyxStopTimer(sms_data_timer);
		sms_data_timer = ZyxStartTimer(60*1000,ZyxSendSmsTimer);
	}else{
		sms_data_timer = ZyxStartTimer(10,ZyxSendSmsTimer);
	}
 
	return 0;
}

void ZyxDeleteSms(int index)
{
    mmi_frm_sms_delete_sms(NULL, MOD_MMI, MMI_FRM_SMS_APP_AWAITS, index);
}

void ZyxSmsAddHoldKey(U16* key,U16 *replykey,U16 *replycontent,ZyxContentCB cb)
{

}
void ZyxSmsAddChannelInfo(int chid,char* key,char *replykey,char *replycontent,ZyxContentCB cb)
{

}

extern void MakeCall(PS8 strNumber);
void ZyxMakeCall(char* num)
{
    int usable_sim = ZyxUsableSim;
    char OutNumber[32] = {0};

    if (usable_sim == 0)
        return;
    ZyxUTF8ToUCS(num,(U16*)OutNumber,64);

#ifdef __MMI_DUAL_SIM__
    if ( usable_sim == 1)
    {
        MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);
        MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_MASTER);
    }
    else if ( usable_sim == 2)
    {
        MTPNP_AD_Free_Channel(MTPNP_AD_CALL_CHANNEL);
        MTPNP_AD_Set_Channel(MTPNP_AD_CALL_CHANNEL, MTPNP_AD_CHANNEL_SLAVE);
    }	
#endif
    ZyxTrace("zyx make call=%s",num);
    MakeCall(OutNumber);
}

#endif

