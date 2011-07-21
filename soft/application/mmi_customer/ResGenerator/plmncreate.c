#if 1
#include "cs_types.h"
#include "custdatares.h"
#include "globalmenuitems.h"
#include "buildcfg.h"
#include "mmi_data_types.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "mmi_features.h"//070306 Alpha layer
#include <string.h>
#include <stdio.h>

#include <stdarg.h>

#include "oslmemory.h"
// #include "ucs2prot.h"
#include "unicodexdcl.h"
#include "custresdef.h"
#include "gui_data_types.h"
#include "gui.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <tchar.h>
//#include "unicodexdcl.h"
//#include "ucs2prot.h"
#include "cs_types.h"
#include "mmi_data_types.h"

#include "mmi_features.h" 
#include "wap.h"
#include "voicememodef.h"
#include "funandgamesdefs.h"
#include "sscstringhandle.h"
#include "shortcutsdefs.h"
#include "callsetupenum.h"
#include "settingdefs.h"
//micha0202
//#include "networksetupdefs.h"
#include "networksetupgprot.h"
#include "phonebookdef.h"/*nvram_revise*/

#include "scheduledpoweronoff.h"
#include "screensaverdefs.h"
#include "wallpaperdefs.h"
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
#include "poweronoffdisplaydefs.h"
#endif
#include "organizerdef.h"
//#include "ExtraDef.h"
//micha0601
#include "servicedefs.h"
#include "satdefs.h"
#include "infonum.h"
#include "profilesenum.h"
#include "wgui_categories.h"
#include "wgui_categories_defs.h"
#if defined(__MMI_BMI__)
#include "bmi.h"
#endif

#ifdef __MMI_NOTEPAD__
#include "notepadenum.h"
#endif

#if defined(__MMI_CALORIE__)
/* under construction !*/
#endif
#if defined(__MMI_MENSTRUAL__)
#include "healthmenstural.h"
#endif
#ifdef __MMI_DICTIONARY__
#include "dictionarymain.h"
#endif
#if defined(__MMI_CURRENCY_CONVERTER__)
#include "currencyconvertor.h"
#elif defined(__MMI_EXTENDED_CURRENCY_CONVERTER__)
#include "currencyconvertore.h"
#endif


#if defined(__MMI_UNIT_CONVERTER__) || defined	(__MMI_EXTENDED_UNIT_CONVERTER__)
#include "unitconvertor.h"
#endif


#include "worldclock.h"


#if defined(__MMI_CALCULATOR__)
#include "gdi_include.h"
#include "calculator.h"
#endif

#include "todolistenum.h"
#include "alarmdef.h"

#include "custresdef.h"
#include "calendardef.h"
#if defined(__MMI_NITZ__)
	#include "nitz.h"
#endif
#include "personalizationdef.h"
#include "bootup.h"
#include "quickaccessmenuitem_def.h"
#include "simdetectiondef.h"
#include "messagesresourcedata.h"
#include "mainmenudef.h"
#include "t9def.h"//renamed file as suggested by elvis on date 20th aug 2003,changes done for t9
#include "idleappdef.h"
//#include "gui_themes.h"
//#include "wgui_categories.h"
#include "alarmdef.h"
#include "wgui_categories.h"
#include "statusiconres.h"
//#include "mmi_data_types.h"
//#include "wgui_fixed_menus.h"
#include "funandgamesdefs.h"
#include "gamedefs.h"
#endif

#define PLMN_CREATOR_MAX_SUPPORT_LANGUAGES	50
#define PLMN_CREATOR_MAX_SUPPORT_RECORDS	1000
#define PLMN_CREATOR_MAX_OPNAME_LENGTH		256
#define PLMN_CREATOR_MAX_MCCMNC_LENGTH		128

#define MAX_LENGTH_DISPLAY_NAME			20 /* need to sync with the target definition */

typedef struct
{
	char *opname;
	char *mccmnc;
	char *extname[PLMN_CREATOR_MAX_SUPPORT_LANGUAGES];
} plmn_record_in_unicode;

unsigned char *res_buffer;
unsigned int plmn_num = 0;
unsigned int total_language = 0;
unsigned char language_title[PLMN_CREATOR_MAX_SUPPORT_LANGUAGES][40]; /* 50 languages, 20 words */
plmn_record_in_unicode plmn_array[PLMN_CREATOR_MAX_SUPPORT_RECORDS];

unsigned short plmn_creator_unicode_2_ansii(unsigned char *pOutBuffer, unsigned char *pInBuffer )
{
	unsigned short count = 0;
	
	while( !((*pInBuffer == 0) && (*(pInBuffer + 1) == 0)) )
	{
		*pOutBuffer = *(pInBuffer) ; 
		pInBuffer += 2; 
		pOutBuffer++;
		count++;
	}
	*pOutBuffer = 0;
	return count;			
}

unsigned long plmn_creator_find_keyword(unsigned char *file_buffer, unsigned long buffer_size)
{
	char *keyword = "plmncreate.exe:";
	char *done = "/* DONE */";
	unsigned long i, j;
	
	for(i=0; i<buffer_size-strlen(keyword); i++)
	{
		if(memcmp(file_buffer+i, keyword, strlen(keyword)) == 0)
		{
			for(j=0; ;j++)
			{
				if(*(file_buffer+i+j) == 0x0D && *(file_buffer+i+j+1) == 0x0A)
					break;
			}

			/* Avoid to re-add resources */
			if(memcmp(file_buffer+i+j+2, done, strlen(done)) == 0)
				return 0;
			else
				return i+j+2;
		}
	}
	return 0;
}

unsigned long mmi_netset_plmn_str_2_bcd_short(unsigned char *plmn)
{
	unsigned long b1=0, b2=0, b3=0;
	b1 = ( *plmn    - 0x30) + ((*(plmn+1) - 0x30) << 4);
	b2 = (*(plmn+2) - 0x30) + (*(plmn+5) == 0 ? 0xF0 : ((*(plmn+5) - 0x30) << 4));
	b3 = (*(plmn+3) - 0x30) + ((*(plmn+4) - 0x30) << 4);
	return b1 + (b2 << 8) + (b3 << 16);
}

unsigned short plmn_creator_unicode_strlen(unsigned char *unicode_string)
{
	unsigned short i=0;

	while( !(*(unicode_string+i) == 0 && *(unicode_string+i+1) == 0) )
	{
		i+=2;
	}

	return i;
}

void plmn_creator_modify_CustResDef_h(void)
{
	FILE *fptr;
	unsigned char *read_file;
	unsigned long i=0, read_size, insert_pos;

	/* open file */
	//if( (fptr = fopen("../CustomerInc/CustResDef.h", "rb")) == NULL)
	if( (fptr = fopen("../../coolmmi/MMI/MiscFramework/include/PlmnEnum.h", "rb")) == NULL)
		return;

	/* get file size */
	fseek(fptr, 0, SEEK_END);
	read_size = ftell(fptr);
	read_file = malloc(read_size);

	/* read it to the buffer */
	fseek(fptr, 0, SEEK_SET);
	fread(read_file, 1, read_size, fptr);
	fclose(fptr);

	/* find keyword to insert strings in next line */
	insert_pos = plmn_creator_find_keyword(read_file, read_size);

	if(insert_pos == 0)
		return;

	/* update file */
	//fptr = fopen("../CustomerInc/CustResDef.h", "wb");
	fptr = fopen("../../coolmmi/MMI/MiscFramework/include/PlmnEnum.h", "wb");
	fwrite(read_file, 1, insert_pos, fptr);

	fprintf(fptr, "/* DONE */\n");

	for(i=0;i<plmn_num;i++)
	{
		if(i==0)
			fprintf(fptr, "   STR_PLMN_NAME_%d = SIM_DETECTION_BASE+500,\n", i+1);
		else if(i==plmn_num-1)
			fprintf(fptr, "   STR_PLMN_NAME_%d\n", i+1);
		else
			fprintf(fptr, "   STR_PLMN_NAME_%d,\n", i+1);
	}

	fwrite(read_file+insert_pos, 1, read_size-insert_pos, fptr);
	fclose(fptr);

	free(read_file);
}

void plmn_creator_modify_Res_MiscFramework_c(void)
{
	FILE *fptr;
	unsigned char *read_file;
	unsigned long i=0, read_size, insert_pos;
	unsigned char des_opname[PLMN_CREATOR_MAX_OPNAME_LENGTH], des_mccmnc[PLMN_CREATOR_MAX_MCCMNC_LENGTH];

	/* open file */
	if( (fptr = fopen("../Res_MMI/Res_MiscFramework.c", "rb")) == NULL)
		return;

	/* get file size */
	fseek(fptr, 0, SEEK_END);
	read_size = ftell(fptr);
	read_file = malloc(read_size);

	/* read it to the buffer */
	fseek(fptr, 0, SEEK_SET);
	fread(read_file, 1, read_size, fptr);
	fclose(fptr);

	/* find keyword to insert strings in next line */
	insert_pos = plmn_creator_find_keyword(read_file, read_size);

	if(insert_pos == 0)
		return;

	/* update file */
	fptr = fopen("../Res_MMI/Res_MiscFramework.c", "wb");
	fwrite(read_file, 1, insert_pos, fptr);

	fprintf(fptr, "/* DONE */\n");

	for(i=0;i<plmn_num;i++)
	{
		plmn_creator_unicode_2_ansii(des_opname, plmn_array[i].opname);
		plmn_creator_unicode_2_ansii(des_mccmnc, plmn_array[i].mccmnc);
		fprintf(fptr, "   ADD_APPLICATION_STRING2(STR_PLMN_NAME_%d,\"%s\",\"PLMN Name %s.\");\n", i+1, des_opname, des_mccmnc);
	}

	fwrite(read_file+insert_pos, 1, read_size-insert_pos, fptr);
	fclose(fptr);

	free(read_file);
}

void plmn_creator_modify_CustMiscData_c(void)
{
	FILE *fptr;
	unsigned char *read_file;
	unsigned long i=0, read_size, insert_pos;
	unsigned char des_mccmnc[PLMN_CREATOR_MAX_MCCMNC_LENGTH];

	/* open file */
	//if( (fptr = fopen("../CustResource/CustMiscData.c", "rb")) == NULL)
	if( (fptr = fopen("../../coolmmi/MMI/MiscFramework/src/PlmnName.c", "rb")) == NULL)
		return;

	/* get file size */
	fseek(fptr, 0, SEEK_END);
	read_size = ftell(fptr);
	read_file = malloc(read_size);

	/* read it to the buffer */
	fseek(fptr, 0, SEEK_SET);
	fread(read_file, 1, read_size, fptr);
	fclose(fptr);

	/* find keyword to insert strings in next line */
	insert_pos = plmn_creator_find_keyword(read_file, read_size);

	if(insert_pos == 0)
		return;

	/* update file */
	//fptr = fopen("../CustResource/CustMiscData.c", "wb");
	fptr = fopen("../../coolmmi/MMI/MiscFramework/src/PlmnName.c", "wb");
	fwrite(read_file, 1, insert_pos, fptr);

	fprintf(fptr, "/* DONE */\n");

	for(i=0;i<plmn_num;i++)
	{
		plmn_creator_unicode_2_ansii(des_mccmnc, plmn_array[i].mccmnc);
		fprintf(fptr, "   \"%s\",\n", des_mccmnc);
	}

	fwrite(read_file+insert_pos, 1, read_size-insert_pos, fptr);
	fclose(fptr);

	free(read_file);
}

void plmn_creator_modify_Ref_list_txt(void)
{
	FILE *fptr;
	unsigned char *read_file;
	unsigned long i=0, j=0, read_size, insert_pos;
	char *keyword = (char*)L"##number";
	unsigned short tab_template[] = {0x0009};
	unsigned short new_line_template[] = {0x000D, 0x000A};
	unsigned short max_display_lenght = 0;
	char	StrIDbuf[200];
	char	StrIDbuf1[400];
	char	StrIDbuf2[200];

	/* open file */
	if( (fptr = fopen("../ResGenerator/Ref_list.txt", "rb")) == NULL)
		return;

	/* get file size */
	fseek(fptr, 0, SEEK_END);
	read_size = ftell(fptr);
	read_file = malloc(read_size);

	/* read it to the buffer */
	fseek(fptr, 0, SEEK_SET);
	fread(read_file, 1, read_size, fptr);
	fclose(fptr);

	/* find keyword to insert strings in next line */
	//insert_pos = plmn_creator_find_keyword(read_file, read_size);
	for(i=0; i<read_size-strlen("##number")*2; i++)
	{
		if(memcmp(read_file+i, keyword, strlen("##number")*2) == 0)
		{
			insert_pos = i;
			break;
		}
	}

	/* update file */
	fptr = fopen("../ResGenerator/Ref_list.txt", "wb");
	fwrite(read_file, 1, insert_pos, fptr);

	for(i=0;i<plmn_num;i++)
	{
		memset(StrIDbuf,0,200);
		memset(StrIDbuf1,0,400);
		memset(StrIDbuf2,0,200);

		UnicodeToAnsii(StrIDbuf2, plmn_array[i].mccmnc);
		sprintf(StrIDbuf,"STR_PLMN_NAME_%d	Undefined	%d	PLMN Name %s	",  i+1,plmn_creator_unicode_strlen(plmn_array[i].opname)/2, StrIDbuf2);
              AnsiiToUnicodeString(StrIDbuf1,StrIDbuf);

		fwrite((void *)StrIDbuf1, 1, UCS2Strlen(StrIDbuf1)*2, fptr);
		//fwprintf(fptr, L"STR_PLMN_NAME_%d	Undefined	%d	PLMN Name %s	", i+1, plmn_creator_unicode_strlen(plmn_array[i].opname)/2, plmn_array[i].mccmnc);
		max_display_lenght = (plmn_creator_unicode_strlen(plmn_array[i].opname)/2 >= MAX_LENGTH_DISPLAY_NAME) ? MAX_LENGTH_DISPLAY_NAME - 1 : plmn_creator_unicode_strlen(plmn_array[i].opname)/2;
		
		fwrite((void *)plmn_array[i].opname, 1, max_display_lenght*2, fptr);
		for(j=0; j<total_language; j++)
		{
			fwrite((void *)tab_template, 1, sizeof(tab_template), fptr);
			if(plmn_array[i].extname[j] == NULL)
			{
				max_display_lenght = (plmn_creator_unicode_strlen(plmn_array[i].opname)/2 >= MAX_LENGTH_DISPLAY_NAME) ? MAX_LENGTH_DISPLAY_NAME - 1 : plmn_creator_unicode_strlen(plmn_array[i].opname)/2;
				fwrite((void *)plmn_array[i].opname, 1, max_display_lenght*2, fptr);
			}
			else
			{
				max_display_lenght = (plmn_creator_unicode_strlen(plmn_array[i].extname[j])/2 >= MAX_LENGTH_DISPLAY_NAME) ? MAX_LENGTH_DISPLAY_NAME - 1 : plmn_creator_unicode_strlen(plmn_array[i].extname[j])/2;
				fwrite((void *)plmn_array[i].extname[j], 1, max_display_lenght*2, fptr);
			}
		}
		fwrite((void *)tab_template, 1, sizeof(tab_template), fptr);
		fwrite((void *)new_line_template, 1, sizeof(new_line_template), fptr);
	}

	fwrite(read_file+insert_pos, 1, read_size-insert_pos, fptr);
	fclose(fptr);

	free(read_file);
}

void plmn_creator_read_op_resource(void)
{
	FILE *fptr;
	unsigned char tmp_buffer[PLMN_CREATOR_MAX_OPNAME_LENGTH];
	unsigned long res_size, count=0, i, j;
	unsigned short opname_template[] = {0x006F, 0x0070, 0x006E, 0x0061, 0x006D, 0x0065, 0x003D};
	unsigned short mccmnc_template[] = {0x006D, 0x0063, 0x0063, 0x006D, 0x006E, 0x0063, 0x003D};
	unsigned short new_line_template[] = {0x000D, 0x000A};

	/* open file */
	if( (fptr = fopen("../ResGenerator/plmnlist.txt", "rb")) == NULL)
		return;

	/* get file size */
	fseek(fptr,0,SEEK_END);
	res_size = ftell(fptr);
	res_buffer = malloc(res_size);

	/* read it to the buffer */
	fseek(fptr,0,SEEK_SET);
	fread(res_buffer, 1, res_size, fptr);
	fclose(fptr);

	/* skip unicode file header */
	res_buffer+=2;
	res_size-=2;

	do
	{
		/* read one line */
		for(i=0; ;i++)
		{
			if( memcmp((void *)new_line_template, res_buffer+count+i, 4) != 0)
			{
				*(tmp_buffer+i) = *(res_buffer+count+i);
			}
			else
			{
				memset(res_buffer+count+i, 0, 4);
				break;
			}
		}

		if(i==0)
		{
			count += 4;
			continue;
		}

		if(memcmp(opname_template, tmp_buffer, sizeof(opname_template)) == 0)
		{
			plmn_num++;
			plmn_array[plmn_num-1].opname = res_buffer+count+sizeof(opname_template);
		}
		else if(memcmp(mccmnc_template, tmp_buffer, sizeof(mccmnc_template)) == 0)
		{
			plmn_array[plmn_num-1].mccmnc = res_buffer+count+sizeof(mccmnc_template);
		}
		else
		{
			for(j=0; j<total_language; j++)
			{
				if(memcmp(language_title[j], tmp_buffer, plmn_creator_unicode_strlen(language_title[j])) == 0)
				{
					plmn_array[plmn_num-1].extname[j] = res_buffer+count+plmn_creator_unicode_strlen(language_title[j])+2;
					break;
				}
			}
		}

		count += (i+4);
	}while(count<res_size);
}

void plmn_creator_get_supported_languages(void)
{
	FILE *fptr;
	unsigned char *read_file;
	unsigned long i=0, j=0, k=0, read_size;
	unsigned short new_line_template[] = {0x000D, 0x000A};
	unsigned short tab_template[] = {0x0009};

	/* open file */
	if( (fptr = fopen("../ResGenerator/Ref_list.txt", "rb")) == NULL)
		return;

	/* get file size */
	fseek(fptr, 0, SEEK_END);
	read_size = ftell(fptr);
	read_file = malloc(read_size);

	/* read it to the buffer */
	fseek(fptr, 0, SEEK_SET);
	fread(read_file, 1, read_size, fptr);
	fclose(fptr);

	/* get total languages */
	for(i=0; ;i++)
	{
		if( memcmp((void *)new_line_template, read_file+i, 4) == 0)
		{
			i+=4;
			break;
		}
	}

	do
	{
		if( memcmp((void *)tab_template, read_file+i, 2) == 0)
			j++;
		//else
		i++;
	}
	while(j<5); // skip Enum Value, Module Name, Max String Length, Description, English

	i+=1;
	j=0;

	do
	{
		if( memcmp((void *)tab_template, read_file+i, 2) == 0)
		{
			memset(language_title[j]+k, 0, 2);
			j++;
			k=0;
			total_language++;
		}
		else
		{
			memcpy(language_title[j]+k, read_file+i, 2);
			k+=2;
		}
		i+=2;
	}
	while(memcmp((void *)new_line_template, read_file+i, 4) != 0);

	free(read_file);
}

void plmn_creator_init(void)
{
	unsigned int i, j;

	for(i=0; i<PLMN_CREATOR_MAX_SUPPORT_RECORDS; i++)
	{
		for(j=0; j<PLMN_CREATOR_MAX_SUPPORT_LANGUAGES; j++)
		{
			plmn_array[i].extname[j] = NULL;
		}
	}
}

void main(void)
{
	printf("plmn create");

	plmn_creator_init();
	plmn_creator_get_supported_languages();
	plmn_creator_read_op_resource();

	plmn_creator_modify_CustResDef_h();
	printf("..");
	plmn_creator_modify_Res_MiscFramework_c();
	printf("..");
	plmn_creator_modify_CustMiscData_c();
	printf("..");
	plmn_creator_modify_Ref_list_txt();

	free(res_buffer-2);
	printf("done!\n");
}
