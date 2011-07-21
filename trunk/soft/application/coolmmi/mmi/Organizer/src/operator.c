/**********************************************************************************
	Filename:		Operator.c
	Author:			hansl
	Date Created:	November-1-2006
	Contains:		Operator
**********************************************************************************/
#ifndef _MMI_OPERATOR_C
#define _MMI_OPERATOR_C
#include "mmi_features.h"
#include "mmi_data_types.h"
#include "operator.h"
#include "cswtype.h"
#include "mmi_trace.h"
#include "ts.h"
//#include "cswtype.h" 

/********************************************************
	
 �ַ�����ʼ�ַ��ƶ������������
 �����==0ʱ,���ؽ��0,���򷵻�1
********************************************************/
extern void* memset(void *dst0,int c0,size_t length);
extern int memcmp(const void* s1,const void* s2,size_t n);

U8 CalcAdjustResult1(CALOPERATOR_STRUCT *Aoperator)
{
	U8 cnt1;
	U8 CompareOper[17];

	ASSERT(Aoperator != NULL);

	memset((U8*)CompareOper, 0, 17);

	if (memcmp((U8*)CompareOper, (U8*)Aoperator->operBuff, 17) == 0)
	{
		Aoperator->Exponent = 1;
		Aoperator->Symbol = 1;
		Aoperator->Fraction = 0;
		return 0;
	}
		
	// �õ��������ֵĳ���
	for (cnt1 = 0;cnt1 < Aoperator->Exponent;cnt1++)
	{
		if (Aoperator->operBuff[cnt1]!=0)
		{
			break;
		}
	}
	
	Aoperator->Exponent = Aoperator->Exponent - cnt1 + 1;	
	// �ƶ�
	CalCulatorLeftMov(Aoperator->operBuff, cnt1);	
	
	// �õ�С�����ֵĳ���
	cnt1 = (Aoperator->Exponent + Aoperator->Fraction) - 1;
	for ( ; cnt1 >= Aoperator->Exponent + 1; cnt1--)
	{
		if (Aoperator->operBuff[cnt1] != 0)
		{
			break;
		}
	}
	
	Aoperator->Fraction = cnt1 + 1 - Aoperator->Exponent;		
	// ������Ƿ����0
	if ( Aoperator->Exponent == 0  &&  Aoperator->Fraction == 0 )
	{
		Aoperator->Exponent = 1;
		Aoperator->Symbol = 1;
		Aoperator->Fraction = 0;
		return	0;
	}
	
	mmi_trace(g_sw_ORGANIZER, "Aoperator->Exponent   = %d", Aoperator->Exponent );
	mmi_trace(g_sw_ORGANIZER, "Aoperator->Fraction   = %d", Aoperator->Fraction );
	return	1;

}

/********************************************************
	
 С������벢���ַ������ƶ�һ���ֽ�
 
********************************************************/
void CalculatorAligDot(CALOPERATOR_STRUCT *operator1, CALOPERATOR_STRUCT *operator2 )
{
	U8 Cnt;

	ASSERT(operator1 != NULL && operator2 != NULL);

	if ( operator1->Exponent == operator2->Exponent )
	{
		CalCulatorRightMov( operator1->operBuff, 1 );
		CalCulatorRightMov( operator2->operBuff, 1 );
	}
	else if (operator1->Exponent>operator2->Exponent)
	{
		CalCulatorRightMov( operator1->operBuff, 1 );
		
		Cnt = operator1->Exponent - operator2->Exponent;
		CalCulatorRightMov( operator2->operBuff, Cnt + 1 );
		operator2->Exponent = operator1->Exponent;
	}
	else
	{
		CalCulatorRightMov( operator2->operBuff, 1 );
		
		Cnt = operator2->Exponent - operator1->Exponent;
		CalCulatorRightMov( operator1->operBuff, Cnt + 1 );
		operator1->Exponent = operator2->Exponent;
	}
}

/********************************************************
	
 �ַ������Ƽ����ֽ�
********************************************************/
S8 CalCulatorLeftMov( U8 *stringBuff,U8 movCnt)
{
	U8 objectCnt;
	
	ASSERT(stringBuff != NULL);
	
	if ( movCnt == 0 )
	{
		return 1;
	}
	
	objectCnt = 0;
	for ( ; movCnt < OPERATOR_LEN; movCnt++, objectCnt++ )
	{
		stringBuff[objectCnt] = stringBuff[movCnt];
	}
	
	stringBuff[objectCnt] = 0x0;
	objectCnt++;

	for (; objectCnt < OPERATOR_LEN -1; objectCnt++)
	{
		stringBuff[objectCnt] = 0x0;
	}
	
	return 0;
}

/********************************************************
	
 �ַ������Ƽ����ֽ�
********************************************************/
S8 CalCulatorRightMov( U8 *stringBuff, U8 movCnt)
{
	U8 objectCnt;

	ASSERT(stringBuff != NULL);
	
	if ( movCnt == 0 )
	{
		return 1;
	}
	
	objectCnt = OPERATOR_LEN ;
	movCnt = OPERATOR_LEN -movCnt;

	for ( ; movCnt > 0; movCnt--, objectCnt-- )
	{
		stringBuff[objectCnt] = stringBuff[movCnt];
	}
	
	stringBuff[objectCnt] = stringBuff[movCnt];
	objectCnt--;	
	for (;objectCnt > 0;objectCnt--)
	{
		stringBuff[objectCnt] = 0x0;
	}	
	stringBuff[objectCnt] = 0x0;	

	return 0;
}

/********************************************************
	
 �ַ����Ƚϴ�С
********************************************************/
S8 CalculatorCmpOperator(U8 *string1, U8 *string2)
{
	U8 cnt;

	ASSERT(string1 != NULL && string2 != NULL);
	
	for (cnt = 0; cnt <OPERATOR_LEN ; cnt++)
	{
		if ( string1[cnt] == string2[cnt] )
		{
			continue;
		}	
		else if ( string1[cnt] > string2[cnt] )
		{
			return	1;
		}
		else
		{
			return	-1;
		}
	}
	
	return	0;
}

/********************************************************
	
��������
********************************************************/
void   Calresulttoberound(CALOPERATOR_STRUCT *result)
{
	U8 stringCnt;
	S32 cnt;
//qiff modify for bug:10673
	S32 enhance_flag=0;
	U32 num_cnt = 0;
	ASSERT(result != NULL);
	
	stringCnt = 0;
	if (result->Fraction != 0) 
	{
		stringCnt++;
	}
	if(result->Exponent < 0)
	{
		stringCnt++;
	}
//qiff modify for bug:10673
	num_cnt = result->Fraction + result->Exponent ;
	
	mmi_trace(1,"Calresulttoberound stringCnt %d",stringCnt);
	mmi_trace(1,"Calresulttoberound result->Fraction %d",result->Fraction);
	mmi_trace(1,"Calresulttoberound result->Exponent  %d",result->Exponent );
	if(num_cnt  > (OPERATOR_LEN -1-stringCnt ))//need >
	{
		result->Fraction = OPERATOR_LEN -stringCnt -1 - result->Exponent  ;
		num_cnt = OPERATOR_LEN -stringCnt;
		mmi_trace(1,"Calresulttoberound num_cnt %d",num_cnt);
		mmi_trace(1,"Calresulttoberound num_cnt result->operBuff[%d] = %d",num_cnt-1,result->operBuff[num_cnt-1]);
		if(result->operBuff[num_cnt-1] >= 5)
		{
			enhance_flag = 1;
			for( cnt = num_cnt -2 ; cnt >=0 ; cnt--)
			{
				if((cnt == 0)&&(result->operBuff[cnt]==9))
				{
					//do a shift
					mmi_trace(1,"do a shift");
					for(cnt = num_cnt; cnt >= 2;cnt--)
					{
						result->operBuff[cnt-1] = result->operBuff[cnt-2] ;
					}
					result->operBuff[0] = 1; 
					result->Exponent++;
					result->Fraction--;
					enhance_flag = 0;
				}
				else
				{
					if( result->operBuff[cnt-1] == 9 )
					{
						result->operBuff[cnt -1] = 0;
					}
					else
					{
						result->operBuff[cnt-1] = result->operBuff[cnt -1]+1;
						enhance_flag = 0;
					}
				}
				mmi_trace(1,"enhance_flag num_cnt  %d",num_cnt);
				mmi_trace(1,"enhance_flag cnt  %d",cnt);
				mmi_trace(1,"enhance_flag %d",enhance_flag);
				if(enhance_flag == 0)
					break;
			}
		}
	}
	else
	{
		mmi_trace(1,"Calresulttoberound num_cnt %d",num_cnt);
		mmi_trace(1,"Calresulttoberound num_cnt result->operBuff[%d] = %d",num_cnt-1,result->operBuff[num_cnt-1]);
		if(result->Exponent<0)
		{
		    	num_cnt=6;
			if(result->operBuff[num_cnt] >= 5)
			{
				enhance_flag = 1;
				for( cnt = num_cnt ; cnt >=0 ; cnt--)
				{
					if((cnt == 0)&&(result->operBuff[cnt]==9))
					{
						//do a shift
						mmi_trace(1,"do a shift");
						for(cnt = num_cnt; cnt >= 2;cnt--)
						{
							result->operBuff[cnt-1] = result->operBuff[cnt-2] ;
						}
						result->operBuff[0] = 1; 
						result->Exponent++;
						result->Fraction--;
						enhance_flag = 0;
					}
					else
					{
						if( result->operBuff[cnt-1] == 9 )
						{
							result->operBuff[cnt-1] = 0;
						}
						else
						{
							result->operBuff[cnt-1] = result->operBuff[cnt-1]+1;
							enhance_flag = 0;
						}
					}
					mmi_trace(1,"enhance_flag num_cnt  %d",num_cnt);
					mmi_trace(1,"enhance_flag cnt  %d",cnt);
					mmi_trace(1,"enhance_flag %d",enhance_flag);
					if(enhance_flag == 0)
						break;
				}
			}
		}
		else if(result->Exponent==0)
		{
			if(result->operBuff[num_cnt] >= 5)
			{
				enhance_flag = 1;
				for( cnt = num_cnt ; cnt >=0 ; cnt--)
				{
					if((cnt == 0)&&(result->operBuff[cnt]==9))
					{
						//do a shift
						mmi_trace(1,"do a shift");
						for(cnt = num_cnt; cnt >= 2;cnt--)
						{
							result->operBuff[cnt-1] = result->operBuff[cnt-2] ;
						}
						result->operBuff[0] = 1; 
						result->Exponent++;
						result->Fraction--;
						enhance_flag = 0;
					}
					else
					{
						if( result->operBuff[cnt-1] == 9 )
						{
							result->operBuff[cnt-1] = 0;
						}
						else
						{
							result->operBuff[cnt-1] = result->operBuff[cnt-1]+1;
							enhance_flag = 0;
						}
					}
					mmi_trace(1,"enhance_flag num_cnt  %d",num_cnt);
					mmi_trace(1,"enhance_flag cnt  %d",cnt);
					mmi_trace(1,"enhance_flag %d",enhance_flag);
					if(enhance_flag == 0)
						break;
				}
			}
		}
		else
		{
		       if(num_cnt >= 7)
		       {
				if(result->operBuff[num_cnt] >= 5)
				{
					enhance_flag = 1;
					for( cnt = num_cnt ; cnt >=0 ; cnt--)
					{
						if((cnt == 0)&&(result->operBuff[cnt]==9))
						{
							//do a shift
							mmi_trace(1,"do a shift");
							for(cnt = num_cnt; cnt >= 2;cnt--)
							{
								result->operBuff[cnt-1] = result->operBuff[cnt-2] ;
							}
							result->operBuff[0] = 1; 
							result->Exponent++;
							result->Fraction--;
							enhance_flag = 0;
						}
						else
						{
							if( result->operBuff[cnt-1] == 9 )
							{
								result->operBuff[cnt-1] = 0;
							}
							else
							{
								result->operBuff[cnt-1] = result->operBuff[cnt-1]+1;
								enhance_flag = 0;
							}
						}
						mmi_trace(1,"enhance_flag num_cnt  %d",num_cnt);
						mmi_trace(1,"enhance_flag cnt  %d",cnt);
						mmi_trace(1,"enhance_flag %d",enhance_flag);
						if(enhance_flag == 0)
							break;
					}
				}
			}
		}
	}
	#if 0
	for(tmp_cnt=0;tmp_cnt<result->Fraction+result->Exponent  ;tmp_cnt++)
	{
		mmi_trace(1,"Calresulttoberound result[%d] = %d",tmp_cnt,result.operBuff[tmp_cnt]);
	}
	if (result->operBuff[8-stringCnt] >= 5)  
	{
		for (cnt = 8 - stringCnt - 1; cnt >= 0; cnt--)
		{
			if (result->operBuff[cnt] == 9)
			{
				result->operBuff[cnt] = 0;
				/*dongwg Add Start For 5941 Ver: TBM780  on 2007-8-1 13:30 when */
				if((cnt == 0)&&(result->Exponent>0)&&(result->Fraction>=0))		
				{
					result->operBuff[0] = 1;
					result->Exponent++;
					result->Fraction--;
				}
				/*dongwg Add End  For 5941 Ver: TBM780  on 2007-8-1 13:30 */
			}
			else
			{
				result->operBuff[cnt]++;
				break;
			}
		}
	} 

	/*dongwg Add Start For 6118 Ver: TBM780  on 2007-8-7 19:49 */
	if(stringCnt != 0)
	{
		if(result->Exponent<0)
			result->operBuff[6] = 0;
		else
			result->operBuff[7] = 0;
	}
	/*dongwg Add End  For 6118 Ver: TBM780  on 2007-8-7 19:49 */
	else
		result->operBuff[8] = 0;	
	/*dongwg Add Start For 5940 Ver: TBM780  on 2007-8-1 21:18*/
	if(result->Fraction > 0)
	{
		if(result->Exponent > 0)
		{
			for(cnt = result->Exponent + result->Fraction-1; cnt > result->Exponent-1; cnt--)
			{
				if(result->operBuff[cnt] == 0)
					result->Fraction--;
				else
					break;
			}
		}
		else
		{
			if(result->Fraction > 7)
				result->Fraction = 7;
			for(cnt = result->Fraction-1; cnt >= 0; cnt--)
			{
				if(result->operBuff[cnt] == 0)
					result->Fraction--;
				else
					break;
			}
		}	
		if(result->Fraction > 6)
			result->Fraction = 6;
		if(result->Fraction < 0)
			result->Fraction = 0;
	}
	/*dongwg Add End  For 5940 Ver: TBM780  on 2007-8-1 21:18 */
	#endif
//qiff modify for bug:10673 end
}
/********************************************************
	
 �õ�����Ԫ
********************************************************/
U8 CalculatorGetOperator(S8 *pOperatorString, U8 *stringCnt, CALOPERATOR_STRUCT *Aoperator)								 								
{
	U8	cnt = 0, dotFlag = 0;

	 ASSERT(pOperatorString != NULL && stringCnt != NULL && Aoperator != NULL);
	 
	if (pOperatorString[*stringCnt] == '\0')
	{
		return	FALSE;
	}
	
	memset(Aoperator->operBuff, 0, OPERATOR_LEN + 1);
	for (Aoperator->Exponent = 0,Aoperator->Fraction = 0; ;(*stringCnt)++, cnt++)
	{
		if (pOperatorString[*stringCnt] == '\0')
		{
			Aoperator->operBuff[cnt] = '\0';
			break;
		}
			
		if (pOperatorString[*stringCnt] == '.')
		{
			dotFlag = 1;
			cnt--;
			continue;
		}
		
		if (dotFlag == 0)
		{
			Aoperator->Exponent++;
		}
		else
		{
			Aoperator->Fraction++;
		}
		Aoperator->operBuff[cnt] = pOperatorString[*stringCnt] - 0x30;
	}
	
	return	TRUE;
	
}

/********************************************************
 Function : 	CalculatorOperatorsFormat2
 Description : ��ʽ������Ԫ,ʹ���Ϊָ����ʽ
 Input : 
 		 Aoperator
 Ouput : 
 		 UINT8 : �������ͷ���ֵ,���Ϊ0,��ʾ������ԪΪ0; ���Ϊ1,��Ϊ0
 		 Aoperator : ��ʽ��֮�������Ԫ
 
********************************************************/
U8 CalculatorOperatorsFormat2(CALOPERATOR_STRUCT *Aoperator)
{
	U8 cnt1, cnt2, cnt3;

	ASSERT(Aoperator != NULL);
	
	for(cnt1 = 0; cnt1 < (Aoperator->Exponent + Aoperator->Fraction) ; cnt1++)
	{
		if ( Aoperator->operBuff[cnt1] != 0 )
		{
			break;
		}
	}
	
	if (cnt1 == (Aoperator->Exponent + Aoperator->Fraction))
	{
		return 0;
	}
	
	cnt3 = cnt1;
	
	for (cnt2 = 0; cnt1 < (Aoperator->Exponent + Aoperator->Fraction); cnt1++, cnt2++ )
	{
		Aoperator->operBuff[cnt2] = Aoperator->operBuff[cnt1];
	}

	for ( ; cnt2 < (Aoperator->Exponent + Aoperator->Fraction); cnt2++)
	{
		Aoperator->operBuff[cnt2] = 0;
	}
	
	Aoperator->Exponent = Aoperator->Exponent - cnt3;

	return 1;
}

/********************************************************
	
 �õ�ʵ�ʵ�С�����ֳ���(Ϊ�˼ӿ������ٶ�)
********************************************************/
void	CalculatorGetFractionLen(CALOPERATOR_STRUCT *Aoperator)
{
	/*dongwg Modify Start For Ver: TBM780  on 2007-8-20 20:24 */
	S32 cnt;
	//U8 cnt;
	/*dongwg Modify End  For Ver: TBM780  on 2007-8-20 20:24 */

	ASSERT(Aoperator != NULL);
	
	cnt = (Aoperator->Exponent + Aoperator->Fraction) - 1;
	for ( ; cnt >= Aoperator->Exponent; cnt--)
	{
		if (Aoperator->operBuff[cnt]!=0)
		{
			break;
		}
	}
	
	Aoperator->Fraction = cnt + 1 - Aoperator->Exponent;
}

/********************************************************
	
 �õ�ʵ�ʵ�С�����ֳ���(Ϊ�˼ӿ������ٶ�)
********************************************************/
void Calgetrealresult(CALOPERATOR_STRUCT *resultbuf, CALOPERATOR_STRUCT *result)
{
	U8 stringCnt = 0, operator1Cnt;
	
	ASSERT(resultbuf != NULL && result != NULL);	

	// �õ����Ų���
	if(result->Symbol == OPERATORSYMBOL_NEGA)
	{
		if(result->Fraction == 0 && result->operBuff[0] == 0)  //�����ʾ"-0"��bug
		{
			if(result->Exponent == 1)
				stringCnt = 0;
			/*dongwg Add Start For Ver: TBM780  on 2007-8-17 16:28 */
			else 
			{
				if(result->Exponent <= 0)
				{
					stringCnt = 1;
					resultbuf->operBuff[0] = '0';
				}
			}
			/*dongwg Add End  For Ver: TBM780  on 2007-8-17 16:28 */
		}
		else
		{
			if (result->Exponent <= 0)    //�������0 �����ʾΪС�����bug  added by hansl
			{
				stringCnt = 2;
				resultbuf->operBuff[0] = '-';
				resultbuf->operBuff[1] = '0';
			}
			else
			{
				stringCnt = 1;
				resultbuf->operBuff[0] = '-';
			}
			
		}		
	}
	else
	{
		if (result->Exponent <= 0)    //�������0 �����ʾΪС�����bug  added by hansl
		{
			stringCnt = 1;
			resultbuf->operBuff[0] = '0';
		}
		else
		stringCnt = 0;
	}
	
	// �õ���������
	for( operator1Cnt = 0; operator1Cnt < result->Exponent; stringCnt++, operator1Cnt++)
	{
		resultbuf->operBuff[stringCnt] = result->operBuff[operator1Cnt] + 0x30;
	}
	
	// �õ�С������
	if (result->Fraction!=0)
	{
		resultbuf->operBuff[stringCnt] = '.';
		stringCnt++;
		//operator1Cnt++;
		if (result->Exponent >= 0)
		{
		for ( ; operator1Cnt < (result->Exponent + result->Fraction) ; stringCnt++, operator1Cnt++)
		resultbuf->operBuff[stringCnt] = result->operBuff[operator1Cnt] + 0x30;
		}
		else
		{			
			U8 i;
			i = stringCnt;
			
			for (; operator1Cnt < result->Fraction; stringCnt++, operator1Cnt++)
				resultbuf->operBuff[stringCnt ] = result->operBuff[operator1Cnt ] + 0x30;
		}				
	}
	
	resultbuf->operBuff[stringCnt] = '\0';	
	
}
/********************************************************
	
 �ӷ����㣬��������� operator1
********************************************************/
void	CalculatorAdd(U8 *operator1,U8 *operator2)
{
	U8 cnt;

	ASSERT(operator1 != NULL && operator2 != NULL);

		
	for (cnt = OPERATOR_LEN - 1; cnt > 0;cnt--)
	{
		operator1[cnt] = operator1[cnt] + operator2[cnt];
		if ( operator1[cnt] >= 10 )
		{
			operator1[cnt] = operator1[cnt] - 10;
			operator1[cnt-1]++;
		}
	}

	operator1[0] = operator1[0] + operator2[0];
	
}


/********************************************************
	
�������㣬 ��������� operator1
********************************************************/
void	CalculatorSub(U8 *operator1, U8 *operator2)
{
	U8 cnt = 0, carry = 0;
		
	for ( cnt = OPERATOR_LEN - 1; cnt > 0; cnt-- )
	{
		if ( operator1[cnt] >= (operator2[cnt] + carry) )
		{
			operator1[cnt] = operator1[cnt] - operator2[cnt] - carry;
			carry = 0;
		}
		else
		{
			operator1[cnt] = operator1[cnt] + 10 - operator2[cnt] - carry;
			carry = 1;
		}
	}
	
	operator1[0] = operator1[0] - carry;
	
}

/********************************************************
	
 �˷����㣬��������� operator1 �� operator1Symbol
********************************************************/
void	CalculatorMult(CALOPERATOR_STRUCT *operator1, CALOPERATOR_STRUCT *operator2 )
{	
	U8 result[OPERATOR_LEN + 1];
	S32 cnt1, cnt2;

	ASSERT(operator1 != NULL && operator2 != NULL);
	
	// �õ�ʵ�ʵ�С�����ֳ���(Ϊ�˼ӿ������ٶ�)
	CalculatorGetFractionLen(operator1);
	CalculatorGetFractionLen(operator2);
	
	// ����Ч������̵�������Ϊ����
	if ( (operator1->Exponent + operator1->Fraction) < (operator2->Exponent + operator2->Fraction) )
	{
		for( cnt1 = 0; cnt1 < OPERATOR_LEN-1; cnt1++ )
		{
			result[cnt1] = operator1->operBuff[cnt1];		
			operator1->operBuff[cnt1] = operator2->operBuff[cnt1];
			operator2->operBuff[cnt1] = result[cnt1];
		}
		
		cnt1 = operator1->Exponent;
		operator1->Exponent = operator2->Exponent;
		operator2->Exponent = cnt1;
		cnt1 = operator1->Fraction;
		operator1->Fraction = operator2->Fraction;
		operator2->Fraction = cnt1;
		
	}
	
	// �ѱ�����operator1�ƶ������ұ�
	cnt1 = OPERATOR_LEN - operator1->Exponent - operator1->Fraction;
	CalCulatorRightMov( operator1->operBuff, cnt1);
	// �õ��˻�
	memset(result, 0, OPERATOR_LEN + 1);
	cnt1 = operator2->Exponent + operator2->Fraction - 1;
	
	while ( 1 )
	{		
		
		for ( cnt2 = operator2->operBuff[cnt1]; cnt2 > 0; cnt2-- )
		{
			CalculatorAdd(result, operator1->operBuff);
		}
		
		cnt1--;		
		
		if ( cnt1 >= 0 )
		{
			CalCulatorLeftMov( operator1->operBuff,1);
		}		
		else
		{
			break;
		}		
	}
	// �õ�����,ָ���Լ�С������
	operator1->Symbol = operator1->Symbol*operator2->Symbol;
	operator1->Fraction = operator1->Fraction + operator2->Fraction;
	
	
	for ( cnt1 = 0; cnt1 < OPERATOR_LEN - operator1->Fraction ; cnt1++ )  //С����ǰ��λΪ������
	{
		if ( result[cnt1] != 0 )
		{
			break;
		}
	}
	
	operator1->Exponent = OPERATOR_LEN - cnt1 - operator1->Fraction;
	
	// �ƶ��������
	for ( cnt2 = 0; cnt1 < OPERATOR_LEN; cnt1++,cnt2++)
	{
		operator1->operBuff[cnt2] = result[cnt1];
	}
	
	for ( ; cnt2 < OPERATOR_LEN; cnt2++ )
	{
		operator1->operBuff[cnt2] = 0;
	}
	
	// �õ�ʵ�ʵ�С�����ֳ���
	CalculatorGetFractionLen(operator1);
	mmi_trace(g_sw_ORGANIZER, ":::::::::::::::::operator1->Exponent = %d", operator1->Exponent);
	mmi_trace(g_sw_ORGANIZER, ":::::::::::::::::operator1->Fraction = %d", operator1->Fraction);
	
}

/********************************************************
	
 �������㣬��������� operator1 �� operator1Symbol
 �������Ϊ0,����1;���򷵻�0
********************************************************/
#define	CALC_DIVI_RESULT_LEN	(OPERATOR_LEN+1)
BOOL CalculatorDivi(CALOPERATOR_STRUCT *operator1, CALOPERATOR_STRUCT *operator2 )
{
	U8 result[CALC_DIVI_RESULT_LEN];
	S32 cnt1,cnt2;
	S8 cmpResult;

	ASSERT(operator1 != NULL && operator2 != NULL);

	if ( CalculatorOperatorsFormat2(operator1) == 0 )
	{// ��Ϊ0
		if (CalculatorOperatorsFormat2(operator2) == 0)
		{
			return FALSE;
		}
		else
		{
		operator1->Symbol = 1;
		operator1->Exponent = 0;
		operator1->Fraction = 0;
		for ( cnt1 = 0; cnt1 < CALC_DIVI_RESULT_LEN; cnt1++ )
		{
			operator1->operBuff[cnt1] = 0;
		}
		
			return TRUE;
		}
		
	}

	if ( CalculatorOperatorsFormat2(operator2) == 0 )
	{
		return  FALSE;
	}
	
	memset(result, 0, CALC_DIVI_RESULT_LEN);

	// �������ͳ�������һ���ֽ�	
	CalCulatorRightMov( operator1->operBuff,1);
	CalCulatorRightMov( operator2->operBuff,1);

	// ���м���
	for ( cnt1 = 0; cnt1 < CALC_DIVI_RESULT_LEN; cnt1++ )
	{
		cnt2 = 0;

		while( (cmpResult = CalculatorCmpOperator( operator1->operBuff,operator2->operBuff )) > 0 )
		{
			cnt2++;
			CalculatorSub( operator1->operBuff, operator2->operBuff );
		}
		
		if (cmpResult == 0)
		{
			cnt2++;
			result[cnt1] = cnt2;
			break;
		}
		
		result[cnt1] = cnt2;
			
		CalCulatorLeftMov( operator1->operBuff,1);
		
	}
	
	// �õ�����,ָ���Լ�С������
	operator1->Symbol = operator1->Symbol*operator2->Symbol;
	operator1->Exponent = operator1->Exponent + 1 - operator2->Exponent;
	
	for (cnt1 = 0; cnt1 < operator1->Exponent; cnt1++)
	{
		if (result[cnt1]!= 0)
		{
			break;
		}
	}
	
	if ((operator1->Exponent-cnt1)!= 0)
	{
		operator1->Exponent = operator1->Exponent - cnt1;
		CalCulatorLeftMov( result,cnt1);
	}
	
	for ( cnt1 = OPERATOR_LEN - 1; cnt1 >= operator1->Exponent; cnt1-- )
	{
		if ( result[cnt1]!= 0 )
		{
			break;
		}
	}

	if ((cnt1 == OPERATOR_LEN - 1) && (operator1->Exponent < -1))        
		operator1->Fraction = 17 ;
	else
		operator1->Fraction = cnt1 + 1 - operator1->Exponent;
       /*���"����̫Сʱ,��С�����λ������(OPERATOR_LEN + 1)λ,���������"������*/
	if(operator1->Fraction > (OPERATOR_LEN + 1))
	{
		operator1->Fraction = OPERATOR_LEN + 1;
	}

	for ( cnt1 = 0; cnt1 < OPERATOR_LEN; cnt1++ )
	{
		operator1->operBuff[cnt1] = result[cnt1];
	}

	if (operator1->Exponent < 0)
	{
		U8 cnt3;
		cnt3 = 0 - operator1->Exponent;
		CalCulatorRightMov((U8*)operator1, cnt3);
	}
	
	return TRUE;
}

#endif
