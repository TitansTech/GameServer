
class NewsClass
{
public:
	void Init(char * FilePath);

//Vars:
	BYTE	NewsEnabled;
	BYTE	MinutesBetweenEachNews;
	BYTE	TotalNewsSections;
	BYTE	TotalNewsLines;
	BYTE	CurrentNewsPosition;
	char	NewsSectionsList[99][64];
};

void NewsClass__Announce(void * lpParam);

extern NewsClass NewsSystem;