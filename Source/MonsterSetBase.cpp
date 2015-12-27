#include "stdafx.h"
#include "MonsterSetBase.h"
#include "GameMain.h"
#include "MapServerManager.h"
#include "LogProc.h"
#include "LargeRand.h"
#include "..\include\readscript.h"
#include "..\common\WzMemScript.h"
// GS-N 0.99.60T 0x0041AB30 - Completed
//	GS-N	1.00.18	JPN	0x00421BB0	-	Completed

CMonsterSetBase::CMonsterSetBase()
{
	return;
}

CMonsterSetBase::~CMonsterSetBase()
{
	return;
}

void gObjMonsterAddSet(int Number, int Map, int X, int Y, int Count)
{	
	for(int i=0;i<Count;i++)
	{
		int MobPos = gObjMonsterAdd(Number,Map,X,Y);
		if(MobPos >= 0)
		{
			int MobID = gObjAddMonster(Map);
			if(MobID >= 0)
			{
				gObjSetPosMonster(MobID, MobPos);
				gObjSetMonster(MobID, Number,"gObjMonsterAddSet");
			}
		}
	}
}

void ReadMonstersAndAdd(char * FilePath)
{
	char sLineTxt[255] = {0};
	int Number;
	int Map;
	int X;
	int Y;
	int Count;

	SMDFile = fopen(FilePath, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("Spawn data load error %s", FilePath);
		return;
	}

	while (!feof(SMDFile))
	{
		if(fgets(sLineTxt, 255, SMDFile) != NULL)
		{
			if(sLineTxt[0] == '/')continue;  
			if(sLineTxt[0] == ';')continue;  
			sscanf(sLineTxt, "%d %d %d %d %d", &Number,&Map,&X,&Y,&Count);

			gObjMonsterAddSet(Number,Map,X,Y,Count);
		}
	}
		
	fclose(SMDFile);
}

void CMonsterSetBase::SetBoxPosition(int TableNum, int mapnumber, int ax, int ay, int aw, int ah)
{
	this->m_Mp[TableNum].m_MapNumber = mapnumber;
	this->m_Mp[TableNum].m_W  = aw;
	this->m_Mp[TableNum].m_H  = ah;
	this->m_Mp[TableNum].m_X  = ax;
	this->m_Mp[TableNum].m_Y  = ay;
}

BOOL CMonsterSetBase::GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my)
{
	int count = 100 ; 
	int w=0;
	int h=0;
	int tx=0; 
	int ty=0;
	BYTE attr=0;

	while ( count-- != 0)
	{
		w = aw - ax;
		h = ah - ay;

		__try
		{
			tx = ax + (rand()%w);
			ty = ay + (rand()%h);
		}
		__except (w=1,h=1,1)
		{

		}

		attr = MapC[mapnumber].GetAttr(tx, ty);

		if ( ((attr&1) != 1) && ((attr &4) != 4) && ((attr&8) != 8) )
		{
			mx = tx;
			my = ty;
			return TRUE;
		}
	}

	return false;
}

BOOL CMonsterSetBase::GetPosition(int TableNum, short MapNumber, short & x, short & y)
{
	int count = 100;
	BYTE attr;
	int tx;
	int ty;
	int w;
	int h;

	int Max = OBJ_MAXMONSTER - 1;
	if ( TableNum < 0 || TableNum > Max-1 )
	{
		if (MapNumber != MAP_INDEX_CASTLESIEGE)
		{
			LogAdd("ERROR : %d %d / %s %d",MapNumber, TableNum, __FILE__, __LINE__ );
		}
		return false;
	}

	if ( this->m_Mp[TableNum].m_ArrangeType == 1 || this->m_Mp[TableNum].m_ArrangeType == 3 )
	{
		while ( count-- != 0 )
		{
			w = this->m_Mp[TableNum].m_W - this->m_Mp[TableNum].m_X;
			h = this->m_Mp[TableNum].m_H - this->m_Mp[TableNum].m_Y;

			if ( w < 1 )
			{
				w = 1;
			}

			if ( h < 1 )
			{
				h = 1;
			}

			int iRX = GetLargeRand()%w;
			int iRY = GetLargeRand()%h;

			tx = this->m_Mp[TableNum].m_X +  iRX;
			ty = this->m_Mp[TableNum].m_Y +  iRY;
			attr = MapC[MapNumber].GetAttr(tx, ty);

			if ( ((attr&1) != 1) && ((attr &4) != 4) && ((attr&8) != 8) )
			{
				x = tx;
				y = ty;
				return TRUE;
			}
		}
	}
	else if ( this->m_Mp[TableNum].m_ArrangeType == 0 )
	{
		x = this->m_Mp[TableNum].m_X;
		y = this->m_Mp[TableNum].m_Y;
		return true;
	}
	else if ( this->m_Mp[TableNum].m_ArrangeType == 2 )
	{
		while ( count-- != 0 )
		{
			int Orx = this->m_Mp[TableNum].m_X -3;
			int Ory = this->m_Mp[TableNum].m_Y -3;

			Orx += rand() % 7;
			Ory += rand() % 7;

			attr = MapC[MapNumber].GetAttr(Orx, Ory);

			if ( ((attr&1) != 1) && ((attr &4) != 4) && ((attr&8) != 8) )
			{
				x = Orx;
				y = Ory;
				return TRUE;
			}
		}
	}
	else if ( this->m_Mp[TableNum].m_ArrangeType == 3 )
	{
		return TRUE;
	}

	else if ( this->m_Mp[TableNum].m_ArrangeType == 4 )
	{
		x = this->m_Mp[TableNum].m_X;
		y = this->m_Mp[TableNum].m_Y;
		return TRUE;
	}

	return false;
}

void CMonsterSetBase::LoadSetBase(char * filename)
{
	int Token;
	int rcount;
	int copycount;
	BYTE Sdir;

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("file open error %s", filename);
		return;
	}

	this->m_Count = 0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
			break;

		if ( Token == 1 )
		{
			int Type = TokenNumber;

			while ( true )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
					break;
				
				this->m_Mp[this->m_Count].m_Type = TokenNumber;
				Token = GetToken();	this->m_Mp[this->m_Count].m_MapNumber = TokenNumber;
				Token = GetToken();	this->m_Mp[this->m_Count].m_Dis = TokenNumber;
				Token = GetToken();	this->m_Mp[this->m_Count].m_X = TokenNumber;
				Token = GetToken();	this->m_Mp[this->m_Count].m_Y = TokenNumber;
				this->m_Mp[this->m_Count].m_ArrangeType = Type;

				if ( Type == 1 || Type == 3 )
				{
					Token = GetToken();	this->m_Mp[this->m_Count].m_W = TokenNumber;
					Token = GetToken();	this->m_Mp[this->m_Count].m_H = TokenNumber;
				}
				else if ( Type == 2 )
				{
					this->m_Mp[this->m_Count].m_W = 0;
					this->m_Mp[this->m_Count].m_H = 0;

					auto int w = this->m_Mp[this->m_Count].m_X - 3;
					auto int h = this->m_Mp[this->m_Count].m_Y - 3;

					w += rand() % 7;
					h += rand() % 7;

					this->m_Mp[this->m_Count].m_X = w;
					this->m_Mp[this->m_Count].m_Y = h;
				}
				else if ( Type == 4 )
				{
					this->m_Mp[this->m_Count].m_W = 0;
					this->m_Mp[this->m_Count].m_H = 0;
				}
				else
				{
					this->m_Mp[this->m_Count].m_W = 0;
					this->m_Mp[this->m_Count].m_H = 0;
				}

				Token = GetToken();	this->m_Mp[this->m_Count].m_Dir = TokenNumber;
				Sdir = this->m_Mp[this->m_Count].m_Dir;

				if ( this->m_Mp[this->m_Count].m_Dir == (BYTE)-1 )
					this->m_Mp[this->m_Count].m_Dir = rand() % 8;

				if ( Type == 1 || Type == 3 )
				{
					BYTE w = this->m_Mp[this->m_Count].m_X;
					BYTE h = this->m_Mp[this->m_Count].m_Y;

					Token = GetToken();	rcount = TokenNumber;
					copycount = this->m_Count;

					if ( rcount > 1 )
					{
						for ( int k = 0; k<rcount-1 ; k++ )
						{
							if ( g_MapServerManager.CheckMapCanMove(this->m_Mp[copycount].m_MapNumber) == FALSE )
								break;
						
							this->m_Count++;

							if ( this->m_Count > OBJ_MAXMONSTER-1 )
							{
								MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
								fclose(SMDFile);
								return;
							}

							this->m_Mp[this->m_Count].m_Dis = this->m_Mp[copycount].m_Dis;
							this->m_Mp[this->m_Count].m_Type = this->m_Mp[copycount].m_Type;
							this->m_Mp[this->m_Count].m_MapNumber = this->m_Mp[copycount].m_MapNumber;
							this->m_Mp[this->m_Count].m_W = this->m_Mp[copycount].m_W;
							this->m_Mp[this->m_Count].m_H = this->m_Mp[copycount].m_H;
							this->m_Mp[this->m_Count].m_X = this->m_Mp[copycount].m_X;
							this->m_Mp[this->m_Count].m_Y = this->m_Mp[copycount].m_Y;
							this->m_Mp[this->m_Count].m_ArrangeType = this->m_Mp[copycount].m_ArrangeType;

							if ( Sdir == (BYTE)-1 )
								this->m_Mp[this->m_Count].m_Dir = rand() % 8;
							else
								this->m_Mp[this->m_Count].m_Dir = this->m_Mp[copycount].m_Dir;
						}
					}
				}

				if ( g_MapServerManager.CheckMapCanMove(this->m_Mp[this->m_Count].m_MapNumber) == FALSE )
					continue;

				this->m_Count++;

				if ( this->m_Count > OBJ_MAXMONSTER-1 )
				{
					MsgBox("Monster attribute max over %d (%s %d)", this->m_Count, __FILE__, __LINE__);
					fclose(SMDFile);
					return;
				}
			}
		}
	}

#if (WL_PROTECT==1)
	VM_START_WITHLEVEL(13)
	int MyCheckVar4;  
	CHECK_PROTECTION(MyCheckVar4, 0x15382414)  	 
	if (MyCheckVar4 != 0x15382414)
	{
		bCanTrade = 0;
		gAddExperience = 1.0f;	
		gServerMaxUser=30;
		gItemDropPer = 20;
	}
	VM_END
#endif
	fclose(SMDFile);
}


void CMonsterSetBase::LoadSetBase(char* Buffer, int iSize)
{
	CWzMemScript WzMemScript;
	int Token;
	int rcount;
	int copycount;
	BYTE Sdir;

	WzMemScript.SetBuffer(Buffer, iSize);

	this->m_Count = 0;

	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
			break;

		if ( Token == 1 )
		{
			int Type = WzMemScript.GetNumber();

			while ( true )
			{
				Token = WzMemScript.GetToken();

				if ( strcmp("end", WzMemScript.GetString()) == 0 )
					break;
				
				this->m_Mp[this->m_Count].m_Type = WzMemScript.GetNumber();

				Token = WzMemScript.GetToken();	this->m_Mp[this->m_Count].m_MapNumber = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken();	this->m_Mp[this->m_Count].m_Dis = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken();	this->m_Mp[this->m_Count].m_X = WzMemScript.GetNumber();
				Token = WzMemScript.GetToken();	this->m_Mp[this->m_Count].m_Y = WzMemScript.GetNumber();

				this->m_Mp[this->m_Count].m_ArrangeType = Type;


				if ( Type == 1 || Type == 3 )
				{
					Token = WzMemScript.GetToken();	this->m_Mp[this->m_Count].m_W = WzMemScript.GetNumber();
					Token = WzMemScript.GetToken();	this->m_Mp[this->m_Count].m_H = WzMemScript.GetNumber();
				}
				else if ( Type == 2 )
				{
					this->m_Mp[this->m_Count].m_W = 0;
					this->m_Mp[this->m_Count].m_H = 0;

					auto int w = this->m_Mp[this->m_Count].m_X - 3;
					auto int h = this->m_Mp[this->m_Count].m_Y - 3;

					w += rand() % 7;
					h += rand() % 7;

					this->m_Mp[this->m_Count].m_X = w;
					this->m_Mp[this->m_Count].m_Y = h;
				}
				else if ( Type == 4 )
				{
					this->m_Mp[this->m_Count].m_W = 0;
					this->m_Mp[this->m_Count].m_H = 0;
				}
				else
				{
					this->m_Mp[this->m_Count].m_W = 0;
					this->m_Mp[this->m_Count].m_H = 0;
				}

				Token = WzMemScript.GetToken();	this->m_Mp[this->m_Count].m_Dir = WzMemScript.GetNumber();

				Sdir = this->m_Mp[this->m_Count].m_Dir;

				if ( this->m_Mp[this->m_Count].m_Dir == (BYTE)-1 )
				{
					this->m_Mp[this->m_Count].m_Dir = rand() % 8;
				}

				if ( Type == 1 || Type == 3 )
				{
					BYTE w = this->m_Mp[this->m_Count].m_X;
					BYTE h = this->m_Mp[this->m_Count].m_Y;

					Token = WzMemScript.GetToken();
					rcount = WzMemScript.GetNumber();
					copycount = this->m_Count;

					if ( rcount > 1 )
					{
						for ( int k = 0; k<rcount-1 ; k++ )
						{
							if ( g_MapServerManager.CheckMapCanMove(this->m_Mp[copycount].m_MapNumber) == FALSE )
								break;
						
							this->m_Count++;

							if ( this->m_Count > OBJ_MAXMONSTER-1 )
							{
								MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
								return;
							}

							this->m_Mp[this->m_Count].m_Dis = this->m_Mp[copycount].m_Dis;
							this->m_Mp[this->m_Count].m_Type = this->m_Mp[copycount].m_Type;
							this->m_Mp[this->m_Count].m_MapNumber = this->m_Mp[copycount].m_MapNumber;
							this->m_Mp[this->m_Count].m_W = this->m_Mp[copycount].m_W;
							this->m_Mp[this->m_Count].m_H = this->m_Mp[copycount].m_H;
							this->m_Mp[this->m_Count].m_X = this->m_Mp[copycount].m_X;
							this->m_Mp[this->m_Count].m_Y = this->m_Mp[copycount].m_Y;
							this->m_Mp[this->m_Count].m_ArrangeType = this->m_Mp[copycount].m_ArrangeType;

							if ( Sdir == (BYTE)-1 )
								this->m_Mp[this->m_Count].m_Dir = rand() % 8;
							else
								this->m_Mp[this->m_Count].m_Dir = this->m_Mp[copycount].m_Dir;
						}
					}
				}

				if ( g_MapServerManager.CheckMapCanMove(this->m_Mp[this->m_Count].m_MapNumber) == FALSE )
					continue;

				this->m_Count++;

				if ( this->m_Count > OBJ_MAXMONSTER-1 )
				{
					MsgBox("Monster attribute max over %d (%s %d)", this->m_Count, __FILE__, __LINE__);
					//fclose(SMDFile);
					return;
				}
			}
		}
	}

	//fclose(SMDFile);
}
