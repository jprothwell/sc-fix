
#include <cfw_config_prv.h>

#if (CFW_NW_DUMP_ENABLE==0) && (CFW_NW_SRV_ENABLE==1)

#include "cfw_nw_data.h"
#include "cfw_nw_tool.h"

#include <ts.h>
//
//convert API Plmn format to array: nOperatorId[6]
//[in] pLAI
//[out] nOperatorId
//
VOID PlmnApi2Array(UINT8 * pLAI, UINT8 *nOperatorId)
{
    nOperatorId[0] = pLAI[0] & 0x0F;
    nOperatorId[1] = (pLAI[0] & 0xF0) >> 4;
    nOperatorId[2] = pLAI[1] & 0x0F;
    nOperatorId[3] = pLAI[2] & 0x0F;
    nOperatorId[4] = (pLAI[2] & 0xF0) >> 4;
    nOperatorId[5] = (pLAI[1] & 0xF0) >> 4;
}

//
//convert array: nOperatorId[6] to api plmn format  
//[in] nOperatorId
//[out] pLAI
//
VOID plmnArray2Api(UINT8 *nOperatorId, UINT8 * pLAI)
{
    pLAI[0] = (nOperatorId[0] & 0x0F) + ((nOperatorId[1] << 4) & 0xF0);
    pLAI[1] = (nOperatorId[2] & 0x0F) + ((nOperatorId[5] << 4) & 0xF0);
    pLAI[2] = (nOperatorId[3] & 0x0F) + ((nOperatorId[4] << 4) & 0xF0); 
}

// convert API net status to AT status refer: AT+CREG
// [in] bStackStop: the stack is stop or not
// [in] nStatusApi: status from api
// [in] bRoaming : bRoaming from api
// [out] pStatusAt : the net status refer: AT+CREG
VOID NetStatusApi2At(UINT8 nStatusApi, BOOL bRoaming, UINT8* pStatusAt) 
{
    
    if (nStatusApi == API_NW_NO_SVCE)    // no service
    {
        *pStatusAt = CFW_NW_STATUS_NOTREGISTERED_SEARCHING;
    }
    else if (nStatusApi == API_NW_LTD_SVCE)  // limited service
    {
        *pStatusAt = CFW_NW_STATUS_REGISTRATION_DENIED;
    } 
    else if (nStatusApi == API_NW_FULL_SVCE)  // full service
    {
        if (bRoaming)   // roaming
        {
            *pStatusAt = CFW_NW_STATUS_REGISTERED_ROAMING;
        }
        else // home
        {
            *pStatusAt = CFW_NW_STATUS_REGISTERED_HOME;
        }
    } 
    else
    {
        *pStatusAt = CFW_NW_STATUS_UNKNOW;
    } 
}

//// have not finished
VOID GprsStatusApi2At(UINT8 nStatusApi, BOOL bRoaming, UINT8* pStatusAt) 
{
    if (nStatusApi == API_NW_NO_SVCE)    // no service
    {
        *pStatusAt = CFW_NW_STATUS_NOTREGISTERED_NOTSEARCHING;
    }
    else if (nStatusApi == API_NW_FULL_SVCE)  // full service
    {
        
        if (bRoaming)   // roaming
        {
            *pStatusAt = CFW_NW_STATUS_REGISTERED_ROAMING;
        }
        else // home
        {
            *pStatusAt = CFW_NW_STATUS_REGISTERED_HOME;
        }
    }
    else if (nStatusApi == API_NW_REG_PENDING)   //Outcome of registn not known yet; PS only
    {
        *pStatusAt = CFW_NW_STATUS_NOTREGISTERED_SEARCHING;
    }
    else
    {
        *pStatusAt = CFW_NW_STATUS_UNKNOW;
    }
}


#endif // ENABLE 
