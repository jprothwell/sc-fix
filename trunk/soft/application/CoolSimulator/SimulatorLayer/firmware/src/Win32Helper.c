/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
* Filename:
* ---------
*	Win32Helper.c
*
* Project:
* --------
*
* Description:
* ------------
*   MNT utilities for WIN32 environment
*
* Author:
* -------
*
*==============================================================================
* 				HISTORY
*------------------------------------------------------------------------------
 * $Log$
 *
 *------------------------------------------------------------------------------
*==============================================================================
*******************************************************************************/
#ifdef WIN32
#include "windows.h"

#define lsb(bp) (((char*)(bp))[0])
#define msb(bp) (((char*)(bp))[1])

static HANDLE hFind;
int PCFindFirstFile( char* file_expr, char* filename )
{
	WIN32_FIND_DATA FileData;
	
	filename[0] = 0;
	if( (hFind = FindFirstFile( file_expr, &FileData )) == INVALID_HANDLE_VALUE )
		return 0;
	strcpy( filename, FileData.cFileName );
	return 1;
}

int PCFindNextFile( char* filename )
{
	WIN32_FIND_DATA FileData;
	
	filename[0] = 0;
	if( !FindNextFile( hFind, &FileData ) )
		return 0;
	strcpy( filename, FileData.cFileName );
	return 1;
}

void PCFindCloseFile()
{
	FindClose( hFind );
}

static void reverse(unsigned short *start, int wlen)
{
   int i, j;
   unsigned short w; 

   // t = s[i];
   // s[i] = s[j];
   // s[j] = t;
   for (i=0, j=wlen-1; i<j; i++, j--)
   {
 	lsb(&w) = lsb(start+i);
 	msb(&w) = msb(start+i); 

 	lsb(start+i) = lsb(start+j);
 	msb(start+i) = msb(start+j);

 	lsb(start+j) = lsb(&w); 
 	msb(start+j) = msb(&w);
   }
}

static unsigned short* itow(unsigned short *bp, int ival, char zflag, int width, int x, char u)
{
   int i, sign, rem, len; 
   unsigned int val, oldval;
   unsigned short *start;

   if (u)
   {
      sign = 0; 
      oldval = val = (unsigned int) ival;
   }
   else
   {
      oldval = val = (unsigned int) (((sign = ival) < 0) ? -ival : ival); 
   }

   // calculate the result length in wchar
   for (len = (sign < 0) ? 2 : 1 ; (val /= x) > 0 ; len++)
   	;
   if (len < width) // insert "space" or 0's before real value
   { 
      for (i=0; i <width-len ; i++)
      { 
         lsb(bp) = zflag ? '0' : ' ' ;
         msb(bp) = 0;
         bp++;
      }
   }

   val = oldval;
   start = bp;
   do 
   {
      rem = val % x;
      if (rem < 10)
      {
         lsb(bp) = rem + '0'; 
         msb(bp) = 0;
      }
      else // hexadecimal
      {
         rem -= 10;
         lsb(bp) = rem + 'a'; 
         msb(bp) = 0;         
      }
      
      bp++;
   } while ((val /= x) > 0);

   if (sign < 0) 
   {
      lsb(bp) = '-'; 
      msb(bp) = 0;
      bp++;
   }
   
   reverse(start, len); 

   return bp;
}

#ifndef __AMNT__
void kal_wsprintf(unsigned short *outstr, char *fmt,...)
{
   va_list ap;
   int dval, width;
   unsigned int udval;
   char zflag;
   char *p, *sval;
   char cval;
   unsigned short *bp=outstr, *wval; 

   va_start (ap, fmt);
   for (p= fmt, width=0, zflag = 0; *p; p++, width=0, zflag = 0)
   {
	if (*p != '%')
	{
		lsb(bp) = *p;
		msb(bp) = 0;
		bp++;
		continue;
	}

again:
	switch (*++p) {
	case 'c':
		cval= va_arg(ap, int);
		lsb(bp) = cval;
		msb(bp) = 0;
		bp++;
		break;
			
	case 'd':
		dval= va_arg(ap, int);
		bp = itow(bp, dval, zflag, width, 10, 0);
		break;

	case 's':
		for (sval = va_arg(ap, char *) ; *sval ; sval++ )
	       {
		    lsb(bp) = *sval;
		    msb(bp) = 0;
		    bp++;
	       }
		break;
     
	case 'u':
		udval= va_arg(ap, unsigned int);
		bp = itow(bp, udval, zflag, width, 10, 1);
		break;
		
	case 'w':
		for (wval = va_arg(ap, unsigned short *) ;  ; wval++ )
	       {
	           if ((lsb(wval)==0) && (msb(wval)==0))
	           	break;
		    lsb(bp) = lsb(wval);
		    msb(bp) = msb(wval);
		    bp++;
	       }
		break;
	
	case 'x':
		dval= va_arg(ap, int);
		bp = itow(bp, dval, zflag, width, 16, 0);
		break;

	default: // sould be a number....... for example, %7d, %07d
	       if ((width==0) && (zflag==0)) 
	          zflag=1; // need '0' before real value
		width = width*10 + (*p - '0'); 
		goto again;
		break;
	}
   }
   lsb(bp) = 0;
   msb(bp) = 0;

   va_end (ap);
}
#endif
unsigned int BAL_TH(unsigned char pri){}
unsigned int COS_GetDefaultMmiTaskHandle(VOID){}
unsigned int hal_timRTC_GetTime(void){return 0;}
#endif


