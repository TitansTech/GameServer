// Giocp.h
//------------------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
//------------------------------------------

#ifndef GIOCP_H
#define GIOCP_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define HIDE_ERRORL10
#define MAX_EXSENDBUF_SIZE 7024
#define MAX_IO_BUFFER_SIZE		6000
#define MAX_IO_THREAD_HANDLES	16

struct _PER_IO_CONTEXT
{ 
	WSAOVERLAPPED Overlapped; // 0
	WSABUF wsabuf; 
	unsigned char Buffer[MAX_IO_BUFFER_SIZE]; // 1C
	unsigned char BufferSecond[MAX_IO_BUFFER_SIZE]; // 178C
	int nSecondOfs; // 2EFC
	int nTotalBytes;	// 2F00
	int nSentBytes; // 2F04
	int IOOperation; // 2F08
	int nWaitIO; // 2F0C

}; 



struct _PER_SOCKET_CONTEXT
{
	SOCKET m_socket;	// 0
	int nIndex;	// 4
	_PER_IO_CONTEXT IOContext[2];	// 8
	int dwIOCount;	// 5E28
}; 



extern unsigned char* ExSendBuf;
extern int g_ServerPort;
extern HANDLE g_IocpThreadHandle;
extern HANDLE g_ThreadHandles[MAX_IO_THREAD_HANDLES];
extern CRITICAL_SECTION criti;
extern enum SOCKET_FLAG;
extern DWORD g_dwThreadCount;
extern HANDLE g_CompletionPort;	
extern SOCKET g_Listen;



void GiocpInit();
void GiocpDelete();
int CreateGIocp(int server_port);
void DestroyGIocp();
int CreateListenSocket();
unsigned long __stdcall IocpServerWorker(void * p);
unsigned long __stdcall ServerWorkerThread(HANDLE CompletionPortID);
BOOL RecvDataParse(_PER_IO_CONTEXT * lpIOContext, int uIndex);
BOOL DataSend(int aIndex, unsigned char* lpMsg, DWORD dwSize);
BOOL IoSendSecond(_PER_SOCKET_CONTEXT * lpPerSocketContext);
BOOL IoMoreSend(_PER_SOCKET_CONTEXT * lpPerSocketContext);
BOOL UpdateCompletionPort(SOCKET sd, int ClientIndex, BOOL bAddToList);
void CloseClient(_PER_SOCKET_CONTEXT * lpPerSocketContext, int result);
void CloseClient(int index);
void ResponErrorCloseClient(int index);



#endif