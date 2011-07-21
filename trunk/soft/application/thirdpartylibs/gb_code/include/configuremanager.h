#ifndef __CONFIGUREMANAGER_H__
#define __CONFIGUREMANAGER_H__

#include "engineconfig.h"


#include "misc.h"
#if OPEN_CONFIG > 0

enum
{
	CONFIG_FLAG_NORMAL
	,CONFIG_FLAG_ACTIVE
	,CONFIG_FLAG_RELATE
};

// 配置关系错误类型
enum
{
	CONF_RELATION_OK = 0
	,CONF_RELATION_IGNORE = -1
	,CONF_RELATION_INFO = -2
	,CONF_RELATION_WARNING_DIE_RETOTE = -3//闭循环，但是状态没有冲突
	,CONF_RELATION_ERROR = -4 //错误
	,CONF_RELATION_MULTIPLE_ACTIVE = -5//同一个配置，相同的状态作为条件同时出现在多个关联中
	,CONF_RELATION_NO_RELATE = -6 //有条件无结果的错误
	,CONF_RELATION_WARNING_MULTIPLE_MODIFY = -7//多次修改，但是状态没有冲突
	,CONF_RELATION_LINE_SEPERATOR=-8//分行输出信息
	,CONF_RELATION_ERROR_DUPL_ON=-9 //On状态错误
	,CONF_RELATION_ERROR_DUPL_OFF=-10 //Off状态错误
};
typedef struct _tag_GBConfItemStatus{
	int type;
	int id;
	int used;
}GBConfItemStatus;
typedef struct _tag_GBConfItemStatusHistory{
	GBConfItemStatus onHistory;
	GBConfItemStatus offHistory;
}GBConfItemStatusHistory;

#define CONF_LIST_MAX 100
typedef struct tagConfList
{
	int id;
	char name[256];
}GBConfList;

#define CONF_ITEM_MAX 1024
typedef struct tagConfItem
{
	int data[5];
#define item_listID data[0]
#define item_role data[1]
#define item_type data[2]
#define item_id data[3]
#define item_status data[4]
}GBConfItem;

extern void createConfigureWin(void);
extern void destroyConfigureWin(void);

#endif

#endif