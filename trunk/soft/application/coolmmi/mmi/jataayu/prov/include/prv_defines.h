#ifndef	PRV_DEFNS_H
#define	PRV_DEFNS_H

#include <jdi_communicator.h>
#include "cfw.h"


#define PROV_MAX_SCREEN_BUFF		5*1024

#define WAP_MAX_PROFILE_PASSWD_LEN		15

#define E_PRV_MAX_RETRIES		3




#define E_DC_MAX_PROFILE_NAME        15      /* Maximum length of profile name */
#define E_DC_MAX_URL_LEN             255     /* Maximum length of URL */
#define E_DC_MAX_IP_LEN              15      /* Maximum length of IP address  */
#define E_DC_MAX_PHONE_NUMBER_LEN    21      /* Maximum length of phone number  */ 
#define E_DC_MAX_NAPID_LEN           15      /* Maximum length of nap id  */
#define E_DC_MAX_PROXYID_LEN         15      /* Maximum length of proxy id  */


#define E_DC_FLAG_USED               0x01    /* The profile is in use. */
#define E_DC_FLAG_PRESTORE           0x02    /* The profile is prestored */
#define E_DC_FLAG_PROVISION          0x04    /* The profile is OTA provision. */


/* Bearer type  */

typedef enum
{
    E_DC_BEARER_CSD = 0,         /* CSD */
    E_DC_BEARER_GPRS,            /*GPRS */
    E_DC_BEARER_MAX             
}E_DC_BEARER;

/* connection type  */

typedef enum
{
    E_DC_CONNECTION_ORIENTED = 0,        /*WSP connection */
    E_DC_CONNECTION_ORIENTED_SECURE,     /* WSP connection with secure*/
    E_DC_WPHTTP_PROXY,                   /* HTTP connection with proxy*/
    E_DC_WPHTTP_DIRECT,                  /* HTTP direct connection */
    E_DC_CONN_MODE_MAX                  
}E_DC_CONN_MODE;

/* call type */
typedef enum
{
    E_DC_CSD_CALL_ISDN = 0,      /*ISDN */
    E_DC_CSD_CALL_ANALOG,        /*call type */
    E_DC_CSD_CALL_MAX           
}E_DC_CSD_CALL;

typedef enum
{
	GPRS_AUTH_PAP ,
	GPRS_AUTH_CHAP ,	
	GPRS_AUTH_NONE 
}E_DC_GPRS_AUTH_TYPE;



typedef struct st_datacon_profile
{
    UINT32  profile_id;                                 /* Profile unique id */
    INT16   profile_name[E_DC_MAX_PROFILE_NAME + 1];     /* Profile name */
    INT8    homepage[E_DC_MAX_URL_LEN + 1];              /* Home page */
    INT8    apn[MAX_DATA_CONN_APN_SIZE + 1];                   /* Access point name */
    INT8    username[MAX_DATA_CONN_USERNAME_SIZE + 1];         /* Username */
    INT8    password[MAX_DATA_CONN_PASSWORD_SIZE + 1];           /* Password */
    INT8    proxy_ip[E_DC_MAX_IP_LEN + 1];               /*IP address */
    INT8    dialup_number[E_DC_MAX_PHONE_NUMBER_LEN + 1];/*CSD dialup number */
    INT8    dns1[E_DC_MAX_IP_LEN + 1];                   /*DNS1 */
    INT8    dns2[E_DC_MAX_IP_LEN + 1];                   /* DNS2 */
    INT8    nap_id[E_DC_MAX_NAPID_LEN + 1];              /*Nap id */
    INT8    proxy_id[E_DC_MAX_PROXYID_LEN + 1];          /* Proxy id */

    UINT32  baudrate;       /* CSD buad rate. */
    UINT32  timeout;        /* CSD timeout interval. */
    UINT16  port;           /* Port number. */

    E_DC_BEARER        bear_type;                      /* Bearer type */
    E_DC_CONN_MODE     conn_mode;                      /* Connection mode */
    E_DC_CSD_CALL      csd_call_type;                  /* CSD call type */
	
    E_DC_GPRS_AUTH_TYPE      gprs_auth_type;           /* GPRS auth type */
	
    //UINT32  profile_type;   /* Profile type. */
 }DATACONN_PROFILE;

typedef struct _fetch_req_list_
{
	JC_UINT32				ulReferenceID ;
	E_HTTP_STATUS			eStatus ;
	JC_UINT32				ulCurSize ;
	JC_INT8					*pszContentType ;
	JC_UINT8				*pucbuffer ;
	struct _fetch_req_list_	*pNext ;
} ST_FETCH_REQ_LIST ;

typedef struct provInfo
{
	JC_UINT32		uiRequestID ;
	JC_UINT8 		*pucBuffer ;
	JC_UINT32		uiBufferLen ;
	JC_UINT8 		*pucHeader ;
	JC_UINT32		uiHeaderLen ;
	JC_BOOLEAN		bIsHTTPHeader ;
	CFW_SIM_ID 		nSimID ;
	struct provInfo	*pstNext ;
} ST_PROV_INFO ;


/**
 * @brief This enumeration specifies the type of the profile.
 */
typedef enum e_prov_profile_type 
{
	E_PROF_BROWSER, 
	E_PROF_MMS 
}E_PROV_PROFILE_TYPE ; 

/**
 * @struct	ST_PROV_INFO_LIST
 * @brief	Provisioning profile info list
 */
typedef struct provInfoList
{
	DATACONN_PROFILE		stProfile ;	
	E_PROV_PROFILE_TYPE		eProfType ;
	struct provInfoList		*pstNext ;
} ST_PROV_INFO_LIST;

/**
 * @brief This enumeration specifies the type of settings received.
 */
typedef enum provConfirmType 
{
	E_PROV_CONFIRM_NEW_SETTINGS, 
	E_PROV_CONFIRM_UPDATE_SETTINGS,
	E_PROV_CONFIRM_DOWNLOAD
}E_PROV_CONFIRM_TYPE ;



/**
 * @struct	ST_PROV_CONTEXT
 * @brief	Provisioning context
 */
typedef struct provContext
{
	JC_HANDLE			vProvHandle ;
	E_PROV_DATA_TYPE	eProvType ;
	E_SEC_TYPE			eSecType ;
	JC_INT8 			*pcProvURL ;
	JC_HANDLE 			vCommHandle ;
	//ST_PROV_INFO		*pstProveInfo ;
	ST_PROV_INFO_LIST	*pstProfList ;
	ST_PROV_INFO_LIST	*pstContProfList ;
	//ST_PROV_INFO_LIST	*pstProfNode;
	//E_PROV_STATUS       eProvStatus;
	//E_PROV_CONFIRM_TYPE	eConfirmType ;
	//ST_PROV_INFO_LIST	*pstCurProfile ;
	JC_CHAR				*pmPinValue ;
	JC_UINT8			ucNumOfAttempts ;
	JC_CHAR				szScreeBuff[PROV_MAX_SCREEN_BUFF];
	JC_BOOLEAN			bEndKeyPressed;
	ST_PROV_INFO		*pstProvInfo;
	struct _st_cs_profile_node  *pstCsProfInfo; // new 
	struct _st_cs_profile_node  *pstCurrCsProfInfo; // new 
} ST_PROV_CONTEXT ;


#define  PRV_PORT_9200						9200
#define  PRV_PORT_9201						9201
#define  PRV_PORT_443						443	
#define  PRV_PORT_9202						9202
#define  PRV_PORT_9203						9203
#define  PRV_PORT_80						80
#define  PRV_PORT_9401						9401
#define  PRV_PORT_3128						3128
#define  PRV_PORT_8080						8080
#define	 PRV_PORT_8008						8008
#define	 PRV_DEFAULT_TIMEOUT				60 
#define  PRV_DEFAULT_BAUDRATE				9600


extern ST_PROV_CONTEXT gstProvContext ;



#endif //PRV_DEFNS_H
