#ifndef MAP_EXPORT_H
#define MAP_EXPORT_H

/* This file defines the addresses of exported variables.
   It's created by voc_map automatically.*/

#define VPP_SPEECH_DEC_IN_STRUCT                	(0 + RAM_X_BEGIN_ADDR)
#define VPP_SPEECH_ENC_OUT_STRUCT               	(26 + RAM_X_BEGIN_ADDR)
#define VPP_SPEECH_ENC_IN_STRUCT                	(48 + RAM_X_BEGIN_ADDR)
#define VPP_SPEECH_DEC_OUT_STRUCT               	(368 + RAM_X_BEGIN_ADDR)
#define GLOBAL_NOTCH_FILTER_COEF_ADDR           	(7000 + RAM_X_BEGIN_ADDR)
#define VPP_SPEECH_INIT_STRUCT                  	(0 + RAM_Y_BEGIN_ADDR)
#define VPP_SPEECH_AUDIO_CFG_STRUCT             	(56 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_MIC_DIGITAL_GAIN_ADDR            	(386 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_MIC_DIGITAL_MAXVALUE_ADDR        	(387 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_RECEIVER_DIGITAL_GAIN_ADDR       	(388 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_RECEIVER_DIGITAL_MAXVALUE_ADDR   	(389 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_OUTPUTENERGY_ADDR                	(390 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_INPUTENERGY_ADDR                 	(392 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_SPEECH_RAMX_BACKUP_ADDR          	(554 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_SPEECH_CODE_BACKUP_ADDR          	(556 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_AEC_RAMX_BACKUP_ADDR             	(558 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_AEC_CODE_ADDR                    	(560 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_AEC_CONSTX_ADDR                  	(562 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_RX_ADDR                          	(564 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_TX_ADDR                          	(566 + RAM_Y_BEGIN_ADDR)
#define m_AECProcPara_ADDR                      	(576 + RAM_Y_BEGIN_ADDR)
#define MORPHVOICE_ENABLE_FLAG_ADDR             	(610 + RAM_Y_BEGIN_ADDR)
#define PITCH_SHIFT_ADDR                        	(611 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_MORPH_CODE_ADDR                  	(612 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_MORPH_CONSTY_ADDR                	(614 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_SPEECH_RAMY_BACKUP_ADDR          	(616 + RAM_Y_BEGIN_ADDR)
#define HIGH_PASS_FILTER_ENABLE_FLAG_ADDR       	(748 + RAM_Y_BEGIN_ADDR)
#define NOTCH_FILTER_ENABLE_FLAG_ADDR           	(749 + RAM_Y_BEGIN_ADDR)
#define NOISE_SUPPRESSER_ENABLE_FLAG_ADDR       	(750 + RAM_Y_BEGIN_ADDR)
#define NOISE_SUPPRESSER_WITHOUT_SPEECH_ENABLE_FLAG_ADDR	(751 + RAM_Y_BEGIN_ADDR)

#endif
