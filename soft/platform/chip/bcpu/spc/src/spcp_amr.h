
typedef struct
{
    UINT8 dtxUsed;
    UINT8 prevId;
    UINT8 rescheduledSidFirst;
    UINT8 rescheduledSidUpdate;
    UINT8 previousFrameType;
} SPC_AMR_STATE_T;

EXPORT SPC_AMR_STATE_T g_spcAmrState;


