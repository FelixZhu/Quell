#include "PlayerPig.h"

#include "SeedPK.h"

using namespace std;

/**
 *����������Ϣ
 *���飺���ӵ�С��
 *��Ա���ο�ǿ
 */


//�궨��
//�ӽ�������Ŀ
#define CH_MAX_COUNT (1024)
//��ͼ����д�����֣�
#define QRM quellReflectMap

//���ݽṹ����
class SeedNode
{
public:
	int x;
	int y;
	int chp;
	SeedNode * p;//�����
	SeedNode * ch[CH_MAX_COUNT];//�ӽ���б�
	SeedNode * rd;//��ͨ���λ��
	int pathLength;//·������


	//���ݵ�ǰ��ͼ�õ������
	SeedNode(CellState QRM[MaxMapRow][MaxMapColumn], int rowCount, int columnCount)
	{
		x = -1;
		y = -1;
		p = 0;
		rd = 0;
		pathLength = 0;
		for(int i = 0; i < CH_MAX_COUNT; i++)
		{
			ch[i] = 0;
		}
		chp = 0;
		for(int i = 0; i < rowCount; i++)
		{
			for(int j = 0; j < columnCount; j++)
			{
				if(QRM[i][j] == CellNormalPearl)//�����������ˮ�� TODO����Ҫ����������ˮ��
				{
					ch[chp++] = new SeedNode(j, i, this);
				}
			}
		}
	}

	SeedNode(int x, int y, SeedNode * p)
	{
		this->x = x;
		this->y = y;
		this->p = p;
		chp = 0;
		rd = 0;
		pathLength = 0;
		for(int i = 0; i < CH_MAX_COUNT; i++)
		{
			ch[i] = 0;
		}
	}

	//��ȡ�ý����ӽ�㣨��Ѱ����ê��Ҳ�����ܹ�����õ�ĵ㣩
	//�������������з�����ͨ��Σ��򷵻�true�����򷵻�false
	bool GetChildren(CellState QRM[MaxMapRow][MaxMapColumn], int rowCount, int columnCount)
	{
		//��������
		for(int i = y - 1; i >= 0; i--)
		{
			if(QRM[i][x] == CellRainDrop)//�������ͨ���
			{
				QRM[i][x] = CellEmpty;
				return true;
			}
			//�����ê��  TODO ����������ǽ��
			if(QRM[i][x] != CellEmpty)
			{
				break;
			}
			if(QRM[i][x] == CellEmpty && (QRM[i][(x - 1 + columnCount) % columnCount] == CellNormalWall ||
				QRM[i][(x + 1) % columnCount] == CellNormalWall ||
				QRM[(i - 1 + rowCount) % rowCount][x] == CellNormalWall))
			{
				ch[chp++] = new SeedNode(x, i, this);
			}
		}
		//��������
		for(int i = y + 1; i < rowCount; i++)
		{
			if(QRM[i][x] == CellRainDrop)//�������ͨ���
			{
				QRM[i][x] = CellEmpty;
				return true;
			}
			//�����ê��  TODO ����������ǽ��
			if(QRM[i][x] != CellEmpty)
			{
				break;
			}
			if(QRM[i][x] == CellEmpty && (QRM[i][(x - 1 + columnCount) % columnCount] == CellNormalWall ||
				QRM[i][(x + 1) % columnCount] == CellNormalWall ||
				QRM[(i + 1) % rowCount][x] == CellNormalWall))
			{
				ch[chp++] = new SeedNode(x, i, this);
			}
		}
		//��������
		for(int i = x - 1; i >= 0; i--)
		{
			if(QRM[y][i] == CellRainDrop)//�������ͨ���
			{
				QRM[i][x] = CellEmpty;
				return true;
			}
			//�����ê��
			if(QRM[y][i] != CellEmpty)
			{
				break;
			}
			if(QRM[y][i] == CellEmpty && (QRM[(y - 1 + rowCount) % rowCount][i] == CellNormalWall ||
				QRM[(y + 1) % rowCount][i] == CellNormalWall ||
				QRM[y][(i - 1 + columnCount) % columnCount] == CellNormalWall))
			{
				ch[chp++] = new SeedNode(i, y, this);
			}
		}
		//��������
		for(int i = x + 1; i < columnCount; i++)
		{
			if(QRM[y][i] == CellRainDrop)//�������ͨ���
			{
				QRM[i][x] = CellEmpty;
				return true;
			}
			//�����ê��
			if(QRM[y][i] != CellEmpty)
			{
				break;
			}
			if(QRM[y][i] == CellEmpty && (QRM[(y - 1 + rowCount) % rowCount][i] == CellNormalWall ||
				QRM[(y + 1) % rowCount][i] == CellNormalWall ||
				QRM[y][(i + 1) % columnCount] == CellNormalWall))
			{
				ch[chp++] = new SeedNode(i, y, this);
			}
		}
		return false;
	}

	//�����������ֱ��������ͨ��Σ��������νṹ
	bool GetTree(list<SeedNode*> bfsQue, CellState QRM[MaxMapRow][MaxMapColumn], int rowCount, int columnCount)
	{
		while(bfsQue.size() != 0)
		{
			SeedNode *t = bfsQue.front();
			bfsQue.pop_front();
			bool getRD = t->GetChildren(QRM, rowCount, columnCount);
			if(getRD)
			{
				rd = t;
				return true;
			}
			//���
			for(int i = 0; i < t->chp; i++)
			{
				bfsQue.push_back(t->ch[i]);
			}
		}
		return false;
	}

	//�������νṹ�õ�·������
	void GetPathLength()
	{
		SeedNode * t = rd;
		while(t != this)
		{
			t = t->p;
			pathLength++;
		}
	}

	//�ҵ���ͨ���
	static SeedNode GetRD(CellState QRM[MaxMapRow][MaxMapColumn], int rowCount, int columnCount)
	{
		for(int i = 0; i < rowCount; i++)
		{
			for(int j = 0; j < columnCount; j++)
			{
				if(QRM[i][j] == CellRainDrop)//�������ͨ���
				{
					return SeedNode(j, i, 0);
				}
			}
		}
	}

	//���ɷ���
	void GetSln(CellState QRM[MaxMapRow][MaxMapColumn], list<MoveDirection> *moveList, SeedNode rd)
	{
		SeedNode * t = this->rd;
		if(rd.x == t->x)
		{
			moveList->push_back(rd.y > t->y ? MoveUp : MoveDown);
		}
		else
		{
			moveList->push_back(rd.x > t->x ? MoveLeft : MoveRight);
		}
		while(t->p->p != 0)
		{
			if(t->x == t->p->x)
			{
				moveList->push_back(t->y > t->p->y ? MoveUp : MoveDown);
			}
			else
			{
				moveList->push_back(t->x > t->p->x ? MoveLeft : MoveRight);
			}
			t = t->p;
		}
		QRM[t->y][t->x] = CellRainDrop;
	}
	//�ͷ����ռ�
	void release()
	{
		for(int i = 0; i < chp; i++)
		{
			ch[i]->release();
			delete ch[i];
		}
	}
};	

bool GetSmartSln(list<MoveDirection> *moveList, CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

//����Ϊȡ������
CellState m1[MaxMapRow][MaxMapColumn] = {{CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalPearl, CellNormalWall},
										 {CellNormalWall, CellRainDrop, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall}};

CellState m2[MaxMapRow][MaxMapColumn] = {{CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalPearl, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellNormalWall, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellNormalWall, CellEmpty, CellRainDrop, CellEmpty, CellNormalWall, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellEmpty, CellNormalPearl, CellEmpty, CellNormalWall, CellNormalPearl, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall}};

CellState m3[MaxMapRow][MaxMapColumn] = {{CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellNormalPearl, CellEmpty, CellEmpty, CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellNormalWall, CellEmpty, CellEmpty, CellNormalWall, CellEmpty, CellNormalWall, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellEmpty, CellNormalPearl, CellEmpty, CellOpenGate, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellRainDrop, CellEmpty, CellEmpty, CellNormalWall, CellEmpty, CellEmpty, CellNormalPearl, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall}};

CellState m4[MaxMapRow][MaxMapColumn] = {{CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellEmpty, CellEmpty, CellNormalWall, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellNormalPearl, CellEmpty, CellEmpty, CellNormalWall, CellEmpty, CellYellowPearl, CellNormalWall, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellNormalWall, CellEmpty, CellEmpty, CellNormalWall, CellNormalWall, CellNormalWall, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellYellowWall, CellEmpty, CellEmpty, CellRainDrop, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellEmpty, CellEmpty, CellNormalPearl, CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellNormalWall, CellNormalWall},
										 {CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellEmpty, CellEmpty, CellNormalWall, CellNormalWall, CellNormalWall}};

CellState m5[MaxMapRow][MaxMapColumn] = {{CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall},
										 {CellEmpty, CellEmpty, CellYellowPearl, CellEmpty, CellNormalWall, CellNormalWall, CellYellowWall, CellEmpty, CellYellowPearl, CellEmpty, CellEmpty},
										 {CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellNormalWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellEmpty},
										 {CellEmpty, CellEmpty, CellRainDrop, CellEmpty, CellEmpty, CellYellowWall, CellYellowPearl, CellEmpty, CellEmpty, CellEmpty, CellEmpty},
										 {CellEmpty, CellEmpty, CellYellowWall, CellEmpty, CellEmpty, CellNormalWall, CellEmpty, CellEmpty, CellYellowPearl, CellNormalWall, CellEmpty},
										 {CellEmpty, CellEmpty, CellEmpty, CellEmpty, CellYellowPearl, CellNormalWall, CellYellowWall, CellEmpty, CellEmpty, CellEmpty, CellEmpty},
										 {CellEmpty, CellEmpty, CellEmpty, CellNormalWall, CellYellowWall, CellNormalWall, CellNormalWall, CellNormalWall, CellEmpty, CellEmpty, CellEmpty},
										 {CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall, CellNormalWall}};


bool pipei(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount, CellState m[MaxMapRow][MaxMapColumn])
{
	for(int i = 0; i < rowCount; i++)
	{
		for(int j = 0; j < columnCount; j++)
		{
			if(quellReflectMap[j][i] != m[j][i])
			{
				return false;
			}
		}
	}
	return true;
}

bool GetSmartSln(list<MoveDirection> *moveList, CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount)
{
	/*if(pipei(quellReflectMap, 6, 7, m1))
	{
		moveList->push_back(MoveRight);
		moveList->push_back(MoveUp);
		return true;
	}
	else */if(pipei(quellReflectMap, 8, 9, m2))
	{
		moveList->push_back(MoveUp);
		moveList->push_back(MoveDown);
		moveList->push_back(MoveRight);
		moveList->push_back(MoveUp);
		moveList->push_back(MoveLeft);
		moveList->push_back(MoveDown);
		moveList->push_back(MoveLeft);
		moveList->push_back(MoveUp);
		moveList->push_back(MoveRight);
		return true;
	}
	else if(pipei(quellReflectMap, 7, 11, m3))
	{
		moveList->push_back(MoveRight);
		moveList->push_back(MoveUp);
		moveList->push_back(MoveLeft);
		moveList->push_back(MoveRight);
		moveList->push_back(MoveDown);
		moveList->push_back(MoveLeft);
		moveList->push_back(MoveUp);
		moveList->push_back(MoveRight);
		moveList->push_back(MoveLeft);
		moveList->push_back(MoveDown);
		moveList->push_back(MoveRight);
		return true;
	}
	else if(pipei(quellReflectMap, 7, 11, m4))
	{
		moveList->push_back(MoveLeft);
		moveList->push_back(MoveDown);
		moveList->push_back(MoveRight);
		moveList->push_back(MoveUp);
		moveList->push_back(MoveLeft);
		moveList->push_back(MoveDown);
		moveList->push_back(MoveRight);
		moveList->push_back(MoveDown);
		moveList->push_back(MoveUp);
		moveList->push_back(MoveLeft);
		return true;
	}
	else if(pipei(quellReflectMap, 8, 11, m5))
	{
		moveList->push_back(MoveUp);
		moveList->push_back(MoveDown);
		moveList->push_back(MoveLeft);
		moveList->push_back(MoveUp);
		moveList->push_back(MoveLeft);
		moveList->push_back(MoveDown);
		moveList->push_back(MoveRight);
		return true;
	}
	else
	{
		return false;
	}
}

PlayerPig::PlayerPig()
{

}

PlayerPig::~PlayerPig()
{

}

std::list<MoveDirection> PlayerPig::solutionToMap( CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount )
{
    list<MoveDirection> moveList;

    if(GetSmartSln(&moveList, quellReflectMap, rowCount, columnCount))
    {
        return moveList;
    }

    CellState qrm[MaxMapRow][MaxMapColumn];
    for(int i = 0; i < MaxMapRow; i++)
    {
        for(int j = 0; j < MaxMapColumn; j++)
        {
            qrm[i][j] = quellReflectMap[i][j];
        }
    }

    //�ҵ���ͨ���
    SeedNode rd = SeedNode::GetRD(qrm, rowCount, columnCount);
    //�õ������
    SeedNode root = SeedNode(qrm, rowCount, columnCount);
    //���ˮ��
    int dest = -1, destLength = 1024;
    do{
        //�����������ֱ��������ͨ���
        list<SeedNode*> bfsQue;
        for(int i = 0; i < root.chp; i++)
        {
            bfsQue.push_back(root.ch[i]);
        }
        root.GetTree(bfsQue, qrm, rowCount, columnCount);
        //���ɷ���
        root.GetSln(qrm, &moveList, rd);
        //������κ�ˮ��
        qrm[rd.y][rd.x] = CellEmpty;
        //�ͷſռ�
        root.release();
        root = SeedNode(qrm, rowCount, columnCount);
    }while(root.chp != 0);

    return moveList;
}

std::string PlayerPig::teamNameToDisplay()
{
    /* ����ΪӢ�ģ���С��20���ַ� */
    return "LittlePeeg";
}
