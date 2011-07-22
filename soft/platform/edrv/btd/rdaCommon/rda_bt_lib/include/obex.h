/*
  response codes
 */

#define OBEX_CONTINUE 0x90

#define OBEX_OK 0xa0
#define OBEX_CREATED 0xa1
#define OBEX_ACCEPTED 0xa2
#define OBEX_NON_AUTH_INFO 0xa3
#define OBEX_NO_CONTENT 0xa4
#define OBEX_RESET_CONTENT 0xa5
#define OBEX_PARTIAL_RESET 0xa6

#define OBEX_MULTIPLE_CHOICE 0xb0
#define OBEX_MOVED_PERM 0xb1
#define OBEX_MOVED_TEMP 0xb2
#define OBEX_SEE_OTHER 0xb3
#define OBEX_NOT_MODIFIED 0xb4
#define OBEX_USE_PROXY 0xb5

#define OBEX_BAD_REQUEST 0xc0
#define OBEX_UNAUTHORIZED 0xc1
#define OBEX_PAYMENT_REQUIRED 0xc2
#define OBEX_FORBIDDEN 0xc3
#define OBEX_NOT_FOUND 0xc4
#define OBEX_METHOD_NOT_ALLOWED 0xc5
#define OBEX_NOT_ACCEPTABLE 0xc6
#define OBEX_PROXY_AUTH_REQUIRED 0xc7
#define OBEX_REQUEST_TIME_OUT 0xc8
#define OBEX_CONFLICT 0xc9
#define OBEX_GONE 0xca
#define OBEX_LENGTH_REQUIRED 0xcb
#define OBEX_PRECONDITION_FAIL 0xcc
#define OBEX_REQUEST_ENTITY_TOO_LARGE 0xcd
#define OBEX_REQUEST_URL_TOO_LARGE 0xce
#define OBEX_UNSUPPORTED_MEDIA_TYPE 0xcf

#define OBEX_SERVER_ERROR 0xd0
#define OBEX_NOT_IMPLEMENTED 0xd1
#define OBEX_BAD_GATEWAY 0xd2
#define OBEX_SERVICE_UNAVAILABLE 0xd3
#define OBEX_GATEWAY_TIMEOUT 0xd4
#define OBEX_HTTP_VER_NOT_SUPPORTED 0xd5

#define OBEX_DATABASE_FULL 0xe0
#define OBEX_DATABASE_LOCKED 0xe1

/*
  Operation Opcodes
 */

#define OBEX_CONNECT 0x80
#define OBEX_DISCONNECT 0x81
#define OBEX_PUT_FINAL 0x82
#define OBEX_PUT 0X02
#define OBEX_GET 0x03
#define OBEX_GET_FINAL 0x83
#define OBEX_SETPATH 0x85
#define OBEX_RESERVED 0x04
#define OBEX_RESERVED_FINAL 0x84
#define OBEX_SETPATH 0x85
#define OBEX_ABORT 0xff
#define OBEX_OP_RESERVED_LOWER 0x06
#define OBEX_OP_RESERVER_UPPER 0x0f
#define OBEX_OP_USER_LOWER 0x10
#define OBEX_OP_USER_UPPER 0x1f

/* header codes */

#define OBEX_COUNT 0xc0
#define OBEX_NAME 0x01
#define OBEX_TYPE 0x42
#define OBEX_LENGTH 0xc3
#define OBEX_TIME_ISO 0x44
#define OBEX_TIME_32 0xc4
#define OBEX_DESCRIPTION 0x05
#define OBEX_TARGET 0x46
#define OBEX_HTTP 0x47
#define OBEX_BODY 0x48
#define OBEX_END_BODY 0x49
#define OBEX_WHO 0x4a
#define OBEX_CID 0xcb
#define OBEX_APP_PARAM 0x4c
#define OBEX_AUTH_CHALLANGE 0x4d
#define OBEX_AUTH_RESPONSE 0x4e
#define OBEX_CLASS 0x4f
#define OBEX_HD_RESERVED_LOWER 0x10
#define OBEX_HD_RESERVER_UPPER 0x2f
#define OBEX_HD_USER_LOWER 0x30
#define OBEX_HD_USER_UPPER 0x3f

#define OBEX_HEADER 3
#define OBEX_SETPATH_HEADER 5
#define OBEX_CONNECT_HEADER 7

/* supported types */

#define OBEX_PUSH_VCARD_21 1
#define OBEX_PUSH_VCARD_30 2
#define OBEX_PUSH_VCAL_10 4
#define OBEX_PUSH_ICAL_20 8
#define OBEX_PUSH_VNOTE : 16
#define OBEX_PUSH_VMESSAGE : 32
#define OBEX_PUSH_ANY : 64

/* supported datastores */

#define OBEX_SYNC_PB 1
#define OBEX_SYNC_CAL 2
#define OBEX_SYNC_NT 4
#define OBEX_SYNC_MSG 8

/* service type */

#define OBEX_INBOX_SERVICE
#define OBEX_CAPABILITY_SERVICE
#define OBEX_IRMC_SERVCE

/* object types */

#define OBEX_VCARD 1
#define OBEX_VCAL  2
#define OBEX_VNOTE 3
#define OBEX_VMESS 4
#define OBEX_VOTHER 5

static const u_int8 OBEX_FTP_UUID	[16] = {
	0xf9,0xec,0x7b,0xc4,0x95,0x3c,0x11,0xd2,0x98,0x4e,0x52,0x54,0x00,0xdc,0x9e,0x09};

typedef void (*t_obex_command_handler)(u_int8 status, u_int8* data, u_int16 length);


struct prh_obex_command_callback {
  u_int32 tid;
  t_obex_command_handler callback;
  struct prh_obex_command_callback* p_next;
};	


typedef struct st_t_obex_header_offsets
{
	u_int16 count,
					name,
					type,
					len,
					time_iso,
					time_32,
					des,
					tar,
					http,
					body,
					eob,
					who,
					cid,
					app,
					au_cal,
					au_rsp,
					cla;
}t_obex_header_offsets;

typedef struct{
	
	u_int16 nameOffset;
	u_int16 typeOffset;
	u_int16 bodyOffset;

} t_obex_headers;

/* Header encoding functions*/
APIDECL1 int APIDECL2 OBEX_CreateStringHeader(u_int8 id, u_int16 length, u_int16* pData, t_DataBuf *txBuffer, u_int16* pOffset);
APIDECL1 int APIDECL2 OBEX_CreateSequenceHeader(u_int8 id, u_int16 length, u_int8* pData, t_DataBuf *txBuffer, u_int16* pOffset);
APIDECL1 int APIDECL2 OBEX_CreateByteHeader(u_int8 id, u_int8 data, t_DataBuf* txBuffer, u_int16* pOffset);
APIDECL1 int APIDECL2 OBEX_Create4ByteHeader(u_int8 id, u_int32 data, t_DataBuf *txBuffer, u_int16* pOffset);
APIDECL1 int APIDECL2 OBEX_ExtractHeaders(t_obex_header_offsets* headers,u_int16* offset,u_int8* finger, u_int16 len);


/* Write Buffer functions*/
APIDECL1 int APIDECL2 OBEX_GetWriteBuffer(u_int16 len, t_DataBuf **writeBuffer);
APIDECL1 int APIDECL2 OBEX_FreeWriteBuffer(t_DataBuf *writeBuffer);


typedef void (*t_obex_service_handler)(u_int32 tid_s, u_int8 messageType, t_obex_headers headers, u_int8 *data,u_int16 len,void* cbref);


struct prh_obex_session_state
{
  u_int32 tid;
  u_int32 server_tid;
  u_int8 state;
  t_obex_service_handler handler;
  struct prh_obex_session_state* p_next;
};

struct prh_obex_inbox_registration
{
  u_int32 tid;
  t_obex_service_handler handler;
	struct prh_obex_inbox_registration * p_next;
};

struct prh_obex_service_registration
{
  u_int32 tid;
  t_obex_service_handler handler;
  u_int8* target;
  u_int8 target_length;
  u_int32 cid;
	u_int8 cid_length;
  struct prh_obex_service_registration* p_next;
};


APIDECL1 int APIDECL2 OBEX_Connect(u_int32 tid_s, u_int8 version, u_int8 flags, u_int16 max_length, u_int16 target_length, u_int8* target, u_int16 header_length, t_DataBuf *txbuffer, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_Disconnect(u_int32 tid_s, u_int16 header_length, t_DataBuf *tx_buffer, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_Put(u_int32 tid_s, u_int8 final, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_Get(u_int32 tid_s, u_int8 final, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_SetPath(u_int32 tid_s, u_int8 flags, u_int8 constants, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_Abort(u_int32 tid_s, u_int16 header_length, t_DataBuf *txBuffer, t_obex_command_handler handler);

APIDECL1 int APIDECL2 OBEX_GetMTU(u_int32 tid_s,u_int16* mtu);
/* the connection routines */
APIDECL1 int APIDECL2 OBEX_TransportConnect_RF(t_bdaddr address,u_int8 server_channel, u_int32* tid_b, u_int16 mtu,void (*disconnect)(u_int32 tid_s));
/*int OBEX_TransportConnect_Serial(port);
int OBEX_TransportConnect_Socket(address,port);*/


APIDECL1 int APIDECL2 OBEX_TransportDisconnect(u_int32 tid_s);


APIDECL1 int APIDECL2 OBEX_RegisterInbox(u_int32 tid_s,t_obex_service_handler handler, void *cref);
APIDECL1 int APIDECL2 OBEX_RegisterServerToInbox(u_int32 tid_s, u_int8 type, t_obex_service_handler handler);
APIDECL1 int APIDECL2 OBEX_RegisterService(u_int32 tid_s,t_obex_service_handler handler, u_int8 length, u_int8* target, void* cref);
APIDECL1 int APIDECL2 OBEX_Response(u_int32 tid_s, u_int8 response_code,u_int16 header_length, t_DataBuf* txbuffer);
APIDECL1 int APIDECL2 OBEX_ConnectResponse(u_int32 tid_s, u_int8 response_code, u_int8 version, u_int8 flags,u_int16 maxLen, u_int16 header_length, t_DataBuf* txbuffer);
APIDECL1 int APIDECL2 OBEX_GetCID(u_int32 tid_s,u_int8 target_length,u_int8* target,u_int32* cid);
APIDECL1 int APIDECL2 OBEX_ServerTransportDisconnect(u_int32 tid_s);

/* the listen routines must return back a transport identifier that can
be used in the service registration */
#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.20
APIDECL1 int APIDECL2 OBEX_TransportListen_RF(u_int8 server_channel,u_int32* tid,void (*disconnect)(u_int32 tid));
#else
APIDECL1 int APIDECL2 OBEX_TransportListen_RF(u_int8 server_channel,u_int32* tid, u_int16 max_obex,u_int8* rx_buffer,void (*disconnect)(u_int32 tid), void (*callbackFunc)(t_bdaddr address),void (*authorizecallbackFunc)(t_bdaddr address, u_int8 dlci, u_int8 schan, u_int8 tid_s));
#endif
APIDECL1 int APIDECL2 OBEX_GetConnectionInfo_RF(u_int32 tid,t_bdaddr *address,u_int8 *server_channel);

typedef u_int8 t_obex_nonce[18];
typedef u_int8 t_obex_digest[18];

#define PRH_MAX_PINCODE_SIZE 20

enum OBEX_SecurityLevel{ NOSECURITY, AUTHENTICATION};

void CallSecurityCallback(void);

/*APIDECL1 int APIDECL2 OBEX_RegisterSecurityLevel(u_int8 secLevel, char *userID, char *localUserPin, void (*SecCallback)(u_int8 cid, u_int16 serverChanNum, u_int8 status));*/
/*APIDECL1 int APIDECL2 OBEX_EnterAuthInfo(char *userID, char *remoteUserPin, u_int8 cid, u_int16 serverChanNum);*/
APIDECL1 int APIDECL2 OBEX_GenerateNonce(u_int8 pinLen, u_int8 *pinCode, u_int8 *nonce);
APIDECL1 int APIDECL2 OBEX_GenerateDigest(u_int8 pinLen, u_int8 *pinCode, u_int8 *nonce, u_int8 *digest);
APIDECL1 int APIDECL2 OBEX_AuthRequest(u_int8 *localPinCode, u_int8 localPinCodeLen, t_obex_nonce nonce, u_int16 *offset_len, t_DataBuf *txBuffer);
APIDECL1 int APIDECL2 OBEX_AuthResponse(u_int8 *remotePinCode, u_int8 remotePinCodeLen, u_int8 *remoteNonceHeader, u_int16 *offset_len, t_DataBuf *txBuffer);
APIDECL1 int APIDECL2 OBEX_AuthCheck(u_int8 *localPinCode, u_int8 localPinCodeLen, t_obex_nonce localNonce, u_int8 *digestHeader);


APIDECL1 int APIDECL2 OBEX_ObjectPush(u_int32 tid, u_int8 type, u_int16 *name, u_int16 nameLen, u_int8 *data, u_int16 len, u_int32 totalLen,u_int8 final,t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_ObjectPull(u_int32 tid, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_ObjectExchange(u_int32 tid, u_int16 *name, u_int16 nameLen, u_int8 *data, u_int16 len, u_int32 totalLen,t_obex_command_handler handler);
#if (SINGLE_THREAD_FLAG==0)  // jiancui added it. 2008.2.20
APIDECL1 int APIDECL2 OBEX_FindObjectPushServer(t_bdaddr* address,u_int8* channel,u_int8* formats);
#else
APIDECL1 int APIDECL2 OBEX_FindObjectPushServer(t_bdaddr* address,u_int8* channel,u_int8* formats, void (*callbackFunc)(t_bdaddr address,u_int8 channel,u_int8* formats));
#endif
APIDECL1 u_int8 OBEX_L2_ACL_Expired(t_bdaddr* p_bd_addr) ;


/*APIDECL1 int APIDECL2 OBEX_PushConnectResponse(u_int32 tid, u_int16 maxLength, u_int8 responseCode);*/
APIDECL1 int APIDECL2 OBEX_PullResponse(u_int32 tid_s, u_int8 responseCode, u_int16 *name, u_int8 nameLen, u_int8* data, u_int16 dataLen, u_int16 totalLen);
APIDECL1 int APIDECL2 OBEX_RegisterObjectPushServer(u_int8 channel, u_int8 supported_types);

#if (SINGLE_THREAD_FLAG==0) 
APIDECL1 int APIDECL2 OBEX_FindFTPServer(t_bdaddr* address,u_int8* channel);
#else
APIDECL1 int APIDECL2 OBEX_FindFTPServer(t_bdaddr* address,u_int8* channel,u_int8* formats, void (*callbackFunc)(t_bdaddr address,u_int8 channel,u_int8* formats));
#endif
APIDECL1 int APIDECL2 OBEX_FTPConnect(u_int32 tid_s, u_int16 mtu, t_DataBuf *txBuffer,u_int16 headerLength, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_Delete(u_int32 tid_s, u_int32 cid, u_int16* name, u_int8 length, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_CreateFolder(u_int32 tid_s,u_int32 cid, u_int16* name,u_int8 length, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_ChooseCurrentFolder(u_int32 tid_s, u_int32 cid, u_int16* name, u_int8 length,t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_FTPPushFile(u_int32 tid_s, u_int32 cid, u_int16 *name, u_int16 nameLen, u_int8 *data, u_int16 dataLen, u_int32 totalLen,u_int8 final,t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_FTPPullFile(u_int32 tid_s, u_int32 cid, u_int16 *name, u_int16 nameLen, t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_GetFolderListingObject(u_int32 tid_s, u_int32 cid, /*u_int16 *name, u_int16 nameLen,*/ t_obex_command_handler handler);

APIDECL1 int APIDECL2 OBEX_RegisterFTPServer(u_int8 channel);
APIDECL1 int APIDECL2 OBEX_FTPConnectResponse(u_int32 tid_s, u_int16 maxLength,u_int8 responseCode);
APIDECL1 int APIDECL2 OBEX_ChooseCurrentFolderResponse(u_int32 tid_s, u_int8 responseCode, u_int8 *folderListingOb, u_int8 floLen, u_int16 totalLen);
APIDECL1 int APIDECL2 OBEX_FTPPullResponse(u_int32 tid_s, u_int8 responseCode, u_int16 *name, u_int8 nameLen, u_int16 totalLen, u_int8* data, u_int16 dataLen);

APIDECL1 int APIDECL2 OBEX_RegisterSyncCommandServer(u_int8 channel);
APIDECL1 int APIDECL2 OBEX_FindSyncServer(t_bdaddr* address,u_int8* channel,u_int8* formats);

APIDECL1 int APIDECL2 OBEX_SuggestSync(u_int32 tid_s, u_int8 type, char* pAny, u_int8 length,t_obex_command_handler handler);
APIDECL1 int APIDECL2 OBEX_RegisterSyncServer(u_int8 channel, u_int8 supported_types);
APIDECL1 int APIDECL2 OBEX_FindSyncCommandServer(t_bdaddr* address,u_int8* channel);
APIDECL1 int APIDECL2  OBEX_Init_Variable(void);
