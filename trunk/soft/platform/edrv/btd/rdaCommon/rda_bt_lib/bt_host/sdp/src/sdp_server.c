/***********************************************************************
 *
 * MODULE NAME:    sdp_db_api.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery database api
 * MAINTAINER:     Mark Donnelly
 *
 * SOURCE CONTROL: $Id: sdp_server.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#include "host_config.h"

#if SDP_SERVER

#include "sdp_constants.h"
#include "sdp_le.h"
#include "sdp_ue.h"

t_pMutex prh_sdp_server_access;
u_int8 prh_sdp_server_active;


#if SDP_RECORD_GEN

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_CreateRecord
 * DESCRIPTION:
 
 This function will create record and add it to the list of service
records.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_CreateRecord(u_int32* handle,t_SDP_serviceRecord** pRecord)
{
	t_api status = BT_NOERROR;
	
	
	if(!handle && !pRecord)
		return BT_INVALIDPARAM;
	
	pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
	
	*handle =  prh_sdp_allocate_record_handle();
	
	status = prh_sdp_add_record(*handle);
	
	if(status == BT_NOERROR)
		prh_sdp_valid_record(*handle,pRecord);
	
	pMutexUnlock(prh_sdp_server_access); /* >>>> C-Section */
	return status;
}


/***********************************************************************
 *
 * FUNCTION NAME:   SDP_RemoveRecord
 * DESCRIPTION:
 
 This function will remove a record from the sdp service database.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_RemoveRecord(u_int32 handle)
{
	int ret;
	
	pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
	ret = prh_sdp_remove_record(handle);
	pMutexUnlock(prh_sdp_server_access); /* >>>> C-Section */
	return ret;
}


/***********************************************************************
 *
 * FUNCTION NAME:   SDP_EncodeAttribute
 * DESCRIPTION:
 
 This function will encode an attribute in a format that can be used by
the database.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_EncodeAttribute(u_int8 type,u_int32 size, u_int8* pData, t_SDP_dataElement* attribute)
{
	t_api status = BT_NOERROR;
	u_int8 attrib_size;
	
	if(!pData && !attribute)
		return BT_INVALIDPARAM;
	
	if(type == SDP_UINT || type == SDP_INT || type == SDP_UUID || type == SDP_BOOLEAN)
	{
		if(size > 8)
		{
			attrib_size = 4;
		} else if( size > 4) {
			attrib_size = 3 ;
		} else if( size > 2) {
			attrib_size = 2;
		} else if( size > 1 ) {
			attrib_size = 1;
		} else {
			attrib_size = 0;
		}  			  			  
	} else if(type == SDP_TEXT || type == SDP_URL) {
		if(size >= (1 << 16))
		{
			attrib_size = 7;
		} else if ( size >= (1 << 8)) {
			attrib_size = 6;
		} else {
			attrib_size = 5;
		}
		
	} else {
		return BT_INVALIDPARAM;
	}
	
	
	attribute->type = type;
	attribute->size = attrib_size;
	attribute->data_length = size;
	
	status = prh_sdp_encode_data(attribute,pData);
	
	return status;  
}

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_CreateList
 * DESCRIPTION:
 
 This function will create sequence or an alternative list type. The type
 will be passed as a parameter. 

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_CreateList(u_int8 type,u_int32 number,t_SDP_dataElement** attributes,t_SDP_dataElement* list_attributes)
{
	t_api status = BT_NOERROR;
	
	if(!attributes && !list_attributes)
		return BT_INVALIDPARAM;
	
	if(type == SDP_SEQUENCE || type == SDP_UNION)
    {
		status = prh_sdp_encode_list(list_attributes,number,attributes);
		list_attributes->type = type;
    } else {
		status = BT_INVALIDPARAM;
    }
	
	return status;
}


/***********************************************************************
 *
 * FUNCTION NAME:   SDP_AddAttribute
 * DESCRIPTION:
 
 This function will add an attribute to a service record.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_AddAttribute(t_SDP_serviceRecord* pRecord,u_int16 attribute_id,t_SDP_dataElement* data)
{
	int ret;
	if(!pRecord && !data)
		return BT_INVALIDPARAM;
	
	pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
	ret = prh_sdp_add(pRecord,attribute_id,data);
	pMutexUnlock(prh_sdp_server_access); /* >>>> C-Section */
	
	
	return ret;
}


/***********************************************************************
 *
 * FUNCTION NAME:   SDP_ReplaceAttribute
 * DESCRIPTION:
 
 This function will change an attribute of a service record. If a record
does not exist then it will be added to the list.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_ReplaceAttribute(t_SDP_serviceRecord* pRecord,u_int16 attribute_id,t_SDP_dataElement* data)
{
	t_api status = BT_NOERROR;
    t_SDP_attribute* p_attribute;
	
	if(!pRecord && !data)
		return BT_INVALIDPARAM;
	
	p_attribute = &(pRecord->attribute);
    
	do {
		if(p_attribute->id == attribute_id)
			break;
		
		p_attribute = p_attribute->p_next_attribute;
	} while (p_attribute != 0x00);
	
	pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
	
	if(p_attribute)
    {    
		prh_sdp_free_nested_element(&p_attribute->value,&p_attribute->value);
		p_attribute->value = *data;
		
		prh_sdp_db_modify_record_state(pRecord);
    } else {
		status = prh_sdp_add(pRecord,attribute_id,data);
    }
	
	pMutexUnlock(prh_sdp_server_access); /* >>>> C-Section */
	
	return status;
}



/***********************************************************************
 *
 * FUNCTION NAME:   SDP_GetRecord
 * DESCRIPTION:
 
 This function will retrieve a record from the database based on the
record handle

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_GetRecord(u_int32 recordHandle, t_SDP_serviceRecord** pRecord)
{
	return prh_sdp_valid_record(recordHandle,pRecord);
}




/***********************************************************************
 *
 * FUNCTION NAME:   SDP_GetNextRecord
 * DESCRIPTION:
 
 This function will retrieve the next record handle in the database. If
there is no next handle then BT_UNKNOWNERROR will be returned.

 * KNOWN ISSUES:
 
 none.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_GetNextRecord(u_int32 currentRecordHandle,u_int32 *pNextHandle)
{
	t_api status = BT_NOERROR;
	t_SDP_serviceRecord* pRecord;
	
	if(!pNextHandle)
		return BT_INVALIDPARAM;	
	
	status = prh_sdp_valid_record(currentRecordHandle,&pRecord);
	
	if(status)
    {		
		if(pRecord->p_next_record)
		{
			*pNextHandle = prh_sdp_read32((u_int8*)&pRecord->p_next_record->attribute.value.data.local);  
			
			status = 1;
		} else {
			status = 0;
		}
	}
	
	
	return status;
}



/***********************************************************************
 *
 * FUNCTION NAME:   SDP_SetRecordState
 * DESCRIPTION:
 
 This function will change the state of a record from free , to modified
, to queried.

 * KNOWN ISSUES:
 
 at the moment it is unsupported.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2  SDP_SetRecordState(u_int32 recordHandle,u_int8 state)
{
	t_api status = BT_UNSUPPORTED;
	
	pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
	/* tdb */
	pMutexUnlock(prh_sdp_server_access); /* >>>> C-Section */
	
	return status;
}



/***********************************************************************
 *
 * FUNCTION NAME:   SDP_StartServer
 * DESCRIPTION:
 
 This function will enable the server to receive incomming connections.

 * KNOWN ISSUES:
 
 at the moment this is unsupported and the server will always respond
to connections.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2  SDP_StartServer(u_int16 flags, void *reserved)
{
	pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
	prh_sdp_server_active = 1;
	pMutexUnlock(prh_sdp_server_access); /* >>>> C-Section */
	return BT_NOERROR;
}



/***********************************************************************
 *
 * FUNCTION NAME:   SDP_StopServer
 * DESCRIPTION:
 
 This function will disable the server from receive incoming connections.

 * KNOWN ISSUES:
 
 this is unsupported, i.e. the server will always accept connections.

 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2  SDP_StopServer(u_int16 flags, void *reserved)
{
	pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
	prh_sdp_server_active = 0;
	pMutexUnlock(prh_sdp_server_access); /* >>>> C-Section */
	return BT_NOERROR;
}

#if SDP_ROM_CREATION

u_int8* prh_sdp_db_to_static(u_int32 length);

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_DBtoStatic
 * DESCRIPTION:
 
 This function will convert a database from a dynamic format to a 
 flattened format in a buffer.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_DBtoStatic(u_int8** raw,u_int32 length)
{
	u_int8* pTemp;
	
	pTemp = prh_sdp_db_to_static(length);
	
	if(pTemp)
	{
		*raw = pTemp;
		return BT_NOERROR;
	} else {
		return BT_NORESOURCES;
	}
}

u_int32 prh_sdp_determine_database_size(void);

/***********************************************************************
 *
 * FUNCTION NAME:   SDP_DBSize
 * DESCRIPTION:
 
 This function will return the size in bytes that the database is taking
 up in memory.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_DBSize(u_int32* length)
{
	*length = prh_sdp_determine_database_size();
	
	return BT_NOERROR;
}


/***********************************************************************
 *
 * FUNCTION NAME:   SDP_BuildROMForTransfer
 * DESCRIPTION:
 
 This function will take the current database and create a flattened 
 version is will then remove the old database and create a new one with
 the flattened database an attribute of a service record. If the database
 is empty then it will remain empty.

 * KNOWN ISSUES:

 none.
 
 *
 *
 ***********************************************************************/
APIDECL1 t_api APIDECL2 SDP_BuildROMForTransfer(void)
{
	u_int32 handle;
	
	u_int32 _32bit;
	u_int32 length;
	u_int8* ref;
	
	t_SDP_serviceRecord *pRecord;
	t_SDP_dataElement e1,e2,e3,e4;
	
	t_SDP_dataElement* list[3];
	unsigned char text[] = "Parthus SDP ROM Image";
	
	length = prh_sdp_determine_database_size();

	if(length)
	{
		ref = prh_sdp_db_to_static(length);
		
		/* clear out the database */
		prh_sdp_flush_database();
		
		SDP_CreateRecord(&handle,&pRecord);
		
		_32bit = SDP_PARTHUS_ROM_UUID;
		
		SDP_EncodeAttribute(SDP_UUID,4,(u_int8*)&_32bit,&e1);
		SDP_EncodeAttribute(SDP_TEXT,sizeof(text),text,&e2);
		SDP_EncodeAttribute(SDP_TEXT,length,ref,&e3);
		
		list[0] = &e1;
		list[1] = &e2;
		list[2] = &e3;
		
		SDP_CreateList(SDP_SEQUENCE,3,list,&e4);
		
		SDP_AddAttribute(pRecord,SDP_PARTHUS_ROM_AID,&e4);
		
		pFree(ref);
	}

	return BT_NOERROR;
}


#endif /* SDP_ROM_CREATION */

#endif/* SDP_RECORD_GEN */

t_api prh_sdp_srv_layer_ctrl(u_int8 flags) {
    if (flags&PRH_HOST_STARTUP) {
		prh_sdp_server_access = pMutexCreate(0);
		prh_sdp_server_active = 1;
        SDP_RegisterPSM();
    } else if (flags&PRH_HOST_RESET) {
#if SDP_RECORD_GEN
		pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
		prh_sdp_flush_database();
		prh_sdp_server_active = 1;
		pMutexUnlock(prh_sdp_server_access); /* <<<< C-Section */
#endif /* SDP_RECORD_GEN */
        SDP_RegisterPSM();
	} else if (flags&PRH_HOST_SHUTDOWN) {
#if SDP_RECORD_GEN
		pMutexLock(prh_sdp_server_access); /* <<<< C-Section */
		prh_sdp_server_active = 0;
		prh_sdp_flush_database();
		pMutexUnlock(prh_sdp_server_access); /* <<<< C-Section */
		pMutexFree(prh_sdp_server_access);
#endif /* SDP_RECORD_GEN */
    }
    return BT_NOERROR;
}

#endif /* SDP_SERVER */


