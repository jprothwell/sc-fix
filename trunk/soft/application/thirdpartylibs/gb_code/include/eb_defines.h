//Last Modify Date : 2003-5-6 17:02:29
//:EB_defines.h : ErBiDigital public defines.

#ifndef __EB_DEFINES_H_
#define __EB_DEFINES_H_

//
#define EB_NULL						0 //��

#define EB_TRUE						1
#define EB_FALSE					0

//Returns
#define EB_OK						0  //OK���ɹ�
#define EB_EMPTY					100 //û�з��������ĺ�ѡ��[��]
#define EB_FAULT_TOLERANT           101 //�ݴ�ƥ��
#define EB_FAILED					-1 //ʧ��

#define EB_NOTINIT					-2 //û�е���EBStartup()!
#define EB_PEBIS_NULL				-3 //pEBISΪ��.
#define EB_METHOD_INVALID			-4 //���뷨���ʹ���
#define EB_DESIRE_LEN_INVALID		-5 //�û���Ҫ�ĳ��ȴ���.
#define EB_KEY_INVALID				-6 //����EBTestKeyʱ�İ������Ϸ�
#define EB_STATUS_ERROR				-7 //pEBIS->status����ȷ�����ܵ���.
#define EB_ASSOC_ISN_ERROR			-8 //AssociateISN == EB_NULL

/*Invalidate�������صĴ���ֵ*/
#define EB_INVALID_CHAR             -9 //�Ƿ����ַ�
#define EB_INVALID_TONE             -10 //�Ƿ�������
#define EB_INVALID_INPUT_LEN        -11 //���봮�ĳ��ȹ���
#define EB_FIRST_IS_WILDCARD        -12 //��һ��������ַ���ͨ���
#define EB_NO_MATCH_WORD            -13 //û��ƥ�����
#define EB_NO_MATCH_COMPONENT       -14 //û��ƥ��Ĳ���
#define EB_INVALID_SYLLABLE         -15 //�Ƿ�������
#define EB_INVALID_SYLLABLE_INDEX   -16 //�Ƿ��������±�
#define EB_INPUT_STRING_NULL        -17 //���봮Ϊ��
/*Starpup�������صĴ���*/
#define EB_DATA_FILE_NOT_FOUND      -18 //�Ҳ��������ļ�
#define EB_INVALID_DATA_FORMAT      -19 //�Ƿ������ݸ�ʽ
#define EB_FAILED_TO_ALLOC_MEMORY   -20 //
#define EB_MISS_DATA                -21 //ȱ������
#define EB_STROKE_LEN_MISMATCH      -22 //����ıʻ��������ݵıʻ�����һ��
#define EB_INVALID_SYLLLABLES_COUNT -23
#define EB_SH_BUFFER_TO_SMALL       -24 //����EBShDataInit��ʱ��, �������ĳ��Ȳ���
#define EB_SH_CORRUPT_DATA          -25 //��������������
#define EB_SH_INVALID_ADDRESS       -26 //��������ݵ�ͷ��ַ���ܱ�4����
//#define EB_SH_PHRASE_EXISTS         -27 //EBShCheckPhrase��һ�����ܷ���ֵ��
#define EB_SH_NOT_ENOUGH_SPACE      -28
//#define EB_SH_PHRASE_EXISTS_HEADER  -29 //EBShAddPhrase��һ�����ܷ���ֵ��
#define EB_SH_NOT_INIT              -30 //����ʺʹ����Ƶ����û�г�ʼ��
//#define EB_SH_WORD_EXISTS           -31 //������Ѿ��ڴʱ��д���
#define EB_SH_INVALID_WORD          -32 //����������а����Ƿ�����ĸ
#define EB_SH_SPACE_NOT_ENOUGH      -33 //�ռ䲻��
#define EB_SH_DUPLICATE             -34 //�����Ѿ�����һ����ȫ��ͬ�ĵ���
#define EB_SH_PHRASE_INVALID_LENGTH -35 //�����̫���ˣ�������10������
#define EB_TO_INPUT_PHRASE          -36 

#define EB_DIALECT_DATA_MISSMATCH	-37 //�������������������ݲ�ƥ��
#define EB_INVALID_SYLLABLE_TABLE	-38	//�����ƴ�������ݣ���Ҫ��ƴ����ʽ����������ʹ��

#define EB_ERR_CLOSE_PHRASE_FUNC	-39 //���鹦���Ѿ��ر�, ��EBFindFirstPageC���淵�أ�

#define EB_MIN_SH_DATA_LEN          5120 //ʹ�ô����Ƶ������ʹ��ܵ���С�ռ�

#define FOUND_EXACTLY               1 //
#define FOUND_PART                  2
#define FOUND_NONE                  0


//Engine���ܴ���İ���[for EBTestKey]
#define EB_KEYS					"0123456789#"
#define EB_PY_KEYS				"0123456789#" //��


//EBInputStruct Constant
#define EB_INPUT_METHOD_NUM			6 //���뷨����
#define EB_INPUT_METHOD_PINYIN      1 //ƴ��
#define EB_INPUT_METHOD_STROKE      2 //�ʻ�
#define EB_INPUT_METHOD_YIN_STROKE  3 //�ʻ�����
#define EB_INPUT_METHOD_CANGJIE		4
#define EB_INPUT_METHOD_BPMF        5 //ע��
#define EB_INPUT_METHOD_JYUTPING    6 //�㶫ƴ��

#define EB_INPUT_METHOD_MAX			6

#define EB_INPUT_MAX_LEN	 		32 //������볤��

#define	EB_TONE_1					1 //����: 
#define EB_TONE_2					2 //����: 
#define	EB_TONE_3					3 //����: 
#define EB_TONE_4					4 //����: 
#define	EB_TONE_5					5 //����: ��
#define EB_TONE_6					6 //����: ��������

#define EB_CANDIDATES_MAX_LEN		128 //��ѡ�������Ŀ

#define EB_SYLLABLE_MAX_SELECT		32 //ͬһ������ܵ�׼ȷ�����������
#define EB_SYLLABLE_MAX_LEN			12 //������󳤶�

#define EB_COMPONENT_MAX_LEN		5 //������ʾ��Ŀ

#ifndef EB_PHRASE_INPUT_SPLIT
#define EB_PHRASE_INPUT_SPLIT       '#'
#endif

#ifndef EB_WILDCARD
#if EB_PHRASE_INPUT_SPLIT == '#'
#define EB_WILDCARD				'*'
#else
#define EB_WILDCARD				'#'
#endif
#endif

#define EB_CACHE_ACTION_ALLOC       1
#define EB_CACHE_ACTION_FREE        2

#define EB_STATUS_ASSOCIATE			0x00000001 //AssociateMode
#define EB_STATUS_INTERPUNCTION	    0x00000002 //interpunction mode
#define EB_STATUS_PAGEDOWN			0x00000004 //PageDown
#define EB_STATUS_PAGEUP			0x00000008 //PageUp

#endif	//__EB_DEFINES_H_

