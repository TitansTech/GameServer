#include "stdafx.h"
#include "TUnionInfo.h"
#include "LogProc.h"
// GS-N 0.99.60T 0x004B0B80
//	GS-N	1.00.18	JPN	0x004CED60	-	Completed


TUnionInfo::TUnionInfo()
{
	this->m_iUnionTimeStamp = 0;
	this->m_iRivalTimeStamp = 0;
	this->m_iMasterGuild = 0;
	memset(this->m_szMasterGuild, 0, sizeof(this->m_szMasterGuild));

}

TUnionInfo::~TUnionInfo()
{
	this->Clear(0);
}


int TUnionInfo::GetUnionMemberCount()
{
	//DebugLog("%s START",__FUNCTION__);
	int iCount = 0;

	this->m_Sync.Lock();
	iCount = this->m_vtUnionMember.size();
	this->m_Sync.Unlock();

	//DebugLog("%s END",__FUNCTION__);
	return iCount;
}


int TUnionInfo::GetRivalMemberCount()
{
	//DebugLog("%s START",__FUNCTION__);
	int iCount = 0;

	this->m_Sync.Lock();
	iCount = this->m_vtRivalMember.size();
	this->m_Sync.Unlock();

	//DebugLog("%s END",__FUNCTION__);
	return iCount;
}


BOOL TUnionInfo::IsUnionMember(int iGuildNumber)
{
	//DebugLog("%s START",__FUNCTION__);
	this->m_Sync.Lock();
	std::vector<int>::iterator _Itor = std::find(this->m_vtUnionMember.begin(), this->m_vtUnionMember.end(), iGuildNumber );
	
	if ( _Itor != this->m_vtUnionMember.end() )
	{
		this->m_Sync.Unlock();
		//DebugLog("%s END",__FUNCTION__);
		return TRUE;
	}

	this->m_Sync.Unlock();
	
	//DebugLog("%s END",__FUNCTION__);
	return FALSE;
}



BOOL TUnionInfo::IsRivalMember(int iGuildNumber)
{
	//DebugLog("%s START",__FUNCTION__);
	this->m_Sync.Lock();
	std::vector<int>::iterator _Itor = std::find(this->m_vtRivalMember.begin(), this->m_vtRivalMember.end(), iGuildNumber );
	
	if ( _Itor != this->m_vtRivalMember.end() )
	{
		this->m_Sync.Unlock();
		//DebugLog("%s END",__FUNCTION__);
		return TRUE;
	}

	this->m_Sync.Unlock();
	//DebugLog("%s END",__FUNCTION__);
	return FALSE;
}



BOOL TUnionInfo::SetUnionMaster(int iGuildNumber, char * szGuildName)
{
	//DebugLog("%s START",__FUNCTION__);
	this->m_Sync.Lock();
	this->m_iMasterGuild = iGuildNumber;
	memcpy(this->m_szMasterGuild, szGuildName, sizeof(this->m_szMasterGuild)-1);
	this->m_Sync.Unlock();

	//DebugLog("%s END",__FUNCTION__);
	return TRUE;
}


void TUnionInfo::Clear(int iRelationShipType)
{
	//DebugLog("%s START",__FUNCTION__);
	this->m_Sync.Lock();

	if ( iRelationShipType == 0 )
	{
		this->m_vtRivalMember.clear();
		this->m_vtUnionMember.clear();
	}
	else if ( iRelationShipType == 1 ) // Union
	{
		this->m_vtUnionMember.clear();
	}
	else if ( iRelationShipType == 2 ) // Rival
	{
		this->m_vtRivalMember.clear();
	}

	this->m_Sync.Unlock();
	//DebugLog("%s END",__FUNCTION__);
}
