#ifndef	PRV_BOOTSTRAP_H
#define	PRV_BOOTSTRAP_H



void prv_AddToProvList(ST_COMM_PUSH_IND *pstProvPushData);

void DeleteHeadProvList();
void prv_DeleteProfList();



void FreeMimHeader (ST_MIME_HEADERS *pstIndexHeader) ;

void GetMimHeader (ST_MIME_HEADERS **ppstHeadersList, JC_UINT8 *pucKey, JC_UINT32 uiKeyLen, 
				   JC_UINT8 *pucMsg, JC_UINT32 uiMsgLen) ;

JC_RETCODE HmacCallback (JC_UINT8  *pucKey,JC_UINT32 uiKeyLength, 
						   JC_UINT8  *pcMsg, JC_UINT32 uiMsgLength, 
						   JC_UINT8  **ppucData,JC_UINT32 *puiLength) ;


void prv_SaveSettings (void);
void prv_DiscardSettings (void);

JC_RETCODE prv_DeInitCommunicator (void);
JC_RETCODE prv_DeInitContext (void);
JC_RETCODE prv_HandleContinuousAndBootstrapProvisioning (void);
JC_RETCODE prv_FillPxLogicalInfo (ST_PROV_PXLOGICAL *pstPxLogic, 
										 ST_PROV_INFO_LIST **ppstProfList) ;

JC_RETCODE prv_FillApplicationInfo (ST_PROV_INFO_LIST **ppstProfList) ;
void prv_GetPxLogic (ST_PROV_VALUES *pstToProxy, ST_PROV_PXLOGICAL **ppstPxLogic);
void prv_GetNapDef (ST_PROV_VALUES *pstToNapid, ST_PROV_NAPDEF **ppstNapDef);
JC_RETCODE prv_FillNapDefInfo (ST_PROV_NAPDEF *pstNapDef, ST_PROV_INFO_LIST **ppstProfList);
void prv_FillPortServiceInfo (ST_PROV_PORT *pstPortService, ST_PROV_INFO_LIST *pstProfile);


JC_RETCODE prv_FillPxPhysicalInfo (ST_PROV_PXPHYSICAL *pstPxPhys, 
										  ST_PROV_INFO_LIST **ppstProfList);

void prv_ProcessProvInfo();

void prv_FormProfList();

void LogProfileList();
void prv_HandleOtaPushInd();
//void prv_ShowNextProfile();

void prv_DownloadContinousProvData ();
void prv_updateMissingProfileInfo (DATACONN_PROFILE *pstDestProf);

JC_RETCODE prv_CBCommProvIndication(
							void					*pvAppArg,
							E_COMM_INDICATION		eCommIndicationType,
							void					*pvIndication);


JC_RETCODE prv_HeaderIndication (ST_COMM_HEADER_IND	*pstHeaderIndication);
JC_RETCODE prv_FetchIndication (ST_COMM_FETCH_IND *pstFetchIndication);

JC_RETCODE prv_AppendHeader (const JC_UINT8 *pucHeader, const JC_UINT32 uiHeaderLen,
							const JC_BOOLEAN 		bIsHTTPHeader,
							const JC_UINT32			uiRequestID);

//ST_PROV_INFO *prv_GetProvInfo (JC_UINT32 uiRequestID, JC_BOOLEAN bNewNode);
JC_RETCODE prv_AppendBuffer (const JC_UINT8 *pucBuffer, const JC_UINT32 uiBufferLen,
							const JC_BOOLEAN 		bHasMoreData,
							const JC_UINT32			uiRequestID) ;

void prv_CheckAuthentication();
void prv_FormNewSettingsSummaryList();
void prv_FormContinousProfDeatilsBuff();
void prv_HandleDownloadContinousProvData();

void prv_HandleStopRequest();

void prv_ResetContinousProvdata();
JC_UINT8 prv_ApplicationExitHandler(void* p);

void prv_ContDownloadEndKeyHandler();
JC_RETCODE prv_CheckNETPINAuthentication(UINT8 * pIMSIasc , UINT8 OutLen);

void prv_SkipNextProxyProfile();

void prv_ReplaceProxyProfile();
void prv_ReplaceDataConnProfile();

void prv_ReplaceSettings (void);
void prv_CheckmaxProfiles();



#endif //PRV_BOOTSTRAP_H
