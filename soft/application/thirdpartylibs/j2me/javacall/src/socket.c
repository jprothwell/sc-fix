/*
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */


#ifdef __cplusplus
extern "C" {
#endif
#include "kal_release.h"
#include "kal_non_specific_general_types.h"
#include "cfw.h"
#include "sockets.h"
#include "tcpip_tcp.h"
#include "cos.h"
#include "sxr_tim.h"
#include "javacall_socket.h" 
#include "mmi_trace.h"
#include "adp_events.h"
#include "timerevents.h"


#define SECOND        * HAL_TICK1S
#define MILLI_SECOND  SECOND / 1000
#define MILLI_SECONDS MILLI_SECOND
#define SECONDS       SECOND

int _active_socket_count = 0;
BOOL j2me_socket_io_activated = FALSE;

static BOOL javacall_socket_event_cb (COS_EVENT *pstCosEv)
{
    BOOL bIsProcessed = TRUE;
	mmi_trace(1," javacall_socket_event_cb :nEventId [%d] nParam1[%d] nParam2[%d] nParam3[%d]",pstCosEv->nEventId,
		pstCosEv->nParam1,pstCosEv->nParam2,pstCosEv->nParam3) ;

	
	switch (pstCosEv->nEventId)
	{
		case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
            _active_socket_count++;
            javanotify_socket_event(JAVACALL_EVENT_SOCKET_OPEN_COMPLETED, (javacall_handle)pstCosEv->nParam1, JAVACALL_OK);
			mmi_trace(1," EV_CFW_TCPIP_SOCKET_CONNECT_RSP Func: %s _active_socket_count=%d", __FUNCTION__,_active_socket_count);
			break ;
	
    	case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
			mmi_trace(1," EV_CFW_TCPIP_SOCKET_CLOSE_RSP Func: %s ", __FUNCTION__);
			break ;
			
    	case EV_CFW_TCPIP_SOCKET_SEND_RSP:
			mmi_trace(1," EV_CFW_TCPIP_SOCKET_SEND_RSP Func: %s ", __FUNCTION__);
			break ;
		case EV_CFW_TCPIP_REV_DATA_IND:
			mmi_trace(1," EV_CFW_TCPIP_REV_DATA_IND Func: %s ", __FUNCTION__);
            javanotify_socket_event(JAVACALL_EVENT_SOCKET_RECEIVE, (javacall_handle)pstCosEv->nParam1, JAVACALL_OK);
			break ;
		case EV_CFW_TCPIP_ACCEPT_IND:
			mmi_trace(1," EV_CFW_TCPIP_ACCEPT_IND Func: %s ", __FUNCTION__);
			break ;
		case EV_CFW_DNS_RESOLV_SUC_IND:
			mmi_trace(1," EV_CFW_DNS_RESOLV_SUC_IND Func: %s ", __FUNCTION__);
			break ;
		case EV_CFW_DNS_RESOLV_ERR_IND:				 
			mmi_trace(1,"SOC: EV_CFW_DNS_RESOLV_ERR_IND Func: %s ", __FUNCTION__);
			break ;

		case EV_CFW_TCPIP_ERR_IND:
			mmi_trace(1,"SOC: EV_CFW_TCPIP_ERR_IND Func: %s ,cause=%d,detail_cause=%d", __FUNCTION__,pstCosEv->nParam2, CFW_TcpipGetLastError());
            CFW_TcpipSocketClose(pstCosEv->nParam1);
            javanotify_socket_event(JAVACALL_EVENT_SOCKET_SEND, pstCosEv->nParam1, JAVACALL_FAIL);
            break ;

		case EV_CFW_TCPIP_CLOSE_IND:
			mmi_trace(1,"SOC: EV_CFW_TCPIP_CLOSE_IND Func: %s ", __FUNCTION__);
			break ;
			
		default:
			mmi_trace(1,"SOC: jdd_SocketEventcb : Unknown case") ;
			break ;
	}

	return bIsProcessed ;
		
}

static void javacall_socket_write_block_callback(void *handle)
{
    SOCKET sockfd = (SOCKET)handle;
    mmi_trace(1,"javacall_socket_write_block_callback %d",sockfd);
	javanotify_socket_event(JAVACALL_EVENT_SOCKET_SEND, (javacall_handle)sockfd, JAVACALL_OK);
}
/**
 * Initiates the connection of a client socket.
 *
 * @param ipBytes IP address of the local device in the form of byte array
 * @param port number of the port to open
 * @param pHandle address of variable to receive the handle; this is set
 *        only when this function returns JAVACALL_WOULD_BLOCK or JAVACALL_OK.
 * @param pContext address of a pointer variable to receive the context;
 *        this is set only when the function returns JAVACALL_WOULDBLOCK.
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an IO error   
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function to complete the operation
 * @retval JAVACALL_CONNECTION_NOT_FOUND when there was some other error (Connection not found exception case)
 */
javacall_result javacall_socket_open_start(unsigned char *ipBytes,int port,
                                           void **pHandle, void **pContext) {
	CFW_TCPIP_SOCKET_ADDR stSockConnect;
    SOCKET sockfd;
    
    j2me_socket_io_activated = TRUE;
    
    if(ipBytes[0] == 10 && ipBytes[1] == 0 && ipBytes[2] == 0 && ipBytes[3]== 172)
    {
        mmi_trace(1,"J2ME: socket %s,SOCKET_ERROR:JAVACALL_CONNECTION_NOT_FOUND",__FUNCTION__);
        return JAVACALL_CONNECTION_NOT_FOUND;
    }

	sockfd = CFW_TcpipSocket(CFW_TCPIP_AF_INET,CFW_TCPIP_SOCK_STREAM,CFW_TCPIP_IPPROTO_IP);
    mmi_trace(1,"J2ME: socket %s,sockfd=%d,ipBytes=%d %d %d %d,port=%d",__FUNCTION__,sockfd,ipBytes[0],ipBytes[1],ipBytes[2],ipBytes[3],port);
    
    CFW_SetTCPIPCallBackEx (javacall_socket_event_cb,sockfd);
    memset (&stSockConnect, 0, sizeof (CFW_TCPIP_SOCKET_ADDR)) ;
    stSockConnect.sin_len = 0 ;
    stSockConnect.sin_family = CFW_TCPIP_AF_INET ;
    stSockConnect.sin_port = htons(port) ;
    stSockConnect.sin_addr.s_addr = *((uint32 *)ipBytes) ;
    mmi_trace(1,"J2ME: socket %s,stSockConnect.sin_addr.s_addr=%u",__FUNCTION__,stSockConnect.sin_addr.s_addr);
    if (CFW_TcpipSocketConnect (sockfd, &stSockConnect, sizeof (CFW_TCPIP_SOCKET_ADDR))== SOCKET_ERROR)
    {
        mmi_trace(1,"J2ME: socket %s,SOCKET_ERROR",__FUNCTION__);
    }
    *pHandle = (void*)sockfd;
    *pContext = NULL;
    return JAVACALL_WOULD_BLOCK;
}
    
/**
 * Finishes a pending open operation.
 *
 * @param handle the handle returned by the open_start function
 * @param context the context returned by the open_start function
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if an error occurred  
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function again to complete the operation
 */
javacall_result javacall_socket_open_finish(void *handle, void *context) {
    mmi_trace(1,"J2ME: socket %s",__FUNCTION__);

    javacall_result res = JAVACALL_OK;
    return res;
}
    
/**
 * Initiates a read from a platform-specific TCP socket.
 *  
 * @param handle handle of an open connection
 * @param pData base of buffer to receive read data
 * @param len number of bytes to attempt to read
 * @param pBytesRead returns the number of bytes actually read; it is
 *        set only when this function returns JAVACALL_OK
 * @param pContext address of pointer variable to receive the context;
 *        it is set only when this function returns JAVACALL_WOULDBLOCK
 * 
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the operation would block
 * @retval JAVACALL_INTERRUPTED for an Interrupted IO Exception
 */
javacall_result javacall_socket_read_start(void *handle,unsigned char *pData,int len, 
                                           int *pBytesRead, void **pContext) {
    SOCKET sockfd = (SOCKET)handle;
    mmi_trace(1,"J2ME: socket %s,handle=%d,len=%d",__FUNCTION__,sockfd,len);
    int status = CFW_TcpipSocketRead(sockfd,pData,len);
    mmi_trace(1,"J2ME: socket %s,status=%d",__FUNCTION__,status);  
    *pContext = NULL;
    
    if (status == 0) 
    {
        *pBytesRead = status;
        return JAVACALL_WOULD_BLOCK;
    }
    if (status > 0) 
    {
        *pBytesRead = status;
        return JAVACALL_OK;
    } 
    else 
    {
        return JAVACALL_FAIL;
    }

}
    
/**
 * Finishes a pending read operation.
 *
 * @param handle handle of an open connection
 * @param pData base of buffer to receive read data
 * @param len number of bytes to attempt to read
 * @param pBytesRead returns the number of bytes actually read; it is
 *        set only when this function returns JAVACALL_OK
 * @param context the context returned by read_start
 * 
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function again to complete the operation
 * @retval JAVACALL_INTERRUPTED for an Interrupted IO Exception
 */
javacall_result javacall_socket_read_finish(void *handle,unsigned char *pData,int len,int *pBytesRead,void *context) {
    (void)context;
    SOCKET sockfd = (SOCKET)handle;
    mmi_trace(1,"J2ME: socket %s,handle=%d",__FUNCTION__,sockfd);
    int status = CFW_TcpipSocketRead(sockfd,pData,len);
    mmi_trace(1,"J2ME: socket %s,status=%d",__FUNCTION__,status);  
    
    if (status == 0) 
    {
        return JAVACALL_WOULD_BLOCK;
    }
    if (status > 0) 
    {
        *pBytesRead = status;
        return JAVACALL_OK;
    } 
    else 
    {
        return JAVACALL_FAIL;
    }


}
    
/**
 * Initiates a write to a platform-specific TCP socket.
 *
 * @param handle handle of an open connection
 * @param pData base of buffer containing data to be written
 * @param len number of bytes to attempt to write
 * @param pBytesWritten returns the number of bytes written after
 *        successful write operation; only set if this function returns
 *        JAVACALL_OK
 * @param pContext address of a pointer variable to receive the context;
 *	  it is set only when this function returns JAVACALL_WOULDBLOCK
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the operation would block
 * @retval JAVACALL_INTERRUPTED for an Interrupted IO Exception
 */
javacall_result javacall_socket_write_start(void *handle,char *pData,int len,int *pBytesWritten,void **pContext) {
    *pContext = NULL;
    SOCKET sockfd = (SOCKET)handle;
    uint32 AvailbleSize;
    AvailbleSize = CFW_TcpipAvailableBuffer(sockfd);
    mmi_trace(1,"J2ME: socket %s,handle=%d,AvailbleSize=%d,len=%d",__FUNCTION__,sockfd,AvailbleSize,len);
    if(AvailbleSize >= len)
    {
        int status = CFW_TcpipSocketWrite(sockfd,pData,len);
        mmi_trace(1,"J2ME: socket %s,status=%d",__FUNCTION__,status);
        if(SOCKET_ERROR != status)
        {
            *pBytesWritten = status;
            return JAVACALL_OK;
        }
        else
        {
            return JAVACALL_FAIL;
        }
    }
    else
    {
        mmi_trace(1,"J2ME: socket %s,JAVACALL_WOULD_BLOCK");
		sxr_StartFunctionTimer(300 MILLI_SECONDS, javacall_socket_write_block_callback, (void *)sockfd, 0);
        return JAVACALL_WOULD_BLOCK;
    }

}
    
/**
 * Finishes a pending write operation.
 *
 * @param handle handle of an open connection
 * @param pData base of buffer containing data to be written
 * @param len number of bytes to attempt to write
 * @param pBytesWritten returns the number of bytes written after
 *        successful write operation; only set if this function returns
 *        JAVACALL_OK
 * @param context the context returned by write_start
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function again to complete the operation
 * @retval JAVACALL_INTERRUPTED for an Interrupted IO Exception
 */
javacall_result javacall_socket_write_finish(void *handle,char *pData,int len,int *pBytesWritten,void *context) {
    mmi_trace(1,"J2ME: socket %s,handle=%d",__FUNCTION__,(SOCKET)handle);
    return javacall_socket_write_start(handle, pData, len, pBytesWritten, NULL); 
}
    
/**
 * Initiates the closing of a platform-specific TCP socket.
 *
 * @param handle handle of an open connection
 * @param pContext address of a pointer variable to receive the context;
 *	  it is set only when this function returns JAVACALL_WOULDBLOCK
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error
 * @retval JAVACALL_WOULD_BLOCK  if the operation would block 
 */
javacall_result javacall_socket_close_start(void *handle,void **pContext) {
    SOCKET sockfd = (SOCKET)handle;
	int ret;
    mmi_trace(1,"J2ME: socket %s,sockfd=%d",__FUNCTION__,sockfd);
	ret = CFW_TcpipSocketClose(sockfd);
    _active_socket_count--;
    j2me_socket_io_activated = FALSE;
    mmi_trace(1,"J2ME: socket %s,sockfd=%d ret=%d _active_socket_count=%d",__FUNCTION__,sockfd,ret,_active_socket_count);
    return JAVACALL_OK;
}
    
/**
 * Initiates the closing of a platform-specific TCP socket.
 *
 * @param handle handle of an open connection
 * @param context the context returned by close_start
 *
 * @retval JAVACALL_OK          success
 * @retval JAVACALL_FAIL        if there was an error   
 * @retval JAVACALL_WOULD_BLOCK  if the caller must call the finish function again to complete the operation
 */
javacall_result javacall_socket_close_finish(void *handle,void *context) {
    (void)handle;
    (void)context;
    return JAVACALL_OK;
}

/**
 * Gets the number of bytes available to be read from the platform-specific
 * socket without causing the system to block. If it is not possible to find
 * out the actual number of available bytes then the resulting number is 0.
 * <b>IMPORTANT</b>: Some features of the product (not convered by spec but still essential,
 * for example java debugging) might not work if this function does not return
 * actual value.
 *
 * @param handle handle of an open connection
 * @param pBytesAvailable returns the number of available bytes
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there was an error 
 */
javacall_result javacall_socket_available(javacall_handle handle,
                                          int *pBytesAvailable) {
    SOCKET sockfd = (SOCKET)handle;
    int ret;
	ret = CFW_TcpipGetRecvAvailable(sockfd);
    mmi_trace(1,"J2ME: socket %s,sockfd=%d ret=%d",__FUNCTION__,sockfd,ret);  
    if(ret >= 0)
    {
        *pBytesAvailable = ret;
        return JAVACALL_OK;
    }
    else
    {
        return JAVACALL_FAIL;
    }
}

/******************************************************************************
 ******************************************************************************
 ******************************************************************************
    OPTIONAL FUNCTIONS
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************/
    
/**
 * Shuts down the output side of a platform-specific TCP socket.
 * Further writes to this socket are disallowed.
 *
 * Note: a function to shut down the input side of a socket is
 * explicitly not provided.
 *
 * @param handle handle of an open connection
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    if there was an error
 */
javacall_result /*OPTIONAL*/ javacall_socket_shutdown_output(javacall_handle handle) {
    mmi_trace(1,"J2ME: socket %s,handle=%d",__FUNCTION__,handle);
	return JAVACALL_OK;
}

javacall_result /*OPTIONAL*/ javacall_server_socket_open_start(
	int port, 
	void **pHandle, 
	void **pContext) 
{
    mmi_trace(1,"J2ME: socket %s",__FUNCTION__);  
    return JAVACALL_FAIL;
}

/**
 * See pcsl_network.h for definition.
 */
javacall_result javacall_server_socket_open_finish(void *handle,void *context){
    mmi_trace(1,"J2ME: socket %s",__FUNCTION__);  

    	return JAVACALL_FAIL;
}


/**
 * See javacall_socket.h for definition.
 */
javacall_result /*OPTIONAL*/ javacall_server_socket_accept_start(
      javacall_handle handle, 
      javacall_handle *pNewhandle) {
   
   mmi_trace(1,"J2ME: socket %s",__FUNCTION__);  
     	return JAVACALL_FAIL;
}

/**
 * See javacall_socket.h for definition.
 */
javacall_result /*OPTIONAL*/ javacall_server_socket_accept_finish(
	javacall_handle handle, 
	javacall_handle *pNewhandle) {
    mmi_trace(1,"J2ME: socket %s",__FUNCTION__);  

    return JAVACALL_FAIL;
}

#ifdef __cplusplus
}
#endif


