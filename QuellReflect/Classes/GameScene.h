#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "cocos2d.h"
#include "Global.h"

class GameScene : public cocos2d::CCLayer
{
public:
    virtual bool init(void);

	/* create函数的重写 */
	static cocos2d::CCScene *sceneWithStage(int stage);

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);

/* 用于显示的私有方法 */
private:
	GameScene();
	~GameScene();

	/* 真实坐标与地图坐标之间的转换，地图坐标是32*32为一个单位 */
	cocos2d::CCPoint toMapPos(cocos2d::CCPoint pos);

	/* 地图坐标与真实坐标之间的转换 */
    cocos2d::CCPoint toRealPos(cocos2d::CCPoint pos);

	/* 初始化_stage变量 */
    void initWithStage(int stage);

	/* 输入移动方向返回那个方向的运动动画 */
    cocos2d::CCAnimation *createAnimationByDirection(MoveDirection direction);

	/* 方向键按下的回调函数 */
	void menuCallBackMove(cocos2d::CCObject *pSender);

	/* replay按钮的回调 */
	void replayCallBack(cocos2d::CCObject *pSender);

	/* next按钮的回调 */
	void nextStage(cocos2d::CCObject *psender);

	/* 动画完成的回调函数 */
	void onWalkDone(cocos2d::CCNode *pSender, void *data);
	void onMoveDone(void);

/* 用于显示的私有数据 */
private:
    /* 当前关卡 */
	int _stage;
	/* 当前动作是否完成 */
	bool m_bIsMovingFinished;
    /* 地图的数据结构 */
    cocos2d::CCTMXTiledMap *_tileMap;
    /* 从地图里面读出来的层 */
	cocos2d::CCTMXLayer *_backgroundLayer;
	/* 当前步数的显示 */
	cocos2d::CCLabelTTF *curStepLabel;
	/* 当前分数的显示 */
	cocos2d::CCLabelTTF *curScoreLabel;
    /* 动画数组 */
    cocos2d::CCAnimation **walkAnimation;
    /* 精灵 */
    cocos2d::CCSprite *heroSprite;
	/* 显示各种道具的frame */
	cocos2d::CCSpriteFrame *openGateFrame;
	cocos2d::CCSpriteFrame *yellowPearlFrame;
	cocos2d::CCSpriteFrame *whitePearlFrame;
	int iTagForOpenGate;

    /* 用于精灵状态转变的数组及动作 */
	cocos2d::CCArray *turnStateArray;

	/* 方向按钮数组 */
	cocos2d::CCMenu *arrowMenu;

    /* 定时器，用于定时调用选手的结果 */
    cocos2d::CCTimer *m_timer;

/* 用于游戏逻辑控制的私有方法 */
private:
    /* 载入地图(注意是txt的地图文件，不是tiledMap) */
    void initQuellReflectMap(const char * filename);

    /* 载入选手的操作答案 */
    void loadAnswerToRun(CCObject *pSender);

    /* 移动雨滴， rainDropNumber代表要移动的雨滴编号(因为当前地图可能不止一个雨滴)，direction代表该雨滴的移动方向 */
    void moveRainDrop(int rainDropNumber, MoveDirection direction);

    /* 通过选手的答案去自动移动 */
    void moveRainDropUsingAnswer(float ct);

    /* 检查游戏是否结束 */
    bool isGameOver();

    /* 雨滴已经移动的次数 */
    int moveCount();

    /* 通过雨滴坐标获取雨滴的编号 */
    int getRainDropNumberFromPosition(Point point);

    /* 下一步 */
    Point nextPosition(Point currentPosition, MoveDirection direction);

    /* 上一步 */
    Point previousPosition(Point currentPosition, MoveDirection direction);

	/* Point结构体转为CCPoint*  */
	cocos2d::CCPoint toCCPoint(Point prePos);

    /* 重置游戏 */
    void resetGame();

    /* 在命令行上打印文本地图 */
    void showQuellReflectMap();

/* 用于游戏逻辑控制的私有数据 */
private:
    /* 雨滴移动的次数，注意无效移动不被计入其中 */
    int m_userMoveCount;

    /* 本张地图过关最少移动次数 */
    int m_bestMoveCount;

    /* 总共的雨滴数(包括已死亡的) */
    int m_totalRainPointNumber;

    /* 目前还存在的雨滴数 */
    int m_existedRainDrop;

    /* 目前还存在的小水滴数目 */
    int m_existedPearls;

    /* 雨滴进入了死循环 */
    bool m_endlessLoop;

    /* 当前地图的雨滴 */
    RainDrop m_rainDrops[MaxRainDropNumber];
};

#endif