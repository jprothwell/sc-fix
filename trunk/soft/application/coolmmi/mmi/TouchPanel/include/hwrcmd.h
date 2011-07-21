//EPH1200��дʶ������ģ��

#ifndef	__HWRCMD_H__
#define	__HWRCMD_H__

#include <csw.h>
#include "string.h"
#include "crc8.h"
#include "mmi_trace.h"
#include "stack_config.h"
#include "oslmemory.h"
#define SIZEOFARRAY(array)	(sizeof(array)/sizeof(array[0]))

//������붨��
#define HWR_ERR_OK				0
#define HWR_ERR_SEND_FAIL		1
#define HWR_ERR_RECV_FAIL		2
#define HWR_ERR_INVALID_PARA	3
#define HWR_ERR_NOCMDENTRY		4	//û����Ӧ�������
#define HWR_ERR_SEND_TIMEOUT	5
#define HWR_ERR_RECV_TIMEOUT	6	//��ʱ
#define HWR_ERR_CHKSUM			7	//У���
#define HWR_RESPONSE_ACK_ERR	8	//Ack error
#define HWR_ERR_RECV_CMD		9	//���շ��ص������ִ���
#define HWR_ERR_OPEN_FAIL		10	//HWRͨѶ���ڴ򿪴���


//���ͽ������ݳ���
#define MAX_BUF_LEN		32	//�ⲿ���ݰ�����
#define MAX_BODY_LEN	32	//���������������
#define MAX_STUFF_LEN	16	//�����������stuff���ֳ���
#define HWR_MAX_RECOGNIZE_CHAR_NUM	10	//����ʶ��10������
#define HWR_MAX_RECOGNIZE_BUF_LEN	20	//����ʶ��10������

//��ʱʱ��:1s,����penup�ȴ�ʱ��,����Ҫ����ʶ��ȴ�ʱ��
//xk #define TIMEOUT_NUM		(OS_TICKS_PER_SEC)
 #define TIMEOUT_NUM		200
//�����ֶζ���
#define HWR_CMD_HEADER			0x50	//����ͷ
#define HWR_CMD_HEADER_LEN		1
#define HWR_CMD_CODE_LEN		1
#define HWR_CMD_PARA_LEN		1
#define HWR_CMD_PACKHEAD_LEN	(HWR_CMD_HEADER_LEN+HWR_CMD_CODE_LEN+HWR_CMD_PARA_LEN)	//����ͷ��Ϣ����:header+cmd+paralen
#define HWR_CMD_BUF_LEN			4
#define HWR_CMD_CHECKSUM_LEN	1
#define HWR_CMD_LEN				(HWR_CMD_PACKHEAD_LEN+HWR_CMD_BUF_LEN+HWR_CMD_CHECKSUM_LEN)	//������Ϣ����:����ͷ+��������+У���

//HWR�����ֶ�λ�ö���
#define HWR_CMD_HEADER_OFFSET	0
#define HWR_CMD_CODE_OFFSET		(HWR_CMD_HEADER_OFFSET+HWR_CMD_HEADER_LEN)
#define HWR_CMD_PARA_OFFSET		(HWR_CMD_CODE_OFFSET+HWR_CMD_CODE_LEN)	
#define HWR_CMD_BUF_OFFSET		(HWR_CMD_PARA_OFFSET+HWR_CMD_PARA_LEN)

//HWRʶ���ַ�����(0x0100~0x2000��Ӧ��3�ֽ�,0x0001��Ӧ��4�ֽ�)
#define HWR_RECOGNIZE_TYPE_CHINESEI		0x0001
#define HWR_RECOGNIZE_TYPE_CHINESEII	0x0002
#define HWR_RECOGNIZE_TYPE_ENGCAPITAL	0x0004
#define HWR_RECOGNIZE_TYPE_NUM			0x0008
#define HWR_RECOGNIZE_TYPE_SYMBOL		0x0010
#define HWR_RECOGNIZE_TYPE_DEFAULT		0x0020
#define HWR_RECOGNIZE_TYPE_ENG			0x0100
#define HWR_RECOGNIZE_TYPE_ALL			0x01ff

//stroke(gesture)Ԥ�����Ʒ��Ŷ���
#define	HWR_SYMBOL_BACKSPACE	0x0008
#define	HWR_SYMBOL_ENTER		0x000D
#define	HWR_SYMBOL_DELETE		0x0010
#define	HWR_SYMBOL_SPACE		0x0020

//inking�����Ƿ���
#define HWR_INKING_TRANSMIT_ON	0x01
#define HWR_INKING_TRANSMIT_OFF	0x00

//PenUp�ȴ�ʱ��(ÿ��200ms)
#define HWR_PENUP_WAITTIME_MIN		1	//penup�ȴ�ʱ����С��ֵ
#define HWR_PENUP_WAITTIME_MAX		10	//penup�ȴ�ʱ�����ֵ
#define HWR_PENUP_WAITTIME_DEFAULT	4	//3
#define HWR_PENUP_WAITTIME_UNIT	200		//ÿ��200ms�ȴ�ʱ��,ȱʡ600ms

//���´������Ƿ���eph1200,����ʶ����ַ��Ƿ�������
#define HWR_SETEVENPACKAGE_ON	0x01
#define HWR_SETEVENPACKAGE_OFF	0x00

//PenUp�ȴ�ʱ��ƫ��
#define HWR_PENUP_TIMEOUTOFFSET_ON		0x01
#define HWR_PENUP_TIMEOUTOFFSET_OFF		0x00
#define HWR_PENUP_TIMEOUTOFFSET_UNIT	100//ƫ��ֵ100ms

//�����������
#define HWR_CODETBL_GB2312		0x00
#define HWR_CODETBL_UNICODE		0x01

//��д����Χ:����LCD����
#define HWR_XCOORDINATE_MIN		0x00
#define HWR_XCOORDINATE_MAX		0xFE
#define HWR_YCOORDINATE_MIN		0x00
#define HWR_YCOORDINATE_MAX		0xFE
#define HWR_XCOORDINATE_LEFT_DEFAULT	0x00
#define HWR_XCOORDINATE_RIGHT_DEFAULT	0xFE
#define HWR_YCOORDINATE_TOP_DEFAULT		0x00
#define HWR_YCOORDINATE_BOTTOM_DEFAULT	0xFE

//ʶ��ͼ��ģʽ����ʶ��ģʽ
#define HWR_RECOGNIZE_MODE		0x00
#define HWR_GRAPHIC_MODE		0x01

//ʡ��ģʽ:���»���eph1200�Ƿ�enable
#define HWR_TAP_WAKEUP_DISABLE	0X00
#define HWR_TAP_WAKEUP_ENABLE	0X01

//ʡ��ģʽ:ʡ��ģʽ��Чidleʱ��(ÿ��15s)
#define HWR_POWERSAVING_IDLETIME_MIN	1
#define HWR_POWERSAVING_IDLETIME_MAX	20
#define HWR_POWERSAVING_IDLETIME_UNIT	15		//ÿ��15s
#define	HWR_POWERSAVING_NOENTER			0x00	//������ʡ��ģʽ
#define	HWR_POWERSAVING_INSTANTENTER	0x80	//��������ʡ��ģʽ

//ADת��ģʽ
#define HWR_ADRESOLUTE_TENBIT	0x00
#define HWR_ADRESOLUTE_EIGHTBIT	0x01

//����XY����
#define HWR_ROTATE_XYCHANGE		0x40	//xy���仯:XN/YN,XP/YP exchange
#define HWR_ROTATE_YFIXED		0x20	//XN/XP exchange,Y fixed
#define HWR_ROTATE_XFIXED		0x10	//YN/YP exchange,X fixed		

//���صĹ켣����
#define HWR_XCOORDINATE_STROKEOVER	0xff
#define HWR_YCOORDINATE_STROKEOVER	0xff
#define HWR_XCOORDINATE_WORDOVER	0xff
#define HWR_YCOORDINATE_WORDOVER	0x00

//���ʶ���ַ�����
#define HWR_RECOGNIZE_CHARACTERS_NUM	0x0a	//ʶ��10������


INT32 HW_INIT(void);
BOOL HW_OPEN(void);
void   HW_Close(void);
INT32 HW_INIT(void);
INT32 HW_TXData(DRV_UARTID UartID,UINT8 *sData,UINT16 sDataLen);
INT32 HW_ReceiveData(DRV_UARTID UartID,UINT8 *RecDataBuf,UINT16 RDataLen);
INT16 HW_RecLEN(DRV_UARTID UartID);

INT16 HWRSetRecognizeGraphicMode(UINT8 mode);
INT16 HWRRotateTouchPanel(UINT8 style);
INT16 HWRSetWritingArea(UINT8 left, UINT8 top, UINT8 right, UINT8 bottom);

INT16 HWRSetRecognizeMode(UINT16 mode);
VOID   HW_TaskEntry(void *pData);

INT16 HWRSetPowerSavingMode(UINT8 enable, UINT8 idletime);
BOOL HW_OPEN_NOTENENT(void);
void   HW_UartCallback1(UINT16 status);
INT16 HWRHostReady(void);

#if 0
//��д����ִ�з��ؽ���Ľ���
int HWRRecvResponse(pHWRCommand pcmd);

//����eph1200
int HWRWakeUpEPH1200(void);

//�����ַ�ʶ��ģʽ
int HWRSetRecognizeMode(UINT16 mode);

//�����Ƿ�򿪹켣����
int HWRSetInkingSwitch(UINT8 mode);

//���ñ���ȴ�ʱ��
int HWRSetPenUpWaitTime(UINT8 time);

//��ʱ֮ǰִ��ʶ����
int HWRRecognizeBeforeTimeout(void);

//host֪ͨeph1200��׼����
int HWRHostReady(void);

//�����������Ͱ��´���������eph1200
int HWRSetEvenPackage(UINT8 type);

//���ñ���ʱʱ��ƫ��
int HWRSetPenUpTimeoutOffset(UINT8 offset);

//��ȡϵͳ�汾��
int HWRGetFirmwareVer(void);

//ʹ�ò�ͬ��������
int HWROutputWithDiffCodeTbl(UINT8 type);

//�����λeph1200
int HWRSoftwareReset(void);

//�����ǰ�켣
int HWRClearCurrentInking(void);

//eph1200У׼
int HWRCalibration(void);

//������д����Χ
int HWRSetWritingArea(UINT8 left, UINT8 top, UINT8 right, UINT8 bottom);

//����ʶ��ͼ��ģʽ


//���õ�Դ��ʡģʽ
int HWRSetPowerSavingMode(UINT8 enable, UINT8 idletime);

//����ADת��ģʽ
int HWRSetADResoluteMode(UINT8 mode);

//����XY����
int HWRRotateTouchPanel(UINT8 style);

//��ȡROMУ���
int HWRGetRomChecksum(void);


//HWR��ʼ��:����ͼ��ʶ��ģʽѡ��,��ʱ������
int HWRRecognizeInit(void);
#endif
//INT16 HWRSetRecognizeGraphicMode(UINT8 mode);

#endif
