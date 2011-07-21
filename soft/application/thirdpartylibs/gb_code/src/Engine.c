#include "mmi_features.h"
#include "engine.h"
//#include "engineconfig.h"
//#include "configurebusiness.h"
#include "producebusiness.h"
#include "debuginitdef.h"
#ifdef __MMI_GB_V5__
GBU16			g_nDataCount;
GBInputStruct	_huge g_gbis;
GBAuxInfo		g_aux;
GBLDB			g_ldbs[MAX_DATA_COUNT];

// 内部函数接口定义
static GBINT	initEngine(void);
static void	setPageOption(void);
static GBINT	loadEngineLDB(GBACHAR *path, GBINT isEBDFile);
//static void	GBEngineAddMDB();

/*************************************************
 * \brief 加载语言数据 
 * \param path 
 * \return 
*************************************************/
static GBINT loadEngineLDB(GBACHAR *path, GBINT isEBDFile)
{
	GBLANG		lang;
	GBLANG		subLang;
	GBU32		fileSize;
	if(g_nDataCount >= MAX_DATA_COUNT)
	{
		return FALSE;
	}
#ifdef WIN32
	if(isEBDFile)
	{
		FILE * fp = NULL;
		GBU8 header_data[256];

		
		fp = fopen(path, "rb");
		if(!fp)
		{
			return FALSE;
		}
		if(fread(header_data, sizeof(header_data), 1, fp) == 1)
		{
			if(GBGetDataLanguage((GBLPCVOID)header_data, (GBLANG*)&lang, (GBSUBLANG*)&subLang, &fileSize) == GBOK
				&& !subLang)
			{
				g_ldbs[g_nDataCount].nLang = lang;
				g_ldbs[g_nDataCount].nSubLang = subLang;
				//ASSERT(fileSize < 1024 * 1024 * 4);
				g_ldbs[g_nDataCount].pData = (GBLPCVOID)malloc(fileSize);
				fseek(fp, 0, SEEK_SET);
				if(fread((GBU8*)g_ldbs[g_nDataCount].pData, 1, fileSize, fp) != fileSize)
				{
					goto RET_FALSE;
				}
				fclose(fp);
				g_nDataCount++;
				return TRUE;
			}
		}
RET_FALSE:
		fclose(fp);
		return FALSE;
	}
	else
	{
		if(GBGetDataLanguage((const unsigned long*)path, (GBLANG*)&lang, (GBSUBLANG*)&subLang, &fileSize) == GBOK
			&& !subLang)
		{
			g_ldbs[g_nDataCount].nLang = lang;
			g_ldbs[g_nDataCount].nSubLang = subLang;
			g_ldbs[g_nDataCount].pData = (const unsigned long*)path;
			g_nDataCount++;
			return TRUE;
		}
		return FALSE;
	}

#else	
	isEBDFile;
	if(GBGetDataLanguage((const unsigned long*)path, (GBLANG*)&lang, (GBSUBLANG*)&subLang, &fileSize) == GBOK
		&& !subLang)
	{
		g_ldbs[g_nDataCount].nLang = lang;
		g_ldbs[g_nDataCount].nSubLang = subLang;
		g_ldbs[g_nDataCount].pData = (const unsigned long*)path;
		g_nDataCount++;
		return TRUE;
	}
	return FALSE;
#endif
}

/*************************************************
 * \brief 初始化引擎
 * \return 
*************************************************/
static GBINT initEngine()
{
	int ret = 0;
	GBTerminate(&g_gbis);
	g_gbis.pAuxInfo = &g_aux;
	g_gbis.engine_private_buffer_size = sizeof(g_gbis.engine_private_buffer);
	ret = GBInitialize(&g_gbis, g_ldbs, g_nDataCount);
	if(ret == GBOK)
	{
		return 0;
	}
	else
	{
	
		MMI_ASSERT(0);
		return 1;
	}
}

/*************************************************
 * \brief 设置分页选项 
 * \param void 
 * \return 
*************************************************/
static void setPageOption(void)
{
	GBGlobalPageOption	pageOption;

	// 设置候选框分页参数
	GBGetPageOption(&g_gbis, &pageOption);
	//pageOption.pGetStringWidthParam = 0;
	pageOption.mChnCandPageOption.nTotalWidthInPixel = PREEDIT_WIDTH;
	pageOption.mChnCandPageOption.nCharWidthInPixel = FONT_WIDTH;
	pageOption.mChnCandPageOption.nSplitWidthInPixel = 5;

	pageOption.mSyllablePageOption.nMaxCandCountPerPage = 10;
	pageOption.mSyllablePageOption.nTotalWidthInPixel = PREEDIT_WIDTH;
	pageOption.mSyllablePageOption.nCharWidthInPixel = FONT_WIDTH;
	pageOption.mSyllablePageOption.nSplitWidthInPixel = 5;

	pageOption.mPageOption.nMaxCandCountPerPage = 8;
	pageOption.mPageOption.nTotalWidthInPixel = PREEDIT_WIDTH;
	pageOption.mPageOption.nCharWidthInPixel = FONT_WIDTH;
	pageOption.mPageOption.nSplitWidthInPixel = 5;

	GBSetPageOption(&g_gbis, &pageOption);
}

/*************************************************
 * \brief 读取语言数据
 * \param void 
 * \return 
 ************************************************/
void LoadCData(void)
{
	//读取.c格式数据文件	
	
	loadEngineLDB((GBACHAR *)prv_enDataArray, FALSE);
	
	#if defined(__MMI_GB_SM_CHINESE__)
	loadEngineLDB((GBACHAR *)prv_scDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_URDU__)
	loadEngineLDB((GBACHAR *)prv_urDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_ARABIC__)
	loadEngineLDB((GBACHAR *) prv_enArabicDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_GERMAN__)
	loadEngineLDB((GBACHAR *) prv_gerDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_FRENCH__)
	loadEngineLDB((GBACHAR *) prv_frDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_ITALIAN__)
	loadEngineLDB((GBACHAR *) prv_italianDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_MALAY__)
	loadEngineLDB((GBACHAR *) prv_malayDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_TAMIL__)
	loadEngineLDB((GBACHAR *)prv_tamilDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_SPANISH__)
	loadEngineLDB((GBACHAR *)prv_spaDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_BENGALI__)
	loadEngineLDB((GBACHAR *)prv_bengailDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_HINDI__)
	loadEngineLDB((GBACHAR *)prv_hiDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_INDONESIAN__)
	loadEngineLDB((GBACHAR *)prv_indonesianDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_TELUGU__)
	loadEngineLDB((GBACHAR *)prv_teDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_VIETNAMESE__)
	loadEngineLDB((GBACHAR *)prv_viDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_THAI__)
	loadEngineLDB((GBACHAR *)prv_thDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_PUNJABI__)
	loadEngineLDB((GBACHAR *)prv_punjabiDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_PERSIAN__)
	loadEngineLDB((GBACHAR *)prv_enPersianDataArray, FALSE);
	#endif
	#if defined(__MMI_GB_SMART_PORTUGUESE__)
	loadEngineLDB((GBACHAR *)prv_ptDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_RUSSIAN__)
	loadEngineLDB((GBACHAR *)prv_ruDataArray, FALSE);
	#endif
	#if defined(__MMI_SMART_TURKISH__)
	loadEngineLDB((GBACHAR *)prv_turkishDataArray, FALSE);
	#endif
}

/*************************************************
 * \brief 引擎初始化 
 * \param void 
 * \return 
 ************************************************/
void newEngine(void)
{
	if(GBOK != GBIsInited(&g_gbis))
	{
		LoadCData();

		// 初始化引擎
		initEngine();
		
	#if OPEN_CONFIG > 0
		//note : 加载产品和配置并设置一个默认的产品
		loadProduceConfig();
		loadRelationConfig();
		SetProductList(4);//设置一个默认的产品配置
	#else
		SetEngineConfig();
	#endif

		// 候选分页设置
		setPageOption();
	}
}
#endif/*__MMI_GB_V5__*/