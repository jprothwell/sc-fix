//EPH1200手写识别命令模块

#ifndef	__HWRCMD_H__
#define	__HWRCMD_H__

#include <csw.h>
#include "string.h"
#include "crc8.h"
#include "mmi_trace.h"
#include "stack_config.h"
#include "oslmemory.h"
#define SIZEOFARRAY(array)	(sizeof(array)/sizeof(array[0]))

//错误代码定义
#define HWR_ERR_OK				0
#define HWR_ERR_SEND_FAIL		1
#define HWR_ERR_RECV_FAIL		2
#define HWR_ERR_INVALID_PARA	3
#define HWR_ERR_NOCMDENTRY		4	//没有相应函数入口
#define HWR_ERR_SEND_TIMEOUT	5
#define HWR_ERR_RECV_TIMEOUT	6	//超时
#define HWR_ERR_CHKSUM			7	//校验错
#define HWR_RESPONSE_ACK_ERR	8	//Ack error
#define HWR_ERR_RECV_CMD		9	//接收返回的命令字错误
#define HWR_ERR_OPEN_FAIL		10	//HWR通讯串口打开错误


//发送接收数据长度
#define MAX_BUF_LEN		32	//外部数据包长度
#define MAX_BODY_LEN	32	//发送命令参数长度
#define MAX_STUFF_LEN	16	//发送命令参数stuff部分长度
#define HWR_MAX_RECOGNIZE_CHAR_NUM	10	//最多可识别10个汉字
#define HWR_MAX_RECOGNIZE_BUF_LEN	20	//最多可识别10个汉字

//超时时间:1s,根据penup等待时间,这里要超过识别等待时间
//xk #define TIMEOUT_NUM		(OS_TICKS_PER_SEC)
 #define TIMEOUT_NUM		200
//命令字段定义
#define HWR_CMD_HEADER			0x50	//命令头
#define HWR_CMD_HEADER_LEN		1
#define HWR_CMD_CODE_LEN		1
#define HWR_CMD_PARA_LEN		1
#define HWR_CMD_PACKHEAD_LEN	(HWR_CMD_HEADER_LEN+HWR_CMD_CODE_LEN+HWR_CMD_PARA_LEN)	//命令头信息长度:header+cmd+paralen
#define HWR_CMD_BUF_LEN			4
#define HWR_CMD_CHECKSUM_LEN	1
#define HWR_CMD_LEN				(HWR_CMD_PACKHEAD_LEN+HWR_CMD_BUF_LEN+HWR_CMD_CHECKSUM_LEN)	//命令信息长度:命令头+参数数据+校验和

//HWR命令字段位置定义
#define HWR_CMD_HEADER_OFFSET	0
#define HWR_CMD_CODE_OFFSET		(HWR_CMD_HEADER_OFFSET+HWR_CMD_HEADER_LEN)
#define HWR_CMD_PARA_OFFSET		(HWR_CMD_CODE_OFFSET+HWR_CMD_CODE_LEN)	
#define HWR_CMD_BUF_OFFSET		(HWR_CMD_PARA_OFFSET+HWR_CMD_PARA_LEN)

//HWR识别字符类型(0x0100~0x2000对应第3字节,0x0001对应第4字节)
#define HWR_RECOGNIZE_TYPE_CHINESEI		0x0001
#define HWR_RECOGNIZE_TYPE_CHINESEII	0x0002
#define HWR_RECOGNIZE_TYPE_ENGCAPITAL	0x0004
#define HWR_RECOGNIZE_TYPE_NUM			0x0008
#define HWR_RECOGNIZE_TYPE_SYMBOL		0x0010
#define HWR_RECOGNIZE_TYPE_DEFAULT		0x0020
#define HWR_RECOGNIZE_TYPE_ENG			0x0100
#define HWR_RECOGNIZE_TYPE_ALL			0x01ff

//stroke(gesture)预设手势符号定义
#define	HWR_SYMBOL_BACKSPACE	0x0008
#define	HWR_SYMBOL_ENTER		0x000D
#define	HWR_SYMBOL_DELETE		0x0010
#define	HWR_SYMBOL_SPACE		0x0020

//inking传送是否开启
#define HWR_INKING_TRANSMIT_ON	0x01
#define HWR_INKING_TRANSMIT_OFF	0x00

//PenUp等待时间(每步200ms)
#define HWR_PENUP_WAITTIME_MIN		1	//penup等待时间最小步值
#define HWR_PENUP_WAITTIME_MAX		10	//penup等待时间最大步值
#define HWR_PENUP_WAITTIME_DEFAULT	4	//3
#define HWR_PENUP_WAITTIME_UNIT	200		//每步200ms等待时间,缺省600ms

//按下触摸屏是否唤醒eph1200,设置识别的字符是否奇数包
#define HWR_SETEVENPACKAGE_ON	0x01
#define HWR_SETEVENPACKAGE_OFF	0x00

//PenUp等待时间偏差
#define HWR_PENUP_TIMEOUTOFFSET_ON		0x01
#define HWR_PENUP_TIMEOUTOFFSET_OFF		0x00
#define HWR_PENUP_TIMEOUTOFFSET_UNIT	100//偏差值100ms

//内码输出类型
#define HWR_CODETBL_GB2312		0x00
#define HWR_CODETBL_UNICODE		0x01

//书写区域范围:根据LCD定义
#define HWR_XCOORDINATE_MIN		0x00
#define HWR_XCOORDINATE_MAX		0xFE
#define HWR_YCOORDINATE_MIN		0x00
#define HWR_YCOORDINATE_MAX		0xFE
#define HWR_XCOORDINATE_LEFT_DEFAULT	0x00
#define HWR_XCOORDINATE_RIGHT_DEFAULT	0xFE
#define HWR_YCOORDINATE_TOP_DEFAULT		0x00
#define HWR_YCOORDINATE_BOTTOM_DEFAULT	0xFE

//识别图形模式还是识别模式
#define HWR_RECOGNIZE_MODE		0x00
#define HWR_GRAPHIC_MODE		0x01

//省电模式:按下唤醒eph1200是否enable
#define HWR_TAP_WAKEUP_DISABLE	0X00
#define HWR_TAP_WAKEUP_ENABLE	0X01

//省电模式:省电模式生效idle时间(每步15s)
#define HWR_POWERSAVING_IDLETIME_MIN	1
#define HWR_POWERSAVING_IDLETIME_MAX	20
#define HWR_POWERSAVING_IDLETIME_UNIT	15		//每步15s
#define	HWR_POWERSAVING_NOENTER			0x00	//不进入省电模式
#define	HWR_POWERSAVING_INSTANTENTER	0x80	//立即进入省电模式

//AD转化模式
#define HWR_ADRESOLUTE_TENBIT	0x00
#define HWR_ADRESOLUTE_EIGHTBIT	0x01

//调整XY坐标
#define HWR_ROTATE_XYCHANGE		0x40	//xy都变化:XN/YN,XP/YP exchange
#define HWR_ROTATE_YFIXED		0x20	//XN/XP exchange,Y fixed
#define HWR_ROTATE_XFIXED		0x10	//YN/YP exchange,X fixed		

//返回的轨迹坐标
#define HWR_XCOORDINATE_STROKEOVER	0xff
#define HWR_YCOORDINATE_STROKEOVER	0xff
#define HWR_XCOORDINATE_WORDOVER	0xff
#define HWR_YCOORDINATE_WORDOVER	0x00

//最大识别字符个数
#define HWR_RECOGNIZE_CHARACTERS_NUM	0x0a	//识别10个汉字


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
//手写命令执行返回结果的接收
int HWRRecvResponse(pHWRCommand pcmd);

//唤醒eph1200
int HWRWakeUpEPH1200(void);

//设置字符识别模式
int HWRSetRecognizeMode(UINT16 mode);

//设置是否打开轨迹传送
int HWRSetInkingSwitch(UINT8 mode);

//设置笔起等待时间
int HWRSetPenUpWaitTime(UINT8 time);

//超时之前执行识别动作
int HWRRecognizeBeforeTimeout(void);

//host通知eph1200已准备好
int HWRHostReady(void);

//设置奇数包和按下触摸屏唤醒eph1200
int HWRSetEvenPackage(UINT8 type);

//设置笔起超时时间偏差
int HWRSetPenUpTimeoutOffset(UINT8 offset);

//获取系统版本号
int HWRGetFirmwareVer(void);

//使用不同内码表输出
int HWROutputWithDiffCodeTbl(UINT8 type);

//软件复位eph1200
int HWRSoftwareReset(void);

//清除当前轨迹
int HWRClearCurrentInking(void);

//eph1200校准
int HWRCalibration(void);

//设置书写区域范围
int HWRSetWritingArea(UINT8 left, UINT8 top, UINT8 right, UINT8 bottom);

//设置识别图形模式


//设置电源节省模式
int HWRSetPowerSavingMode(UINT8 enable, UINT8 idletime);

//设置AD转化模式
int HWRSetADResoluteMode(UINT8 mode);

//调整XY坐标
int HWRRotateTouchPanel(UINT8 style);

//获取ROM校验和
int HWRGetRomChecksum(void);


//HWR初始化:包括图形识别模式选择,延时等配置
int HWRRecognizeInit(void);
#endif
//INT16 HWRSetRecognizeGraphicMode(UINT8 mode);

#endif
