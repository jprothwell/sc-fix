#ifndef __GBAR_TRAN_H__
#define __GBAR_TRAN_H__

/************************************************************************/
/* ��������ֵ�Ķ���                                                     */
/************************************************************************/
#define GBT_NOT_ENOUGH_BUFFER           -1 //����������
#define GBT_INVALID_ADDRESS             -2 //������ͷ��ַ������Ҫ��
#define GBT_INVALIDATE_DATA             -3 //�Ƿ�����������
#define GBT_OK                          0  //�ɹ�

#define GBT_DEF_SECTION_DATA_SIZE       64
typedef unsigned long GBT_SECTION[GBT_DEF_SECTION_DATA_SIZE / sizeof(long)];

#ifdef __cplusplus
extern "C" {
#endif
        #if 0
    //param:
    //gbTranSectionData : ���Ա������ݵĵ�ǰ״̬��Ϣ, �˲�����Ҫ����GBTranformInitSection��ʼ����
    //words         : [����]arabic����������arabic���Ե�ԭʼunicode
    //presentation  : [���]ת��֮���arabic����������arabic���Ե���ʾunicode
    //bufferLen     : [����]presentation�������ĳ��ȣ�
    //return :
    //    ����ռ䲻���򷵻�GBT_NOT_ENOUGH_BUFFER�����򷵻�presentation�ַ����ĳ���
    int GBTransform(
            const void * gbTranSectionData,
            const unsigned short * words,
            unsigned short * presentation,
            unsigned int presentationBufferLen);

    //param
    //gbTranSectionData : ���Ա������ݵĵ�ǰ״̬��Ϣ, �˲�����Ҫ����GBTranformInitSection��ʼ����
    //presentation : [����]arabic����������arabic���Ե���ʾunicode
    //words        : [���]arabic��ԭʼunicode�����wordsΪNULL����������ַ���ת��Ϊ���ٸ�ԭʼ�ַ�
    //bufferLen    : [����]words�������ĳ��ȣ���wordsΪNULL��ʱ����Դ˲�����
    //return
    //    ����ռ䲻���򷵻�GBT_NOT_ENOUGH_BUFFER�����򷵻�word�ַ����ĳ���
    int GBTransformReverse(
            const void * gbTranSectionData,
            const unsigned short * presentation,
            unsigned short * words,
            unsigned int wordBufferLen);

    //���ͳ�ʼ������
    //param:
    //langTransformData : [����]���Եı�������
    //gbTranSectionData : [���]����������Ϣ���˲�����
    //len               : [����]����������Ϣ�������ĳ��ȣ����ֽ�Ϊ��λ
    //����ֵ��
    //��gbTranSectionDataΪNULL����lenΪ0��ʱ�򣬷���gbTranSectionData��Ҫ���Ŀռ�
    //��gbTranSectionData��ΪNULL����len����0��ʱ�򣬷��س�ʼ���Ľ����GBT_OK(0)Ϊ�ɹ���
    //����Ϊʧ�ܡ�
    //GBT_NOT_ENOUGH_BUFFER  : ���������㡣
    //GBT_INVALID_ADDRESS    : langTransformData����gbTranSectionDataָ�벻��4����
    //GBT_INVALIDATE_DATA    : langTransformData���ݷǷ�
    int GBTranformInitSection(const void * langTransformData,
        void * gbTranSectionData,
        size_t len);
        
        #endif
#ifdef __cplusplus
}
#endif

#endif
