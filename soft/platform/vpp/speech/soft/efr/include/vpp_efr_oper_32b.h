/* Double precision operations */

void L_Extract (INT32 L_32, INT16 *hi, INT16 *lo);
INT32 L_Comp (INT16 hi, INT16 lo);
INT32 Mpy_32 (INT16 hi1, INT16 lo1, INT16 hi2, INT16 lo2);
INT32 Mpy_32_16 (INT16 hi, INT16 lo, INT16 n);
INT32 Div_32 (INT32 L_num, INT16 denom_hi, INT16 denom_lo);
