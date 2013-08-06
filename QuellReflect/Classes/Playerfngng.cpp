#include "Playerfngng.h"

#include "SeedPK.h"
#include <vector>
using namespace std;
list <MoveDirection> min_result;

typedef enum
{
    RainDropStateMin = 0,

    /* 正常的雨滴 */
    RainDropNormal,

    /* 黄色的雨滴，可以通过黄色的墙壁 */
    RainDropYellow,

    /* 雨滴死亡 */
    RainDropDead,

    RainDropStateMax
}RainDropState;

typedef struct
{
    int row, column;
}Point;

typedef struct  
{
    Point point;
    int state;
}Pearl;

typedef struct
{
    /* 坐标 */
    Point point;

    /* 当前雨滴的状态 */
    RainDropState state;
}RainDrop;

typedef struct 
{
    RainDrop raindrop;
    int pearls_num;
} Rain_state; 
/* 雨滴的坐标 */
int k;
RainDrop find(CellState quell[MaxMapRow][MaxMapColumn], int row, int col, int& pnum, int& pynum, vector<Pearl> &pearls);
int Go_left(list <MoveDirection> motivate, CellState quellReflectMap2[MaxMapRow][MaxMapColumn],int rowCount, int columnCount, RainDrop rain_now, vector<Pearl> pearls, vector<Rain_state> state_list);
int Go_right(list <MoveDirection> motivate, CellState quellReflectMap2[MaxMapRow][MaxMapColumn],int rowCount, int columnCount, RainDrop rain_now, vector<Pearl> pearls, vector<Rain_state> state_list);
int Go_up(list <MoveDirection> motivate, CellState quellReflectMap2[MaxMapRow][MaxMapColumn],int rowCount, int columnCount, RainDrop rain_now, vector<Pearl> pearls, vector<Rain_state> state_list);
int Go_down(list <MoveDirection> motivate, CellState quellReflectMap2[MaxMapRow][MaxMapColumn],int rowCount, int columnCount, RainDrop rain_now, vector<Pearl> pearls, vector<Rain_state> state_list);
RainDrop find(CellState quell[MaxMapRow][MaxMapColumn], int row, int col, int& pnum, int& pynum, vector<Pearl> &pearls)
{
    pnum = 0;
    pynum = 0;
    RainDrop rainpoint;
    Pearl	pearl;
    for(int i = 0; i < row; i ++)
    {
        for(int j = 0; j < col; j++)
        {
            if(quell[i][j] == 46)
            {
                rainpoint.point.row = i;
                rainpoint.point.column = j;
                rainpoint.state = RainDropNormal;
            }
            if(quell[i][j] == 31)
            {
                pearl.point.row = i;
                pearl.point.column = j;
                pearl.state = 1;
                pnum++;
                pearls.push_back(pearl);

            }
            if(quell[i][j] == 47)
            {
                pearl.point.row = i;
                pearl.point.column = j;
                pearl.state = 2;
                pearls.push_back(pearl);
                pynum++;
            }

        }
    }
    return rainpoint;
}

int Go_left(list <MoveDirection> motivate, CellState quellReflectMap2[MaxMapRow][MaxMapColumn],int rowCount, int columnCount, RainDrop rain_now, vector<Pearl> pearls, vector<Rain_state> state_list)
{
    int i, j;
    int isover;
    int row, col;
    vector<Pearl>::iterator ite;
    CellState quellReflectMap[MaxMapRow][MaxMapColumn];
    for (i = 0; i < MaxMapRow; i++)
        for (j = 0; j < MaxMapRow; j++)
            quellReflectMap[i][j] = quellReflectMap2[i][j];
    if (pearls.size() == 0) //??
    {
        if (min_result.size() == 0)
            min_result = motivate;
        else if (motivate.size() < min_result.size())
            min_result = motivate;
        if (k-- < 0)
            return -1;
        else
            return 0;
    }
    int stage = motivate.size();
    int rainstate = rain_now.state;
    row = rain_now.point.row;
    if(rain_now.point.column - 1 < 0)
        col = columnCount - 1;
    else
        col = rain_now.point.column - 1;
    if(quellReflectMap[row][col] == CellNormalWall)
    {
        return 0;
    }
    if(quellReflectMap[row][col] == CellYellowWall && rain_now.state == RainDropNormal)
    {
        return 0;
    }
    for(col;; col--)
    {
        if(col < 0)
            col = columnCount - 1;
        if(col == rain_now.point.column)
            return 0;
        if(quellReflectMap[row][col] == CellNormalWall)
        {
            break;
        }
        else if(quellReflectMap[row][col] == CellYellowWall)
        {
            if(rain_now.state == RainDropYellow)
            {
                rain_now.state = RainDropNormal;
                quellReflectMap[row][col] = CellEmpty;
            }
            else
                break;
        }
        else if( quellReflectMap[row][col] == CellNormalPearl)
        {
            quellReflectMap[row][col] = CellEmpty;
            for(ite = pearls.begin(); ite != pearls.end(); ite++ )
            {
                if(ite->point.row == row && ite->point.column == col)
                {
                    pearls.erase(ite);
                    break;
                }
            }
        }
        else if( quellReflectMap[row][col] == CellYellowPearl)
        {
            quellReflectMap[row][col] = CellEmpty;
            for(ite = pearls.begin(); ite != pearls.end(); ite++ )
            {
                if(ite->point.row == row && ite->point.column == col)
                {
                    pearls.erase(ite);
                    break;
                }
            }
            rain_now.state = RainDropYellow;
        }
        else if( quellReflectMap[row][col] == CellOpenGate)
        {
            quellReflectMap[row][col] = CellNormalWall;
        }
    }
    col =(col + 1 == columnCount) ? 0: col+1;
    rain_now.point.row = row;
    rain_now.point.column = col;
    Rain_state rain_state;
    rain_state.raindrop = rain_now;
    rain_state.pearls_num = pearls.size();
    for (i = 0; i < state_list.size(); i++)
        if (state_list[i].raindrop.state == rain_state.raindrop.state && 
            state_list[i].raindrop.point.column == rain_state.raindrop.point.column &&
            state_list[i].raindrop.point.row == rain_state.raindrop.point.row &&
            state_list[i].pearls_num == rain_state.pearls_num)
            return 0;
    state_list.push_back(rain_state);
    motivate.push_back(MoveLeft);
    isover = Go_right(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    isover = Go_up(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    isover = Go_down(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    return 0;
}
int Go_right(list <MoveDirection> motivate, CellState quellReflectMap2[MaxMapRow][MaxMapColumn],int rowCount, int columnCount, RainDrop rain_now, vector<Pearl> pearls, vector<Rain_state>  state_list)
{
    int isover;
    int i, j ;
    int row, col;
    vector<Pearl>::iterator ite;
    CellState quellReflectMap[MaxMapRow][MaxMapColumn];
    for (i = 0; i < MaxMapRow; i++)
        for (j = 0; j < MaxMapRow; j++)
            quellReflectMap[i][j] = quellReflectMap2[i][j];
    if (pearls.size() == 0) //??
    {
        if (min_result.size() == 0)
            min_result = motivate;
        else if (motivate.size() < min_result.size())
            min_result = motivate;
        if (k-- < 0)
            return -1;
        else
            return 0;
    }
    int stage = motivate.size();
    int rainstate = rain_now.state;
    row = rain_now.point.row;
    if(rain_now.point.column + 1 == columnCount)
        col = columnCount + 1;
    else
        col = rain_now.point.column + 1;
    if(quellReflectMap[row][col] == CellNormalWall)
    {
        return 0;
    }
    if(quellReflectMap[row][col] == CellYellowWall && rain_now.state == RainDropNormal)
    {
        return 0;
    }
    for(col;; col++)
    {
        if(col == columnCount)
            col = 0;
        if(col == rain_now.point.column)
            return 0;
        if(quellReflectMap[row][col] == CellNormalWall)
        {
            break;
        }
        else if(quellReflectMap[row][col] == CellYellowWall)
        {
            if(rain_now.state == RainDropYellow)
            {
                rain_now.state = RainDropNormal;
                quellReflectMap[row][col] = CellEmpty;
            }
            else
                break;
        }
        else if( quellReflectMap[row][col] == CellNormalPearl)
        {
            quellReflectMap[row][col] = CellEmpty;
            for(ite = pearls.begin(); ite != pearls.end(); ite++ )
            {
                if(ite->point.row == row && ite->point.column == col)
                {
                    pearls.erase(ite);
                    break;
                }
            }
        }
        else if( quellReflectMap[row][col] == CellYellowPearl)
        {
            quellReflectMap[row][col] = CellEmpty;
            for(ite = pearls.begin(); ite != pearls.end(); ite++ )
            {
                if(ite->point.row == row && ite->point.column == col)
                {
                    pearls.erase(ite);
                    break;
                }
            }
            rain_now.state = RainDropYellow;
        }
        else if( quellReflectMap[row][col] == CellOpenGate)
        {
            quellReflectMap[row][col] = CellNormalWall;
        }


    }
    col =(col - 1 < 0 ) ? columnCount - 1: col - 1;
    rain_now.point.row = row;
    rain_now.point.column = col;
    Rain_state rain_state;
    rain_state.raindrop = rain_now;
    rain_state.pearls_num = pearls.size();
    for (i = 0; i < state_list.size(); i++)
        if (state_list[i].raindrop.state == rain_state.raindrop.state && 
            state_list[i].raindrop.point.column == rain_state.raindrop.point.column &&
            state_list[i].raindrop.point.row == rain_state.raindrop.point.row &&
            state_list[i].pearls_num == rain_state.pearls_num)
            return 0;
    state_list.push_back(rain_state);
    motivate.push_back(MoveRight);
    isover = Go_left(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    isover = Go_up(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    isover = Go_down(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    return 0;
}

int Go_up(list <MoveDirection> motivate, CellState quellReflectMap2[MaxMapRow][MaxMapColumn],int rowCount, int columnCount, RainDrop rain_now, vector<Pearl> pearls, vector<Rain_state>  state_list)
{
    int isover;
    int i, j ;
    int row, col;
    vector<Pearl>::iterator ite;
    CellState quellReflectMap[MaxMapRow][MaxMapColumn];
    for (i = 0; i < MaxMapRow; i++)
        for (j = 0; j < MaxMapRow; j++)
            quellReflectMap[i][j] = quellReflectMap2[i][j];
    if (pearls.size() == 0) //??
    {
        if (min_result.size() == 0)
            min_result = motivate;
        else if (motivate.size() < min_result.size())
            min_result = motivate;
        if (k-- < 0)
            return -1;
        else
            return 0;
    }
    int stage = motivate.size();
    int rainstate = rain_now.state;
    col = rain_now.point.column;
    if(rain_now.point.row - 1 < 0)
        row = rowCount - 1;
    else
        row = rain_now.point.row - 1;
    if(quellReflectMap[row][col] == CellNormalWall)
    {
        return 0;
    }
    if(quellReflectMap[row][col] == CellYellowWall && rain_now.state == RainDropNormal)
    {
        return 0;
    }
    for(row;; row--)
    {
        if(row < 0)
            row = rowCount - 1;
        if(row == rain_now.point.row)
            return 0;
        if(quellReflectMap[row][col] == CellNormalWall)
        {
            break;
        }
        else if(quellReflectMap[row][col] == CellYellowWall)
        {
            if(rain_now.state == RainDropYellow)
            {
                rain_now.state = RainDropNormal;
                quellReflectMap[row][col] = CellEmpty;
            }
            else
                break;
        }
        else if( quellReflectMap[row][col] == CellNormalPearl)
        {
            quellReflectMap[row][col] = CellEmpty;
            for(ite = pearls.begin(); ite != pearls.end(); ite++ )
            {
                if(ite->point.row == row && ite->point.column == col)
                {
                    pearls.erase(ite);
                    break;
                }
            }
        }
        else if( quellReflectMap[row][col] == CellYellowPearl)
        {
            quellReflectMap[row][col] = CellEmpty;
            for(ite = pearls.begin(); ite != pearls.end(); ite++ )
            {
                if(ite->point.row == row && ite->point.column == col)
                {
                    pearls.erase(ite);
                    break;
                }
            }
            rain_now.state = RainDropYellow;
        }
        else if( quellReflectMap[row][col] == CellOpenGate)
        {
            quellReflectMap[row][col] = CellNormalWall;
        }


    }
    row =(row + 1 == rowCount) ? 0: row+1;
    rain_now.point.row = row;
    rain_now.point.column = col;
    Rain_state rain_state;
    rain_state.raindrop = rain_now;
    rain_state.pearls_num = pearls.size();
    for (i = 0; i < state_list.size(); i++)
        if (state_list[i].raindrop.state == rain_state.raindrop.state && 
            state_list[i].raindrop.point.column == rain_state.raindrop.point.column &&
            state_list[i].raindrop.point.row == rain_state.raindrop.point.row &&
            state_list[i].pearls_num == rain_state.pearls_num)
            return 0;
    state_list.push_back(rain_state);
    motivate.push_back(MoveUp);
    isover = Go_left(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    isover = Go_right(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    isover = Go_down(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    return 0;
}


int Go_down(list <MoveDirection> motivate, CellState quellReflectMap2[MaxMapRow][MaxMapColumn],int rowCount, int columnCount, RainDrop rain_now, vector<Pearl> pearls, vector<Rain_state> state_list)
{
    int i, j ;
    int isover;
    int row, col;
    vector<Pearl>::iterator ite;
    CellState quellReflectMap[MaxMapRow][MaxMapColumn];
    for (i = 0; i < MaxMapRow; i++)
        for (j = 0; j < MaxMapRow; j++)
            quellReflectMap[i][j] = quellReflectMap2[i][j];
    if (pearls.size() == 0) //??
    {
        if (min_result.size() == 0)
            min_result = motivate;
        else if (motivate.size() < min_result.size())
            min_result = motivate;
        if (k-- < 0)
            return -1;
        else
            return 0;
    }
    int stage = motivate.size();
    int rainstate = rain_now.state;
    col = rain_now.point.column;
    if(rain_now.point.row + 1 == rowCount)
        row = rowCount + 1;
    else
        row = rain_now.point.row + 1;
    if(quellReflectMap[row][col] == CellNormalWall)
    {
        return 0;
    }
    if(quellReflectMap[row][col] == CellYellowWall && rain_now.state == RainDropNormal)
    {
        return 0;
    }
    for(row;; row++)
    {
        if(row == rowCount)
            row = 0;
        if(row == rain_now.point.row)
            return 0;
        if(quellReflectMap[row][col] == CellNormalWall)
        {
            break;
        }
        else if(quellReflectMap[row][col] == CellYellowWall)
        {
            if(rain_now.state == RainDropYellow)
            {
                rain_now.state = RainDropNormal;
                quellReflectMap[row][col] = CellEmpty;
            }
            else
                break;
        }
        else if( quellReflectMap[row][col] == CellNormalPearl)
        {
            quellReflectMap[row][col] = CellEmpty;
            for(ite = pearls.begin(); ite != pearls.end(); ite++ )
            {
                if(ite->point.row == row && ite->point.column == col)
                {
                    pearls.erase(ite);
                    break;
                }
            }
        }
        else if( quellReflectMap[row][col] == CellYellowPearl)
        {
            quellReflectMap[row][col] = CellEmpty;
            for(ite = pearls.begin(); ite != pearls.end(); ite++ )
            {
                if(ite->point.row == row && ite->point.column == col)
                {
                    pearls.erase(ite);
                    break;
                }
            }
            rain_now.state = RainDropYellow;
        }
        else if( quellReflectMap[row][col] == CellOpenGate)
        {
            quellReflectMap[row][col] = CellNormalWall;
        }


    }
    row = (row - 1 < 0 ) ? rowCount - 1: row - 1;
    rain_now.point.row = row;
    rain_now.point.column = col;
    Rain_state rain_state;
    rain_state.raindrop = rain_now;
    rain_state.pearls_num = pearls.size();
    for (i = 0; i < state_list.size(); i++)
        if (state_list[i].raindrop.state == rain_state.raindrop.state && 
            state_list[i].raindrop.point.column == rain_state.raindrop.point.column &&
            state_list[i].raindrop.point.row == rain_state.raindrop.point.row &&
            state_list[i].pearls_num == rain_state.pearls_num)
            return 0;
    state_list.push_back(rain_state);
    motivate.push_back(MoveDown);
    isover = Go_left(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    isover = Go_right(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    isover = Go_up(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);
    if (isover == -1 && state_list.size() > 5)
        return -1;
    return 0;
}

Playerfngng::Playerfngng()
{

}

Playerfngng::~Playerfngng()
{

}

std::list<MoveDirection> Playerfngng::solutionToMap( CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount )
{
    list<MoveDirection> moveList;
    list<MoveDirection> motivate;
    vector<Rain_state> state_list;
    RainDrop rain_now;
    vector<Pearl> pearls;
    vector<int> pearlsnum;
    int stage = 0;
    int perlsnumNormal;
    int perlsnumYellow;
    k = 8;
    min_result.clear();
    rain_now = find(quellReflectMap, rowCount, columnCount, perlsnumNormal, perlsnumYellow,pearls);
    pearlsnum.push_back(perlsnumNormal);
    pearlsnum.push_back(perlsnumYellow);

    int isover = Go_left(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);

    isover = Go_right(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls,state_list);

    isover = Go_up(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls, state_list);

    isover = Go_down(motivate, quellReflectMap, rowCount, columnCount, rain_now, pearls,state_list);

    moveList = min_result;
    return moveList;
}

std::string Playerfngng::teamNameToDisplay()
{
    /* 必须为英文，且小于20个字符 */
    return "fngng";
}
