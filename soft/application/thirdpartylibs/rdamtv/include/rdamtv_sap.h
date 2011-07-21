#ifndef __RDAMTVSAP_H__
#define __RDAMTVSAP_H__

    // -------------------------------------------------------------
    // message code.
    // -------------------------------------------------------------
	MSG_ID_RDAMTV_POWER_UP_REQ       = RDAMTV_MSG_CODE_BEGIN,
	MSG_ID_RDAMTV_CODE_BEGIN = MSG_ID_RDAMTV_POWER_UP_REQ,
	
	MSG_ID_RDAMTV_INIT_REQ,
	MSG_ID_RDAMTV_SCAN_CHANNEL_REQ,
	MSG_ID_RDAMTV_SET_CHANNEL_REQ,	
	MSG_ID_RDAMTV_SET_PARAM_REQ,
	MSG_ID_RDAMTV_GET_PARAM_REQ,
	MSG_ID_RDAMTV_EXIT_REQ,
	
	MSG_ID_RDAMTV_POWER_DOWN_REQ,
	MSG_ID_RDAMTV_CODE_END = MSG_ID_RDAMTV_POWER_DOWN_REQ,
	
    // -------------------------------------------------------------
    // message response.
    // -------------------------------------------------------------
	MSG_ID_RDAMTV_POWER_UP_RSP,
	
	MSG_ID_RDAMTV_INIT_RSP,
	MSG_ID_RDAMTV_SCAN_CHANNEL_RSP, 
	MSG_ID_RDAMTV_SET_CHANNEL_RSP,
	MSG_ID_RDAMTV_SET_PARAM_RSP,
	MSG_ID_RDAMTV_GET_PARAM_RSP,
	MSG_ID_RDAMTV_EXIT_RSP,
	
	MSG_ID_RDAMTV_POWER_DOWN_RSP,
#endif