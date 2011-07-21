//Create at 2003-5-20 19:59:42 by abing
#ifndef __EBEN_H_
#define __EBEN_H_

//#pragma warning(disable:4103)
//#pragma	pack(1)

#include "gblt_def.h"

/*
ENInputStruct : is the data tranfering between engine and engine's user.
*/
typedef struct __en_input_struct
{
    /*
    (in)inputString : the input number string.
    number '0' matches punctuations like ' and - in word.
    for example if you want to input A-bomb, you should 
    input number 202662. AAfter you changed inputString, 
    firstly you should fill inputLen with the length of 
    inputString, secondly you should invoke EnFirstHeader
    to get a header. Thirdly you should invoke EnFirstPage 
    /EnNextPage/EnPrevPage to get some candidates.
    */
    EN_WORD             inputString;

    /*
    (in)inputLen : the len of inpuString
    it should be smaller than EN_INPUT_MAX_LEN
    */
    unsigned char       inputLen;
#if LATIN_ENCODE_LENGTH == 2
	unsigned char 		unused1;
#endif

    /*
    (out)header: a string which is match the inputString 
    and at least one valid word is begin with. 
    engine's user should not modify this member. 
    for example : when user input "23", the first header will 
    be "be", and candidates is "be", "bedroom", "bedbug"....
    */
    EN_WORD             header;
    /*
 
    candidates. when calculating the sum engine will add 1 byte 
    length between each candidate 
    for example : if the candidates are "able" "absent" "abandon" 
    the sum length is 4 + 1 + 6 + 1 + 7 = 19. Generally speaking 
    if you want to display candidates in one line horizontally,
    you should set this member with the count of charaters which 
    you can display in one time. Before you invoke EnStartup, 
    you must set this member with a value between 0 and 
    EN_MAX_DESIRE_LEN. if you specify 0 to this member when 
    you invoke EnStartup, the engine will not care about this 
    member when finding candidates. so engine will try to find as 
    many as candidateDesireCount describe candidates. When invoke 
    EnStartUp, at least one of candidateDesireLen and 
    candidateDesireCount is to be greater than 0. this value should 
    not be changed after invoking EnStartup
    */
    unsigned char       candidateDesireLen;
    /*
    (in)candidateDesireCount :   at least one of 
    candidateDesireLen and candidateDesireCount should be greater than 0.
    */
	unsigned char		candidateDesireCount;
    /*
    (out)candidateCount :       store the count of all candidates.
    */
    unsigned char       candidateCount;
#if LATIN_ENCODE_LENGTH == 2
        unsigned char 		unused2;
#endif
        /*
    (out)candidates :           store each candidate
    */
    EN_WORD             candidates[EN_CANDIDATE_MAX_COUNT + 1];
    /*
    (out)headerIsAWord :  1 indicate the string store in header is a word, 
    otherwise will be 0.
    */
    unsigned char       headerIsAWord;
#if LATIN_ENCODE_LENGTH == 2
        unsigned char 		unused3;
#endif
    
    /*
    (in)
    EN_OPTION_DONT_ADJUST_CHAR_ORDER :  
        �������ĸ��ѡ��ʱ��Ҫ�������Ե����Խ��к�ѡ��˳�����
    EN_OPTION_OUTPUT_NORMAL :
        �����ʱ��ʹ�����ܴ�Сд����ʽ��������ĸ��ʱ��Сд��ǰ
    EN_OPTION_OUTPUT_FCC : 
        �����ʱ��ʹ������ĸ��д����ʽ
    EN_OPTION_OUTPUT_CAPITAL : 
        �����ʱ��ʹ��ȫ����д��ʹ�ã�������ĸ��ʱ���д��ǰ
    EN_OPTION_OUTPUT_LOWER:
        �����ʱ��ȫ��ʹ��Сд
    EN_OPTION_OUTPUT_LOWER_CHAR:
        ������ĸ��ʱ��ֻ���Сд��ĸ������ʱ�������ͨ�ĵ���
    EN_OPTION_OUTPUT_CAPITAL_CHAR:
        ������ĸ��ʱ��ֻ�����д��ĸ������ʱ�������ͨ�ĵ���
    */
    unsigned short      option;

} ENInputStruct, * pENInputStruct;


#include "gblt_func.h"

#endif
