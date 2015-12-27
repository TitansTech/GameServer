// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x004E3170
//	GS-N	1.00.18	JPN	0x0050BB90	-	Completed

#include "stdafx.h"
#include "SetItemOption.h"
#include "WzMemScript.h"
#include "..\source\logproc.h"
#include "..\include\readscript.h"


CSetItemOption gSetItemOption;



CSetItemOption::CSetItemOption()
{
	return;
}

CSetItemOption::~CSetItemOption()
{
	return;
}

BOOL CSetItemOption::LoadTypeInfo(char* filename)
{
	int i;
	int Token;

	SMDFile=fopen(filename,"r");
	if (SMDFile == NULL )
	{
		return 0;
	}
	memset(this->m_SetItemTypeInfo , -1, sizeof(this->m_SetItemTypeInfo) );
	
	for ( i=0; i<MAX_ITEMS; i++)
	{
		this->m_SetItemTypeInfo[i].IsLoad = FALSE;
	}
	
	while ( true )
	{
		int number;
		
		Token=GetToken();
		if (Token == END)
		{
			break;
		}

		if (Token == NUMBER)
		{
			number = TokenNumber;
			
			while ( true )
			{
				int number2;
				LPSETITEM_TYPEINFO pointer;

				Token = GetToken();
				number2=TokenNumber;
				if ( ( Token== 0) && (strcmp("end", &TokenString[0]) == 0))
				{
					break;
				}
				else
				{
					pointer = &this->m_SetItemTypeInfo[number*MAX_SUBTYPE_ITEMS+number2];

					Token=GetToken();
					pointer->OptionIndex[0]=TokenNumber;
					Token=GetToken();
					pointer->OptionIndex[1]=TokenNumber;
					Token=GetToken();
					pointer->ChaosMixLevel[0]=TokenNumber;
					Token=GetToken();
					pointer->ChaosMixLevel[1]=TokenNumber;

					pointer->IsLoad=true;
				}
			}
		}

	}

	return true;
}


BOOL CSetItemOption::LoadOptionInfo(LPSTR filename)
{
	int i;
	int Token;
	int loop_count=0;
	
	SMDFile=fopen(filename, "r");
	if (SMDFile == NULL )
	{
		return 0;
	}
	memset(&this->m_SetItemOptionInfo, 0, sizeof(this->m_SetItemOptionInfo));

	for ( i=0;i<MAX_SETITEM_OPTION;i++)
	{
		memset(&this->m_SetItemOptionInfo[i].SetOptionTable[0], -1, sizeof(this->m_SetItemOptionInfo[0].SetOptionTable) );
		memset(&this->m_SetItemOptionInfo[i].SetExPOptionTable, -1, sizeof(this->m_SetItemOptionInfo[0].SetExPOptionTable) );
		memset(&this->m_SetItemOptionInfo[i].SetFullOtionTable[0], -1, sizeof(this->m_SetItemOptionInfo[0].SetFullOtionTable) );
	}

	while ( true )
	{
		Token=GetToken();
		int number=TokenNumber;
		
		if (Token==0 && strcmp("end", &TokenString[0]) == 0)
		{
			break;
		}
		else
		{
			LPSETITEM_OPTIONINFO p;
			p=&this->m_SetItemOptionInfo[number];

			Token=GetToken();
			strcpy(&p->Name[0], &TokenString[0]);

			if(!strcmp(TokenString,""))
			{
				MsgBox("ERROR: FILE %s (INDEX:%d)", filename,loop_count);
				exit(1);
			}

			for (i=0;i<OPTION_TABLE_SIZE;i++)
			{
				Token=GetToken();
				p->SetOptionTable[i][0] = TokenNumber;
				Token=GetToken();
				p->SetOptionTableValue[i][0] =TokenNumber;

				Token=GetToken();
				p->SetOptionTable[i][1] = TokenNumber;
				Token=GetToken();
				p->SetOptionTableValue[i][1] =TokenNumber;

				if ( (p->SetOptionTable[i][0] != -1) || (p->SetOptionTable[i][1] != -1) )
				{
					p->SetOptionCount++;
				}
			}
			
			// Special Set Item Options
			Token=GetToken();
			p->SetExPOptionTable[0]  = TokenNumber;
			Token=GetToken();
			p->SetExPOptionTableValue[0] = TokenNumber;

			if ( p->SetExPOptionTable[0] != -1)
			{
				p->SetOptionCount ++;
			}

			Token=GetToken();
			p->SetExPOptionTable[1]  = TokenNumber;
			Token=GetToken();
			p->SetExPOptionTableValue[1] = TokenNumber;

			if ( p->SetExPOptionTable[1] != -1)
			{
				p->SetOptionCount ++;
			}
			
			// Full Type Set Item
			Token=GetToken();
			p->SetFullOtionTable[0]=TokenNumber;
			Token=GetToken();
			p->SetFullOtionTableValue[0]=TokenNumber;

			Token=GetToken();
			p->SetFullOtionTable[1]=TokenNumber;
			Token=GetToken();
			p->SetFullOtionTableValue[1]=TokenNumber;

			Token=GetToken();
			p->SetFullOtionTable[2]=TokenNumber;
			Token=GetToken();
			p->SetFullOtionTableValue[2]=TokenNumber;

			Token=GetToken();
			p->SetFullOtionTable[3]=TokenNumber;
			Token=GetToken();
			p->SetFullOtionTableValue[3]=TokenNumber;

			Token=GetToken();
			p->SetFullOtionTable[4]=TokenNumber;
			Token=GetToken();
			p->SetFullOtionTableValue[4]=TokenNumber;

			// Save Character Class Disponibility
			Token=GetToken();
			p->RequireClass[CLASS_WIZARD]=TokenNumber;

			Token=GetToken();
			p->RequireClass[CLASS_KNIGHT]=TokenNumber;

			Token=GetToken();
			p->RequireClass[CLASS_ELF]=TokenNumber;

			Token=GetToken();
			p->RequireClass[CLASS_MAGICGLADIATOR]=TokenNumber;

			Token=GetToken();
			p->RequireClass[CLASS_DARKLORD]=TokenNumber;

			Token=GetToken();
			p->RequireClass[CLASS_SUMMONER]=TokenNumber;
			
			if(ReadConfig.S6E1 == 1)
			{
				Token=GetToken();
				p->RequireClass[CLASS_RAGEFIGHTER]=TokenNumber;
			}
			
			// Active SetItem
			p->IsLoad=true;	
			loop_count++;
		}
	}

	return 1;
}


BOOL CSetItemOption::LoadTypeInfo(char* Buffer, int iSize)
{
	CWzMemScript WzMemScript;
	int i;
	int Token;

	WzMemScript.SetBuffer(Buffer, iSize);

	memset(this->m_SetItemTypeInfo , -1, sizeof(this->m_SetItemTypeInfo) );

	for ( i=0; i<MAX_ITEMS; i++)
	{
		this->m_SetItemTypeInfo[i].IsLoad =FALSE;
	}
	
	while ( true )
	{
		int number;
		
		Token=WzMemScript.GetToken();
		if (Token == 2)
		{
			break;
		}

		if (Token == 1)
		{
			number = WzMemScript.GetNumber();
			
			while ( true )
			{
				int number2;
				LPSETITEM_TYPEINFO pointer;

				Token = WzMemScript.GetToken();
				number2=WzMemScript.GetNumber();
				if ( ( Token== 0) && (strcmp("end", WzMemScript.GetString() )== 0))
				{
					break;
				}
				else
				{
					pointer = &this->m_SetItemTypeInfo[number*MAX_SUBTYPE_ITEMS+number2];

					Token=WzMemScript.GetToken();
					pointer->OptionIndex[0]=WzMemScript.GetNumber();
					Token=WzMemScript.GetToken();
					pointer->OptionIndex[1]=WzMemScript.GetNumber();
					Token=WzMemScript.GetToken();
					pointer->ChaosMixLevel[0]=WzMemScript.GetNumber();
					Token=WzMemScript.GetToken();
					pointer->ChaosMixLevel[1]=WzMemScript.GetNumber();

					pointer->IsLoad=true;
				}
			}
		}
		
	}

	return 1;
}


BOOL CSetItemOption::LoadOptionInfo(LPSTR Buffer, int iSize)
{
	CWzMemScript WzMemScript;
	int i;
	int Token;
	
	WzMemScript.SetBuffer(Buffer, iSize);

	memset(&this->m_SetItemOptionInfo, 0, sizeof(this->m_SetItemOptionInfo));

	for ( i=0;i<MAX_SETITEM_OPTION;i++)
	{
		memset(&this->m_SetItemOptionInfo[i].SetOptionTable[0], -1, sizeof(this->m_SetItemOptionInfo[0].SetOptionTable) );
		memset(&this->m_SetItemOptionInfo[i].SetExPOptionTable, -1, sizeof(this->m_SetItemOptionInfo[0].SetExPOptionTable) );
		memset(&this->m_SetItemOptionInfo[i].SetFullOtionTable[0], -1, sizeof(this->m_SetItemOptionInfo[0].SetFullOtionTable) );
	}

	while ( true )
	{
		Token=WzMemScript.GetToken();
		int number=WzMemScript.GetNumber();
		
		if (Token==0 && strcmp("end", WzMemScript.GetString()) == 0)
		{
			break;
		}
		else
		{
			LPSETITEM_OPTIONINFO p;
			p=&this->m_SetItemOptionInfo[number];

			Token=WzMemScript.GetToken();
			strcpy(&p->Name[0], WzMemScript.GetString());

			for (i=0;i<OPTION_TABLE_SIZE;i++)
			{
				Token=WzMemScript.GetToken();
				p->SetOptionTable[i][0] = WzMemScript.GetNumber();
				Token=WzMemScript.GetToken();
				p->SetOptionTableValue[i][0] =WzMemScript.GetNumber();

				Token=WzMemScript.GetToken();
				p->SetOptionTable[i][1] = WzMemScript.GetNumber();
				Token=WzMemScript.GetToken();
				p->SetOptionTableValue[i][1] =WzMemScript.GetNumber();

				if ( (p->SetOptionTable[i][0] != -1) || (p->SetOptionTable[i][1] != -1) )
				{
					p->SetOptionCount++;
				}
			}
			
			// Special Set Item Options
			Token=WzMemScript.GetToken();
			p->SetExPOptionTable[0]  = WzMemScript.GetNumber();
			Token=WzMemScript.GetToken();
			p->SetExPOptionTableValue[0] = WzMemScript.GetNumber();

			if ( p->SetExPOptionTable[0] != -1)
			{
				p->SetOptionCount ++;
			}

			Token=WzMemScript.GetToken();
			p->SetExPOptionTable[1]  = WzMemScript.GetNumber();
			Token=WzMemScript.GetToken();
			p->SetExPOptionTableValue[1] = WzMemScript.GetNumber();

			if ( p->SetExPOptionTable[1] != -1)
			{
				p->SetOptionCount ++;
			}
			
			// Full Type Set Item
			Token=WzMemScript.GetToken();
			p->SetFullOtionTable[0]=WzMemScript.GetNumber();
			Token=WzMemScript.GetToken();
			p->SetFullOtionTableValue[0]=WzMemScript.GetNumber();

			Token=WzMemScript.GetToken();
			p->SetFullOtionTable[1]=WzMemScript.GetNumber();
			Token=WzMemScript.GetToken();
			p->SetFullOtionTableValue[1]=WzMemScript.GetNumber();

			Token=WzMemScript.GetToken();
			p->SetFullOtionTable[2]=WzMemScript.GetNumber();
			Token=WzMemScript.GetToken();
			p->SetFullOtionTableValue[2]=WzMemScript.GetNumber();

			Token=WzMemScript.GetToken();
			p->SetFullOtionTable[3]=WzMemScript.GetNumber();
			Token=WzMemScript.GetToken();
			p->SetFullOtionTableValue[3]=WzMemScript.GetNumber();

			Token=WzMemScript.GetToken();
			p->SetFullOtionTable[4]=WzMemScript.GetNumber();
			Token=WzMemScript.GetToken();
			p->SetFullOtionTableValue[4]=WzMemScript.GetNumber();

			// Save Character Class Disponibility
			Token=WzMemScript.GetToken();
			p->RequireClass[CLASS_WIZARD]=WzMemScript.GetNumber();

			Token=WzMemScript.GetToken();
			p->RequireClass[CLASS_KNIGHT]=WzMemScript.GetNumber();

			Token=WzMemScript.GetToken();
			p->RequireClass[CLASS_ELF]=WzMemScript.GetNumber();

			Token=WzMemScript.GetToken();
			p->RequireClass[CLASS_MAGICGLADIATOR]=WzMemScript.GetNumber();

			Token=WzMemScript.GetToken();
			p->RequireClass[CLASS_DARKLORD]=WzMemScript.GetNumber();

			Token=WzMemScript.GetToken();
			p->RequireClass[CLASS_SUMMONER]=WzMemScript.GetNumber();
			
			if(ReadConfig.S6E1 == 1)
			{
				Token=WzMemScript.GetToken();
				p->RequireClass[CLASS_RAGEFIGHTER]=WzMemScript.GetNumber();
			}
			
			// Active SetItem
			p->IsLoad=true;			
		}
	}

	return 1;
}

int CSetItemOption::IsSetItem(int itemnum)	
{
	if (itemnum<0 || itemnum > MAX_ITEMS-1)
	{
		LogAdd("IsSetItem Check error: (itemnum:%d)", itemnum);
		return 0;
	}
	return this->m_SetItemTypeInfo[itemnum].IsLoad ;
}



int CSetItemOption::GetOptionTableIndex(int itemnum, int setoption)	
{
	if (itemnum<0 || itemnum > MAX_ITEMS-1)
	{
		LogAdd("GetOptionTableIndex Check error: (itemnum:%d)", itemnum);
		return -1;
	}

	if (setoption != 1 && setoption != 2)
	{
		LogAdd("GetOptionTableIndex setoption Check error: (itemnum:%d)", itemnum);
		return -1;
	}

	if (this->m_SetItemTypeInfo[itemnum].IsLoad == FALSE)
	{
		return -1;
	}

	return this->m_SetItemTypeInfo[itemnum].OptionIndex[setoption-1];
}



char * CSetItemOption::GetSetOptionName(int optionindex)
{
	if ( optionindex < 0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		LogAdd("GetSetOptionName Check error: (itemnum:%d)", optionindex);
		return "";
	}
	if (this->m_SetItemOptionInfo[optionindex].IsLoad   ==0)
	{
		return "";
	}
	return &this->m_SetItemOptionInfo[optionindex].Name[0];
}





	
LPSTR CSetItemOption::GetSetOptionName(int itemnum, int settype)
{
	int optionindex;

	if (itemnum<0 || itemnum > MAX_ITEMS-1)
	{
		LogAdd("GetSetOptionName Check error: (itemnum:%d)", itemnum);
		return "";
	}

	if (this->m_SetItemTypeInfo[itemnum].IsLoad == FALSE)
	{
		return "";
	}
	
	optionindex= this->m_SetItemTypeInfo[itemnum].OptionIndex[settype-1];
	
	if (optionindex<0 || optionindex > MAX_SETITEM_OPTION-1)
	{	
		LogAdd("GetSetOptionName Check error: (itemnum:%d)", optionindex);
		return "";
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad  == 0)
	{
		return "";
	}

	return &this->m_SetItemOptionInfo[optionindex].Name[0];
}


int CSetItemOption::GetSetOption(int optionindex, int optablenum, int& op1, int& op2, int& opvalue1,int& opvalue2, int Class, int ChangeUp)
{
	opvalue1=0;
	opvalue2=0;

	if (optionindex <0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		LogAdd("GetSetOption Check error: (itemnum:%d)", optionindex);
		return 0;
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad==0)
	{
		return 0;
	}

	if (optablenum >= this->m_SetItemOptionInfo[optionindex].SetOptionCount)
	{
		return 0;
	}
	int isRequireClass;

	isRequireClass=this->IsRequireClass(optionindex, Class, ChangeUp);

	if (optablenum <OPTION_TABLE_SIZE)
	{
		op1=this->m_SetItemOptionInfo[optionindex].SetOptionTable[optablenum][0];
		opvalue1=this->m_SetItemOptionInfo[optionindex].SetOptionTableValue[optablenum][0];

		if ( op1 > 183 )
		{
			if ( isRequireClass == 0 )
			{
				op1 = -1;
			}
		}

		if ( op1 != -1 )
		{
			op1 += 0xA0;
		}

		op2=this->m_SetItemOptionInfo[optionindex].SetOptionTable[optablenum][1];
		opvalue2=this->m_SetItemOptionInfo[optionindex].SetOptionTableValue[optablenum][1];

		if ( op2 > 183 )
		{
			if ( isRequireClass == 0 )
			{
				op2 = -1;
			}
		}

		if ( op2 != -1 )
		{
			op2 += 0xA0;
		}
	}
	else
	{
		op1=this->m_SetItemOptionInfo[optionindex].SetExPOptionTable[optablenum-OPTION_TABLE_SIZE];
		opvalue1=this->m_SetItemOptionInfo[optionindex].SetExPOptionTableValue[optablenum-OPTION_TABLE_SIZE];

		if ( op1 > 183 )
		{
			if ( isRequireClass == 0 )
			{
				op1 = -1;
			}
		}

		if ( op1 != -1 )
		{
			op1 += 0xA0;
		}

		op2 = -1;
	}

	return 1;
}


int CSetItemOption::GetMaxSetOptionCount(int optionindex)
{
	if (optionindex <0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		LogAdd("GetMaxOptionCount Check error: (itemnum:%d)", optionindex);
		return -1;
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad == 0)
	{
		return -1;
	}

	return this->m_SetItemOptionInfo[optionindex].SetOptionCount;
}

int CSetItemOption::GetGetFullSetOption(int optionindex, int& op1, int& op2, int& op3, int& op4, int& op5, int& opvalue1, int& opvalue2, int& opvalue3, int& opvalue4, int& opvalue5, int Class, int ChangeUp)
{
	opvalue1 = 0;
	opvalue2 = 0;
	opvalue3 = 0;
	opvalue4 = 0;
	opvalue5 = 0;

	if (optionindex <0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		LogAdd("GetMaxOptionCount Check error: (itemnum:%d)", optionindex);
		return 0;
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad == 0)
	{
		return 0;
	}

	BOOL isRequireClass=this->IsRequireClass(optionindex, Class, ChangeUp);
	op1 = this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[0];
	opvalue1 = this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[0];

	if ( op1 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op1 = -1;
		}
	}

	if ( op1 != -1 )
	{
		op1 += 0xA0;
	}

	op2=this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[1];
	opvalue2=this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[1];

	if ( op2 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op2 = -1;
		}
	}

	if ( op2 != -1 )
	{
		op2 += 0xA0;
	}

	op3=this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[2];
	opvalue3=this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[2];

	if ( op3 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op3 = -1;
		}
	}

	if ( op3 != -1 )
	{
		op3 += 0xA0;
	}

	op4=this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[3];
	opvalue4=this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[3];

	if ( op4 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op4 = -1;
		}
	}

	if ( op4 != -1 )
	{
		op4 += 0xA0;
	}

	op5=this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[4];
	opvalue5=this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[4];

	if ( op5 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op5 = -1;
		}
	}

	if ( op5 != -1 )
	{
		op5 += 0xA0;
	}

	return 1;
}

BOOL CSetItemOption::IsRequireClass(int optionindex, int Class, int ChangeUP)
{
	int requireclass = 0;
	int addChangeUp = 0;
	
	if (optionindex <0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		LogAdd("GetMaxOptionCount Check error: (itemnum:%d)", optionindex);
		return 0;
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad == 0)
	{
		return 0;
	}

	if ( Class <0 || Class > MAX_TYPE_PLAYER-1)
	{
		return 0;
	}

	requireclass= this->m_SetItemOptionInfo[optionindex].RequireClass[Class];
	if (requireclass == 0)
	{
		return 0;
	}

	if (ChangeUP > 0)
	{
		if ((Class == CLASS_DARKLORD) || (Class == CLASS_MAGICGLADIATOR) || (Class == CLASS_RAGEFIGHTER))
			addChangeUp = 2;
		else
			addChangeUp = 1;
	}

	if (requireclass > 1)
	{
		if ( requireclass > (ChangeUP+addChangeUp) )
		{
			return 0;
		}
	}
	return 1;
}



int CSetItemOption::CheckMixContition(int itemnum, int mixitemlevel)
{
	if (mixitemlevel == -1)
	{
		return 0;
	}

	if ( itemnum <0 || itemnum > MAX_ITEMS-1)
	{
		LogAdd("MakeSetOption Check error: (itemnum:%d)", itemnum);
		return 0;
	}

	if (this->m_SetItemTypeInfo[itemnum].IsLoad == 0)
	{
		return 0;
	}

	if (this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[0] == mixitemlevel || this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[1] == mixitemlevel)
	{
		return 1;
	}

	return 0;
}

int CSetItemOption::MakeSetOption(int itemnum, int mixitemlevel)
{
	if ( itemnum <0 || itemnum > MAX_ITEMS-1)
	{
		LogAdd("MakeSetOption Check error: (itemnum:%d)", itemnum);
		return 0;
	}

	if (this->m_SetItemTypeInfo[itemnum].IsLoad == FALSE)
	{
		return 0;
	}

	if (this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[0] == this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[1])
	{
		return (rand() % 2) +1;
	}
	else if ( this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[0] == mixitemlevel )
	{
		return 1;
	}
	else if ( this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[1] == mixitemlevel )
	{
		return 2;
	}

	return 0;
}

int CSetItemOption::GenRandomItemNum()
{
	int iItemIndex;
	do
	{
		int type = (rand()%MAX_TYPE_ITEMS);
		int maxindex = ( g_MaxItemIndexOfEachItemType[type] <= 0 ) ?  MAX_SUBTYPE_ITEMS : g_MaxItemIndexOfEachItemType[type];
		int index = rand() % maxindex;
		iItemIndex = ITEMGET(type, index);
		
		if ( this->IsSetItem(iItemIndex) != FALSE && ItemAttribute[iItemIndex].MondownFlag == 1)
			return iItemIndex;
	}
	while ( true );

	return ITEMGET(0,0);
}

int CSetItemOption::GenSetOption(int itemnum)
{
	if ( itemnum < 0 || itemnum > MAX_ITEMS-1)
	{
		LogAdd("GenSetType Check error: (itemnum:%d)", itemnum);
		return FALSE;
	}

	if ( this->m_SetItemTypeInfo[itemnum].IsLoad == FALSE )
		return FALSE;

	int SetOption;

	if ( this->m_SetItemTypeInfo[itemnum].OptionIndex[0] &&
		 this->m_SetItemTypeInfo[itemnum].OptionIndex[1] )
	{
		SetOption = rand() % 2 + 1;
	}
	else if ( this->m_SetItemTypeInfo[itemnum].OptionIndex[0] )
	{
		SetOption = 1;
	}
	else
	{
		SetOption = 2;
	}

	if ( (rand()%100) < 80 )
	{
		SetOption |= 4;
	}
	else
	{
		SetOption |= 8;
	}

	return SetOption;
}

