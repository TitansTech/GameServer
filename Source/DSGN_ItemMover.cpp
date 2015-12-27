//DSGN Item Move Engine
#include "stdafx.h"
#include "GameServer.h"
#include "GameMain.h"
#include "LogProc.h"
#include "LogToFile.h"
#include "DSProtocol.h"
#include "..\include\Readscript.h"
#include "..\common\winutil.h"
#include "DSGN_ItemMover.h"

#if (DSGN_ITEM_DBMOVE == 1)
extern CLogToFile DSGN_ITEMMOVE_LOG;

DSGN_Item_Mover DS_Mover;

void DSGN_Item_Mover::Init()
{
	this->ItemObjectCount = 0;
	this->highestDBNumber = 3;

	for (int i=0; i < MAX_REC_ITEM_MACRO; i++)
	{
		this->recArray[i].loaded = 0;
		this->recArray[i].newDBNumber = 0;
		this->recArray[i].newIndex = 0;
		this->recArray[i].oldDBNumber = 0;
		this->recArray[i].oldIndex = 0;
	}
}



void DSGN_Item_Mover::LoadItem(char* script_file)
{
	SMDFile = fopen(script_file, "r");

	if ( SMDFile == NULL )
	{
		LogAdd(lMsg.Get(MSGGET(1, 197)), script_file);
		return ;
	}

	int Token;
	int n = 0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			int st = TokenNumber;	// script_type
			if ( st == 0 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					int TypeFrom = 0;
					int IndexFrom = 0;
					int TypeTo = 0;
					int IndexTo = 0;

					//Load Old Item Index + DbVersion
					TypeFrom = TokenNumber;

					Token = GetToken();
					IndexFrom = TokenNumber;

					this->recArray[n].oldIndex = ITEMGET(TypeFrom,IndexFrom);

					Token = GetToken();
					this->recArray[n].oldDBNumber = TokenNumber;

					//Load New Item Index + DbVersion
					Token = GetToken();
					TypeTo = TokenNumber;

					Token = GetToken();
					IndexTo = TokenNumber;

					this->recArray[n].newIndex = ITEMGET(TypeTo,IndexTo);

					Token = GetToken();
					this->recArray[n].newDBNumber = TokenNumber;

					//Get the New DbVersion
					if (this->highestDBNumber < this->recArray[n].newDBNumber)
					{
						this->highestDBNumber = this->recArray[n].newDBNumber;
					}

					//Mark Item as Loaded
					this->recArray[n].loaded = 1;
					this->ItemObjectCount++;

					DSGN_ITEMMOVE_LOG.Output("[ItemMove][Load] %d Item: oldId:%d, oldDb:%d, newId:%d, newDb:%d ",
						this->ItemObjectCount,
						this->recArray[n].oldIndex, this->recArray[n].oldDBNumber,
						this->recArray[n].newIndex, this->recArray[n].newDBNumber
					);

					n++;

					if ( this->ItemObjectCount > MAX_REC_ITEM_MACRO-1 )
					{
						break;
					}
				}
			}
		}
	}

	DSGN_ITEMMOVE_LOG.Output("[ItemMove] Successfuly Loaded %d Items [New DB: %d]!",
		this->ItemObjectCount,
		this->highestDBNumber
	);
}



int DSGN_Item_Mover::CheckTransformItem(int mNumber,int DbVersion)
{
	if (this->highestDBNumber == 0 || this->ItemObjectCount == 0)
	{
		return -1;
	}

rewindSearch:
	for(int i=0; i < MAX_REC_ITEM_MACRO; i++)
	{
		if (this->recArray[i].loaded == 1)
		{
			if (mNumber == this->recArray[i].oldIndex &&
				DbVersion == this->recArray[i].oldDBNumber)
			{
				if (this->recArray[i].newDBNumber == this->highestDBNumber)
				{
					return this->recArray[i].newIndex;
				} else {
					mNumber = this->recArray[i].newIndex;
					DbVersion = this->recArray[i].newDBNumber;
					goto rewindSearch;
				}
			}
		}
	}

	return mNumber;
}

#endif