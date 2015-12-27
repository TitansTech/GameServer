#define GMCommands		20

struct GMStruct
{		
	char Name[11];
	BYTE CtlCode;
	BYTE Day;
	BYTE Month;
	WORD Year;
	BOOL Command[GMCommands];
};

class cGMSystem
{
public:
	void CheckStatus(LPOBJ pObj);
	BOOL CheckCommand(LPOBJ pObj,int Command);
	void ReadConfig(char * FilePath);
	BOOL Enabled;
	BOOL Announce;
private:
	GMStruct GMpObj[100];
	BYTE Count;
};

extern cGMSystem GMSystem;