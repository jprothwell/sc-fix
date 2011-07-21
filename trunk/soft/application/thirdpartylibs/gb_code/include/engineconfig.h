#ifndef __ENGINECONFIG_H__
#define __ENGINECONFIG_H__
#include "mmi_features.h"
#ifdef __MMI_GB_V5__
#include "engine.h"
#include "misc.h"

#ifdef _cplusplus
extern "C" {
#endif
#if OPEN_CONFIG > 0


//#define DEBUG_PRINT

/************************************************************************/
/*								 配置的设定                             */
/************************************************************************/

	enum 
	{
		// 词语产品
		PRODUCE_NUMPAD_1
		,PRODUCE_NUMPAD_2
		,PRODUCE_NUMPAD_3
		,PRODUCE_NUMPAD_4

		,PRODUCE_QWERTY_1
		,PRODUCE_QWERTY_2
		,PRODUCE_QWERTY_3
		,PRODUCE_QWERTY_4

		,PRODUCE_HALF_QWERTY_1
		,PRODUCE_HALF_QWERTY_2
		,PRODUCE_HALF_QWERTY_3
		,PRODUCE_HALF_QWERTY_4

		// 单字产品
		,PRODUCE_NUMPAD_5
		,PRODUCE_NUMPAD_6
		,PRODUCE_NUMPAD_7
		,PRODUCE_NUMPAD_8

		,PRODUCE_QWERTY_5
		,PRODUCE_QWERTY_6
		,PRODUCE_QWERTY_7
		,PRODUCE_QWERTY_8

		,PRODUCE_HALF_QWERTY_5
		,PRODUCE_HALF_QWERTY_6
		,PRODUCE_HALF_QWERTY_7
		,PRODUCE_HALF_QWERTY_8

		,PRODUCE_NUMPAD_9
		,PRODUCE_NUMPAD_10

		,PRODUCE_NUMPAD_11
		,PRODUCE_QWERTY_9

		,PRODUCE_EXPLICIT_1
		,PRODUCE_EXPLICIT_2
		,PRODUCE_EXPLICIT_3
		,PRODUCE_EXPLICIT_4

		,CURRENT_ENGINE_CONFIG

		,PRODUCE_MAX
	};
#define PRO_CAND_2(__produce)	(PRODUCE_NUMPAD_1 == __produce || PRODUCE_NUMPAD_2 == __produce \
	|| PRODUCE_NUMPAD_5 == __produce || PRODUCE_NUMPAD_6 == __produce \
	|| PRODUCE_NUMPAD_9 == __produce || PRODUCE_NUMPAD_10 == __produce \
	|| PRODUCE_NUMPAD_11 == __produce \
	|| PRODUCE_QWERTY_1 == __produce || PRODUCE_QWERTY_2 == __produce \
	|| PRODUCE_QWERTY_5 == __produce || PRODUCE_QWERTY_6 == __produce \
	|| PRODUCE_QWERTY_9 == __produce \
	|| PRODUCE_EXPLICIT_1 == __produce || PRODUCE_EXPLICIT_2 == __produce\
	|| PRODUCE_HALF_QWERTY_1 == __produce || PRODUCE_HALF_QWERTY_2 == __produce \
	|| PRODUCE_HALF_QWERTY_5 == __produce || PRODUCE_HALF_QWERTY_6 == __produce)

#define PRO_CAND_3(__produce)	(PRODUCE_NUMPAD_3 == __produce || PRODUCE_NUMPAD_4 == __produce \
	|| PRODUCE_NUMPAD_7 == __produce || PRODUCE_NUMPAD_8 == __produce \
	|| PRODUCE_QWERTY_3 == __produce || PRODUCE_QWERTY_4 == __produce \
	|| PRODUCE_QWERTY_7 == __produce || PRODUCE_QWERTY_8 == __produce \
	|| PRODUCE_EXPLICIT_3 == __produce || PRODUCE_EXPLICIT_4 == __produce\
	|| PRODUCE_HALF_QWERTY_3 == __produce || PRODUCE_HALF_QWERTY_4 == __produce \
	|| PRODUCE_HALF_QWERTY_7 == __produce || PRODUCE_HALF_QWERTY_8 == __produce)

#define PRO_CAND_INLINE(__produce)	(PRODUCE_NUMPAD_2 == __produce || PRODUCE_NUMPAD_4 == __produce \
	|| PRODUCE_NUMPAD_6 == __produce || PRODUCE_NUMPAD_8 == __produce \
	|| PRODUCE_NUMPAD_9 == __produce || PRODUCE_NUMPAD_10 == __produce \
	|| PRODUCE_QWERTY_2 == __produce || PRODUCE_QWERTY_4 == __produce \
	|| PRODUCE_QWERTY_6 == __produce || PRODUCE_QWERTY_8 == __produce \
	|| PRODUCE_EXPLICIT_2 == __produce || PRODUCE_EXPLICIT_4 == __produce\
	|| PRODUCE_HALF_QWERTY_2 == __produce || PRODUCE_HALF_QWERTY_4 == __produce \
	|| PRODUCE_HALF_QWERTY_6 == __produce || PRODUCE_HALF_QWERTY_8 == __produce)
/** \brief 引擎配置信息*/
typedef struct tagEngineSetInfo
{
	GBINT		index;
	GBPCACHAR	intro;
	//GBWCHAR		*pValue;
	GBU8		idName[128];
	GBU8		typeName[128];
	GBACHAR		*pValue;
	struct tagEngineSetInfo *pChild;
}GBEngineSetInfo;

/** \brief 引擎配置数据*/
typedef struct tagEngineSetData
{
	GBINT		index;
	GBU32		id;
	GBINT		type;
	GBINT		status;
	GBINT		flag;
	GBU8		idName[128];
	GBU8		typeName[128];
}GBEngineOptionData;

/** \brief 该输入模式下有用的配置 */
typedef struct tagInputModeConfig
{
	GBINT im;
	GBPCACHAR	OptionalBuf;
}GBInputModeConfig;

/** \brief 产品样式 */
typedef struct tagProductData
{
	GBINT	Chn_candListType;	//中文候选行类型
	GBINT	Chn_nCandRowCount;	//中文候选行数
	GBINT	Alp_candListType;	//英文候选行类型
	GBINT	Alp_nCandRowCount;	//英文候选行数
}GBProductStyle;

typedef struct tagEngineProductList
{
	GBINT			index;	//产品下标
	GBINT			id;
	GBPCACHAR		name;
	GBProductStyle	productData;	//产品表现形式
	GBU8		idName[128];
	GBU8		typeName[128];
}GBEngineProductList;

extern GBEngineProductList _huge ProductList[];
extern GBEngineOptionData _huge  configData[];
extern GBINT g_currentProduct;
extern GBINT GetProductIndex(GBINT nProductID);
extern GBINT SetProductList(GBINT index);

// 引擎配置数据操作接口
extern void GBResetAllEngineOption(void);
extern void GBSetEngineOptionStatus(int type, int id, int status);
extern void GBSetEngineOptionEnable(int type, int id, int enbale);
// 产品处理相关
extern GBBOOL GBSetBusinessConfig(GBINT id, GBINT type, GBBOOL bUpdated);
extern GBINT GBGetSelectedProduce(void);
extern void GBSetSelectedProduce(GBINT id);
extern void GBSaveEngineConfig(GBINT produce);
extern GBINT GBGetProduceIDByindex(GBINT index);
extern GBINT GBGetKeypadByProduceID(GBINT id);

extern GBBOOL GBGetStrStrByIndex(GBWCHAR *dst, const GBWCHAR *src, GBINT index, GBWCHAR split);
extern GBWCHAR *getEngineConfigValue(GBINT id, GBINT type);
extern const GBEngineSetting** GBGetAllProduceItem(GBINT *itemCount);
extern const GBEngineSetting** GBGetAllConfigItem(GBINT *itemCount);
// 其它
/*!
* \brief 数据必须以\n结束,以\t为分隔符  
*/
extern void getContentLineParam(char *line, char **param, int *num);
#else

void SetEngineConfig(void);

#endif


#ifdef _cplusplus
};
#endif
#endif /*__MMI_GB_V5__*/
#endif

