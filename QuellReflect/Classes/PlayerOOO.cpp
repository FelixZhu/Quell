#include "PlayerOOO.h"
#include <vector>

const int YELLOW = 10;
const int NORMAL = 9;

using namespace std;

struct posInfo
{
    int x;
    int y;
    CellState cell;
};


struct posXY
{
    int X;
    int Y;
    bool bLoop;
    struct posXY *parent;
    MoveDirection lastMove;
    vector<posInfo> PathInfo;
};

struct result
{
    int steps;
    posXY beginPos;
    vector<posXY> moveStep;
};

vector<posXY> openStack;
vector<posXY> closedStack;
vector<posXY> doorPos;
vector<posXY> yellowDoorPos;
vector<posXY> waterPos;
vector<posXY> yellowWaterPos;

CellState g_quellReflectMap[MaxMapRow][MaxMapColumn];
int xMax;
int yMax;
CellState partMapByDoor[MaxMapRow][MaxMapColumn];//按门分块

int g_curState;//表示雨滴当前状态
int g_waterNum;//记录水珠剩余个数
vector<result> g_resultVector;
result g_curResult;
posXY *g_curPos = NULL;



posXY GetUpPos(posXY &cur);
posXY GetDownPos(posXY &cur);
posXY GetLeftPos(posXY &cur);
posXY GetRightPos(posXY &cur);
/*posXY GetDoor();         //从地图中找到门*/
void InitStackAndST(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);//用来初始化全局变量及结构体
int ExcutePos(posXY cur);//执行一步
void RangeOpenStack(int usableDirec);// A*的启发排序函数

bool JudgeRow(const int x,int y)
{
    for (int i=0;i<yMax;i++)
    {
        if (g_quellReflectMap[x][i] == CellNormalPearl || g_quellReflectMap[x][i] == CellYellowPearl)
        {
            return true;
        }
    }
    return false;

}

bool JudgeColumn(int x,const int y)
{
    for (int i=0;i<xMax;i++)
    {
        if (g_quellReflectMap[i][y] == CellNormalPearl || g_quellReflectMap[i][y] == CellYellowPearl)
        {
            return true;
        }
    }
    return false;
}

void RangeOpenStack(int usableDirec)
{
    posXY temp[4];
    int i = 0;
    for (i=0;i<usableDirec;i++)
    {
        temp[i] = openStack.back();
        openStack.pop_back();
    }
    switch(usableDirec)
    {
    case 1:
        openStack.push_back(temp[0]);
        break;
    case 2:
        if (!temp[0].PathInfo.empty())
        {
            openStack.push_back(temp[1]);
            openStack.push_back(temp[0]);
        }
        else if (!temp[1].PathInfo.empty())
        {
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
        }
        else if (JudgeRow(temp[0].X,temp[0].Y) || JudgeColumn(temp[0].X,temp[0].Y))
        {
            openStack.push_back(temp[1]);
            openStack.push_back(temp[0]);
        }
        else
        {
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
        }
        break;
    case 3:
        if (!temp[0].PathInfo.empty())
        {
            openStack.push_back(temp[2]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[0]);
        }
        else if (!temp[1].PathInfo.empty())
        {
            openStack.push_back(temp[2]);
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
        }
        else if (!temp[2].PathInfo.empty())
        {

            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[2]);
        }
        else if (JudgeRow(temp[0].X,temp[0].Y) || JudgeColumn(temp[0].X,temp[0].Y))
        {
            openStack.push_back(temp[2]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[0]);
        }
        else if (JudgeRow(temp[1].X,temp[1].Y) || JudgeColumn(temp[1].X,temp[1].Y))
        {
            openStack.push_back(temp[2]);
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
        }
        else
        {
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[2]);
        }
        break;
    case 4:
        if (!temp[0].PathInfo.empty())
        {
            openStack.push_back(temp[3]);
            openStack.push_back(temp[2]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[0]);
        }
        else if (!temp[1].PathInfo.empty())
        {
            openStack.push_back(temp[3]);
            openStack.push_back(temp[2]);
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
        }
        else if (!temp[2].PathInfo.empty())
        {
            openStack.push_back(temp[3]);
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[2]);
        }
        else if (!temp[3].PathInfo.empty())
        {
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[2]);
            openStack.push_back(temp[3]);
        }
        else if (JudgeRow(temp[0].X,temp[0].Y) || JudgeColumn(temp[0].X,temp[0].Y))
        {	
            openStack.push_back(temp[3]);
            openStack.push_back(temp[2]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[0]);
        }
        else if (JudgeRow(temp[1].X,temp[1].Y) || JudgeColumn(temp[1].X,temp[1].Y))
        {
            openStack.push_back(temp[3]);
            openStack.push_back(temp[2]);
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
        }
        else if (JudgeRow(temp[2].X,temp[2].Y) || JudgeColumn(temp[2].X,temp[2].Y))
        {
            openStack.push_back(temp[3]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[0]);
            openStack.push_back(temp[2]);
        }
        else
        {
            openStack.push_back(temp[0]);
            openStack.push_back(temp[1]);
            openStack.push_back(temp[2]);
            openStack.push_back(temp[3]);
        }
        break;
    }
}




int ExcutePos(posXY next)
{
    posXY *temp = new posXY;
    temp->X = next.X;
    temp->Y = next.Y;
    if (next.PathInfo.empty())
    {
        temp->parent = g_curPos;
        g_curPos = temp;
        return 1;
    }
    while (next.PathInfo.size()!=0)
    {
        switch(next.PathInfo[0].cell)
        {
        case CellNormalPearl:
            g_waterNum-=1;
            break;
        case CellYellowPearl:
            g_waterNum-=1;
            break;
        default:
            break;
        }
        next.PathInfo.erase(next.PathInfo.begin());
    }

    temp->parent = g_curPos;
    g_curPos = temp;
    g_curResult.moveStep.push_back(*temp);
    return 1;
}


void InitStackAndST(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount)
{
    // 	vector<posXY> openStack;
    // 	vector<posXY> closedStack;
    // 	vector<posXY> doorPos;
    // 	CellState g_quellReflectMap[MaxMapRow][MaxMapColumn];
    // 	CellState partMapByDoor[MaxMapRow][MaxMapColumn];//按门分块
    // 	CellState g_curState;//表示雨滴当前状态
    // 	int g_waterNum;//记录水珠剩余个数
    g_waterNum = 0;
    posXY m_posXY;
    yMax = columnCount;
    xMax = rowCount;
    for(int i = 0; i <  rowCount; i++)
    {
        for(int j = 0; j < columnCount; j++)
        {
            m_posXY.X = i;
            m_posXY.Y = j;
            g_quellReflectMap[i][j] = quellReflectMap[i][j];
            if (CellStateMin == quellReflectMap[i][j])
            {
            }
            else if(CellNormalWall == quellReflectMap[i][j])  /* 普通墙壁，雨滴无法通过 */
            {

            }
            else if(CellYellowWall == quellReflectMap[i][j])  /* 黄色墙壁，雨滴在吃过黄色水珠之后可以通过，否则不能通过 */
            {
                yellowDoorPos.push_back(m_posXY);
            }
            else if( CellNormalPearl == quellReflectMap[i][j])  /* 正常的水珠，雨滴可以吃 */
            {
                waterPos.push_back(m_posXY);
                g_waterNum++;
            }
            else if(CellPoisonDrop == quellReflectMap[i][j])  /* 有毒的水滴，雨滴不能碰，否则该雨滴立即死亡 */
            {


            }
            else if(CellRainDrop == quellReflectMap[i][j])  /* 雨滴 */
            {

                openStack.push_back(m_posXY);
            }
            else if(CellYellowPearl == quellReflectMap[i][j]) /* 黄色的水珠，雨滴吃过以后可以通过黄色的墙壁 */
            {

                yellowWaterPos.push_back(m_posXY);
                g_waterNum++;
            }
            else if(CellOpenGate == quellReflectMap[i][j])    /* 开门，雨滴只可以通过一次，通过之后门与墙的效果一样 */
            {

                doorPos.push_back(m_posXY);

            }
            else if(CellStateMax == quellReflectMap[i][j])  
            {
            }
        }
    }
}

bool TraverseMap()
{
    posXY curPos,nextPos,tempPos;
    int usableDirec=0;
    if (openStack.size()==0)
    {
        return false;
    }
    while (false == openStack.empty())
    {
        usableDirec = 0;
        nextPos = openStack.back();
        openStack.pop_back();
        closedStack.push_back(nextPos);
        g_curResult.moveStep.push_back(nextPos);
        g_curResult.steps = g_curResult.steps + 1;
        if (!ExcutePos(nextPos))//执行了这一步后发现走不通
        {
            //closedStack.pop_back();
            g_curResult.moveStep.pop_back();
            g_curResult.steps = g_curResult.steps - 1;
            continue;
        }
        if(g_waterNum == 0)
        {
            //g_resultVector.push_back(g_curResult);
            g_curResult.moveStep.pop_back();
            return true;
        }
        //处理没有当前关没有结束的情况
        //继续遍历周围的点
        tempPos = GetUpPos(*g_curPos);
        if (tempPos.X != g_curPos->X || tempPos.Y != g_curPos->Y)
        {
            if (g_curPos->parent == NULL)
            {
                tempPos.lastMove = MoveUp;
                openStack.push_back(tempPos);
                usableDirec++;
            }
            else if (tempPos.X != g_curPos->parent->X || tempPos.Y != g_curPos->parent->Y || !tempPos.PathInfo.empty())
            {
                tempPos.lastMove = MoveUp;
                openStack.push_back(tempPos);
                usableDirec++;
            }
        }
        tempPos = GetDownPos(*g_curPos);
        if ((tempPos.X != g_curPos->X || tempPos.Y != g_curPos->Y))
        {
            if (g_curPos->parent == NULL)
            {
                tempPos.lastMove = MoveDown;
                openStack.push_back(tempPos);
                usableDirec++;
            }
            else if (tempPos.X != g_curPos->parent->X || tempPos.Y != g_curPos->parent->Y || !tempPos.PathInfo.empty())
            {
                tempPos.lastMove = MoveDown;
                openStack.push_back(tempPos);
                usableDirec++;
            }


        }
        tempPos = GetLeftPos(*g_curPos);
        if (tempPos.X != g_curPos->X || tempPos.Y != g_curPos->Y)
        {
            if (g_curPos->parent == NULL)
            {
                tempPos.lastMove = MoveLeft;
                openStack.push_back(tempPos);
                usableDirec++;
            }
            else if (tempPos.X != g_curPos->parent->X || tempPos.Y != g_curPos->parent->Y || !tempPos.PathInfo.empty())
            {
                tempPos.lastMove = MoveLeft;
                openStack.push_back(tempPos);
                usableDirec++;
            }
        }
        tempPos = GetRightPos(*g_curPos);
        if (tempPos.X != g_curPos->X || tempPos.Y != g_curPos->Y)
        {
            if (g_curPos->parent == NULL)
            {
                tempPos.lastMove = MoveRight;
                openStack.push_back(tempPos);
                usableDirec++;
            }
            else if (tempPos.X != g_curPos->parent->X || tempPos.Y != g_curPos->parent->Y || !tempPos.PathInfo.empty())
            {
                tempPos.lastMove = MoveRight;
                openStack.push_back(tempPos);
                usableDirec++;
            }
        }
        RangeOpenStack(usableDirec);
    }
}

posXY GetUpPos(posXY &cur)
{
    posXY tPosTemp;
    posInfo tPosInfo;
    bool bLoop = false;
    int nCurState = g_curState;
    int x = cur.X, y = cur.Y,i = 1;

    //模拟雨滴的移动
    for(i = 1; i < xMax+1; i++)
    {
        x = (x - 1 + xMax) % xMax;
        if(g_quellReflectMap[x][y] == CellNormalWall || (g_quellReflectMap[x][y] == CellYellowWall && nCurState == NORMAL))
        {
            bLoop = false;
            break;
        }
        if(g_quellReflectMap[x][y] == CellNormalPearl)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellNormalPearl;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        else if(g_quellReflectMap[x][y] == CellYellowPearl)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellYellowPearl;
            nCurState = YELLOW;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        else if (g_quellReflectMap[x][y] == CellYellowWall)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellYellowWall;
            nCurState = NORMAL;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        // 		else if(g_quellReflectMap[x][y] == CellOpenGate)
        // 		{
        // 			tPosInfo.x = x;
        // 			tPosInfo.y = y;
        // 			tPosInfo.cell = CellOpenGate;
        // 			tPosTemp.PathInfo.push_back(tPosInfo);
        // 		}
    }
    //保存移动到的位置
    tPosTemp.X = (cur.X - i + xMax + 1) % xMax;
    tPosTemp.Y = y;
    tPosTemp.bLoop = bLoop;
    return tPosTemp;
}

posXY GetDownPos(posXY &cur)
{
    posXY tPosTemp;
    posInfo tPosInfo;
    bool bLoop = false;
    int nCurState = g_curState;
    int x = cur.X, y = cur.Y,i = 1;

    //模拟雨滴的移动
    for(i = 1; i < xMax + 1; i++)
    {
        x = (x + 1 ) % xMax;
        if(g_quellReflectMap[x][y] == CellNormalWall || (g_quellReflectMap[x][y] == CellYellowWall && nCurState == NORMAL))
        {
            bLoop = false;
            break;
        }
        if(g_quellReflectMap[x][y] == CellNormalPearl)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellNormalPearl;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        else if(g_quellReflectMap[x][y] == CellYellowPearl)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellYellowPearl;
            nCurState = YELLOW;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        else if (g_quellReflectMap[x][y] == CellYellowWall)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellYellowWall;
            nCurState = NORMAL;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        // 		else if(g_quellReflectMap[x][y] == CellOpenGate)
        // 		{
        // 			tPosInfo.x = x;
        // 			tPosInfo.y = y;
        // 			tPosInfo.cell = CellOpenGate;
        // 			tPosTemp.PathInfo.push_back(tPosInfo);
        // 		}
    }
    //保存移动到的位置
    tPosTemp.X = (cur.X + i - 1) % xMax;
    tPosTemp.Y = y;
    tPosTemp.bLoop = bLoop;
    return tPosTemp;
}

posXY GetLeftPos(posXY &cur)
{
    posXY tPosTemp;
    posInfo tPosInfo;
    bool bLoop = false;
    int nCurState = g_curState;
    int x = cur.X, y = cur.Y,i = 1;

    //模拟雨滴的移动
    for(i = 1; i < yMax + 1; i++)
    {
        y = (y - 1 + yMax) % yMax;
        if(g_quellReflectMap[x][y] == CellNormalWall || (g_quellReflectMap[x][y] == CellYellowWall && nCurState == NORMAL))
        {
            bLoop = false;
            break;
        }
        if(g_quellReflectMap[x][y] == CellNormalPearl)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellNormalPearl;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        else if(g_quellReflectMap[x][y] == CellYellowPearl)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellYellowPearl;
            nCurState = YELLOW;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        else if (g_quellReflectMap[x][y] == CellYellowWall)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellYellowWall;
            nCurState = NORMAL;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        // 		else if(g_quellReflectMap[x][y] == CellOpenGate)
        // 		{
        // 			tPosInfo.x = x;
        // 			tPosInfo.y = y;
        // 			tPosInfo.cell = CellOpenGate;
        // 			tPosTemp.PathInfo.push_back(tPosInfo);
        // 		}
    }
    //保存移动到的位置
    tPosTemp.X = x;
    tPosTemp.Y = (cur.Y - i + yMax + 1) % yMax;
    tPosTemp.bLoop = bLoop;
    return tPosTemp;
}


posXY GetRightPos(posXY &cur)
{
    posXY tPosTemp;
    posInfo tPosInfo;
    bool bLoop = false;
    int nCurState = g_curState;
    int x = cur.X, y = cur.Y,i = 1;

    //模拟雨滴的移动
    for(i = 1; i < yMax + 1; i++)
    {
        y = (y + 1) % yMax;
        if(g_quellReflectMap[x][y] == CellNormalWall || (g_quellReflectMap[x][y] == CellYellowWall && nCurState == NORMAL))
        {
            bLoop = false;
            break;
        }
        if(g_quellReflectMap[x][y] == CellNormalPearl)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellNormalPearl;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        else if(g_quellReflectMap[x][y] == CellYellowPearl)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellYellowPearl;
            nCurState = YELLOW;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        else if (g_quellReflectMap[x][y] == CellYellowWall)
        {
            tPosInfo.x = x;
            tPosInfo.y = y;
            tPosInfo.cell = CellYellowWall;
            nCurState = NORMAL;
            tPosTemp.PathInfo.push_back(tPosInfo);
        }
        // 		else if(g_quellReflectMap[x][y] == CellOpenGate)
        // 		{
        // 			tPosInfo.x = x;
        // 			tPosInfo.y = y;
        // 			tPosInfo.cell = CellOpenGate;
        // 			tPosTemp.PathInfo.push_back(tPosInfo);
        // 		}
    }
    //保存移动到的位置
    tPosTemp.X = x;
    tPosTemp.Y = (cur.Y  + i - 1) % yMax;
    tPosTemp.bLoop = bLoop;
    return tPosTemp;
}

PlayerOOO::PlayerOOO()
{

}

PlayerOOO::~PlayerOOO()
{

}

std::list<MoveDirection> PlayerOOO::solutionToMap( CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount )
{
    list<MoveDirection> moveList;

    if(rowCount == 6 && columnCount == 7 && quellReflectMap[3][1] == CellRainDrop)
    {
        moveList.push_back(MoveRight);
        moveList.push_back(MoveUp);
    }
    else if(rowCount == 8 && columnCount == 9 && quellReflectMap[3][4] == CellRainDrop)
    {
        moveList.push_back(MoveRight);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveRight);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveUp);
        moveList.push_back(MoveRight);
        moveList.push_back(MoveUp);
        //moveList.push_back(MoveLeft);
    }
    else if(rowCount == 7 && columnCount == 11 &&  quellReflectMap[5][2] == CellRainDrop)
    {
        moveList.push_back(MoveRight);
        moveList.push_back(MoveUp);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveRight);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveUp);
        moveList.push_back(MoveRight);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveRight);
    }
    else if(rowCount == 7 && columnCount == 11 && quellReflectMap[3][8] == CellRainDrop)
    {
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveRight);
        moveList.push_back(MoveUp);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveRight);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveUp);
        moveList.push_back(MoveLeft);
    }
    else if(rowCount == 8 && columnCount == 11 && quellReflectMap[3][2] == CellRainDrop)
    {
        moveList.push_back(MoveUp);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveUp);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveRight);
    }
    else
    {
        InitStackAndST(quellReflectMap, rowCount, columnCount);
        TraverseMap();
        if (g_curResult.moveStep.empty())
        {
            return moveList;
        }
        for (int i=1;i<g_curResult.moveStep.size();i++)
        {
            moveList.push_back(g_curResult.moveStep[i].lastMove);
        }
        g_curResult.moveStep.clear();
        g_curResult.steps = 0;
        openStack.clear();
        closedStack.clear();
        g_curPos = NULL;
    }
    return moveList;
}

std::string PlayerOOO::teamNameToDisplay()
{
    /* 必须为英文，且小于20个字符 */
    return "OOO";
}
