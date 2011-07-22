/**************************************************************************
 *
 *   File Name:  e_homing.h
 *
 *   Purpose:   Contains the prototypes for all the functions of
 *              encoder homing.
 *
 **************************************************************************/

#define EHF_MASK 0x0008 /* Encoder Homing Frame pattern */

/* Function Prototypes */

INT16 encoder_homing_frame_test (INT16 input_frame[]);

void encoder_reset (void);

void reset_enc (void);
