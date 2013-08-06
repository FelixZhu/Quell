#ifndef _SCORE_
#define _SCORE_

typedef struct tagTeam {
	int iScore;
	char name[20];
	char nameScore[30];
}TEAM_S;

class Score 
{  
public:  
	static Score *GetInstance();
	static int teamTag;
	static TEAM_S teamScore[7];

	void sortByScore(void);
	static void makeNameScore(void);

private:  
	Score();
	static Score *m_pInstance;  
} ;

#endif