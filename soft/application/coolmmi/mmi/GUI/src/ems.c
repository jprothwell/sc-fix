/*
zrx add 20060524
*/
#include "cswtype.h"
#include "mmi_data_types.h"
#include "kal_non_specific_general_types.h"   // zrx add 20060522
#include "ems.h"

//zhanggh0
#include "smsal_l4c_enum.h"
#include "ucs2prot.h"
#include "string.h"
#include "mmi_trace.h"

PUBLIC UINT16 CursorPlace = 0;

EMSTATUS AddString(EMSData *emsData ,
                   kal_uint8 *string,
                   kal_uint16 num_char,
                   EMSTextFormat *txt_fmt)
{

	int i = 0;
	mmi_trace(g_sw_SMS," Func: %s emsData->dcs=%d, num_char=%d", __FUNCTION__,emsData->dcs,num_char);
	for (i=0; i<num_char*2; i++)
	{
		mmi_trace(g_sw_SMS," Func: %s string[%d]:%x", __FUNCTION__,i,string[i]);
	}

	UCS2Strncat(emsData->textBuffer,string,num_char);
	emsData->textLength += num_char * ENCODING_LENGTH;


#if 0
		emsData->textBuffer[emsData->textLength]= string[0];
		emsData->textLength++;
		emsData->textBuffer[emsData->textLength]=string[1];
		emsData->textLength++;
		emsData->CurrentPosition.OffsetToText +=2;
#endif /* Guoj delete. It's not used now */
		return EMS_OK;
}

#ifdef __SP__
EMSTATUS AddString_Ext(EMSData *emsData ,
                   kal_uint8 *string,
                   kal_uint16 num_char,
                   EMSTextFormat *txt_fmt)
{
	
    UCS2Strcpy(emsData->textBuffer,string);
    emsData->textLength = num_char*ENCODING_LENGTH;	
		emsData->CurrentPosition.OffsetToText =num_char*ENCODING_LENGTH;
		return EMS_OK;
	
}
#endif
void ResetTextFormat(EMSTextFormat *txt_fmt)
{
     txt_fmt->FontSize=0;
     txt_fmt->Alignment=3;
     txt_fmt->isBold=0;
     txt_fmt->isItalic=0;
     txt_fmt->isUnderline=0;
     txt_fmt->isStrikethrough=0;
      txt_fmt->bgColor=9;
      txt_fmt->fgColor=0;
      txt_fmt->textLength=2;
}

 EMSTATUS AddTextAlignment(EMSData *emsData, kal_uint16 offsetToText,
                                 EMSTextFormat *txt_fmt)
                                 {
	return EMS_OK;
}


/* Pack / Unpack Category */

EMSTATUS EMSPack(EMSData *emsData,
                 kal_uint8     need_new_ref,
                 kal_uint8     *numOfMsg,
                 kal_uint8     *TPUD[],
                 kal_uint8		 TPUDLen[])
                 {
	return EMS_OK;
}



EMSTATUS EMSUnPack(EMSData *emsData,
                   kal_uint8      udhi,
                   kal_uint8      numOfMsg,
                   kal_uint8      *TPUD[],
                   kal_uint8      TPUDLen[])
                   {
	return EMS_OK;
}


EMSTATUS EMSMsgPreview(kal_uint8     numOfMsg,
			              kal_uint8     dcs,
                       kal_uint8     udhi,
                       kal_uint8     *TPUD[],
                       kal_uint8     TPUDLen[],
                       kal_uint16    BuffLen,    /* octets */
                       kal_uint8     *textBuff,
                       kal_uint8     *is_obj_present,
                       kal_uint16    *textLen)  /* octets */
{
	return EMS_OK;
}

/* Miscellaneous Category */

EMSTATUS EMSSetDCS(EMSData *emsData, kal_uint8 dcs)
{
		//zhanggh0
		switch(dcs)
		{
			case SMSAL_DEFAULT_DCS:
			case SMSAL_8BIT_DCS:
			case SMSAL_UCS2_DCS:
				emsData->dcs = dcs;
				return EMS_OK;
			default:
				return EMS_INVALID_DCS;
		}
		//zhanggh0 end
}


EMSTATUS EMSResetPortNum(EMSData *emsData)
	{
	return EMS_OK;
}


EMSTATUS EMSSetPortNum(EMSData *emsData, kal_uint16 src_port, kal_uint16 dst_port)
	{
	return EMS_OK;
		
}


kal_bool GetUDHIBit(kal_uint8 fo)

{
	return TRUE;
	
}

EMSTATUS CopyEMSData(EMSData *dest, EMSData *src)
	{
	return EMS_OK;
}


#if 0
void EMSConcatMsgRefFunc(kal_uint16(*)(void))
{
}
#endif
#define EMS_DATA_END_OFFSET(x)	((x))
kal_uint8 EMSGetNextObject(EMSData     *data,
                           EMSPosition *p,
                           kal_uint16  *c,
                           EMSObject   **o)
{
    	kal_uint8		return_type=0;
	if((p->Object!=NULL) && (p->Object->OffsetToText==p->OffsetToText))
	{
		if((p->Object->next!=NULL) && (p->Object->OffsetToText==p->Object->next->OffsetToText))
		{
			return_type=2;
			*o=p->Object;
			p->Object=p->Object->next;
		}
		else if(p->OffsetToText<=EMS_DATA_END_OFFSET(data->textLength))
		{
			return_type=2;
			*o=p->Object;
			p->Object=p->Object->next;
		}
	}
	else if(p->OffsetToText<EMS_DATA_END_OFFSET(data->textLength))
	{
		return_type=1;
#if(EMS_BYTE_ORDER_BIG_ENDIAN)
		*c=(data->textBuffer[p.OffsetToText+1]|(data->textBuffer[p->OffsetToText]<<8));
#else
		*c=(data->textBuffer[p->OffsetToText]|(data->textBuffer[p->OffsetToText+1]<<8));
#endif
		p->OffsetToText+=2;
	}
	return(return_type);

}


kal_uint8 EMSGetPreviousObject(EMSData     *data,
                               EMSPosition *p,
                               kal_uint16  *c,
                               EMSObject   **o)
{
	return 1;
}


kal_int32 EMSGetObjectSize (kal_uint8 type, EMSObjData *objdata)
	{
	return 0;
}


kal_int32 EMSGetObjectSizeWithCheck ( kal_uint8 type, EMSObjData *objdata, kal_bool *isAllow )
	{
	return 0;
	
}


EMSTATUS EMSSetCompressMode(kal_bool mode)
	{
	return EMS_OK;
}

 kal_bool EMSIsReusedObj(EMSData *emsData, EMSObject *obj_q)
 	{
	return TRUE;
}


kal_uint16 BackwardCurrentPosition(EMSData *data, kal_uint16 steps)
{
	if (data->CurrentPosition.OffsetToText > 0)
	{
		data->CurrentPosition.OffsetToText -= 2 * steps;
		return 1;
	}
	else
		data->CurrentPosition.OffsetToText = 0;
	return 0;
}

kal_uint16 ForwardCurrentPosition(EMSData *data, kal_uint16 steps)
{
	data->CurrentPosition.OffsetToText += 2 * steps;
	return 1;
}
EMSTATUS AddObject(EMSData *emsData,
                   kal_uint8 type,
                   EMSObjData *objData,
                   kal_uint8 PredefNo)
{
	return EMS_OK;
}

kal_uint16 CancelCurrentPosition(EMSData *data, kal_uint16 steps)
{
	return 1;
}
void ResetCurrentPosition(EMSData *data)
{
	data->CurrentPosition.OffsetToText = 0;
}

void GoToEndPosition (EMSData *data)
{
}

