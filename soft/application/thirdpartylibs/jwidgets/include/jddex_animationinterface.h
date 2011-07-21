

#ifdef __cplusplus
    extern   "C"
    {
#endif
// the ANIM_DRAW_CB is defined in emps.h
//typedef void (*ANIM_DRAW_CB)(JC_HANDLE hAnim, void* pDecodedBuffer, UINT32 uiLength, void* param);
typedef struct TAG_DRAW_ANIM_EVENT_DATA
{
	JC_HANDLE hAnim;
    void* pDecodedBuffer;
    JC_UINT32 uiLength;
    void* param;
} DRAW_ANIM_EVENT_DATA_T ;


JC_RETCODE jddex_PlatformGetAnimResolution(JC_OBJECT_INFO *pstObjectInfo, JC_UINT32 *puiWidth, JC_UINT32 *puiHeight);


JC_HANDLE jddex_PlatformOpenAnim(JC_OBJECT_INFO	*pstObjectInfo, JC_UINT32 uiWidth, JC_UINT32 uiHeight, /*ANIM_DRAW_CB pvCallback*/ void* param);


JC_RETCODE jddex_PlatformPlayAnimReq (JC_HANDLE	hAnim, EImageProperty eImageProperty);



JC_RETCODE jddex_PlatformCloseAnim (JC_HANDLE vHAnim);


JC_RETCODE jddex_PlatformStopAnimReq (JC_HANDLE vHAnim);
void jddex_ShowAnimation( DRAW_ANIM_EVENT_DATA_T* pAnimDrawEventData);

#ifdef __cplusplus
    }    
#endif
