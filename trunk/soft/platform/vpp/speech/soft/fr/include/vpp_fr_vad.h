#ifndef VAD_H
#define VAD_H

#define LTHRESH 2
#define NTHRESH 4

struct _fp
{
    INT16 e;          /* exponent */
    INT16 m;          /* mantissa */
};

typedef struct _fp Pfloat;


INT16 vad_computation (
	INT32 *L_ACF
//	INT16 scalvad,
//	INT16 * sof
//	INT16 ptch
);

void periodicity_update ( INT16 *Nc );//, INT16 *ptch);

INT16 Periodicity_detection(INT16 oldlagcount, INT16 veryoldlagcount);

void vad_init(void);

void energy_computation (
    INT32 L_ACF[],
//    INT16 scalvad,
    INT16 rvad[],
    Pfloat * acf0,
    Pfloat * pvad
);

void acf_averaging (
//    INT16  r_h[],
//    INT16  r_l[],
//    INT16  scal_acf,
	INT32 L_ACF[],
    INT32 L_av0[],
    INT32 L_av1[]
);

void predictor_values (
    INT32 L_av1[],
    INT16  rav1[],
    INT16  *normrav1
);

void schur_recursion (
    INT32 L_av1[],
    INT16  vpar[]
);

void step_up (
    INT16  np,
    INT16  vpar[],
    INT16  aav1[]
);

void compute_rav1 (
    INT16  aav1[],
    INT16  rav1[],
    INT16  *normrav1
);

INT16  spectral_comparison (
    INT16  rav1[],
    INT16  normrav1,
    INT32 L_av0[]
);

void threshold_adaptation (
    INT16  stat,
    INT16  ptch,
    INT16  tone,
    INT16  rav1[],
    INT16  normrav1,
    Pfloat pvad,
    Pfloat acf0,
    INT16  rvad[],
    INT16  *normrvad,
    Pfloat * thvad
);

void tone_detection (
//    INT16  rc[],
	INT16  *sig_of,
    INT16  *tone
);

INT16  vad_decision (
    Pfloat pvad,
    Pfloat thvad
);

INT16  vad_hangover (
    INT16  vvad
);

void periodicity_update (
    INT16  lags[]
//    INT16  *ptch
);



#endif


