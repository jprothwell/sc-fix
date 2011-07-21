#ifndef __RDAMTV_ADP_H__
#define __RDAMTV_ADP_H__

#ifndef ATV_APP_MODULE
#define ATV_APP_MODULE  	1
#endif

#ifndef CAMERA_APP_MODULE
#define CAMERA_APP_MODULE   0
#endif

int TLGAPP_SetChannelMapExt(int area);
int TLGAPP_SetChannel(int chn);

void analogtv_set_curmodule(unsigned char is_atv_app);
void analogtv_module_power_on(unsigned char on);
unsigned char analogtv_get_curmodule(void);
void SetAtv_Fullscreen(unsigned char on);
unsigned short analogtv_get_chn_count(void);

int TLG_GetYUVScale(uint32 base_addr, uint16 *y, uint16 *u, uint16 *v);
int TLG_SetYUVScale(uint32 base_addr, uint16 y,uint16 u, uint16 v);

int TLGAPP_SetChannelMapChina(void);
int TLGAPP_SetChannelMapJapan(void);
int TLGAPP_SetChannelMapVietnam(void);
int TLGAPP_SetChannelMapBrazil(void);
int TLGAPP_SetChannelMapArgentina(void);
int TLGAPP_SetChannelMapAmerica(void);
int TLGAPP_SetChannelMapHongkong(void);
int TLGAPP_SetChannelMapEurope(void);
int TLGAPP_SetChannelMapShenzhen(void);

#endif

