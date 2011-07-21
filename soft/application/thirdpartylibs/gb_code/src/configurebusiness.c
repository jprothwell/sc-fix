#include "mmi_features.h"
#include "configurebusiness.h"
#ifdef __MMI_GB_V5__
#if OPEN_CONFIG > 0

const GBConfItem *pConfItemIndex[CONF_ITEM_MAX] = {0};
int nConfItemCount = 0;

GBConfItem _huge pConfItem[CONF_ITEM_MAX] = {0};
GBConfList _huge pConfList[CONF_LIST_MAX] = {0};
int nConfListCount = 0;

static void loadConfList(void);
static void loadConfItem(void);

void loadRelationConfig(void)
{
	loadConfList();
	loadConfItem();
}

/********************************
* \brief 加载配置列表 
********************************/
static void loadConfList(void)
{
	int idx;
	int col = 0;
	int count = 0;
	char *pParam[32];
	char pStr[128] = {0};

	while(NULL != configList[count])
	{
		strcpy(pStr,configList[count]);
		getContentLineParam(pStr, pParam, &col);
		for (idx = 0; idx < col; idx++)
		{
			if (0 == idx)
				pConfList[count].id = atoi(pParam[idx]);
			else
				strcpy(pConfList[count].name, pParam[idx]);
		}
		count++;
	}
	nConfListCount = count;

}
/********************************
* \brief 加载配置项 
********************************/
static void loadConfItem(void)
{
	int idx;
	int col = 0;
	int count = 0;
	char *pParam[32];
	char pStr[128] = {0};

	while(NULL != configItem[count])
	{
		strcpy(pStr, configItem[count]);
		getContentLineParam(pStr, pParam, &col);
		for (idx = 0; idx < col; idx++)
		{
			pConfItem[count].data[idx] = atoi(pParam[idx]);
		}
		pConfItemIndex[count] = &pConfItem[count];
		count++;
	}
	nConfItemCount = count;

}

#endif
#endif/*__MMI_GB_V5__*/
