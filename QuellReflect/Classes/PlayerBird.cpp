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
    /*�ָ�����*/
    for(i = 0; i < row_max; i++)
    {
        for(j = 0; j < column_max; j++)
        {
            /*�������*/
            curquellReflectMap[i][j] = savemap[i][j];
        }
    }
}
/*stateΪ���״̬��0������1��ɫ���*/
void dfs(int lv, int state)
{
    if(findflag == 1)return;
    /*�����Ӽ�֦*/
    int i,j, flag = 0;
    int temp;
    int gateflag = 0;
    int changeflag = 0;
    int curRow, curColumn;
    /*��֦1��������30��*/
    if(lv >= 3)return;
    /*�������ˮ�鶼������*/
    for(i = 0; i < row_max; i++)
    {
        for(j = 0; j < column_max; j++)
        {
            /*�������*/
            savemap[i][j] = curquellReflectMap[i][j];
            if(curquellReflectMap[i][j] == CellNormalPearl || curquellReflectMap[i][j] == CellYellowPearl)
                flag = 1;
            /*��¼��λ��*/
            if(curquellReflectMap[i][j] == CellRainDrop)
            {
                curRow = i;
                curColumn = j;
            }

        }
    }
    /*��֦1��״̬�ظ�*/

    /*��֦1��������30��*/
    /*��֦1��������30��*/
    /*ȫ��������,����*/
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

    /*�ƶ�����MoveUp*/
    changeflag = 0;
    gateflag  = 0;

    My_cur_moveList.push_back(MoveUp);
    for(i = curRow-1; (i != curRow || gateflag == 1) ; i--)
    {
        i = (i + row_max)%row_max;
        /*�����ǽ�ڣ���ͣ�������������ɫǽ��*/
        if(curquellReflectMap[i][curColumn] == CellNormalWall || (curquellReflectMap[i][curColumn] == CellYellowWall && state == 0))
        {
            temp = (i+1+row_max)%row_max;
            /*���ˮ����ԭ�ز���û������仯�����߻ص��˸��׽ڵ㲢��δ����仯��������һ��֧,������������*/
            {
                /*����ƶ�����ǰλ��*/
                curquellReflectMap[curRow][curColumn] = CellEmpty;
                curquellReflectMap[temp][curColumn] = CellRainDrop;


                /*���״̬���ظ����˳�*/
                if(checkstate(curquellReflectMap, row_max, column_max,  state) == 0)
                {
                    break;
                }else
                {
                    addstate(curquellReflectMap, row_max, column_max,  state);
                }
                /*�����ƶ������˱仯*/
                dfs(lv+1,state);

                removestate();
                break;
            }
        }
        /*����ǻ�ɫǽ�ڣ���ˮ���,ֱ�Ӵ���,����һ��*/
        if((curquellReflectMap[i][curColumn] == CellYellowWall && state == 1))
        {
            state = 0;
            changeflag = 1;
            curquellReflectMap[i][curColumn] = CellEmpty;
            continue;
        }
        /*�����ˮ��,�Ե�*/
        if(curquellReflectMap[i][curColumn] == CellNormalPearl ||  curquellReflectMap[i][curColumn] == CellYellowPearl)
        {
            changeflag = 1;
            if(curquellReflectMap[i][curColumn] == CellYellowPearl)state = 1;
            curquellReflectMap[i][curColumn] = CellEmpty;
            continue;
        }
        /*������ţ�����Ҫ��Ҫ����*/
        if(curquellReflectMap[i][curColumn] == CellOpenGate)
        {
            /*�ȼ���ֱ�ӹ�*/
            changeflag = 1;
            gateflag = 1;
            curquellReflectMap[i][curColumn] = CellNormalWall;
            continue;
        }
    }
    /*�ָ�����*/
    recover();
    My_cur_moveList.pop_back();


    /*�ƶ�����MoveUp*/
    changeflag = 0;
    gateflag  = 0;

    My_cur_moveList.push_back(MoveDown);
    for(i = curRow+1; (i != curRow || gateflag == 1) ; i++)
    {
        i = (i + row_max)%row_max;
        /*�����ǽ�ڣ���ͣ�������������ɫǽ��*/
        if(curquellReflectMap[i][curColumn] == CellNormalWall || (curquellReflectMap[i][curColumn] == CellYellowWall && state == 0))
        {
            temp = (i-1+row_max)%row_max;
            /*���ˮ����ԭ�ز���û������仯�����߻ص��˸��׽ڵ㲢��δ����仯��������һ��֧,������������*/
            {
                /*����ƶ�����ǰλ��*/
                curquellReflectMap[curRow][curColumn] = CellEmpty;
                curquellReflectMap[temp][curColumn] = CellRainDrop;


                /*���״̬���ظ����˳�*/
                if(checkstate(curquellReflectMap, row_max, column_max,  state) == 0)
                {
                    break;
                }else
                {
                    addstate(curquellReflectMap, row_max, column_max,  state);
                }
                /*�����ƶ������˱仯*/
                dfs(lv+1,state);

                removestate();
                break;
            }
        }
        /*����ǻ�ɫǽ�ڣ���ˮ���,ֱ�Ӵ���,����һ��*/
        if((curquellReflectMap[i][curColumn] == CellYellowWall && state == 1))
        {
            state = 0;
            changeflag = 1;
            curquellReflectMap[i][curColumn] = CellEmpty;
            continue;
        }
        /*�����ˮ��,�Ե�*/
        if(curquellReflectMap[i][curColumn] == CellNormalPearl ||  curquellReflectMap[i][curColumn] == CellYellowPearl)
        {
            changeflag = 1;
            if(curquellReflectMap[i][curColumn] == CellYellowPearl)state = 1;
            curquellReflectMap[i][curColumn] = CellEmpty;
            continue;
        }
        /*������ţ�����Ҫ��Ҫ����*/
        if(curquellReflectMap[i][curColumn] == CellOpenGate)
        {
            /*�ȼ���ֱ�ӹ�*/
            changeflag = 1;
            gateflag = 1;
            curquellReflectMap[i][curColumn] = CellNormalWall;
            continue;
        }
    }
    /*�ָ�����*/
    recover();
    My_cur_moveList.pop_back();

    /*�ƶ�����MoveRight*/
    changeflag = 0;
    gateflag  = 0;

    My_cur_moveList.push_back(MoveRight);
    for(i = curColumn+1; (i != curColumn || gateflag == 1) ; i++)
    {
        i = (i + column_max)%column_max;
        /*�����ǽ�ڣ���ͣ�������������ɫǽ��*/
        if(curquellReflectMap[curRow][i] == CellNormalWall || (curquellReflectMap[curRow][i] == CellYellowWall && state == 0))
        {
            temp = (i-1+column_max)%column_max;
            /*���ˮ����ԭ�ز���û������仯�����߻ص��˸��׽ڵ㲢��δ����仯��������һ��֧,������������*/
            {
                /*����ƶ�����ǰλ��*/
                curquellReflectMap[curRow][curColumn] = CellEmpty;
                curquellReflectMap[curRow][temp] = CellRainDrop;


                /*���״̬���ظ����˳�*/
                if(checkstate(curquellReflectMap, row_max, column_max,  state) == 0)
                {
                    break;
                }else
                {
                    addstate(curquellReflectMap, row_max, column_max,  state);
                }
                /*�����ƶ������˱仯*/
                dfs(lv+1,state);

                removestate();
                break;
            }
        }
        /*����ǻ�ɫǽ�ڣ���ˮ���,ֱ�Ӵ���,����һ��*/
        if((curquellReflectMap[curRow][i] == CellYellowWall && state == 1))
        {
            state = 0;
            changeflag = 1;
            curquellReflectMap[curRow][i] = CellEmpty;
            continue;
        }
        /*�����ˮ��,�Ե�*/
        if(curquellReflectMap[curRow][i] == CellNormalPearl ||  curquellReflectMap[curRow][i] == CellYellowPearl)
        {
            changeflag = 1;
            if(curquellReflectMap[curRow][i] == CellYellowPearl)state = 1;
            curquellReflectMap[curRow][i] = CellEmpty;
            continue;
        }
        /*������ţ�����Ҫ��Ҫ����*/
        if(curquellReflectMap[curRow][i] == CellOpenGate)
        {
            /*�ȼ���ֱ�ӹ�*/
            changeflag = 1;
            gateflag = 1;
            curquellReflectMap[curRow][i] = CellNormalWall;
            continue;
        }
    }

    /*�ָ�����*/
    recover();
    My_cur_moveList.pop_back();

    /*�ƶ�����MoveLeft*/
    changeflag = 0;
    gateflag  = 0;

    My_cur_moveList.push_back(MoveLeft);
    for(i = curColumn-1; (i != curColumn || gateflag == 1) ; i--)
    {
        i = (i + column_max)%column_max;
        /*�����ǽ�ڣ���ͣ�������������ɫǽ��*/
        if(curquellReflectMap[curRow][i] == CellNormalWall || (curquellReflectMap[curRow][i] == CellYellowWall && state == 0))
        {
            temp = (i+1+column_max)%column_max;
            {
                /*����ƶ�����ǰλ��*/
                curquellReflectMap[curRow][curColumn] = CellEmpty;
                curquellReflectMap[curRow][temp] = CellRainDrop;


                /*���״̬���ظ����˳�*/
                if(checkstate(curquellReflectMap, row_max, column_max,  state) == 0)
                {
                    break;
                }else
                {
                    addstate(curquellReflectMap, row_max, column_max,  state);
                }
                /*�����ƶ������˱仯*/
                dfs(lv+1,state);

                removestate();
                break;
            }
        }
        /*����ǻ�ɫǽ�ڣ���ˮ���,ֱ�Ӵ���,����һ��*/
        if((curquellReflectMap[curRow][i] == CellYellowWall && state == 1))
        {
            state = 0;
            changeflag = 1;
            curquellReflectMap[curRow][i] = CellEmpty;
            continue;
        }
        /*�����ˮ��,�Ե�*/
        if(curquellReflectMap[curRow][i] == CellNormalPearl ||  curquellReflectMap[curRow][i] == CellYellowPearl)
        {
            changeflag = 1;
            if(curquellReflectMap[curRow][i] == CellYellowPearl)state = 1;
            curquellReflectMap[curRow][i] = CellEmpty;
            continue;
        }
        /*������ţ�����Ҫ��Ҫ����*/
        if(curquellReflectMap[curRow][i] == CellOpenGate)
        {
            /*�ȼ���ֱ�ӹ�*/
            changeflag = 1;
            gateflag = 1;
            curquellReflectMap[curRow][i] = CellNormalWall;
            continue;
        }
    }
    /*�ָ�����*/
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
    /* ���� */
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
    /* ����ΪӢ�ģ���С��20���ַ� */
    return "СС���";
}
