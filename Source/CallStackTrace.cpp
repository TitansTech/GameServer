#include "stdafx.h"
#include "CallStackTrace.h"
#include "IMAGEHLP.H"

// GS-Normal 0x0048D6B1
//	GS-N	1.00.18	JPN	0x004A84F0

#pragma comment( lib , "Imagehlp.lib" )

CCallStackTrace::CCallStackTrace(char * const szLogID,char * const szLogFileName, char * const szLogDirectoryName)
{
	if ( !strcmp(szLogID, ""))
	{
		MessageBox(NULL, "INSERT LOG-ID !!!", "ERROR", MB_OK);
		return;
	}

	strcpy(this->m_szLogID, szLogID);

	////////////////////////////////
	if ( !strcmp(szLogFileName, ""))
		strcpy(this->m_szLogFileName, "LOGDATA");
	else
		strcpy(this->m_szLogFileName, szLogFileName);

	////////////////////////////////
	if ( !strcmp(szLogDirectoryName, ""))
		strcpy(this->m_szLogDirectoryName, "LOG");
	else
		strcpy(this->m_szLogDirectoryName, szLogDirectoryName);

	CreateDirectory(szLogDirectoryName, NULL);
	InitializeCriticalSection(&this->m_critLogToFile);
}



CCallStackTrace::~CCallStackTrace()
{
	DeleteCriticalSection(&this->m_critLogToFile);
}



void CCallStackTrace::Output()
{
	__try
	{
	if ( !strcmp(this->m_szLogID, ""))
	{
		MessageBox(NULL, "NO LOG-ID !!!", "ERROR", 0);
		return;
	}

	char szLogFileName[260];
	SYSTEMTIME strSystime;

	GetLocalTime(&strSystime);

	wsprintf(szLogFileName, "%s\\%s %04d-%02d-%02d.txt", this->m_szLogDirectoryName, this->m_szLogFileName,
		strSystime.wYear, strSystime.wMonth, strSystime.wDay);

	this->m_fLogFile = fopen(szLogFileName, "a+");

	if ( this->m_fLogFile == NULL )
		return;

	fprintf(this->m_fLogFile, "\nCallStack Trace Infomation (DATE:%04d-%02d-%02d %02d:%02d:%02d / ID:%s)\n",
		strSystime.wYear, strSystime.wMonth, strSystime.wDay, strSystime.wHour, strSystime.wMinute,
		strSystime.wSecond, this->m_szLogID);

	fprintf(this->m_fLogFile, "\n*-----> CallStack List <-----*\n", this->m_szLogID);	// #error, delete last argument

	for ( int iC=0;iC<this->m_strCallStackLog.size(); iC++)
		fputc(this->m_strCallStackLog[iC], this->m_fLogFile);

	fclose(this->m_fLogFile);
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
	}
}




DWORD CCallStackTrace::ConvertAddress(HANDLE hProcess, DWORD address, LPSTR output_buffer)
{
	__try
	{
	char* current_pointer = output_buffer;
	IMAGEHLP_MODULE imagehlp_module;
	memset(&imagehlp_module, 0, sizeof(imagehlp_module));
	imagehlp_module.SizeOfStruct = sizeof(imagehlp_module);

	if ( SymGetModuleInfo(hProcess, address, &imagehlp_module) != FALSE )
	{
		char * image_name = strrchr(imagehlp_module.ImageName, '\\');

		if ( image_name != NULL )
			image_name += 1;
		else
			image_name = imagehlp_module.ImageName;

		current_pointer += sprintf(current_pointer, "%s : ", image_name);
	}
	else
	{
		current_pointer += sprintf(current_pointer, "<unknown module> : ");
	}

#ifdef _WIN64
	DWORD64 displacement;
#else
	DWORD displacement;
#endif

	char temp[0x11c];
	IMAGEHLP_SYMBOL * imagehlp_symbol = (IMAGEHLP_SYMBOL *)temp;
	memset(imagehlp_symbol, 0, sizeof(temp));
	imagehlp_symbol->SizeOfStruct = 24;
	imagehlp_symbol->Address = address;
	imagehlp_symbol->MaxNameLength = 0x104;

	if ( SymGetSymFromAddr(hProcess, address, &displacement, imagehlp_symbol) != FALSE )
	{
		current_pointer += sprintf(current_pointer, "%s() ", imagehlp_symbol->Name);

		IMAGEHLP_LINE imagehlp_line;
		memset(&imagehlp_line, 0, sizeof(imagehlp_line));
		imagehlp_line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

#ifdef _WIN64
		DWORD displacement2 = (DWORD) displacement;
		if ( SymGetLineFromAddr(hProcess, address, &displacement2, &imagehlp_line) != FALSE )
#else
		if ( SymGetLineFromAddr(hProcess, address, &displacement, &imagehlp_line) != FALSE )
#endif
		{
			current_pointer += sprintf(current_pointer, "// %s(%i)", imagehlp_line.FileName, imagehlp_line.LineNumber);
		}
#ifdef _WIN64
		displacement =  displacement2;
#endif
	}
	else
	{
		current_pointer += sprintf(current_pointer, "<unknown symbol>");
	}
	
	current_pointer += sprintf(current_pointer, "\r\n");
	return current_pointer - output_buffer;
	}__except( EXCEPTION_ACCESS_VIOLATION == GetExceptionCode() )
	{
		return 0;
	}
}


int CCallStackTrace::SaveCallStack(BOOL bWriteInFile)
{
	return this->TraceStack(bWriteInFile);
}


// START REVIEW HERE


int CCallStackTrace::TraceStack(BOOL bWriteInFile)
{
	if ( strcmp(this->m_szLogID, "") == 0 )
	{
		MessageBox(NULL, "INSERT LOG-ID !!!", "ERROR", MB_OK);
		return -1;
	}

	this->m_strCallStackLog = "";

	EnterCriticalSection(&this->m_critLogToFile);

	__try
	{
		HANDLE hProcess = GetCurrentProcess();
		HANDLE hThread  = GetCurrentThread();
		DWORD option = SymGetOptions();
		option |= 0x10;
		option &= 0xFD;
		SymSetOptions(option|0x10);

		if (SymInitialize(hProcess, NULL, TRUE) == FALSE )
		{
			return -1;
		}

		DWORD addresses[0x2000]={0};
		UINT address_count = 0;
		CONTEXT context;
		memset(&context, 0, sizeof(context));
		context.ContextFlags = 0x10007;

		if ( GetThreadContext(GetCurrentThread(), &context) == FALSE )
		{
			return -1;
		}

		STACKFRAME stackframe;
		memset(&stackframe, 0, sizeof(stackframe));
		stackframe.Params[0] = 3;
		stackframe.Far = context.ContextFlags;
#ifdef _WIN64
		stackframe.Far = context.Rbp;
		stackframe.Far = context.Rip;
#else
		stackframe.Far = context.Ebp;
		stackframe.Far = context.Eip;
#endif
		stackframe.Far = 3;
		stackframe.Virtual = 3;
		
		for ( UINT i=0;i<512;i++ )
		{
			if ( StackWalk( 0x14C, hProcess, hThread, &stackframe, &context, NULL,
				SymFunctionTableAccess, (PGET_MODULE_BASE_ROUTINE)CCallStackTrace::GetModuleBase, NULL) == FALSE )
			{
				break;
			}

			if ( stackframe.Far > 1 )
			{
				 if ( stackframe.Far )
				 {
					 addresses[address_count] = stackframe.Far;
					 address_count++;
				 }
			}
		}

		for ( UINT addr_index =0 ;addr_index < stackframe.Far ; addr_index++ )
		{
			char symbol[0x504] ={0};
			DWORD symbol_size = this->ConvertAddress(hProcess, addresses[addr_index], symbol);
			this->m_strCallStackLog += "\t";
			this->m_strCallStackLog += symbol;
		}

		if ( bWriteInFile )
		{
			if ( this->m_strCallStackLog.empty() == false )
			{
				this->Output();
			}
		}
	}
	__finally
	{
		LeaveCriticalSection(&this->m_critLogToFile);
		return this->m_strCallStackLog.size();
	}
		/*
Data           :     ebp Relative, [0xffffffe0], Local, Type: void *, hProcess
Data           :     ebp Relative, [0xffffffdc], Local, Type: void *, hThread
Data           :     ebp Relative, [0xffffffd8], Local, Type: unsigned long, option
Data           :     ebp Relative, [0xffff7fd8], Local, Type: unsigned long[0x2000], addresses
Data           :     ebp Relative, [0xffff7fd4], Local, Type: unsigned int, address_count


Data           :     ebp Relative, [0xffff7d08], Local, Type: struct _CONTEXT, context
Data           :     ebp Relative, [0xffff7c64], Local, Type: struct _tagSTACKFRAME, stackframe


Data           :     ebp Relative, [0xffff7c60], Local, Type: unsigned int, i
Data           :     ebp Relative, [0xffff7c5c], Local, Type: unsigned int, addr_index


Data           :       ebp Relative, [0xffff7758], Local, Type: char[0x504], symbol
Data           :       ebp Relative, [0xffff7754], Local, Type: unsigned long, symbol_size*/

}



DWORD __stdcall CCallStackTrace::GetModuleBase(HANDLE hProcess, DWORD address)
{
	IMAGEHLP_MODULE imagehlp_module;
	MEMORY_BASIC_INFORMATION memory_basic_information;
	char module_file_name[260];
	DWORD module_file_name_length;
	HANDLE file_handle;
	char * image_name;

	imagehlp_module.SizeOfStruct = sizeof(imagehlp_module);

	if ( SymGetModuleInfo(hProcess, address, &imagehlp_module) == TRUE )
	{
		return imagehlp_module.BaseOfImage;
	}

	if ( VirtualQueryEx(hProcess, (LPCVOID)address, &memory_basic_information, sizeof(memory_basic_information)) == FALSE )
	{
		return 0;
	}

	module_file_name_length = GetModuleFileName((HMODULE)memory_basic_information.AllocationBase, module_file_name, sizeof(module_file_name));

	if ( module_file_name_length )
	{
		file_handle = CreateFile(module_file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	{
		file_handle = NULL;
	}

	image_name = NULL;

	if ( module_file_name_length > 0 )
	{
		image_name = module_file_name;
	}
	else
	{
		image_name = NULL;
	}

	SymLoadModule(hProcess, file_handle, image_name, 0, (DWORD)memory_basic_information.AllocationBase, 0);

	return (DWORD)memory_basic_information.AllocationBase;
}

