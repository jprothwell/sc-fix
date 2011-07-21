/***************************************************************************
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdd_floatapi.h,v $
 * Revision 1.7  2008/05/15 08:18:43  browserdevjtbase
 * Core Update
 *
 * Revision 1.6  2007/12/27 15:05:01  browserdevjtbase
 * Core Update.
 *
 * Revision 1.5  2007/11/30 14:29:55  browserdevjtbase
 * API added for doing float operation.
 *
 * Revision 1.4  2007/10/15 07:16:46  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.4  2007/10/08 14:07:03  kavithadevhtmlbrow
 * ScriptEngine_V_0_8_0_51_05_10_2007
 *
 * Revision 1.3  2007/03/20 06:22:52  kumardevhtmlbrow
 * Update the Doxygen comments.
 *
 * Revision 1.2  2007/01/12 12:27:19  kumardevhtmlbrow
 * Updated to JCF abstraction layer
 *
 * Revision 1.1  2007/01/10 12:23:25  kumardevhtmlbrow
 * Added new abstraction layer for float & math
 *
 *
 ***************************************************************************/

/**
 * @file jdd_floatapi.h
 * @ingroup jcal
 * @brief Defines the float utils abstraction. All the float
 *			functions defined here can be directly mapped to
 *			IEEE recommended apis in <float.h> except 
 *			wherever overriden in note section of each API
 * 
 * @note Floating Point Arithmetic Abstraction Service
 *
 *	The Operator APIs are abstracted primarily because of the 
 *	difference in the support extended by various Embedded
 *	Platforms. For eg: Some platforms have a built in Floating point 
 *  support in the Hardware and fully support the IEEE floating point 
 *  arithmetic and on the other hand there are many other platforms 
 *  that does not have built-in Floating point support in the Hardware 
 *  but instead they have software Floating point unit and does not support 
 *  fully the IEEE recommendations. This means that some of the Floating point unit
 *  (to be precise the operators that work on Floating point values) does not 
 *  handle certain conditions when IEEE special values such as 
 *  NAN, INF, -INF, +INF, -0, +0 occur in the operations. Due to this, there arises the need
 *  to abstract the core product from these operations that are platform 
 *  dependent. The APIS are designed such that if the built-in Floating point 
 *  support already handles all the special cases then these services 
 *  can be easily implemented as macros using the corresponding standard 
 *  'c' language operators thus negating any function call overhead. But if the built-in 
 *  Floating point does not handle the special cases then these APIS can handle those 
 *  special cases explicitly. Additionally this abstraction provides the 
 *  option to optimize the floating point arithmetic if efficient mechanisms 
 *  are available, so as to gain more performance, with acceptable precision result values.
 *  The IEEE special values that is to be expected as dLeftNum or dRightNum operands 
 *  (apart from the floating point range of values) in any combination are as follows
 *	
 *	1. NAN
 *	2. INF
 *	3. -INF
 *  4. +INF
 *	5. -Zero
 *  6. +Zero
 *  
 *  The expected output result of each Operation is same as IEEE recommendation.
 */


#if !defined(_JDD_FLOAT_API_H)
#define _JDD_FLOAT_API_H

#ifdef __cplusplus
extern "C" {
#endif

/* String to Double Service */

#define JC_DTOA_MODE_SHORT				0	/* shortest string */
#define JC_DTOA_MODE_SHORT_SW			1	/* " w/ Steele & White rule */
#define JC_DTOA_MODE_ECVT				2	 
#define JC_DTOA_MODE_FCVT				3	/* ndigits past decimal pt */
#define JC_DTOA_MODE_SHORT_ECVT			4
#define JC_DTOA_MODE_SHORT_FCVT			5
#define JC_DTOA_MODE_ECVT_DEBUG			6
#define JC_DTOA_MODE_FCVT_DEBUG			7
#define JC_DTOA_MODE_SHORT_ECVT_DEBUG	8
#define JC_DTOA_MODE_SHORT_FCVT_DEBUG	9


/**
 * @brief	Converts double-precision to a ascii string 
 *
 * @param[in]	dVal Input Double value to be coverted to string
 * @param[in]	iMode One of the above prescribed DTOA modes
 * @param[in]	iDigits Number of digits stored
 * @param[in]	pDecpt Stored decimal-point position
 * @param[in]	pSign Sign of converted number
 * @param[out]	pRve Points to the End of the return value
 *
 * return		The Converted Ascii String
 *
 * Returns the string value equivalent of the input double value dVal
 * The memory of the returned string value MAY be
 * allocated by this api which shall be freed using
 * the jc_freedtoa api.
 * Alternatively, ecvt, gcvt, fcvt or snprintf() 
 * can be used to do this conversion
 * with loss in precision or compromise in output numerical 
 * format when rounding occurs.
 * Though IEEE compatible it may not be built into float.h
 */

JC_INT8 *	jdd_FloatDtoA(JC_DOUBLE dVal, 
					JC_INT32 iMode, 
					JC_INT32 iDigits, 
					JC_INT32 *pDecpt, 
					JC_INT32 *pSign, 
					JC_INT8 **pRve) ;

/**
 * @brief Deallocates the Allocated String returned by jc_dtoa()
 *
 * @param[in]  pStringMem Input String memory to be deallocated
 *
 * Returns void
 * 
 * @note	This complements the jdd_FloatDtoA
 */

void	jdd_FloatFreeDtoA(JC_INT8 *pStringMem) ;

/**
 * @brief Returns the NaN value, as per IEEE recommendation
 *
 * Returns NaN
 */

JC_DOUBLE jdd_FloatGetNaN(void) ;

/**
 * @brief Returns the INF value, as per IEEE recommendation
 *
 * Returns INF
 */

JC_DOUBLE jdd_FloatGetInf(void) ;

/**
 * @brief Returns the +INF value, as per IEEE recommendation
 *
 * Returns +INF
 */

JC_DOUBLE jdd_FloatGetPosInf(void) ;

/**
 * @brief Returns the -INF value, as per IEEE recommendation
 *
 * Returns -INF
 */

JC_DOUBLE jdd_FloatGetNegInf(void) ;

/**
 * @brief Returns the +Zero value, as per IEEE recommendation 
 *
 * Returns +Zero
 */

JC_DOUBLE jdd_FloatGetPosZero(void) ;

/**
 * @brief Returns the -Zero value, as per IEEE recommendation 
 *
 * Returns -Zero
 */

JC_DOUBLE jdd_FloatGetNegZero(void) ;

/**
 * @brief Returns the Maximum Double precision value, as per IEEE recommendation 
 *
 * Returns Maximum Double precision value
 */

JC_DOUBLE jdd_FloatGetMax(void) ;

/**
 * @brief Returns the Minimum Double precision value, as per IEEE recommendation 
 *
 * Returns Minimum Double precision value
 */

JC_DOUBLE jdd_FloatGetMin(void) ;

#if defined(FLOAT_USE_SYSTEMCALL)

#define jdd_FloatCopySign			_copysign
#define jdd_FloatIsNaN(dVal)		_isnan(dVal)
#define jdd_FloatIsFinite(dVal)		_finite(dVal)
#define jdd_FloatIsInf(dVal)		(!_finite(dVal))
#define jdd_FloatIsPosInf(dVal)		(_FPCLASS_PINF == _fpclass(dVal))
#define jdd_FloatIsNegInf(dVal)		(_FPCLASS_NINF == _fpclass(dVal))
#define jdd_FloatIsNegZero(dVal)	(_FPCLASS_NZ == _fpclass(dVal))
#define jdd_FloatIsPosZero(dVal)	(_FPCLASS_PZ == _fpclass(dVal))

#define	jdd_FloatIsMax(dVal)						((jdd_FloatGetMax()) == dVal)
#define jdd_FloatIsMin(dVal)						((jdd_FloatGetMin()) == dVal)
#define jdd_FloatAddition(dLeftNum,dRightNum)		((dLeftNum) + (dRightNum))
#define jdd_FloatSubtraction(dLeftNum,dRightNum)	((dLeftNum) - (dRightNum))
#define jdd_FloatDivision(dLeftNum,dRightNum)		((dLeftNum) / (dRightNum))
#define jdd_FloatMultiplication(dLeftNum,dRightNum)	((dLeftNum) * (dRightNum))
#define jdd_FloatModulus(dLeftNum,dRightNum)		((dLeftNum) % (dRightNum))
#define jdd_FloatRightShift(dLeftNum,dRightNum)		((dLeftNum) >> (dRightNum))
#define jdd_FloatLeftShift(dLeftNum,dRightNum)		((dLeftNum) << (dRightNum))
#define jdd_FloatBitwiseOR(dLeftNum,dRightNum)		((dLeftNum) | (dRightNum))
#define jdd_FloatBitwiseAND(dLeftNum,dRightNum)		((dLeftNum) & (dRightNum))
#define jdd_FloatBitwiseXOR(dLeftNum,dRightNum)		((dLeftNum) ^ (dRightNum))
#define jdd_FloatBitwiseNOT(dNum)					(!(dNum))

/* These 3 services are purposefully split into 5 rather than combined into one service,
	so that built-in operator can be used in the macro itself, if default behaviour
	is preferred */
#define jdd_FloatCompareEquals(dLeftNum,dRightNum)				((dLeftNum) == (dRightNum))
#define jdd_FloatCompareGreaterThan(dLeftNum,dRightNum)			((dLeftNum) > (dRightNum))
#define jdd_FloatCompareLesserThan(dLeftNum,dRightNum)			((dLeftNum) < (dRightNum))
#define jdd_FloatCompareGreaterThanEquals(dLeftNum,dRightNum)	((dLeftNum) >= (dRightNum))
#define jdd_FloatCompareLesserThanEquals(dLeftNum,dRightNum)	((dLeftNum) <= (dRightNum))

#else

/**
 * @brief Checks given double-precision floating-point value for
 *			not a number (NaN).
 * @param[in]	dVal The double value to be tested.
 *
 * Returns a nonzero value if the argument x is a NaN; 
 * otherwise it returns 0
 *
 */

JC_INT32 jdd_FloatIsNaN( JC_DOUBLE dVal ) ;

/**
 * @brief Determines whether given double-precision 
 *			floating point value is finite.
 *
 * @param[in]	dVal The double value to be tested.
 *
 * Returns a nonzero value if its argument x is not infinite, 
 * that is, if INF < x < +INF. It returns 0 
 * if the argument is infinite or a NaN.
 *
 */

JC_INT32 jdd_FloatIsFinite ( JC_DOUBLE dVal );

/**
 * @brief Opposite of jdd_FloatIsFinite
 *
 * @param[in]	dVal The double value to be tested.
 *
 */

JC_INT32 jdd_FloatIsInf ( JC_DOUBLE dVal );

/**
 * @brief Determines whether given double-precision 
 *			floating point value is +ve Infinity.
 *
 * @param[in]	dVal The double value to be tested.
 *
 * Returns a nonzero value if its argument x is +ve infinite, 
 * that is, if  x = +INF, 0 otherwise
 *
 */

JC_INT32 jdd_FloatIsPosInf( JC_DOUBLE dVal) ;

/**
 * @brief Determines whether given double-precision 
 *			floating point value is -ve Infinity.
 *
 * @param[in]	dVal The double value to be tested.
 *
 * Returns a nonzero value if its argument x is -ve infinite, 
 * that is, if  x = -INF, 0 otherwise
 *
 */

JC_INT32 jdd_FloatIsNegInf( JC_DOUBLE dVal) ;

/**
 * @brief Determines whether given double-precision 
 *			floating point value is -ve Zero.
 *
 * @param[in]  dVal The double value to be tested.
 *
 * Returns a nonzero value if its argument x is -ve Zero, 
 * that is, if  x = -0, 0 otherwise
 *
 */

JC_INT32 jdd_FloatIsNegZero( JC_DOUBLE dVal) ;

/**
 * @brief Determines whether given double-precision 
 *			floating point value is +ve Zero.
 *
 * @param[in]	dVal The double value to be tested.
 *
 * Returns a nonzero value if its argument x is +ve Zero, 
 * that is, if  x = +0, 0 otherwise
 *
 */

JC_INT32 jdd_FloatIsPosZero( JC_DOUBLE dVal) ;

/**
 * @brief	Return one value with the sign of another.
 *
 * @param[in]	dxMagnVal The double value whose magnitude is the result magnitude
 * @param[in]	dySignVal The double value whose sign is the result sign
 *
 * Returns its double-precision floating point 
 * argument dxMagnVal with the same sign as its double-precision 
 * floating-point argument dySignVal. There is no error return
 *
 */

JC_DOUBLE jdd_FloatCopySign(JC_DOUBLE dxMagnVal, JC_DOUBLE dySignVal)	;

/**
 * @brief Returns the Sum of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The resulting sum of the operands
 */
         
JC_DOUBLE jdd_FloatAddition(JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;
     
/**
 * @brief Returns the Difference of two floating point values (dLeftNum - dRightNum)
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The resulting Difference of the operands
 */
   
JC_DOUBLE jdd_FloatSubtraction(JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the Quotient by dividing dLeftNum/dRightNum
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The resulting Quotient
 */

JC_DOUBLE jdd_FloatDivision (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;
  
/**
 * @brief Returns the product of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The resulting product of the operands
 */
       
JC_DOUBLE jdd_FloatMultiplication (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the Modulus of two floating point values (dLeftNum % dRightNum)
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The resulting Modulus of the operands
 */

JC_DOUBLE jdd_FloatModulus (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the RightShift of two floating point values dLeftNum >> dRightNum
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The resulting shifted temporary value of the operands
 */

JC_DOUBLE jdd_FloatRightShift (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the LeftShift of two floating point values dLeftNum << dRightNum
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The resulting shifted temporary value of the operands
 */

JC_DOUBLE jdd_FloatLeftShift (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the Bitwise ORing result of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The ORing result
 */

JC_DOUBLE jdd_FloatBitwiseOR (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the Bitwise ANDing of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The ANDing result
 */

JC_DOUBLE jdd_FloatBitwiseAND (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the Bitwise XORing of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns The XORing result
 */

JC_DOUBLE jdd_FloatBitwiseXOR (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the Bitwise NOTing
 * @param[in] dNum The Left Operand
 * Returns The NOTing result
 */

JC_DOUBLE jdd_FloatBitwiseNOT (JC_DOUBLE dNum) ;

/**
 * @brief Returns the result of floating point '==' comparison operation of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns E_TRUE if dLeftNum == dRightNum (including if any acceptable precision errors), 
 * otherwise E_FALSE is returned
 */

JC_BOOLEAN jdd_FloatCompareEquals (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the result of floating point '>' comparison operation of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns E_TRUE if dLeftNum > dRightNum (including if any acceptable precision errors), 
 * otherwise E_FALSE is returned
 */

JC_BOOLEAN jdd_FloatCompareGreaterThan (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the result of floating point '<' comparison operation of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns E_TRUE if dLeftNum < dRightNum (including if any acceptable precision errors), 
 * otherwise E_FALSE is returned
 */

JC_BOOLEAN jdd_FloatCompareLesserThan (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the result of floating point '>=' comparison operation of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns E_TRUE if dLeftNum >= dRightNum (including if any acceptable precision errors), 
 * otherwise E_FALSE is returned
 */

JC_BOOLEAN jdd_FloatCompareGreaterThanEquals (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Returns the result of floating point '<=' comparison operation of two floating point values
 * @param[in] dLeftNum The Left Operand
 * @param[in] dRightNum The Right Operand
 * Returns E_TRUE if dLeftNum<= dRightNum (including if any acceptable precision errors), 
 * otherwise E_FALSE is returned
 */

JC_BOOLEAN jdd_FloatCompareLesserThanEquals (JC_DOUBLE dLeftNum, JC_DOUBLE dRightNum) ;

/**
 * @brief Check if the given value is Maximum Double precision value, as per IEEE recommendation 
 *
 
 */

JC_BOOLEAN jdd_FloatIsMax(JC_DOUBLE dVal) ;

/**
 * @brief Check if the given value is Minimum Double precision value, as per IEEE recommendation 
 *
 * Returns E_TRUE if value is min else E_FALSE
 */

JC_BOOLEAN jdd_FloatIsMin(JC_DOUBLE dVal) ;



#endif /* FLOAT_USE_SYSTEMCALL */

#ifdef __cplusplus
}
#endif

#endif /*_JDD_FLOAT_API_H*/


/* End of File */
