#ifndef _PLAYER_
#define _PLAYER_

#include "SeedPKGlobal.h"

#include <list>
#include <string>

class Player
{
public:
    Player();

    virtual ~Player();

    //==================================================================
    //���ܣ���������ĵ�ͼ���������ζ�Ӧ���ƶ�˳��
    //���������quellReflectMap�����ͼ����ע���ڴ�Ϊ�˷��㣬ֱ�Ӹ���������ͼ����ʵ���Ͼ��������
    //                û���õ�Ԫ�ء�����CellState��MoveDirection�Ķ������
    //                SeedPKGlobal.h
    //                rowCount����ʵ�ʵ�ͼ����������������ͼ�������±����0��ʼ��
    //                columnCount����ʵ�ʵ�ͼ����������������ͼ�������±����0��ʼ��
    //����ֵ�� ��ε��ƶ�����˳��ע��ͨ������Ҫ���ƶ�����Խ�٣�����Խ��
    //==================================================================
    virtual std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount) = 0;

    /* ���ز�����������ƣ�������20���ַ�(����ΪӢ���ַ�) */
    virtual std::string teamNameToDisplay() = 0;

private:
    int m_score;
};

#endif