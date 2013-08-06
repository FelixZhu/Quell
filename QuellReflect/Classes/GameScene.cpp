#include "SeedPK.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "BoxMenu.h"
#include "Score.h"
#include "CharTran.h"

#include "Player.h"
#include "Player317.h"
#include "PlayerBird.h"
#include "Playerfngng.h"
#include "PlayerLJX.h"
#include "PlayerOOO.h"
#include "PlayerPig.h"
#include "PlayerXXX.h"

#include <iostream>
#include <fstream>
#include <process.h>

#define MaxMoveCount 200

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

/* ѡ�ֵĴ� */
std::list<MoveDirection> m_answerList;

/* ��ǰ��ͼ���� */
CellState m_quellReflectMap[MaxMapRow][MaxMapColumn];

/* ��ǰ��ͼ���� */
int m_currentMapRow;

/* ��ǰ��ͼ���� */
int m_currentMapColumn;

/* Player������ */
Player* m_playerArr[7];

unsigned __stdcall solutionThread(void* pArguments)
{
	Score *pCurScore = Score::GetInstance();
	try
	{
		m_answerList = m_playerArr[pCurScore->teamTag /1000 -1]->solutionToMap(m_quellReflectMap, m_currentMapRow, m_currentMapColumn);
	}
	catch (...){}

	_endthreadex(0);

	return 0;
} 

GameScene::GameScene()
{
	m_playerArr[TeamLJX / 1000 -1] = new PlayerLJX();
	m_playerArr[TeamXXX / 1000 -1] = new PlayerXXX();
	m_playerArr[TeamBird / 1000 -1] = new PlayerBird();
	m_playerArr[TeamOOO / 1000 -1] = new PlayerOOO();
	m_playerArr[TeamThree / 1000 -1] = new Player317();
	m_playerArr[TeamPig / 1000 -1] = new PlayerPig();
	m_playerArr[TeamFngng / 1000 -1] = new Playerfngng();
}

/* ���������ͷŶ������ڴ� */
GameScene::~GameScene()
{
}

/* �����Լ�����һ��create���� */
CCScene* GameScene::sceneWithStage(int stage)
{
    CCScene *scene = CCScene::create();

    GameScene *layer = new GameScene(); 
    layer->initWithStage(stage);
    if (layer && layer->init()) 
    { 
        layer->autorelease(); 
    } 
    else 
    { 
        CC_SAFE_DELETE(layer); 
        layer = NULL; 
        return NULL; 
    } 
    scene->addChild(layer);

    return scene;
}

void GameScene::initWithStage(int stage)
{
    GameScene::_stage = stage;
    
    return;
}

/* ��Ҫ�Ļ����ʼ�� */
bool GameScene::init()
{
    const int BufSize = 256;
	/* �����߼���ͼ�ļ� */
	char mapFileName[BufSize];
	sprintf(mapFileName, "../Resources/level_0%d.txt", GameScene::_stage);
	initQuellReflectMap(mapFileName);

	/* ������ͨ����ͼ���������򿪵�ͼ */
	char mapName[BufSize];
	sprintf(mapName, "../Resources/level_0%d.tmx", GameScene::_stage);
    _tileMap = CCTMXTiledMap::create(mapName);
	if(NULL == _tileMap) {return false;}

	/* ���ص�ͼ */
	this->addChild(_tileMap);

	/* ���ر���ͼƬ */
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *headerSprite = CCSprite::create("../Resources/header.png");
	headerSprite->setPosition(CCPointMake(winSize.width/2, winSize.height-32));
	CCSprite *backgroudSprite = CCSprite::create("../Resources/loading2.png");
	backgroudSprite->setPosition(CCPointMake(winSize.width/2, winSize.height/2));
	addChild(backgroudSprite, -2);
	addChild(headerSprite, -2);

	/* ��ȡˮ�εĳ�ʼ���� */
	CCTMXObjectGroup *objects = _tileMap->objectGroupNamed("object");
	CCDictionary *objPoint = objects->objectNamed("CellRainDrop0");
	int x, y;
	x = objPoint->valueForKey("x")->intValue();
	y = objPoint->valueForKey("y")->intValue();

	/* ͨ��ͼƬ������ˮ�� */
	whitePearlFrame = CCSpriteFrame::create(GameAtlasFile, CCRectMake(0, 0, TileWidth, TileHeight));
	heroSprite = CCSprite::createWithSpriteFrame(whitePearlFrame);
	heroSprite->setPosition(CCPointMake(x + TileWidth / 2, y + TileHeight / 2));
	this->addChild(heroSprite);

	/* ��ȡˮ��ĳ�ʼ���� */
	/* ����һ��ʼ��ʹ�õ�array�����б��棬��������ò������array�ǵ�ɾ�� */
	for (int i = 0; i < m_existedPearls; i++)
	{
		char pearlStr[20];
		sprintf(pearlStr, "CellNormalPearl%d", i);
		CCDictionary *objPoint = objects->objectNamed(pearlStr);

		if (NULL == objPoint)
        {
            break;
        }

		x = objPoint->valueForKey("x")->intValue();
		y = objPoint->valueForKey("y")->intValue();
		CCSprite *pearlSprite = CCSprite::create(GameAtlasFile, CCRectMake(TileWidth * 0, TileHeight * 4, TileWidth, TileHeight));
		pearlSprite->setPosition(CCPointMake(x + TileWidth / 2, y + TileHeight / 2));
		pearlSprite->setTag(x * m_currentMapRow / TileWidth + (m_currentMapRow - y / TileHeight - 1));
		this->addChild(pearlSprite);
	}

	/* ���ӻ�ɫˮ�����ʾ */
	for(int j = 0; j < 15; j++)
	{
		char pearlStr[20];
		sprintf(pearlStr, "CellYellowPearl%d", j);
		CCDictionary *objPoint = objects->objectNamed(pearlStr);

		if (NULL == objPoint)
        {
            break;
        }

		x = objPoint->valueForKey("x")->intValue();
		y = objPoint->valueForKey("y")->intValue();
		CCSpriteFrame *yellowPearlFrame = CCSpriteFrame::create(GameAtlasFile, CCRectMake(TileWidth * 1, TileHeight * 4, TileWidth, TileHeight));
		CCSprite *yellowPearlSprite = CCSprite::createWithSpriteFrame(yellowPearlFrame);
		yellowPearlSprite->setPosition(CCPointMake(x + TileWidth / 2, y + TileHeight / 2));
		yellowPearlSprite->setTag(x * m_currentMapRow / TileWidth + (m_currentMapRow - y / TileHeight - 1));
		this->addChild(yellowPearlSprite);
	}

	/* ���ӻ�ɫǽ����ʾ */
	for(int k = 0; k < 15; k++)
	{
		char pearlStr[20];
		sprintf(pearlStr, "CellYellowWall%d", k);
		CCDictionary *objPointDic = objects->objectNamed(pearlStr);

		if (NULL == objPointDic){break;}
		x = objPointDic->valueForKey("x")->intValue();
		y = objPointDic->valueForKey("y")->intValue();
		CCSprite *yellowWallSprite = CCSprite::create(GameAtlasFile, CCRectMake(TileWidth * 3, TileHeight * 4, TileWidth, TileHeight));
		yellowWallSprite->setPosition(CCPointMake(x + TileWidth / 2, y + TileHeight / 2));
		yellowWallSprite->setTag(x * m_currentMapRow / TileWidth + (m_currentMapRow - y / TileHeight - 1));
		this->addChild(yellowWallSprite);
	}

	/* ����opengate����ʾ */
	iTagForOpenGate = 0;
	for (int m = 0; m < 5; m++)
	{
		char openGateStr[20];
		sprintf(openGateStr, "CellOpenGate%d", m);
		CCDictionary *openGatePointDic = objects->objectNamed(openGateStr);
		if (NULL == openGatePointDic){break;}
		x = openGatePointDic->valueForKey("x")->intValue();
		y = openGatePointDic->valueForKey("y")->intValue();
		CCSprite *openGateSprite = CCSprite::create(GameAtlasFile, CCRectMake(TileWidth * 9, TileHeight*0, TileWidth, TileHeight));
		openGateSprite->setPosition(CCPointMake(x + TileWidth / 2, y+TileHeight / 2));
		iTagForOpenGate = x * m_currentMapRow / TileWidth + (m_currentMapRow - y / TileHeight - 1);
		openGateSprite->setTag(iTagForOpenGate);
		this->addChild(openGateSprite);
	}

	/* �����������Ų�������ǰ������Titile����Ϣ����ʾ */
	Score *scoreInstance = Score::GetInstance();
	TEAM_S *pstTeamScore = scoreInstance->teamScore;
	int teamTag = scoreInstance->teamTag;
	char bestStepArr[20], curStepArr[20], curScoreArr[20];
	sprintf(bestStepArr, "(%d)", m_bestMoveCount);
	sprintf(curStepArr, "%d", m_userMoveCount);
	sprintf(curScoreArr, "%d", pstTeamScore[teamTag/1000 - 1].iScore);

	string gameTitle = GameTitleString;
	GBKToUTF8(gameTitle,"gbk","utf-8");

	CCLabelTTF *titleLabel = CCLabelTTF::create(gameTitle.c_str(), "΢���ź�", 30);
	titleLabel->setColor(ccc3(0xc3, 0xa8, 0x91));
	CCLabelTTF *bestStepLabel = CCLabelTTF::create(bestStepArr, "Calibri", 20);
	curStepLabel = CCLabelTTF::create(curStepArr, "Calibri", 20);
	curScoreLabel = CCLabelTTF::create(curScoreArr, "Calibri", 20);
	curScoreLabel->setColor(ccRED);
	curScoreLabel->setFontSize(25);

	titleLabel->setPosition(CCPointMake(winSize.width / 2, winSize.height - 30));
	bestStepLabel->setPosition(CCPointMake(50, winSize.height - 45));
	curStepLabel->setPosition(CCPointMake(20, winSize.height - 45));
	curScoreLabel->setPosition(CCPointMake(35, winSize.height - 18));

	addChild(titleLabel);
	addChild(bestStepLabel);
	addChild(curStepLabel);
	addChild(curScoreLabel);

	/* �������������Ұ����ĳ�ʼ�� */
	CCMenuItem *downMenuItem = CCMenuItemImage::create(ArrowUpUnselectedFile, ArrowUpSelectedFile, this, menu_selector(GameScene::menuCallBackMove));
	CCMenuItem *upMenuItem = CCMenuItemImage::create(ArrowUpUnselectedFile, ArrowUpSelectedFile, this, menu_selector(GameScene::menuCallBackMove));
	CCMenuItem *leftMenuItem = CCMenuItemImage::create(ArrowUpUnselectedFile, ArrowUpSelectedFile, this, menu_selector(GameScene::menuCallBackMove));
	CCMenuItem *rightMenuItem = CCMenuItemImage::create(ArrowUpUnselectedFile, ArrowUpSelectedFile, this, menu_selector(GameScene::menuCallBackMove));
    /* �����Զ�����ѡ�ִ𰸵İ�ť */
    CCMenuItem *loadMenuItem = CCMenuItemImage::create(LoadUnselectedFile, LoadSelectedFile, this, menu_selector(GameScene::loadAnswerToRun));
	/* ��������İ�ť */
	CCMenuItem *replayMenuItem = CCMenuItemImage::create(replayUnselectedFile, replaySelectedFile, this, menu_selector(GameScene::replayCallBack));
	/* ������һ�صİ�ť */
	CCMenuItem *nextMenuItem = CCMenuItemImage::create(nextUnselectedFile, nextSelectedFile, this, menu_selector(GameScene::nextStage));

	upMenuItem->setRotation(0);
	rightMenuItem->setRotation(90);
	leftMenuItem->setRotation(-90);
	downMenuItem->setRotation(180);

	loadMenuItem->setPosition(winSize.width - 105, winSize.height/2 + 20);
	nextMenuItem->setPosition(winSize.width - 35, winSize.height/2 + 20);
	downMenuItem->setPosition(winSize.width - 70, winSize.height/2 - 120);
	upMenuItem->setPosition(winSize.width - 70, winSize.height/2 - 40);
	leftMenuItem->setPosition(winSize.width - 110, winSize.height/2 - 80);
	rightMenuItem->setPosition(winSize.width - 30, winSize.height/2 - 80);
	replayMenuItem->setPosition(winSize.width -  30, winSize.height - 40);

	downMenuItem->setTag(MoveDown * 100);
    upMenuItem->setTag(MoveUp * 100);
    leftMenuItem->setTag(MoveLeft * 100);
    rightMenuItem->setTag(MoveRight * 100);

	CCMenu *controllMenu = CCMenu::create(loadMenuItem, replayMenuItem, nextMenuItem, NULL);
	arrowMenu = CCMenu::create(downMenuItem, upMenuItem, leftMenuItem, rightMenuItem, NULL);
	controllMenu->setPosition(0,0);
	arrowMenu->setPosition(0, 0);

	addChild(controllMenu);
	addChild(arrowMenu);

    return true;
}

void GameScene::nextStage(cocos2d::CCObject *psender)
{
	if (_stage < MaxStageNumber)
	{
		CCScene *scene = GameScene::sceneWithStage(++_stage);
		CCTransitionFade *tran = CCTransitionFade::create(1.0f, scene, ccBLACK);
		CCDirector::sharedDirector()->replaceScene(tran);
	}
	else
	{
		CCScene *box = BoxMenu::scene();
		CCTransitionFade *tran = CCTransitionFade::create(1.0f, box, ccBLACK);
		CCDirector::sharedDirector()->replaceScene(tran);
	}
}

void GameScene::replayCallBack(CCObject *pSender)
{
	CCDirector::sharedDirector()->replaceScene(GameScene::sceneWithStage(_stage));
}

/* �����Ļص�������ͨ��tagֵ����ȡ�ƶ��ķ��� */
void GameScene::menuCallBackMove(CCObject *pSender)
{
    CCNode *node = (CCNode *)pSender;
    int targetDir = (node->getTag())/100;
    CCPoint moveByPosition;
    switch(targetDir)
    {
        case MoveDown:
            moveRainDrop(0, MoveDown);
            break;
        case MoveLeft:
            moveRainDrop(0, MoveLeft);
            break;
        case MoveRight:
            moveRainDrop(0, MoveRight);
            break;
        case MoveUp:
            moveRainDrop(0, MoveUp);
            break;
        default:
            break;
    }

    return;
}

/* ���鶯����ɺ�Ļص����� */
void GameScene::onWalkDone(CCNode *pSender, void *data)
{
	/* ɾ���ƶ�·���ϵ�object */
	if (iTagForOpenGate == int(data)  && iTagForOpenGate != 0)
	{
		openGateFrame = CCSpriteFrame::create(GameAtlasFile, CCRectMake(TileWidth * 10, TileHeight * 0, TileWidth, TileHeight));
		CCSprite *openGateSprite = (CCSprite *)this->getChildByTag(iTagForOpenGate);
		openGateSprite->setDisplayFrame(openGateFrame);
		return ;
	}

	if (NULL == this->getChildByTag((int)data)) 
    {
        return;
    }

	removeChildByTag((int)data, true);

    return;
}

/* һ���ƶ���� */
void GameScene::onMoveDone(void)
{
	m_bIsMovingFinished = true;

	/* �ж���Ϸ�Ƿ���������������һ�� */
	if(isGameOver())
	{
		if(_stage < 9)
		{
			_stage++;
			CCScene *scene = GameScene::sceneWithStage(_stage);
			CCTransitionFade *tran = CCTransitionFade::create(1.0f, scene, ccBLACK);
			CCDirector::sharedDirector()->replaceScene(tran);
		}
		else
		{
			CCScene *box = BoxMenu::scene();
			CCTransitionFade *tran = CCTransitionFade::create(3.0f, box, ccBLACK);
			CCDirector::sharedDirector()->replaceScene(tran);
		}
	}

    return;
}

/* ��ʵ�������ͼ����֮���ת������ͼ������32*32Ϊһ����λ */
CCPoint GameScene::toMapPos(CCPoint pos)
{
    CCPoint _pos = CCPointMake((pos.x - TileWidth / 2) / TileWidth, (pos.y - TileHeight / 2) / TileHeight);

    return _pos;
}

/* ��ͼ��������ʵ����֮���ת�� */
CCPoint GameScene::toRealPos(CCPoint pos)
{
    CCPoint _pos = CCPointMake(pos.x * TileWidth+ TileWidth / 2, (m_currentMapRow - pos.y- 1) * TileHeight + TileHeight / 2);
    
    return _pos;
}

CCPoint GameScene::toCCPoint(Point prePos)
{
	CCPoint retPoint = CCPointMake((prePos.column) * TileWidth + TileWidth / 2, (m_currentMapRow - prePos.row - 1) * TileHeight + TileHeight / 2);

	return retPoint;
}

/* Methods for game logic */
bool GameScene::isGameOver()
{
    bool result = false;

    /* �����ж�Pearls�Ƿ��Ѿ������꣬���ж�RainDrop�Ƿ��Ѿ�ȫ������ */
	if (0 == m_existedPearls)
    {
		/* ������·��� */
		int iScore = 0;
		int iDiff = m_userMoveCount - m_bestMoveCount;
		if (0 == iDiff)
		{
			iScore = 10;
		}
		else if (1 == iDiff)
		{
			iScore = 8;
		}
		else if (iDiff >= 2 && iDiff <= 5)
		{
			iScore = 7;
		}
		else if (iDiff >= 6 && iDiff <=10)
		{
			iScore = 6;
		}
		else if (iDiff >= 11 && iDiff <=30)
		{
			iScore = 5;
		}

		Score *scoreInstance = Score::GetInstance();
		TEAM_S *pstTeamScore = scoreInstance->teamScore;
		int teamTag = scoreInstance->teamTag;

		cout << "-----current tag: " << teamTag << endl;
		pstTeamScore[teamTag/1000 - 1].iScore += iScore;

		char curScoreArr[20];
		sprintf(curScoreArr, "%d", pstTeamScore[teamTag/1000 - 1].iScore);
		cout << "-----team:" << teamTag/1000 - 1;
		cout << "-----score:" << pstTeamScore[teamTag/1000 - 1].iScore;
		curScoreLabel->setString(curScoreArr);

		/* ����ͨ������ */
		SimpleAudioEngine::sharedEngine()->playEffect("../Resources/levelcomplete.mp3");

        result = true;
    }
    else if (0 == m_existedRainDrop)
    {
        result = true;
    }
    else if (m_endlessLoop)
    {
        /* ��ν�����ѭ������ҲӦ��������Ϸ */

		/* ������������ */
		SimpleAudioEngine::sharedEngine()->playEffect("../Resources/pairvanish.mp3");

        result = true;
    }

    return result;
}

void GameScene::resetGame()
{
    /* �������б��� */
    m_userMoveCount = 0;
    m_bestMoveCount = 0;
    m_currentMapRow = 0;
    m_currentMapColumn = 0;
    m_totalRainPointNumber = 0;
    m_existedRainDrop = 0;
    m_existedPearls = 0;
    m_endlessLoop = false;
	m_bIsMovingFinished = true;

    /* ��ʼ����ͼ���� */
    for (int i = 0; i < MaxMapRow; i++)
    {
        for (int j = 0; j < MaxMapColumn; j++)
        {
            m_quellReflectMap[i][j] = CellStateMin;
        }
    }

    /* ��ʼ����� */
    for (int i = 0; i < MaxRainDropNumber; i++)
    {
        m_rainDrops[i].point.row = 0;
        m_rainDrops[i].point.column = 0;
        m_rainDrops[i].state = RainDropDead;
    }

    /* ���ѡ�ִ� */
    m_answerList.clear();

    /* ȡ����ʱ�ص� */
    CCScheduler *sharedScheduler = CCDirector::sharedDirector()->getScheduler();
    sharedScheduler->unscheduleSelector(schedule_selector(GameScene::moveRainDropUsingAnswer), this);

    return;
}

void GameScene::initQuellReflectMap(const char * filename)
{
    /* ����������Ϸ */
    resetGame();

    const int BufSize = 1024;
    int temp;
    char buf[BufSize];
    fstream openStream;
    openStream.open(filename);
    if (!openStream.is_open())
    {
        cout << "���ļ�ʧ�ܣ�";
        return;
    }

    /* ��ȡ�߶� */
    openStream.getline(buf, BufSize);
    sscanf_s(buf, "height:%d", &m_currentMapRow);
    
    /* ��ȡ���� */
    openStream.getline(buf, BufSize);
    sscanf_s(buf, "width:%d", &m_currentMapColumn);

    /* ��ȡ�����ƶ����� */
    openStream.getline(buf, BufSize);
    sscanf_s(buf, "best:%d", &m_bestMoveCount);
    
    openStream.getline(buf, BufSize);
    string regExp("%d");
    for (int i = 0; i < m_currentMapRow; i++)
    {
        for (int j = 0; j < m_currentMapColumn; j++)
        {
            /* ͨ���������ʽ��ȡcsv */
            sscanf_s(buf, regExp.c_str(), &temp);
            CellState state = (CellState)temp;
            m_quellReflectMap[i][j] = state;
            regExp.insert(0, "%*d, ");

            /* �������Σ������ */
            if (CellRainDrop == state)
            {
                Point rainPoint;
                rainPoint.row = i;
                rainPoint.column = j;
                m_rainDrops[m_existedRainDrop].point = rainPoint;
                m_rainDrops[m_existedRainDrop].state = RainDropNormal;
                m_existedRainDrop++;
            }
            else if (CellNormalPearl == state || CellYellowPearl == state)
            {
                m_existedPearls++;
            }
        }
    }

    /* ��¼������� */
    m_totalRainPointNumber = m_existedRainDrop;

    /* ע�����Ҫ�ر��ļ� */
    openStream.close();

    return;
};

int GameScene::getRainDropNumberFromPosition(Point point)
{
    int result = -1;
    for (int i = 0; i < m_totalRainPointNumber; i++)
    {
        if ((m_rainDrops[i].point.row == point.row) && (m_rainDrops[i].point.column == point.column))
        {
            result = i;
        }
    }

    return result;
}

void GameScene::showQuellReflectMap()
{
    for (int i = 0; i < m_currentMapRow; i++)
    {
        for (int j = 0; j < m_currentMapColumn; j++)
        {
            char temp;
            Point point;
            point.row = i;
            point.column = j;
            switch(m_quellReflectMap[i][j])
            {
            case CellNormalWall:
                temp = 'x';
                break;
            case CellYellowWall:
                temp = 'X';
                break;
            case CellEmpty:
                temp = '_';
                break;
            case CellNormalPearl:
                temp = 'o';
                break;
            case CellPoisonDrop:
                temp = 'P';
                break;
            case CellRainDrop:
                if (0 == getRainDropNumberFromPosition(point))
                {
                    temp = 'R';
                }
                else
                {
                    temp = 'r';
                }
                break;
            case CellYellowPearl:
                temp = 'O';
                break;
            case CellOpenGate:
                temp = 'G';
                break;
            default:
                break;
            }
            cout << temp << ' ';
        }
        cout << '\n';
    }

    return;
}

Point GameScene::nextPosition(Point currentPosition, MoveDirection direction)
{
    Point point = currentPosition;

    switch(direction)
    {
    case MoveUp:
        point.row --;
        break;
    case MoveRight:
        point.column ++;
        break;
    case MoveDown:
        point.row ++;
        break;
    case MoveLeft:
        point.column --;
        break;
    default:
        break;
    }

    /* �������Ѿ�Խ�磬˵�����Դ�Խ������һ�� */
    if (m_currentMapRow == point.row)
    {
        point.row = 0;
    }
    else if (-1 == point.row)
    {
        point.row = m_currentMapRow - 1;
    }
    else if (m_currentMapColumn == point.column)
    {
        point.column = 0;
    }
    else if (-1 == point.column)
    {
        point.column = m_currentMapColumn - 1;
    }

    return point;
}

Point GameScene::previousPosition(Point currentPosition, MoveDirection direction)
{
    Point point = currentPosition;

    switch(direction)
    {
    case MoveUp:
        point.row ++;
        break;
    case MoveRight:
        point.column --;
        break;
    case MoveDown:
        point.row --;
        break;
    case MoveLeft:
        point.column ++;
        break;
    default:
        break;
    }
    /* �������Ѿ�Խ�磬˵�����Դ�Խ������һ�� */
    if (m_currentMapRow == point.row)
    {
        point.row = 0;
    }
    else if (-1 == point.row)
    {
        point.row = m_currentMapRow - 1;
    }
    else if (m_currentMapColumn == point.column)
    {
        point.column = 0;
    }
    else if (-1 == point.column)
    {
        point.column = m_currentMapColumn - 1;
    }

    return point;
}

/* �ƶ���Σ� rainDropNumber����Ҫ�ƶ�����α��(��Ϊ��ǰ��ͼ���ܲ�ֹһ�����)��direction��������ε��ƶ����� */
void GameScene::moveRainDrop(int rainDropNumber, MoveDirection direction)
{
    /* �����ǰ��ŵ�����Ѿ������������ƶ� */
    if ((rainDropNumber < 0) 
        || (rainDropNumber >= MaxRainDropNumber) 
        || (RainDropDead == m_rainDrops[rainDropNumber].state)
		|| (m_bIsMovingFinished == false))
    {
        return;
    }
    
    Point nextPos = m_rainDrops[rainDropNumber].point;
    bool keepMoving = true;
	m_bIsMovingFinished = false;

    /* ����ƶ��Ĵ��������������ƶ��������ܳ���m_currentMapColumn�����������ѭ����
    ���������ƶ��������ܳ���m_currentMapRow�����������ѭ�� */
    int moveCount = 0;

    /* ��ǰ�㲻��Ϊ���״̬ */
    m_quellReflectMap[nextPos.row][nextPos.column] = CellEmpty;

	/* �����еĶ���������һ�������� */
	CCArray *actionArray = CCArray::create();
    while (keepMoving)
    {
		/* �����μ����˶�������һ���ٽ�λ�� */
		nextPos = nextPosition(nextPos, direction);
		turnStateArray = NULL;
		switch(m_quellReflectMap[nextPos.row][nextPos.column])
		{
		case CellNormalWall:
			/* ����ǽ��ֹͣǰ����ǰһ����Ӧ��Ϊ���״̬ */
			keepMoving = false;
			nextPos = previousPosition(nextPos, direction);
			m_rainDrops[rainDropNumber].point = nextPos;
			m_quellReflectMap[nextPos.row][nextPos.column] = CellRainDrop;
			direction = MoveDirectionMin;
			break;
		case CellYellowWall:
			/* ����ǽ��ɫ��ǽ��������Ϊ��ɫ����������ͻ�ƣ����򲻿��� */
			if (RainDropYellow == m_rainDrops[rainDropNumber].state)
			{
				/* ������ɫ��ǽ֮��Ӧ�������ͨ��� */
				m_rainDrops[rainDropNumber].state = RainDropNormal;
				/* ��ɫǽ����ʧ */
				m_quellReflectMap[nextPos.row][nextPos.column] = CellEmpty;

				/* �����ɫǽ��Ӧ����ʧ�������ǰ�ƶ�һ�� */
				whitePearlFrame = CCSpriteFrame::create(GameAtlasFile, CCRectMake(TileWidth * 0, TileHeight * 0, TileWidth, TileHeight));
				turnStateArray = CCArray::create(whitePearlFrame, NULL);
			} 
			else
			{
				keepMoving = false;
				nextPos = previousPosition(nextPos, direction);
				m_rainDrops[rainDropNumber].point = nextPos;
				m_quellReflectMap[nextPos.row][nextPos.column] = CellRainDrop;
				direction = MoveDirectionMin;
			}
			break;
		case CellRainDrop:
			/* ���������Ӧ���ж�Ϊ���� */
			m_rainDrops[getRainDropNumberFromPosition(nextPos)].state = RainDropDead;
			m_existedRainDrop -= 1;
			m_quellReflectMap[nextPos.row][nextPos.column] = CellEmpty;
			break;
		case CellPoisonDrop:
			/* ��ǰ������� */
			keepMoving = false;
			m_existedRainDrop -= 1;
			m_rainDrops[rainDropNumber].point = nextPos;
			m_rainDrops[rainDropNumber].state = RainDropDead;
			m_quellReflectMap[nextPos.row][nextPos.column] = CellEmpty;
			break;
		case CellNormalPearl:
			/* ˮ������-1���ҵ�ǰ���ΪEmpty */
			m_existedPearls--;
			m_quellReflectMap[nextPos.row][nextPos.column] = CellEmpty;

			if (0 == m_existedPearls)
			{
				keepMoving = false;
			}
			break;
		case CellYellowPearl:
			/* �Ե���ɫ��Ӧ������ǰ��α�ɻ�ɫ����� */
			m_existedPearls--;
			m_rainDrops[rainDropNumber].state = RainDropYellow;
			m_quellReflectMap[nextPos.row][nextPos.column] = CellEmpty;

			if(0 == m_existedPearls)
			{
				keepMoving = false;
			}

            /* Ӧ������ε�ͼƬ��Ϊ��ɫ����� */
			yellowPearlFrame = CCSpriteFrame::create(GameAtlasFile, CCRectMake(TileWidth * 2, TileHeight * 4, TileWidth, TileHeight));
			turnStateArray = CCArray::create(yellowPearlFrame, NULL);

			break;
		case CellOpenGate:
			/* ���������Gate����ǰ���Ϊ��ͨ��ǽ */
			m_quellReflectMap[nextPos.row][nextPos.column] = CellNormalWall;

			// TODO ����ע�⣬Ӧ������ǰ���cell��ʾΪ��ʹ�ù�һ�ε�Gate	
			break;
		default:
			break;
		}//end of switch

		cout << nextPos.column << " " << nextPos.row << endl;

		/* ������Խ����� */
		CCMoveTo *turnMove;
		Point virtualPoint = nextPos;
		if(virtualPoint.column == 0 && direction == MoveRight)
		{
			virtualPoint.column = -1;
		}
		else if(virtualPoint.column == m_currentMapColumn-1 && direction == MoveLeft)
		{
			virtualPoint.column = m_currentMapColumn;
		}
		else if(virtualPoint.row == 0 && direction == MoveDown)
		{
			virtualPoint.row = -1;
			cout << "----------terrible!!" << endl;
		}
		else if(virtualPoint.row == m_currentMapRow - 1 && direction == MoveUp)
		{
			virtualPoint.row = m_currentMapRow;
		}
		if(virtualPoint.row != nextPos.row || virtualPoint.column != nextPos.column)
		{
			turnMove = CCMoveTo::create(0.0f, toCCPoint(virtualPoint));
			actionArray->addObject(turnMove);
		}

		/* ����object��tagֵ */
		int iTagForObj = (nextPos.column) * m_currentMapRow + nextPos.row;
		CCMoveTo *move = CCMoveTo::create(0.14f, toCCPoint(nextPos));
		CCCallFuncND *onWalkDoneCall =  CCCallFuncND::create(this, callfuncND_selector(GameScene::onWalkDone), (void *)iTagForObj);
		actionArray->addObject(move);
		actionArray->addObject(onWalkDoneCall);

		/* ����״̬ת�䶯�� */
		if (turnStateArray)
		{
            CCAnimation *turnStateAnimation = CCAnimation::createWithSpriteFrames(turnStateArray, 0.01f);
            CCAnimate *turnStateAnimate = CCAnimate::create(turnStateAnimation);
			actionArray->addObject(turnStateAnimate);
		}
		/* Ҫ�ж�����Ƿ��������ѭ�� */
		if (MaxMoveCount == moveCount++)
		{
			/* ���������ѭ������Ӧ��������Ϸ */
			keepMoving = false;
			m_endlessLoop = true;
		}
	}//end of while

	/* ����һ���ƶ���ϵĻص�����ʼִ�ж������� */
	CCCallFunc *onMoveDoneCall = CCCallFunc::create(this, callfunc_selector(GameScene::onMoveDone));
	actionArray->addObject(onMoveDoneCall);
	CCAction *curAction = CCSequence::create(actionArray);
	heroSprite->runAction(curAction);

    /* ȷ�ϴ˴��ƶ�Ӧ���ǺϷ��ģ���ע����ο�����ǽ���ڣ��ڳ����д˴��ƶ�Ӧ��Ҳ�����ƶ����� */
	m_userMoveCount += 1;
	char curStepArr[20];
	sprintf(curStepArr, "%d", m_userMoveCount);
	curStepLabel->setString(curStepArr);

    return;
}

int GameScene::moveCount()
{
    return m_userMoveCount;
}

void GameScene::loadAnswerToRun(CCObject *pSender)
{
    /* ��պ�����ѡ�ִ� */
    m_answerList.clear();

	arrowMenu->retain();
	removeChild(arrowMenu, false);

    HANDLE hThread;
    unsigned threadID;
    DWORD dwFlag = 0;

    // �����ڶ����߳�,��һ���߳������߳�!
    hThread = (HANDLE)_beginthreadex(NULL, 0, &solutionThread, NULL, 0, &threadID);

    // �������߳�����ʱ�䣬�����趨Ϊ5��
	DWORD   dwExitCode;
    dwFlag = WaitForSingleObject(hThread, 5000);
	//GetExitCodeThread(hThread, &dwExitCode);
	TerminateThread(hThread, dwExitCode);
	CloseHandle(hThread);

	while ((int)m_answerList.size() > (m_bestMoveCount + 30))
	{
		m_answerList.pop_back();
	}

    /* ��ʱ�ص�ѡ�ֵĴ� */
    CCScheduler *sharedScheduler = CCDirector::sharedDirector()->getScheduler();
    sharedScheduler->unscheduleSelector(schedule_selector(GameScene::moveRainDropUsingAnswer), this);
    sharedScheduler->scheduleSelector(schedule_selector(GameScene::moveRainDropUsingAnswer), this, 1.5f, false);

    return;
}

void GameScene::moveRainDropUsingAnswer(float ct)
{
    if (m_answerList.empty())
    {
        CCScheduler *sharedScheduler = CCDirector::sharedDirector()->getScheduler();
        sharedScheduler->unscheduleSelector(schedule_selector(GameScene::moveRainDropUsingAnswer), this);

		if(arrowMenu != NULL)
		{
			addChild(arrowMenu);
		}
    }
    else
    {
        /* ��ȡ��һ��Ԫ�أ���ú�����õ�һ��Ԫ�� */
        MoveDirection direction = m_answerList.front();
        m_answerList.pop_front();

        if (MoveDirectionMin < direction && direction < MoveDirectionMax)
        {
            moveRainDrop(0, direction);
        }
    }

    return;
}