// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// DirPath.cpp
// 4f5557 GS-N 0.99.60T Status : Completed
// GS-N	1.00.18	JPN	0x004A3650	-	Completed

#include "stdafx.h"
#include "dirpath.h"

CDirPath::CDirPath()
{

}

CDirPath::~CDirPath()
{

}

void CDirPath::SetFirstPath(char* dir_path)
{
	strcpy(this->m_path ,dir_path);
}

char* CDirPath::GetNewPath(char* npath)
{
	strcpy(this->m_newpath ,this->m_path);	// NULL
	strcat(this->m_newpath,npath);	// Derived Path

	return this->m_newpath;
}
//