/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
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
/**************************************************************

	FILENAME	: NWSocket.cpp

  	PURPOSE		: Sockets file

	REMARKS		: nil

	AUTHOR		:  Kuldeep

	DATE		: Oct 10,03

**************************************************************/

#include "stdafx.h"
#include "NWSocket.h"
#include <winsock.h>
#include <ATLCONV.H>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define SERV_HOST_ADDR_DEFAULT	"127.0.0.1"

CNWSocket::CNWSocket()
{
	m_hSocket = INVALID_SOCKET;
}

CNWSocket::~CNWSocket()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	Close();
}

BOOL CNWSocket::Create(UINT nSocketPort, int nSocketType, LPCTSTR lpszSocketAddress)
{
	if (m_hSocket != INVALID_SOCKET)
		return FALSE;

	m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_hSocket == INVALID_SOCKET)
		return FALSE;

	return TRUE;
}

BOOL CNWSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	if (m_hSocket == INVALID_SOCKET)
		return FALSE;

	//// CAsyncSocket code
	/*SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	LPSTR lpszAscii = T2A((LPTSTR)lpszHostAddress);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszAscii);

	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAscii);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);*/
	SOCKADDR_IN sockAddr;
	int addrLen;
	char tempBuf[10];
	char hostAddress[30];
	
	HOSTENT *myHost;
	memset(hostAddress, 0, 30);
	memset(tempBuf, 0, 10);
	addrLen = (int)sizeof(struct sockaddr_in);

	myHost = gethostbyaddr(hostAddress,addrLen,AF_INET);

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port	= htons(nHostPort);
	if(!hostAddress)
		sockAddr.sin_addr.s_addr = inet_addr(hostAddress);
	else
		sockAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR_DEFAULT);

	int nError = connect(m_hSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
	
	//// End CAsyncSocket code

	if (nError == SOCKET_ERROR)
		return FALSE;

	return TRUE;
}

void CNWSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		int nErr = closesocket(m_hSocket);

		if (nErr == SOCKET_ERROR)
		{
			nErr = GetLastError();

			AfxMessageBox(_T("Error Closing Socket!"));
		}
		else
			m_hSocket = INVALID_SOCKET;
	}

	return;
}

int CNWSocket::Send( const void* lpBuf, int nBufLen, int nFlags)
{
	return send(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
}

int CNWSocket::Receive( void* lpBuf, int nBufLen, int nFlags)
{
	return recv(m_hSocket, (LPSTR)lpBuf, nBufLen, nFlags);
}

BOOL CNWSocket::GetPeerName(CString& rPeerAddress, UINT& rPeerPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = GetPeerName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rPeerPort = ntohs(sockAddr.sin_port);
		rPeerAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

BOOL CNWSocket::GetSockName(CString& rSocketAddress, UINT& rSocketPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = GetSockName((SOCKADDR*)&sockAddr, &nSockAddrLen);
	if (bResult)
	{
		rSocketPort = ntohs(sockAddr.sin_port);
		rSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}
