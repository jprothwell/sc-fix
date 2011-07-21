#ifndef __CDFLASHUSAGE_H_
#define __CDFLASHUSAGE_H_


typedef enum
{
	CDFU_QQ,
	CDFU_GEO,
	CDFU_MSN,
	CDFU_JMETOC,
	CDFU_INIT,
	CDFU_CSDAPP,
	CDFU_KK,
	CDFU_WAP,
	CDFU_OHTER,
	CDFU_OVERLAY_END,
	CDFU_FONT,
	CDFU_RES,
	CDFU_MAX_APP_CNT
}CDFU_APP_IDX;




void cdfu_app_init(const unsigned char *bufin,  unsigned int bufinlen, unsigned char *bufDes);
void cdfu_do_init(CDFU_APP_IDX id);
void cdfu_init(void);
void cdfu_init_boot(void);
#endif //__CDFLASHUSAGE_H_

