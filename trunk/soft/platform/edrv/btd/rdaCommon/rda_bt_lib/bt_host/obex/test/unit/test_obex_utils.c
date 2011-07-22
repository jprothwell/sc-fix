/*
  test file for obex_utils.c
 */

#include "host_config.h"

#include "host_types.h"
#include "obex_utils.h"

#include "hu_harness.h"



extern u_int32 unit_test_state1;
extern u_int32 unit_test_state2;
extern u_int32 unit_test_state3;
extern u_int8* unit_test_pointer1;
extern u_int8* unit_test_pointer2;
extern u_int8* unit_test_pointer3;

void test_OBEX_CreateStringHeader(void);
void test_OBEX_CreateSequenceHeader(void);
void test_OBEX_CreateByteHeader(void);
void test_OBEX_Create4ByteHeader(void);
void test_OBEX_HeaderCombinationTest(void);
void test_DifferentOffsetTests(void);






/*

Test the OBEX_CreateStringHeader fn

(ret_value != BT_NOERROR) indicates should fail
(ret_value == BT_NOERROR) indicates should pass

*/



static prh_t_hu_outcome prh_obex_unit_001(int p1, void *arg)
{
    u_int8 id;
	u_int16 length;
	u_int16* pData;
	t_DataBuf *txBuffer;
	u_int16 offset;
	int ret_value;
	struct host_buf* h_buff;
	
	length = 2;
	pData = pMalloc(10);
	
	pData[0] = 'a';
	pData[1] = 'b';
	pData[2] = '\0'; 
	
	h_buff = host_buf_alloc(50);
	
	txBuffer = (t_DataBuf*)h_buff;
	
	offset = 3;
	
	
	id = 0x01;
	ret_value = OBEX_CreateStringHeader(id,length,pData,txBuffer,&offset);
	
	pFree(pData);
	host_buf_free(h_buff);
	
	
	ret_value = (ret_value == BT_NOERROR) ? HU_PASSED : HU_FAILED;
	
	return ret_value;
}




static prh_t_hu_outcome prh_obex_unit_002(int p1, void *arg)
{
    u_int8 id;
	u_int16 length;
	u_int16* pData;
	t_DataBuf *txBuffer;
	u_int16 offset;
	int ret_value;
	struct host_buf* h_buff;
	
	length = 2;
	pData = pMalloc(10);; 
	
	h_buff = host_buf_alloc(50);
	
	length = 3;
	
	pData[0] = 'c';
	pData[1] = 'd';
	pData[2] = 'e';
	pData[3] = '\0';
	
	txBuffer = (t_DataBuf*)h_buff;
	
	offset = 3;
	
	
	id = 0x01;
	ret_value = OBEX_CreateStringHeader(id,length,pData,txBuffer,&offset);
	
	pFree(pData);
	host_buf_free(h_buff);
	
	
	ret_value = (ret_value == BT_NOERROR) ? HU_PASSED : HU_FAILED;
	
    return ret_value;
}



void test_OBEX_CreateStringHeader(void)
{
  u_int8 id;
  u_int16 length;
  u_int16* pData;
  t_DataBuf *txBuffer;
  u_int16 offset;
  int ret_value;
  struct host_buf* h_buff;
 
  length = 2;
  pData = pMalloc(10);

  pData[0] = 'a';
  pData[1] = 'b';
  pData[2] = '\0'; 

  h_buff = host_buf_alloc(50);

  txBuffer = (t_DataBuf*)h_buff;

  offset = 3;

/* Tests to ensure correct operation in OBEX_CreateStringHeader()*/

  id = 0x01;
  ret_value = OBEX_CreateStringHeader(id,length,pData,txBuffer,&offset);
  //cuReport("correct id- valid params 1",(ret_value == BT_NOERROR));

  length = 3;

  pData[0] = 'c';
  pData[1] = 'd';
  pData[2] = 'e';
  pData[3] = '\0';
  pData[5] = 'f';
	pData[6] = 'g';
	pData[7] = 'h';
	pData[8] = 'i';
	pData[9] = 'j';

  id = 0x01;
  ret_value = OBEX_CreateStringHeader(id,length,pData,txBuffer,&offset);
  //cuReport("correct id - valid params 2",(ret_value == BT_NOERROR));


/* Tests to see how OBEX_CreateStringHeader deals with missing/void parameters */

  ret_value = OBEX_CreateStringHeader(id,0x00,pData,txBuffer,&offset);
  //cuReport("no length",(ret_value != BT_NOERROR));

  ret_value = OBEX_CreateStringHeader(id,length,pData,0x00,&offset);
  //cuReport("no txbuffer",(ret_value != BT_NOERROR));

  ret_value = OBEX_CreateStringHeader(id,length,pData,txBuffer,0x00);
  //cuReport("no offset",(ret_value != BT_NOERROR));

  ret_value = OBEX_CreateStringHeader(id,length,0x00,txBuffer,&offset);
  //cuReport("no data",(ret_value == BT_NOERROR));


/* Test the different combinations of no Data/ zero length*/
	
  /*length and no data*/ /* should pass*/
	ret_value = OBEX_CreateStringHeader(id,length,0x00,txBuffer,&offset);
  //cuReport("length and no data",(ret_value == BT_NOERROR));

	/*data and no length*/ /* should fail*/
	ret_value = OBEX_CreateStringHeader(id,0x00,pData,txBuffer,&offset);
  //cuReport("data and no length",(ret_value != BT_NOERROR));

	/*no data and no length*/ /* should pass*/
	ret_value = OBEX_CreateStringHeader(id,0x00,0x00,txBuffer,&offset);
  //cuReport("no data and no length",(ret_value == BT_NOERROR));


/* Tests to see how OBEX_CreateStringHeader deals with invalid id's */

  id = 0x42;
  ret_value = OBEX_CreateStringHeader(id,length,pData,txBuffer,&offset);
  //cuReport("invalid id - byte sequence",(ret_value != BT_NOERROR));

  id = 0x80;
  ret_value = OBEX_CreateStringHeader(id,length,pData,txBuffer,&offset);
  //cuReport("invalid id - 1 byte",(ret_value != BT_NOERROR));

  id = 0xc4;
  ret_value = OBEX_CreateStringHeader(id,length,pData,txBuffer,&offset);
  //cuReport("invalid id - 4 byte",(ret_value != BT_NOERROR));

}

/*

Test the OBEX_CreateSequenceHeader fn

*/
void test_OBEX_CreateSequenceHeader(void)
{
  u_int8 id;
  u_int16 length;
  u_int8* pData;
  t_DataBuf *txBuffer;
  u_int16 offset;
  int ret_value;
  struct host_buf* h_buff;
 
  pData = pMalloc(10);

  h_buff = host_buf_alloc(50);

  txBuffer = (t_DataBuf*)h_buff;

  offset = 3;


/* Tests to ensure correct operation in OBEX_CreateSequenceHeader()*/
  length = 3;	

  pData[0] = 0x01;
  pData[1] = 0x02;
  pData[2] = 0x03;


  id = 0x42;
  ret_value = OBEX_CreateSequenceHeader(id,length,pData,txBuffer,&offset);
  //cuReport("valid byte sequence",(ret_value == BT_NOERROR));
 
  length = 2;
  pData[0] = 0x04;
  pData[1] = 0x05;
  pData[2] = 0x06;	/* to ensure only copies as specified by the length*/
	pData[3] = 0x07;	
	pData[4] = 0x08;	
	pData[5] = 0x09;	
	pData[6] = 0x0a;	
	pData[7] = 0x0b;	
	pData[8] = 0x0c;	

  
  id = 0x46;
  ret_value = OBEX_CreateSequenceHeader(id,length,pData,txBuffer,&offset);
  //cuReport("valid byte sequence",(ret_value == BT_NOERROR));


/* Tests to see how OBEX_CreateSequenceHeader deals with missing/void parameters  */
	offset = 3;
  ret_value = OBEX_CreateSequenceHeader(id,0x00,pData,txBuffer,&offset);
  //cuReport("no length",(ret_value != BT_NOERROR));

  ret_value = OBEX_CreateSequenceHeader(id,length,pData,0x00,&offset);
  //cuReport("no txbuffer",(ret_value != BT_NOERROR));

  ret_value = OBEX_CreateSequenceHeader(id,length,pData,txBuffer,0x00);
  //cuReport("no offset",(ret_value != BT_NOERROR));

  ret_value = OBEX_CreateSequenceHeader(id,length,0x00,txBuffer,&offset);
  //cuReport("no data",(ret_value == BT_NOERROR));


/* Test the different combinations of no Data/ zero length*/
	
  /*length and no data*/ /* should pass*/
	ret_value = OBEX_CreateSequenceHeader(id,length,0x00,txBuffer,&offset);
  //cuReport("length and no data",(ret_value == BT_NOERROR));

	/*data and no length*/ /* should fail*/
	ret_value = OBEX_CreateSequenceHeader(id,0x00,pData,txBuffer,&offset);
  //cuReport("data and no length",(ret_value != BT_NOERROR));
	offset = 3;
	/*no data and no length*/ /* should pass*/
	ret_value = OBEX_CreateSequenceHeader(id,0x00,0x00,txBuffer,&offset);
  //cuReport("no data and no length",(ret_value == BT_NOERROR));


/* Tests to see how OBEX_CreateSequenceHeader deals with wrong id's */
  id = 0x01;
  ret_value = OBEX_CreateSequenceHeader(id,length,pData,txBuffer,&offset);
  //cuReport("invalid id - String",(ret_value != BT_NOERROR));

  id = 0x80;
  ret_value = OBEX_CreateSequenceHeader(id,length,pData,txBuffer,&offset);
  //cuReport("invalid id -1 byte",(ret_value != BT_NOERROR));

  id = 0xc4;
  ret_value = OBEX_CreateSequenceHeader(id,length,pData,txBuffer,&offset);
  //cuReport("invalid id -4 byte",(ret_value != BT_NOERROR));

}

/*

Test the OBEX_CreateByteHeader fn

*/
void test_OBEX_CreateByteHeader(void)
{
  u_int8 id;
  t_DataBuf *txBuffer;
  u_int16 offset;
  int ret_value;
  struct host_buf* h_buff;
  u_int8 data;

  h_buff = host_buf_alloc(50);
  txBuffer = (t_DataBuf*)h_buff;

  offset = 3;

/* Tests to ensure correct operation in OBEX_CreateByteHeader()*/


  data = 0xff;	

  id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data,txBuffer,&offset);
  //cuReport("OBEX_CreateByteHeader - 0xff",(ret_value == BT_NOERROR));
 
  data = 0x00;
  
  id = 0x85;
  ret_value = OBEX_CreateByteHeader(id,data,txBuffer,&offset);
  //cuReport("OBEX_CreateByteHeader - 0x00",(ret_value == BT_NOERROR));


/* Tests to see how OBEX_CreateByteHeader deals with missing/void parameters */

  ret_value = OBEX_CreateByteHeader(id,data,0x00,&offset);
  //cuReport("no txbuffer",(ret_value != BT_NOERROR));

  ret_value = OBEX_CreateByteHeader(id,data,txBuffer,0x00);
  //cuReport("no offset",(ret_value != BT_NOERROR));


/* Tests to see how OBEX_CreateByteHeader deals with wrong id's */

  id = 0x01;
  ret_value = OBEX_CreateByteHeader(id,data,txBuffer,&offset);
  //cuReport("invalid id - String",(ret_value != BT_NOERROR));

  id = 0x42;
  ret_value = OBEX_CreateByteHeader(id,data,txBuffer,&offset);
  //cuReport("invalid id -sequence",(ret_value != BT_NOERROR));

  id = 0xc4;
  ret_value = OBEX_CreateByteHeader(id,data,txBuffer,&offset);
  //cuReport("invalid id -4 byte",(ret_value != BT_NOERROR));

}
/*

Test the OBEX_Create4ByteHeader fn

*/
void test_OBEX_Create4ByteHeader(void)
{
  u_int8 id;
  t_DataBuf *txBuffer;
  u_int16 offset;
  int ret_value;
  struct host_buf* h_buff;
  u_int32 data;

  h_buff = host_buf_alloc(50);
  txBuffer = (t_DataBuf*)h_buff;
  offset = 3;

/* Tests to ensure correct operation in OBEX_Create4ByteHeader()*/


  data = 0xffeeddcc;	

  id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data,txBuffer,&offset);
  //cuReport("OBEX_Create4ByteHeader - 0xff",(ret_value == BT_NOERROR));
 
  data = 0xbbaa9988;
  
  id = 0xcb;
  ret_value = OBEX_Create4ByteHeader(id,data,txBuffer,&offset);
  //cuReport("OBEX_Create4ByteHeader - 0x00",(ret_value == BT_NOERROR));


/* Tests to see how OBEX_Create4ByteHeader deals with missing/void parameters */

  ret_value = OBEX_Create4ByteHeader(id,data,0x00,&offset);
  //cuReport("no txbuffer",(ret_value != BT_NOERROR));

  ret_value = OBEX_Create4ByteHeader(id,data,txBuffer,0x00);
  //cuReport("no offset",(ret_value != BT_NOERROR));


/* Tests to see how OBEX_Create4ByteHeader deals with wrong id's */


  id = 0x01;
  ret_value = OBEX_Create4ByteHeader(id,data,txBuffer,&offset);
  //cuReport("invalid id - String",(ret_value != BT_NOERROR));

  id = 0x42;
  ret_value = OBEX_Create4ByteHeader(id,data,txBuffer,&offset);
  //cuReport("invalid id -sequence",(ret_value != BT_NOERROR));

  id = 0x80;
  ret_value = OBEX_Create4ByteHeader(id,data,txBuffer,&offset);
  //cuReport("invalid id - 1 byte",(ret_value != BT_NOERROR));

}

/*

Test the combination of the header encoder fn's

*/
void test_OBEX_HeaderCombinationTest(void)
{

  u_int8 id;
  u_int16 length;
  u_int16* pData16;
	u_int8* pData8;
	u_int8 data8;
	u_int32 data32;
  t_DataBuf *txBuffer;
  u_int16 offset;
  int ret_value;
  struct host_buf* h_buff;
 
  length = 2;
  pData16 = pMalloc(10);
	pData8  = pMalloc(10);

  pData16[0] = 'a';
  pData16[1] = 'b';
  pData16[2] = '\0'; 

  pData8[0] = 0x01;
  pData8[1] = 0x02;
  pData8[2] = 0x03;

  h_buff = host_buf_alloc(50);

  txBuffer = (t_DataBuf*)h_buff;

  offset = 3;

/*Test that the Header encoders run ok one after the other*/	
	/*String Header*/
  id = 0x01;
  ret_value = OBEX_CreateStringHeader(id,length,pData16,txBuffer,&offset);
  //cuReport("Combo1 - String",(ret_value == BT_NOERROR));

	/*Sequence Header*/
  length = 3;	
  id = 0x42;
  ret_value = OBEX_CreateSequenceHeader(id,length,pData8,txBuffer,&offset);
  //cuReport("Combo1 - Sequence",(ret_value == BT_NOERROR));

	/*Byte Header*/

  data8 = 0xff;	

  id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data8,txBuffer,&offset);
  //cuReport("Combo1 - Byte",(ret_value == BT_NOERROR));


	/*4 Byte Header*/
  data32 = 0xffeeddcc;	

  id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data32,txBuffer,&offset);
  //cuReport("Combo1 - 4 Byte",(ret_value == BT_NOERROR));




/*Test that the Header encoders run ok one after the other - different order*/	


	/*4 Byte Header*/
  data32 = 0xffeeddcc;	
	id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data32,txBuffer,&offset);
  //cuReport("Combo2 - 4 Byte",(ret_value == BT_NOERROR));

	/*Byte Header*/

  data8 = 0xff;	
	id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data8,txBuffer,&offset);
  //cuReport("Combo2 - Byte",(ret_value == BT_NOERROR));


	/*Sequence Header*/
  id = 0x42;
	length = 3;	
  ret_value = OBEX_CreateSequenceHeader(id,length,pData8,txBuffer,&offset);
  //cuReport("Combo2 - Sequence",(ret_value == BT_NOERROR));

	/*String Header*/
  id = 0x01;
	length = 2;
  ret_value = OBEX_CreateStringHeader(id,length,pData16,txBuffer,&offset);
  //cuReport("Combo2 - String",(ret_value == BT_NOERROR));

}



/*

Test the combination of the header encoder fn's
Test the effect of different offsets on the header encoder  fn's

*/

void test_DifferentOffsetTests(void)
{

 u_int8 id;
  u_int16 length;
  u_int16* pData16;
	u_int8* pData8;
	u_int8 data8;
	u_int32 data32;
  t_DataBuf *txBuffer;
  u_int16 offset;
  int ret_value;
  struct host_buf* h_buff;
 
  length = 2;
  pData16 = pMalloc(10);
	pData8  = pMalloc(10);

  pData16[0] = 'a';
  pData16[1] = 'b';
  pData16[2] = '\0';

  pData8[0] = 0x01;
  pData8[1] = 0x02;
  pData8[2] = 0x03;

  h_buff = host_buf_alloc(50);

  txBuffer = (t_DataBuf*)h_buff;


/*Test that the header encoders run ok with offset problems*/

	/*offset index 1*/


	/*String Header*//*should fail */
	offset = 49;
  id = 0x01;
	length = 2;
  ret_value = OBEX_CreateStringHeader(id,length,pData16,txBuffer,&offset);
  //cuReport("Offset too High 1- String",(ret_value != BT_NOERROR));

	/*Sequence Header*//*should fail */
	offset = 49;
  id = 0x42;
	length = 3;	
  ret_value = OBEX_CreateSequenceHeader(id,length,pData8,txBuffer,&offset);
  //cuReport("Offset too high 1- Sequence",(ret_value != BT_NOERROR));

	/*Byte Header*//*should fail */

  data8 = 0xff;	

  offset = 49;
	id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data8,txBuffer,&offset);
  //cuReport("Offset too high 1- Byte",(ret_value != BT_NOERROR));


	/*4 Byte Header*//*should fail */
  data32 = 0xffeeddcc;	

  offset = 49;
	id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data32,txBuffer,&offset);
  //cuReport("Offset too high 1- 4 Byte",(ret_value != BT_NOERROR));



/*different offset - index 2*/


	/*String Header*//*should fail */
	offset = 48;
  id = 0x01;
	length = 2;
  ret_value = OBEX_CreateStringHeader(id,length,pData16,txBuffer,&offset);
  //cuReport("Offset too high 2- String",(ret_value != BT_NOERROR));

	/*Sequence Header*//*should fail */
  offset = 48;
	id = 0x42;
	length = 3;	
  ret_value = OBEX_CreateSequenceHeader(id,length,pData8,txBuffer,&offset);
  //cuReport("Offset too high 2- Sequence",(ret_value != BT_NOERROR));

	/*Byte Header*/ /*should pass - only needs 2 bytes*/
  offset = 48;
	id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data8,txBuffer,&offset);
  //cuReport("Offset exact amount 2- Byte",(ret_value == BT_NOERROR));


	/*4 Byte Header*//*should fail */
  offset = 48;
	id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data32,txBuffer,&offset);
  //cuReport("Offset too high 2- 4 Byte",(ret_value != BT_NOERROR));




/*different offset- index 3*/
	offset = 45;

	/*String Header*//*should fail */
  offset = 45;
	id = 0x01;
	length = 2;
  ret_value = OBEX_CreateStringHeader(id,length,pData16,txBuffer,&offset);
  //cuReport("Offset too high 3- String",(ret_value != BT_NOERROR));

	/*Sequence Header*//*should fail */
	offset = 45;
	id = 0x42;
	length = 3;	
  ret_value = OBEX_CreateSequenceHeader(id,length,pData8,txBuffer,&offset);
  //cuReport("Offset too high 3- Sequence",(ret_value != BT_NOERROR));

	/*Byte Header*/ /*should pass - only needs 2 bytes*/
  offset = 45;
	id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data8,txBuffer,&offset);
  //cuReport("Offset exact amount 3- Byte",(ret_value == BT_NOERROR));


	/*4 Byte Header*//*should pass - only needs 5 bytes*/
	offset = 45;
	id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data32,txBuffer,&offset);
  //cuReport("Offset plenty 3- 4 Byte",(ret_value == BT_NOERROR));



/*different offset - index 4*/
	offset = 44;

	/*String Header*/ /*should fail */
  offset = 44;
	id = 0x01;
	length = 2;
  ret_value = OBEX_CreateStringHeader(id,length,pData16,txBuffer,&offset);
  //cuReport("Offset exact amount 4- String",(ret_value != BT_NOERROR));

	/*Sequence Header*//*should pass - only needs 6 bytes*/
  offset = 44;
	id = 0x42;
	length = 3;	
  ret_value = OBEX_CreateSequenceHeader(id,length,pData8,txBuffer,&offset);
  //cuReport("Offset exact amount 4- Sequence",(ret_value == BT_NOERROR));

	/*Byte Header*/ /*should pass - only needs 2 bytes*/
  offset = 44;
	id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data8,txBuffer,&offset);
  //cuReport("Offset plenty 4- Byte",(ret_value == BT_NOERROR));


	/*4 Byte Header*//*should pass - only needs 5 bytes*/
  offset = 44;
	id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data32,txBuffer,&offset);
  //cuReport("Offset plenty 4- 4 Byte",(ret_value == BT_NOERROR));


/*different offset - index 5*/

	/*String Header*/ /*should pass - only needs 9 (2*2+2) +3 bytes*/
  offset = 41;
	id = 0x01;
	length = 2;
  ret_value = OBEX_CreateStringHeader(id,length,pData16,txBuffer,&offset);
  //cuReport("Offset exact amount 5- String",(ret_value == BT_NOERROR));

	/*Sequence Header*//*should pass - only needs 6 bytes*/
  offset = 41;
	id = 0x42;
	length = 3;	
  ret_value = OBEX_CreateSequenceHeader(id,length,pData8,txBuffer,&offset);
  //cuReport("Offset exact amount 5- Sequence",(ret_value == BT_NOERROR));

	/*Byte Header*/ /*should pass - only needs 2 bytes*/
  offset = 41;
	id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data8,txBuffer,&offset);
  //cuReport("Offset plenty 5- Byte",(ret_value == BT_NOERROR));


	/*4 Byte Header*//*should pass - only needs 5 bytes*/
  offset = 41;
	id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data32,txBuffer,&offset);
  //cuReport("Offset plenty 5- 4 Byte",(ret_value == BT_NOERROR));

/*different offset- index 6*/
	
	/*String Header*/ /*should pass - only needs 6 (2*2+2) bytes*/
  offset = 0;
	id = 0x01;
	length = 2;
  ret_value = OBEX_CreateStringHeader(id,length,pData16,txBuffer,&offset);
  //cuReport("Offset zero 6- String",(ret_value == BT_NOERROR));

	/*Sequence Header*//*should pass - only needs 6 bytes*/
  offset = 0;
	id = 0x42;
	length = 3;	
  ret_value = OBEX_CreateSequenceHeader(id,length,pData8,txBuffer,&offset);
  //cuReport("Offset zero 6- Sequence",(ret_value == BT_NOERROR));

	/*Byte Header*/ /*should pass - only needs 2 bytes*/
  offset = 0;
	id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data8,txBuffer,&offset);
  //cuReport("Offset zero 6- Byte",(ret_value == BT_NOERROR));


	/*4 Byte Header*//*should pass - only needs 5 bytes*/
  offset = 0;
	id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data32,txBuffer,&offset);
  //cuReport("Offset zero 6- 4 Byte",(ret_value == BT_NOERROR));

/*different offset- index 7*/
	
	/*String Header*/ /*should fail */
  offset = 50;
	id = 0x01;
	length = 2;
  ret_value = OBEX_CreateStringHeader(id,length,pData16,txBuffer,&offset);
  //cuReport("Offset max 7- String",(ret_value != BT_NOERROR));

	/*Sequence Header*//*should fail */
  offset = 50;
	id = 0x42;
	length = 3;	
  ret_value = OBEX_CreateSequenceHeader(id,length,pData8,txBuffer,&offset);
  //cuReport("Offset max 7- Sequence",(ret_value != BT_NOERROR));

	/*Byte Header*/ /*should fail */
  offset = 50;
	id = 0x80;
  ret_value = OBEX_CreateByteHeader(id,data8,txBuffer,&offset);
  //cuReport("Offset max 7- Byte",(ret_value != BT_NOERROR));


	/*4 Byte Header*//*should fail */
  offset = 50;
	id = 0xc3;
  ret_value = OBEX_Create4ByteHeader(id,data32,txBuffer,&offset);
  //cuReport("Offset max 7- 4 Byte",(ret_value != BT_NOERROR));


}



prh_t_hu_testinfo prh_obex_utils_tests [] =  
{
	{ "TP_OBEX_V_001_U",0x00,prh_obex_unit_001, NULL, 0 },
	{ "TP_OBEX_V_002_U",0x00,prh_obex_unit_002, NULL, 0 },
};


prh_t_hu_harness prh_obex_utils_group =
{ NULL, HU_OBEX, "OBEX UTILS",0x00,PRH_UT_STUBS_ENABLED, 1,sizeof(prh_obex_utils_tests)/sizeof(prh_t_hu_testinfo), prh_obex_utils_tests, 0, 0, 0, 0, 0};



void test_obex_utils(void)
{
/*	cuRegTest("OBEX_CreateStringHeader",12,test_OBEX_CreateStringHeader);
	cuRegTest("OBEX_CreateSequenceHeader",12,test_OBEX_CreateSequenceHeader);
	cuRegTest("OBEX_CreateByteHeader",7,test_OBEX_CreateByteHeader);
	cuRegTest("OBEX_Create4ByteHeader",7,test_OBEX_Create4ByteHeader);
	cuRegTest("HeaderCombinationTest",8,test_OBEX_HeaderCombinationTest);
	cuRegTest("DifferentOffsetTests",28,test_DifferentOffsetTests);
	*/
	prh_hu_add_testgroup(&prh_obex_utils_group);
}

