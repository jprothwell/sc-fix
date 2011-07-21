 
#ifndef __JWAP_HTTP_DOWNLOAD
#define __JWAP_HTTP_DOWNLOAD

typedef struct http_download_info
{
	JC_UINT32				uiRequestID ;
	JC_CHAR					*pszURL ;
	JC_BOOLEAN				bHasUserConfirmed ;
	JC_BOOLEAN				bHasUserCanceled ;
	JC_CHAR					*pmFinalFileName ;
	JC_CHAR					*pmTempFileName ;
	JC_CHAR					*pmContentType ;
	JC_CHAR					*pmContentSize ;
	JC_UINT32				uiTotalSize ;
	JC_UINT32				uiCurrSize ;
	JC_RETCODE				rCode ;
	JC_BOOLEAN				bHasEndPageBeenCalled ;
} ST_HTTP_DOWNLOAD_INFO ;

JC_RETCODE  jWap_HandleHTTPDownload (ST_BE_URL_IND *pstBeURLInd) ;
JC_RETCODE HandleHTTPDLHeaderIndication(ST_BE_URL_IND *pstBeURLIn) ;
JC_RETCODE HandleHTTPDLBufferIndication(ST_BE_URL_IND *pstBeURLInd) ;
JC_RETCODE HandleErrIndication() ;
void GetUserConfirmation () ;
void ShowUserConfirmation () ;
void HandleYes() ;
void HandleNo() ;
void jWap_HTTPDOWNLOAD_saveName() ;
void HandleProgressCancel() ;
JC_BOOLEAN jWap_isValidContent (ST_BE_URL_IND *pstBeURLInd) ;
void jWap_freeDownloadInfo () ;
JC_BOOLEAN jwap_appIsOmaDD (JC_INT8 *pcContentType) ;
void jWap_getFileNameFromUrl (JC_CHAR *pmDestFileName);


#endif

