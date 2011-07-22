#ifndef __VAD
#define __VAD

#include "vpp_hr_typedefs.h"


/*_________________________________________________________________________
 |                                                                         |
 |                            Function Prototypes                          |
 |_________________________________________________________________________|
*/

void   vad_reset(void);

void   vad_algorithm
       (
               INT32 pL_acf[9],
               INT16 swScaleAcf,
               INT16 pswRc[4],
               INT16 swPtch,
               INT16 *pswVadFlag
);

void   energy_computation
       (
               INT32 pL_acf[],
               INT16 swScaleAcf,
               INT16 pswRvadm[],
               INT16 swNormRvadm,
               INT16 *pswM_pvad,
               INT16 *pswE_pvad,
               INT16 *pswM_acf0,
               INT16 *pswE_acf0
);


void   average_acf
       (
               INT32 pL_acf[],
               INT16 swScaleAcf,
               INT32 pL_av0[],
               INT32 pL_av1[]
);

void   predictor_values
       (
               INT32 pL_av1[],
               INT16 pswRav1[],
               INT16 *pswNormRav1
);

void   schur_recursion
       (
               INT32 pL_av1[],
               INT16 pswVpar[]
);

void   step_up
       (
               INT16 swNp,
               INT16 pswVpar[],
               INT16 pswAav1[]
);

void   compute_rav1
       (
               INT16 pswAav1[],
               INT16 pswRav1[],
               INT16 *pswNormRav1
);

void   spectral_comparison
       (
               INT16 pswRav1[],
               INT16 swNormRav1,
               INT32 pL_av0[],
               INT16 *pswStat
);

void   tone_detection
       (
               INT16 pswRc[4],
               INT16 *pswTone
);


void   threshold_adaptation
       (
               INT16 swStat,
               INT16 swPtch,
               INT16 swTone,
               INT16 pswRav1[],
               INT16 swNormRav1,
               INT16 swM_pvad,
               INT16 swE_pvad,
               INT16 swM_acf0,
               INT16 swE_acf0,
               INT16 pswRvadm[],
               INT16 *pswNormRvad,
               INT16 *pswM_thvad,
               INT16 *pswE_thvad
);

void   vad_decision
       (
               INT16 swM_pvad,
               INT16 swE_pvad,
               INT16 swM_thvadm,
               INT16 swE_thvadm,
               INT16 *pswVvad
);

void   vad_hangover
       (
               INT16 swVvad,
               INT16 *pswVadFlag
);

void   periodicity_update
       (
               INT16 pswLags[4],
               INT16 *pswPtch
);

#endif
