#ifndef __GBAR_TRAN_H__
#define __GBAR_TRAN_H__

/************************************************************************/
/* 函数返回值的定义                                                     */
/************************************************************************/
#define GBT_NOT_ENOUGH_BUFFER           -1 //缓冲区不足
#define GBT_INVALID_ADDRESS             -2 //缓冲区头地址不符合要求
#define GBT_INVALIDATE_DATA             -3 //非法的语言数据
#define GBT_OK                          0  //成功

#define GBT_DEF_SECTION_DATA_SIZE       64
typedef unsigned long GBT_SECTION[GBT_DEF_SECTION_DATA_SIZE / sizeof(long)];

#ifdef __cplusplus
extern "C" {
#endif
        #if 0
    //param:
    //gbTranSectionData : 语言变型数据的当前状态信息, 此参数需要调用GBTranformInitSection初始化过
    //words         : [输入]arabic或者其他类arabic语言的原始unicode
    //presentation  : [输出]转化之后的arabic或者其他类arabic语言的显示unicode
    //bufferLen     : [输入]presentation缓冲区的长度，
    //return :
    //    如果空间不足则返回GBT_NOT_ENOUGH_BUFFER，否则返回presentation字符串的长度
    int GBTransform(
            const void * gbTranSectionData,
            const unsigned short * words,
            unsigned short * presentation,
            unsigned int presentationBufferLen);

    //param
    //gbTranSectionData : 语言变型数据的当前状态信息, 此参数需要调用GBTranformInitSection初始化过
    //presentation : [输入]arabic或者其他类arabic语言的显示unicode
    //words        : [输出]arabic的原始unicode，如果words为NULL则计算输入字符串转换为多少个原始字符
    //bufferLen    : [输入]words缓冲区的长度，当words为NULL的时候忽略此参数的
    //return
    //    如果空间不足则返回GBT_NOT_ENOUGH_BUFFER，否则返回word字符串的长度
    int GBTransformReverse(
            const void * gbTranSectionData,
            const unsigned short * presentation,
            unsigned short * words,
            unsigned int wordBufferLen);

    //变型初始化函数
    //param:
    //langTransformData : [输入]语言的变型数据
    //gbTranSectionData : [输出]变型数据信息，此参数，
    //len               : [输入]变型数据信息缓冲区的长度，以字节为单位
    //返回值：
    //当gbTranSectionData为NULL或者len为0的时候，返回gbTranSectionData需要多大的空间
    //当gbTranSectionData不为NULL而且len大于0的时候，返回初始化的结果，GBT_OK(0)为成功，
    //其他为失败。
    //GBT_NOT_ENOUGH_BUFFER  : 缓冲区不足。
    //GBT_INVALID_ADDRESS    : langTransformData或者gbTranSectionData指针不被4整除
    //GBT_INVALIDATE_DATA    : langTransformData数据非法
    int GBTranformInitSection(const void * langTransformData,
        void * gbTranSectionData,
        size_t len);
        
        #endif
#ifdef __cplusplus
}
#endif

#endif
