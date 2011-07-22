/*
  test file for obex_sec_wrap.c
 */

#include "host_config.h"

#include "host_types.h"
#include "host_error.h"
#include "obex_sec_wrap.h"
#include "sys_rand_num_gen.h"

#include "cunit.h"

extern u_int32 unit_test_state1;
extern u_int32 unit_test_state2;
extern u_int32 unit_test_state3;
extern u_int8* unit_test_pointer1;
extern u_int8* unit_test_pointer2;
extern u_int8* unit_test_pointer3;



void test_OBEX_RegisterSecurityLevel(void);
void test_OBEX_EnterAuthInfo(void);
void test_GetNonce(void);
void test_OBEX_GenerateNonce(void);
void test_OBEX_GenerateDigest(void);
void SecCallbackfn(u_int8 cid, u_int16 serverChanNum, u_int8 status);



void test_obex_sec_wrap(void)
{
	cuRegTest("Obex Security Wrappers- EnterAuthInfo Tests",4,test_OBEX_EnterAuthInfo);
	cuRegTest("Obex Security Wrappers- RegisterSecurityLevel Tests",9,test_OBEX_RegisterSecurityLevel);
	cuRegTest("Obex Security Wrappers- Test OBEX_GenerateNonce",4,test_OBEX_GenerateNonce);
	cuRegTest("Obex Security Wrappers- Test OBEX_GenerateDigest",10,test_OBEX_GenerateDigest);
	
}

void test_OBEX_EnterAuthInfo(void)
{
	int status;
	char *pin="19681999";
	u_int8 cid=0x05;
	u_int16 srvChanNum=0x05;
	status=OBEX_EnterAuthInfo(0x00,pin,cid,srvChanNum);
	cuReport("std enter auth info",(status == BT_NOERROR));

	/*check for missing/void parameters*//*all should fail*/
	/*no pin*/
	status=OBEX_EnterAuthInfo(0x00,0x00,cid,srvChanNum);
	cuReport("no pin",(status != BT_NOERROR));
	/*no cid*/
	status=OBEX_EnterAuthInfo(0x00,pin,0x00,srvChanNum);
	cuReport("no cid",(status != BT_NOERROR));
  /*no srvchannum*/
	status=OBEX_EnterAuthInfo(0x00,pin,cid,0x00);
	cuReport("no srvChanNum",(status != BT_NOERROR));
}



void test_OBEX_RegisterSecurityLevel(void)
{
		
	int status;
	char *pin="19681999";

	status=OBEX_RegisterSecurityLevel(NOSECURITY,0x00,pin,SecCallbackfn);
	cuReport("std register",(status == BT_NOERROR));
	CallSecurityCallback();

	/*check for missing/void parameters*/
	/*no secLevel*//*should pass*/
	status=OBEX_RegisterSecurityLevel(0x00,0x00,pin,SecCallbackfn);
	cuReport("secLevel",(status == BT_NOERROR));
	/*no pin*/
	status=OBEX_RegisterSecurityLevel(0x00,0x00,0x00,SecCallbackfn);
	cuReport("no pin",(status != BT_NOERROR));
  /*no callback*/
	status=OBEX_RegisterSecurityLevel(0x00,0x00,pin,0x00);
	cuReport("no callback",(status != BT_NOERROR));

	/*check for correct secLevel*/
	/*secLevel =NOSECURITY *//*should pass*/
	status=OBEX_RegisterSecurityLevel(NOSECURITY,0x00,pin,SecCallbackfn);
	cuReport("secLevel",(status == BT_NOERROR));
	/*secLevel =AUTHENTICATION *//*should pass*/
	status=OBEX_RegisterSecurityLevel(AUTHENTICATION,0x00,pin,SecCallbackfn);
	cuReport("secLevel",(status == BT_NOERROR));
	/*secLevel =2 *//*should fail*/
	status=OBEX_RegisterSecurityLevel(0x02,0x00,pin,SecCallbackfn);
	cuReport("secLevel",(status != BT_NOERROR));
	/*secLevel =9 *//*should fail*/
	status=OBEX_RegisterSecurityLevel(0x09,0x00,pin,SecCallbackfn);
	cuReport("secLevel",(status != BT_NOERROR));
}


void SecCallbackfn(u_int8 cid, u_int16 serverChanNum, u_int8 status)
{
	cuReport("callback fn being subsequently called after Register",1);
}


void test_OBEX_GenerateNonce(void)
{
	int status;
	t_obex_nonce nonce,temp;
	u_int8 i;
	char pin[PRH_MAX_PINCODE_SIZE]="1968";
			
	i=pStrlen(pin);
/*	
	pMemset(nonce,'a',1);
	pMemset(nonce+15,'b',1);
	
	pMemset(temp,'c',1);
	pMemset(temp+15,'d',1);

*/
	pMemcpy(temp,nonce,sizeof(t_obex_nonce));

	status=OBEX_GenerateNonce(i,pin,nonce);
	cuReport("GenerateNonce",(status == BT_NOERROR));

	/*check for missing/void parameters*/
	/*no pinLength*/
	status=OBEX_GenerateNonce(0x00,pin,nonce);
	cuReport("no pinlen",(status != BT_NOERROR));
	/*no pin*/
	status=OBEX_GenerateNonce(i,0x00,nonce);
	cuReport("no pin",(status != BT_NOERROR));
  /*no nonce*/
	status=OBEX_GenerateNonce(i,pin,0x00);
	cuReport("no nonce",(status != BT_NOERROR));
}


void test_OBEX_GenerateDigest(void)
{
	int status;

	t_obex_nonce nonce;
	t_obex_digest digest;
	t_obex_digest temp;

	
	
/*	u_int8 nonce[16];
	  u_int8 digest[16];
*/

	u_int8 i,j;
	char pin[PRH_MAX_PINCODE_SIZE]="1968";

	i=pStrlen(pin);
		
	status=OBEX_GenerateNonce(i,pin,nonce);
	
	status=OBEX_GenerateDigest(i,pin,nonce,digest);
	cuReport("GenerateDigest",(status == BT_NOERROR));

	/*change nonce*/
	pMemcpy(nonce,digest,sizeof(t_obex_digest)); 
	/*keep a copy of what's in the digest*/
	pMemcpy(temp,digest,sizeof(t_obex_digest));  
	/*run test*/
	status=OBEX_GenerateDigest(i,pin,nonce,digest);
	/*see if digest has changed*/
	j=pMemcmp(temp,digest,sizeof(t_obex_digest));
	/*report*/
	cuReport("GenerateDigest",((status == BT_NOERROR) && (j)));

	/*change nonce*/
	pMemcpy(nonce,digest,sizeof(t_obex_digest)); 
	/*keep a copy of what's in the digest*/
	pMemcpy(temp,digest,sizeof(t_obex_digest));  
	/*run test*/
	status=OBEX_GenerateDigest(i,pin,nonce,digest);
	/*see if digest has changed*/
	j=pMemcmp(temp,digest,sizeof(t_obex_digest));
	/*report*/
	cuReport("GenerateDigest",((status == BT_NOERROR) && (j)));

	/*change nonce*/
	pMemcpy(nonce,digest,sizeof(t_obex_digest)); 
	/*keep a copy of what's in the digest*/
	pMemcpy(temp,digest,sizeof(t_obex_digest));  
	/*run test*/
	status=OBEX_GenerateDigest(i,pin,nonce,digest);
	/*see if digest has changed*/
	j=pMemcmp(temp,digest,sizeof(t_obex_digest));
	/*report*/
	cuReport("GenerateDigest",((status == BT_NOERROR) && (j)));

	/*change nonce*/
	pMemcpy(nonce,digest,sizeof(t_obex_digest)); 
	/*keep a copy of what's in the digest*/
	pMemcpy(temp,digest,sizeof(t_obex_digest));  
	/*run test*/
	status=OBEX_GenerateDigest(i,pin,nonce,digest);
	/*see if digest has changed*/
	j=pMemcmp(temp,digest,sizeof(t_obex_digest));
	/*report*/
	cuReport("GenerateDigest",((status == BT_NOERROR) && (j)));

	/*change nonce*/
	pMemcpy(nonce,digest,sizeof(t_obex_digest)); 
	/*keep a copy of what's in the digest*/
	pMemcpy(temp,digest,sizeof(t_obex_digest));  
	/*run test*/
	status=OBEX_GenerateDigest(i,pin,nonce,digest);
	/*see if digest has changed*/
	j=pMemcmp(temp,digest,sizeof(t_obex_digest));
	/*report*/
	cuReport("GenerateDigest",((status == BT_NOERROR) && (j)));

/*
	nonce[0]=0x09;
	nonce[1]=0x8f;
	nonce[2]=0x6b;
	nonce[3]=0xcd;
	nonce[4]=0x46;
	nonce[5]=0x21;
	nonce[6]=0xd3;
	nonce[7]=0x73;
	nonce[8]=0xca;
	nonce[9]=0xde;
	nonce[10]=0x4e;
	nonce[11]=0x83;
	nonce[12]=0x26;
	nonce[13]=0x27;
	nonce[14]=0xb4;
	nonce[15]=0xf6;

	pin[0]='t';
	pin[1]='e';
	pin[2]='s';
	pin[3]='t';
	pin[4]=0x00;

	i=pStrlen(pin);

	status=OBEX_GenerateDigest(i,pin,nonce,digest);
	cuReport("GenerateDigest - Marks numbers",(status == BT_NOERROR));
*/
	/*check for missing/void parameters*/

	/*no pinLength*/
	status=OBEX_GenerateDigest(0x00,pin,nonce,digest);
	cuReport("no pinLen",(status != BT_NOERROR));
	/*no pin*/
	status=OBEX_GenerateDigest(i,0x00,nonce,digest);
	cuReport("no pin",(status != BT_NOERROR));
  /*no nonce*/
	status=OBEX_GenerateDigest(i,pin,0x00,digest);
	cuReport("no nonce",(status != BT_NOERROR));
	/*no digest*/
	status=OBEX_GenerateDigest(i,pin,nonce,0x00);
	cuReport("no digest",(status != BT_NOERROR));
	
}




