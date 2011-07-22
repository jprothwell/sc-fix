#ifdef CFW_TCPIP_SUPPORT
#include <csw.h>


#ifndef APS_MSG_H__
#define APS_MSG_H__

#include "smtp.h"

//*****************
#define MAIL_SMTP 1
#define MAIL_POP3 2

//*****************
#define MEMP_NUM_NETCONN                4
/****************************************/
/*struct aps_msg_smtp
{
	CFW_TCPIP_SOCKET_ADDR smtpSvr_addr;
};
typedef struct aps_msg{
//	enum api_msg_type type;
	union{
		CFW_TCPIP_SOCKET_ADDR smtpSvr_addr;
		//others;
	}Msg_body;
}aps_msg_t;
*/
typedef union{
		CFW_TCPIP_SOCKET_ADDR smtpSvr_addr;
		//others;
}aps_Msg_body_t;

typedef struct{
		UINT8 NMstate;//current network, GPRS or GSM
		UINT8 sockflag[MEMP_NUM_NETCONN];
	
		smtp_contxt_t smtp_contxt;
		//others
		
		
}aps_contxt_t;

extern aps_contxt_t Aps_Contxt;
/***************************************/


#endif /* __LWIP_API_MSG_H__ */
#endif

