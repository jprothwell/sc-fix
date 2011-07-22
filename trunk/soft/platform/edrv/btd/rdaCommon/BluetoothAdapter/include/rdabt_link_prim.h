#ifndef _RDABT_LINK_PRIM_H_

#define _RDABT_LINK_PRIM_H_


#define LINK_PRIM_BASE	0x0000  
#define LINK_SEND_PRIM_BASE	CPL_UPSTREAM_PRIM_BASE

/*************************************************************************************
 Primitive definitions    
************************************************************************************/
typedef struct  
{
    btbm_bd_addr_t	bd_addr;
    kal_uint8	sdap_len;
    kal_uint32	sdap_uuid[BTBM_ADP_MAX_SDAP_UUID_NO];
}pin_code_sdap_struct;


typedef enum
{
	LINK_DISCOVERY_RESULT_IND = RDA_UPTOMMI_PRIM_BASE,   
	LINK_DISCOVERY_CNF,
       LINK_DISCOVERY_CANCEL_CNF,
       LINK_WRITE_SCANENABLE_MODE_CNF,
       LINK_WRITE_LOCALADDR_CNF,
	LINK_WRITE_AUTHENTICATION_MODE_CNF,
	LINK_PIN_CODE_IND,
	LINK_BONDING_RESULT_IND,
	LINK_BONDING_CNF,
} LINK_PRIM_T;












typedef struct
{
    kal_uint16  type;
    kal_uint8	result;
    kal_uint8	total_number;    
}LINK_CANCEL_DISCOVERY_ST;

typedef struct
{
    kal_uint16   type;
    kal_uint8	result;
}LINK_WRITE_SCANENABLE_MODE_ST;

typedef struct
{
    u_int16   type;
    u_int8	result;
    btbm_bd_addr_t bd_addr;
}LINK_WRITE_LOCALADDR_ST;


typedef struct
{
    u_int16   type;
    u_int8	result;
}LINK_WRITE_AUTHENTICATION_MODE_ST;

typedef struct
{
    u_int16   type;
    u_int8	result;
    u_int8 total_number;
}LINK_DISCOVERY_CNF_ST;

typedef struct
{
    u_int16  type;
    u_int8	result;
    u_int8	reserved;
    t_MGR_DeviceEntry  *device;
}LINK_DISCOVERY_RESULT_ST;


typedef struct
{
    u_int16  type;
    t_bdaddr     bd_addr;
    u_int8	        name_len;
    u_int8	        name[BTBM_ADP_MAX_NAME_LEN];
}LINK_PIN_CODE_IND_ST;


typedef struct
{
    u_int16 type;
    u_int8	result;
    btbm_bd_addr_t	bd_addr;
    kal_uint8	KeyType;
    kal_uint8 linkKey[0x10]; //support linkkey store in MMI	
    u_int32  cod;
}LINK_PIN_CODE_RESULT_IND_ST;

typedef struct
{
    u_int16    type;
    u_int8	result;
    btbm_bd_addr_t	bd_addr;
}LINK_PIN_CODE_CNF_ST;

#endif
