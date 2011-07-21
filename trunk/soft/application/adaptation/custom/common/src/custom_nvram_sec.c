/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * custom_nvram_sec.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for NVRAM security customization. 
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#include <string.h>
#include <stdio.h>

#include "custom_nvram_sec.h"
// wangbei add start 20060614
void nvram_get_key(char* init_key, unsigned char* keys)
{
}
void nvram_encrypt(unsigned char const* key, 
                        unsigned char *buffer, 
                        unsigned int length,
                        int (*scramble)(void))
{
}
void nvram_decrypt(unsigned char const* key, 
                        unsigned char *buffer, 
                        unsigned int length,
                        int (*scramble)(void))
{
}
// wangbei add end
extern void nvram_get_key(char* init_key, unsigned char* keys);
extern void nvram_encrypt(unsigned char const* key, 
                        unsigned char *buffer, 
                        unsigned int length,
                        int (*scramble)(void));
extern void nvram_decrypt(unsigned char const* key, 
                        unsigned char *buffer, 
                        unsigned int length,
                        int (*scramble)(void));

int hashIndex = 0;
#define A 7
#define B 13
#define C 23
int X;
int IV_generator(){
/*f(x)=A*|X-B|+C
	if(X>B)
		hashIndex = (A*(X-B)+C)%256;
	else
		hashIndex = (A*(B-X)+C)%256;
*/
	return hashIndex;
}
void custom_nvram_get_key(char* init_key, unsigned char* keys) {
	nvram_get_key(init_key, keys);
}

void custom_nvram_encrypt(unsigned char* keys, unsigned char *buffer, unsigned int length, int scrambleIndex) {
	X = scrambleIndex;	
	nvram_encrypt(keys, buffer, length, IV_generator);
}

void custom_nvram_decrypt(unsigned char* keys, unsigned char *buffer, unsigned int length, int scrambleIndex) {
	X = scrambleIndex;
	nvram_decrypt(keys, buffer, length, IV_generator);
}

/*
void binsym_scramble(unsigned char *plaintext, unsigned int length){
	//plaintext is the data that needs encryption
	int i;
	for(i = 0 ; i < sizeof(plaintext)/2;i++){
		switch( plaintext[i], plaintext[ sizeof(plaintext) - i ]);	//binary switch (range = plaintext)
		plaintext[i] = plaintext[i]<<4 | plaintext[i]>>4;		//symmetric switch (range = 16x16 matrix)
	}
}
*/


