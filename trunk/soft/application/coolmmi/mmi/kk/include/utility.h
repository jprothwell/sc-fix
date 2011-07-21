#ifndef _UTILITY_H
#define _UTILITY_H


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int32_t UTL_UniStrLen(const byte_t *ustr);
byte_t* UTL_UniStrCpy(byte_t *dest, const byte_t *src);

byte_t* UTL_UniStrSwapEndian(byte_t *dest, byte_t *src);
byte_t* UTL_UniStrnSwapEndian(byte_t *dest, byte_t *src, int32_t len);

int32_t UTL_IntGetFromBet(const byte_t *str);
int32_t UTL_IntGetFromRich(const byte_t *str);

int32_t UTL_RichStrGet(const byte_t * str, byte_t richString[]);
int32_t UTL_BetStrGet(const byte_t * str, byte_t betString[]);

byte_t* UTL_Int2ByteArray(int32_t intData, byte_t byteArr[]);
int32_t UTL_byteArray2Int(const byte_t * pData);

int32_t UTL_Int2NetUniByte(int32_t nInt, byte_t buf[]);
int32_t UTL_NetUniByte2Int(byte_t * s, int32_t len);

int32_t UTL_Ascii2NetUniByte(byte_t * pDest, const char_t * pSrc);
int32_t UTL_NetUniByte2Ascii(char_t * pDest, const byte_t * pSrc, int32_t srcLen);

int32_t UTL_HexStr2Int(char_t * pHexStr, int32_t hexstrlen);

int32_t UTL_Byte2NetUniByte(byte_t * pDest, const byte_t * pSrc, int32_t srcLen);

byte_t* UTL_UniByteStrCat(byte_t *str1, const byte_t *str2);

int32_t UTL_CtlStrGet(byte_t const * s, int32_t inx, byte_t buf[], int32_t bufLen);
int32_t UTL_CtlNumGet(byte_t const * s, int32_t inx);

//////////////////////////////////////////////////////////////////////////
void UTL_TimerVitalMsgStart(void * pDevData, void * pUserData);
void UTL_TimerVitalMsgStop(void * pDevData);

#ifdef GAME_MY_TURN_PROMPT
void UTL_TimerPromptStart(void * pDevData, void * pUserData);
void UTL_TimerPromptStop(void * pDevData);
#endif 

#endif
