#ifndef __CONFIGURE_BUSINESS_H__
#define __CONFIGURE_BUSINESS_H__

#include "configuremanager.h"

#if OPEN_CONFIG > 0

#ifndef CONFIG_READ_TXT
extern char* configItem[];
extern char* configList[];
#endif

/*!
 * \brief ��������ҵ������ 
 */
void loadRelationConfig(void);

#endif
#endif