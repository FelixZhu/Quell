#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "cocos2d.h"
#include "Global.h"

class GameScene : public cocos2d::CCLayer
{
public:
    virtual bool init(void);

	/* create��������д */
	static cocos2d::CCScene *sceneWithStage(int stage);

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);

/* ������ʾ��˽�з��� */
private:
	GameScene();
	~GameScene();

	/* ��ʵ�������ͼ����֮���ת������ͼ������32*32Ϊһ����λ */
	cocos2d::CCPoint toMapPos(cocos2d::CCPoint pos);

	/* ��ͼ��������ʵ����֮���ת�� */
    cocos2d::CCPoint toRealPos(cocos2d::CCPoint pos);

	/* ��ʼ��_stage���� */
    void initWithStage(int stage);

	/* �����ƶ����򷵻��Ǹ�������˶����� */
    cocos2d::CCAnimation *createAnimationByDirection(MoveDirection direction);

	/* ��������µĻص����� */
	void menuCallBackMove(cocos2d::CCObject *pSender);

	/* replay��ť�Ļص� */
	void replayCallBack(cocos2d::CCObject *pSender);

	/* next��ť�Ļص� */
	void nextStage(cocos2d::CCObject *psender);

	/* ������ɵĻص����� */
	void onWalkDone(cocos2d::CCNode *pSender, void *data);
	void onMoveDone(void);

/* ������ʾ��˽������ */
private:
    /* ��ǰ�ؿ� */
	int _stage;
	/* ��ǰ�����Ƿ���� */
	bool m_bIsMovingFinished;
    /* ��ͼ�����ݽṹ */
    cocos2d::CCTMXTiledMap *_tileMap;
    /* �ӵ�ͼ����������Ĳ� */
	cocos2d::CCTMXLayer *_backgroundLayer;
	/* ��ǰ��������ʾ */
	cocos2d::CCLabelTTF *curStepLabel;
	/* ��ǰ��������ʾ */
	cocos2d::CCLabelTTF *curScoreLabel;
    /* �������� */
    cocos2d::CCAnimation **walkAnimation;
    /* ���� */
    cocos2d::CCSprite *heroSprite;
	/* ��ʾ���ֵ��ߵ�frame */
	cocos2d::CCSpriteFrame *openGateFrame;
	cocos2d::CCSpriteFrame *yellowPearlFrame;
	cocos2d::CCSpriteFrame *whitePearlFrame;
	int iTagForOpenGate;

    /* ���ھ���״̬ת������鼰���� */
	cocos2d::CCArray *turnStateArray;

	/* ����ť���� */
	cocos2d::CCMenu *arrowMenu;

    /* ��ʱ�������ڶ�ʱ����ѡ�ֵĽ�� */
    cocos2d::CCTimer *m_timer;

/* ������Ϸ�߼����Ƶ�˽�з��� */
private:
    /* �����ͼ(ע����txt�ĵ�ͼ�ļ�������tiledMap) */
    void initQuellReflectMap(const char * filename);

    /* ����ѡ�ֵĲ����� */
    void loadAnswerToRun(CCObject *pSender);

    /* �ƶ���Σ� rainDropNumber����Ҫ�ƶ�����α��(��Ϊ��ǰ��ͼ���ܲ�ֹһ�����)��direction�������ε��ƶ����� */
    void moveRainDrop(int rainDropNumber, MoveDirection direction);

    /* ͨ��ѡ�ֵĴ�ȥ�Զ��ƶ� */
    void moveRainDropUsingAnswer(float ct);

    /* �����Ϸ�Ƿ���� */
    bool isGameOver();

    /* ����Ѿ��ƶ��Ĵ��� */
    int moveCount();

    /* ͨ����������ȡ��εı�� */
    int getRainDropNumberFromPosition(Point point);

    /* ��һ�� */
    Point nextPosition(Point currentPosition, MoveDirection direction);

    /* ��һ�� */
    Point previousPosition(Point currentPosition, MoveDirection direction);

	/* Point�ṹ��תΪCCPoint*  */
	cocos2d::CCPoint toCCPoint(Point prePos);

    /* ������Ϸ */
    void resetGame();

    /* ���������ϴ�ӡ�ı���ͼ */
    void showQuellReflectMap();

/* ������Ϸ�߼����Ƶ�˽������ */
private:
    /* ����ƶ��Ĵ�����ע����Ч�ƶ������������� */
    int m_userMoveCount;

    /* ���ŵ�ͼ���������ƶ����� */
    int m_bestMoveCount;

    /* �ܹ��������(������������) */
    int m_totalRainPointNumber;

    /* Ŀǰ�����ڵ������ */
    int m_existedRainDrop;

    /* Ŀǰ�����ڵ�Сˮ����Ŀ */
    int m_existedPearls;

    /* ��ν�������ѭ�� */
    bool m_endlessLoop;

    /* ��ǰ��ͼ����� */
    RainDrop m_rainDrops[MaxRainDropNumber];
};

#endif