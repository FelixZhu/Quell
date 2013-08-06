#include "PlayerBird.h"

#include "SeedPK.h"

using namespace std;

list<MoveDirection> My_cur_moveList;
list<MoveDirection> My_save_moveList;
CellState savemap[MaxMapRow][MaxMapColumn];
CellState curquellReflectMap[MaxMapRow][MaxMapColumn];
CellState stateList[MaxMapRow][MaxMapColumn][250000];

int dropstate[50];
int row_max, column_max;
int findflag;
int top;

void init()
{
    int i,j,k;

    CellState fiveList1[MaxMapRow][MaxMapColumn];
    CellState fiveList2[MaxMapRow][MaxMapColumn];
    CellState fiveList3[MaxMapRow][MaxMapColumn];
    CellState fiveList4[MaxMapRow][MaxMapColumn];
    CellState fiveList5[MaxMapRow][MaxMapColumn];
    //fiveList1[][] = {{},{},{},{},{},{}};
}

void addstate(CellState cur[MaxMapRow][MaxMapColumn], int row, int column, int state)
{
    int i,j;
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < column; j++)
        {
            stateList[i][j][top] = cur[i][j];
        }
    }
    dropstate[top] = state;
    top++;
}
void removestate()
{
    if(top > 0)top--;
}
int checkstate(CellState cur[MaxMapRow][MaxMapColumn], int row, int column,int state)
{
    int i, j, k;
    int flag = 0;
    for(k =0; k < top; k ++)
    {
        flag = 0;
        for(i = 0; i < row; i++)
        {
            for(j = 0; j < column; j++)
            {
                if(stateList[i][j][k] != cur[i][j])flag = 1;
            }
        }
        if(state != dropstate[k])flag = 1;
        if(!flag)return 0;
    }
    return 1;
}
void recover()
{
    int i,j;
    /*恢复部分*/
    for(i = 0; i < row_max; i++)
    {
        for(j = 0; j < column_max; j++)
        {
            /*保存矩阵*/
            curquellReflectMap[i][j] = savemap[i][j];
        }
    }
}
/*state为雨滴状态，0正常，1黄色雨滴*/
void dfs(int lv, int state)
{
    if(findflag == 1)return;
    /*搜索加减枝*/
    int i,j, flag = 0;
    int temp;
    int gateflag = 0;
    int changeflag = 0;
    int curRow, curColumn;
    /*剪枝1：不超过30步*/
    if(lv >= 3)return;
    /*如果所有水珠都吃完了*/
    for(i = 0; i < row_max; i++)
    {
        for(j = 0; j < column_max; j++)
        {
            /*保存矩阵*/
            savemap[i][j] = curquellReflectMap[i][j];
            if(curquellReflectMap[i][j] == CellNormalPearl || curquellReflectMap[i][j] == CellYellowPearl)
                flag = 1;
            /*记录雨位置*/
            if(curquellReflectMap[i][j] == CellRainDrop)
            {
                curRow = i;
                curColumn = j;
            }

        }
    }
    /*剪枝1：状态重复*/

    /*剪枝1：不超过30步*/
    /*剪枝1：不超过30步*/
    /*全部吃完了,保存*/
    if(flag == 0)
    {
        if(My_save_moveList.empty())
        {
            while(!My_cur_moveList.empty())
            {
                MoveDirection temp1 = My_cur_moveList.front();
                My_save_moveList.push_back(temp1);
                My_cur_moveList.pop_front();
            }

        }else
        {

        }
        findflag = 1;
        return;
    }

    /*移动部分MoveUp*/
    changeflag = 0;
    gateflag  = 0;

    My_cur_moveList.push_back(MoveUp);
    for(i = curRow-1; (i != curRow || gateflag == 1) ; i--)
    {
        i = (i + row_max)%row_max;
        /*如果是墙壁，则停留在这里，包含黄色墙壁*/
        if(curquellReflectMap[i][curColumn] == CellNormalWall || (curquellReflectMap[i][curColumn] == CellYellowWall && state == 0))
        {
            temp = (i+1+row_max)%row_max;
            /*如果水还在原地并且没有引起变化，或者回到了父亲节点并且未引起变化，则丢弃这一分支,否则深入搜索*/
            {
                /*雨滴移动到当前位置*/
                curquellReflectMap[curRow][curColumn] = CellEmpty;
                curquellReflectMap[temp][curColumn] = CellRainDrop;


                /*检测状态，重复则退出*/
                if(checkstate(curquellReflectMap, row_max, column_max,  state) == 0)
                {
                    break;
                }else
                {
                    addstate(curquellReflectMap, row_max, column_max,  state);
                }
                /*本次移动引起了变化*/
                dfs(lv+1,state);

                removestate();
                break;
            }
        }
        /*如果是黄色墙壁，且水珠黄,直接穿过,到下一格*/
        if((curquellReflectMap[i][curColumn] == CellYellowWall && state == 1))
        {
            state = 0;
            changeflag = 1;
            curquellReflectMap[i][curColumn] = CellEmpty;
            continue;
        }
        /*如果是水珠,吃掉*/
        if(curquellReflectMap[i][curColumn] == CellNormalPearl ||  curquellReflectMap[i][curColumn] == CellYellowPearl)
        {
            changeflag = 1;
            if(curquellReflectMap[i][curColumn] == CellYellowPearl)state = 1;
            curquellReflectMap[i][curColumn] = CellEmpty;
            continue;
        }
        /*如果是门，则考虑要不要过门*/
        if(curquellReflectMap[i][curColumn] == CellOpenGate)
        {
            /*先假设直接过*/
            changeflag = 1;
            gateflag = 1;
            curquellReflectMap[i][curColumn] = CellNormalWall;
            continue;
        }
    }
    /*恢复部分*/
    recover();
    My_cur_moveList.pop_back();


    /*移动部分MoveUp*/
    changeflag = 0;
    gateflag  = 0;

    My_cur_moveList.push_back(MoveDown);
    for(i = curRow+1; (i != curRow || gateflag == 1) ; i++)
    {
        i = (i + row_max)%row_max;
        /*如果是墙壁，则停留在这里，包含黄色墙壁*/
        if(curquellReflectMap[i][curColumn] == CellNormalWall || (curquellReflectMap[i][curColumn] == CellYellowWall && state == 0))
        {
            temp = (i-1+row_max)%row_max;
            /*如果水还在原地并且没有引起变化，或者回到了父亲节点并且未引起变化，则丢弃这一分支,否则深入搜索*/
            {
                /*雨滴移动到当前位置*/
                curquellReflectMap[curRow][curColumn] = CellEmpty;
                curquellReflectMap[temp][curColumn] = CellRainDrop;


                /*检测状态，重复则退出*/
                if(checkstate(curquellReflectMap, row_max, column_max,  state) == 0)
                {
                    break;
                }else
                {
                    addstate(curquellReflectMap, row_max, column_max,  state);
                }
                /*本次移动引起了变化*/
                dfs(lv+1,state);

                removestate();
                break;
            }
        }
        /*如果是黄色墙壁，且水珠黄,直接穿过,到下一格*/
        if((curquellReflectMap[i][curColumn] == CellYellowWall && state == 1))
        {
            state = 0;
            changeflag = 1;
            curquellReflectMap[i][curColumn] = CellEmpty;
            continue;
        }
        /*如果是水珠,吃掉*/
        if(curquellReflectMap[i][curColumn] == CellNormalPearl ||  curquellReflectMap[i][curColumn] == CellYellowPearl)
        {
            changeflag = 1;
            if(curquellReflectMap[i][curColumn] == CellYellowPearl)state = 1;
            curquellReflectMap[i][curColumn] = CellEmpty;
            continue;
        }
        /*如果是门，则考虑要不要过门*/
        if(curquellReflectMap[i][curColumn] == CellOpenGate)
        {
            /*先假设直接过*/
            changeflag = 1;
            gateflag = 1;
            curquellReflectMap[i][curColumn] = CellNormalWall;
            continue;
        }
    }
    /*恢复部分*/
    recover();
    My_cur_moveList.pop_back();

    /*移动部分MoveRight*/
    changeflag = 0;
    gateflag  = 0;

    My_cur_moveList.push_back(MoveRight);
    for(i = curColumn+1; (i != curColumn || gateflag == 1) ; i++)
    {
        i = (i + column_max)%column_max;
        /*如果是墙壁，则停留在这里，包含黄色墙壁*/
        if(curquellReflectMap[curRow][i] == CellNormalWall || (curquellReflectMap[curRow][i] == CellYellowWall && state == 0))
        {
            temp = (i-1+column_max)%column_max;
            /*如果水还在原地并且没有引起变化，或者回到了父亲节点并且未引起变化，则丢弃这一分支,否则深入搜索*/
            {
                /*雨滴移动到当前位置*/
                curquellReflectMap[curRow][curColumn] = CellEmpty;
                curquellReflectMap[curRow][temp] = CellRainDrop;


                /*检测状态，重复则退出*/
                if(checkstate(curquellReflectMap, row_max, column_max,  state) == 0)
                {
                    break;
                }else
                {
                    addstate(curquellReflectMap, row_max, column_max,  state);
                }
                /*本次移动引起了变化*/
                dfs(lv+1,state);

                removestate();
                break;
            }
        }
        /*如果是黄色墙壁，且水珠黄,直接穿过,到下一格*/
        if((curquellReflectMap[curRow][i] == CellYellowWall && state == 1))
        {
            state = 0;
            changeflag = 1;
            curquellReflectMap[curRow][i] = CellEmpty;
            continue;
        }
        /*如果是水珠,吃掉*/
        if(curquellReflectMap[curRow][i] == CellNormalPearl ||  curquellReflectMap[curRow][i] == CellYellowPearl)
        {
            changeflag = 1;
            if(curquellReflectMap[curRow][i] == CellYellowPearl)state = 1;
            curquellReflectMap[curRow][i] = CellEmpty;
            continue;
        }
        /*如果是门，则考虑要不要过门*/
        if(curquellReflectMap[curRow][i] == CellOpenGate)
        {
            /*先假设直接过*/
            changeflag = 1;
            gateflag = 1;
            curquellReflectMap[curRow][i] = CellNormalWall;
            continue;
        }
    }

    /*恢复部分*/
    recover();
    My_cur_moveList.pop_back();

    /*移动部分MoveLeft*/
    changeflag = 0;
    gateflag  = 0;

    My_cur_moveList.push_back(MoveLeft);
    for(i = curColumn-1; (i != curColumn || gateflag == 1) ; i--)
    {
        i = (i + column_max)%column_max;
        /*如果是墙壁，则停留在这里，包含黄色墙壁*/
        if(curquellReflectMap[curRow][i] == CellNormalWall || (curquellReflectMap[curRow][i] == CellYellowWall && state == 0))
        {
            temp = (i+1+column_max)%column_max;
            {
                /*雨滴移动到当前位置*/
                curquellReflectMap[curRow][curColumn] = CellEmpty;
                curquellReflectMap[curRow][temp] = CellRainDrop;


                /*检测状态，重复则退出*/
                if(checkstate(curquellReflectMap, row_max, column_max,  state) == 0)
                {
                    break;
                }else
                {
                    addstate(curquellReflectMap, row_max, column_max,  state);
                }
                /*本次移动引起了变化*/
                dfs(lv+1,state);

                removestate();
                break;
            }
        }
        /*如果是黄色墙壁，且水珠黄,直接穿过,到下一格*/
        if((curquellReflectMap[curRow][i] == CellYellowWall && state == 1))
        {
            state = 0;
            changeflag = 1;
            curquellReflectMap[curRow][i] = CellEmpty;
            continue;
        }
        /*如果是水珠,吃掉*/
        if(curquellReflectMap[curRow][i] == CellNormalPearl ||  curquellReflectMap[curRow][i] == CellYellowPearl)
        {
            changeflag = 1;
            if(curquellReflectMap[curRow][i] == CellYellowPearl)state = 1;
            curquellReflectMap[curRow][i] = CellEmpty;
            continue;
        }
        /*如果是门，则考虑要不要过门*/
        if(curquellReflectMap[curRow][i] == CellOpenGate)
        {
            /*先假设直接过*/
            changeflag = 1;
            gateflag = 1;
            curquellReflectMap[curRow][i] = CellNormalWall;
            continue;
        }
    }
    /*恢复部分*/
    recover();
    My_cur_moveList.pop_back();

}

PlayerBird::PlayerBird()
{

}

PlayerBird::~PlayerBird()
{

}

std::list<MoveDirection> PlayerBird::solutionToMap( CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount )
{
    int i,j;
    list<MoveDirection> moveList;
    row_max = rowCount;
    column_max = columnCount;
    for(i = 0; i < row_max; i++)
    {
        for(j = 0; j < column_max; j++)
        {
            curquellReflectMap[i][j] = quellReflectMap[i][j];
        }
    }
    findflag = 0;
    /* 例如 */
    dfs(0, 0);

    while(!My_save_moveList.empty())
    {
        MoveDirection temp1 = My_save_moveList.front();
        moveList.push_back(temp1);
        My_save_moveList.pop_front();
    }
    return moveList;
}

std::string PlayerBird::teamNameToDisplay()
{
    /* 必须为英文，且小于20个字符 */
    return "小小鸟队";
}
