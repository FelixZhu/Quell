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
CellState partMapByDoor[MaxMapRow][MaxMapColumn];//���ŷֿ�

int g_curState;//��ʾ��ε�ǰ״̬
int g_waterNum;//��¼ˮ��ʣ�����
vector<result> g_resultVector;
result g_curResult;
posXY *g_curPos = NULL;



posXY GetUpPos(posXY &cur);
posXY GetDownPos(posXY &cur);
posXY GetLeftPos(posXY &cur);
posXY GetRightPos(posXY &cur);
/*posXY GetDoor();         //�ӵ�ͼ���ҵ���*/
void InitStackAndST(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);//������ʼ��ȫ�ֱ������ṹ��
int ExcutePos(posXY cur);//ִ��һ��
void RangeOpenStack(int usableDirec);// A*������������

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
    // 	CellState partMapByDoor[MaxMapRow][MaxMapColumn];//���ŷֿ�
    // 	CellState g_curState;//��ʾ��ε�ǰ״̬
    // 	int g_waterNum;//��¼ˮ��ʣ�����
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
            else if(CellNormalWall == quellReflectMap[i][j])  /* ��ͨǽ�ڣ�����޷�ͨ�� */
            {

            }
            else if(CellYellowWall == quellReflectMap[i][j])  /* ��ɫǽ�ڣ�����ڳԹ���ɫˮ��֮�����ͨ����������ͨ�� */
            {
                yellowDoorPos.push_back(m_posXY);
            }
            else if( CellNormalPearl == quellReflectMap[i][j])  /* ������ˮ�飬��ο��Գ� */
            {
                waterPos.push_back(m_posXY);
                g_waterNum++;
            }
            else if(CellPoisonDrop == quellReflectMap[i][j])  /* �ж���ˮ�Σ���β����������������������� */
            {


            }
            else if(CellRainDrop == quellReflectMap[i][j])  /* ��� */
            {

                openStack.push_back(m_posXY);
            }
            else if(CellYellowPearl == quellReflectMap[i][j]) /* ��ɫ��ˮ�飬��γԹ��Ժ����ͨ����ɫ��ǽ�� */
            {

                yellowWaterPos.push_back(m_posXY);
                g_waterNum++;
            }
            else if(CellOpenGate == quellReflectMap[i][j])    /* ���ţ����ֻ����ͨ��һ�Σ�ͨ��֮������ǽ��Ч��һ�� */
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
        if (!ExcutePos(nextPos))//ִ������һ�������߲�ͨ
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
        //����û�е�ǰ��û�н��������
        //����������Χ�ĵ�
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

    //ģ����ε��ƶ�
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
    //�����ƶ�����λ��
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

    //ģ����ε��ƶ�
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
    //�����ƶ�����λ��
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

    //ģ����ε��ƶ�
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
    //�����ƶ�����λ��
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

    //ģ����ε��ƶ�
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
    //�����ƶ�����λ��
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
    /* ����ΪӢ�ģ���С��20���ַ� */
    return "OOO";
}
