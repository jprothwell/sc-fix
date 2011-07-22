#include "api_msg.h"
#include <cswtype.h>


//
//convert API Plmn format to array: nOperatorId[6]
//[in] pLAI
//[out] nOperatorId
//
VOID PlmnApi2Array(UINT8 * pLAI, UINT8 *nOperatorId); 

//
//convert array: nOperatorId[6] to api plmn format  
//[in] nOperatorId
//[out] pLAI
//
VOID plmnArray2Api(UINT8 *nOperatorId, UINT8 * pLAI);


// convert API net status to AT status refer: AT+CREG
// [in] nStatusApi
// [in] bRoaming
// [out] pStatusAt
VOID NetStatusApi2At(UINT8 nStatusApi, BOOL bRoaming, UINT8* pStatusAt);

// convert API gprs status to AT status refer: AT+CGREG
// [in] nStatusApi
// [out] pStatusAt

VOID GprsStatusApi2At(UINT8 nStatusApi, BOOL bRoaming, UINT8* pStatusAt) ;


