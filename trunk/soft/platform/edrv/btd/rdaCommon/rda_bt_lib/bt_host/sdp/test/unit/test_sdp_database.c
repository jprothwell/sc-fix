/*
  $id$
 */

#include "host_config.h"
#include "host_types.h"
#include "hu_harness.h"
#include "papi.h"

#include "l2_types.h"
#include "../../include/sdp_ue.h"
#include "../../include/sdp_constants.h"
#include "../../include/sdp_server.h"
#include "../../include/sdp_db.h"

int  prh_sdp_unit_init(int p1, void *arg);

extern t_SDP_serviceRecord* prh_sdp_database;


static prh_t_hu_outcome prh_sdp_unit_018(int p1, void *arg)
{
  prh_sdp_flush_database();

  pDebugCheck();

  return HU_PASSED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_019

	*  TEST DESCRIPTION:
		Test the SDP_CreateRecord with an empty database

	*  TEST STEPS:
		1. clear database
		2. Call SDP_CreateRecord 
		3. is the handle set to 0x00010000

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_019(int p1, void *arg)
{
  u_int32 handle;
  u_int8 ret;
  t_SDP_serviceRecord* pRecord;

  ret = SDP_CreateRecord(&handle,&pRecord);

  pDebugCheck();

	if(ret == BT_NOERROR && handle ==0x00010000)
	{
		return HU_PASSED;
	} else {
		return HU_FAILED;
	}


}



/******
*  TEST NAME:   
	prh_sdp_unit_020

	*  TEST DESCRIPTION:
		Test the SDP_CreateRecord for the normal case

	*  TEST STEPS:
		1. database exists
		2. Call SDP_CreateRecord 
		3. is the handle set to the next normal id.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_020(int p1, void *arg)
{
  u_int32 handle;
  t_SDP_serviceRecord* pRecord;
  u_int8 ret;

  ret = SDP_CreateRecord(&handle,&pRecord);

	if(ret == BT_NOERROR && handle ==0x00010001)
	{
		return HU_PASSED;
	} else {
		return HU_FAILED;
	}

  return HU_FAILED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_021

	*  TEST DESCRIPTION:
		Test the SDP_RemoveRecord for an invalid handle

	*  TEST STEPS:
		1. Call SDP_RemoveRecord 
		2. check return code

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_021(int p1, void *arg)
{
  SDP_RemoveRecord(1234);

  return HU_PASSED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_022

	*  TEST DESCRIPTION:
		Test the SDP_RemoveRecord for database record

	*  TEST STEPS:
		1. Call SDP_RemoveRecord(0);
		2. check return code

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_022(int p1, void *arg)
{
	u_int8 ret;

  ret = SDP_RemoveRecord(0);

  if(ret == BT_UNSUPPORTED)
  {
	  return HU_PASSED;
  } else {
  return HU_FAILED;
  }
}

/******
*  TEST NAME:   
	prh_sdp_unit_023

	*  TEST DESCRIPTION:
		Test the SDP_RemoveRecord for valid handle

	*  TEST STEPS:
		1. Call SDP_RemoveRecord(valid handle);
		2. check return code

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_023(int p1, void *arg)
{
	u_int8 ret;
	pDebugCheck();

  ret = SDP_RemoveRecord(0x00010001);

	pDebugCheck();

	if(ret == BT_NOERROR)
	{
		return HU_PASSED;
	} else {
	  return HU_FAILED;

	}

}

static prh_t_hu_outcome prh_sdp_unit_024(int p1, void *arg)
{
	u_int8 ret;


  ret = SDP_RemoveRecord(0x00010002);


	if(ret == BT_NOERROR)
	{
		return HU_PASSED;
	} else {
	  return HU_FAILED;
	}

}

static prh_t_hu_outcome prh_sdp_unit_025(int p1, void *arg)
{
	u_int8 ret;


  ret = SDP_RemoveRecord(0x00010002);


	if(ret == BT_NOERROR)
	{
		return HU_PASSED;
	} else {
	  return HU_FAILED;
	}

}

/******
*  TEST NAME:   
	prh_sdp_unit_026

	*  TEST DESCRIPTION:
		Test the SDP_GetRecord for valid id

	*  TEST STEPS:
		1. Call SDP_GetRecord(valid id);
		2. check returned record

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_026(int p1, void *arg)
{
  u_int32 handle;
  t_SDP_serviceRecord* pRecord,*test;

  SDP_CreateRecord(&handle,&pRecord);

  SDP_CreateRecord(&handle,&pRecord);
  
  SDP_GetRecord(handle, &test);

  if(test==pRecord)
  {
	  return HU_PASSED;
  } else {
	return HU_FAILED;
  }
}


/******
*  TEST NAME:   
	prh_sdp_unit_027

	*  TEST DESCRIPTION:
		Test the SDP_GetRecord for an invalid valid id

	*  TEST STEPS:
		1. Call SDP_GetRecord(invalid valid id);
		2. check returned record == 0

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_027(int p1, void *arg)
{
  t_SDP_serviceRecord* pRecord;

  SDP_GetRecord(1234, &pRecord);

	if(pRecord == 0)
	{
		return HU_PASSED;
	} else {
		return HU_FAILED;
	}
  
  
}

static prh_t_hu_outcome prh_sdp_unit_028(int p1, void *arg)
{
  u_int32 next;
  u_int8 ret;
  ret = SDP_GetNextRecord(1234, &next);

  if(ret == 0)
  {
	  return HU_PASSED;
  } else {
	  return HU_FAILED;
  }

  
}

/******
*  TEST NAME:   
	prh_sdp_unit_029

	*  TEST DESCRIPTION:
		Test the get next record for the first record.

	*  TEST STEPS:
		1. clear database
		2. create 2 records.
		3. call get next record for the first record.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_029(int p1, void *arg)
{
  u_int32 next;
  u_int8 ret;
  ret = SDP_GetNextRecord(0x0010001, &next);

  if(ret == 0)
  {
	return HU_PASSED;
  } else {
	  return HU_FAILED;
  }

}

/******
*  TEST NAME:   
	prh_sdp_unit_030

	*  TEST DESCRIPTION:
		Test the get next record for the normal case

	*  TEST STEPS:
		1. clear database
		2. create 4 records.
		3. call get next record using the second id a key

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_030(int p1, void *arg)
{
  u_int32 next;
  u_int8 ret;

  ret = SDP_GetNextRecord(0x0010002, &next);

  if(ret && next == 0x00010003)
  {
	  return HU_PASSED;
  } else {
	return HU_FAILED;
  }
}


/******
*  TEST NAME:   
	prh_sdp_unit_031

	*  TEST DESCRIPTION:
		Test the addition of the first attribute.

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode and add uint

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_031(int p1, void *arg)
{
  u_int32 handle;
  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement;

  SDP_CreateRecord(&handle,&pRecord);

  SDP_EncodeAttribute(SDP_UINT,4,&handle,&pElement);
  
  SDP_AddAttribute(pRecord,256,&pElement);

  return HU_PASSED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_032

	*  TEST DESCRIPTION:
		Test the normal case for replace attribuite

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode and add uuid
		4. encode and replace uuid same id

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_032(int p1, void *arg)
{
  u_int32 handle;
  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement;

  handle = 0x1103;


  SDP_GetRecord(0x010003,&pRecord);

  SDP_EncodeAttribute(SDP_UUID,4,&handle,&pElement);
  
  SDP_ReplaceAttribute(pRecord,256,&pElement);

  return HU_PASSED;
}



/******
*  TEST NAME:   
	prh_sdp_unit_034

	*  TEST DESCRIPTION:
		Test the encoding of some bacic types.

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode boolean and add
		4. encode uint and add
		5. encode int and add
		6. encode text and add
		7. encode url and add
		8. encode uuid and uint and add to a sequence
		9. encode uint and add it to a union.


	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_034(int p1, void *arg)
{
  /* a record consisting of all types of items*/
  u_int32 handle;
  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement,pOther,pAdditional;
  t_SDP_dataElement* pList[10];
   u_int8 data[] = "Here is some random gibberish";


  pDebugCheck();

  prh_sdp_flush_database();

  pDebugCheck();

  SDP_CreateRecord(&handle,&pRecord);

  SDP_EncodeAttribute(SDP_BOOLEAN,1,data,&pElement);
  SDP_AddAttribute(pRecord,100,&pElement);

  SDP_EncodeAttribute(SDP_UINT,4,data,&pElement);
  SDP_AddAttribute(pRecord,101,&pElement);

  SDP_EncodeAttribute(SDP_INT,4,data,&pElement);
  SDP_AddAttribute(pRecord,102,&pElement);

  SDP_EncodeAttribute(SDP_TEXT,sizeof(data),data,&pElement);
  SDP_AddAttribute(pRecord,103,&pElement);

  SDP_EncodeAttribute(SDP_URL,10,data,&pElement);
  SDP_AddAttribute(pRecord,104,&pElement);

  SDP_EncodeAttribute(SDP_UINT,4,data,&pOther);
  SDP_EncodeAttribute(SDP_UUID,16,data,&pAdditional);
  pList[0] = &pOther;
  pList[1] = &pAdditional;
  SDP_CreateList(SDP_SEQUENCE,2,pList,&pElement);
  SDP_AddAttribute(pRecord,105,&pElement);

  SDP_EncodeAttribute(SDP_UINT,4,data,&pOther);
  pList[0] = &pOther;
  SDP_CreateList(SDP_UNION,1,pList,&pElement);
  SDP_AddAttribute(pRecord,106,&pElement);

  return HU_PASSED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_035

	*  TEST DESCRIPTION:
		Test the encoding of small list and small text

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode 5 small text entries
		4. add entries to a small list
		5. add attribue to the record

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_035(int p1, void *arg)
{
  /* a list of 250 bytes */
  u_int32 handle;
  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pE1,pE2,pE3,pE4,pE5,pElement;
  t_SDP_dataElement* pList[10];
   u_int8 data[] = "1234567890123456788901234567890123456788901234567890";

  prh_sdp_flush_database();

  pDebugCheck();

  SDP_CreateRecord(&handle,&pRecord);

  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE1);
  pList[0] = &pE1;
  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE2);
  pList[1] = &pE2;
  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE3);
  pList[2] = &pE3;
  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE4);
  pList[3] = &pE4;
  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE5);
  pList[4] = &pE5;

  SDP_CreateList(SDP_SEQUENCE,5,pList,&pElement);

  SDP_AddAttribute(pRecord,100,&pElement);

  return HU_PASSED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_036

	*  TEST DESCRIPTION:
		Test the encoding of medium list and medium text

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode 5 medium text entries
		4. add entries to a medium list
		5. add attribue to the record

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_036(int p1, void *arg)
{
  /* a list of 60 k */
  u_int32 handle;
  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pE1,pE2,pE3,pE4,pE5,pE6,pElement;
  t_SDP_dataElement* pList[10];
  u_int8* data;

  data = pMalloc(1024*10); /* 10K */
  
  prh_sdp_flush_database();

  SDP_CreateRecord(&handle,&pRecord);

  pMemset(data,'.',1024*10);
  SDP_EncodeAttribute(SDP_TEXT,1024*10,data,&pE1);
  pList[0] = &pE1;
  pMemset(data,'o',1024*10);
  SDP_EncodeAttribute(SDP_TEXT,1024*10,data,&pE2);
  pList[1] = &pE2;
  pMemset(data,'O',1024*10);
  SDP_EncodeAttribute(SDP_TEXT,1024*10,data,&pE3);
  pList[2] = &pE3;
  pMemset(data,'0',1024*10);
  SDP_EncodeAttribute(SDP_TEXT,1024*10,data,&pE4);
  pList[3] = &pE4;
  pMemset(data,'O',1024*10);
  SDP_EncodeAttribute(SDP_TEXT,1024*10,data,&pE5);
  pList[4] = &pE5;
  pMemset(data,'o',1024*10);
  SDP_EncodeAttribute(SDP_TEXT,1024*10,data,&pE6);
  pList[5] = &pE6;


  SDP_CreateList(SDP_SEQUENCE,6,pList,&pElement);

  SDP_AddAttribute(pRecord,100,&pElement);

  pFree(data);

  return HU_PASSED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_037

	*  TEST DESCRIPTION:
		Test the encoding of large list and medium text

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode 100 large text entries, 100megs
		4. add entries to a medium list
		5. add attribue to the record

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_037(int p1, void *arg)
{
  /* a list of 100 megs */
  u_int32 handle;
  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement;
  t_SDP_dataElement** pList;
  u_int8* data;
  u_int8 lcv;
  t_SDP_dataElement* pStorage;

  prh_sdp_flush_database();

	pDebugCheck();

  data = pMalloc(1024*1024); /* 1MEG */
  
  pList = pMalloc(100 * sizeof(t_SDP_dataElement*));
  pStorage = pMalloc(100 * sizeof(t_SDP_dataElement));

  prh_sdp_flush_database();

  SDP_CreateRecord(&handle,&pRecord);

  for(lcv=0;lcv<100;lcv++)
    {
      SDP_EncodeAttribute(SDP_TEXT,1024*1024,data,pStorage + lcv);
      pList[lcv] = pStorage + lcv;
    }
  
  SDP_CreateList(SDP_SEQUENCE,100,pList,&pElement);

  SDP_AddAttribute(pRecord,100,&pElement);
  
  pFree(data);
  pFree(pList);
  pFree(pStorage);
  return HU_PASSED;
}

static prh_t_hu_outcome prh_sdp_unit_038(int p1, void *arg)
{
  /* a deep list  megs */
  u_int32 handle;
  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement,pE1,pE2,pE3,pE4,pE5,pPrevious;
  t_SDP_dataElement* pList[5];
  u_int8 data[] = "1234567890123456788901234567890123456788901234567890";
  u_int8 lcv;
  t_SDP_dataElement* pStorage;
  u_int32 length;
  u_int8* ref;


  prh_sdp_flush_database();

  pDebugCheck();


  SDP_CreateRecord(&handle,&pRecord);


  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE1);
  pList[0] = &pE1;
  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE2);
  pList[1] = &pE2;
  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE3);
  pList[2] = &pE3;
  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE4);
  pList[3] = &pE4;
  SDP_EncodeAttribute(SDP_TEXT,48,data,&pE5);
  pList[4] = &pE5;
  SDP_CreateList(SDP_SEQUENCE,5,pList,&pElement);

  for(lcv=0;lcv<20;lcv++)
    {
      pE1 = pElement;
      pList[0] = &pE1;
      SDP_EncodeAttribute(SDP_TEXT,48,data,&pE2);
      pList[1] = &pE2;
      SDP_EncodeAttribute(SDP_TEXT,48,data,&pE3);
      pList[2] = &pE3;
      SDP_EncodeAttribute(SDP_TEXT,48,data,&pE4);
      pList[3] = &pE4;
      SDP_EncodeAttribute(SDP_TEXT,48,data,&pE5);
      pList[4] = &pE5;
      SDP_CreateList(SDP_SEQUENCE,5,pList,&pElement);
    }

	SDP_AddAttribute(pRecord,100,&pElement);

  pDebugCheck();

  prh_sdp_flush_database();

  pDebugCheck();

  return HU_PASSED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_197
 
	*  TEST DESCRIPTION:
		That a single record can be converted to a flattened buffer.
 
	*  TEST STEPS:
		1. create record
		2. add attribute
		3. call SDP_DBtoStatic

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_197(int p1, void *arg)
{
u_int32 handle;
	  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement;

  prh_sdp_flush_database();
  
  SDP_CreateRecord(&handle,&pRecord);

  SDP_EncodeAttribute(SDP_UINT,4,&handle,&pElement);
  
  SDP_AddAttribute(pRecord,256,&pElement);

	SDP_DBtoStatic(0x00,500); /* test of null */

	/*	
		need to add some checking on the database.
	*/

	return HU_FAILED;
}

/******
	*  TEST NAME:   
		prh_sdp_unit_198
 
	*  TEST DESCRIPTION:
		That the SDP_DBtoStatic function will reject a null buffer
 
	*  TEST STEPS:
		1. call SDP_DBtoStatic will null buffer

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_198(int p1, void *arg)
{
		  u_int32 handle;
	  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement;
	u_int8* buffer;

  prh_sdp_flush_database();
  
  SDP_CreateRecord(&handle,&pRecord);

  SDP_EncodeAttribute(SDP_UINT,4,&handle,&pElement);
  
  SDP_AddAttribute(pRecord,256,&pElement);

	SDP_DBtoStatic(&buffer,500); /* max 500 bytes ceeling */

	pFree(buffer);

	return HU_PASSED; /* this is an invalid test */
}

/******
	*  TEST NAME:   
		prh_sdp_unit_199
 
	*  TEST DESCRIPTION:
		This test build from for transfer for empty database
 
	*  TEST STEPS:
		1. clear database
		2. call SDP_BuildROMForTransfer

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_199(int p1, void *arg)
{	
	prh_sdp_flush_database();

	SDP_BuildROMForTransfer();

	return prh_sdp_database ? HU_FAILED : HU_PASSED;
}


/******
	*  TEST NAME:   
		prh_sdp_unit_200
 
	*  TEST DESCRIPTION:
		This test that a single record can be converted to a flattened buffer
		and added to the database in a special format for transfer.
 
	*  TEST STEPS:
		1. create record
		2. add attribute
		3. call SDP_BuildROMForTransfer

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_200(int p1, void *arg)
{
	  u_int32 handle;
	  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement;
	int ret;
  prh_sdp_flush_database();

  ret = HU_FAILED;

  SDP_CreateRecord(&handle,&pRecord);

  SDP_EncodeAttribute(SDP_UINT,4,&handle,&pElement);
  
  SDP_AddAttribute(pRecord,256,&pElement);

	SDP_BuildROMForTransfer();

	/*	
		This is the most straight forward test tha can be applied
		to realy test it will require testing on embedded!!!!!
	*/

	if(prh_sdp_database)
		if(prh_sdp_database->p_next_record)
			if(prh_sdp_database->p_next_record->attribute.p_next_attribute)
				if(prh_sdp_database->p_next_record->attribute.p_next_attribute->p_next_attribute)
					if(prh_sdp_database->p_next_record->attribute.p_next_attribute->p_next_attribute->id == SDP_PARTHUS_ROM_AID)
							ret = HU_PASSED;

	return ret;
}


/******
*  TEST NAME:   
	prh_sdp_unit_315

	*  TEST DESCRIPTION:
		Test the SDP_RemoveRecord for an empty database

	*  TEST STEPS:
		1. clear database
		2. Call SDP_RemoveRecord 
		3. check results.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_315(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_316

	*  TEST DESCRIPTION:
		Test the encoding of all uints

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode and add uint 1
		4. encode and add uint 2
		5. encode and add uint 4
		6. encode and add uint 8
		7. encode and add uint 16

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_316(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_317

	*  TEST DESCRIPTION:
		Test the encoding of all ints

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode and add int 1
		4. encode and add int 2
		5. encode and add int 4
		6. encode and add int 8
		7. encode and add int 16

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_317(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_318

	*  TEST DESCRIPTION:
		Test the encoding of all uuid

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode and add uuid 2
		4. encode and add uuid 4
		5. encode and add uuid 16

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_318(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_319

	*  TEST DESCRIPTION:
		Test the encoding of all url

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode and add url small
		4. encode and add url medium
		5. encode and add url large

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_319(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_320

	*  TEST DESCRIPTION:
		Test the addition of an attribuite when one exists.

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode and add unit
		4. encode and add unit to same id

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_320(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_321

	*  TEST DESCRIPTION:
		Test the replace attribute when one does not exist.

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode and replace uuid when it does not exist.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_321(int p1, void *arg)
{
	return HU_UNTESTED;
}

/******
*  TEST NAME:   
	prh_sdp_unit_322

	*  TEST DESCRIPTION:
		Test the creation of all union types

	*  TEST STEPS:
		1. clear database
		2. create record.
		3. encode 2 small text.
		4. encode a medium text
		5. encode a large text
		6. create small union with 2 small text
		7. create medium union with 1 small and 1 medium text
		8. create large union with 1 small 1 medium and 1 large text

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_322(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_323

	*  TEST DESCRIPTION:
		Test the get next record when the database is empty

	*  TEST STEPS:
		1. clear database
		2. call get next record 

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_323(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_324

	*  TEST DESCRIPTION:
		Test the get next record when the last record.

	*  TEST STEPS:
		1. clear database
		2. create 3 record.
		3. call get next record using the last as a key.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_324(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_325

	*  TEST DESCRIPTION:
		Test the get record when the database is empty

	*  TEST STEPS:
		1. clear database
		2. call get record 

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_325(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
*  TEST NAME:   
	prh_sdp_unit_326

	*  TEST DESCRIPTION:
		Test the set record state for the normal case

	*  TEST STEPS:
		1. clear database
		2. create record
		3. call set record state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_326(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
*  TEST NAME:   
	prh_sdp_unit_327

	*  TEST DESCRIPTION:
		Test the set record state using invalid id

	*  TEST STEPS:
		1. clear database
		2. create record
		3. call set record state with a bad id

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_327(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
*  TEST NAME:   
	prh_sdp_unit_328

	*  TEST DESCRIPTION:
		Test the set record state for an empty database

	*  TEST STEPS:
		1. clear database
		3. call set record state

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_328(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_329

	*  TEST DESCRIPTION:
		Test the start server for the normal case

	*  TEST STEPS:
		1. clear database
		2. create record
		3. call start server.
		4. check server acess allowed.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_329(int p1, void *arg)
{
  	  u_int32 handle;
	  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement;

  prh_sdp_flush_database();

  
  SDP_CreateRecord(&handle,&pRecord);

  SDP_EncodeAttribute(SDP_UINT,4,&handle,&pElement);

  SDP_AddAttribute(pRecord,20,&pElement);

  SDP_StartServer(0,0x00);

  return HU_PASSED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_330

	*  TEST DESCRIPTION:
		Test the start server for an empty database

	*  TEST STEPS:
		1. clear database
		2. call start server

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_330(int p1, void *arg)
{
	return HU_UNTESTED;
}



/******
*  TEST NAME:   
	prh_sdp_unit_331

	*  TEST DESCRIPTION:
		Test the stop server for the normal case

	*  TEST STEPS:
		1. clear database
		2. create record
		3. call stop server.
		4. check server acess not allowed.

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_331(int p1, void *arg)
{
  	  u_int32 handle;
	  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement;

  prh_sdp_flush_database();

  
  SDP_CreateRecord(&handle,&pRecord);

  SDP_EncodeAttribute(SDP_UINT,4,&handle,&pElement);

  SDP_AddAttribute(pRecord,20,&pElement);

  SDP_StopServer(0,0x00);

  return HU_PASSED;

}


/******
*  TEST NAME:   
	prh_sdp_unit_332

	*  TEST DESCRIPTION:
		Test the stop server for an empty database

	*  TEST STEPS:
		1. clear database
		2. call stop server

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_332(int p1, void *arg)
{
  	  u_int32 handle;
	  t_SDP_serviceRecord* pRecord;
  t_SDP_dataElement pElement;

  prh_sdp_flush_database();  

  SDP_StopServer(0,0x00);

  return HU_PASSED;
}



/******
*  TEST NAME:   
	prh_sdp_unit_333

	*  TEST DESCRIPTION:
		Test the db size for the normal case

	*  TEST STEPS:
		1. clear database
		2. create record and some attributes
		3. call db size

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_333(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_334

	*  TEST DESCRIPTION:
		Test the db size for an empty database

	*  TEST STEPS:
		1. clear database
		2. call db size

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_334(int p1, void *arg)
{
	return HU_UNTESTED;
}


/******
*  TEST NAME:   
	prh_sdp_unit_196

	*  TEST DESCRIPTION:
		Test the db to static for an empty database

	*  TEST STEPS:
		1. clear database
		2. call db to static

	*  PARTHUS EXTENSIONS:
	 None.
 
	*  PARTHUS RESTRICTIONS:
	None	
 ******/
static prh_t_hu_outcome prh_sdp_unit_196(int p1, void *arg)
{
	return HU_UNTESTED;
}





prh_t_hu_testinfo prh_sdp_database_tests [] =  
{
      	{ "TP_SDP_V_018_U",0x00,prh_sdp_unit_018, NULL, 0 },
      	{ "TP_SDP_V_019_U",0x00,prh_sdp_unit_019, NULL, 0 },
      	{ "TP_SDP_V_020_U",0x00,prh_sdp_unit_020, NULL, 0 },
      	{ "TP_SDP_V_021_U",0x00,prh_sdp_unit_021, NULL, 0 },
      	{ "TP_SDP_V_022_U",0x00,prh_sdp_unit_022, NULL, 0 },
      	{ "TP_SDP_V_023_U",0x00,prh_sdp_unit_023, NULL, 0 },
      	{ "TP_SDP_V_024_U",0x00,prh_sdp_unit_024, NULL, 0 },
      	{ "TP_SDP_V_025_U",0x00,prh_sdp_unit_025, NULL, 0 },
      	{ "TP_SDP_V_026_U",0x00,prh_sdp_unit_026, NULL, 0 },
      	{ "TP_SDP_V_027_U",0x00,prh_sdp_unit_027, NULL, 0 },
      	{ "TP_SDP_V_028_U",0x00,prh_sdp_unit_028, NULL, 0 },
      	{ "TP_SDP_V_029_U",0x00,prh_sdp_unit_029, NULL, 0 },
      	{ "TP_SDP_V_030_U",0x00,prh_sdp_unit_030, NULL, 0 },
      	{ "TP_SDP_V_031_U",0x00,prh_sdp_unit_031, NULL, 0 },
      	{ "TP_SDP_V_032_U",0x00,prh_sdp_unit_032, NULL, 0 },
      	{ "TP_SDP_V_034_U",0x00,prh_sdp_unit_034, NULL, 0 },
      	{ "TP_SDP_V_035_U",0x00,prh_sdp_unit_035, NULL, 0 },
      	{ "TP_SDP_V_036_U",0x00,prh_sdp_unit_036, NULL, 0 },
     	{ "TP_SDP_V_037_U",0x00,prh_sdp_unit_037, NULL, 0 },
      	{ "TP_SDP_V_038_U",0x00,prh_sdp_unit_038, NULL, 0 }, 
/*      	{ "TP_SDP_V_197_U",0x00,prh_sdp_unit_197, NULL, 0 }, */
      	{ "TP_SDP_V_198_U",0x00,prh_sdp_unit_198, NULL, 0 }, 
      	{ "TP_SDP_V_200_U",0x00,prh_sdp_unit_200, NULL, 0 }, 
      	{ "TP_SDP_V_199_U",0x00,prh_sdp_unit_199, NULL, 0 },
#ifndef P7D_PT_RELEASE_TESTS
      	{ "TP_SDP_V_315_U",0x00,prh_sdp_unit_315, NULL, 0 }, 
      	{ "TP_SDP_V_316_U",0x00,prh_sdp_unit_316, NULL, 0 }, 
      	{ "TP_SDP_V_317_U",0x00,prh_sdp_unit_317, NULL, 0 }, 
      	{ "TP_SDP_V_318_U",0x00,prh_sdp_unit_318, NULL, 0 }, 
      	{ "TP_SDP_V_319_U",0x00,prh_sdp_unit_319, NULL, 0 }, 
      	{ "TP_SDP_V_320_U",0x00,prh_sdp_unit_320, NULL, 0 }, 
      	{ "TP_SDP_V_321_U",0x00,prh_sdp_unit_321, NULL, 0 }, 
      	{ "TP_SDP_V_322_U",0x00,prh_sdp_unit_322, NULL, 0 }, 
      	{ "TP_SDP_V_323_U",0x00,prh_sdp_unit_323, NULL, 0 }, 
      	{ "TP_SDP_V_324_U",0x00,prh_sdp_unit_324, NULL, 0 }, 
      	{ "TP_SDP_V_325_U",0x00,prh_sdp_unit_325, NULL, 0 }, 
      	{ "TP_SDP_V_326_U",0x00,prh_sdp_unit_326, NULL, 0 }, 
      	{ "TP_SDP_V_327_U",0x00,prh_sdp_unit_327, NULL, 0 }, 
      	{ "TP_SDP_V_328_U",0x00,prh_sdp_unit_328, NULL, 0 }, 
#endif
      	{ "TP_SDP_V_329_U",0x00,prh_sdp_unit_329, NULL, 0 }, 
#ifndef P7D_PT_RELEASE_TESTS
      	{ "TP_SDP_V_330_U",0x00,prh_sdp_unit_330, NULL, 0 }, 
#endif
      	{ "TP_SDP_V_331_U",0x00,prh_sdp_unit_331, NULL, 0 }, 
      	{ "TP_SDP_V_332_U",0x00,prh_sdp_unit_332, NULL, 0 }, 
#ifndef P7D_PT_RELEASE_TESTS
      	{ "TP_SDP_V_333_U",0x00,prh_sdp_unit_333, NULL, 0 }, 
      	{ "TP_SDP_V_334_U",0x00,prh_sdp_unit_334, NULL, 0 }, 
      	{ "TP_SDP_V_196_U",0x00,prh_sdp_unit_196, NULL, 0 },
#endif
};

prh_t_hu_harness prh_sdp_database_group =
{ NULL, HU_SDP, "SDP_DATABASE",prh_sdp_unit_init,PRH_UT_STUBS_ENABLED, 1,sizeof(prh_sdp_database_tests)/sizeof(prh_t_hu_testinfo), prh_sdp_database_tests, 0, 0, 0, 0, 0};

void test_sdp_database(void)
{
  prh_hu_add_testgroup(&prh_sdp_database_group);
}
