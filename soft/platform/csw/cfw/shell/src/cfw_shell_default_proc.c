#include <cswtype.h>
#include <cfw.h>
#include <sul.h>
#include <cfw_prv.h>
#include <rfm.h>
#include <ts.h>

#if 0 
PRIVATE CFW_SMS_PARAMETER g_Info;
PRIVATE UINT8 g_IMSI[16];

UINT32 SimInitSmsParamProc(CFW_EVENT* pEvent)
{    
    UINT32 nEvId   = pEvent->nEventId;
	UINT32 nReturn = 0;

    if(nEvId == EV_CFW_SRV_INIT_IND && pEvent->nType==1)
    {
        CFW_CfgGetDefaultSmsParam(&g_Info);

		//
		//Get the SCA
        // 
		// TODO... need to check the default index in SIM card for SCA.
        // 
        nReturn = CFW_SimGetSmsParameters(0, APP_UTI_SHELL); 

		if(nReturn != ERR_SUCCESS)
		{
			return(nReturn);
		}
    }
    if(nEvId == EV_CFW_SIM_GET_SMS_PARAMETERS_RSP)
    {
        //g_Info.;

		if(pEvent->nType == 0x00) 
		{
			//
			//Get the SMS SCA number, 12 bytes.
			//
			SUL_MemCopy8( g_Info.nNumber,(UINT8 *)pEvent->nParam1, SIZEOF(g_Info.nNumber) );
		
			//
			//Get the MR
			//
			nReturn = CFW_SimGetMR( APP_UTI_SHELL );
			
			if(nReturn != ERR_SUCCESS)
			{
				return(nReturn);
			}
		}
		else
		{
			return 1;
		}
    }
    else if(nEvId == EV_CFW_SIM_GET_MR_RSP) 
    {
		if(pEvent->nType == 0x00)
        {
            g_Info.mr = (UINT8)(pEvent->nParam1);
            CFW_SimGetProviderId( APP_UTI_SHELL );
        }
        else
        {
            CSW_TRACE(0, TSTXT("EV_CFW_SIM_GET_MR_RSP Error! \n"));
        }
    }
    else if(nEvId == EV_CFW_SIM_GET_PROVIDER_ID_RSP) 
    {
        //g_Info.
		if(pEvent->nType == 0x00)
		{
            //SCA_PARAM sca;
            SUL_MemCopy8( g_IMSI, (UINT8*)(pEvent->nParam1), SIZEOF(g_IMSI));
			CFW_CfgSetDefaultSmsParam(&g_Info);
            //sca.nBCD[21] = '\0';
            //SUL_GetSCAStruct(g_Info.nNumber ,&sca);
            CSW_TRACE(0, TSTXT("CFW SIM INIT Complete! \n"));
            //CSW_TRACE(0, TSTXT("CFW SIM INIT Complete: %s, %d, %x \n"), sca.nBCD, sca.nLength, sca.nType);           
			return 0;
		}
		else
		{
			return 1;
		}
    }
    else if(nEvId == EV_CFW_SIM_GET_PROVIDER_ID_RSP) 
    {

    }

    return 1;
}

#endif 
