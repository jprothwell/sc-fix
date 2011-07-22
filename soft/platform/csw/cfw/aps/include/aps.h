#ifdef CFW_TCPIP_SUPPORT

#ifndef __API_H__
#define __APS_H__
#include <csw.h>



#define APS_TCP 0x01
#define APS_PPP 0x02

//INT32 CFW_SendPppData(UINT8* pData, UINT16 nDataSize);
typedef struct {

  BOOL  PPP_Att;
  BOOL  PPP_Act;
  UINT8 PPP_Cid;
  UINT8 PPP_Uti;
}ppp_conf_t;
extern ppp_conf_t ppp_Gprs_state;



#endif// __APS_H__

#endif
