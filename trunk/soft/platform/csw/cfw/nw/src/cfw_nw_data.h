#include <cswtype.h>
#include <cfw.h>



typedef struct _NW_SM_INFO {
    UINT32 nMsgId;
    VOID *pMsgBody;
    UINT32 nIndex;
    UINT8  nNetMode;      // 0: automatic, 1: manual, 4: maual/automatic
    UINT8  nNetModeEx;    // net work mode last used
    UINT8  Rssi;          // received signal strength indication
    UINT8  Ber;           // channel bit error rate
    UINT32 nAreaCellId;   // area code and cell id
    CFW_NW_STATUS_INFO sStatusInfo; // status info
    CFW_NW_STATUS_INFO sGprsStatusInfo; // status info
    UINT8  nOperatorId[6];  // the current PLMN
    UINT8  nAttOperatorId[6]; // the PLMN that want to attached
    BOOL   bStaticAO;       // static ao ?
    BOOL   bReselNW;        // sel or resel 
    UINT32 nCOPSUTI;        // UTI sent to at
    UINT32 nBandUTI;        // change band UTI
    UINT32 nAttachUTI;        // change attach UTI

    UINT8  nBand;           // the band frequence
    BOOL   bDetached;       // stoped or detached
    BOOL   bChangeBand;     // change band ?
    BOOL   bNetAvailable;   // net work available

    UINT16  nReselUTI;       // resel or sel UTI u8 from at
    BOOL   bRoaming;        // roaming ?
    UINT8  nGprsAttState;   // gprs state for attach or detach?
    
    UINT8  nAttach;                  //doing attach 0 : detach 1 : attach 0xFF nothing
    UINT8  nCsStatusIndCount;          // count the cs status from stack

    BOOL  bTimerneed;           // timer need, true or false
    BOOL  bTimeDeleted;        // the timer deleted
    UINT8 nStpCause;           //0: no using; 1: flight mode; 2: Frequency Band
    
}NW_SM_INFO;
//FM:flight mode; FB:brequency band
typedef enum e_stop_cause {
  STOP_CAUSE_NOMAL,
  STOP_CAUSE_FM,
  STOP_CAUSE_FB
} STOP_CAUSE;
typedef struct _NW_FM_INFO{   
    UINT8  n_PrevStatus;
    UINT8  n_CurrStatus;
    UINT8  nTryCount;      
    UINT8  nMode;
    UINT8  nStorageFlag;
}NW_FM_INFO;
typedef struct _NW_STORELIST_INFO{
	CFW_StoredPlmnList p_StoreList[CFW_SIM_COUNT];
}CFW_STORELIST_INFO;
