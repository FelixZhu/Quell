#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "SeedPKGlobal.h"

/* 最多雨滴数量 */
#define MaxRainDropNumber 2

/* 最多关卡 */
#define MaxStageNumber 9

/* 瓦片地图的瓦片大小 */
#define TileHeight 32
#define TileWidth 32

/* 图片集文件 */
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


/* 游戏Title */
#define  GameTitleString "2012年种子杯决赛"

/* 雨滴的状态 */
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

/* 雨滴的坐标 */
typedef struct
{
    /* 坐标 */
    Point point;

    /* 当前雨滴的状态 */
    RainDropState state;
}RainDrop;

/* 地图上的一些tag值 */

#endif