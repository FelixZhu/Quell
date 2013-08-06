#include "BoxMenu.h"
#include "StageLayer.h"
#include "GameScene.h"
#include "SeedPK.h"
#include "Global.h"
#include "CharTran.h"
#include "Score.h"

using namespace cocos2d;

CCScene *BoxMenu::scene()
{
    CCScene *scene = CCScene::create();
    BoxMenu *layer = BoxMenu::create();
    scene->addChild(layer);
    
    return scene;    
}

bool BoxMenu::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
	CCMenu *teamMenu;
	CCMenuItemFont::setFontName("Î¢ÈíÑÅºÚ");
	CCMenuItemFont::setFontSize(20);
	teamMenu = getBoxMenu();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();	
	string gameTitle = GameTitleString;
	GBKToUTF8(gameTitle,"gbk","utf-8");
	CCMenuItemFont::setFontSize(26);
	CCMenuItemFont *titleItem = CCMenuItemFont::create(gameTitle.c_str(), this, menu_selector(BoxMenu::scoreSort));
	titleItem->setColor(ccc3(0xc3, 0xa8, 0x91));
	titleItem->setPosition(CCPointMake(0, winSize.height/2 - 30));
	teamMenu->addChild(titleItem);

	CCSprite *backgroudSprite = CCSprite::create(mainBackgroudFile);
	backgroudSprite->setPosition(CCPointMake(winSize.width/2, winSize.height/2));

	addChild(teamMenu, 0);
	addChild(backgroudSprite, -2);

    return true;
}

CCMenu *BoxMenu::getBoxMenu()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();

	Score *scoreInstance = Score::GetInstance();
	scoreInstance->makeNameScore();
	TEAM_S *pstTeamScore = scoreInstance->teamScore;

	CCMenuItemFont *ljxTeam = CCMenuItemFont::create(pstTeamScore[0].nameScore, this, menu_selector(BoxMenu::newGame));
	CCMenuItemFont *xxxTeam = CCMenuItemFont::create(pstTeamScore[1].nameScore, this, menu_selector(BoxMenu::newGame));
	CCMenuItemFont *birdTeam = CCMenuItemFont::create(pstTeamScore[2].nameScore, this, NULL);
	CCMenuItemFont *oooTeam = CCMenuItemFont::create(pstTeamScore[3].nameScore, this, menu_selector(BoxMenu::newGame));
	CCMenuItemFont *threeTeam = CCMenuItemFont::create(pstTeamScore[4].nameScore, this, menu_selector(BoxMenu::newGame));
	CCMenuItemFont *pigTeam = CCMenuItemFont::create(pstTeamScore[5].nameScore, this, menu_selector(BoxMenu::newGame));
	CCMenuItemFont *fngngTeam = CCMenuItemFont::create(pstTeamScore[6].nameScore, this, menu_selector(BoxMenu::newGame));
/*
	ljxTeam->setColor(ccBLACK);
	xxxTeam->setColor(ccBLACK);
	birdTeam->setColor(ccBLACK);
	oooTeam->setColor(ccBLACK);
	threeTeam->setColor(ccBLACK);
	pigTeam->setColor(ccBLACK);
	fngngTeam->setColor(ccBLACK);
*/
	ljxTeam->setTag(TeamLJX);
	xxxTeam->setTag(TeamXXX);
	birdTeam->setTag(TeamBird);
	oooTeam->setTag(TeamOOO);
	threeTeam->setTag(TeamThree);
	pigTeam->setTag(TeamPig);
	fngngTeam->setTag(TeamFngng);
    
    CCMenu *menu = CCMenu::create(ljxTeam, xxxTeam, birdTeam, oooTeam, threeTeam, pigTeam, fngngTeam, NULL);
    menu->alignItemsVertically();
    
    return menu;
}

void BoxMenu::newGame(CCObject* pSender)
{
	Score *scoreInstance = Score::GetInstance();

	CCMenuItem *menuItem = (CCMenuItem *)pSender;
	scoreInstance->teamTag = menuItem->getTag();

	std::cout << "get tag:" << scoreInstance->teamTag << std::endl;

    CCDirector::sharedDirector()->replaceScene(GameScene::sceneWithStage(0));
}

void BoxMenu::selectStage(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(StageLayer::scene());
}

void BoxMenu::scoreSort(CCObject* pSender)
{
	Score *teamScore = Score::GetInstance();
	teamScore->sortByScore();

	CCScene *pScene = BoxMenu::scene();
	CCTransitionFade *tran = CCTransitionFade::create(1.0f, pScene, ccBLACK);
	CCDirector::sharedDirector()->replaceScene(tran);
}