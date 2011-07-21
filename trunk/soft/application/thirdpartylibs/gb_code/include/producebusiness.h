#ifndef __PRODUCE_BUSINESS_H__
#define __PRODUCE_BUSINESS_H__

#include "producemanager.h"

#if OPEN_CONFIG > 0

#ifndef CONFIG_READ_TXT
extern char* _huge produceItem[];
#endif

/*!
 * \brief º”‘ÿ≤˙∆∑≈‰÷√ 
 */
void loadProduceConfig(void);

void updateConfigDataByProduce(int proID);
#endif

#endif