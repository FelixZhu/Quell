#ifndef _SEEDPK_H_
#define _SEEDPK_H_

#include "SeedPKGlobal.h"

#include <list>
#include <string>

//==================================================================
//���ܣ���������ĵ�ͼ���������ζ�Ӧ���ƶ�˳��
//���������quellReflectMap�����ͼ����ע���ڴ�Ϊ�˷��㣬ֱ�Ӹ���������ͼ����ʵ���Ͼ��������
//                û���õ�Ԫ�ء�����CellState��MoveDirection�Ķ������
//                SeedPKGlobal.h
//                rowCount����ʵ�ʵ�ͼ����������������ͼ�������±����0��ʼ��
//                columnCount����ʵ�ʵ�ͼ����������������ͼ�������±����0��ʼ��
//����ֵ�� ��ε��ƶ�����˳��ע��ͨ������Ҫ���ƶ�����Խ�٣�����Խ��
//==================================================================
std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

/* ���ز�����������ƣ�������20���ַ�(����ΪӢ���ַ�) */
std::string teamNameToDisplay();

#endif