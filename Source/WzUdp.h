// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef WZUDP_H
#define WZUDP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef void (*UdpProtocolCore)(BYTE,LPBYTE, int);



struct PER_IO_OPERATION_DATA
{
	WSAOVERLAPPED Overlapped;	// 0
	WSABUF DataBuf;	// 14
	char Buffer[2048];	// 1C
	DWORD Bytes;	// 81C
	int lOfs;	// 820
};

class WzUdp
{
public:

	WzUdp();
	virtual ~WzUdp();

	BOOL Init();
	BOOL CreateSocket();
	BOOL Run();
	BOOL SendData(LPBYTE SendData, DWORD nSendDataLen);
	BOOL MuProtocolParse(LPBYTE RecvData, int& nRecvDataLen);
	BOOL Close();
	BOOL SendSet(LPSTR ip, int port);
	BOOL RecvSet(int port);
	BOOL __declspec(noreturn) RecvThread();	// #error Why it return a value in PDB?
	BOOL SetProtocolCore(UdpProtocolCore pc);
	
	
private:

	int m_Port;	//4
	HANDLE m_ThreadHandle;	//8
	DWORD m_ThreadID;	//C
	SOCKET m_Socket;	// 10
	LPBYTE m_Recvbuf;	//14
	LPBYTE m_Sendbuf;	//18
	int m_dwLength; //1C
	int m_dwRecvOfs;	//20
	int m_dwSendLength;	//24
	PER_IO_OPERATION_DATA m_PerIoSendData;	// 28
	sockaddr_in	m_SockAddr;	// 84C
	UdpProtocolCore ProtocolCore;	// 85C

};



#endif