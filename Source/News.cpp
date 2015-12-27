#include "stdafx.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
//#include "GameServerAuth.h"
#include "News.h"
#include <strsafe.h>

NewsClass NewsSystem;

void NewsClass::Init(char * FilePath)
{
	char	news_line[12];
	int		i=0;

	NewsEnabled				= 0;
	MinutesBetweenEachNews	= 0;
	TotalNewsSections		= 0;
	TotalNewsLines			= 0;
	CurrentNewsPosition		= 0;
	memset(&NewsSectionsList[0],0,sizeof(NewsSectionsList));

	//LogLine("%s\n", "Loading News settings...");
	NewsEnabled = GetPrivateProfileInt("Config", "SCF_EnableNews", 0, FilePath);
	
	if(NewsEnabled) 
	{
		//Loading News Settings
		MinutesBetweenEachNews	= GetPrivateProfileInt("Config", "SCF_MinBetweenEachNews", 10, FilePath);
		TotalNewsSections		= GetPrivateProfileInt("Config", "SCF_TotalNewsSections", 5, FilePath);

		if (TotalNewsSections>33) {TotalNewsSections=33;}

		TotalNewsLines=(TotalNewsSections*3)+1;
		//Loading News Records
		for (i=0;i<TotalNewsLines;i++)
		{
			StringCchPrintf(news_line, 12, "SCF_News_%02d", i);
			GetPrivateProfileString("News", news_line, "\0", NewsSectionsList[i], 64, FilePath);
		}

		//Transfer to Seconds from minutes
		//MinutesBetweenEachNews	= (MinutesBetweenEachNews ;

		//Launching News Timer
		_beginthread( NewsClass__Announce, 0, NULL  );
	}

	//Log portion for bug tracing
	LogAddTD("[News System] Enabled: %d / Total News Sections: %d [%d] / Time Interval (min): %d ",NewsEnabled,TotalNewsSections,TotalNewsSections*3,MinutesBetweenEachNews);

	if(NewsEnabled) 
	{
		//Displaying Loaded News [DEBUG]
		for(i=1;i<TotalNewsLines;i++)
		{
			StringCchPrintf(news_line, 12, "SCF_News_%02d", i);
		}
	}
}
void NewsClass__Announce(void * lpParam)
{
	while(NewsSystem.NewsEnabled)
	{
		Sleep(NewsSystem.MinutesBetweenEachNews * 60 * 1000);

		int	counter=0;
		
		if(NewsSystem.CurrentNewsPosition+3<NewsSystem.TotalNewsLines) 
		{
			for (counter=0;counter<3;counter++)
			{
				NewsSystem.CurrentNewsPosition=NewsSystem.CurrentNewsPosition+1;
				AllSendServerMsg(NewsSystem.NewsSectionsList[NewsSystem.CurrentNewsPosition]);
				LogAddTD("[News System][%d] %s", NewsSystem.MinutesBetweenEachNews, NewsSystem.NewsSectionsList[NewsSystem.CurrentNewsPosition]);
			}
		}else 
		{
			NewsSystem.CurrentNewsPosition = 0;
			for (counter=0;counter<3;counter++)
			{
				NewsSystem.CurrentNewsPosition=NewsSystem.CurrentNewsPosition+1;
				AllSendServerMsg(NewsSystem.NewsSectionsList[NewsSystem.CurrentNewsPosition]);
				LogAddTD("[News System][%d] %s", NewsSystem.MinutesBetweenEachNews, NewsSystem.NewsSectionsList[NewsSystem.CurrentNewsPosition]);
			}
		}
		
	}
	_endthread();
	return;
}
