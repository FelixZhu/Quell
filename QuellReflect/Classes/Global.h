#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "SeedPKGlobal.h"

/* ���������� */
#define MaxRainDropNumber 2

/* ���ؿ� */
#define MaxStageNumber 9

/* ��Ƭ��ͼ����Ƭ��С */
#define TileHeight 32
#define TileWidth 32

/* ͼƬ���ļ� */
#define mainBackgroudFile "../Resources/window_open_HD.png"
#define GameAtlasFile "../Resources/game_atlas_Q2.png"

#define ArrowUpSelectedFile "../Resources/arrow_up_selected.png"
#define ArrowUpUnselectedFile "../Resources/arrow_up_unselected.png"

#define LoadSelectedFile "../Resources/load_selected.png"
#define LoadUnselectedFile "../Resources/load_unselected.png"

#define replaySelectedFile "../Resources/retry.png"
#define replayUnselectedFile "../Resources/retry.png"

#define nextSelectedFile "../Resources/next_selected.png"
#define nextUnselectedFile "../Resources/next_unselected.png"


/* ��ϷTitle */
#define  GameTitleString "2012�����ӱ�����"

/* ��ε�״̬ */
typedef enum
{
    RainDropStateMin = 0,

    /* ��������� */
    RainDropNormal,

    /* ��ɫ����Σ�����ͨ����ɫ��ǽ�� */
    RainDropYellow,

    /* ������� */
    RainDropDead,

    RainDropStateMax
}RainDropState;

typedef struct
{
    int row, column;
}Point;

/* ��ε����� */
typedef struct
{
    /* ���� */
    Point point;

    /* ��ǰ��ε�״̬ */
    RainDropState state;
}RainDrop;

/* ��ͼ�ϵ�һЩtagֵ */

#endif