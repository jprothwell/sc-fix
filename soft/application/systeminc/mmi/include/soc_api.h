/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *	soc_api.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file contains function prototypes, constants and enum for Socket API.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#ifndef _SOC_API_H
#define _SOC_API_H

#if 0/* JIASHUO FOR WAP/MMS */
#if (!defined(__COOLSAND_TARGET__) || defined(WIN_SOC)) && !defined(GEN_FOR_PC)

#if !defined(_WINDOWS_) && defined(_PIXCOM_DATA_TYPES_H)
#error "Please include Windows.h before MMI_Data_Types.h in non __COOLSAND_TARGET__ compile option"
#endif

#ifndef _WINSOCKAPI_
#include <winsock2.h>
#endif//_WINSOCKAPI_
#endif
#endif

/*=============================
 * Constants
 *=============================*/
#define MAX_SOCKET_NUM	            (10) /* maximum sockets supported */
#define SOC_MAX_A_ENTRY              (5)

#define MAX_SOCK_ADDR_LEN           (16)

#define  IP_ADDR_LEN    (4)     /* length of an IP address */
#define  ETHER_ADDR_LEN     (6) /* length of an Ethernet address */

#ifdef MMI_ON_HARDWARE_P

/*
 * Protocol Familty
 */
#define PF_INET (0) /* currently only support Internet */

/* 
 * Protocols (RFC 1700)
 */

/* currently, only allow applicaion to set this type when RAW_SOCKET is used */
#define	IPPROTO_ICMP		(1)	   /* control message protocol */

/* for SOC/TCP/IP internal use, application shall NOT use */
#define	IPPROTO_IP		(0)             /* dummy for IP */
#define	IPPROTO_HOPOPTS	        (0)             /* IP6 hop-by-hop options */
#define	IPPROTO_IGMP		(2)	        /* group mgmt protocol */
#define	IPPROTO_IPV4		(4) 		/* IPv4 encapsulation */
#define	IPPROTO_IPIP		IPPROTO_IPV4	/* for compatibility */
#define	IPPROTO_TCP		(6)		/* tcp */
#define	IPPROTO_UDP		(17)		/* user datagram protocol */
#define	IPPROTO_RAW		(255)		/* raw IP packet */

/* 
 * Macros for SELECT 
 */
 #if 0
#define	FD_SET(n, p)	((p)->fds_bits[n] |= 0x01)
#define	FD_CLR(n, p)	((p)->fds_bits[n] &= ~(0x01))
#define	FD_ISSET(n, p)	((p)->fds_bits[n] & 0x02)
#define	FD_ZERO(p)	kal_mem_set(p, 0x00, sizeof(*(p)))

typedef struct
{
   kal_uint8	fds_bits[MAX_SOCKET_NUM];
} fd_set;
#endif

/*
 * Socket Type
 */
typedef enum
{
   SOCK_STREAM =0,	/* stream socket, TCP */
   SOCK_DGRAM,	        /* datagram socket, UDP */
   SOCK_SMS,         /* SMS bearer */
   SOCK_RAW          /* raw socket */
} socket_type_enum;

#else

typedef enum
{

   SOCK_SMS=100         /* SMS bearer */

} socket_type_enum;

#endif /* !WIN_SOC */

/* 
 * Any IP address
 */
//johnnie#define	INADDR_ANY		0x00000000

/*
 * Socket return codes, negative values stand for errors
 */




/*=============================
 * Enums
 *=============================*/


 
/*
 * howto arguments for shutdown(2), specified by Posix.1g.
 */
typedef enum
{
  SHUT_RD	=0,		/* shut down the reading side */
  SHUT_WR,			/* shut down the writing side */
  SHUT_RDWR			/* shut down both sides */
} soc_shutdown_enum;		

/*
 * Socket Options
 */
typedef enum
{
   SOC_OOBINLINE   = 0x01, /* not support yet */	
   SOC_LINGER	   = 0x02,  /* linger on close */
   SOC_NBIO	   = 0x04, /* Nonblocking */
   SOC_ASYNC	   = 0x08,  /* Asynchronous notification */   

   SOC_NODELAY     = 0x10,  /* disable Nagle algorithm or not */
   SOC_KEEPALIVE   = 0x20,  
   SOC_RCVBUF      = 0x40,  
   SOC_SENDBUF     = 0x80,

   SOC_NREAD       = 0x0100,  /* no. of bytes for read, only for soc_getsockopt */
   SOC_PKT_SIZE    = 0x0200,   
    SOC_SILENT_LISTEN = 0x0400, /* SOCK_SMS property */
    SOC_QOS = 0x0800,
    SOC_TCP_MAXSEG = 0x1000,
    SOC_IP_TTL = 0x2000,
    SOC_LISTEN_BEARER = 0x4000
} soc_option_enum;

/* 
 * event
 */
typedef enum
{
   SOC_READ    = 0x01,  /* Notify for read */
   SOC_WRITE   = 0x02,  /* Notify for write */
   SOC_ACCEPT  = 0x04,  /* Notify for accept */
   SOC_CONNECT = 0x08,  /* Notify for connect */
   SOC_CLOSE   = 0x10,   /* Notify for close */
   SOC_ERROR_IND   = 0x20   /* Notify for close */

} soc_event_enum;

typedef enum
{
    SOC_SUCCESS           = 0,
    SOC_ERROR             = -1,
    SOC_WOULDBLOCK        = -2,
    SOC_LIMIT_RESOURCE    = -3,    /* limited resource */
    SOC_INVALID_SOCKET    = -4,    /* invalid socket */
    SOC_INVALID_ACCOUNT   = -5,    /* invalid account id */
    SOC_NAMETOOLONG       = -6,    /* address too long */
    SOC_ALREADY           = -7,    /* operation already in progress */
    SOC_OPNOTSUPP         = -8,    /* operation not support */
    SOC_CONNABORTED       = -9,    /* Software caused connection abort */
    SOC_INVAL             = -10,   /* invalid argument */
    SOC_PIPE              = -11,   /* broken pipe */
    SOC_NOTCONN           = -12,   /* socket is not connected */
    SOC_MSGSIZE           = -13,   /* msg is too long */
    SOC_BEARER_FAIL       = -14,   /* bearer is broken */
    SOC_CONNRESET         = -15,   /* TCP half-write close, i.e., FINED */
    SOC_DHCP_ERROR        = -16,
    SOC_IP_CHANGED        = -17,
    SOC_ADDRINUSE         = -18,
    SOC_CANCEL_ACT_BEARER = -19    /* cancel the activation of bearer */
} soc_error_enum;

/*
 * bearer 
 */
typedef enum
{
    SOC_CSD  = 0x01,
    SOC_BEARER_CSD = SOC_CSD,
    SOC_GPRS = 0x02,
    SOC_BEARER_GPRS = SOC_GPRS,
    SOC_WIFI = 0x04,
    SOC_BEARER_WIFI = SOC_WIFI,
    SOC_BEARER_ANY = 0xff
} soc_bearer_enum;

typedef enum
{
   SOC_ACTIVATING,	
   SOC_ACTIVATED,
   SOC_DEACTIVATED,
   SOC_DEACTIVATING,
   SOC_AUTO_DISC_TIMEOUT
} soc_auto_disc_state_enum;

typedef enum
{
    SOC_QOS_BEST_EFFORT = 0x00,
    SOC_QOS_BACKGROUND = 0x10,
    SOC_QOS_VIDEO = 0x20,
    SOC_QOS_VOICE = 0x30
} soc_qos_enum;

/*=============================
 * structs 
 *=============================*/
typedef struct 
{
   kal_uint32 tv_sec;  /* no. of second */
   kal_uint32 tv_usec; /* no. of microsecond */
} soc_timeval_struct;



typedef struct 
{
   kal_int16	addr_len;
   kal_uint8	addr[MAX_SOCK_ADDR_LEN];
   kal_uint16	port;
 
} sockaddr_struct;

typedef struct
{
   kal_bool   onoff;
   kal_uint16 linger_time; /* unit: sec */
} soc_linger_struct;

typedef struct
{
   kal_uint8       ref_count;
   kal_uint16      msg_len;	
   kal_int8        socket_id;    /* socket ID */
   soc_event_enum  event_type;   /* soc_event_enum */
   kal_bool        result;
   soc_error_enum  error_cause;  /* used only when EVENT is close/connect */
   kal_int32       detail_cause; /* refer to ps_cause_enum if error_cause is
                                  * SOC_BEARER_FAIL */
} app_soc_notify_ind_struct;

/* The structure for DNS A RR entry */
typedef struct
{
    kal_uint8 address[4];   /* resolved IP address for queried domain name */
} soc_dns_a_struct;


/* Notification to application the result of DNS address query (soc_gethostbyname).
 * P.S. if application wants to accept this primitive, it should run in 
 * its own task instead of a task containing multiple applications, 
 * such as MMI. However, if application wants to run in the task of MMI 
 * and also wants to receive this primitive, it should use the 
 * "framework multiple protocol event handler" (since 08B). 
 * Otherwise, the primitive may be received by other 
 * applications running in the same task as you.
 */
 
typedef struct
{
    kal_uint8   ref_count;
    kal_uint16  msg_len;
    kal_bool    result;     /* the result of soc_gethostbyname */
    kal_int32	request_id; /* request id */
    kal_uint8   access_id;  /* access id */
    kal_uint32  account_id; /* network account id */
    kal_uint8	addr_len;   /* the first record in entry,  */
    kal_uint8	addr[16];   /* for backward compatibility */
    kal_uint8   num_entry;  /* number of entries. field of num_entry and entry are made 
                               to carry multiple answers for one domain name mapping to 
                               multiple IP addresses. */
    soc_dns_a_struct entry[SOC_MAX_A_ENTRY]; /* entries */

    kal_int8    error_cause;   /* bearer fail */
    kal_int32   detail_cause;  /* refer to ps_cause_enum if error_cause
                                * is SOC_BEARER_FAIL */
} app_soc_get_host_by_name_ind_struct;

#define MAX_DOMAIN_NAME_LENGTH 101//32 modified by wuys 2008-05-21
#define MAX_CONNECT_NUM    7*2+6

#define WEP_DNS_ID 1
#define JAVA_DNS_ID 2

typedef struct
{
    kal_int32	request_id; /* request id */
    kal_uint8    access_id;
    kal_int8      nsimID;
    kal_int8      ncid;
   kal_char      domain_name[MAX_DOMAIN_NAME_LENGTH];
   
}HostName_RequestID_struct;



/*=================================================== 
 *
 * Socket APIs prototypes
 *
 *===================================================*/
extern kal_int8 soc_create(kal_uint8  domain,
                           kal_uint8  type,
                           kal_uint8  protocol,
                           kal_uint16 mod_id,
                           kal_uint32 nwk_account_id);

extern kal_int8 soc_close(kal_int8 s);

extern kal_int8 soc_bind(kal_int8 s, sockaddr_struct *addr);

extern kal_int8 soc_listen(kal_int8 s, kal_uint8 backlog);

extern kal_int8 soc_accept(kal_int8 s, sockaddr_struct *addr);

extern kal_int8 soc_connect(kal_int8 s, sockaddr_struct *addr);

extern kal_int32 soc_sendto(kal_int8	     s,
                            kal_uint8       *buf,
                            kal_int32	     len,
                            kal_uint8 	     flags,
                            sockaddr_struct *addr);

extern kal_int32 soc_send(kal_int8	 s,
                          kal_uint8  *buf,
                          kal_int32  len,
                          kal_uint8	 flags);

extern kal_int32 soc_recvfrom(kal_int8	       s,
                              kal_uint8       *buf,
                              kal_int32       len,
                              kal_uint8	    flags,
                              sockaddr_struct *fromaddr);

extern kal_int32 soc_recv(kal_int8	     s,
                          kal_uint8      *buf,
                          kal_int32      len,
                          kal_uint8	     flags);         

#if 0/* JIASHUO FOR WAP/MMS */
#ifndef SOC_NOT_SUPPORT_SELECT
#ifndef GEN_FOR_PC
extern kal_int8 soc_select(kal_uint8	ndesc,
                           fd_set	*in,
                           fd_set	*out,
                           fd_set	*ex,
                           soc_timeval_struct	*tv);
#endif
#endif
#endif

extern kal_int8 soc_shutdown(kal_int8 s, kal_uint8 how);

extern kal_int8 soc_setsockopt(kal_int8   s,
                               kal_uint16 option,
                               kal_uint8  *val,
                               kal_uint8  val_size);

extern kal_int8 soc_getsockopt(kal_int8   s,
                               kal_uint16 option,
                               kal_uint8  *val,
                               kal_uint8  val_size);

/* currently, this function only support non-blocking mode */
extern kal_int8 soc_gethostbyname(kal_bool       is_blocking,
                           kal_uint16     mod_id,
                           kal_int32      request_id,
                           const kal_char *domain_name,
                           kal_uint8	   *addr,
                           kal_uint8	   *addr_len,
                           kal_uint8      access_id,
                           kal_uint32 nwk_account_id);	

/* currently, this function only support non-blocking mode */
extern
kal_int8 soc_gethostbyaddr(kal_bool         is_blocking,
                           kal_uint16       mod_id,
                           kal_int32        request_id,
                           kal_char         *domain_name,
                           const kal_uint8  *addr,
                           kal_uint8	     addr_len,
                           kal_uint8        access_id,
                           kal_uint32       nwk_account_id);

extern
kal_int8 soc_abort_dns_query(kal_bool   by_mod_id,
                             kal_uint16 mod_id,
                             kal_bool   by_request_id,
                             kal_uint32 request_id,
                             kal_bool   by_access_id,
                             kal_uint8  access_id,
                             kal_bool   by_nwk_account_id,
                             kal_uint32 nwk_account_id);
                             
extern void soc_close_nwk_account(kal_uint16 mod_id);
  
extern void soc_close_nwk_account_by_id(kal_uint16 mod_id, 
                                        kal_uint8 account_id);
  
extern kal_int8 soc_getlocalip(kal_uint8 *local_ip);

extern 
kal_int8 soc_get_account_localip(kal_int8 s, kal_uint8 *local_ip);

extern 
kal_int8 soc_getsockaddr(kal_int8 s, 
                         kal_bool is_local,
                         sockaddr_struct *addr);  

extern
kal_int8 soc_get_last_error(kal_int8 s,
                            kal_int8 *error, 
                            kal_int32 *detail_cause);

extern
kal_int8 soc_set_last_error(kal_int8 s,
                            kal_int8 error, 
                            kal_int32 detail_cause);

extern
kal_int8 soc_open_bearer(kal_int8 s);

extern
kal_int8 soc_close_bearer(kal_int8 s);

extern
kal_bool soc_ip_check(kal_char *asci_addr, 
                      kal_uint8 *ip_addr, 
                      kal_bool *ip_validity);
                      
#ifdef __COOLSAND_TARGET__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
extern kal_uint32 htonl(kal_uint32 a);
#define ntohl(a)    htonl(a)
extern kal_uint16 htons(kal_uint16 a);
#define ntohs(a)    htons(a)
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __COOLSAND_TARGET__ */
#define	NTOHL(x)	(x) = ntohl((kal_uint32)(x))
#define	NTOHS(x)	(x) = ntohs((kal_uint16)(x))
#define	HTONL(x)	(x) = htonl((kal_uint32)(x))
#define	HTONS(x)	(x) = htons((kal_uint16)(x))

#endif /* _SOC_API_H */

