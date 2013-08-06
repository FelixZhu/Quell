#include "Score.h"
#include "CharTran.h"

Score *Score::m_pInstance = 0;
int Score::teamTag = 0;
TEAM_S Score::teamScore[7];

Score::Score()
{
	teamTag = 0;
	for (int i=0; i<7; i++)
	{
		teamScore[i].iScore = 0;
	}
}

Score *Score::GetInstance()
{
	if (m_pInstance == ((void *)0))
	{
		m_pInstance = new Score();

		std::string smallBirdString="小小鸟队";
		std::string pigString = "掉队的小猪";
		GBKToUTF8(smallBirdString,"gbk","utf-8");
		GBKToUTF8(pigString,"gbk","utf-8");
		strcpy(teamScore[0].name, "LJX:            ");
		strcpy(teamScore[1].name, "XXX:           ");
		sprintf(teamScore[2].name, "%s:    ", smallBirdString.c_str());
		strcpy(teamScore[3].name, "000:           ");
		strcpy(teamScore[4].name, "[317]:         ");
		sprintf(teamScore[5].name, "%s: ", pigString.c_str());
		strcpy(teamScore[6].name, "fngng:        ");

		makeNameScore();
	}

	return m_pInstance;
}

void Score::sortByScore()
{
	int i, j;
	TEAM_S stTmp;

	for (i =  0; i < 7; i++)
	{
		for (j = i+1; j < 7; j++)
		{
			if (teamScore[i].iScore < teamScore[j].iScore)
			{
				stTmp.iScore = teamScore[i].iScore;
				strcpy(stTmp.name, teamScore[i].name);
				teamScore[i].iScore = teamScore[j].iScore;
				strcpy(teamScore[i].name, teamScore[j].name);
				teamScore[j].iScore = stTmp.iScore;
				strcpy(teamScore[j].name, stTmp.name);
			}
		}
	}
}

void Score::makeNameScore()
{
	sprintf(teamScore[0].nameScore, "%s%d", teamScore[0].name, teamScore[0].iScore);
	sprintf(teamScore[1].nameScore, "%s%d", teamScore[1].name, teamScore[1].iScore);
	sprintf(teamScore[2].nameScore, "%s%d", teamScore[2].name, teamScore[2].iScore);
	sprintf(teamScore[3].nameScore, "%s%d", teamScore[3].name, teamScore[3].iScore);
	sprintf(teamScore[4].nameScore, "%s%d", teamScore[4].name, teamScore[4].iScore);
	sprintf(teamScore[5].nameScore, "%s%d", teamScore[5].name, teamScore[5].iScore);
	sprintf(teamScore[6].nameScore, "%s%d", teamScore[6].name, teamScore[6].iScore);
}