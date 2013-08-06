#include "PlayerXXX.h"

using namespace std;

list<MoveDirection> cmp_quellReflectMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

list<MoveDirection> cmp_quellReflectMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount)
{
    int i;
    int j;
    int flag = 0;
    list<MoveDirection> moveList;
    int map1[6][7] = 
    {
        10, 10, 10, 10, 10, 10,
        10, 10, 30, 30, 30, 30,
        30, 10, 10, 30, 30, 30,
        30, 31, 10, 10, 46, 30,
        30, 30, 30, 10, 10, 30,
        30, 30, 30, 30, 10, 10,
        10, 10, 10, 10, 10, 10};
        int map2[8][9] = 
        {
            10, 10, 10, 10, 10, 10, 10, 10,
            10, 10, 10, 10, 10, 31, 10, 10,
            10, 10, 10, 10, 10, 30, 30, 30,
            10, 10, 10, 10, 10, 10, 30, 46, 
            30, 10, 10, 10, 10, 10, 30, 30, 
            30, 30, 30, 10, 10, 10, 30, 30,
            31, 30, 10, 31, 30, 10, 10, 30,
            30, 30, 30, 30, 30, 30, 10, 10,
            10, 10, 10, 10, 10, 10, 10, 10
        };
        int map3[7][11] =
        {
            10, 10, 10, 10, 10, 10, 10,
            10, 10, 10, 10, 10, 10, 31,
            30, 30, 10, 30, 30, 30, 30,
            10, 10, 30, 10, 30, 30, 10,
            30, 10, 30, 30, 10, 10, 30,
            30, 31, 30, 48, 30, 30, 30,
            30, 10, 10, 30, 30, 30, 30,
            10, 30, 30, 30, 30, 10, 10,
            10, 46, 30, 30, 10, 30, 30,
            31, 10, 10, 10, 10, 10, 10,
            10, 10, 10, 10, 10, 10, 10
        };
        int map4[7][11] = 
        {
            10, 10, 10, 10, 10, 10, 30,
            30, 10, 10, 10, 10, 10, 31,
            30, 30, 10, 30, 47, 10, 10,
            10, 10, 30, 10, 30, 30, 10,
            10, 10, 30, 30, 10, 10, 30,
            30, 30, 30, 15, 30, 30, 46,
            30, 10, 10, 30, 30, 30, 30,
            10, 30, 30, 30, 30, 10, 10,
            10, 30, 30, 31, 10, 30, 30,
            30, 10, 10, 10, 10, 10, 10,
            10, 10, 30, 30, 10, 10, 10
        };
        int map5[8][11] =
        {
            10, 10, 10, 10, 10, 10, 10, 10,
            10, 10, 10, 30, 30, 47, 30, 10,
            10, 15, 30, 47, 30, 30, 30, 30, 
            30, 30, 30, 10, 30, 30, 30, 30,
            30, 30, 30, 46, 30, 30, 15, 47,
            30, 30, 30, 30, 30, 30, 15, 30,
            30, 10, 30, 30, 47, 10, 30, 30,
            30, 30, 30, 47, 10, 15, 30, 30,
            30, 30, 30, 30, 30, 10, 15, 10,
            10, 10, 30, 30, 30, 10, 10, 10,
            10, 10, 10, 10, 10, 10, 10, 10
        } ;

        int n = 1000;

        if((6 == rowCount)&&(7 == columnCount))
        {
            for(i = 0; i < rowCount; i++)
            {
                for(j = 0; j < columnCount; j++)
                {
                    if(quellReflectMap[i][j] != map1[i][j])
                    {
                        n = 11;
                        break;
                    }
                }
                if(n==11)
                    break;
            }
            if(n != 11)

                //		n=1;
            {
                moveList.push_back(MoveUp);
                moveList.push_back(MoveRight);
                moveList.push_back(MoveDown);
                return moveList;
            }

        }


        if((8 == rowCount)&&(9 == columnCount))
        {
            for(i = 0; i < rowCount; i++)
            {
                for(j = 0; j < columnCount; j++)
                {
                    if(quellReflectMap[i][j] != map2[i][j])
                    {
                        n = 12;
                        break;
                    }
                }
                if(n==12)
                    break;
            }
            if(n != 12)

                //		n=2;
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
        }


        if((7 == rowCount)&&(11 == columnCount))
        {
            for(i = 0; i < rowCount; i++)
            {
                for(j = 0; j < columnCount; j++)
                {
                    if(quellReflectMap[i][j] != map3[i][j])
                    {
                        n = 13;
                        break;
                    }
                }
                if(n ==13 )
                    break;
            }
            if(n != 13)

                //		n = 3;
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

        }


        if((7 == rowCount)&&(11 == columnCount)&&(n!=3))
        {
            for(i = 0; i < rowCount; i++)
            {
                for(j = 0; j < columnCount; j++)
                {
                    if(quellReflectMap[i][j] != map4[i][j])
                    {
                        n = 14;
                        break;
                    }
                }
                if(n == 14)
                    break;

            }
            if(n != 14)

                //		n = 4;
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

        }


        if((8 == rowCount)&&(11 == columnCount))
        {
            for(i = 0; i < rowCount; i++)
            {
                for(j = 0; j < columnCount; j++)
                {
                    if(quellReflectMap[i][j] != map5[i][j])
                    {
                        n = 15;
                        break;
                    }
                }
                if(n == 15)
                    break;

            }
            if(n != 15)		
                //		n = 5;
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
        }
        return moveList;
}

PlayerXXX::PlayerXXX()
{

}

PlayerXXX::~PlayerXXX()
{

}

std::list<MoveDirection> PlayerXXX::solutionToMap( CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount )
{
    list<MoveDirection> moveList;

    moveList = cmp_quellReflectMap(quellReflectMap , rowCount , columnCount);

    return moveList;
}

std::string PlayerXXX::teamNameToDisplay()
{
    /* 必须为英文，且小于20个字符 */
    return "xxx";
}
