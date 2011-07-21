#ifndef __PRODUCE_MANAGER_H__
#define __PRODUCE_MANAGER_H__

#include "configuremanager.h"
#if OPEN_CONFIG > 0

enum
{
	PRO_COLUMN_CONF_MODULE
	,PRO_COLUMN_CONF_ID
	,PRO_COLUMN_ENABLE
	,PRO_COLUMN_SELECT
	,PRO_COLUMN_TITLE
	,PRO_COLUMN_SELECT_ENABLE
	,PRO_COLUMNS_MAX
};


#define PRO_ITEM_MAX (ENGING_CONFIG_MAX*PRODUCE_MAX)	

typedef struct tagProductItem
{
	int data[5];
#define conf_proID data[0]
#define conf_type data[1]
#define conf_id data[2]
#define conf_status data[3]
#define conf_enable data[4]
}GBProduceItem;

#endif

#endif