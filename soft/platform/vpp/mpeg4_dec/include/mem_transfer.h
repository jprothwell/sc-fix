/*****************************************************************************
 *
 *  XVID MPEG-4 VIDEO CODEC
 *  - 8<->16 bit buffer transfer header -
 *
 *  Copyright(C) 2001-2003 Peter Ross <pross@xvid.org>
 *
 *  This program is free software ; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation ; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY ; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program ; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * $Id: mem_transfer.h,v 1.17 2005/09/13 12:12:15 suxen_drol Exp $
 *
 ****************************************************************************/

#ifndef _MEM_TRANSFER_H
#define _MEM_TRANSFER_H

/*****************************************************************************
 * transfer16to8 API
 ****************************************************************************/

typedef void (TRANSFER_16TO8COPY) (uint8_t * dst,
								   int16_t * src,
								   uint16_t stride);
typedef TRANSFER_16TO8COPY *TRANSFER_16TO8COPY_PTR;

/* Our global function pointer - Initialized in xvid.c */
extern TRANSFER_16TO8COPY_PTR transfer_16to8copy;

/* Implemented functions */
extern TRANSFER_16TO8COPY transfer_16to8copy_c;

/*****************************************************************************
 * transfer16to8 + addition op API
 ****************************************************************************/

typedef void (TRANSFER_16TO8ADD) (uint8_t * dst,
								  int16_t * src,
								  uint32_t stride);

typedef TRANSFER_16TO8ADD *TRANSFER_16TO8ADD_PTR;

/* Our global function pointer - Initialized in xvid.c */
extern TRANSFER_16TO8ADD_PTR transfer_16to8add;

/* Implemented functions */
extern TRANSFER_16TO8ADD transfer_16to8add_c;

#endif

