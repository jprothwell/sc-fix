//Last Modify Date : 2003-5-6 17:02:37
//: EB_structures.h : ErBiDigital public structures.

#ifndef __EB_STRUCTURES_H_
#define __EB_STRUCTURES_H_
#include <string.h>

#include "eb.h"
//������ѡ�ִʵĽṹ��
typedef struct __candidate_info {
     unsigned char inputLen;//���봮�ĳ��ȣ���������������ķָ���, 0��ʾΪ��ǰ���봮�ĳ���
     unsigned char len;//����ĳ���
     unsigned char start;//��ѡ�ִ���EBInputStruct::candidates�Ŀ�ʼλ��
     unsigned char reserved;//��ʱû��ʹ��
} CandInfo;

//����ÿһ��ѡ���ѡ�ִʵĽṹ��
typedef struct __candidate_sel_info {
     unsigned char inputLen;//���봮�ĳ��ȣ���������������ķָ���
     unsigned char outputLen;//��Ӧ��ѡ�ʵĳ���
} CandSelInfo;

//��ע�⣺ÿ��һ����Ա���Ƕ���Ҫ�޸�EBDAssocInit, EBInputReset
typedef struct __eb_input_struct
{
//in : can modified by user
    unsigned char                           inputMethod ; //���뷨
    unsigned char                           candidateDesireLen ; //�û���Ҫ�ĺ�ѡ�ֳ���
    unsigned char                           interpunctionDesireLen;//�û���Ҫ�ı����ŵĳ���
    unsigned char                           inputStringLen ; //�����봮����
    unsigned char                           inputString[EB_INPUT_MAX_LEN] ; //�����봮[0~9,*]


#define EB_PHRASE_MAX_LEN          10 //�ʵ���󳤶�
#define EB_ASSOC_ISN_MAX_LEN      (EB_PHRASE_MAX_LEN - 1) //�����ֵ���󳤶�
    unsigned short                          associateISNs[EB_ASSOC_ISN_MAX_LEN] ; //������
    unsigned char                           tone ; //����
    unsigned char                           associateISNCount; //�����ֵĸ���
    unsigned char                           syllableIndex ; //��ǰѡ���׼ȷ��������[1 -- 6]



//out : can not modified by user

    unsigned char                           candidateLen ; //ʵ�ʳ���
    unsigned short                          candidates[EB_CANDIDATES_MAX_LEN] ;    //��ѡ��
// syllables��ǰΪһ����ά�����룬����Ϊ1ά���飬ͬ�������˸������ڣ�
//����Ϊ�������ķ�ʽ���������ڵ�ͷ��ַ����ͨ��syllablesPointer����Ķ�Ӧ��ֵ��ã�
//��ȡָ���±������
//#define EBIS_GET_SYLLABLE(eb, i)     ((eb)->syllables + (eb)->syllablesPointer[i]) 
//��ȡ��ǰѡ�������
//#define EBIS_GET_SEL_SYLLABLE(eb)    
//     ((eb)->syllableIndex == EB_NULL) ? NULL : EBIS_GET_SYLLABLE((eb), (eb)->syllableIndex - 1)
//����������꽫�����û���ȡĳһ���ض�����
    char                                    syllables[256] ;

    unsigned short                          components[EB_COMPONENT_MAX_LEN] ; //����
    unsigned char                           syllableLen ; //syllables��������
    unsigned char                           componentLen ; //components����
//ADD Date : 2003-10-27 15:44:38 by ABING
    unsigned char                           noUseSmartPunc;
    unsigned char                           noFirstKeyAssoc;
//2.0
//����������ڵĶ�Ӧsyllables����ͷ��ַ��ƫ��
    unsigned char                           syllablesPointer[EB_SYLLABLE_MAX_SELECT];
//���������ѡ�ִʵ���Ϣ����Ҫ�Ƕ�Ӧ���봮�ĳ��ȣ��Լ��ִʵĳ���
    CandInfo                                cis[EB_CANDIDATES_MAX_LEN];
//���²�������ΪEBHelper����ʹ�ã���Ҫ��Ϊ�˷������뷨�ļ���
//��ǰ���������봮
    char                                    fullInputString[EB_INPUT_MAX_LEN + 1] ; //�����봮[0~9,*]
    //selInfo��������Ч�ĵ�Ԫ������
    unsigned char                           selCount;
    //��ǰ������ַ���
    unsigned short                          fullOutputString[EB_INPUT_MAX_LEN + 1] ; //�����[]
//��ǰ�ĸ�����ѡ�ִʵ�ѡ����Ϣ
    CandSelInfo                             selInfo[EB_INPUT_MAX_LEN];
} EBInputStruct, *pEBInputStruct ;

typedef const EBInputStruct * pcEBInputStruct;

//��ȡָ���±������
#define EBIS_GET_SYLLABLE(eb, i)        ((eb)->syllables + (eb)->syllablesPointer[i])
//��ȡ��ǰѡ�������
#define EBIS_GET_SEL_SYLLABLE(eb)    \
    (((eb)->syllableIndex == EB_NULL) ? NULL : EBIS_GET_SYLLABLE((eb), (eb)->syllableIndex - 1))

//�������ڷ���棩��ȡָ���±��ע��
#define EBIS_GET_SYLLABLE_BPMF(eb, i)   ((const unsigned short *)EBIS_GET_SYLLABLE(eb, i))
//�������ڷ���棩��ȡ��ǰѡ���ע��
#define EBIS_GET_SEL_SYLLABLE_BPMF(eb)  ((const unsigned short *)EBIS_GET_SEL_SYLLABLE(eb))
//Copyһ����ѡ�ֵ�һ��unsigned short�������У�targetΪ�����顣
#define EBIS_COPY_CANDIDATE(target, eb, i)      \
    memcpy((target), ((eb)->candidates + (eb)->cis[i].start), (eb)->cis[i].len * sizeof(short));\
    (target)[(eb)->cis[i].len] = 0;
    

//INIT ...
#define EBISInit(pEBIS) \
        memset((void *)(pEBIS), EB_NULL, sizeof(EBInputStruct)) ;

#define EBCleanCandidates(pEBIS) \
    (pEBIS)->candidateLen = 0 ;\
    memset((void *)(pEBIS)->candidates, EB_NULL, sizeof((pEBIS)->candidates)) ;

#define EBCleanSyllables(pEBIS) \
    (pEBIS)->syllableLen = 0 ; \
    (pEBIS)->syllableIndex = 0 ; \
    memset((void *)(pEBIS)->syllables, EB_NULL, sizeof((pEBIS)->syllables)) ;

#define EBCleanComponents(pEBIS) \
    (pEBIS)->componentLen = 0 ;\
    memset((void *)(pEBIS)->components, EB_NULL, sizeof((pEBIS)->components)) ;

//Macro : EBIsValidInputMethod :    Is valid Input Method ?
#define EBIsValidInputMethod(im) \
        ((im) >= EB_INPUT_METHOD_PINYIN && \
         (im) <= EB_INPUT_METHOD_MAX)

//Macro : EBHaveInput :                Have Input ?
#define EBHaveInput(pEBIS) \
        ((pEBIS)->inputStringLen > 0 && \
         (pEBIS)->inputStringLen <= EB_INPUT_MAX_LEN)

//Macro : EBIsValidTone :            Is valid Tone ?
#define EBIsValidTone(pEBIS) \
         (((pEBIS)->tone >= EB_TONE_1 && \
         (pEBIS)->tone <= EB_TONE_6) || \
        (pEBIS)->tone == EB_NULL)

//Macro : EBCanSelectSyllables :    can select Syllables ?
#define EBCanSelectSyllable(pEBIS) \
        ((pEBIS)->syllableLen > 0 && \
         (pEBIS)->syllableLen <= EB_SYLLABLE_MAX_SELECT)

//Macro : EBHaveComponents :        have Components ?
#define EBHaveComponents(pEBIS) \
        ((pEBIS)->componentLen > 0 && \
         (pEBIS)->componentLen <= EB_COMPONENT_MAX_LEN)

//Macro : EBHaveCandidates :        have Candidates ?
#define EBHaveCandidates(pEBIS) \
        ((pEBIS)->candidateLen > 0 && \
         (pEBIS)->candidateLen <= EB_CANDIDATES_MAX_LEN)

//Macro : EBIsValidDesireLen :        Is valid candidate desire len ?
#define EBIsValidDesireLen(pEBIS) \
        ((pEBIS)->candidateDesireLen > 0 && \
         (pEBIS)->candidateDesireLen <= EB_CANDIDATES_MAX_LEN)

//Macro : EBIsValidSyllableIndex :    Is valid syllableIndex ?
#define EBIsValidSyllableIndex(pEBIS) \
        ((pEBIS)->syllableIndex > 0 && \
         (pEBIS)->syllableIndex <= (pEBIS)->syllableLen)

#endif    //__EB_STRUCTURES_H_
