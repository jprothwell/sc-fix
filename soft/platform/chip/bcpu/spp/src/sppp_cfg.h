#ifndef SPP_GLOBAL_PARAMS_H
#define SPP_GLOBAL_PARAMS_H

#include "cs_types.h"
#include "spp_cfg.h"


extern volatile SPP_GLOBAL_PARAMS_T g_sppGlobalParams;

// Accessor Macros
#define BYPASS_DCOC         (g_sppGlobalParams.bypassDcoc)
#define CORRECT_CS1         (g_sppGlobalParams.correctCS1)
#define CORRECT_CS4         (g_sppGlobalParams.correctCS4)
#define EQU_HBURST_MODE     (g_sppGlobalParams.equ_hburst_mode)
//#define VITAC_INT_ENABLE    (g_sppGlobalParams.vitacIntEnable)
#define SYMB_MAX            (g_sppGlobalParams.symb_max)
#define SYMB_MIN            (g_sppGlobalParams.symb_min)
#define FOF_NB_SYMB         (g_sppGlobalParams.fof_nb_symb)
#define FOF_START_1         (g_sppGlobalParams.fof_start_1)
#define FOF_START_2         (g_sppGlobalParams.fof_start_2)
#define FOF_CORR_GAP        (g_sppGlobalParams.fof_corr_gap)
#define MON_WIN_SIZE        (g_sppGlobalParams.mon_win_size)
#define SHIFT_BM_OFFSET     (g_sppGlobalParams.shift_bm_offset)
#define SHIFT_SB_OFFSET     (g_sppGlobalParams.shift_sb_offset)
#if (SPC_IF_VER >= 2)
#define SPP_FCCH_SIZE       (g_sppGlobalParams.fcch_size)
#define SPP_FCCH_NB_WIN     (((625*9)/(SPP_FCCH_SIZE*4) + 1))
#endif

#endif // SPP_GLOBAL_PARAMS_H

