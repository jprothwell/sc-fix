/*

  This is a sample test file for ecos it will use bt.h

 */


#include "../../user/bt.h"

u_int8 test_state = 0;
t_bdaddr bd_addr_inq;

void test_finish_callback(u_int8 code)
{
  test_state = 0;
}

void sample_setup(int rfcomm_chan)
{
  u_int8 channel;
  u_int32 handle;
  u_int16 serial_uuid,l2cap_uuid,rfcomm_uuid,language_id,language_encoding,language_base;
  t_SDP_serviceRecord *pRecord;
  t_SDP_dataElement SerialPortUUID,ServiceClassList,L2CAP_UUID,Protocol0,RFCOMM_UUID,RFCOMM_CHANNEL,Protocol1;
  t_SDP_dataElement ProtocolDescriptorList,Serial_Name,Lang_id,Lang_Enc,Lang_base,Lang_list_1,Lang_base_list;
  t_SDP_dataElement* list[10];
  
  char* Service_name = "Serial_user_a";

  handle = 16;

  /* Create a Service Record */ 

  SDP_CreateRecord(&handle,&pRecord);

  /* service class list */
  
  serial_uuid = 0x1101; /* serial port uuid */
  SDP_EncodeAttribute(SDP_UUID,2,&serial_uuid,&SerialPortUUID);

  /* place the uuid in a list */
  
  list[0] = &SerialPortUUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&ServiceClassList);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0001,&ServiceClassList);

  /* protocol descriptor list */


  /* construct the l2cap protocol entry */
  
  l2cap_uuid = 0x0100;
  SDP_EncodeAttribute(SDP_UUID,2,&l2cap_uuid,&L2CAP_UUID);

  /* place l2cap uuid in a list */

  list[0] = &L2CAP_UUID;
  SDP_CreateList(SDP_SEQUENCE,1,list,&Protocol0);
  
  
  /* construct the rfcomm entry */

  rfcomm_uuid = 0x0003;
  SDP_EncodeAttribute(SDP_UUID,2,&rfcomm_uuid,&RFCOMM_UUID);

  channel = rfcomm_chan; /* setting of the RFCOMM channel number */
  SDP_EncodeAttribute(SDP_UINT,1,&channel,&RFCOMM_CHANNEL);

  /* place rfcomm uuid and chanel number in a list */

  list[0] = &RFCOMM_UUID;
  list[1] = &RFCOMM_CHANNEL;
  SDP_CreateList(SDP_SEQUENCE,2,list,&Protocol1);
  
  /* place protocol 0 and protocol 1 in a list */

  list[0] = &Protocol0;
  list[1] = &Protocol1;
  
  SDP_CreateList(SDP_SEQUENCE,2,list,&ProtocolDescriptorList);
  
  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,0x0004,&ProtocolDescriptorList);
  

  /* language base list */
  
  /* language identifier english */

  language_id = ('e' << 8) + 'n';

  SDP_EncodeAttribute(SDP_UINT,2,&language_id,&Lang_id);

  /* language encoding  utf-8 */

  language_encoding = 106; 
  
  SDP_EncodeAttribute(SDP_UINT,2,&language_encoding,&Lang_Enc);

  /* language base */

  language_base = 0x0100;
  
  SDP_EncodeAttribute(SDP_UINT,2,&language_base,&Lang_base);
  
  /* place the entries in a list */

  list[0] = &Lang_id;
  list[1] = &Lang_Enc;
  list[2] = &Lang_base;

  SDP_CreateList(SDP_SEQUENCE,3,list,&Lang_list_1);

  /* add this to the main list */

  list[0] = &Lang_list_1;
  
  SDP_CreateList(SDP_SEQUENCE,1,list,&Lang_base_list);

  /* add this as an attribute to the service record */
  
  SDP_AddAttribute(pRecord,0x0006,&Lang_base_list);

  /* service name */

  SDP_EncodeAttribute(SDP_TEXT,sizeof(Service_name),Service_name,&Serial_Name);

  /* add this as an attribute to the service record */

  SDP_AddAttribute(pRecord,language_base + 0,&Serial_Name);
      
}


/*
  This function will issue a service search to obtain the protocol descriptor
  of a serial port profile compliant service.

  It will then call the helper function sample results to examine the results.
  
  
 */
void sample_test(void)
{
  u_int32 serial_uuid;
  t_SDP_Addresses devices;
  u_int16 criteria;
  t_SDP_SearchPattern pattern;
  t_SDP_AttributeIDs attribs;
  t_bdaddr address;
  t_SDP_StopRule rule;
  
  t_SDP_Results *pResults;
  
  address=bd_addr_inq;
  serial_uuid = 0x1101;

  /* bd address list to search for */

  devices.numItems = 1;
  devices.addresses = malloc(sizeof(t_bdaddr));  
  devices.addresses[0] = address;

  /* device relation criteria */

  criteria = SDP_TRUSTED;

  /* search pattern */

  pattern.numItems = 1;
  pattern.patternUUIDs = malloc(4);
  pattern.patternUUIDs[0] = serial_uuid;

  /* attributes to retrieve */

  attribs.numItems = 1;
  attribs.attributeIDs = malloc(4);
  attribs.attributeIDs[0] = 0x0004; /* protocol decriptor list */
  
  /* stop rule */

  rule.maxItems = 10;
  rule.maxDuration = 30;
  rule.maxBytes = 250;

  /* call a service search and do not retrieve the device name */

  test_state = 1;

  SDP_ServiceSearch(&pResults,&devices,&pattern,&attribs,&rule,criteria,test_finish_callback);    

  while(test_state);

  test_state = 0;

  /* examine the results */

  /* sample_results(pResults->element.attribs.pData); */

  SDP_FreeResults(pResults);
}


void sample_results(u_int8* pData)
{
  u_int16 sdp_u16 = 0x09;
  u_int16 sdp_list = 0x35;
  u_int16 sdp_u8 = 0x08;
  u_int16 sdp_uu16 = 0x19;
  
  u_int16 offset;
  
  if(*pData++ == sdp_list) /* outer list of attribute list */
    {
      pData++;
      if(*pData++ == sdp_u16) /* attribute id */
    {
      printf(("Attribute id"));
      if( (pData[0] << 8) + pData[1] == 0x00004) /* equals protocol descriptor list */
        {
          pData +=2;
          printf(("Protocol Descriptor List "));
          pData++;
          for(offset = 0 ; offset < pData[0] ; ) /* for contents of descriptor list */
        {
          offset+=2;
          printf(("{"));
          switch(pData[offset]) /* switch on protocol entry length */
            {
            case 3: /* protocol contains only a uuid */
              if(pData[offset+1] == sdp_uu16)
            printf(("   UUID : %d", (pData[offset+2] << 8) + pData[offset+3]));
              offset += 3;
              printf(("}"));
              break;
            case 5: /* protocl contains a uuid and a u_int8 parameter */
              if(pData[offset+1] == sdp_uu16)
            printf(("   UUID : %d", (pData[offset+2] << 8) + pData[offset+3]));
              if(pData[offset+4] == sdp_u8)
            printf(("   Parameter : %d", pData[offset + 5]));             
              offset += 5;
              printf(("}"));
              break;
            }
        }         
        }
    }
    }
}
