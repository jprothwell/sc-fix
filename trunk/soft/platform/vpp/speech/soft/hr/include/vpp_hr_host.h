#ifndef __HOSTGSM
#define __HOSTGSM

#include <stdio.h>
#include "vpp_hr_typedefs.h"
#include "hal_speech.h"
/*_________________________________________________________________________
 |                                                                         |
 |                           Function Prototypes                           |
 |_________________________________________________________________________|
*/
//void dePackEnc(UINT16* In, UINT16* Out);

void   fillBitAlloc(int iVoicing, int iR0, int *piVqIndeces,
                           int iSoftInterp, int *piLags,
                           int *piCodeWrdsA, int *piCodeWrdsB,
                           int *piGsp0s, INT16 swVadFlag,
                           INT16 swSP, INT16 *pswBAlloc);


#endif
