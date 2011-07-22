/***********************************************************************
 *
 * MODULE NAME:    sdp_db.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    Service Discovery Test code
 * MAINTAINER:     Mark Donnelly
 * CREATION DATE:  08 March 2000
 *
 * SOURCE CONTROL: $Id: sdp_db.h 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 Parthus Technologies
 *     All rights reserved.
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/

#include "host_config.h"
#include "host_types.h"

#if SDP_SERVER

#ifndef SDP_DB_H
#define SDP_DB_H

typedef struct 
{
  u_int8 size;
  u_int8 type;
  u_int32 data_length;  // TODO: change size to 16bit
  //u_int8* data; // removed for mem efficiency
  union {
	  u_int8* pointer;
	  u_int32 local;
  } data;
} t_SDP_dataElement;

typedef struct st_t_SDP_attribute
{
  u_int16 id;
  t_SDP_dataElement value;
  struct st_t_SDP_attribute* p_next_attribute; // TODO: add a 16bit offset ??
} t_SDP_attribute;

typedef struct st_t_SDP_serviceRecord
{
  t_SDP_attribute attribute; /* this will always be the service record handle */
  struct st_t_SDP_serviceRecord* p_next_record;
} t_SDP_serviceRecord;

enum sdp_data_types{
  SDP_UINT = 1,
  SDP_INT = 2,
  SDP_UUID = 3,
  SDP_TEXT = 4,
  SDP_BOOLEAN = 5,
  SDP_SEQUENCE = 6,
  SDP_UNION = 7,
  SDP_URL = 8
};


#if SDP_RECORD_GEN

u_int8 prh_sdp_add(t_SDP_serviceRecord* record,u_int16 id,t_SDP_dataElement* data);
u_int8 prh_sdp_add_record(u_int32 handle);
u_int8 prh_sdp_remove_record(u_int32 handle);
u_int8 prh_sdp_encode_data(t_SDP_dataElement* data,u_int8* raw_data);
u_int8 prh_sdp_encode_list(t_SDP_dataElement* value,u_int32 num_elements, t_SDP_dataElement** elements);

u_int8 prh_sdp_flush_database(void);

#endif /* SDP_RECORD_GEN */

u_int8 prh_sdp_query(t_SDP_serviceRecord* pRecord,u_int16 attribute_id,t_SDP_dataElement** pEelement);
u_int8 prh_sdp_valid_record(u_int32 service_record_handle, t_SDP_serviceRecord** pRec);
u_int32 prh_sdp_service_search(u_int32* pSearchPattern,u_int32 number_uuids,u_int16* number_handles,u_int32** pHandles);	
u_int16 prh_sdp_traverse_list(t_SDP_dataElement* element,t_SDP_dataElement** pUIDS,u_int32* uuid_counter);
u_int8 prh_sdp_uuid_compair(u_int32 Pattern,t_SDP_dataElement* pElement);

#if SDP_RECORD_GEN

u_int32 prh_sdp_allocate_record_handle(void);

void prh_sdp_free_attribute(t_SDP_attribute* p_attribute);
void prh_sdp_free_attribute_data(t_SDP_attribute* attrib);
u_int32 prh_sdp_free_nested_element(t_SDP_dataElement* data,t_SDP_dataElement *stop);
 
void prh_sdp_db_modify_record_state(t_SDP_serviceRecord* pRecord);

#endif /* SDP_RECORD_GEN */

u_int32 sdp_determine_database_size(void);
u_int8* sdp_db_to_static(void);

#endif /* SDP_DB_H */

#endif /* SDP_SERVER */








