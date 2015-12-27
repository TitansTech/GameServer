#ifndef GUARDIAN_H
#define GUARDIAN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (GS_CASTLE==1)
class CGuardian
{
public:

	CGuardian();
	virtual ~CGuardian();

	BOOL CreateGuardian(int iIndex, BYTE cTX, BYTE cTY);
	void GuardianAct(int iIndex);
	BOOL DeleteGuardian(int iIndex);

private:
	
	
};

extern CGuardian g_CsNPC_Guardian;
#endif

#endif