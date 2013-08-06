#include <stdlib.h>

#include "Global.h"
#include "StageLayer.h"

#define TotalStages 10

using namespace cocos2d;

int StageLayer::totalStages()
{
    return TotalStages;
}

CCScene *StageLayer::scene()
{
    CCScene *scene = CCScene::create();
    StageLayer *layer = StageLayer::create();
    
    scene->addChild(layer);
 
    return scene;
}

bool StageLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCMenu *stages = CCMenu::create();
    int i;
    for (i = 0; i < this->totalStages(); i++)
    {
        char a[10];
        sprintf(a, "%d", i);
		//CCLabelTTF *label = CCLabelTTF::create(a,"", 25)
        CCMenuItemFont *stageMenuItem = CCMenuItemFont::create(a, 
                                                                       this,
                                                                       menu_selector(StageLayer::selectStage));
        stages->addChild(stageMenuItem);
    }
    stages->alignItemsHorizontallyWithPadding(15);
    this->addChild(stages);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *backgroudSprite = CCSprite::create(mainBackgroudFile);
	backgroudSprite->setPosition(CCPointMake(winSize.width/2, winSize.height/2));
	addChild(backgroudSprite, -2);

    return true;
}

void StageLayer::selectStage(CCObject *sender)
{
    CCMenuItemLabel *item = (CCMenuItemFont *)sender;
    CCLabelTTF *stageLabel = (CCLabelTTF *)item->getLabel();
	int stage = (int)(stageLabel->getString()[0] - '0');

    CCScene *scene = GameScene::sceneWithStage(stage);
    CCDirector::sharedDirector()->replaceScene(scene);
}