/*
  test file for obex_sec.c
 */

#include "host_config.h"

#include "host_types.h"
#include "obex_sec.h"
#include "sys_rand_num_gen.h"

#include "cunit.h"

extern u_int32 unit_test_state1;
extern u_int32 unit_test_state2;
extern u_int32 unit_test_state3;
extern u_int8* unit_test_pointer1;
extern u_int8* unit_test_pointer2;
extern u_int8* unit_test_pointer3;

#define RANTESTLEN 1000

void test_obex_MD5(void);
void test_obex_rand(void);

void test_obex_sec(void)
{
	cuRegTest("Obex Security - MD5 code tests",7,test_obex_MD5);
	cuRegTest("Obex Security - Random Num Gen",1,test_obex_rand);
}

/*

Test the obex_Security i.e. the MD5 algorithm implementation

*/
/*
	MD5 test suite as got from  http://www.ietf.org/rfc/rfc1321.txt .



MD5 test suite:
MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =

MD5 ("123456789012345678901234567890123456789012345678901234567890123456
78901234567890") = 57edf4a22be3c955ac49da2e2107b67a
	
*/




void test_obex_MD5(void)
{

	char *string;
	u_int8 digest[16];
	int len;

	string = "";
	len = strlen(string);
	prh_MD5(digest,string,len);
	cuReport("correct id- valid params 1",((digest[0] == 0xd4) && (digest[1]== 0x1d)
																			&& (digest[2] == 0x8c) && (digest[3]== 0xd9)
																			&& (digest[4] == 0x8f) && (digest[5]== 0x00)
																			&& (digest[6] == 0xb2) && (digest[7]== 0x04)
																			&& (digest[8] == 0xe9) && (digest[9]== 0x80)
																			&& (digest[10] == 0x09) && (digest[11]== 0x98)
																			&& (digest[12] == 0xec) && (digest[13]== 0xf8)
																			&& (digest[14] == 0x42) && (digest[15]== 0x7e)));

	string = "a";
	len = strlen(string);
	prh_MD5(digest,string,len);
	cuReport("correct id- valid params 1",((digest[0] == 0x0c) && (digest[1]== 0xc1)
																			&& (digest[2] == 0x75) && (digest[3]== 0xb9)
																			&& (digest[4] == 0xc0) && (digest[5]== 0xf1)
																			&& (digest[6] == 0xb6) && (digest[7]== 0xa8)
																			&& (digest[8] == 0x31) && (digest[9]== 0xc3)
																			&& (digest[10] == 0x99) && (digest[11]== 0xe2)
																			&& (digest[12] == 0x69) && (digest[13]== 0x77)
																			&& (digest[14] == 0x26) && (digest[15]== 0x61)));
	string = "abc";
	len = strlen(string);
	prh_MD5(digest,string,len);
	cuReport("correct id- valid params 1",((digest[0] == 0x90) && (digest[1]== 0x01)
																			&& (digest[2] == 0x50) && (digest[3]== 0x98)
																			&& (digest[4] == 0x3c) && (digest[5]== 0xd2)
																			&& (digest[6] == 0x4f) && (digest[7]== 0xb0)
																			&& (digest[8] == 0xd6) && (digest[9]== 0x96)
																			&& (digest[10] == 0x3f) && (digest[11]== 0x7d)
																			&& (digest[12] == 0x28) && (digest[13]== 0xe1)
																			&& (digest[14] == 0x7f) && (digest[15]== 0x72)));


	string = "message digest";
	len = strlen(string);
	prh_MD5(digest,string,len);
	cuReport("correct id- valid params 1",((digest[0] == 0xf9) && (digest[1]== 0x6b)
																			&& (digest[2] == 0x69) && (digest[3]== 0x7d)
																			&& (digest[4] == 0x7c) && (digest[5]== 0xb7)
																			&& (digest[6] == 0x93) && (digest[7]== 0x8d)
																			&& (digest[8] == 0x52) && (digest[9]== 0x5a)
																			&& (digest[10] == 0x2f) && (digest[11]== 0x31)
																			&& (digest[12] == 0xaa) && (digest[13]== 0xf1)
																			&& (digest[14] == 0x61) && (digest[15]== 0xd0)));

	string = "abcdefghijklmnopqrstuvwxyz";
	len = strlen(string);
	prh_MD5(digest,string,len);
	cuReport("correct id- valid params 1",((digest[0] == 0xc3) && (digest[1]== 0xfc)
																			&& (digest[2] == 0xd3) && (digest[3]== 0xd7)
																			&& (digest[4] == 0x61) && (digest[5]== 0x92)
																			&& (digest[6] == 0xe4) && (digest[7]== 0x00)
																			&& (digest[8] == 0x7d) && (digest[9]== 0xfb)
																			&& (digest[10] == 0x49) && (digest[11]== 0x6c)
																			&& (digest[12] == 0xca) && (digest[13]== 0x67)
																			&& (digest[14] == 0xe1) && (digest[15]== 0x3b)));

	string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	len = strlen(string);
	prh_MD5(digest,string,len);
	cuReport("correct id- valid params 1",((digest[0] == 0xd1) && (digest[1]== 0x74)
																			&& (digest[2] == 0xab) && (digest[3]== 0x98)
																			&& (digest[4] == 0xd2) && (digest[5]== 0x77)
																			&& (digest[6] == 0xd9) && (digest[7]== 0xf5)
																			&& (digest[8] == 0xa5) && (digest[9]== 0x61)
																			&& (digest[10] == 0x1c) && (digest[11]== 0x2c)
																			&& (digest[12] == 0x9f) && (digest[13]== 0x41)
																			&& (digest[14] == 0x9d) && (digest[15]== 0x9f)));


	string = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
	len = strlen(string);
	prh_MD5(digest,string,len);
	cuReport("correct id- valid params 1",((digest[0] == 0x57) && (digest[1]== 0xed)
																			&& (digest[2] == 0xf4) && (digest[3]== 0xa2)
																			&& (digest[4] == 0x2b) && (digest[5]== 0xe3)
																			&& (digest[6] == 0xc9) && (digest[7]== 0x55)
																			&& (digest[8] == 0xac) && (digest[9]== 0x49)
																			&& (digest[10] == 0xda) && (digest[11]== 0x2e)
																			&& (digest[12] == 0x21) && (digest[13]== 0x07)
																			&& (digest[14] == 0xb6) && (digest[15]== 0x7a)));
							

/*

	pMemset(string,0x00,16);
	pMemset(string,0x098f6bcd4621d373cade832627b4f6,16);
	pMemset(string+16,':',1);
	pMemset(string+17,"test",4);
	len = strlen(string);
	prh_MD5(digest,string,len);
	cuReport("correct id- valid params 1",((digest[0] == 0x57) && (digest[1]== 0xed)
																			&& (digest[2] == 0xf4) && (digest[3]== 0xa2)
																			&& (digest[4] == 0x2b) && (digest[5]== 0xe3)
																			&& (digest[6] == 0xc9) && (digest[7]== 0x55)
																			&& (digest[8] == 0xac) && (digest[9]== 0x49)
																			&& (digest[10] == 0xda) && (digest[11]== 0x2e)
																			&& (digest[12] == 0x21) && (digest[13]== 0x07)
																			&& (digest[14] == 0xb6) && (digest[15]== 0x7a)));


*/

/*
	Alternative testing - use this function and view result by inspection

	MDString ("");
  MDString ("a");
  MDString ("abc");
  MDString ("message digest");
  MDString ("abcdefghijklmnopqrstuvwxyz");
  MDString
 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  MDString
 ("1234567890123456789012345678901234567890\
1234567890123456789012345678901234567890");
*/
	

}
/*alternative fn to test manually*/
static void MDString (char *string)
{
	u_int16 digest[16];
  unsigned int i;
	int len = strlen(string);

	prh_MD5(digest,string,len);

	printf("\nMD5 on \"%s\" is:\n\t",string);

  for (i = 0; i < 16; i++)
		 printf ("%02x", digest[i]);

	return;
}

void test_obex_rand(void)
{
	u_int32 seed = 0xffeeddaa;
	int randomNum;
	char *string = "testtesttest";
	int ranNums[RANTESTLEN];
	int i;
	int j;
	int failed  =0;
		
	SYSrand_Seed_Rand(seed);

	for(i=0; i < RANTESTLEN; i++)
	{
		randomNum = SYSrand_Get_Rand();
		ranNums[i] = randomNum;

		for(j=0; j < i; j++  )
		{
			if(ranNums[j] == randomNum)
			{
				failed =1;
				j= i;			
			}
		
		}/*end for*/
	}

	cuReport("random Num generator",(!failed));
	
	
}