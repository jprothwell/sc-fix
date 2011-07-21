#include "mmi_features.h"
#include "producebusiness.h"
#include "engineconfig.h"
#ifdef __MMI_GB_V5__
#if OPEN_CONFIG > 0

static GBProduceItem pProduceItem[PRO_ITEM_MAX] = {0};
static int nProduceItemCount = 0;

void loadProduceConfig(void)
{
	int idx;
	int col = 0;
	int count = 0;
	int produceCount = 0;
	char *pParam[32];
	char pStr[128] = {0};

	while(NULL != produceItem[count])
	{
		strcpy(pStr,produceItem[count]);
		getContentLineParam(pStr, pParam, &col);
		for (idx = 0; idx < col && PRODUCE_MAX > atoi(pParam[0]); idx++)
		{
			pProduceItem[produceCount].data[idx] = atoi(pParam[idx]);
		}
		if(PRODUCE_MAX > atoi(pParam[0]))
			produceCount++;
		count++;
	}
	nProduceItemCount = count;
}

void updateConfigDataByProduce(int proID)
{
	int idx;
	GBProduceItem *item = NULL;
	// 将配置数据恢复默认状态
	GBResetAllEngineOption();
	// 根据配置文件，更新配置数据
	for (idx = 0; idx < nProduceItemCount; idx++)
	{
		item = &pProduceItem[idx];
		if (item->conf_proID == proID)
		{
			GBSetEngineOptionStatus(item->conf_type, item->conf_id, item->conf_status);
			GBSetEngineOptionEnable(item->conf_type, item->conf_id, item->conf_enable);
		}
	}
}

#endif
#endif /*__MMI_GB_V5__*/
