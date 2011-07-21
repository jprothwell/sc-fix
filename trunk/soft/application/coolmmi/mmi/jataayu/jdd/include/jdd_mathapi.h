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
 * $Log: jdd_mathapi.h,v $
 * Revision 1.3  2007/03/20 06:20:58  browserdevjtbase
 * Update to comments
 *
 * Revision 1.2  2007/01/12 11:18:05  browserdevjtbase
 * Update from the main base
 *
 * Revision 1.1  2006/12/28 09:48:22  browserdevjtbase
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

/**
 * @file jdd_mathapi.h
 * @ingroup jcal
 * @brief Defines the math utils abstraction. All the math
 *			functions defined here are ANSI compatible in <math.h>
 *			except wherever overriden in note section of each api
 * 
 */

#if !defined(_JDD_MATH_API_H)
#define _JDD_MATH_API_H

#ifdef __cplusplus
extern "C" {
#endif


#if defined(MATH_USE_SYSTEMCALL)

#define jdd_MathABS					abs
#define jdd_MathACOS				acos
#define jdd_MathASIN				asin
#define jdd_MathATAN				atan
#define jdd_MathATAN2				atan2
#define jdd_MathCeil				ceil
#define jdd_MathCOS					cos
#define jdd_MathEXP					exp
#define jdd_MathFloor				floor
#define jdd_MathLOG					log
#define jdd_MathPOW					pow
#define jdd_MathSIN					sin
#define jdd_MathSQRT				sqrt
#define jdd_MathTAN					tan
#define jdd_MathFmod				fmod

#else

/**
 * @brief Calculates the absolute value.
 *
 * @param[in] iVal - Input value 
 *
 * The abs function returns the absolute value of its parameter
 *
 */

JC_INT32 jdd_MathABS( JC_INT32 iVal );

/**
 * @brief Calculates the ceiling of a value.
 *
 * @param[in] dVal - Input value 
 *
 * The ceil function returns a JC_DOUBLE value representing the smallest 
 * integer that is greater than or equal to dVal. 
 *
 */

JC_DOUBLE jdd_MathCeil( JC_DOUBLE dVal );

/**
 * @brief Calculates the floor of a value.
 *
 * @param[in] dVal - Input value 
 *
 * The floor function returns a floating-point value representing the 
 * largest integer that is less than or equal to dVal.
 *
 */

JC_DOUBLE jdd_MathFloor( JC_DOUBLE dVal );

/**
 * @brief Calculates the exponential.
 *
 * @param[in] dVal - Input value 
 *
 * The exp function returns the exponential value of the floating-point
 * parameter, dVal, if successful. On overflow, the function returns INF
 * (infinite) and on underflow, exp returns 0.
 *
 */

JC_DOUBLE jdd_MathEXP( JC_DOUBLE dVal );


/**
 * @brief Calculates logarithms.
 *
 * @param[in] dVal - Input value 
 *
 * The log functions return the logarithm of dVal if successful. 
 * If dVal is negative, these functions return an indefinite (same as a quiet NaN). 
 * If dVal is 0, they return INF (infinite). 
 *
 */

JC_DOUBLE jdd_MathLOG( JC_DOUBLE dVal );

/**
 * @brief Calculates dxVal raised to the power of dPowerOf.
 *
 * @param[in] dxVal - Input value 
 * @param[in] dPowerOf - Input Power factor 
 *
 * pow returns the value of dxVal ^ dPowerOf. 
 *
 */

JC_DOUBLE jdd_MathPOW( JC_DOUBLE dxVal, JC_DOUBLE dPowerOf );


/**
 * @brief Calculates the square root
 *
 * @param[in] dVal - Input value 
 *
 * The sqrt function returns the square-root of dVal. If dVal is negative, sqrt returns NaN 
 *
 */

JC_DOUBLE jdd_MathSQRT( JC_DOUBLE dVal );

/**
 * @brief Calculates the floating-point remainder.
 *
 * @param[in] dxVal - Input value 
 * @param[in] dyVal - Input value 
 *
 * returns the floating-point remainder of dxVal / dyVal. 
 * If the value of y is 0.0, returns a NaN.
 *
 */

JC_DOUBLE jdd_MathFmod( JC_DOUBLE dxVal, JC_DOUBLE dyVal );

/**
 * @brief Calculate sines
 *
 * @param[in] dVal - Input value 
 *
 * NaN is returned when If dVal is greater than or equal to 2^63, 
 * or less than or equal to -2^63
 *
 */

JC_DOUBLE jdd_MathSIN( JC_DOUBLE dVal );


/**
 * @brief Calculates the arcsine.
 *
 * @param[in] dVal - Input value 
 *
 * The asin function returns the arcsine of dVal in the range p/2 to p/2 radians. 
 * If dVal is less than 1 or greater than 1, asin returns an 
 * indefinite (same as a quiet NaN). 
 *
 */

JC_DOUBLE jdd_MathASIN( JC_DOUBLE dVal );


/**
 * @brief Calculate the cosine (cos).
 *
 * @param[in] dVal - Input value 
 *
 * If dVal is greater than or equal to 2^63, or less than or equal to -2^63
 * then NaN is returned
 *
 */

JC_DOUBLE jdd_MathCOS( JC_DOUBLE dVal );


/**
 * @brief Calculates the arccosine.
 *
 * @param[in] dVal - Input value
 *
 * The acos function returns the arccosine of dVal in the range 0 to p radians. 
 * If dVal is less than 1 or greater than 1, acos returns an indefinite 
 * (same as a quiet NaN).
 *
 *
 */

JC_DOUBLE jdd_MathACOS( JC_DOUBLE dVal );

/**
 * @brief Calculate the tangent .
 *
 * @param[in] dVal - Input value 
 *
 * NaN is returned when If dVal is greater than or equal to 2^63, 
 * or less than or equal to -2^63
 *
 */

JC_DOUBLE jdd_MathTAN( JC_DOUBLE dVal );

/**
 * @brief Calculates the arctangent
 *
 * @param[in] dVal - Input value 
 *
 * atan returns a value in the range p/2 to p/2 radians; 
 *
 */

JC_DOUBLE jdd_MathATAN( JC_DOUBLE dVal );

/**
 * @brief Calculates the  arctangent of dyVal/dxVal
 *
 * @param[in] dyVal - Input value 
 * @param[in] dxVal - Input value 
 *
 * atan2 returns a value in the range p to p radians, using the signs 
 * of both parameters to determine the quadrant of the return value
 *
 */

JC_DOUBLE jdd_MathATAN2( JC_DOUBLE dyVal, JC_DOUBLE dxVal );

#endif

#ifdef __cplusplus
}
#endif

#endif /*_JDD_MATH_API_H*/


/* End of File */
