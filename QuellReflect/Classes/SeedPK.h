#ifndef _SEEDPK_H_
#define _SEEDPK_H_

#include "SeedPKGlobal.h"

#include <list>
#include <string>

//==================================================================
//功能：根据输入的地图矩阵给出雨滴对应的移动顺序
//输入参数：quellReflectMap代表地图矩阵，注意在此为了方便，直接给出了最大地图矩阵，实际上矩阵里存在
//                没有用的元素。关于CellState和MoveDirection的定义详见
//                SeedPKGlobal.h
//                rowCount代表实际地图矩阵的行数，矩阵地图的行列下标均从0开始。
//                columnCount代表实际地图矩阵的列数，矩阵地图的行列下标均从0开始。
//返回值： 雨滴的移动方向顺序，注意通关所需要的移动次数越少，分数越高
//==================================================================
std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

/* 返回参赛队伍的名称，需少于20个字符(必须为英文字符) */
std::string teamNameToDisplay();

#endif