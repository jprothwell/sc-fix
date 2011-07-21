#ifndef	PRV_PLATFORM_H
#define	PRV_PLATFORM_H

#include <jdi_communicator.h>


typedef enum
{
	E_CS_DATACONN_PROFILE,
	E_CS_PROXY_PROFILE

}E_CS_PROFILE_TYPE;



typedef struct _st_cs_profile_node
{
	E_CS_PROFILE_TYPE eCSProfileType;
	JC_BOOLEAN IsProfileDiscarded;
	void * pProfile;
	struct _st_cs_profile_node *pNext;
	struct _st_cs_profile_node *pPrev;
}ST_CS_PROFILE_NODE;




typedef struct _st_cs_datacon_profile
{
    INT8    apn[MAX_DATA_CONN_APN_SIZE + 1];            /* Access point name */
    INT8    username[MAX_DATA_CONN_USERNAME_SIZE + 1];  /* Username */
    INT8    password[MAX_DATA_CONN_PASSWORD_SIZE + 1];  /* Password */
  
    INT8    nap_id[E_DC_MAX_NAPID_LEN + 1];             /*Nap id */
	
 
    E_DC_BEARER        bear_type;                      /* Bearer type */
    E_DC_GPRS_AUTH_TYPE      gprs_auth_type;           /* GPRS auth type */
}ST_CS_DATACONN_PROFILE;

typedef struct _st_cs_datacon_profile_name
{  
    INT8    nap_id[(E_DC_MAX_NAPID_LEN + 1)*2];             /*Nap id */	
 
}ST_CS_DATACONN_PROFILE_NAME;

typedef struct _st_cs_proxy_profile
{
    INT8    homepage[E_DC_MAX_URL_LEN + 1];              /* Home page */

    INT8    proxy_ip[E_DC_MAX_IP_LEN + 1];               /*IP address */

    INT8    dns1[E_DC_MAX_IP_LEN + 1];                   /*DNS1 */
    INT8    dns2[E_DC_MAX_IP_LEN + 1];                   /* DNS2 */
   
    INT8    proxy_id[E_DC_MAX_PROXYID_LEN + 1];          /* Proxy id */
	INT8    nap_id[E_DC_MAX_NAPID_LEN + 1];             /*Nap id  corresponding to data connection profile */
   
    UINT16  port;           /* Port number. */
    E_DC_CONN_MODE     conn_mode;                      /* Connection mode */
    E_PROV_PROFILE_TYPE  profile_type;   /* Profile type. */
	CFW_SIM_ID 		nSimID ;
}ST_CS_PROXY_PROFILE;

typedef struct _st_cs_proxy_profile_name
{
    INT8    proxy_id[(E_DC_MAX_PROXYID_LEN + 1)*2];          /* Proxy id */
}ST_CS_PROXY_PROFILE_NAME;


//Interface to check if there exists any data connection profile with same nap_id.
//True - if all fields are macthed
//False- if all fields did not match
JC_BOOLEAN cs_DataConnProfileExist(ST_CS_DATACONN_PROFILE *pstDataConn, JC_UINT8 *pIndex);

//Interface to check if there exists any profile with same proxy_id.
//True - if all fields are macthed
//False- if all fields did not match
JC_BOOLEAN cs_ProxyProfileExist(ST_CS_PROXY_PROFILE *pstProxyProfile, JC_INT8* pIndex);

//Interface to save/update the data conn profile .
//Updation is done if the nap_id matches and other fields differ

JC_RETCODE cs_SaveDataConnProfile(ST_CS_DATACONN_PROFILE *pstProxyProfile, INT16 index);

//replacing/saving  the datconn profile at a specific index
JC_RETCODE cs_ReplaceDataConnProfile(ST_CS_DATACONN_PROFILE *pstProxyProfile, INT8 index);


//Interface to save/update the proxy profile .
//Updation is done if the proxy_id matches and other fields differ

JC_RETCODE cs_SaveProxyProfile(ST_CS_PROXY_PROFILE *pstProxyProfile, INT16 index);

//replacing/saving  the proxy profile at a specific index

JC_RETCODE cs_ReplaceProxyProfile(ST_CS_PROXY_PROFILE *pstProxyProfile, INT8 index);

//gets the no.of maximum proxy proifles supported by platform
JC_INT32 cs_GetMaxProxyProfiles(E_PROV_PROFILE_TYPE  profile_type);

//gets the no.of maximum data conn proifles supported by platform
JC_INT32 cs_GetMaxDataConnProfiles();


//gets the total no.of maximum proxy proifles settings filled currently
JC_INT32 cs_GetTotalProxyProfiles(E_PROV_PROFILE_TYPE  profile_type, CFW_SIM_ID nSimID);

//gets the total no.of maximum data conn proifles settings filled currently

JC_INT32 cs_GetTotalDataConnProfiles();

//gets the list of the data connection's napd id .
 
JC_RETCODE cs_GetDataConnProfileList(ST_CS_DATACONN_PROFILE_NAME* pszDataConnList);

//gets the list of the proxy profile's  proxy id .
 
JC_RETCODE cs_GetProxyProfileList(ST_CS_PROXY_PROFILE_NAME* pszProxyProfileList, E_PROV_PROFILE_TYPE  profile_type, CFW_SIM_ID nSimID);


void prv_FormCSDataConnProfile(ST_PROV_INFO_LIST	*pstProfNode);

void prv_FormCSProxyProfile(ST_PROV_INFO_LIST	*pstProfNode);

void  prv_AddToCSProfiles(void * pProfile, E_CS_PROFILE_TYPE eCSProfileType);
void prv_DeleteAllCSProfiles();

void prv_FormCSProfDetailsBuff();
void prv_FormCSDataConnProfDeatilsBuff();
void prv_FormCSProxyProfDeatilsBuff();
//void prv_GetNextNewCSProfile();

void prv_ShowNextCSProfile();
void prv_FormCSProfiles();
JC_BOOLEAN prv_IsProvPushAvail();
JC_HANDLE  prv_getCommHandle ();
JC_BOOLEAN prv_CmpCSDataConnProfiles(ST_CS_DATACONN_PROFILE *pstProf1 , ST_CS_DATACONN_PROFILE *pstProf2);

void prv_SkipRedudantCSDataConnProfile();



#endif //PRV_PLATFORM_H
