// 
// ================================================================================
// 
//   This source code is property of StackCom. The information contained in this
//   file is confidential. Distribution, reproduction, as well as exploitation,or 
//   transmisison of any content of this file is not allowed except if expressly
//   permitted. Infringements result in damage claims.
// 
//   Copyright StackCom (c) 2003
// 
// ================================================================================
// 

// 
// ================================================================================
//   File       :  STT.H
// --------------------------------------------------------------------------------
// 
//   Scope      :  Stub and Test functions headers
// 
//   History    :
// --------------------------------------------------------------------------------
//   Nov 15 05  |  PFA   | STT_WAIT uses sxr_Sleep instead of a timer
// --------------------------------------------------------------------------------
//   Oct 05 05  |  MCE   | STT_MEM_CHECK issues only a warning if less buffer than
//              |        | flashed
// --------------------------------------------------------------------------------
//   Apr 29 04  |  MCE   | Addition of stt_UnVoidEvt
// --------------------------------------------------------------------------------
//   Apr 22 04  |  FCH   | Add macros STT_MEM_FLASH and STT_MEM_CHECK
// --------------------------------------------------------------------------------
//   Jun 12 03  |  PFA   | Creation            
// ================================================================================
// 
#ifndef __STT_H__
#define __STT_H__

#include "sxs_type.h"
#include "sxs_cfg.h"
#include "itf_msgc.h"
#include "itf_stt.h"
#include "cmn_defs.h"
#include "fsm.h"


#ifdef __STT__
#define DefExtern
#else
#define DefExtern extern
#endif


#define NB_DIGIT_SCE 3

#define STT_EVT_NOT_FOUND -1

#define STT_CHK_CLUSTER    1

void stt_Fail (void);


#define STT_FAIL                                                        \
        {                                                               \
          stt_Fail();                                                   \
          return 0;                                                     \
        }


#define STT_CHECK( _evt, _task )                                        \
        if (stt_CheckEvt(_evt, _task) == 0 )                            \
          {                                                             \
           stt_Fail();                                                  \
           return 0;                                                    \
          }                                                             \
        else                                                            \
          {                                                             \
           sxs_fprintf( _STT|TSMAP(1), "Evt  <%s> checked\n", #_evt );  \
          }


#define STT_CHECK_NOT( _evt, _task )                                      \
        if (stt_CheckNotEvt(_evt, _task) == 0 )                           \
        {                                                                 \
           stt_Fail();                                                    \
           return 0;                                                      \
        }                                                                 \
        else                                                              \
        {                                                                 \
           sxs_fprintf( _STT|TSMAP(1), "Evt  <%s> checked not\n", #_evt); \
        }

#ifdef __STT_NO_END__

#define STT_LAST_END                                            \
        {                                                       \
         UINT32 Evt[SXR_EVT_MBX_SIZE] = { STT_SUCCESS, 0 };        \
         sxr_Send( Evt, STT_MBX, SXR_SEND_EVT );                \
         return 1 ;                                             \
        }

#define STT_LAST_END_CHK_POOL                                          \
        {                                                              \
         UINT32 Evt[SXR_EVT_MBX_SIZE] = { STT_SUCCESS, STT_CHK_CLUSTER }; \
         sxr_Send( Evt, STT_MBX, SXR_SEND_EVT );                       \
         return 1 ;                                                    \
        }

#define STT_END           {return 1;}                                    
#define STT_END_CHK_POOL  {return 1;}                                    

#else

#define STT_END                                                 \
        {                                                       \
         UINT32 Evt[SXR_EVT_MBX_SIZE] = { STT_SUCCESS, 0 };        \
         sxr_Send( Evt, STT_MBX, SXR_SEND_EVT );                \
         return 1 ;                                             \
        }

#define STT_END_CHK_POOL                                               \
        {                                                              \
         UINT32 Evt[SXR_EVT_MBX_SIZE] = { STT_SUCCESS, STT_CHK_CLUSTER }; \
         sxr_Send( Evt, STT_MBX, SXR_SEND_EVT );                       \
         return 1 ;                                                    \
        }
#endif

#define STT_WAIT(_val)           sxr_Sleep ( _val );

#define STT_WAIT_SECONDS(_val)   sxr_Sleep ( _val SECONDS  );


#ifndef __TARGET__
DefExtern UINT32 scen_MemFlash;
extern UINT32 sxr_NbUsedBlk;;

#define STT_MEM_FLASH                                                  \
        {                                                              \
        sxr_CheckCluster( );                                           \
        STT_WAIT(500 MILLI_SECONDS);                                   \
        scen_MemFlash = sxr_NbUsedBlk;                                 \
        sxs_fprintf ( _STT,"Start scen with %d blocks", scen_MemFlash);\
        }  

#define STT_MEM_CHECK                                                         \
        {                                                                     \
        UINT32 _Tmp;                                                              \
        sxr_CheckCluster();                                                   \
        STT_WAIT(500 MILLI_SECONDS);                                          \
        _Tmp = sxr_NbUsedBlk;                                                  \
        sxs_fprintf(_STT,"End scen with %d blocks", _Tmp);                     \
        if (scen_MemFlash < _Tmp)                                             \
           {                                                                  \
           sxs_fprintf(_STT,"!!!!! ERROR : Remaining %d blocks instead of %d",\
                             _Tmp, scen_MemFlash);                             \
           sxr_CheckHeap();                                                   \
           STT_FAIL;                                                          \
           }                                                                  \
        if (scen_MemFlash > _Tmp)                                             \
          {                                                                  \
          sxs_fprintf(_STT,"!!!!! Warning : Remaining %d blocks instead of %d",\
                            _Tmp, scen_MemFlash);                             \
          sxr_CheckHeap();                                                   \
          }                                                                  \
        }
#endif // __TARGET__



#define STT_PRINT(_str)    sxs_fprintf( _STT , #_str ); 

// 
// ** Event definition for a Stub
// 
typedef struct 
{
  UINT32     EvtNum            ; // Event Number (as in Protocol Stack)
  UINT16     EvtRec            ; // Marker : 1 if received    
  UINT16     EvtVoid           ; // Marker : 1 if to be voided for check 
  UINT16     (* ScFn)   (void) ; // Scen. Func. to call when evt EvtNum received.
                              // None if 0
  CHAR * EvtName           ;

} stt_Evt_t;	       

#define STT_EVT(_Name) { _Name, 0, 0, 0, #_Name }

// 
// ** Task context for a Stub
// 
typedef struct 
{ 
  stt_Evt_t *  EvtList;  // List of Events	       
  UINT16          EvtSize;  // Number of possible Events
} stt_TaskCtx_t;
 
// 
// ** Task context for stt : List of Scenari fonctions
// 
typedef UINT16 stt_ScenFunc_t( void ) ; // Scenario Fn Type

typedef struct
{
 stt_ScenFunc_t * Func ;
 CHAR          * Name ;
 CHAR          * Parameters;
 CHAR          * Help;
 CHAR          * ActualParameters;
} stt_Scen_t ;

typedef struct
{
 UINT32          ScenNb     ;
 stt_Scen_t * ScenList   ;
 UINT32          EvtNb      ;
 EvtFunc_t  * EvtFuncList;
} stt_Param_t ;


void stt_Task (stt_Param_t * Param);

INT16 stt_IdxEvt( EvtFunc_t* TabFunc, UINT32 EvtId, UINT16 Size );
UINT16 stt_FindIdx     ( stt_Evt_t * SttEvt, UINT16 Size, UINT32 Val );
void stt_VoidEvt    ( UINT32 EvtNum, stt_TaskCtx_t * TaskCtx );
void stt_UnVoidEvt  ( UINT32 EvtNum, stt_TaskCtx_t * TaskCtx );
void stt_CfgEvt     ( UINT32 EvtNum, stt_TaskCtx_t * TaskCtx, UINT16(* Func)(void) );
CHAR *stt_RaiseEvt   ( UINT32 EvtNum, stt_TaskCtx_t * TaskCtx );
void stt_HandleEvt  ( UINT32 EvtNum, stt_TaskCtx_t * TaskCtx );
UINT16 stt_CheckEvt    ( UINT32 EvtNum, stt_TaskCtx_t * TaskCtx );
UINT16 stt_CheckNotEvt ( UINT32 EvtNum, stt_TaskCtx_t * TaskCtx );
void stt_CleanEvtRec( stt_TaskCtx_t * TaskCtx );
void stt_FreeParam  ( stt_Param_t * Param );

DefExtern UINT16 scen_argc;
DefExtern UINT8 *ActualParameters;
#ifdef __STT__
UINT8 *scen_argv[6];


#ifdef __TARGET__
UINT8 Args[40][100]; // 40 Scenario max when Running on Target
#endif

#else
extern UINT8 *scen_argv[];

#ifdef __TARGET__
extern UINT8 Args[40][100];
#endif

#endif

#undef DefExtern
#endif
