// Giocp.cpp
//------------------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
//------------------------------------------

// GS-N 0.99.60T 0x00473020 - Status : Completed :)
//	GS-N	1.00.18	JPN	0x00489FD0	-	Completed
#include "stdafx.h"
#include <winsock2.h>
#include "spe.h"
#include "giocp.h"
#include "logproc.h"
#include "GameMain.h"
#include "GameServer.h"
#include "user.h"
#include "AntiFloodSystem.h"
#include "CrystalWall.h"
#if(PROXY_SHOW == 1)
#include "ChildWindows.h"
#endif

#if(ISEX700==1)
#include "ComplexModulus.h"
#endif








HANDLE g_CompletionPort;
DWORD g_dwThreadCount;
enum SOCKET_FLAG;
CRITICAL_SECTION criti;
HANDLE g_ThreadHandles[MAX_IO_THREAD_HANDLES];
int g_ServerPort;
HANDLE g_IocpThreadHandle;
unsigned char* ExSendBuf;


SOCKET g_Listen = INVALID_SOCKET;	// THIS IS NOT THE PLACE OF TTHIS VARIABLE








void GiocpInit()
{
	ExSendBuf=new unsigned char[MAX_EXSENDBUF_SIZE];
}

void GiocpDelete()
{
	delete[] ExSendBuf;
}


BOOL CreateGIocp(int server_port)
{

#if (WL_PROTECT==1)
	VM_START_WITHLEVEL(15)
	int MyCheckVar;  
	CHECK_PROTECTION(MyCheckVar, 0x58117398)  	 
	if (MyCheckVar != 0x58117398)
	{
		WLRegDisableCurrentKey(wlMarkStolenKey);
		WLRegRemoveCurrentKey();
	}
	VM_END
#endif

	unsigned long ThreadID;
	
	g_ServerPort=server_port;

	g_IocpThreadHandle=CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)IocpServerWorker, NULL, 0, &ThreadID);

	if ( g_IocpThreadHandle == 0 )
	{
		LogAdd("CreateThread() failed with error %d", GetLastError());
		return 0;
	}
	else
	{
		return 1;
	}

	
	
}

void DestroyGIocp()
{
	return;
/*#ifndef HIDE_ERRORL10
	LogAddTD("Error-L10");
#endif
	return;
	closesocket(g_Listen);

	for (DWORD dwCPU=0; dwCPU < g_dwThreadCount;dwCPU++ )
	{
		TerminateThread( g_ThreadHandles[dwCPU] , 0);
	}

	TerminateThread(g_IocpThreadHandle, 0);

	if ( g_CompletionPort != NULL )
	{
		CloseHandle(g_CompletionPort);
		g_CompletionPort=NULL;
	} */
}


int CreateListenSocket()
{
	sockaddr_in InternetAddr;
	int nRet;

	g_Listen=WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if ( g_Listen == -1 )
	{
		LogAdd("WSASocket() failed with error %d", WSAGetLastError() );
		return 0;
	}
	else
	{
		InternetAddr.sin_family=AF_INET;
		InternetAddr.sin_addr.S_un.S_addr=htonl(0);
		InternetAddr.sin_port=htons(g_ServerPort);
		nRet=bind(g_Listen, (sockaddr*)&InternetAddr, 16);
		
		if ( nRet == -1 )
		{
			MsgBox(lMsg.Get(MSGGET(2, 55)));
			SendMessage(ghWnd, WM_CLOSE, 0,0);	// Kill aplication
			return 0 ;
		}
		else
		{
			nRet=listen(g_Listen, 5);
			if (nRet == -1)
			{
				LogAdd("listen() failed with error %d", WSAGetLastError());
				return 0;
			}
			else
			{
				return 1;
			}
		}
	} 
}


unsigned long __stdcall IocpServerWorker(void * p)
{
	SYSTEM_INFO SystemInfo;
	DWORD ThreadID;
	SOCKET Accept;
	int nRet;
	int ClientIndex;
	sockaddr_in cAddr;
	in_addr cInAddr;
	int cAddrlen;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	int RecvBytes;
	unsigned long Flags;
	
	


	cAddrlen=16;
	lpPerSocketContext=0;
	Flags=0;

	InitializeCriticalSection(&criti);
	GetSystemInfo(&SystemInfo);

	g_dwThreadCount = SystemInfo.dwNumberOfProcessors * 2;
	__try
	{

		g_CompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

		if ( g_CompletionPort == NULL )
		{
			LogAdd("CreateIoCompletionPort failed with error: %d", GetLastError());
			__leave;
		}

		for ( DWORD n = 0; n<g_dwThreadCount; n++ )
		{
			

			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerWorkerThread, g_CompletionPort, 0, &ThreadID);

			if ( hThread == 0 )
			{
				LogAdd("CreateThread() failed with error %d", GetLastError() );
				__leave;
			}

			g_ThreadHandles[n] = hThread;

			CloseHandle(hThread);
		}

		if ( CreateListenSocket() == 0 )
		{

		}
		else
		{
			while ( true )
			{
				Accept = WSAAccept(g_Listen, (sockaddr*)&cAddr, &cAddrlen, NULL, 0 );

				if ( Accept == -1 )
				{
					EnterCriticalSection(&criti);
					LogAdd("WSAAccept() failed with error %d", WSAGetLastError() );
					LeaveCriticalSection(&criti);
					continue;
				}

				EnterCriticalSection(&criti);
				memcpy(&cInAddr, &cAddr.sin_addr  , sizeof(cInAddr) );

				ClientIndex = gObjAddSearch(Accept, inet_ntoa(cInAddr) );

				if(AntiFlood.Check(inet_ntoa(cInAddr)) == false)
				{
					closesocket(Accept);
					LeaveCriticalSection(&criti);
					continue;
				}

				if ( ClientIndex == -1 )
				{
					LogAddL("error-L2 : ClientIndex = -1");
					closesocket(Accept);
					LeaveCriticalSection(&criti);
					continue;
				}

				if (UpdateCompletionPort(Accept, ClientIndex, 1) == 0 )
				{
					LogAddL("error-L1 : %d %d CreateIoCompletionPort failed with error %d", Accept, ClientIndex, GetLastError() );
					closesocket(Accept);
					LeaveCriticalSection(&criti);
					continue;
				}

				if (gObjAdd(Accept, inet_ntoa(cInAddr), ClientIndex) == -1 )
				{
					LogAddL("error-L1 : %d %d gObjAdd() failed with error %d", Accept, ClientIndex, GetLastError() );
					LeaveCriticalSection(&criti);
					closesocket(Accept);
					continue;
				}
				
				memset(&gObj[ClientIndex].PerSocketContext->IOContext[0].Overlapped, 0, sizeof(WSAOVERLAPPED));
				memset(&gObj[ClientIndex].PerSocketContext->IOContext[1].Overlapped, 0, sizeof(WSAOVERLAPPED));

				gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf.buf = (char*)&gObj[ClientIndex].PerSocketContext->IOContext[0].Buffer;
				gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf.len = MAX_IO_BUFFER_SIZE;
				gObj[ClientIndex].PerSocketContext->IOContext[0].nTotalBytes = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[0].nSentBytes = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[0].nSecondOfs = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[0].IOOperation = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].wsabuf.buf = (char*)gObj[ClientIndex].PerSocketContext->IOContext[0].Buffer;
				gObj[ClientIndex].PerSocketContext->IOContext[1].wsabuf.len = MAX_IO_BUFFER_SIZE;
				gObj[ClientIndex].PerSocketContext->IOContext[1].nTotalBytes = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].nSentBytes = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].nWaitIO = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].nSecondOfs = 0;
				gObj[ClientIndex].PerSocketContext->IOContext[1].IOOperation = 1;
				gObj[ClientIndex].PerSocketContext->m_socket = Accept;
				gObj[ClientIndex].PerSocketContext->nIndex = ClientIndex;

				nRet = WSARecv(Accept, &gObj[ClientIndex].PerSocketContext->IOContext[0].wsabuf , 1, (unsigned long*)&RecvBytes, &Flags, 
						&gObj[ClientIndex].PerSocketContext->IOContext[0].Overlapped, NULL);

				if ( nRet == -1 )
				{
					if ( WSAGetLastError() != WSA_IO_PENDING )
					{
						LogAddL("error-L1 : WSARecv() failed with error %d", WSAGetLastError() );
						gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO = 4;
						CloseClient(gObj[ClientIndex].PerSocketContext, 0);
						LeaveCriticalSection(&criti);
						continue;
					}
				}

				gObj[ClientIndex].PerSocketContext->IOContext[0].nWaitIO  = 1;
				gObj[ClientIndex].PerSocketContext->dwIOCount++;

				LeaveCriticalSection(&criti);
				SCPJoinResultSend(ClientIndex, 1);
			}


		}
	}
	__finally
	{
		
		if ( g_CompletionPort != NULL )
		{
			for ( int i = 0 ; i < g_dwThreadCount ; i++ )
			{
				PostQueuedCompletionStatus( g_CompletionPort , 0, 0, 0);
			}
		}

		if ( g_CompletionPort != NULL )
		{
			CloseHandle(g_CompletionPort);
			g_CompletionPort = NULL;
		}
		if ( g_Listen != INVALID_SOCKET )
		{
			closesocket( g_Listen);
			g_Listen = INVALID_SOCKET;
		}
	}

	return 1;
		
}


unsigned long __stdcall ServerWorkerThread(HANDLE CompletionPortID)
{
	HANDLE CompletionPort;
	DWORD dwIoSize;
	unsigned long RecvBytes;
	unsigned long Flags;
	DWORD dwSendNumBytes;
	BOOL bSuccess;
	int nRet;
#ifdef _WIN64
	ULONG_PTR ClientIndex=0;
#else
	DWORD ClientIndex=0;
#endif
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	LPOVERLAPPED lpOverlapped;
	_PER_IO_CONTEXT * lpIOContext;
	
	

	CompletionPort=CompletionPortID;
	dwSendNumBytes=0;
	bSuccess=0;
	lpPerSocketContext=0;
	lpOverlapped=0;
	lpIOContext=0;
	
	while ( true )
	{
		bSuccess=GetQueuedCompletionStatus( CompletionPort, &dwIoSize, &ClientIndex, &lpOverlapped, -1); // WAIT_FOREVER

		if (bSuccess == 0)
		{
			if (lpOverlapped != 0)
			{
				int aError = GetLastError();
				//Win2k3 SP2 Fix
				//64	ERROR_NETNAME_DELETED
				//121	ERROR_SEM_TIMEOUT
				//1230	ERROR_CONNECTION_ACTIVE
				//1231	ERROR_NETWORK_UNREACHABLE
				//1232	ERROR_HOST_UNREACHABLE
				//1233	ERROR_PROTOCOL_UNREACHABLE
				//1234	ERROR_PORT_UNREACHABLE
				//1235	ERROR_REQUEST_ABORTED
				//1236	ERROR_CONNECTION_ABORTED
				//1237	ERROR_RETRY
				//1238	ERROR_CONNECTION_COUNT_LIMIT
				//1239	ERROR_LOGIN_TIME_RESTRICTION

				//The semaphore timeout period has expired.
				if ((aError != ERROR_SEM_TIMEOUT) && 
					(aError != ERROR_NETNAME_DELETED) && 
					(aError != ERROR_CONNECTION_ABORTED) && 
					(aError != ERROR_OPERATION_ABORTED) &&
					(aError != ERROR_HOST_UNREACHABLE)	//DaRKav Experimental
				)
				{
					EnterCriticalSection(&criti);
					LogAdd("Error Thread : GetQueueCompletionStatus( %d )", GetLastError());
					LeaveCriticalSection(&criti);
					return 0;
				}
				LogAdd("[TRACE] Error Thread : GetQueueCompletionStatus( %d )", GetLastError());
			}
		}

		EnterCriticalSection(&criti);

		lpPerSocketContext=gObj[ClientIndex].PerSocketContext;
		lpPerSocketContext->dwIOCount --;
				
		if ( dwIoSize == 0 )
		{
			LogAdd("Connection Closed, dwIoSize == 0 (Index:%d)", lpPerSocketContext->nIndex);
			CloseClient(lpPerSocketContext, 0);
			LeaveCriticalSection(&criti);
			continue;
		}

#if (W2K3SP2_FIX==0)
		lpIOContext = (_PER_IO_CONTEXT *)lpOverlapped;

		if ( lpIOContext == 0 )
		{
			LogAddTD("Error-L1000 : Dont Allow More Users");
			LeaveCriticalSection(&criti);
			continue;
		}
#else	
		if ( lpOverlapped == 0 )
		{
			//TEST
			LogAddTD("Error-L1000 : Dont Allow More Users");
			LeaveCriticalSection(&criti);
			//TEST

			continue;
		}

		lpIOContext = CONTAINING_RECORD( lpOverlapped, _PER_IO_CONTEXT, Overlapped );
#endif

		if ( lpIOContext->IOOperation == 1 )
		{
			lpIOContext->nSentBytes += dwIoSize;


			if ( lpIOContext->nSentBytes >= lpIOContext->nTotalBytes )
			{
				lpIOContext->nWaitIO = 0;
						
				if ( lpIOContext->nSecondOfs > 0)
				{
					IoSendSecond(lpPerSocketContext);
				}
			}
			else
			{
				IoMoreSend(lpPerSocketContext);
			}
			
		}
		else if ( lpIOContext->IOOperation == 0 )
		{
			RecvBytes = 0;
			lpIOContext->nSentBytes += dwIoSize;

			//struct sockaddr_in sin;
			//memset(&sin,0,sizeof(&sin));
			//int len = sizeof(sin);
			//char ip[20]={0};
			//in_addr cInAddr;
			//getpeername(lpPerSocketContext->m_socket,(struct sockaddr*)&sin, &len);
			//memcpy(&cInAddr, &sin.sin_addr  , sizeof(cInAddr) );
			//strcpy(ip,inet_ntoa(cInAddr));
			//if(strcmp(gObj[ClientIndex].Ip_addr,ip))
			//{
			//	LogAdd("Proxy detected");
			//}
			//if(gObj[ClientIndex].m_socket != lpPerSocketContext->m_socket)
			//{
			//	LogAdd("Proxy detected");
			//}

			if ( RecvDataParse(lpIOContext, lpPerSocketContext->nIndex ) == 0 )
			{
				LogAdd("error-L1 : Socket Header error %d, %d", WSAGetLastError(), lpPerSocketContext->nIndex);
				CloseClient(lpPerSocketContext, 0);
				LeaveCriticalSection(&criti);
				continue;
			}

			lpIOContext->nWaitIO = 0;
			Flags = 0;
			memset(&lpIOContext->Overlapped, 0, sizeof (WSAOVERLAPPED));
			lpIOContext->wsabuf.len = MAX_IO_BUFFER_SIZE - lpIOContext->nSentBytes;
			lpIOContext->wsabuf.buf = (char*)&lpIOContext->Buffer[lpIOContext->nSentBytes];
			lpIOContext->IOOperation = 0;

			nRet = WSARecv(lpPerSocketContext->m_socket, &lpIOContext->wsabuf, 1, &RecvBytes, &Flags,
						&lpIOContext->Overlapped, NULL);

			if ( nRet == -1 )
			{
				if ( WSAGetLastError() != WSA_IO_PENDING)
				{
					LogAdd("WSARecv() failed with error %d", WSAGetLastError() );
					CloseClient(lpPerSocketContext, 0);
					LeaveCriticalSection(&criti);
					continue;
				}
			}

			lpPerSocketContext->dwIOCount ++;
			lpIOContext->nWaitIO = 1;
		}
		LeaveCriticalSection(&criti);
		

	}


	return 1;
}



BOOL RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int uIndex)	
{
	unsigned char* recvbuf;
	int lOfs;
	int size;
	BYTE headcode;
	BYTE xcode;

	// Check If Recv Data has More thatn 3 BYTES
	if ( lpIOContext->nSentBytes < 3 )
	{
		return TRUE;
	}

	// Initialize Variables
	lOfs=0;
	size=0;
	xcode=0;
	recvbuf = lpIOContext->Buffer;

	unsigned char byDec[7024];
	
	if ( OBJMAX_RANGE(uIndex))
	{
		gObj[uIndex].iPingTime = GetTickCount();
		gObj[uIndex].bPacketsReceivedCount = 0;
	}

	// Start Loop
	while ( true )
	{
		
		
		// Select packets with
		// C1 or C2 as HEader
		if ( recvbuf[lOfs] == 0xC1 ||
			 recvbuf[lOfs] == 0xC3 
#if (CRYSTAL_EDITION == 1)
			 ||	 recvbuf[lOfs] == 0xC6 
			 ||	 recvbuf[lOfs] == 0xC8 
#endif
			 )
		{
			unsigned char * pBuf;

			// Set Local Var to save values from
			// Cryps
			// th C1 and C3
			//recvbuf[2] ^= 195;
			pBuf = &recvbuf[lOfs];
			size = pBuf[1];

#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)
			if(CrystalWall.EncryptKey != 0)
				if(pBuf[2] != 0x04 && pBuf[2] != 0x05)
					pBuf[2] ^= CrystalWall.EncryptKey;
#endif
#endif
			headcode = pBuf[2];
			xcode = recvbuf[lOfs];
		}
		else if ( recvbuf[lOfs] == 0xC2 ||
			      recvbuf[lOfs] == 0xC4 )
		{
			unsigned char * pBuf;

			// Set Local Var to save values from
			// Cryps
			// th C2 and C4
			//recvbuf[3] ^= 195;
			pBuf = &recvbuf[lOfs];
			size = pBuf[1] * 256;
			size |= pBuf[2];
#if (PACK_EDITION>=3)
#if (CRYSTAL_EDITION == 1)
			if(CrystalWall.EncryptKey != 0)
				if(pBuf[3] != 0x04 && pBuf[3] != 0x05)
					pBuf[3] ^= CrystalWall.EncryptKey;
#endif
#endif
			headcode = pBuf[3];
			xcode = recvbuf[lOfs];
		}
		// If HEader Differs
		else
		{
			LogAdd("error-L1 : Header error (%s %d)lOfs:%d, size:%d",
											__FILE__, __LINE__,
											lOfs,
											lpIOContext->nSentBytes);

			lpIOContext->nSentBytes = 0;
			return false;
		}

		// Check Size is leess thant 0
		if ( size <= 0 )
		{
			LogAdd("error-L1 : size %d",
				size);

			return false;
		}

		//DC HACK FIX DARKAV
		/*if ( OBJMAX_RANGE(uIndex))
		{
			if ( gObj[uIndex].iPingTime > GetTickCount() - 2000)
			{
				gObj[uIndex].bPacketsReceivedCount++;

				if (gObj[uIndex].bPacketsReceivedCount >= 30)
				{
					LogAddC(2, "error-L1 : Possible DC-HACK attempt : ip = %s account:%s name:%s PACKET:%x %x %x",
						gObj[uIndex].Ip_addr,
						gObj[uIndex].AccountID, gObj[uIndex].Name,
						xcode,size,headcode
					);
		
					return false;
				}
			}
		}*/

		// Check if Size is On Range
		if ( size <= lpIOContext->nSentBytes )
		{
			// Case C3 type
			if ( xcode == 0xC3 )
			{

				// Decrypt Packet
#if(ISEX700==1)
				int ret = g_ComplexModulusCS.Decrypt( &byDec[2],
													&recvbuf[lOfs+2],
													size-2);
#else
				int ret = g_SimpleModulusCS.Decrypt( &byDec[2],
													&recvbuf[lOfs+2],
													size-2);
#endif

				// Case if ret BYTE is Less thant ZERO
				if ( ret < 0 )
				{

				}
				else
				{
					unsigned char* pDecBuf = &byDec[1];
					
					// Set Local Var 
					// Accoirding Recv Buffer
					headcode = pDecBuf[2];
					BYTE subhead = byDec[2];
					byDec[1] = 0xC1;
					byDec[2] = (ret&0xFF)+2;

					// Initialize PAcket Stream
					CStreamPacketEngine_Server PacketStream;

					// Clear Packet Stream
					PacketStream.Clear();

					// Add Data PAckte Stream
					if ( PacketStream.AddData(&byDec[1], ret +2) == 0 )
					{

						LogAddC(2, "error-L1 : CStreamPacketEngine Adding Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
							gObj[uIndex].Ip_addr,
							gObj[uIndex].AccountID, gObj[uIndex].Name,
							headcode,
							__FILE__, __LINE__,
							gObj[uIndex].Connected);
						//PacketStream.~CStreamPacketEngine_Server();
						return 0;
					}

					if ( PacketStream.ExtractPacket(byDec) != 0 )
					{
						LogAddC(2, "error-L1 : CStreamPacketEngine ExtractPacket Error (0): ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
							gObj[uIndex].Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);
						//PacketStream.~CStreamPacketEngine_Server();
						break;//IcaruS no Disconnect FIX
						//return 0;//OLD
					}

					if ( xcode == 0xC3 && headcode == 0xC5 )
					{
						LogAdd("암호패킷 : [0x%x]" , headcode); // Wrong Password
					}
	
					ProtocolCore(headcode, byDec, ret, uIndex, 1, subhead);
					//return;
				}
			}

			else if ( xcode == 0xC4 )
			{
#if(ISEX700==1)
				int ret = g_ComplexModulusCS.Decrypt(&byDec[3], &recvbuf[lOfs+3], size-3);
#else
				int ret = g_SimpleModulusCS.Decrypt(&byDec[3], &recvbuf[lOfs+3], size-3);
#endif
				if ( ret < 0 )
				{

				}
				else
				{
					unsigned char* pDecBuf = &byDec[1];
					
					
					BYTE hcode = byDec[3];
					byDec[1] = 0xC2;
					WORD size = (ret &0xFFFF)+3;
					byDec[2] = SET_NUMBERH(size);
					byDec[3] = SET_NUMBERL(size); 
					CStreamPacketEngine_Server PacketStream;

					PacketStream.Clear();
					if ( PacketStream.AddData(&byDec[1], ret+3) == 0 )
					{
						LogAddC(2, "error-L1 : CStreamPacketEngine Adding Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
							gObj[uIndex].Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);
						//PacketStream.~CStreamPacketEngine_Server();
						return 0;
					}

					if ( PacketStream.ExtractPacket(byDec) != 0 )
					{
						LogAddC(2, "error-L1 : CStreamPacketEngine ExtractPacket Error (1): ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
							gObj[uIndex].Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);
						//PacketStream.~CStreamPacketEngine_Server();
						break;//IcaruS no Disconnect FIX
						//return 0;//OLD
					}
					headcode = pDecBuf[2];
					if ( xcode == 0xC4 && headcode == 0xC5 )
					{
						LogAdd("암호패킷 : [0x%x]" , headcode); // Wrong Password
					}
	
					ProtocolCore(headcode, byDec, ret, uIndex, 1, hcode);
					//continue;
				}
			}
#if (CRYSTAL_EDITION == 1)
			else if(xcode == 0xC6 || xcode == 0xC8)
			{
				ProtocolCore(headcode, recvbuf, size, uIndex, 0, -1);
			}
#endif
			else
			{
#if(ISEX700==1)
				if(xcode == 0xC1)
				{
					int ret = g_ComplexModulusCS.Decrypt( &byDec[2],
													&recvbuf[lOfs+2],
													size-2);
				//	g_ComplexModulusCS.FixPacket_F3( &recvbuf[2],
				//									size-2);
				//memcpy(&byDec[0],&recvbuf[0],size);
				}
				else
					int ret = g_ComplexModulusCS.Decrypt(&byDec[3], &recvbuf[lOfs+3], size-3);
#endif

				CStreamPacketEngine_Server ps;
				ps.Clear();
				
				if ( ps.AddData(&recvbuf[lOfs], size) == 0 )
				{
					LogAddC(2, "error-L1 : CStreamPacketEngine Adding Error : ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
						gObj[uIndex].Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);
					return 0;
				}

				if ( ps.ExtractPacket(byDec) != 0 )
				{
					LogAddC(2, "error-L1 : CStreamPacketEngine ExtractPacket Error (2): ip = %s account:%s name:%s HEAD:%x (%s,%d) State:%d",
						gObj[uIndex].Ip_addr, gObj[uIndex].AccountID, gObj[uIndex].Name, headcode, __FILE__, __LINE__, gObj[uIndex].Connected);
					break;//IcaruS no Disconnect FIX
					//return 0;//OLD
				}
				ProtocolCore(headcode, byDec, size, uIndex, 0, -1);
			}

			lOfs += size;
			lpIOContext->nSentBytes  -= size;

			if ( lpIOContext->nSentBytes <= 0 )
			{
				break;
			}
		}
		else if ( lOfs > 0 )
		{
			if ( lpIOContext->nSentBytes < 1 )
			{
				LogAdd("error-L1 : recvbuflen 1 %s %d", __FILE__, __LINE__);
				break;
			}

			if ( lpIOContext->nSentBytes < MAX_IO_BUFFER_SIZE ) 
			{
				memcpy(recvbuf, &recvbuf[lOfs], lpIOContext->nSentBytes);
				LogAdd("Message copy %d", lpIOContext->nSentBytes);
				//break;
			}
			break;
		
		}
		else
		{
			break;
		}
		
	}

	return true;
}


BYTE SendProtocolENG(BYTE Type)
{
    switch(Type)
    {
        case 0xD3: return 0xD4;        //Walk Protocol 100%
        case 0xDF: return 0x15;        //Skills Use Fix 100%
        case 0xD7: return 0x11;        //Attack Protocol 100%
		case 0x10: return 0xDB;
    }
    return Type;
}

BOOL DataSend(int aIndex, unsigned char* lpMsg, DWORD dwSize)
{	
	if(ReadConfig.IsEngProtocol == 1)
	{
		if(lpMsg[0] == 0xC1 || lpMsg[0] == 0xC3)
			lpMsg[2] = SendProtocolENG(lpMsg[2]);
		else
			lpMsg[3] = SendProtocolENG(lpMsg[3]);
	}
	//int i = 0;
	//unsigned char NewProtocol[1024];
	//unsigned char changedProtocol = 0;
	//int aLen = sizeof(lpMsg);
	//if(lpMsg[2] == 0x94)
	//	return true;
	//if(lpMsg[2] == 0xF3)
	//	if(lpMsg[3] == 0x11)
	//		return true;
	//if(lpMsg[0] == 0xC2)
	//switch(lpMsg[2])
	//{
	//	case 0x12: //viewport protocol
	//		return true;
	//		memcpy(&NewProtocol[0],&lpMsg[0],aLen+1);
	//		NewProtocol[1] = HIBYTE(5+36*lpMsg[4]);
	//		NewProtocol[2] = LOBYTE(5+36*lpMsg[4]);
	//		aLen = 5+36*lpMsg[4];
	//		for( i=0; i<lpMsg[4]; i++)
	//		{
	//			memcpy(&NewProtocol[5+(i*36)],&lpMsg[5+(i*44)],22);
	//			memcpy(&NewProtocol[27+(i*36)],&lpMsg[33+(i*44)],14);
	//		}
	//		memcpy(lpMsg,NewProtocol,aLen);
	//		changedProtocol = 1;
	//		break;
	//	case 0x13: //view monster protocol
	//		return true;
	//		memcpy(&NewProtocol[0],&lpMsg[0],5);
	//		NewProtocol[1] = HIBYTE(lpMsg[4]*10+5);
	//		NewProtocol[2] = LOBYTE(lpMsg[4]*10+5);
	//		aLen = 5+10*lpMsg[4];
	//		for( i=0; i<lpMsg[4]; i++)
	//		{
	//			memcpy(&NewProtocol[5+10*i],&lpMsg[5+16*i],4);
	//			memcpy(&NewProtocol[9+10*i],&lpMsg[13+16*i],6);
	//		}
	//		memcpy(lpMsg,NewProtocol,aLen);
	//		changedProtocol = 1;
	//		break;
	//}
	// Check if we wanna log the packets
	if ( gStalkProtocol )
	{
		LogAddHeadHex("S",gObj[aIndex].AccountID, (char*)lpMsg, dwSize);
	}


	unsigned long SendBytes;
	_PER_SOCKET_CONTEXT * lpPerSocketContext;
	unsigned char * SendBuf;

	if ( aIndex < OBJ_STARTUSERINDEX )
	{
		return true;
	}

	EnterCriticalSection(&criti);

	if ( ((aIndex < 0)? FALSE : (aIndex > OBJMAX-1)? FALSE : TRUE )  == FALSE )
	{
		LogAdd("error-L2 : Index(%d) %x %x %x ", dwSize, lpMsg[0], lpMsg[1], lpMsg[2]);
		LeaveCriticalSection(&criti);
		return false;
	}

	if ( lpMsg[0] == 0xC3 || lpMsg[0] == 0xC4 )
	{
		int ret;
		BYTE btsize;

		if ( lpMsg[0] == 0xC3 )
		{
			btsize = lpMsg[1];
			lpMsg[1]=gNSerialCheck[aIndex].GetSendSerial();

//#if (ISEX700 == 1)
//			ret = g_ComplexModulusCS.Encrypt(&ExSendBuf[2], &lpMsg[1], dwSize-1);
//#else
//			ret = g_SimpleModulusSC.Encrypt(&ExSendBuf[2], &lpMsg[1], dwSize-1);
//#endif
			ret = g_SimpleModulusSC.Encrypt(&ExSendBuf[2], &lpMsg[1], dwSize-1);
			ExSendBuf[0] = 0xC3;
			ExSendBuf[1] = ret + 2;
			SendBuf = ExSendBuf;
			dwSize = ret + 2;
			lpMsg[1] = btsize;
		}
		else
		{
			btsize = lpMsg[2];
			lpMsg[2] = gNSerialCheck[aIndex].GetSendSerial();
//#if (ISEX700 == 1)
//			ret = g_ComplexModulusCS.Encrypt(&ExSendBuf[3], &lpMsg[2], dwSize-2);
//#else
//			ret = g_SimpleModulusSC.Encrypt(&ExSendBuf[3], &lpMsg[2], dwSize-2);
//#endif
			ret = g_SimpleModulusSC.Encrypt(&ExSendBuf[3], &lpMsg[2], dwSize-2);
			ExSendBuf[0] = 0xC4;
			ExSendBuf[1] = SET_NUMBERH(ret +3 );
			ExSendBuf[2] = SET_NUMBERL(ret+3);
			SendBuf = ExSendBuf;
			dwSize = ret + 3;
			lpMsg[2] = btsize;
		}
	}
	else
	{
		SendBuf = lpMsg;
	}


	if ( gObj[aIndex].Connected < PLAYER_CONNECTED )
	{
		LeaveCriticalSection(&criti);
		return FALSE;
	}

	lpPerSocketContext= gObj[aIndex].PerSocketContext;

	if ( dwSize > sizeof(lpPerSocketContext->IOContext[0].Buffer))
	{
		LogAdd("Error : Max msg(%d) %s %d", dwSize, __FILE__, __LINE__);
		CloseClient(aIndex);
		LeaveCriticalSection(&criti);
		return false;
	}

	_PER_IO_CONTEXT  * lpIoCtxt;

	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		if ( ( lpIoCtxt->nSecondOfs + dwSize ) > MAX_IO_BUFFER_SIZE-1 )
		{
			LogAdd("(%d)error-L2 [DS] MAX BUFFER OVER %d %d %d [%s][%s]", aIndex, lpIoCtxt->nTotalBytes, lpIoCtxt->nSecondOfs, dwSize, gObj[aIndex].AccountID, gObj[aIndex].Name);
			lpIoCtxt->nWaitIO = 0;
			ResponErrorCloseClient(aIndex);
			//CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return true;
		}

		memcpy( &lpIoCtxt->BufferSecond[lpIoCtxt->nSecondOfs], SendBuf, dwSize);
		lpIoCtxt->nSecondOfs += dwSize;
		LeaveCriticalSection(&criti);
		return true;
	}

	lpIoCtxt->nTotalBytes = 0;
	
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs = 0;
	}

	if ( (lpIoCtxt->nTotalBytes+dwSize) > MAX_IO_BUFFER_SIZE-1 )
	{
		LogAdd("(%d)error-L2 MAX BUFFER OVER %d %d [%s][%s]", aIndex, lpIoCtxt->nTotalBytes, dwSize, gObj[aIndex].AccountID, gObj[aIndex].Name);
		lpIoCtxt->nWaitIO = 0;
		ResponErrorCloseClient(aIndex);
		//CloseClient(aIndex);
		LeaveCriticalSection(&criti);
		return FALSE;
	}

	memcpy( &lpIoCtxt->Buffer[lpIoCtxt->nTotalBytes], SendBuf, dwSize);
	lpIoCtxt->nTotalBytes += dwSize;
	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = 1;
	

	if ( WSASend( gObj[aIndex].m_socket, &lpIoCtxt->wsabuf , 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{

		if ( WSAGetLastError() != WSA_IO_PENDING )	
		{
			lpIoCtxt->nWaitIO = 0;
			

			if ( lpIoCtxt->wsabuf.buf[0] == 0xC1 )
			{
				LogAdd("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[2], WSAGetLastError(), gObj[aIndex].Ip_addr);
			}
			else if ( lpIoCtxt->wsabuf.buf[0] == 0xC2 )
			{
				LogAdd("(%d)WSASend(%d) failed with error [%x][%x] %d %s ", __LINE__, aIndex, (BYTE)lpIoCtxt->wsabuf.buf[0],
					(BYTE)lpIoCtxt->wsabuf.buf[3], WSAGetLastError(), gObj[aIndex].Ip_addr);
			}
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	
#if(PROXY_SHOW == 1)
	WriteProxyPacket(aIndex,lpMsg,dwSize,1);
#endif
	LeaveCriticalSection(&criti);
	return true;
}





BOOL IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	unsigned long SendBytes;
	int aIndex;
	_PER_IO_CONTEXT * lpIoCtxt;

	EnterCriticalSection(&criti);
	aIndex = lpPerSocketContext->nIndex;
	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( lpIoCtxt->nWaitIO > 0 )
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpIoCtxt->nTotalBytes = 0;
	if ( lpIoCtxt->nSecondOfs > 0 )
	{
		memcpy(lpIoCtxt->Buffer, lpIoCtxt->BufferSecond, lpIoCtxt->nSecondOfs);
		lpIoCtxt->nTotalBytes = lpIoCtxt->nSecondOfs;
		lpIoCtxt->nSecondOfs = 0;
	}
	else
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer;
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes;
	lpIoCtxt->nSentBytes = 0;
	lpIoCtxt->IOOperation = 1;

	if ( WSASend(gObj[aIndex].m_socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			LogAdd("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), gObj[aIndex].Ip_addr);
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	
	return true;
}


BOOL IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext)
{
	unsigned long SendBytes;
	int aIndex;
	_PER_IO_CONTEXT * lpIoCtxt;

	EnterCriticalSection(&criti);
	aIndex = lpPerSocketContext->nIndex;
	lpIoCtxt = &lpPerSocketContext->IOContext[1];

	if ( (lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes) < 0 )
	{
		LeaveCriticalSection(&criti);
		return false;
	}

	lpIoCtxt->wsabuf.buf = (char*)&lpIoCtxt->Buffer[lpIoCtxt->nSentBytes];
	lpIoCtxt->wsabuf.len = lpIoCtxt->nTotalBytes - lpIoCtxt->nSentBytes;
	lpIoCtxt->IOOperation = 1;

	if ( WSASend(gObj[aIndex].m_socket, &lpIoCtxt->wsabuf, 1, &SendBytes, 0, &lpIoCtxt->Overlapped, NULL) == -1 )
	{
		if ( WSAGetLastError() != WSA_IO_PENDING )
		{
			LogAdd("WSASend(%d) failed with error %d %s ", __LINE__, WSAGetLastError(), gObj[aIndex].Ip_addr);
			CloseClient(aIndex);
			LeaveCriticalSection(&criti);
			return false;
		}
	}
	else
	{
		lpPerSocketContext->dwIOCount ++;
	}
	
	
	lpIoCtxt->nWaitIO = 1;
	LeaveCriticalSection(&criti);
	return true;
}


BOOL UpdateCompletionPort(SOCKET sd, int ClientIndex, BOOL bAddToList)
{
	_PER_SOCKET_CONTEXT * lpPerSocketContext = NULL;

	HANDLE cp = CreateIoCompletionPort((HANDLE) sd, g_CompletionPort, ClientIndex, 0);

	if ( cp == 0 )
	{
		LogAdd("CreateIoCompletionPort: %d", GetLastError() );
		return FALSE;
	}

	gObj[ClientIndex].PerSocketContext->dwIOCount = 0;
	return TRUE;
}


void CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result)
{
	int index = -1;
	index = lpPerSocketContext->nIndex ;

	if ( index >= OBJ_STARTUSERINDEX && index < OBJMAX )
	{
		if ( gObj[index].m_socket != INVALID_SOCKET )
		{
			if (closesocket(gObj[index].m_socket) == -1 )
			{
				if ( WSAGetLastError() != WSAENOTSOCK )
				{
					return;
				}
			}

			LogAdd("[Close Client][1] Call to Close Client of %s",gObj[index].Name);
			gObj[index].m_socket = INVALID_SOCKET;
		}

		gObjDel(index);
	}
}



void CloseClient(int index)
{
	if ( index < 0 || index > OBJMAX-1 )
	{
		LogAdd("error-L1 : CloseClient index error");
		return;
	}

	if ( gObj[index].Connected == PLAYER_EMPTY )
	{
		LogAdd("error-L1 : CloseClient connect error");
		return;
	}

	LogAdd("[Close Client][0] Call to Close Client of %s",gObj[index].Name);
	
	EnterCriticalSection(&criti);

	if ( gObj[index].m_socket != INVALID_SOCKET )
	{
		closesocket(gObj[index].m_socket );
		gObj[index].m_socket = INVALID_SOCKET;

		//gObjDel(index);	//DaRKav Test
	}
	else
	{
		LogAdd("error-L2 (%d): CloseClient INVALID_SOCKET", index);// (%d)(%s)",index,gObj[index].Ip_addr);
		//ResponErrorCloseClient(index);	//DaRKav Test
	}

	LeaveCriticalSection(&criti);
}

void ResponErrorCloseClient(int index)
{
	if ( index < 0 || index > OBJMAX-1 )
	{
		LogAdd("error-L1 : ResponErrorCloseClient index error");
		return;
	}

	if ( gObj[index].Connected == PLAYER_EMPTY )
	{
		LogAdd("error-L1 : ResponErrorCloseClient connect error");
		return;
	}

	LogAdd("[ResponClose Client][0] Call to Close Client of %s",gObj[index].Name);

	EnterCriticalSection(&criti);
	closesocket(gObj[index].m_socket);
	gObj[index].m_socket = INVALID_SOCKET;

	if ( gObj[index].m_socket == INVALID_SOCKET )
	{
		LogAdd("error-L1 (%d): ResponErrorCloseClient INVALID_SOCKET",index);
	}

	gObjDel(index);	//DaRKav Test
	LeaveCriticalSection(&criti);
}
