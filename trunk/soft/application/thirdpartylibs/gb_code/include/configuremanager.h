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

// ���ù�ϵ��������
enum
{
	CONF_RELATION_OK = 0
	,CONF_RELATION_IGNORE = -1
	,CONF_RELATION_INFO = -2
	,CONF_RELATION_WARNING_DIE_RETOTE = -3//��ѭ��������״̬û�г�ͻ
	,CONF_RELATION_ERROR = -4 //����
	,CONF_RELATION_MULTIPLE_ACTIVE = -5//ͬһ�����ã���ͬ��״̬��Ϊ����ͬʱ�����ڶ��������
	,CONF_RELATION_NO_RELATE = -6 //�������޽���Ĵ���
	,CONF_RELATION_WARNING_MULTIPLE_MODIFY = -7//����޸ģ�����״̬û�г�ͻ
	,CONF_RELATION_LINE_SEPERATOR=-8//���������Ϣ
	,CONF_RELATION_ERROR_DUPL_ON=-9 //On״̬����
	,CONF_RELATION_ERROR_DUPL_OFF=-10 //Off״̬����
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