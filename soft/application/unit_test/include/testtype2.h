#ifndef __TEST_TYPE2_H__
#define __TEST_TYPE2_H__


//#include "header.h" //for windows simu
#include "csw.h"
#include "string.h"

#define SIMULATOR_SMS_SIM2_DELIVER 16
#define SIMULATOR_SMS_SIM2_SUBMIT 32

#define SIMULATOR_MAX_SMS_SIM2_STORAGE (SIMULATOR_SMS_SIM2_DELIVER+SIMULATOR_SMS_SIM2_SUBMIT)



#define SIMULATOR_SMS_ME2_DELIVER 16
#define SIMULATOR_SMS_ME2_SUBMIT 32
#define SIMULAOTR_MAX_SMS_ME2_STORAGE (SIMULATOR_SMS_ME2_DELIVER+SIMULATOR_SMS_ME2_SUBMIT)

#define SIMULATOR_MAX_PHB_SIM2_STORAGE 32

typedef enum
{
	ADP_CFW_SMS_TXT_DELIVERED_NO_HRD_INFO,
	ADP_CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO,
	ADP_CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO,
	ADP_CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO	
} MESSAGE_NODE_TYPE;

typedef struct _CFW_DELIVERED_DATA
{
	CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO info;
	UINT8 data[8];
}CFW_DELIVERED_DATA;

typedef struct _CFW_SUBMIT_DATA
{
	CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO info;
	UINT8 data[8];
}CFW_SUBMIT_DATA;


typedef struct _simulator_sms_sim2_delivered_t
{
	CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO sms_node[SIMULATOR_MAX_SMS_SIM2_STORAGE];
	U32 length;
}simulator_sms_sim2_delivered_t; 

typedef struct _simulator_sms_sim2_storage_t
{
	CFW_SMS_NODE sms_node[SIMULATOR_MAX_SMS_SIM2_STORAGE];
	U32 length;
}simulator_sms_sim2_storage_t; 

typedef struct _simulator_sms_me2_storage_t
{
	CFW_SMS_NODE sms_node[SIMULAOTR_MAX_SMS_ME2_STORAGE];
	U32 length;
}simulator_sms_me2_storage_t; 

typedef struct _simulator_phb_sim2_storage_t
{
	CFW_SIM_PBK_ENTRY_INFO phb_node[SIMULATOR_MAX_PHB_SIM2_STORAGE];
	U32 length;
}simulator_phb_sim2_storage_t; 

typedef struct _simulator_phb_name_t 
{
	UINT8 name[8];
}simulator_phb_name_t;

typedef struct _simulator_phb_number_t 
{
	UINT8 number[16];
}simulator_phb_number_t;


extern simulator_sms_sim2_storage_t g_simulator_sms_sim2;
extern simulator_sms_me2_storage_t g_simulator_sms_me2;


extern void simulator_init_sms_sim2_storage();
extern void simulator_init_sms_me2_storage();

extern CFW_SMS_NODE* simulator_get_sms_sim2_storage_node(UINT8 nStorageIndex);
extern CFW_SMS_NODE* simulator_get_sms_me2_storage_node(UINT8 nStorageIndex);

extern simulator_phb_sim2_storage_t g_simulator_phb_sim2;

extern void simulator_init_phb_sim2_storage();

extern CFW_SIM_PBK_ENTRY_INFO* AddSim2PhbNode(INT32 index);

#endif
