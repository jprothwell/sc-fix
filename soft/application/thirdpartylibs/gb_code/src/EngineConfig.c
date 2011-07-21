#include "mmi_features.h"
#include "engineconfig.h"
#include "configurebusiness.h"
#include "producebusiness.h"
#ifdef __MMI_GB_V5__
#if OPEN_CONFIG > 0

#define _T(a)	a

GBINT g_currentProduct;	//当前选择的产品

//////////////////////////////////////////////////////////////////////////
static GBINT nCurrSelProduce = 0;
static GBEngineOptionData *pProduceBackup[PRODUCE_MAX];
static GBINT pProduceBackupFlag[PRODUCE_MAX] = {0};

// 保存所有的配置项
static GBEngineSetInfo *pAllConfigItem[ENGING_CONFIG_MAX] = {0};
static int s_nAllConfigItemCount = 0;
static GBEngineSetInfo *pAllProduceItem[PRODUCE_MAX] = {0};
static int s_nAllProduceItemCount = 0;
//////////////////////////////////////////////////////////////////////////
GBEngineProductList _huge ProductList[] = 
{
#if CONSOLE_ENGLISH > 0
	{ 0,PRODUCE_NUMPAD_1,		_T("P1.1 Number+Chinese(phrase+2 lines)+Alp(2 lines)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}
	,{ 1,PRODUCE_NUMPAD_2,		_T("P1.2 Number+Chinese(phrase+2 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}				
	,{ 2,PRODUCE_NUMPAD_3,		_T("P1.3 Number+Chinese(phrase+3 lines)+Alp(3 lines)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}				
	,{ 3,PRODUCE_NUMPAD_4,		_T("P1.4 Number+Chinese(phrase+3 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	

	,{ 4,PRODUCE_NUMPAD_5,		_T("P1.1 Number+Chinese(word+2 lines)+Alp(2 lines)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}		
	,{ 5,PRODUCE_NUMPAD_6,		_T("P1.2 Number+Chinese(word+2 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}				
	,{ 6,PRODUCE_NUMPAD_7,		_T("P1.3 Number+Chinese(word+3 lines)+Alp(3 lines)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}				
	,{ 7,PRODUCE_NUMPAD_8,		_T("P1.4 Number+Chinese(word+3 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	

	,{ 8,PRODUCE_QWERTY_1,		_T("P2.1 Qwerty+Chinese(phrase+2 lines)+Alp(2 lines)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}				
	,{ 9,PRODUCE_QWERTY_2,		_T("P2.2 Qwerty+Chinese(phrase+2 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}		
	,{10,PRODUCE_QWERTY_3,		_T("P2.3 Qwerty+Chinese(phrase+3 lines)+Alp(3 lines)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}				
	,{11,PRODUCE_QWERTY_4,		_T("P2.4 Qwerty+Chinese(phrase+3 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}		

	,{12,PRODUCE_QWERTY_5,		_T("P2.1 Qwerty+Chinese(word+2 lines)+Alp(2 lines)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}				
	,{13,PRODUCE_QWERTY_6,		_T("P2.2 Qwerty+Chinese(word+2 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}		
	,{14,PRODUCE_QWERTY_7,		_T("P2.3 Qwerty+Chinese(word+3 lines)+Alp(3 lines)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}				
	,{15,PRODUCE_QWERTY_8,		_T("P2.4 Qwerty+Chinese(word+3 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	

	,{16,PRODUCE_HALF_QWERTY_1,	_T("P2.1 Half-Qwerty+Chinese(phrase+2 lines)+Alp(2 lines)"),		{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}	
	,{17,PRODUCE_HALF_QWERTY_2,	_T("P2.2 Half-Qwerty+Chinese(phrase+2 lines)+Alp(inline)"),		{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}		
	,{18,PRODUCE_HALF_QWERTY_3,	_T("P2.3 Half-Qwerty+Chinese(phrase+3 lines)+Alp(3 lines)"),		{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}		
	,{19,PRODUCE_HALF_QWERTY_4,	_T("P2.4 Half-Qwerty+Chinese(phrase+3 lines)+Alp(inline)"),		{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	

	,{20,PRODUCE_HALF_QWERTY_5,	_T("P2.1 Half-Qwerty+Chinese(word+2 lines)+Alp(2 lines)"),		{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}	
	,{21,PRODUCE_HALF_QWERTY_6,	_T("P2.2 Half-Qwerty+Chinese(word+2 lines)+Alp(inline)"),		{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}		
	,{22,PRODUCE_HALF_QWERTY_7,	_T("P2.3 Half-Qwerty+Chinese(word+3 lines)+Alp(3 lines)"),		{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}		
	,{23,PRODUCE_HALF_QWERTY_8,	_T("P2.4 Half-Qwerty+Chinese(word+3 lines)+Alp(inline)"),		{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	

	,{24,PRODUCE_NUMPAD_9,		_T("Number+Tradition Chinese(phrase+2 lines)+Alp(inline)"),		{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}
	,{25,PRODUCE_NUMPAD_11,		_T("Number+Touch screen Chinese(phrase+2 lines)+Alp(2 lines)"),		{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}	
	,{26,PRODUCE_NUMPAD_10,		_T("Number+Tradition Chinese(word+2 lines)+Alp(inline)"),		{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}
	,{27,PRODUCE_QWERTY_9,		_T("Qwerty+Touch screen Chinese(phrase+2 lines)+Alp(2 lines)"),		{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}	

	,{28,PRODUCE_EXPLICIT_1,	_T("Explicit+Chinese(phrase+2 lines)+Alp(2 lines)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}
	,{29,PRODUCE_EXPLICIT_2,	_T("Explicit+Chinese(phrase+2 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}	
	,{30,PRODUCE_EXPLICIT_3,	_T("Explicit+Chinese(phrase+3 lines)+Alp(3 lines)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}			
	,{31,PRODUCE_EXPLICIT_4,	_T("Explicit+Chinese(phrase+3 lines)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	
#else
	{ 0,PRODUCE_NUMPAD_1,		_T("P1.1 Number+中文(词语+二行)+Alp(二行)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}	
	,{ 1,PRODUCE_NUMPAD_2,		_T("P1.2 Number+中文(词语+二行)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}				
	,{ 2,PRODUCE_NUMPAD_3,		_T("P1.3 Number+中文(词语+三行)+Alp(三行)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}				
	,{ 3,PRODUCE_NUMPAD_4,		_T("P1.4 Number+中文(词语+三行)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	

	,{ 4,PRODUCE_NUMPAD_5,		_T("P1.1 Number+中文(单字+二行)+Alp(二行)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}		
	,{ 5,PRODUCE_NUMPAD_6,		_T("P1.2 Number+中文(单字+二行)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}				
	,{ 6,PRODUCE_NUMPAD_7,		_T("P1.3 Number+中文(单字+三行)+Alp(三行)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}				
	,{ 7,PRODUCE_NUMPAD_8,		_T("P1.4 Number+中文(单字+三行)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	

	,{ 8,PRODUCE_QWERTY_1,		_T("P2.1 Qwerty+中文(词语+二行)+Alp(二行)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}				
	,{ 9,PRODUCE_QWERTY_2,		_T("P2.2 Qwerty+中文(词语+二行)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}		
	,{10,PRODUCE_QWERTY_3,		_T("P2.3 Qwerty+中文(词语+三行)+Alp(三行)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}				
	,{11,PRODUCE_QWERTY_4,		_T("P2.4 Qwerty+中文(词语+三行)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}		

	,{12,PRODUCE_QWERTY_5,		_T("P2.1 Qwerty+中文(单字+二行)+Alp(二行)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}				
	,{13,PRODUCE_QWERTY_6,		_T("P2.2 Qwerty+中文(单字+二行)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}		
	,{14,PRODUCE_QWERTY_7,		_T("P2.3 Qwerty+中文(单字+三行)+Alp(三行)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}				
	,{15,PRODUCE_QWERTY_8,		_T("P2.4 Qwerty+中文(单字+三行)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	

	,{16,PRODUCE_HALF_QWERTY_1,	_T("P2.1 Half-Qwerty+中文(词语+二行)+Alp(二行)"),		{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}	
	,{17,PRODUCE_HALF_QWERTY_2,	_T("P2.2 Half-Qwerty+中文(词语+二行)+Alp(inline)"),		{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}		
	,{18,PRODUCE_HALF_QWERTY_3,	_T("P2.3 Half-Qwerty+中文(词语+三行)+Alp(三行)"),		{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}		
	,{19,PRODUCE_HALF_QWERTY_4,	_T("P2.4 Half-Qwerty+中文(词语+三行)+Alp(inline)"),		{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	

	,{20,PRODUCE_HALF_QWERTY_5,	_T("P2.1 Half-Qwerty+中文(单字+二行)+Alp(二行)"),		{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}	
	,{21,PRODUCE_HALF_QWERTY_6,	_T("P2.2 Half-Qwerty+中文(单字+二行)+Alp(inline)"),		{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}		
	,{22,PRODUCE_HALF_QWERTY_7,	_T("P2.3 Half-Qwerty+中文(单字+三行)+Alp(三行)"),		{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}		
	,{23,PRODUCE_HALF_QWERTY_8,	_T("P2.4 Half-Qwerty+中文(单字+三行)+Alp(inline)"),		{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}

	,{24,PRODUCE_NUMPAD_9,		_T("Number+传统中文(词语+二行)+Alp(inline)"),		{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}
	,{25,PRODUCE_NUMPAD_11,		_T("Number+触摸屏 中文(词语+二行)+Alp(二行)"),		{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}	
	,{26,PRODUCE_NUMPAD_10,		_T("Number+传统中文(单字+二行)+Alp(inline)"),		{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}
	,{27,PRODUCE_QWERTY_9,		_T("Qwerty+触摸屏 中文(词语+二行)+Alp(二行)"),		{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}	

	,{28,PRODUCE_EXPLICIT_1,	_T("Explicit+中文(词语+二行)+Alp(二行)"),			{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}
	,{29,PRODUCE_EXPLICIT_2,	_T("Explicit+中文(词语+二行)+Alp(inline)"),			{GBCL_ROW_KEYBD,1,GBCL_SINGLE_CAND,	1}, "", ""}	
	,{30,PRODUCE_EXPLICIT_3,	_T("Explicit+中文(词语+三行)+Alp(三行)"),			{GBCL_ROW_KEYBD,2,GBCL_ROW_KEYBD,	2}, "", ""}			
	,{31,PRODUCE_EXPLICIT_4,	_T("Explicit+中文(词语+三行)+Alp(inline)"),			{GBCL_ROW_KEYBD,2,GBCL_SINGLE_CAND,	2}, "", ""}	


	//,{28,CURRENT_ENGINE_CONFIG,	_T("当前引擎配置"),							{GBCL_ROW_KEYBD,1,GBCL_ROW_KEYBD,	1}, "", ""}

#endif

	,{-1,PRODUCE_MAX,{0},{0}},
};


// 引擎配置数据
GBEngineOptionData _huge configData[]=
{
	/*中文输入法配置*/
	//模糊音配置
	{ 0,	GBCFG_PINYIN_FUZZY_H_F,							GB_ENGINE_CHINESE,	0,	1}
	,{ 1,	GBCFG_PINYIN_FUZZY_K_G,							GB_ENGINE_CHINESE,	0,	1}
	,{ 2,	GBCFG_PINYIN_FUZZY_N_L,							GB_ENGINE_CHINESE,	0,	1}
	,{ 3,	GBCFG_PINYIN_FUZZY_R_L,							GB_ENGINE_CHINESE,	0,	1}
	,{ 4,	GBCFG_PINYIN_FUZZY_NG_N,						GB_ENGINE_CHINESE,	0,	1}
	,{ 5,	GBCFG_PINYIN_FUZZY_XH_X,						GB_ENGINE_CHINESE,	0,	1}
	//音节相关配置
	,{ 6,	GBCFG_OUT_ZI_SYLLABLE,							GB_ENGINE_CHINESE,	0,	1}
	,{ 7,	GBCFG_TRANSLATE_INPUT_STRING,					GB_ENGINE_CHINESE,	0,	1}
	,{ 8,	GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE,	GB_ENGINE_CHINESE,	0,	1}
	,{ 9,	GBCFG_PY_1_MATCH_FUNC,							GB_ENGINE_CHINESE,	0,	1}
	//词语相关配置
	,{10,	GBCFG_CLOSE_PHRASE,								GB_ENGINE_CHINESE,	0,	1}
	,{11,	GBCFG_ONLY_ENTIRE_PHRASE_CAND,					GB_ENGINE_CHINESE,	0,	1}
	,{12,	GBCFG_SET_PHRASE_SPLIT_CHAR,					GB_ENGINE_CHINESE,	0,	1}
	,{13,	GBCFG_SET_PHRASE_LEN_LIMIT,						GB_ENGINE_CHINESE,	0,	1}
	,{14,	GBCFG_CLOSE_ASSOC_PHRASE,						GB_ENGINE_CHINESE,	0,	1}
	,{15,	GBCFG_FIND_ONLY_FULL_MATCHING,					GB_ENGINE_CHINESE,	0,	1}
	,{16,	GBCFG_ASSOCIATE_INTEPCT,						GB_ENGINE_CHINESE,	0,	1}
	,{17,	GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD,			GB_ENGINE_CHINESE,	0,	1}
	/*其他配置*/
	,{18,	GBCFG_CLOSE_COMPONENT,							GB_ENGINE_CHINESE,	0,	1}
	,{19,	GBCFG_NOT_ALLOW_USELESS_STROKE,					GB_ENGINE_CHINESE,	0,	1}
	,{20,	GBCFG_SET_STROKE_WILDCHAR_SHOW,					GB_ENGINE_CHINESE,	0,	1}
	,{21,	GBCFG_CHARSET_SET,								GB_ENGINE_CHINESE,	0,	1}
	,{22,	GBCFG_INIT_GET_HIGHEST_FREQ_WORD,				GB_ENGINE_CHINESE,	0,	1}
	/*Alphabetic 输入法配置*/
	,{23,	GBALP_AUTO_CORRECTION,							GB_ENGINE_ALPHABETIC,	0,	1}
	,{24,	GBALP_CLOSE_AUTO_COMPLETION,					GB_ENGINE_ALPHABETIC,	0,	1}
	,{25,	GBALP_INPUTSTRING_IS_FIRST_CANDIDATE,			GB_ENGINE_ALPHABETIC,	0,	1}
	,{26,	GBALP_QWERTY_OUTPUT_DEFAULT_STRING,				GB_ENGINE_ALPHABETIC,	0,	1}
	,{27,	GBALP_FIRST_KEY_ASSOC,							GB_ENGINE_ALPHABETIC,	0,	1}
	,{28,	GBALP_ASSOC_NO_DERIVE,							GB_ENGINE_ALPHABETIC,	0,	1}
	/*GBAPI & GBMMI配置*/
	,{29,	GBFO_Alp_Up_To_Input_Key_Sequence,				GB_ENGINE_FUNCTION,	0,	1}
	,{30,	GBFO_Chn_Up_To_Input_Key_Sequence,				GB_ENGINE_FUNCTION,	0,	1}
	,{31,	GBFO_Chn_No_Response_When_Key_No_Cand,			GB_ENGINE_FUNCTION,	0,	1}
	,{32,	GBFO_Alp_Add_Space_After_Cand_Confirmed,		GB_ENGINE_FUNCTION,	0,	1}
	,{33,	GBFO_Alp_UAW,									GB_ENGINE_FUNCTION,	0,	1}
	,{34,	GBFO_OK_Enter_Selected,							GB_ENGINE_SYNTAX,	0,	1}
	,{35,	GBFO_Chn_Syntex_Key_Sequence_Edit,				GB_ENGINE_SYNTAX,	0,	1}
	,{36,	GBFO_Chn_Switch_Alp,							GB_ENGINE_FUNCTION,	0,	1}
	,{37,	GBFO_API_Stop_Log,								GB_ENGINE_FUNCTION,	0,	1}
	,{38,	GBFO_Alp_UAW_Confirm,							GB_ENGINE_FUNCTION,	0,	1}
	,{39,	GBFO_Multi_Tap_Show_Cand_Wnd,					GB_ENGINE_FUNCTION,	0,	1}
	,{40,	GBFO_Scroll_Cand_By_Row,						GB_ENGINE_FUNCTION,	0,	1}
	,{41,	GBFO_Tch_Scr,									GB_ENGINE_FUNCTION,	0,	1}
	,{42,	GBFO_Chn_Display_Input_Key_Sequence,			GB_ENGINE_FUNCTION,	0,	1}
	,{43,	GBFO_Chn_All_Num_Can_Continuously_Input,		GB_ENGINE_FUNCTION,	0,	1}
	,{44,	GBFO_Alp_All_Num_Can_Continuously_Input,		GB_ENGINE_FUNCTION,	0,	1}
	,{45,	GBFO_Alp_No_Response_When_Key_No_Cand,			GB_ENGINE_FUNCTION,	0,	1}

	/*Alphabetic 输入法配置*/
	,{46,	GBALP_KEEP_ALL_HEADER,							GB_ENGINE_ALPHABETIC,	0,	1}
	,{47,	GBALP_SET_AVOID_PATENT_SYMBOL,					GB_ENGINE_ALPHABETIC,	0,	1}
	,{48,	GBFO_Alp_Paging_By_Lower,						GB_ENGINE_FUNCTION,		0,	1}
	,{49,	GBALP_SET_EMBEDDED_INTERPUNCTION_KEY,			GB_ENGINE_ALPHABETIC,	0,	1}
	//added by weizhiping at 2011-1-4 15:48:23
	//note : 添加单字母候选大小写更改配置项	
	,{50,	GBFO_Alp_Get_Only_Lower,						GB_ENGINE_FUNCTION,		0,	1}
	,{51,	GBFO_Alp_Get_Only_Cap,							GB_ENGINE_FUNCTION,		0,	1}
	//added by weizhiping end
	,{-1,-1,-1,-1,-1},
};


////////////////////////中文输入法配置//////////////////////////////////////////////////
// GBEngineSetInfo.index和GBEngineOptionData.index一一对应
// index == -1:表示该项和GBEngineOptionData没有对应关系.
// GBEngineSetInfo.pChild表示子列表.
/** \brief 中文配置 -  模糊音配置*/
GBEngineSetInfo _huge ChnFuzzyConfigData[] = 
{
	//模糊音配置
	{ 0,	_T("f = h"),				GET_STR(GBCFG_PINYIN_FUZZY_H_F), GET_STR(GB_ENGINE_CHINESE), "0,1",	NULL}
	,{ 1,	_T("k = g"),				GET_STR(GBCFG_PINYIN_FUZZY_K_G), GET_STR(GB_ENGINE_CHINESE), "0,1",	NULL}
	,{ 2,	_T("n = l"),				GET_STR(GBCFG_PINYIN_FUZZY_N_L), GET_STR(GB_ENGINE_CHINESE), "0,1",	NULL}
	,{ 3,	_T("r = l"),				GET_STR(GBCFG_PINYIN_FUZZY_R_L), GET_STR(GB_ENGINE_CHINESE), "0,1",	NULL}
	,{ 4,	_T("en=eng an=ang in=ing"),GET_STR(GBCFG_PINYIN_FUZZY_NG_N), GET_STR(GB_ENGINE_CHINESE), "0,1",	NULL}
	,{ 5,	_T("s = sh z = zh c = ch"),	GET_STR(GBCFG_PINYIN_FUZZY_XH_X), GET_STR(GB_ENGINE_CHINESE), "0,1",NULL}
	,{-1,	NULL, "", "",	NULL,NULL}
};
GBEngineSetInfo _huge ChnSyllableConfigData[] = 
{
#if CONSOLE_ENGLISH > 0
	{ 6,	_T("Chn-Match Pinyin candidates with first character"),						GET_STR(GBCFG_OUT_ZI_SYLLABLE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{ 7,	_T("Chn-Turn input string into matched Pinyin"),				GET_STR(GBCFG_TRANSLATE_INPUT_STRING), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{ 8,	_T("Chn-Matched Pinyin string as first candidate"),		GET_STR(GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{ 9,	_T("Chn-No Pinyin prediction"),						GET_STR(GBCFG_PY_1_MATCH_FUNC), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
#else
	{ 6,	_T("Chn-只输出字的拼音"),						GET_STR(GBCFG_OUT_ZI_SYLLABLE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{ 7,	_T("Chn-将输入字符串转为拼音串"),				GET_STR(GBCFG_TRANSLATE_INPUT_STRING), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{ 8,	_T("Chn-将转换后的字符串加入拼音音节列表"),		GET_STR(GBCFG_ADD_TRANSLATE_INPUT_STRING_TO_SYLLABLE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{ 9,	_T("Chn-单字拼音精确匹配"),						GET_STR(GBCFG_PY_1_MATCH_FUNC), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
#endif
	,{-1,	NULL, "", "", NULL,NULL}
};
GBEngineSetInfo _huge ChnPhraseConfigData[] = 
{
#if CONSOLE_ENGLISH > 0
	{10, _T("Chn-Close phrase input"),									GET_STR(GBCFG_CLOSE_PHRASE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{11, _T("Chn-Fully matched phrase and possible matched characters as candidates"),				GET_STR(GBCFG_ONLY_ENTIRE_PHRASE_CAND), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{12, _T("Chn-Separator sign"),							GET_STR(GBCFG_SET_PHRASE_SPLIT_CHAR), GET_STR(GB_ENGINE_CHINESE),	"',\\,/,#",NULL}//编辑框
	,{13, _T("Maximum length of phrase(No limit,2,3,4,5,6,7,8,9.zero means no limit of phrase length)"),GET_STR(GBCFG_SET_PHRASE_LEN_LIMIT), GET_STR(GB_ENGINE_CHINESE),	"0,2,3,4,5,6,7,8,9",NULL}//编辑框
	,{14, _T("Chn-Close phrase association"),									GET_STR(GBCFG_CLOSE_ASSOC_PHRASE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{15, _T("Chn-Fully matched phrase as candidates"),				GET_STR(GBCFG_FIND_ONLY_FULL_MATCHING), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{16, _T("Chn-Smart punctuations"),								GET_STR(GBCFG_ASSOCIATE_INTEPCT), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{17, _T("Chn-No default popular words in association"),							GET_STR(GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
#else
	{10, _T("Chn-关闭词语功能"),									GET_STR(GBCFG_CLOSE_PHRASE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{11, _T("Chn-只匹配完整的词语,并且匹配可能的字"),				GET_STR(GBCFG_ONLY_ENTIRE_PHRASE_CAND), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{12, _T("Chn-设置分隔符号的显示字符"),							GET_STR(GBCFG_SET_PHRASE_SPLIT_CHAR), GET_STR(GB_ENGINE_CHINESE),	"',\\,/,#",NULL}//编辑框
	,{13, _T("设置词语的最大长度限制（不限制，2，3，4，5，6，7，8，9，0表示不限制任何词语）"),GET_STR(GBCFG_SET_PHRASE_LEN_LIMIT), GET_STR(GB_ENGINE_CHINESE),	"0,2,3,4,5,6,7,8,9",NULL}//编辑框
	,{14, _T("Chn-关闭词语联想"),									GET_STR(GBCFG_CLOSE_ASSOC_PHRASE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{15, _T("Chn-只匹配完整的词，并且不匹配可能的字"),				GET_STR(GBCFG_FIND_ONLY_FULL_MATCHING), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{16, _T("Chn-支持智能标点预测"),								GET_STR(GBCFG_ASSOCIATE_INTEPCT), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{17, _T("Chn-联想的时候不填充高频字"),							GET_STR(GBCFG_ASSOC_NOT_FILL_HIGHEST_FREQ_WORD), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
#endif
	,{-1,	NULL, "", "", NULL,NULL}
};
GBEngineSetInfo _huge ChnOtherChnData[] = 
{
#if CONSOLE_ENGLISH > 0
	{18,_T("Chn-Close component association"),											GET_STR(GBCFG_CLOSE_COMPONENT), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{19,_T("Chn-In Stroke and Korean mode, no candidate displayed after long input string"),		GET_STR(GBCFG_NOT_ALLOW_USELESS_STROKE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{20,_T("Universal unknown stroke sign(Only for OEM)"),						GET_STR(GBCFG_SET_STROKE_WILDCHAR_SHOW), GET_STR(GB_ENGINE_CHINESE),	"?,*,#",NULL}//编辑框
	,{21,_T("Character set(all,gb2312,big5)"),					GET_STR(GBCFG_CHARSET_SET), GET_STR(GB_ENGINE_CHINESE),	"0,1,2,3,4",NULL}//下拉框
	,{22,_T("Chn-Obtain default popular words"),										GET_STR(GBCFG_INIT_GET_HIGHEST_FREQ_WORD), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
#else
	{18,_T("Chn-关闭部首功能"),											GET_STR(GBCFG_CLOSE_COMPONENT), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{19,_T("Chn-笔画和韩文输入法下，输入超长串后不允许出来候选"),		GET_STR(GBCFG_NOT_ALLOW_USELESS_STROKE), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
	,{20,_T("设置笔画通配符符号（只能OEM配置）"),						GET_STR(GBCFG_SET_STROKE_WILDCHAR_SHOW), GET_STR(GB_ENGINE_CHINESE),	"?,*,#",NULL}//编辑框
	,{21,_T("设置字符集（all,gb2312,big5）"),					GET_STR(GBCFG_CHARSET_SET), GET_STR(GB_ENGINE_CHINESE),	"0,1,2",NULL}//下拉框
	,{22,_T("Chn-获取默认高频字"),										GET_STR(GBCFG_INIT_GET_HIGHEST_FREQ_WORD), GET_STR(GB_ENGINE_CHINESE),	"0,1",NULL}
#endif
	,{-1,	NULL, "", "", NULL,NULL}
};

GBEngineSetInfo _huge ChnConfigData[] = 
{
#if CONSOLE_ENGLISH > 0
	{-1,	_T("Fuzzy Pinyin settings"),			"", "",	NULL,ChnFuzzyConfigData}
	,{-1,	_T("Pinyin settings"),					"", "",	NULL,ChnSyllableConfigData}
	,{-1,	_T("Word/phrase candidate settings"),	"", "",	NULL,ChnPhraseConfigData}
	,{-1,	_T("Other settings"),					"", "",	NULL,ChnOtherChnData}
#else
	{-1,	_T("模糊音配置"),		 "", "",	NULL,ChnFuzzyConfigData}
	,{-1,	_T("音节相关配置"),		 "", "",	NULL,ChnSyllableConfigData}
	,{-1,	_T("词语相关配置"),		 "", "",	NULL,ChnPhraseConfigData}
	,{-1,	_T("其他配置"),			 "", "",	NULL,ChnOtherChnData}
#endif
	,{-1,	NULL, "", "", NULL,NULL}
};

;

/**********************************
 * \brief 设置选择的产品
 * \param index 
 * \return 
 ***********************************/
GBINT SetProductList(GBINT index)
{
	g_currentProduct = index;
	updateConfigDataByProduce(ProductList[index].id);	
	return 1;
}

/**********************************
 * \brief 根据提供的产品id返回该产品在ProductList中的下标
 * \param nProductID 产品id
 * \return ProductList中的下标
**********************************/
GBINT GetProductIndex(GBINT nProductID)
{
	GBINT index = 0;

	while(ProductList[index].index != -1)
	{
		if(nProductID == ProductList[index].id)
			break;
		index++;
	}
	return index;
}

void GBResetAllEngineOption(void)
{
	GBINT idx = 0;// 配置数据操作相关
	GBEngineOptionData *item = NULL;
	for (idx = 0; idx < ARRAY_DIM(configData); idx++)
	{
		item = &configData[idx];
		GBSetEngineOptionStatus(item->type, item->id, 0);
		GBSetEngineOptionEnable(item->type, item->id, 1);
	}
}

void GBSetEngineOptionStatus(int type, int id, int status)
{
	GBINT idx = 0;

	for (idx = 0; idx < ARRAY_DIM(configData); idx++)
	{
		if(id == configData[idx].id && type == configData[idx].type)
		{
			configData[idx].status = status;
			break;
		}
	}
}


void GBSetEngineOptionEnable(int type, int id, int enable)
{
	GBINT idx = 0;
	for (idx = 0; idx < ARRAY_DIM(configData); idx++)
	{
		if(id == configData[idx].id && type == configData[idx].type)
		{
			configData[idx].flag = enable;
			break;
		}
	}
}

extern void getContentLineParam(char *line, char **param, int *num)
{
	int count = 0;
	char *pCursor = NULL;
	char *pContent = line;
	if (NULL == line || 0 == strlen(line))
	{
		return;
	}
	pCursor = strstr(pContent, "\t");
	while (pCursor)
	{
		param[count++] = pContent;
		*pCursor = '\0';
		pContent = pCursor + 1;
		pCursor = strstr(pContent, "\t");
	}
	pCursor = strstr(pContent, "\r\n");
	if (pCursor)
	{
		*pCursor = '\0';
		param[count++] = pContent;
	}
	*num = count;
}

void UpdateConfig(U8 upper_lower)
{
	int i = 0;
	for(i = 0; configData[i].index != -1;i++)
	{
		GBSetEngineOption(&g_gbis, configData[i].type, configData[i].id, configData[i].status);
		GBSetEngineOption(&g_gbis,GB_ENGINE_FUNCTION,GBFO_Alp_No_Response_When_Key_No_Cand,1) ;
		GBSetEngineOption(&g_gbis,GB_ENGINE_FUNCTION,GBFO_Chn_No_Response_When_Key_No_Cand,1) ;
		if(upper_lower == 0)
			GBSetEngineOption(&g_gbis,GB_ENGINE_FUNCTION,GBFO_Alp_Get_Only_Lower,1);
		else
			GBSetEngineOption(&g_gbis,GB_ENGINE_FUNCTION,GBFO_Alp_Get_Only_Cap,1);
	}
}

#else

/*!
 * \brief 设置引擎配置
 * \note 在关闭Console配置功能的情况下设置该配置项
 * \	Tasking中打开配置会编译不过。并且该函数中GBSetEngineOption使用次数过多也会出现运行不了的情况
 * \return 
 */
void SetEngineConfig(void)
{

	GBSetEngineOption(&g_gbis, 0, 262144, 1);
	GBSetEngineOption(&g_gbis, 0, 2097152, 1);
	GBSetEngineOption(&g_gbis, 1, 512, 1);
	GBSetEngineOption(&g_gbis, 3, 1, 1);
	GBSetEngineOption(&g_gbis, 3, 2, 1);
	GBSetEngineOption(&g_gbis, 3, 16, 1);
	GBSetEngineOption(&g_gbis, 3, 512, 1);
	GBSetEngineOption(&g_gbis, 3, 1024, 1);
	GBSetEngineOption(&g_gbis, 3, 8192, 1);
	GBSetEngineOption(&g_gbis, 3, 131072, 1);
	GBSetEngineOption(&g_gbis, 3, 262144, 1);
	GBSetEngineOption(&g_gbis, 3, 524288, 1);
	GBSetEngineOption(&g_gbis, GB_ENGINE_ALPHABETIC, GBALP_INPUTSTRING_IS_FIRST_CANDIDATE, 1);

// 	GBSetEngineOption(&g_gbis, 0, 256, 0);
// 	GBSetEngineOption(&g_gbis, 0, 512, 0);
// 	GBSetEngineOption(&g_gbis, 0, 1024, 0);
// 	GBSetEngineOption(&g_gbis, 0, 16384, 0);
// 	GBSetEngineOption(&g_gbis, 0, 16385, 0);
// 	GBSetEngineOption(&g_gbis, 0, 16387, 0);
// 	GBSetEngineOption(&g_gbis, 0, 16388, 0);
// 	GBSetEngineOption(&g_gbis, 0, 65536, 0);
// 	GBSetEngineOption(&g_gbis, 0, 131072, 0);
// 	GBSetEngineOption(&g_gbis, 0, 524288, 0);
// 	GBSetEngineOption(&g_gbis, 0, 1048576, 0);
// 	GBSetEngineOption(&g_gbis, 0, 4194304, 0);
// 	GBSetEngineOption(&g_gbis, 0, 8388608, 0);
// 	GBSetEngineOption(&g_gbis, 0, 16777216, 0);
// 	GBSetEngineOption(&g_gbis, 0, 33554432, 0);
// 	GBSetEngineOption(&g_gbis, 1, 16, 0);
// 	GBSetEngineOption(&g_gbis, 1, 32, 0);
// 	GBSetEngineOption(&g_gbis, 1, 64, 0);
// 	GBSetEngineOption(&g_gbis, 1, 128, 0);
// 	GBSetEngineOption(&g_gbis, 1, 256, 0);
// 	GBSetEngineOption(&g_gbis, 1, 1024, 0);
// 	GBSetEngineOption(&g_gbis, 1, 2048, 0);
// 	GBSetEngineOption(&g_gbis, 1, 4096, 0);
//	GBSetEngineOption(&g_gbis, 3, 4, 0);
//	GBSetEngineOption(&g_gbis, 2, 64, 0);
//	GBSetEngineOption(&g_gbis, 2, 128, 0);
//	GBSetEngineOption(&g_gbis, 3, 2048, 0);
//	GBSetEngineOption(&g_gbis, 3, 4096, 0);
//	GBSetEngineOption(&g_gbis, 3, 16384, 0);
//	GBSetEngineOption(&g_gbis, 3, 1048576, 0);
//	GBSetEngineOption(&g_gbis, 3, 2097152, 0);
}
#endif
#endif/*__MMI_GB_V5__*/