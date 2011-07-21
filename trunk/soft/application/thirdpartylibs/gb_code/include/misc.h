#ifndef __MISC_H__
#define __MISC_H__
#include "mmi_features.h"
#ifdef __MMI_GB_V5__

//RVCT和ADS都需要把该宏打开
#define RVCT 0
#if RVCT > 0 || __linux__ > 0 || _C166 > 0
#define CONSOLE_ENGLISH		1	//英文版本
#define UNICODE_SHOW_CAND	1	//输入候选用unicode表示
#define OPEN_CONFIG			1	//打开配置功能
#define READ_EBD_DATA		0	//读取.c 格式数据文件
#else
#define CONSOLE_ENGLISH		0	//英文版本
#define UNICODE_SHOW_CAND	0	//输入候选用unicode表示
#define OPEN_CONFIG			1	//打开配置功能
#define READ_EBD_DATA		0	//读取ebd 格式数据文件
#endif

//#define CONFIG_READ_TXT	//读配置文件txt，关闭则读.c


#ifndef ARRAY_DIM
#define ARRAY_DIM(a)	(sizeof(a) / sizeof((a)[0]))
#endif
#ifndef CLEAR_ARRAY
#define CLEAR_ARRAY(a)	(memset((a), 0, sizeof(a)))
#endif

#ifndef CLEAR_VARIABLE
#define CLEAR_VARIABLE(a)	(memset(&(a), 0, sizeof(a)))
#endif

#ifdef WIN32
#define _T(a)		a//L##a
#else
#define _T(a)		a
#endif

#ifndef _C166
#define _huge
#endif

#ifndef WIN32
#define BOOL	int
#endif

////   按键定义  //////////////////////////////////////////////////////////////////////
#define DIR_CHR     224
#define F_CHR       0



#ifdef WIN32
	#define C_UP			-72
	#define C_DOWN			-80
	#define C_LEFT			-75
	#define C_RIGHT			-77
	#define C_NEXT_PAGE		-81
	#define C_PREV_PAGE     -83
	#define C_ESCAPE		VK_ESCAPE
	#define C_BACK			VK_BACK
	#define C_OK			VK_RETURN
	#define C_RETURN		VK_RETURN
	#define C_SPACE			VK_SPACE
	#define C_F1			-59
	#define C_F10			-68
	#define GET_SEL_INDEX(c)	(C_F1 - (c)) 
	#define IS_F(k)				((k) >= C_F10 && (k) <= C_F1)
	#define C_CTRL_F1		-94
	#define C_CTRL_F2		-95
	#define C_CTRL_F3		-96
	#define C_CTRL_F4		-97
	#define C_CTRL_F5		-98
	#define C_CTRL_F6		-99
#else
	#define C_UP			'I'
	#define C_DOWN			'K'
	#define C_LEFT			'J'
	#define C_RIGHT			'L'
	#define C_NEXT_PAGE     'O'
	#define C_PREV_PAGE     'U'
	#define C_ESCAPE		'E'
	#define C_BACK			'B'
	#define C_OK			'R'
	#define C_RETURN		C_OK
	#define C_SPACE			' '
	#define C_F1			'A'
	#define C_F10			('A' + 9)
	#define GET_SEL_INDEX(c)    ((c) - C_F1) 
	#define IS_F(k)     ((k) >= C_F1 && (k) <= C_F10)
	#define C_CTRL_F1		'N'
	#define C_CTRL_F2		'M'
	#define C_CTRL_F3		','
	#define C_CTRL_F4		'.'
	#define C_CTRL_F5		'/'
	#define C_CTRL_F6		';'
#endif
#define C_STAR			'*'
#define C_SHARP			'#'
//音调
#define C_TONE_0	C_F1
#define C_TONE_1	C_TONE_0 - 1
#define C_TONE_2	C_TONE_0 - 2
#define C_TONE_3	C_TONE_0 - 3
#define C_TONE_4	C_TONE_0 - 4
#define C_TONE_5	C_TONE_0 - 5
#define C_TONE_6	C_TONE_0 - 6
//注音音调按键
#define C_BPMF_TONE_0	C_CTRL_F1
#define C_BPMF_TONE_1	C_CTRL_F2
#define C_BPMF_TONE_2	C_CTRL_F3
#define C_BPMF_TONE_3	C_CTRL_F4
#define C_BPMF_TONE_4	C_CTRL_F5
#define C_BPMF_TONE_5	C_CTRL_F6


#define IS_DIR(k)   ((k) == C_UP || (k) == C_DOWN || (k) == C_LEFT || (k) == C_RIGHT)



#ifdef __linux__
#include <locale.h>
int UnicodeToUTF_8(GBLPACHAR pOut,GBLPWCHAR pText);
#define ARY_CLEAN(ary) memset(ary,0,sizeof(ary));
#define unicodeStr2utf8(uStr,utf8Str,uStrLth) \
	do{ \
		unsigned short temp[2]; \
		unsigned short *pt =uStr; \
		unsigned short temputf8[10]; \
		\
		ARY_CLEAN(utf8Str); \
		while(*pt && (pt -uStr <uStrLth)) \
		{ \
			temp[0] =*pt++; \
			temp[1] =0; \
			ARY_CLEAN(temputf8); \
			UnicodeToUTF_8(temputf8,temp); \
			strcat(utf8Str,temputf8); \
		} \
	}while(0);

//#define _wcstombs(mbs, wcs, mbslen)   wcstombs(mbs, wcs, mbslen)
//#define _wcstombs(mbs, wcs, mbslen)	UnicodeToUTF_8(mbs, wcs)
#define _wcstombs(mbs, wcs, mbslen)	unicodeStr2utf8(wcs, mbs, mbslen)
#elif defined(WIN32)
#ifndef MULTI_LANG
#define _wcstombs(mbs, wcs, mbslen)   WideCharToMultiByte(CP_ACP, 0, wcs, (int)wcslen(wcs) + 1, mbs, mbslen, NULL, NULL)
#define _mbstowcs(wcs, mbs, wcslen)   MultiByteToWideChar(CP_ACP, 0, mbs, (int)strlen(mbs) + 1, wcs, wcslen)
#else
#define _wcstombs(mbs, wcs, mbslen)   mywcstombs(mbs, wcs, mbslen)
#define _mbstowcs(wcs, mbs, wcslen)   mymbstowcs(wcs, mbs, wcslen)
#endif
#else
#define _wcstombs(mbs, wcs, mbslen)   mywcstombs(mbs, wcs, mbslen)
#define _mbstowcs(wcs, mbs, wcslen)   mymbstowcs(wcs, mbs, wcslen)
#endif

// 返回错语字符串
#define GET_STR(id)	#id

#define FONT_WIDTH			12			//假定手机使用的字体为12 * 12
#define PREEDIT_WIDTH		240			//假定编辑栏宽度为120(实际用来绘制音节、部首的像素宽度)

typedef struct tagLangDef
{
	GBINT				id;
	GBLPCACHAR			engName;
	GBLPCACHAR			chnName;
} GBLangDef;
enum
{
//	CONFIG_EXPLAIN,
	CHN_CONFIG = 1
	,ALP_CONFIG
	,GBAPI_MMI_CONFIG
	,OTHER_CONFIG
};

#define ENGING_CONFIG_MAX 44
// 由于配置关系相关的配置项数目，一般情况下不会超过所有配置项的一半，为了节省栈空间，才定义下面的宏
#define ENGING_CONFIG_SELECT_RESULT (ENGING_CONFIG_MAX/2)

typedef struct
{
	GBINT		index;
	GBINT		label;
	GBBOOL		status;
	GBPCACHAR   intro;
	GBINT		id;
	GBINT		type;
}GBEngineSetting;

extern GBUACHAR _huge uni2gb[][2];
extern GBPACHAR SubLangName[];


extern GBINT mywcstombs(GBLPACHAR mbs, GBLPCWCHAR wcs, GBINT mbslen);
extern GBINT mymbstowcs(GBLPWCHAR wcs, GBLPACHAR mbs,int wcslen);

extern GBPCACHAR getLanguageNameByID(GBINT id);

extern GBPCACHAR getInputModeNameByID(GBINT id);

extern GBINT getCharToIndex(GBUACHAR c);

extern GBUACHAR getIndexToChar(GBINT index);

/** \brief 实现getch的功能 */
extern GBINT mygetch(void);

extern GBINT getInnerKey(GBINT input);
extern GBINT getInputParam(GBINT input);

/** \brief 字符toBPMF */
extern GBINT getCharToBPMF(GBACHAR c);

/** \brief 字符toHalfQwerty */
extern GBINT getCharToHalfQwerty(GBACHAR c);

extern GBINT Keyboard_switch(GBACHAR c);

extern void cleanScreen(void);

extern void WordToUnicode(GBLPCWCHAR str, GBLPACHAR unicode, GBINT count);

extern unsigned short _gb_far * gbitow(int number, unsigned short _gb_far *str, int radix );
extern int gbwtoi(const unsigned short _gb_far *str);
#endif/*__MMI_GB_V5__*/
#endif