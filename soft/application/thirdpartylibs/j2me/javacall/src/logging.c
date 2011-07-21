/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#include "javacall_logging.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "sxs_io.h"    
#include "dbg.h"
#include "string.h"
#include "kal_release.h"

/**
 * Prints out a string to a system specific output strream
 *
 * @param s a NULL terminated character buffer to be printed
*/
unsigned char sxs_SerialFlush (void);

void javacall_print(const char *s) {

 	static char tmpbuff[260];
	static unsigned int count=0;
	unsigned int len = strlen(s);
	if (count +len >=260)
	{
		dbg_TraceOutputText(0, "J2ME: %s",tmpbuff);
		if (strstr(tmpbuff,"Cannot dispatch: No listener for this event type"))
			;//asm("break 1");
		count = 0;
		memset(tmpbuff,0,260);
	}
	strcat(&tmpbuff[count],s);
	if (!strstr(s, "\n"))
	{
		count += strlen(s);
	}
	else
   	{
   		dbg_TraceOutputText(0, "J2ME: %s",tmpbuff);
		if (strstr(tmpbuff,"Cannot dispatch: No listener for this event type"))
			;//asm("break 1");
		count = 0;
		memset(tmpbuff,0,260);
	}
	sxs_SerialFlush ();
}
 
#ifdef __cplusplus
}
#endif
