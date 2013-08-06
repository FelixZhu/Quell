#ifndef _BOXMENU_H_
#define _BOXMENU_H_

#include "cocos2d.h"

using namespace std;

class BoxMenu : public cocos2d::CCLayer 
{
public:
    static cocos2d::CCScene *scene();

private:
	virtual bool init();  
	cocos2d::CCMenu *getBoxMenu();
	cocos2d::CCMenu *getTeamMenu();
	void newGame(CCObject* pSender);
	void selectStage(CCObject* pSender);
    void scoreSort(CCObject* pSender);

	CREATE_FUNC(BoxMenu);

};
#endif