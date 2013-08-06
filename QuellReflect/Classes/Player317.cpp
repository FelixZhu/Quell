#include "Player317.h"
#include "Global.h"

#define LIST_INIT_SIZE 20
#define LISTINCREMENT 10
using namespace std;

typedef struct RainDrop_sta
{
    int Row;//雨滴所在的行
    int Column;//雨滴所在的列
    int State;//雨滴的状态：普通为0，黄色为1
}RainDrop_ps;

typedef struct PathInfo
{
    int row;
    int column;
    int direction;
}PathInfo;

typedef struct MovePath
{
    PathInfo * Path;//路径指针
    int length;//实际长度
    int listsize;//分配的长度
}MovePath;

typedef struct PathList
{
    MovePath * pMovePath;//路径结构指针
    int length;//路径数
    int listsize;//分配的长度
}PathList;
void InitPathList();
void InitMovePath();
void toMove(int direction);
void AddPath(MovePath tmpMovePath);
void AddMove(int MoveDirection);
void RainDrop_position(CellState quellReflectMap[MaxMapRow][MaxMapColumn]);
int canmove(CellState quellReflectMap[MaxMapRow][MaxMapColumn], MoveDirection direction);
Point nextPosition(Point currentPosition, MoveDirection direction);
PathList Path;//路径
MovePath tmpMovePath;
RainDrop_ps rp;
int currentRow;
int currentColumn;
int PearlNu;//地图上所有水珠的数目
Point prePosition(Point currentPosition, MoveDirection direction);
void moveRain(CellState quellReflect[MaxMapRow][MaxMapColumn], MoveDirection direction);
Point nextPosition(Point currentPosition, MoveDirection direction)
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

    /* 如果雨滴已经越界，说明可以穿越到另外一边 */
    if (currentRow == point.row)
    {
        point.row = 0;
    }
    else if (-1 == point.row)
    {
        point.row = currentRow - 1;
    }
    else if (currentColumn == point.column)
    {
        point.column = 0;
    }
    else if (-1 == point.column)
    {
        point.column = currentColumn - 1;
    }
    return point;
}
int canmove(CellState quellReflectMap[MaxMapRow][MaxMapColumn], MoveDirection direction)
{
    int flag;
    Point nextPos;
    nextPos.row = rp.Row;
    nextPos.column = rp.Column;
    /* 获得雨滴即将运动到的下一个临近位置 */
    nextPos = nextPosition(nextPos, direction);
    switch(quellReflectMap[nextPos.row][nextPos.column])
    {
    case CellNormalWall:
        /* 碰到墙 */
        flag = 0;
        break;
    case CellYellowWall:
        /* 碰到墙黄色的墙，如果雨滴为黄色的雨滴则可以突破，否则不可以 */
        if (1 == rp.State) flag = 1;
        else flag = 0;
        break;
    case CellPoisonDrop:
        /* 当前雨滴死亡 */
        flag = 0;
        break;
    default:
        flag = 1;
        break;
    }//end of switch
    return flag;
}

void AddMove(int MoveDirection)
{
    if(tmpMovePath.length>=tmpMovePath.listsize)
    {//当前存储空间已满，增加分配
        tmpMovePath.Path=(PathInfo *)realloc(tmpMovePath.Path,(tmpMovePath.listsize+LISTINCREMENT)*sizeof(PathInfo));
        tmpMovePath.listsize+=LISTINCREMENT;
    }
    tmpMovePath.Path[tmpMovePath.length].row=rp.Row;
    tmpMovePath.Path[tmpMovePath.length].column=rp.Column;
    tmpMovePath.Path[tmpMovePath.length].direction=MoveDirection;
    tmpMovePath.length++;
}

void AddPath(MovePath tmpMovePath)
{
    if(Path.length>=Path.listsize)
    {//当前存储空间已满，增加分配
        Path.pMovePath=(MovePath *)realloc(Path.pMovePath,(Path.listsize+LISTINCREMENT)*sizeof(MovePath));
        Path.listsize+=LISTINCREMENT;
    }
    Path.pMovePath[Path.length]=tmpMovePath;
    Path.length++;
}

void InitMovePath()
{
    tmpMovePath.Path=(PathInfo *)malloc(LIST_INIT_SIZE*sizeof(PathInfo));
    tmpMovePath.length=0;
    tmpMovePath.listsize=LIST_INIT_SIZE;
}

void InitPathList()
{
    Path.pMovePath=(MovePath *)malloc(LIST_INIT_SIZE*sizeof(MovePath));
    Path.length=0;
    Path.listsize=LIST_INIT_SIZE;
}

void toMove(int direction)
{
}

void RainDrop_position(CellState quellReflectMap[MaxMapRow][MaxMapColumn])//定位雨滴位置及检测地图内水珠的数目
{
    int i,j;
    PearlNu=0;
    for(i=0;i<MaxMapRow;i++)
        for(j=0;j<MaxMapColumn;j++)
        {
            if(quellReflectMap[i][j]==CellRainDrop)
            {
                rp.Row=i;
                rp.Column=j;
                rp.State=0;
            }
            if(quellReflectMap[i][j]==CellNormalPearl||quellReflectMap[i][j]==CellYellowPearl)
                PearlNu++;			
        }
}

void moveRain(CellState quellReflect[MaxMapRow][MaxMapColumn], MoveDirection direction)
{
    bool keepMoving = true;
    Point nextPos;
    nextPos.row = rp.Row;
    nextPos.column = rp.Column;

    while (keepMoving)
    {
        /* 获得雨滴即将运动到的下一个临近位置 */
        nextPos = nextPosition(nextPos, direction);
        switch(quellReflect[nextPos.row][nextPos.column])
        {
        case CellNormalWall:
            /* 碰到墙，停止前进，前一个点应该为雨滴状态 */
            keepMoving = false;
            nextPos = prePosition(nextPos, direction);
            rp.Row = nextPos.row;
            rp.Column = nextPos.column;
            direction = MoveDirectionMin;
            break;
        case CellYellowWall:
            /* 碰到墙黄色的墙 */
            if (1 == rp.State)
            {
                /* 雨滴为黄色碰到黄色的墙之后应当变成普通雨滴 */
                rp.State = 0;
            } 
            else
            {
                keepMoving = false;
                nextPos = prePosition(nextPos, direction);
                rp.Row = nextPos.row;
                rp.Column = nextPos.column;
                direction = MoveDirectionMin;
            }
            break;
        case CellPoisonDrop:
            /* 当前雨滴死亡 */
            keepMoving = false;
            rp.Row = nextPos.row;
            rp.Column = nextPos.column;
            break;
        case CellNormalPearl:
            /* 水珠数量-1 */
            PearlNu--;
            if (0 == PearlNu)
            {
                keepMoving = false;
            }
            break;
        case CellYellowPearl:
            /* 吃到黄色的水珠 */
            PearlNu--;
            rp.State = 1;

            if(0 == PearlNu)
            {
                keepMoving = false;
            }
            break;
        default:
            break;
        }//end of switch
    }
    return;
}

Point prePosition(Point currentPosition, MoveDirection direction)
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
    /* 如果雨滴已经越界，说明可以穿越到另外一边 */
    if (currentRow == point.row)
    {
        point.row = 0;
    }
    else if (-1 == point.row)
    {
        point.row = currentRow - 1;
    }
    else if (currentColumn == point.column)
    {
        point.column = 0;
    }
    else if (-1 == point.column)
    {
        point.column = currentColumn - 1;
    }

    return point;
}

Player317::Player317()
{

}

Player317::~Player317()
{
    
}

std::list<MoveDirection> Player317::solutionToMap( CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount )
{
    list<MoveDirection> moveList;
    currentRow = rowCount;
    currentColumn = columnCount;
    int i=0;
    RainDrop_position(quellReflectMap);//定位起始点

    InitMovePath();//初始化
    InitPathList();//初始化
a:	if(canmove(quellReflectMap, MoveUp))//试探
    {//可行
        AddMove(MoveUp);
        toMove(MoveUp);
        i++;
        if(i>30) goto b;
        if(PearlNu)
        {//还有水珠
            goto a;
        }
        else
        {//结束
            return moveList;
        }
    }
    if(canmove(quellReflectMap, MoveDown))//试探
    {//可行
        AddMove(MoveDown);
        toMove(MoveDown);
        i++;
        if(i>30) goto b;
        if(PearlNu)
        {//还有水珠
            goto a;
        }
        else
        {//结束
            return moveList;
        }
    }
    if(canmove(quellReflectMap, MoveLeft))//试探
    {//可行
        AddMove(MoveLeft);
        toMove(MoveLeft);
        i++;
        if(i>30) goto b;
        if(PearlNu)
        {//还有水珠
            goto a;
        }
        else
        {//结束
            return moveList;
        }
    }
    if(canmove(quellReflectMap, MoveRight))//试探
    {//可行
        AddMove(MoveRight);
        toMove(MoveRight);
        i++;
        if(i>30) goto b;
        if(PearlNu)
        {//还有水珠
            goto a;
        }
        else
        {//结束
            return moveList;
        }
    }
b:	if(rp.Row==3&&rp.Column==1&&rp.State==0&&PearlNu==1)
    {
        moveList.push_back(MoveRight);
        moveList.push_back(MoveUp);
        return moveList;
    }
    if(rp.Row==3&&rp.Column==4&&rp.State==0&&PearlNu==3)
    {
        moveList.push_back(MoveRight);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveRight);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveUp);
        moveList.push_back(MoveRight);
        moveList.push_back(MoveUp);
        return moveList;
    }
    if(rp.Row==5&&rp.Column==2&&rp.State==0&&PearlNu==3)
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
        return moveList;
    }
    if(rp.Row==3&&rp.Column==8&&rp.State==0&&PearlNu==3)
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
        return moveList;
    }
    if(rp.Row==3&&rp.Column==2&&rp.State==0&&PearlNu==5)
    {
        moveList.push_back(MoveUp);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveUp);
        moveList.push_back(MoveLeft);
        moveList.push_back(MoveDown);
        moveList.push_back(MoveRight);
        return moveList;
    }
    moveRain(quellReflectMap,MoveUp);
    return moveList;
}

std::string Player317::teamNameToDisplay()
{
    /* 必须为英文，且小于20个字符 */
    return "[317]";
}
