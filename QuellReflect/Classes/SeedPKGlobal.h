#ifndef _SEEDPKGLOBAL_H_
#define _SEEDPKGLOBAL_H_

/* ��ͼ������� */
#define MaxMapRow 20

/* ��ͼ������� */
#define MaxMapColumn 20

/* ����ƶ����� */
typedef enum
{
    MoveDirectionMin = 0,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    MoveDirectionMax
}MoveDirection;

/* ��ͼ��һ����ĺ���(�����������ʲô) */
typedef enum
{
    CellStateMin = 0,

    /* ��ͨǽ�ڣ�����޷�ͨ�� */
    CellNormalWall = 10,

    /* ��ɫǽ�ڣ�����ڳԹ���ɫˮ��֮�����ͨ����������ͨ�� */
    CellYellowWall = 15,

    /* �հף��������κ��ϰ��� */
    CellEmpty = 30,

    /* ������ˮ�飬��ο��Գ� */
    CellNormalPearl = 31,

    /* �ж���ˮ�Σ���β����������������������� */
    CellPoisonDrop = 40,

    /* ��� */
    CellRainDrop = 46,

    /* ��ɫ��ˮ�飬��γԹ��Ժ����ͨ����ɫ��ǽ�� */
    CellYellowPearl = 47,

    /* ���ţ����ֻ����ͨ��һ�Σ�ͨ��֮������ǽ��Ч��һ�� */
    CellOpenGate = 48,

    CellStateMax
}CellState;

enum {
	TeamLJX = 1000,
	TeamXXX = 2000,
    TeamBird = 3000,
	TeamOOO = 4000,
	TeamThree = 5000,
	TeamPig = 6000,
    TeamFngng = 7000
};

#endif