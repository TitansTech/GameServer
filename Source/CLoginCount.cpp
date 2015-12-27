// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------

// GS-N 0.99.60T Status : Completed
// GS-N 1.00.18	0x0049EC20	JPN - Completed

#include "stdafx.h"
#include "CLoginCount.h"



CLoginCount::CLoginCount()
{
	this->Init();
}

CLoginCount::~CLoginCount()
{
	return;
}

void CLoginCount::Init()
{
	this->m_count=0;
}

void CLoginCount::Add()
{
	this->m_count++;
}

void CLoginCount::Delete()
{
	this->m_count--;

	if ( this->m_count < 0 )
	{
		this->m_count = 0;
	}
}

int CLoginCount::Get()
{
	return this->m_count;
}

