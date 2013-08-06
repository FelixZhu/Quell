#ifndef _SEEDPKGLOBAL_H_
#define _SEEDPKGLOBAL_H_

/* 地图最大行数 */
#define MaxMapRow 20

/* 地图最大列数 */
#define MaxMapColumn 20

/* 雨滴移动方向 */
typedef enum
{
    MoveDirectionMin = 0,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    MoveDirectionMax
}MoveDirection;

/* 地图上一个点的含义(这个点代表的是什么) */
typedef enum
{
    CellStateMin = 0,

    /* 普通墙壁，雨滴无法通过 */
    CellNormalWall = 10,

    /* 黄色墙壁，雨滴在吃过黄色水珠之后可以通过，否则不能通过 */
    CellYellowWall = 15,

    /* 空白，不存在任何障碍物 */
    CellEmpty = 30,

    /* 正常的水珠，雨滴可以吃 */
    CellNormalPearl = 31,

    /* 有毒的水滴，雨滴不能碰，否则该雨滴立即死亡 */
    CellPoisonDrop = 40,

    /* 雨滴 */
    CellRainDrop = 46,

    /* 黄色的水珠，雨滴吃过以后可以通过黄色的墙壁 */
    CellYellowPearl = 47,

    /* 开门，雨滴只可以通过一次，通过之后门与墙的效果一样 */
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