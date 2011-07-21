#ifndef __ENGINE_H__
#define __ENGINE_H__
#include "mmi_features.h"
#ifdef __MMI_GB_V5__
#include "gbapp.h"

#ifdef _cplusplus
extern "C" {
#endif

#define MAX_DATA_COUNT	32

//添加数据声明
#ifdef __MMI_GB_SM_CHINESE__
extern const unsigned long _huge prv_scDataArray[];
#endif
extern const unsigned long _huge prv_enDataArray[];
#ifdef __MMI_SMART_URDU__
extern const unsigned long _huge prv_urDataArray[];
#endif
#ifdef __MMI_SMART_ARABIC__
extern const unsigned long _huge prv_enArabicDataArray[];
#endif
#if defined(__MMI_SMART_GERMAN__)
extern const unsigned long _huge prv_gerDataArray[];
#endif
#if defined(__MMI_SMART_FRENCH__)
extern const unsigned long _huge prv_frDataArray[];
#endif
#if defined(__MMI_SMART_ITALIAN__)
extern const unsigned long _huge prv_italianDataArray[];
#endif
#if defined(__MMI_SMART_MALAY__)
extern const unsigned long _huge prv_malayDataArray[];
#endif
#if defined(__MMI_SMART_TAMIL__)
extern const unsigned long _huge prv_tamilDataArray[];
#endif
#if defined(__MMI_SMART_SPANISH__)
extern const unsigned long _huge prv_spaDataArray[];
#endif
#if defined(__MMI_SMART_BENGALI__)
extern const unsigned long _huge prv_bengailDataArray[];
#endif
#if defined(__MMI_SMART_HINDI__)
extern const unsigned long _huge prv_hiDataArray[];
#endif
#if defined(__MMI_SMART_INDONESIAN__)
extern const unsigned long _huge prv_indonesianDataArray[];
#endif
#if defined(__MMI_SMART_TELUGU__)
extern const unsigned long _huge prv_teDataArray[];
#endif
#if defined(__MMI_SMART_VIETNAMESE__)
extern const unsigned long _huge prv_viDataArray[];
#endif
#if defined(__MMI_SMART_THAI__)
extern const unsigned long _huge prv_thDataArray[];
#endif
#if defined(__MMI_SMART_PUNJABI__)
extern const unsigned long _huge prv_punjabiDataArray[];
#endif
#if defined(__MMI_SMART_PERSIAN__)
extern const unsigned long _huge prv_enPersianDataArray[];
#endif
#if defined(__MMI_GB_SMART_PORTUGUESE__)
extern const unsigned long _huge prv_ptDataArray[];
#endif
#if defined(__MMI_SMART_RUSSIAN__)
extern const unsigned long _huge prv_ruDataArray[];
#endif
#if defined(__MMI_SMART_TURKISH__)
extern const unsigned long _huge prv_turkishDataArray[];
#endif
extern void newEngine(void);

#ifdef _cplusplus
};
#endif
#endif/*__MMI_GB_V5__*/
#endif
 