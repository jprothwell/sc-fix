#ifndef	MMS_RECEIVE_H
#define	MMS_RECEIVE_H

JC_RETCODE jmms_SendMms (JC_BOOLEAN bIsEdit) ;
JC_RETCODE jMMS_HandleFinalDownloadState (CONTENT_DATA *pMMSData, JC_UINT32 uiMessageID) ;
JC_RETCODE MMS_SaveMessage (STORE_HANDLE hStoreHandle, JC_INT8 *pcFolder, 
				MESSAGE_HEADERS *pstMsgHdrs, CONTENT_DATA *pstMsgContent, 
				JC_UINT32 *pulMessagID, JC_UINT32 ulExtraBytes);

//void SendNotificationResp(const JC_INT8 *pTransactionID, const JC_INT8 *pVersion, EMessageStatus eMessageStatus);
JC_RETCODE jMMS_HandlePushMsg (void *pvAppData, ST_COMM_PUSH_IND *pstPushData);
void jMMS_SendSelfEvent (E_JMMS_APP_EVENT eEventToSend, ST_JMMS_EVENT_PROCESS_NEXT_NODE_DATA *pstEventInfo) ;




#endif //MMS_RECEIVE_H

