#ifndef _STAGELAYER_H_
#define _STAGELAYER_H_

#include "cocos2d.h"
#include "GameScene.h"

class StageLayer : public cocos2d::CCLayer 
{    
public:
    int _stage;
    
    virtual bool init();
    static cocos2d::CCScene *scene();
    int totalStages();
    void selectStage(CCObject *sender);
    
	CREATE_FUNC(StageLayer);
};

#endif