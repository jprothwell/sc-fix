#ifndef	PRV_MMI_H
#define	PRV_MMI_H

	//screen1
	void prv_MMIDisplayNewProvAlertScreen();
	
	//screen3
	void prv_MMIDisplayProvProfileDetails();
		
	//screen 4
	void prv_MMIDisplayDiscardConfirmationScreen();
		
	//screen 5, 7
	void prv_MMIDisplayDiscardSuccessScreen();
	
	
	//screen 6
	void prv_MMIDisplaySaveConfirmationScreen();
	
	
	//screen 8
	void prv_MMIDisplaySaveSuccessScreen();
	
	//screen 2
	void prv_MMIDisplayEnterPINScreen();

	void prv_MMIDisplayIDLEScreen();

	void prv_MMIDisplayProvFailScreen();
	void prv_MMIDisplayNetErrorPopUp();
	void prv_MMIUserPINLskHandler ();
	void prv_MMIUserPINLskhighlightHanlder (S32 index);
	void prv_MMIDisplayProvDiscardConfirmationScreen();
	void prv_MMIDisplayNewSettingsSummaryScreen();
	void prv_MMIDisplayContProvProfDetails();
	void prv_DisplayDownloadScreen();
	void prv_MMIDisplayInvalidSetting();
	void prv_MMIDisplayInvalidSettingsPopUp();
	void prv_MMIDisplayContDiscardConfirmationScreen();
	extern int giSelectProfileIndex;
	extern ST_CS_DATACONN_PROFILE_NAME gpszDataConnList[MAX_DATA_ACCOUNT_GPRS_LIMIT];
	extern ST_CS_PROXY_PROFILE_NAME gpszProxyProfileList[NVRAM_WAP_PROFILE_MAX];

	pBOOL prv_MMIGetProxyProfileItemData(S32 item_index, UI_string_type str_buff_p, PU8 *img_buff_p, U8 str_img_mask);
	void prv_MMISelectedProfileIndex(int iSelectIndex);
	void prv_MMIShowProxyProfileList();
	pBOOL prv_MMIGetDataConnItemData(S32 item_index, UI_string_type str_buff_p, PU8 *img_buff_p, U8 str_img_mask);
	void prv_MMIShowDataConnProfileList();
	void prv_MMIDisplayReplaceConfirmationScreen();

#endif //PRV_MMI_H
