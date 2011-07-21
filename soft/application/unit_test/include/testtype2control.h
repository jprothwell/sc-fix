#ifndef __TEST_TYPE2_CONTROL_H__
#define __TEST_TYPE2_CONTROL_H__

#include "testtype2.h"
#include "unit_test.h"

#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
#define SIMULATOR_INVALID_INDEX 0xff
#define  SIMULATOR_MAX_MAP_SMS_SIM2_LEN  24
#define SIMULATOR_MAX_MAP_SMS_ME2_LEN 24
#define SIMULATOR_MAX_MAP_PHB_LEN 24
#define SIMULATOR_MAX_MAP_PHB_UNUSED_LEN 16


extern U8 simulator_map_sms_sim2[SIMULATOR_MAX_MAP_SMS_SIM2_LEN];
extern U8 simulator_map_sms_me2[ SIMULATOR_MAX_MAP_SMS_ME2_LEN];

extern U8 simulator_map_phb_sim2[SIMULATOR_MAX_MAP_PHB_LEN];

extern void simulator_init_sms_storage();
extern void simulator_init_phb_storage();

extern void simulator_init_map_sms_sim2();
extern void simulator_init_map_sms_me2();

extern void simulator_init_map_phb_sim2();

extern U8* simulator_list_map_sms_sim2(U8 startIndex, U8 count, U8* len);
extern U8* simulator_list_map_sms_me2(U8 startIndex, U8 count,U8* len);
extern U8* simulator_list_map_phb_sim2(U8* len);

extern CFW_SMS_NODE* simulator_get_sms_node(U8* listHead, U8 mapIndex);

extern CFW_SMS_NODE* simulator_write_sms_node(CFW_SMS_WRITE* pSmsInfo);

extern void simulator_delete_sms_node(U8 storageIndex, U8 storageType, U8 status);

extern CFW_SMS_NODE* simulator_get_sms_sim2_node(U8* listHead, U8 mapIndex);
extern CFW_SMS_NODE* simulator_get_sms_me2_node(U8* listHead, U8 mapIndex);

enum
{
	SIMULATOR_STORAGE_SIM1,
	SIMULATOR_STORAGE_ME1,
	SIMULATOR_STORAGE_SIM2,
	SIMULATOR_STORAGE_ME2,
	SIMULATOR_STORAGE_MAX_ID,
};

extern void simulator_get_sms_storage_info(CFW_SMS_STORAGE_INFO* storageInfo, UINT32 storageID);

extern UINT8 simulator_map_sms_sim2_add();
extern UINT8 simulator_map_sms_me2_add();
extern UINT8 simulator_map_phb_sim2_add();
extern UINT8 simulator_map_sms_sim2_del(U8 index);
extern UINT8 simulator_map_sms_me2_del(U8 index);
extern UINT8 simulator_map_phb_sim2_del(U8 index);
extern UINT8 simulator_map_sms_sim2_update(U8 index);
extern UINT8 simulator_map_sms_me2_update(U8 index);
extern UINT8 simulator_map_phb_sim2_update(U8 index);

extern CFW_SIM_PBK_ENTRY_INFO* simulator_get_phb_sim2_node(INT32 mapIndex);
extern INT32 simulator_add_phb_sim2_node(CFW_SIM_PBK_ENTRY_INFO* pPbkNode);
extern void simulator_del_phb_sim2_node(INT32 mapIndex);

extern INT32 simulator_get_phb_total_count();
extern INT32 simulator_get_phb_used_count();
#endif
#endif
