#ifndef _DEVNET_H
#define _DEVNET_H
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// !!! public declare start !!!


// !!! public declare end !!!
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#ifdef _DEV_NET_
// !!! private declare start !!!

//#include  "app2soc_struct.h"
#include  "soc_api.h"

/*typedef struct
{
 LOCAL_PARA_HDR
 kal_int8   socket_id;
 kal_uint8 event_type;
 kal_bool  result;
 kal_int8   error_cause;
 kal_int32  detail_cause;
} mmi_socket_notify_ind_struct;*/

typedef struct _NET_Data NET;
struct _NET_Data
{
	INET Vtbl;

	WEPAppData * pWep;

	kal_int8   sockfd;
	kal_uint16 port;
	
	NET_STATUS status;	// current net status

	PFNNETCALLBACK connectCB;
	void * pConnectCBData;
	PFNNETCALLBACK sendCB;
	void * pSendCBData;
	PFNNETCALLBACK readCB;
	void * pReadCBData;

	int32_t numberOfBytesSent;

	bool_t socketConnected;
	
	kal_uint32 currentDataAcountID;

#ifdef SUPPORT_PROXY
	kal_bool newRead;//means can begin a new read action
	char_t currentRequestUrl[MAX_DEFAULT_SERVER_URL_LEN];

#endif
};


#endif	


#endif 

