/*
  test file for obex_sync_server.c
 */

#include "host_config.h"

#include "host_types.h"
#include "obex_server_types.h"
#include "obex_client_types.h"
#include "obex_tal.h"
#include "obex_defines.h"
#include "obex_sync_server.h"
#include "obex_utils.h"
#include "mgr_utils.h"

#include "cunit.h"

extern u_int32 unit_test_state1;
extern u_int32 unit_test_state2;
extern u_int32 unit_test_state3;
extern u_int8* unit_test_pointer1;
extern u_int8* unit_test_pointer2;
extern u_int8* unit_test_pointer3;


extern struct prh_obex_transport* prh_obex_transmit_transport;
extern struct prh_obex_session_state* prh_obex_state;
extern struct prh_obex_command_callback* prh_obex_client_callbacks;

void test_OBEX_SuggestSync(void);

void test_obex_sync_server(void)
{
  cuRegTest("OBEX_SuggestSync",1,test_OBEX_SuggestSync);
}



void test_OBEX_SuggestSync(void)
{
	u_int32 tid;
	u_int8 type;
	char *additional;
	u_int8 length;
	t_obex_command_handler handler;
	t_DataBuf* buff;

	handler = 0x00;
	tid = 0;
	type = 0;
	additional = 0;
	length = 0;

	OBEX_SuggestSync(tid, type, additional, length,handler);

	buff = (t_DataBuf*)unit_test_pointer1;

	OBEX_FreeWriteBuffer(buff);


	handler = 0x00;
	tid = 0;
	type = 1;
	additional = 0;
	length = 0;

	OBEX_SuggestSync(tid, type, additional, length,handler);

	buff = (t_DataBuf*)unit_test_pointer1;

	OBEX_FreeWriteBuffer(buff);


	handler = 0x00;
	tid = 0;
	type = 2;
	additional = 0;
	length = 0;

	OBEX_SuggestSync(tid, type, additional, length,handler);

	buff = (t_DataBuf*)unit_test_pointer1;

	OBEX_FreeWriteBuffer(buff);


	handler = 0x00;
	tid = 0;
	type = 4;
	additional = 0;
	length = 0;

	OBEX_SuggestSync(tid, type, additional, length,handler);

	buff = (t_DataBuf*)unit_test_pointer1;

	OBEX_FreeWriteBuffer(buff);

		handler = 0x00;
	tid = 0;
	type = 8;
	additional = 0;
	length = 0;

	OBEX_SuggestSync(tid, type, additional, length,handler);

	buff = (t_DataBuf*)unit_test_pointer1;

	OBEX_FreeWriteBuffer(buff);


		handler = 0x00;
	tid = 0;
	type = 16;
	additional = 0;
	length = 0;

	OBEX_SuggestSync(tid, type, additional, length,handler);

	buff = (t_DataBuf*)unit_test_pointer1;

	OBEX_FreeWriteBuffer(buff);


		handler = 0x00;
	tid = 0;
	type = 32;
	additional = 0;
	length = 0;

	OBEX_SuggestSync(tid, type, additional, length,handler);

	buff = (t_DataBuf*)unit_test_pointer1;

	OBEX_FreeWriteBuffer(buff);

		handler = 0x00;
	tid = 0;
	type = 64;
	additional = 0;
	length = 0;

	OBEX_SuggestSync(tid, type, additional, length,handler);

	buff = (t_DataBuf*)unit_test_pointer1;

	OBEX_FreeWriteBuffer(buff);


		handler = 0x00;
	tid = 0;
	type = 1+2+4+8+16+32+64;
	additional = 0;
	length = 0;

	OBEX_SuggestSync(tid, type, additional, length,handler);

	buff = (t_DataBuf*)unit_test_pointer1;

	OBEX_FreeWriteBuffer(buff);

}
