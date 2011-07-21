/*****************************************************************************
 *
 * Filename:
 * ---------
 *  Myanmar.h
 *
 * Project:
 * -------- 
 *  
 *
 * Description:
 * ------------
 *  Myanmar header file
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef MYANMAR_H
#define MYANMAR_H

#include "mmi_features.h"
#include "gui_data_types.h"
#include "mmi_data_types.h"


/***************************************************************************** 
* Define
*****************************************************************************/
#define IRE_MYANMAR_LANG_START_RANGE      0x0FFF
#define IRE_MYANMAR_LANG_END_RANGE        0x109F
#define IRE_MYANMAR_PROPRIETRY_START_RANGE   0xE7FF
#define IRE_MYANMAR_PROPRIETRY_END_RANGE  0xE8C2

/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum
{   
	  /* DEVANAGARI SIGNS */
		MYANMAR_DEVANAGARI_S_DANDA		= 0x0964,
		MYANMAR_DEVANAGARI_S_DDANDA		= 0x0965,
			
    MYANMAR_C_KA = 0x1000,
    MYANMAR_C_KHA = 0x1001,
    MYANMAR_C_GA = 0x1002,
    MYANMAR_C_GHA = 0x1003,
    MYANMAR_C_NGA = 0x1004,
    MYANMAR_C_CA = 0x1005,
    MYANMAR_C_CHA = 0x1006,
    MYANMAR_C_JA = 0x1007,
    MYANMAR_C_JHA = 0x1008,
    MYANMAR_C_NYA = 0x1009,
    MYANMAR_C_NNYA = 0x100A,
    MYANMAR_C_TTA = 0x100B,
    MYANMAR_C_TTHA = 0x100C,
    MYANMAR_C_DDA = 0x100D,
    MYANMAR_C_DDHA = 0x100E,
    MYANMAR_C_NNA = 0x100F,
    MYANMAR_C_TA = 0x1010,
    MYANMAR_C_THA = 0x1011,
    MYANMAR_C_DA = 0x1012,
    MYANMAR_C_DHA = 0x1013,
    MYANMAR_C_NA = 0x1014,
    MYANMAR_C_PA = 0x1015,
    MYANMAR_C_PHA = 0x1016,
    MYANMAR_C_BA = 0x1017,
    MYANMAR_C_BHA = 0x1018,
    MYANMAR_C_MA = 0x1019,
    MYANMAR_C_YA = 0x101A,
    MYANMAR_C_RA = 0x101B,
    MYANMAR_C_LA = 0x101C,
    MYANMAR_C_WA = 0x101D,
    MYANMAR_C_SA = 0x101E,
    MYANMAR_C_HA = 0x101F,	
    MYANMAR_C_LLA = 0x1020,	
	
    MYANMAR_IV_A = 0x1021,
    MYANMAR_IV_SA = 0x1022,
    MYANMAR_IV_I = 0x1023,
    MYANMAR_IV_II = 0x1024,
    MYANMAR_IV_U = 0x1025,
    MYANMAR_IV_UU = 0x1026,
    MYANMAR_IV_E = 0x1027,
    MYANMAR_IV_ME = 0x1028,
    MYANMAR_IV_O = 0x1029,
    MYANMAR_IV_AU = 0x102A,	

		MYANMAR_DV_TAA = 0x102B,
    MYANMAR_DV_AA = 0x102C,
    MYANMAR_DV_I = 0x102D,
    MYANMAR_DV_II = 0x102E,
    MYANMAR_DV_U = 0x102F,
    MYANMAR_DV_UU = 0x1030,
    MYANMAR_DV_E = 0x1031,
    MYANMAR_DV_AI = 0x1032,
    MYANMAR_DV_MON_II = 0x1033,
    MYANMAR_DV_MON_O = 0x1034,
    MYANMAR_DV_E_ABOVE = 0x1035,

	
    MYANMAR_S_ANUSVARA = 0x1036,
    MYANMAR_S_DOT = 0x1037,
    MYANMAR_S_VISARGA = 0x1038,
    MYANMAR_S_VIRAMA = 0x1039,
    MYANMAR_S_ASAT = 0x103A,
    
    MYANMAR_CS_YA = 0x103B,
    MYANMAR_CS_RA = 0x103C,
    MYANMAR_CS_WA = 0x103D,
    MYANMAR_CS_HA = 0x103E,
    
    MYANMAR_C_GREAT_SA = 0x103F,

    MYANMAR_D_ZERO = 0x1040, /* MYANMAR digital 0 */
    MYANMAR_D_ONE = 0x1041, /* MYANMAR digital 1 */
    MYANMAR_D_TWO = 0x1042, /* MYANMAR digital 2 */
    MYANMAR_D_THREE = 0x1043, /* MYANMAR digital 3 */
    MYANMAR_D_FOUR = 0x1044, /* MYANMAR digital 4 */
    MYANMAR_D_FIVE = 0x1045, /* MYANMAR digital 5 */
    MYANMAR_D_SIX = 0x1046, /* MYANMAR digital 6 */
    MYANMAR_D_SEVEN = 0x1047, /* MYANMAR digital 7 */
    MYANMAR_D_EIGHT = 0x1048, /* MYANMAR digital 8 */
    MYANMAR_D_NINE = 0x1049, /* MYANMAR digital 9 */

    MYANMAR_S_LITTLE_SECTION = 0x104A,
    MYANMAR_S_SECTION = 0x104B,
    
    MYANMAR_S_LOCATIVE = 0x104C,
    MYANMAR_S_COMPLETED = 0x104D,
    MYANMAR_S_AFOREMENTONED = 0x104E,
    MYANMAR_S_GENITIVE = 0x104F,
    
    MYANMAR_L_SHA = 0x1050,
    MYANMAR_L_SSA = 0x1051,
    MYANMAR_L_VR = 0x1052,
    MYANMAR_L_VRR = 0x1053,
    MYANMAR_L_VL = 0x1054,
    MYANMAR_L_VLL = 0x1055,
    MYANMAR_VS_VR = 0x1056,
    MYANMAR_VS_VRR = 0x1057,
    MYANMAR_VS_VL = 0x1058,
    MYANMAR_VS_VLL = 0x1059,
    
    MYANMAR_L_M_NGA = 0x105A,
    MYANMAR_L_M_JHA = 0x105B,
    MYANMAR_L_M_BBA = 0x105C,
    MYANMAR_L_M_BBE = 0x105D,

    MYANMAR_CS_MON_MEDIALNA = 0x105E,
    MYANMAR_CS_MON_MEDIALMA = 0x105F,
    MYANMAR_CS_MON_MEDIALLA = 0x1060,
    
    MYANMAR_LS_KAREN_SHA = 0x1061,
    MYANMAR_VS_SGAW_KAREN_EU = 0x1062,
    MYANMAR_TM_SGAW_KAREN_HATHI = 0x1063,
    MYANMAR_TM_SGAW_KAREN_KE_PHO = 0x1064,

    MYANMAR_L_WPK_THA = 0x1065,
    MYANMAR_L_WPK_PWA = 0x1066,
    
    MYANMAR_VS_WPK_EU = 0x1067,
    MYANMAR_VS_WPK_UE = 0x1068, 
    
    MYANMAR_S_WPK_TONE1 = 0x1069, 
    MYANMAR_S_WPK_TONE2 = 0x106A, 
    MYANMAR_S_WPK_TONE3 = 0x106B, 
    MYANMAR_S_WPK_TONE4 = 0x106C, 
    MYANMAR_S_WPK_TONE5 = 0x106D, 
    MYANMAR_L_EPK_NNA = 0x106E, 
    MYANMAR_L_EPK_YWA = 0x106F, 
    
    MYANMAR_L_EPK_GHWA = 0x1070,
    
    MYANMAR_VS_GEBA_KAREN1 = 0x1071, 
    
    MYANMAR_VSK_OE = 0x1072, 
    MYANMAR_VSK_U = 0x1073, 
    MYANMAR_VSK_EE = 0x1074, 
    
    MYANMAR_LS_KA = 0x1075,
    MYANMAR_LS_KHA = 0x1076,
    MYANMAR_LS_GA = 0x1077,
    MYANMAR_LS_CA = 0x1078,
    MYANMAR_LS_ZA = 0x1079,
    MYANMAR_LS_NYA = 0x107A,
    MYANMAR_LS_DA = 0x107B,
    MYANMAR_LS_NA = 0x107C,
    MYANMAR_LS_PHA = 0x107D,
    MYANMAR_LS_FA = 0x107E,
    MYANMAR_LS_BA = 0x107F,
    MYANMAR_LS_THA = 0x1080,
    MYANMAR_LS_HA = 0x1081,
  
    MYANMAR_CSSM_WA = 0x1082, 
    MYANMAR_VSS_AA = 0x1083, 
    MYANMAR_VSS_E = 0x1084, 
    MYANMAR_VSS_EA = 0x1085, 
    MYANMAR_VSS_FY = 0x1086, 
    MYANMAR_SS_TONE2 = 0x1087, 
    MYANMAR_SS_TONE3 = 0x1088, 
    MYANMAR_SS_TONE5 = 0x1089, 
    MYANMAR_SS_TONE6 = 0x108A, 
    MYANMAR_SSC_TONE2 = 0x108B, 
    MYANMAR_SSC_TONE3 = 0x108C, 
    
    MYANMAR_SSC_EMPHATICTONE = 0x108D, 
    
    MYANMAR_LRP_FA = 0x108E, 
    MYANMAR_SRP_TONE5 = 0x108F, 
    
    MYANMAR_SD_ZERO = 0x1090, /* MYANMAR digital 0 */
    MYANMAR_SD_ONE = 0x1091, /* MYANMAR digital 1 */
    MYANMAR_SD_TWO = 0x1092, /* MYANMAR digital 2 */
    MYANMAR_SD_THREE = 0x1093, /* MYANMAR digital 3 */
    MYANMAR_SD_FOUR = 0x1094, /* MYANMAR digital 4 */
    MYANMAR_SD_FIVE = 0x1095, /* MYANMAR digital 5 */
    MYANMAR_SD_SIX = 0x1096, /* MYANMAR digital 6 */
    MYANMAR_SD_SEVEN = 0x1097, /* MYANMAR digital 7 */
    MYANMAR_SD_EIGHT = 0x1098, /* MYANMAR digital 8 */
    MYANMAR_SD_NINE = 0x1099, /* MYANMAR digital 9 */
    
    MYANMAR_SK_TONE1 = 0x109A, /* sign */
    MYANMAR_SK_TONE3 = 0x109B,
    
    MYANMAR_VSA_A = 0x109C,
    MYANMAR_VSA_AI = 0x109D,
    MYANMAR_SS_ONE = 0x109E,
    MYANMAR_SS_EXCLAMATION = 0x109F
    
}Myanmar_characters_enum;

typedef enum
{  
MYANMAR_LIG_1 = 0xE800,
MYANMAR_LIG_2 = 0xE801,
MYANMAR_LIG_3 = 0xE802,
MYANMAR_LIG_4 = 0xE803,
MYANMAR_LIG_5 = 0xE804,
MYANMAR_LIG_6 = 0xE805,
MYANMAR_LIG_7 = 0xE806,
MYANMAR_LIG_8 = 0xE807,
MYANMAR_LIG_9 = 0xE808,
MYANMAR_LIG_10 = 0xE809,
MYANMAR_LIG_11 = 0xE80A,
MYANMAR_LIG_12 = 0xE80B,
MYANMAR_LIG_13 = 0xE80C,
MYANMAR_LIG_14 = 0xE80D,
MYANMAR_LIG_15 = 0xE80E,
MYANMAR_LIG_16 = 0xE80F,
MYANMAR_LIG_17 = 0xE810,
MYANMAR_LIG_18 = 0xE811,
MYANMAR_LIG_19 = 0xE812,
MYANMAR_LIG_20 = 0xE813,
MYANMAR_LIG_21 = 0xE814,
MYANMAR_LIG_22 = 0xE815,
MYANMAR_LIG_23 = 0xE816,
MYANMAR_LIG_24 = 0xE817,
MYANMAR_LIG_25 = 0xE818,
MYANMAR_LIG_26 = 0xE819,
MYANMAR_LIG_27 = 0xE81A,
MYANMAR_LIG_28 = 0xE81B,
MYANMAR_LIG_29 = 0xE81C,
MYANMAR_LIG_30 = 0xE81D,
MYANMAR_LIG_31 = 0xE81E,
MYANMAR_LIG_32 = 0xE81F,
MYANMAR_LIG_33 = 0xE820,
MYANMAR_LIG_34 = 0xE821,
MYANMAR_LIG_35 = 0xE822,
MYANMAR_LIG_36 = 0xE823,
MYANMAR_LIG_37 = 0xE824,
MYANMAR_LIG_38 = 0xE825,
MYANMAR_LIG_39 = 0xE826,
MYANMAR_LIG_40 = 0xE827,
MYANMAR_LIG_41 = 0xE828,
MYANMAR_LIG_42 = 0xE829,
MYANMAR_LIG_43 = 0xE82A,
MYANMAR_LIG_44 = 0xE82B,
MYANMAR_LIG_45 = 0xE82C,
MYANMAR_LIG_46 = 0xE82D,
MYANMAR_LIG_47 = 0xE82E,
MYANMAR_LIG_48 = 0xE82F,
MYANMAR_LIG_49 = 0xE830,
MYANMAR_LIG_50 = 0xE831,
MYANMAR_LIG_51 = 0xE832,
MYANMAR_LIG_52 = 0xE833,
MYANMAR_LIG_53 = 0xE834,
MYANMAR_LIG_54 = 0xE835,
MYANMAR_LIG_55 = 0xE836,
MYANMAR_LIG_56 = 0xE837,
MYANMAR_LIG_57 = 0xE838,
MYANMAR_LIG_58 = 0xE839,
MYANMAR_LIG_59 = 0xE83A,
MYANMAR_LIG_60 = 0xE83B,
MYANMAR_LIG_61 = 0xE83C,
MYANMAR_LIG_62 = 0xE83D,
MYANMAR_LIG_63 = 0xE83E,
MYANMAR_LIG_64 = 0xE83F,
MYANMAR_LIG_65 = 0xE840,
MYANMAR_LIG_66 = 0xE841,
MYANMAR_LIG_67 = 0xE842,
MYANMAR_LIG_68 = 0xE843,
MYANMAR_LIG_69 = 0xE844,
MYANMAR_LIG_70 = 0xE845,
MYANMAR_LIG_71 = 0xE846,
MYANMAR_LIG_72 = 0xE847,
MYANMAR_LIG_73 = 0xE848,
MYANMAR_LIG_74 = 0xE849,
MYANMAR_LIG_75 = 0xE84A,
MYANMAR_LIG_76 = 0xE84B,
MYANMAR_LIG_77 = 0xE84C,
MYANMAR_LIG_78 = 0xE84D,
MYANMAR_LIG_79 = 0xE84E,
MYANMAR_LIG_80 = 0xE84F,
MYANMAR_LIG_81 = 0xE850,
MYANMAR_LIG_82 = 0xE851,
MYANMAR_LIG_83 = 0xE852,
MYANMAR_LIG_84 = 0xE853,
MYANMAR_LIG_85 = 0xE854,
MYANMAR_LIG_86 = 0xE855,
MYANMAR_LIG_87 = 0xE856,
MYANMAR_LIG_88 = 0xE857,
MYANMAR_LIG_89 = 0xE858,
MYANMAR_LIG_90 = 0xE859,
MYANMAR_LIG_91 = 0xE85A,
MYANMAR_LIG_92 = 0xE85B,
MYANMAR_LIG_93 = 0xE85C,
MYANMAR_LIG_94 = 0xE85D,
MYANMAR_LIG_95 = 0xE85E,
MYANMAR_LIG_96 = 0xE85F,
MYANMAR_LIG_97 = 0xE860,
MYANMAR_LIG_98 = 0xE861,
MYANMAR_LIG_99 = 0xE862,
MYANMAR_LIG_100 = 0xE863,
MYANMAR_LIG_101 = 0xE864,
MYANMAR_LIG_102 = 0xE865,
MYANMAR_LIG_103 = 0xE866,
MYANMAR_LIG_104 = 0xE867,
MYANMAR_LIG_105 = 0xE868,
MYANMAR_LIG_106 = 0xE869,
MYANMAR_LIG_107 = 0xE86A,
MYANMAR_LIG_108 = 0xE86B,
MYANMAR_LIG_109 = 0xE86C,
MYANMAR_LIG_110 = 0xE86D,
MYANMAR_LIG_111 = 0xE86E,
MYANMAR_LIG_112 = 0xE86F,
MYANMAR_LIG_113 = 0xE870,
MYANMAR_LIG_114 = 0xE871,
MYANMAR_LIG_115 = 0xE872,
MYANMAR_LIG_116 = 0xE873,
MYANMAR_LIG_117 = 0xE874,
MYANMAR_LIG_118 = 0xE875,
MYANMAR_LIG_119 = 0xE876,
MYANMAR_LIG_120 = 0xE877,
MYANMAR_LIG_121 = 0xE878,
MYANMAR_LIG_122 = 0xE879,
MYANMAR_LIG_123 = 0xE87A,
MYANMAR_LIG_124 = 0xE87B,
MYANMAR_LIG_125 = 0xE87C,
MYANMAR_LIG_126 = 0xE87D,
MYANMAR_LIG_127 = 0xE87E,

MYANMAR_LIG_128 = 0xE87F,
MYANMAR_LIG_129 = 0xE880,
MYANMAR_LIG_130 = 0xE881,
MYANMAR_LIG_131 = 0xE882,
MYANMAR_LIG_132 = 0xE883,
MYANMAR_LIG_133 = 0xE884,
MYANMAR_LIG_134 = 0xE885,
MYANMAR_LIG_135 = 0xE886,
MYANMAR_LIG_136 = 0xE887,
MYANMAR_LIG_137 = 0xE888,
MYANMAR_LIG_138 = 0xE889,
MYANMAR_LIG_139 = 0xE88A,
MYANMAR_LIG_140 = 0xE88B,
MYANMAR_LIG_141 = 0xE88C,
MYANMAR_LIG_142 = 0xE88D,
MYANMAR_LIG_143 = 0xE88E,
MYANMAR_LIG_144 = 0xE88F,
MYANMAR_LIG_145 = 0xE890,
MYANMAR_LIG_146 = 0xE891,
MYANMAR_LIG_147 = 0xE892,
MYANMAR_LIG_148 = 0xE893,
MYANMAR_LIG_149 = 0xE894,
MYANMAR_LIG_150 = 0xE895,
MYANMAR_LIG_151 = 0xE896,
MYANMAR_LIG_152 = 0xE897,
MYANMAR_LIG_153 = 0xE898,
MYANMAR_LIG_154 = 0xE899,
MYANMAR_LIG_155 = 0xE89A,
MYANMAR_LIG_156 = 0xE89B,
MYANMAR_LIG_157 = 0xE89C,

MYANMAR_LIG_157_1 = 0xE89D,
MYANMAR_LIG_158 = 0xE89E,
MYANMAR_LIG_159 = 0xE89F,
MYANMAR_LIG_160 = 0xE8A0,
MYANMAR_LIG_161 = 0xE8A1,
MYANMAR_LIG_162 = 0xE8A2,
MYANMAR_LIG_163 = 0xE8A3,
MYANMAR_LIG_164 = 0xE8A4,
MYANMAR_LIG_165 = 0xE8A5,
MYANMAR_LIG_166 = 0xE8A6,
MYANMAR_LIG_167 = 0xE8A7,
MYANMAR_LIG_168 = 0xE8A8,

MYANMAR_LIG_169 = 0xE8A9,
MYANMAR_LIG_170 = 0xE8AA,
MYANMAR_LIG_171 = 0xE8AB,
MYANMAR_LIG_172 = 0xE8AC,
MYANMAR_LIG_173 = 0xE8AD,
MYANMAR_LIG_174 = 0xE8AE,
MYANMAR_LIG_175 = 0xE8AF,
MYANMAR_LIG_176 = 0xE8B0,
MYANMAR_LIG_177 = 0xE8B1,
MYANMAR_LIG_178 = 0xE8B2,
MYANMAR_LIG_179 = 0xE8B3,
MYANMAR_LIG_180 = 0xE8B4,
MYANMAR_LIG_181 = 0xE8B5,
MYANMAR_LIG_182 = 0xE8B6,
MYANMAR_LIG_183 = 0xE8B7,
MYANMAR_LIG_184 = 0xE8B8,
MYANMAR_LIG_185 = 0xE8B9,
MYANMAR_LIG_186 = 0xE8BA,
MYANMAR_LIG_187 = 0xE8BB,
MYANMAR_LIG_188 = 0xE8BC,
MYANMAR_LIG_189 = 0xE8BD,
MYANMAR_LIG_190 = 0xE8BE,
MYANMAR_LIG_191 = 0xE8BF,
MYANMAR_LIG_192 = 0xE8C0,
MYANMAR_LIG_193 = 0xE8C1,
MYANMAR_LIG_194 = 0xE8C2,

}Myanmar_ligrature_enum;
/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/
#if defined(__MMI_MULTITAP_MYANMAR__)

extern const UI_character_type default_myanmar_multitap_strings[MAX_MULTITAPS][32];
extern const UI_character_type default_myanmarmatra_multitap_strings[MAX_MULTITAPS][32];
extern const UI_character_type default_myanmarmatra1_multitap_strings[MAX_MULTITAPS][32];
extern const UI_character_type default_myanmarmatra2_multitap_strings[MAX_MULTITAPS][32];
extern const UI_character_type default_myanmarmatra3_multitap_strings[MAX_MULTITAPS][32];
#endif
/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
#if defined(__MMI_MULTITAP_MYANMAR__)
extern UI_string_type get_myanmar_multitap_string(S32 index);
#endif

#endif /* MYANMAR_H */ 



