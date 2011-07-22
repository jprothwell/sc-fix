/*
 * Copyright 1992 by Jutta Degener and Carsten Bormann, Technische
 * Universitaet Berlin.  See the accompanying file "COPYRIGHT" for
 * details.  THERE IS ABSOLUTELY NO WARRANTY FOR THIS SOFTWARE.
 */

/* $Header: n:\\development/speak_freely/GSM/DECODE.C,v 1.1.1.1 1998/10/15 00:47:38 Administrator Exp $ */

#include <stdio.h>

#include        "vpp_fr_private.h"
//#include        "vpp_fr_gsm.h"
#include        "vpp_fr_proto.h"
#include        "vpp_fr_debug.h"
#include        "vpp_fr_profile_codes.h"

/*
 *	4.3 FIXED POINT IMPLEMENTATION OF THE RPE-LTP DECODER
 */

static void Postprocessing P2((S,s),
		struct gsm_state		* S,
		register INT16			* s)
{
		register int			k;
		register INT16			msr = S->msr;
		register INT32		ltmp;	/* for GSM_ADD */
		register INT16			tmp;

		VPP_FR_PROFILE_FUNCTION_ENTER(Postprocessing);

		for (k = 160; k--; s++) {
				tmp = (INT16) GSM_MULT_R( msr, 28180 );
				msr = (INT16) GSM_SATADD(*s, tmp); 		   /* Deemphasis			 */
				*s	= (INT16) (GSM_SATADD(msr, msr) & 0xFFF8);  /* Truncation & Upscaling */
		}
		S->msr = msr;

                VPP_FR_PROFILE_FUNCTION_EXIT(Postprocessing);

}

void Gsm_Decoder P8((S,LARcr, Ncr,bcr,Mcr,xmaxcr,xMcr,s),
		struct gsm_state		* S,

		INT16			* LARcr,		/* [0..7]				IN		*/

		INT16			* Ncr,			/* [0..3]				IN		*/
		INT16			* bcr,			/* [0..3]				IN		*/
		INT16			* Mcr,			/* [0..3]				IN		*/
		INT16			* xmaxcr,		/* [0..3]				IN		*/
		INT16			* xMcr, 		/* [0..13*4]			IN		*/

		INT16			* s)			/* [0..159] 			OUT 	*/
{
		int 			j, k;
		INT16			erp[40], wt[160];
		INT16			* drp = S->dp0 + 120;

		VPP_FR_PROFILE_FUNCTION_ENTER(Gsm_Decoder);

		for (j=0; j <= 3; j++, xmaxcr++, bcr++, Ncr++, Mcr++, xMcr += 13) {

				Gsm_RPE_Decoding( S, *xmaxcr, *Mcr, xMcr, erp );
				Gsm_Long_Term_Synthesis_Filtering( S, *Ncr, *bcr, erp, drp );

				for (k = 0; k <= 39; k++) wt[ j * 40 + k ] =  drp[ k ];
		}

		Gsm_Short_Term_Synthesis_Filter( S, LARcr, wt, s );
		Postprocessing(S, s);

		VPP_FR_PROFILE_FUNCTION_EXIT(Gsm_Decoder);
}
