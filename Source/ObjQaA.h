

#define QaA_QUESTIONSCOUNT		50
#define QaA_ANSWERSCOUNT		3
#define MAX_QaA_REWARD			50

struct QuestionsStruct
{
	BYTE ID;
	char Question[100];
	char CorrectAnswer;
	char Answer[QaA_ANSWERSCOUNT][100];
	BYTE AnswersCnt;
};

class cObjQaA
{
public:
	void Read(char * FilePath);
	void StartEvent();
	void AnswerCmd(int aIndex, char * pAnswer);
	void SendReward();
	int SearchQuestion(BYTE ID);
	
	QuestionsStruct 		Question[QaA_QUESTIONSCOUNT];
	botReward_Data_Struct 	reward[MAX_QaA_REWARD];
	BYTE QuestionsCnt;
	BYTE RewardCount;
	BYTE Enabled;
	BYTE MaxQuestions;
	int CurrentQuestion;
	int PlayerIDReply;
	bool Start;
	bool StartAnswer;
};

void cObjQaA__InsideTrigger(void * lpParam);

extern cObjQaA ObjQaA;