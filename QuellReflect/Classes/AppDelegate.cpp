#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "BoxMenu.h"
#include "GameScene.h"
#include "StageLayer.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn off display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	CCScene *pScene = BoxMenu::scene();
    // run
	pDirector->runWithScene(pScene);
	/* Ìí¼Ó±³¾°ÒôÀÖ */
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("../Resources/music6.mp3", true);
	SimpleAudioEngine::sharedEngine()->preloadEffect("../Resources/levelcomplete.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("../Resources/pairvanish.mp3");

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
