/*
 * MODULE NAME:    hs_cmdline_mmi.c
 * DESCRIPTION:    Headset Profile Headset Application command-line MMI for Bluedev/AMX platform
 * AUTHOR:         Jenny Murphy
 *
 * SOURCE CONTROL: $Id: hs_cmdline_mmi.c 1532 2010-08-23 05:47:34Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2003 Parthus Technologies.
 *     All rights reserved.
 *
 */


#include <bt.h>
#include <hs_core.h>
#include <hs_mmi.h>
#ifdef __ECOS__
#include <cyg/hal/hal_arch.h>           /* cygnum_hal_stack_size_typical */
#include <cyg/hal/plf_io.h>
#include <cyg/hal/hal_io.h>             // io macros

#define S3C44BOX_KEYS  (*(volatile u_int32*)S3C44B0X_PDATG)
#endif

int spkrVolSetting, micVolSetting;
int cmdLocallyIntiated;

t_bdaddr storedAddress;
u_int8 storedflag;
static u_int8 initflag = 0;
static u_int8 printFlag = 1;

void pin_requestCallback(t_bdaddr address);

#define prh_printf 

char prh_getch(void)
{
  static key = 0;
#ifdef __ECOS__
  printFlag = 1;
   if((S3C44BOX_KEYS & 0x10) == 0) // press key1
   	key = 1;
   else if(key == 1)
   {
     key = 0;
     return 1;
   }
     
   if((S3C44BOX_KEYS & 0x20) == 0) // press key2
   	key = 2;
   else if(key == 2)
   {
     key = 0;
     return 2;
   }
   if((S3C44BOX_KEYS & 0x40) == 0) // press key3
   	key = 3;
   else if(key == 3)
   {
     key = 0;
     return 3;
   }
   if((S3C44BOX_KEYS & 0x80) == 0) // press key4
   	key = 4;
   else if(key == 4)
   {
     key = 0;
     return 4;
   }
#endif
   printFlag = 0;
	return 0;
}

u_int8 prh_getval(void)
{
  u_int8 val = 0;
	return val;
}

void prh_getcode(u_int8* code, u_int32 maxLen)
{
  code[0] = '0';
  code[1] = '0';
  code[2] = '0';
  code[3] = '0';
  code[4] = 0;
}

void OS_DELAY(int dl)
{
      u_int32 volatile delay;

      for (delay = 0; delay < dl * 100000; delay ++)
        {
	  ;
    }
	return;
}

void  BTapp_Sched(u_int32 num_interations)
{
     int status;
    t_bdaddr address;
    char addressString[MGR_ADDRSTRLEN];
    char selection[5];
    int volumeSetting;
	 if(initflag == 0)
	 {
       	status = MGR_Init(0);
		if (status==BT_NOERROR) 
			status=HS_Init();
		if (status==BT_NOERROR) 
		{
			initflag = 1;
		    MGR_RegisterPINHandler(pin_requestCallback);

		    if (MMI_RetrieveGatewayAddress(&address)) {
		        MGR_PrintBDAddress(address,addressString);			
				prh_printf("Currently configured audio gateway device address is %s\n",addressString);
		    } else {
		        prh_printf("There is no currently configured audio gateway device address\n");
		        prh_printf("When an audio gateway has connected, select option 'r' \n");
		        prh_printf("to use this audio gateway for headset-originated connections.\n");
		    }

			prh_printf("\n\n");
		}
	 } else {
	       if(printFlag)
	       {
	        if (scoState==DISCONNECTED && MMI_RetrieveGatewayAddress(&address))
	            prh_printf("\tPress Key1 to create an audio link to the audio gateway\n");
	        if (scoState==CONNECTED)
	            prh_printf("\tPress Key1 to disconnect the audio link to the audio gateway\n");
	        if (aclState==CONNECTED)
	            prh_printf("\tPress Key2 to configure the audio gateway device address\n");
	        if (VOLCTRLENABLED) {
	            prh_printf("\tPress Key3 to change the local speaker volume\n");
	            prh_printf("\tPress Key4 to change the local microphone volume\n");
	        }

		 	    prh_printf("\n\n");
			  }
			cmdLocallyIntiated=0;

	 		selection[0] = prh_getch();   

			cmdLocallyIntiated=1;

	        switch (selection[0]) {
	            case 1: /* was originally <Enter> */
	                if (scoState==DISCONNECTED)
	                    prh_printf("Connecting audio connection...\n");
	                else
	                    prh_printf("Disconnecting audio connection...\n");
	                status=HS_HeadsetButtonPressed();
					if(status)
					{
						prh_printf("Headset Button press was unsuccessful\n");
					}
	                break;

	            case 2:
	                MMI_StoreGatewayAddress(&gatewayBDAddress);
	                MGR_PrintBDAddress(gatewayBDAddress,addressString);
					prh_printf("Audio Gateway address %s stored.\n",addressString);
	                break;

	            case 3:
	                if (!VOLCTRLENABLED)
	                    break;
	                prh_printf("Enter new headset speaker setting (current=%d) (0-15): ",spkrVolSetting);
	                volumeSetting  = prh_getval();
					prh_printf("\n");
	                if (volumeSetting>0 && volumeSetting<16) {
	                    MMI_SpkrVolSet(volumeSetting);
	                    HS_SpkrVolChanged(volumeSetting);
	                }
					prh_printf("\n");
	                break;

	            case 4:
	                if (!VOLCTRLENABLED)
	                    break;
	                prh_printf("Enter new headset microphone setting (current=%d) (0-15): ",micVolSetting);
	                volumeSetting  = prh_getval();
					prh_printf("\n");
	                if (volumeSetting>0 && volumeSetting<16) {
	                    MMI_MicVolSet(volumeSetting);
	                    HS_MicVolChanged(volumeSetting);
	                }
					prh_printf("\n\n");
	                break;
	        }	 
	 }
}

// tianwq: old app thread, no used for single thre
#if 0

int app_task2(void)
{
}


int app_main(void) {
    int status;

    prh_printf("\n\n\nParthus Technologies Headset Profile Headset Application\n\n");

    start_host();
	do
    {
        status = MGR_Init(0);
        if(status == BT_RETRY)
        {
            /* Delay for 100 ms */
            OS_DELAY(100);
        }
    }while(status == BT_RETRY);

    if (status!=BT_NOERROR) {
        prh_printf("Host stack initialisation error (0x%02X)\n",status);
        return 1;
    }

    status=HS_Init();
	if(status)
	{
		prh_printf(" Headset initialisation error\n");		
		return 0;
	}

    MMI_Main();

    return 0;
}


void MMI_Main(void) {
		int status;
    t_bdaddr address;
    char addressString[MGR_ADDRSTRLEN];
    char selection[5];
    int volumeSetting;

    MGR_RegisterPINHandler(pin_requestCallback);

    if (MMI_RetrieveGatewayAddress(&address)) {
        MGR_PrintBDAddress(address,addressString);			
		prh_printf("Currently configured audio gateway device address is %s\n",addressString);
    } else {
        prh_printf("There is no currently configured audio gateway device address\n");
        prh_printf("When an audio gateway has connected, select option 'r' \n");
        prh_printf("to use this audio gateway for headset-originated connections.\n");
    }

	prh_printf("\n\n");

    for (;;) {
        if (scoState==DISCONNECTED && MMI_RetrieveGatewayAddress(&address))
            prh_printf("\tPress b to create an audio link to the audio gateway\n");
        if (scoState==CONNECTED)
            prh_printf("\tPress b to disconnect the audio link to the audio gateway\n");
        if (aclState==CONNECTED)
            prh_printf("\tPress r to configure the audio gateway device address\n");
        if (VOLCTRLENABLED) {
            prh_printf("\tPress s to change the local speaker volume\n");
            prh_printf("\tPress m to change the local microphone volume\n");
        }

		prh_printf("\n\n");
		cmdLocallyIntiated=0;

 		selection[0] = prh_getch();   

		cmdLocallyIntiated=1;

        switch (selection[0]) {
            case 'b': /* was originally <Enter> */
                if (scoState==DISCONNECTED)
                    prh_printf("Connecting audio connection...\n");
                else
                    prh_printf("Disconnecting audio connection...\n");
                status=HS_HeadsetButtonPressed();
				if(status)
				{
					prh_printf("Headset Button press was unsuccessful\n");
				}
                break;

            case 'r':
                MMI_StoreGatewayAddress(&gatewayBDAddress);
                MGR_PrintBDAddress(gatewayBDAddress,addressString);
				prh_printf("Audio Gateway address %s stored.\n",addressString);
                break;

            case 's':
                if (!VOLCTRLENABLED)
                    break;
                prh_printf("Enter new headset speaker setting (current=%d) (0-15): ",spkrVolSetting);
                volumeSetting  = prh_getval();
				prh_printf("\n");
                if (volumeSetting>0 && volumeSetting<16) {
                    MMI_SpkrVolSet(volumeSetting);
                    HS_SpkrVolChanged(volumeSetting);
                }
				prh_printf("\n");
                break;

            case 'm':
                if (!VOLCTRLENABLED)
                    break;
                prh_printf("Enter new headset microphone setting (current=%d) (0-15): ",micVolSetting);
                volumeSetting  = prh_getval();
				prh_printf("\n");
                if (volumeSetting>0 && volumeSetting<16) {
                    MMI_MicVolSet(volumeSetting);
                    HS_MicVolChanged(volumeSetting);
                }
				prh_printf("\n\n");
                break;
        }
	}/*end of stream*/
}

#endif

int MMI_RetrieveGatewayAddress(t_bdaddr *address) {

  if (storedflag)
  {
      *address=storedAddress;
	  return 1;
  }
  else
  {
	  return 0;
  }
}

int MMI_StoreGatewayAddress(t_bdaddr *address) {

 storedAddress = *address;
 storedflag=1;
 return 1;
}

#define USE_FIXED_PIN 0

void pin_requestCallback(t_bdaddr address) {
#if (USE_FIXED_PIN==0)
    static char addressPrintBuffer[MGR_ADDRSTRLEN];
#endif
    u_int8 pincode[16];
    u_int8 pinlen=0;

#if USE_FIXED_PIN
	pincode[0]='5';
	pincode[1]='5';
	pincode[2]='5';
	pincode[3]='5';
	pinlen=4;
    MGR_PINResponse(address, 1, pincode, pinlen);    
#else
    MGR_PrintBDAddress(address,addressPrintBuffer);

    prh_printf("Enter passcode to connection to device %s:",addressPrintBuffer);
    prh_getcode(pincode,16);
	prh_printf("\n\n");
    while (pincode[pinlen++])
        ;
    pinlen--;

    MGR_PINResponse(address, (u_int8)(pinlen>0?1:0), pincode, pinlen);    
#endif

}

void MMI_ProcessSleep(void) {
    /* Delay for 1000 ms */
    OS_DELAY(1000);
}

void MMI_FatalError(void) {
    prh_printf("Fatal error in headset application - exiting\n");
    return;
}

void MMI_SpkrVolSet(int volumeSetting) {
    spkrVolSetting=volumeSetting;
    prh_printf("Local speaker volume is now set to %d\n",volumeSetting);
}

void MMI_MicVolSet(int volumeSetting) {
    micVolSetting=volumeSetting;
    prh_printf("Local microphone volume set to %d\n",volumeSetting);
}

void MMI_Ring(void) {
    prh_printf("RING: Incoming call - press b to connect\n");
}

void MMI_SCORelease(void) {
    prh_printf("Audio Connection Released.\n");
}
void MMI_SCOActive(void) {
    prh_printf("Audio Connection Connected.\n\n");
		if(!cmdLocallyIntiated)
		{
			prh_printf("\tPress Key1 to disconnect the audio link to the audio gateway\n");
			prh_printf("\tPress Key2 to configure the audio gateway device address\n");
			if (VOLCTRLENABLED) {
					prh_printf("\tPress Key3 to change the local speaker volume\n");
					prh_printf("\tPress Key4 to change the local microphone volume\n");
			}
			//prh_printf("\tPress x to exit the headset application\n");
		}
}

void MMI_ACLRelease(void) {
    t_bdaddr address;

		prh_printf("\nACL Connection Released.\n\n");
		if(!cmdLocallyIntiated)
		{
			if (aclState==DISCONNECTED && MMI_RetrieveGatewayAddress(&address))
					prh_printf("\tPress Key1 to create an audio link to the audio gateway\n");
			if (VOLCTRLENABLED) {
					prh_printf("\tPress Key3 to change the local speaker volume\n");
					prh_printf("\tPress Key4 to change the local microphone volume\n");
			}
		}

}

void MMI_ACLActive(void){

	prh_printf("\nACL Connection Connected.");
}

void MMI_NoConn(void)
{
    t_bdaddr address;

	prh_printf("\nACL Connection Could not be established.\n\n");
	if(!cmdLocallyIntiated)
	{
			if (aclState==DISCONNECTED && MMI_RetrieveGatewayAddress(&address))
					prh_printf("\tPress Key1 to create an audio link to the audio gateway\n");
			if (VOLCTRLENABLED) {
					prh_printf("\tPress Key3 to change the local speaker volume\n");
					prh_printf("\tPress Key3 to change the local microphone volume\n");
			}
		
	}

}
