#include "mmi_features.h"
#include "eb_funcs.h"
#ifdef __MMI_GB_V5__
#else
GBAPI GBRETURN EBStartup(GBLPCVOID pdata) ;
GBAPI GBRETURN EBInputMethodInit(pEBInputStruct pEBIS) ;

GBAPI GBRETURN EBSwitchInputMethod(pEBInputStruct pEBIS, GBUINT8 inputMethod) ;
GBAPI GBRETURN EBFindFirstPageC(pEBInputStruct pEBIS) ;
GBAPI GBRETURN EBInputReset(pEBInputStruct pEBIS) ;
GBAPI GBRETURN EBAssocFirstPageC(pEBInputStruct pEBIS) ;
GBAPI void   EBShutdown(void) ;
GBAPI GBRETURN EBStringToStroke(pcEBInputStruct pEBIS, GBPACHAR pBuffer, GBINT bufferLen)
;

GBAPI GBRETURN EBCanPageDown(pcEBInputStruct pEBIS) 
;
GBAPI GBRETURN EBCanPageUp(pcEBInputStruct pEBIS) 
;
GBAPI GBRETURN EBIsAssociateMode(pcEBInputStruct pEBIS) 
;
GBAPI GBRETURN EBIsInterpunctionMode(pcEBInputStruct pEBIS)
;
GBAPI GBRETURN EBGetStatus(pcEBInputStruct pEBIS, unsigned long * pStatus) 
;
GBAPI GBRETURN EBAssocAddHistoryISN(pEBInputStruct pEBIS, const unsigned short * isns, GBINT length)
;
GBAPI GBRETURN EBAssocSetHistoryISNs(pEBInputStruct pEBIS, const unsigned short * ISNs, GBINT length)
;
GBAPI GBRETURN EBInterpunctionFirstPageC(pEBInputStruct pEBIS)
;
GBAPI GBRETURN EBNextPageC(pEBInputStruct pEBIS)
;
GBAPI GBRETURN EBPrevPageC(pEBInputStruct pEBIS)
;
//research
GBAPI GBRETURN EBWord2Codes(pcEBInputStruct pEBIS, unsigned short isn, GBPACHAR pCodeBuffer) 
;


GBAPI GBRETURN EBShDataInit(GBLPVOID pData, unsigned long len, 
    unsigned short minCountToAdjustPhraseFreq, 
    unsigned short minCountToAdjustZiFreq,
    fpEBShDataUpdate updateFunction)
;

//自造词数据重置
GBAPI GBRETURN EBShDataReset(void)
;

GBAPI GBRETURN EBShAddPhrase(const unsigned short * isns, GBLPCACHAR syllable_together)
;

GBAPI GBRETURN EBHelperDeleteInputChar(pEBInputStruct pEBIS)
;

GBAPI GBRETURN EBHelperAddInputChar(pEBInputStruct pEBIS, GBINT chr)
;

//将一个输入字符串加入引擎里面
GBAPI GBRETURN EBHelperAddInputString(pEBInputStruct pEBIS, GBLPCACHAR str)
;

GBAPI GBRETURN EBHelperEBISRestart(pEBInputStruct pEBIS)
;

GBAPI GBRETURN EBHelperSelect(pEBInputStruct pEBIS, unsigned short index)
;

GBAPI GBRETURN EBHelperCancelSelect(pEBInputStruct pEBIS)
;


GBAPI GBRETURN EBEngineConfig(unsigned long config, int onOrOff)
;


GBAPI GBRETURN EBShShutown(void)
;
#endif
