#ifndef PA_DUMMY_H
#define PA_DUMMY_H

#include "cs_types.h"

#include "rfd_cfg.h"
#include "rfd_pa.h"
#include "rfd_defs.h"

//--------------------------------------------------------------------//
//                                                                    //
//                    GPIO and TCO Assignement                        //
//                                                                    //
//--------------------------------------------------------------------//

//--------------------------------------------------------------------//
//                                                                    //
//                            API for PAL                             //
//                                                                    //
//--------------------------------------------------------------------//

void rfd_PaTxOn(INT16 start, GSM_RFBAND_T band);
void rfd_PaTxOff(INT16 stop, GSM_RFBAND_T band);
void rfd_PaSetRamp(INT16 start, UINT8 rampId, RFD_PA_RAMPDIR_T rampDir);
void rfd_PaSetLowPower(UINT16 date);

#endif // PA_DUMMY_H
