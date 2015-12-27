#ifndef ANTIAFKDEF_H
#define ANTIAFKDEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (PACK_EDITION>=3)

class ANTIAFKCHARINFO
{
public:
	ANTIAFKCHARINFO();
	void Clear();

	BYTE AntiAFK_Map;
	BYTE AntiAFK_X;
	BYTE AntiAFK_Y;
	BYTE AntiAFK_WarnNumber;
};

class AntiAFKClass
{
public:
	void Init(char * FilePath);

//vars:
	BYTE Enabled;
	short Minutes;
	short Warnings;
};

extern AntiAFKClass AntiAFK;
void AntiAFKClass__Trigger(void * lpParam);

#endif

#endif