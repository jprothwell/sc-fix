/* !=============================================================! */
/* !                M O D U L E     B O D Y                      ! */
/* !=============================================================! */
/* !                                                             ! */
/* ! Filename: hfp_cli.c  (Nov 2004)                            ! */
/* !                                                             ! */
/* ! Authors:  Shlomi Schlesinger.                               ! */
/* !                                                             ! */
/* ! Description:                                                ! */
/* !                                                             ! */
/* ! History:                                                    ! */
/* !                                                             ! */
/* !=============================================================! */
#include <string.h>  //toupper
#include <ctype.h>   //toupper
#include <stdio.h>   //sprintf

/*#include "bt\host\include\papi.h"
#include "drivers\slic\include\slic.h"
#include "tram\include\tram.h"
#include "drivers\ipc\include\ipc.h"
#include "drivers\ipc\include\ipc_ext.h"
#include "cli\include\console.h"
#include "drivers\uart\include\uart.h"
#include "bt\profiles\common\include\btdevice.h"
#include "bt\host\user\bt.h"

#include "bt\host_ctrllr\core\lmp\include\lmp_const.h"
#include "bt\profiles\common\include\dbgprn.h"
#include "bt\profiles\common\include\dev_db.h"
#include "bt\profiles\hfp\include\hfp_core.h"

#include "bt\profiles\hfp\include\hfp_const.h"*/


#include "papi.h"
#include "bt.h"

#include "hfp_types.h"
#include "hfp_core.h"
#include "hfp_const.h"


/*************************************************************************/
/*********                    Definitions                       **********/
/*************************************************************************/
// add by tianwq
#define DB_SHOW_ALL		0
#define DB_SHOW_CONNECTED_HFP	1

#define HCI_SYN_HV3 0x0004

/*************************************************************************/
/*********                 Global Parameters                    **********/
/*************************************************************************/

/*************************************************************************/
/*********                 Extern Parameters                    **********/
/*************************************************************************/
extern u_int8 g_SCO_AutoAccept;

/*************************************************************************/
/*********                 Exported Functions                   **********/
/*************************************************************************/

/*************************************************************************/
/*********                  Local Functions                     **********/
/*************************************************************************/
// add by tianwq
TdbDeviceRecord *DB_CliSelectDevice(u_int8 dummy)
{
	return NULL;
}

u_int32 BP_getconsole(u_int8 con, u_int8 *buffer, u_int8 length)
{
	return 0;
}

/*************************************************************************/
/*********                  Local Variables	                    **********/
/*************************************************************************/

/***********************************************************************
* FUNCTION:  HFP_CliMenu
* DESCRIPTION:
*    Debugging Console sub-menu
*    Originated: ctp_menu()
**********************************************************************/
void HFP_CliMenu (void)
/**********************************************************************/
{
static u_int8  serialBuffer[100];
u_int32 serial_len;
unsigned char quit = 0;
int key;
char param1[20]= " ";
    
    while (!quit)
    {

		printf("\n\r-------------------------------");
		printf("\n\rHFP sub-menu");
		printf("\n\r-------------------------------");
		printf("\n\r1  - SDP Browse");
		printf("\n\r2  - SCM Connect");
		printf("\n\r3  - SCM Accept");
		printf("\n\r4  - SCM Reject");
		printf("\n\r5  - SCM Disconnect");
		printf("\n\r   -");
		printf("\n\r6  - Setup Audio  [HF|AG] " );
		printf("\n\r7  - Release Audio [HF|AG] ");
		printf("\n\r8  - Accept Audio");
		printf("\n\r9  - Reject Audio");
		printf("\n\r   -");
		printf("\n\r10 - Call - Outgoing to Fixed Number <number>");
		printf("\n\r11 - Call - Outgoing To mem loc <number>");
		printf("\n\r12 - Call - Outgoing to last Number");
		printf("\n\r13 - Call - Answer");
		printf("\n\r14 - Call - Reject");
		printf("\n\r15 - Call - Release");
		printf("\n\r16 - Call - Handle Multiparty");
		printf("\n\r   -");
		printf("\n\r17 - Call - Handsfree/Headset unit [ AG Only ]");

		printf("\n\r17 - Send DTMF <numbers>");
		printf("\n\r   -");
		printf("\n\r18 - Enable  Call Waiting");
		printf("\n\r19 - Disable Call Waiting");
		printf("\n\r20 - Enable  CLIP");
		printf("\n\r21 - Disable CLIP");
		printf("\n\r22 - Retrieve Memory");
		printf("\n\r   -");
		printf("\n\r23 - Toggle SCO Auto [%s] ",  (g_SCO_AutoAccept ? "ON" : "OFF"));
		printf("\n\r   -");
		printf("\n\r25 - Change Speaker Gain  ");
		printf("\n\r26 - Change Microphone Gain");
		printf("\n\r   -");
		printf("\n\r99 - Current SLC State");
		printf("\n\r0  - Quit");
		printf("\n\rEntry> ");

        serial_len = BP_getconsole(0, serialBuffer, sizeof(serialBuffer));
        serialBuffer[serial_len] = 0;
        
        key=-1;
        sscanf(serialBuffer,"%d %s", &key, param1);
        switch (key)
        {
        case 1:
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice(DB_SHOW_ALL);

			if (deviceRecord)
				HF_ServiceDiscover_AG(deviceRecord->bdAddress);
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
            break;
            

		case 2:
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice(DB_SHOW_ALL);
			if (deviceRecord)
				HF_Setup_SLC(deviceRecord->bdAddress);
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
			break;


		case 3:
			{
				TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice(DB_SHOW_ALL);
			if (deviceRecord)
				HF_Accept_SLC(deviceRecord->bdAddress);
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
			break;
		case 4:
			{
				TdbDeviceRecord *deviceRecord;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_ALL);
				if (deviceRecord)
					HF_Reject_SLC(deviceRecord->bdAddress);
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
			break;
			
        case 5:
			{
				TdbDeviceRecord *deviceRecord;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_ALL);
				if (deviceRecord)
					HF_Release_SLC(deviceRecord->bdAddress);
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
			break;

        case 6:
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
				HF_Audio_Setup(deviceRecord->bdAddress);
			else
				printf("\n\rNo Device avaiable.\n\r");

			}
            break;

        case 7 :
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
				HF_Audio_Release(deviceRecord->bdAddress);
			else

				printf("\n\rNo Device avaiable.\n\r");
			}
            break;

		case 8 :
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
				HF_Audio_Accept(deviceRecord->bdAddress, HCI_SYN_HV3);
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
			break;

		case 9 :
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
				HF_Audio_Reject(deviceRecord->bdAddress);
			else

				printf("\n\rNo Device avaiable.\n\r");
			}
			break;

		case 10 :
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
			{
				printf("\n\rCall Num = %s",param1);
				HF_Call_Request(deviceRecord->bdAddress,HF_CALL_TYPE_NORMAL,param1);
			}
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
            break;

 		case 11 :
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
			{
				HF_Call_Request(deviceRecord->bdAddress,HF_CALL_TYPE_MEM,param1);
			}
			else
				printf("\n\rNo Device avaiable.\n\r");

			}
            break;


 		case 12 :
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
			{
				HF_Call_Request(deviceRecord->bdAddress,HF_CALL_TYPE_REDIAL,0);
			}
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
            break;

		case 13 :
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
			{
				HF_Call_Accept(deviceRecord->bdAddress);
			}
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
            break;

		case 14 :
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
			{
				HF_Call_Reject(deviceRecord->bdAddress);
			}
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
            break;

		case 15 :
			{
			TdbDeviceRecord *deviceRecord;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
			if (deviceRecord)
			{
				HF_Call_Release(deviceRecord->bdAddress);
			}
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
            break;


		case 16:
			{
			TdbDeviceRecord *deviceRecord;
			char first_char;
			u_int8 handling_action;
			deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);


			first_char = param1[0];

			handling_action = (u_int8)( first_char - '0');

			if (deviceRecord)
			{

				HF_Multiparty_Call_Handling(deviceRecord->bdAddress,handling_action);
			}
			else
				printf("\n\rNo Device avaiable.\n\r");
			}
            break;

		case 17:
			{
				TdbDeviceRecord *deviceRecord;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
				if (deviceRecord)
				{
					HF_Send_DTMF(deviceRecord->bdAddress,param1);
				}
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
            break;

		case 18:
			{
				TdbDeviceRecord *deviceRecord;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
				if (deviceRecord)
				{
					HF_Modify_Remote_Supplementary_Feature(deviceRecord->bdAddress,HF_FEATURE_CALL_WAIT,TRUE);
				}
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
            break;


		case 19 :
			{
				TdbDeviceRecord *deviceRecord;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
				if (deviceRecord)
				{
					HF_Modify_Remote_Supplementary_Feature(deviceRecord->bdAddress,HF_FEATURE_CALL_WAIT,FALSE);
				}
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
            break;

		case 20:
			{
				TdbDeviceRecord *deviceRecord;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
				if (deviceRecord)
				{
					HF_Modify_Remote_Supplementary_Feature(deviceRecord->bdAddress,HF_FEATURE_CLIP,TRUE);
				}
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
            break;


		case 21:
			{
				TdbDeviceRecord *deviceRecord;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_CONNECTED_HFP);
				if (deviceRecord)
				{
					HF_Modify_Remote_Supplementary_Feature(deviceRecord->bdAddress,HF_FEATURE_CLIP,FALSE);
				}
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
            break;


		case 22:
			{
				TdbDeviceRecord *deviceRecord;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_ALL);
				if (deviceRecord)
				{
					HF_Retrieve_Memory(deviceRecord->bdAddress,20);
				}
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
            break;

		case 23 :
			if (g_SCO_AutoAccept)
	            HF_Configure(0);
			else 
	            HF_Configure(1);
			break;

		case 25 :
			{
				TdbDeviceRecord *deviceRecord;
				char first_char;
				char second_char;
				u_int8 volume;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_ALL);

				first_char = param1[0];
				second_char = param1[1];
				if (deviceRecord)
				{
					if((first_char == '1') && ((second_char >= '0') && (second_char <= '5')))
					{
						// Valid 2 digit number
						volume = 10 + (second_char-'0');
					}
					else if ((first_char >= '0') && (first_char <= '9'))
					{
						// Single Digit number
						volume = first_char - '0';
					}
					HF_Change_Volume_Setting(deviceRecord->bdAddress , 0, volume );
				}
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
			break;

		case 26 :
			{
				TdbDeviceRecord *deviceRecord;
				char first_char;
				char second_char;
				u_int8 volume;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_ALL);

				first_char = param1[0];
				second_char = param1[1];
				
				if (deviceRecord)
				{
					if((first_char == '1') && ((second_char >= '0') && (second_char <= '5')))
					{
						// Valid 2 digit number
						volume = 10 + (second_char-'0');
					}
					else if ((first_char >= '0') && (first_char <= '9'))
					{
						// Single Digit number
						volume = first_char - '0';
					}
				    HF_Change_Volume_Setting(deviceRecord->bdAddress , 1, volume );
				}
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
			break;

		case 99 :
			{
				TdbDeviceRecord *deviceRecord;
				deviceRecord = DB_CliSelectDevice (DB_SHOW_ALL);
				if (deviceRecord)
				{
					printf(" Current Device SCM State = %x ",HF_Device_State(deviceRecord->bdAddress));
				}
				else
					printf("\n\rNo Device avaiable.\n\r");
			}
            break;

        case 0:
			quit = 1;
            break;

        default:
            break;
        }
    }

}/*End of HFP_CliMenu() */

