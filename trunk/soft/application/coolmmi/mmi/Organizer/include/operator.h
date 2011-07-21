/**********************************************************************************
	Filename:		Operator.h
	Author:			hansl
	Date Created:	November-1-2006
	Contains:		Operator
**********************************************************************************/
#ifndef _MMI_OPERATOR_H
#define _MMI_OPERATOR_H
#include "mmi_features.h"
#include "mmi_data_types.h"
#include "cswtype.h"
//#ifdef _MMI_OPERATOR_


#define	OPERATOR_LEN	16
// 数值的正负
#define	OPERATORSYMBOL_POST	1
#define	OPERATORSYMBOL_NEGA	-1

typedef struct tagCALOPERATOR_STRUCT
{
	U8	operBuff[OPERATOR_LEN + 1];		// 数值
	S8	Symbol;							// 符号
	S8	Exponent;						// 指数的长度
	S8	Fraction;							// 小数的长度
}CALOPERATOR_STRUCT;

extern void CalculatorAligDot(CALOPERATOR_STRUCT *operator1,CALOPERATOR_STRUCT *operator2 );
extern S8	CalCulatorLeftMov( U8 *stringBuff,U8 movCnt);
extern S8	CalCulatorRightMov( U8 *stringBuff,U8 movCnt);
extern S8   CalculatorCmpOperator(U8 *string1,U8 *string2);
extern U8	CalculatorGetOperator(S8 *pOperatorString,U8 *stringCnt,CALOPERATOR_STRUCT *Aoperator);
extern U8	CalculatorOperatorsFormat2(CALOPERATOR_STRUCT *Aoperator);							
extern void	CalculatorGetFractionLen(CALOPERATOR_STRUCT *Aoperator);	
extern void	CalculatorAdd(U8 *operator1,U8 *operator2);
extern void	CalculatorSub(U8 *operator1,U8 *operator2);
extern void	CalculatorMult(CALOPERATOR_STRUCT *operator1, CALOPERATOR_STRUCT *operator2 );
extern BOOL CalculatorDivi(CALOPERATOR_STRUCT *operator1, CALOPERATOR_STRUCT *operator2 );
extern void   Calresulttoberound(CALOPERATOR_STRUCT *result);
extern void Calgetrealresult(CALOPERATOR_STRUCT *resultbuf, CALOPERATOR_STRUCT *result);
extern U8 CalcAdjustResult1(CALOPERATOR_STRUCT *Aoperator);
//#endif


#endif

