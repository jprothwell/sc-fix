

#if (CFW_SS_DUMP_ENABLE==0) && (CFW_SS_SRV_ENABLE==1)

//#include "CFW_SS_data.h"

//Param:  pcIn: string;                                             
//        pcOut: BCD code                                           
//        iLength: string length                                    
VOID Str2BCD(UINT8 *pcIn, UINT8* pcOut, UINT8 iLength)
{
    UINT8 i, j;
    for (i = 0; i < (iLength + 1) / 2; i++)
    {
        pcOut[i] = (pcIn[2 * i] & 0xf) | 
            ((pcIn[2 * i + 1] & 0xf ) << 4);
    }
    for (j = 0; j < iLength; j++)
        if ( iLength & 1 )
        {
            pcOut[i - 1] |= 0xf0;
        }
        
}


VOID BCD2Stri(UINT8 *pcIn, UINT8 *pcOut, UINT8 iLength)
{
    UINT8 i = 0;
    
    if (!((*pcIn) & 0x80))
    {
        pcIn++;
        iLength--;
    }
    
    pcIn++;
    iLength--;
    
    for (i = 0; i < iLength; i++)
    {
        pcOut[2 * i]     = (pcIn[i] & 0x0F) + 0x30;
        pcOut[2 * i + 1] = ((pcIn[i] & 0xF0) >> 4) + 0x30;
    }
    
    if (pcOut[2 * i - 1] == 0x3F)
    {
        pcOut[2 * i - 1] = 0;
    }
    else
    {
        pcOut[2 * i] = 0;
    }
}

#endif // ENABLE 

