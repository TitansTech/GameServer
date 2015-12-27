#define MSGGET(x, y) ( (x) * 256 + (y)  )

class CMsg
{
public:
	void DataFileLoad(char * filename);
	LPSTR Get(int);
private:
	char msg[32767][256];

};

extern CMsg lMsg;