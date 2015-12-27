// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// GS-N 0.99.60T 0x00477D6B Status : Completed
// GS-N	1.00.18	JPN	0x0048E8D0 - Completed - Error on WzUdpRecvThread 

#include "stdafx.h"
#include "WzUdp.h"



#define WZ_UPD_WINSOCK_VERSION 0x202


DWORD __stdcall WzUdpRecvThread(WzUdp* lpWzUdp);



WzUdp::WzUdp()
{
	this->Init();
}


WzUdp::~WzUdp()
{
	this->Close();
}

int WzUdp::Init()
{
	WSADATA wsd;

	if ( WSAStartup(WZ_UPD_WINSOCK_VERSION, &wsd) )
	{
		return 0;
	}
	this->m_Socket = INVALID_SOCKET;	// Set socket NULL
	this->m_dwLength = 4096;
	this->m_dwRecvOfs = 0;
	this->m_ThreadHandle = NULL;
	this->ProtocolCore=0;	// Protocol Core Null
	return 1;
}

int WzUdp::Close()
{
	::TerminateThread(this->m_ThreadHandle, 0);

	if (this->m_ThreadHandle != 0)
	{
		WaitForSingleObject(this->m_ThreadHandle , INFINITE);	// Interesting, this could block GS
		CloseHandle(this->m_ThreadHandle );
		this->m_ThreadHandle =0;	// NULL handle
	}

	HeapFree(GetProcessHeap(), 0, this->m_Recvbuf);
	return 1;
}


int WzUdp::CreateSocket()
{
	if ( this->m_Socket != INVALID_SOCKET )
	{
		closesocket(this->m_Socket );
		this->m_Socket=INVALID_SOCKET;
	}
	this->m_Socket=WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_IP, NULL, 0,0);

	if ( this->m_Socket== INVALID_SOCKET)
	{
		return 0;
	}
	return 1;
}

int WzUdp::SendSet(char* ip, int port)
{
	this->m_Port = port;
	this->m_SockAddr.sin_family=AF_INET;

	this->m_SockAddr.sin_port=htons(port);
	this->m_SockAddr.sin_addr.S_un.S_addr=inet_addr(ip);

	if ( this->m_SockAddr.sin_addr.S_un.S_addr == -1)
	{
		hostent* host=0;	// NULL pointer
		host=gethostbyname(ip);

		if (host != 0)
		{
			memcpy(&this->m_SockAddr.sin_addr.S_un.S_addr,*host->h_addr_list ,host->h_length);	// Add Type Structure {$D }
		}
		else
		{
			return 0;
		}
	}
	return 1;
}
extern void MsgBox(char *szlog, ...);
int WzUdp::SetProtocolCore(UdpProtocolCore pc)
{
	this->ProtocolCore=pc;
	return 1;
}

int WzUdp::RecvSet(int port)
{

	this->m_Port=port;

	this->m_SockAddr.sin_port=htons(port);
	this->m_SockAddr.sin_family=AF_INET;

	this->m_SockAddr.sin_addr.S_un.S_addr=htonl(0);
	
	if( bind(this->m_Socket, (sockaddr*)&this->m_SockAddr, 16) == -1 )
	{
		return 0;
	}
	this->m_Recvbuf=(unsigned char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, this->m_dwLength);

	if ( this->m_Recvbuf == 0)
	{
		return 0;
	}
	return 1;
}

int WzUdp::SendData(LPBYTE SendData, DWORD nSendDataLen)
{
	int Ret;
	memset(&this->m_PerIoSendData.Overlapped, 0, 20);	// Add Type Structure {$D}
	memcpy(this->m_PerIoSendData.Buffer , SendData, nSendDataLen);
	
	this->m_PerIoSendData.lOfs=nSendDataLen;
	this->m_PerIoSendData.DataBuf.buf =this->m_PerIoSendData.Buffer;	// 40 Pointer 44 ...
	this->m_PerIoSendData.DataBuf.len =this->m_PerIoSendData.lOfs;	// No complex

	Ret=WSASendTo(this->m_Socket, &this->m_PerIoSendData.DataBuf, 1, (unsigned long*)&nSendDataLen, 0, (sockaddr*)&this->m_SockAddr, 16, &this->m_PerIoSendData.Overlapped, NULL); 		// 28 Add Type Structure {$D}
	if (Ret == -1 )
	{
		if (WSAGetLastError() != WSA_IO_PENDING ) //WSA_IO_PENDING
		{
			return -1;
		}
	}
	return 1;
}

int WzUdp::MuProtocolParse(LPBYTE RecvData, int& nRecvDataLen)
{
	int lOfs=0;
	int size=0;
	BYTE headcode;

	if (this->ProtocolCore == 0)
	{
		return 0;
	}

	while ( true )	
	{
		if ( RecvData[lOfs]==0xC1 ) // Packet Type Manager
		{
			size=RecvData[lOfs+1];	// Set Size
			headcode=RecvData[lOfs+2];
		}
		else if ( RecvData[lOfs]== 0xC2 )
		{
			size=RecvData[lOfs+1];
			size =size<<8;
			size |= RecvData[lOfs+2];
			headcode = RecvData[lOfs+3];
		}
		else
		{
			this->m_dwRecvOfs = 0;
			return 0;
		}

		if ( size <= 0 )
		{
			return 0;
		}

		if ( size <= nRecvDataLen )
		{
			this->ProtocolCore(headcode, &RecvData[lOfs], size );
			lOfs += size;
			this->m_dwRecvOfs -= size;

			if (this->m_dwRecvOfs <= 0 )
			{
				break;
			}
		}
		else if ( lOfs > 0 )
		{
			if (this->m_dwRecvOfs < 1 )
			{
				return 0;
			}

			memcpy(RecvData, &RecvData[lOfs], this->m_dwRecvOfs);
			return 1;
		}
		else
		{
			break;
		}
	}
	return 1;
}



BOOL WzUdp::Run()
{
	this->m_ThreadHandle=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WzUdpRecvThread, this, 0, &this->m_ThreadID);

	if (this->m_ThreadHandle == 0 )
	{
		return 0;
	}
	 return 1;
}

BOOL WzUdp::RecvThread()
{
	int ret;
	DWORD dwSenderSize;
	sockaddr_in sender;
	dwSenderSize=16;
	while ( true )
	{
		ret=recvfrom(this->m_Socket , (char*)&this->m_Recvbuf[this->m_dwRecvOfs], ( 4096 - this->m_dwRecvOfs ), 0, (sockaddr*)&sender, (int*)&dwSenderSize);

		if ( ret == -1 )
		{
			//continue;
		}
		else if ( ret == 0 )
		{
			//continue;
		}
		else
		{
			this->m_dwLength=ret;
			this->m_dwRecvOfs += ret;
			this->MuProtocolParse(this->m_Recvbuf, this->m_dwLength);
		}
	}
}


DWORD __stdcall WzUdpRecvThread(WzUdp* lpWzUdp)
{
	lpWzUdp->RecvThread();
	//__asm MOV EAX, 1
		return 1;
}






