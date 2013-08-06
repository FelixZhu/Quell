#include "PlayerLJX.h"

#include <vector>

using namespace std;

#define XASSERT(x)  (x)

class XState;
// 复制地图
void CopyMap(CellState tar[MaxMapRow][MaxMapColumn], CellState src[MaxMapRow][MaxMapColumn]);
// 搜索XState vector中是否存在相同的记录
bool ExistState(const vector<XState>&state_vec,const XState &state);
// 规格化地图，将超出地图大小的部分全部设置为空，之后即可把地图的大小当成MaxMapRow*MaxMapColumn
void RegulateMap(CellState tar_map[MaxMapRow][MaxMapColumn],int iRow,int iCol);

class XPos
{
public:
    int x;
    int y;
    bool operator==(const XPos &pos)const
    {
        return ( x == pos.x && y == pos.y );
    }
    bool operator!=(const XPos &pos)const
    {
        return !(*this==pos);
    }
};


#define  SET_EMPTY_AND_CHECK_SUCC(x,y) if(SetEmpty(x,y)) return UPRET_SUCC;

enum UpdateRet
{
    UPRET_SUCC,
    UPRET_DEAD_LOOP,
    UPRET_NORMAL
};

class XState
{
public:	
    XState(CellState reflect_map[MaxMapRow][MaxMapColumn], bool bGold = false, bool bDead=false):m_bGold(bGold),m_bDead(bDead)
    {
        SetMap(reflect_map);  // 复制地图到成员变量
        CountPearl(); // 计算水珠个数(不包含雨滴)
        FindMySelf(); // 查找并设置本身坐标，同时消除雨滴本身在地图中的记录
    }
    void CountPearl()
    {
        int i,j;
        m_cntPearl = 0;
        for(i=0;i<MaxMapRow;++i)
        {
            for(j=0;j<MaxMapColumn;++j)
            {
                if(m_Map[i][j]==CellNormalPearl || m_Map[i][j]==CellYellowPearl)
                {
                    ++m_cntPearl;
                }
            }
        }
    }
    void SetMap(CellState reflect_map[MaxMapRow][MaxMapColumn])
    {
        CopyMap(m_Map,reflect_map);
    }
    bool IsMapEqual(const CellState cmp_map[MaxMapRow][MaxMapColumn])const
    {
        int i,j;
        for(i=0;i<MaxMapRow;++i)
        {
            for(j=0;j<MaxMapColumn;++j)
            {
                if(m_Map[i][j] != cmp_map[i][j])
                {
                    return false;
                }
            }
        }
        return true;
    }

    // 判别状态是否相等
    bool IsEqual(const XState& cmp_state)const
    {
        if(m_bGold != cmp_state.m_bGold)
        {
            return false;
        }
        if(m_MyPos!=cmp_state.m_MyPos)
        {
            return false;
        }
        if(m_cntPearl!=cmp_state.m_cntPearl)
        {
            return false;
        }
        return IsMapEqual(cmp_state.m_Map);
    }

    // 获取动作序列
    void GetActions(list<MoveDirection> &actions)const
    {
        actions = m_MoveList;
    }

    // 根据方向进行一步位移，处理到达边界的情况
    void NextStep(XPos &m_MyPos,MoveDirection direct)
    {
        switch(direct)
        {
        case MoveUp:
            {
                m_MyPos.x--;
                if(m_MyPos.x == -1)
                {
                    m_MyPos.x = MaxMapRow - 1;
                }
                break;
            }
        case MoveDown:
            {
                m_MyPos.x++;
                if(m_MyPos.x == MaxMapRow)
                {
                    m_MyPos.x = 0;
                }
                break;
            }
        case MoveLeft:
            {
                m_MyPos.y--;
                if(m_MyPos.y == -1)
                {
                    m_MyPos.y = MaxMapColumn - 1;
                }
                break;
            }
        case MoveRight:
            {
                m_MyPos.y++;
                if(m_MyPos.y == MaxMapColumn)
                {
                    m_MyPos.y = 0;
                }
                break;
            }
        }
    }

    // 根据给定的方向，更新状态并返回更新结果
    UpdateRet Update(MoveDirection direct)
    {
        // 采取direct表示的方向，更新地图及其他状态
        m_MoveList.push_back(direct); // 记录动作
        bool bLooped = false; // 为真表示已循环过一次
        XPos originalPos = m_MyPos; // 采取动作之前的原地址，用于判断死循环
        XPos oldPos = m_MyPos; // 移动一个位置之前的原地址，用于取消一步位移
        NextStep(m_MyPos,direct); // 朝指定方向移动一步
        while(1)
        {
            if(m_MyPos == originalPos)
            {
                if(bLooped) // 2次到达原位置，判别为死循环
                {
                    return UPRET_DEAD_LOOP;
                }
                else // 仅1次到达原位置，不能判别为死循环
                {
                    bLooped = true;
                }
            }
            // 空白，不存在任何障碍物
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellEmpty )
            {
                oldPos = m_MyPos;
                NextStep(m_MyPos,direct);
                continue;
            }
            // 正常的水珠，雨滴可以吃
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellNormalPearl )
            {
                SET_EMPTY_AND_CHECK_SUCC(m_MyPos.x,m_MyPos.y);
                oldPos = m_MyPos;
                NextStep(m_MyPos,direct);
                continue;
            }
            //黄色的水珠，雨滴吃过以后可以通过黄色的墙壁
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellYellowPearl)
            {
                if(m_bGold == false)
                {
                    m_bGold = true;
                }
                SET_EMPTY_AND_CHECK_SUCC(m_MyPos.x,m_MyPos.y);
                oldPos = m_MyPos;
                NextStep(m_MyPos,direct);
                continue;
            }
            // 黄色墙壁
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellYellowWall)
            {
                if(m_bGold == true)
                {
                    SET_EMPTY_AND_CHECK_SUCC(m_MyPos.x,m_MyPos.y);
                    m_bGold = false;
                    oldPos = m_MyPos;
                    NextStep(m_MyPos,direct);
                    continue;
                }
                else 
                {
                    m_MyPos = oldPos;
                    return UPRET_NORMAL;
                }
            }
            // 开门
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellOpenGate)
            {
                m_Map[m_MyPos.x][m_MyPos.y] = CellNormalWall;
                oldPos = m_MyPos;
                NextStep(m_MyPos,direct);
                continue;
            }
            // 普通墙壁
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellNormalWall)
            {
                m_MyPos = oldPos;
                return UPRET_NORMAL;
            }
            // 其他情况，当做空白
            XASSERT(false);
            oldPos = m_MyPos;
            NextStep(m_MyPos,direct);
            continue;
        }
    }

    // 地图上是否没有水珠了
    bool IsNoDrop()const
    {
        return m_cntPearl==0;
    }

    // 将地图(x,y)设置为空，并返回是否成功消除所有水珠
    bool SetEmpty(int x,int y)
    {
        if(m_Map[x][y] == CellNormalPearl || m_Map[x][y] == CellYellowPearl)
        {
            --m_cntPearl;
        }
        m_Map[x][y] = CellEmpty;
        return IsNoDrop();
    }

    // 将地图(x,y)设置为空，并返回是否成功消除所有水珠
    bool SetEmpty(const XPos &pos)
    {
        return SetEmpty(pos.x,pos.y);
    }

    // 判断是否为不可能完成的状态
    bool IsImpossible()const
    {
        // TO WRITE
        return false;
    }
    // 标记状态已死亡
    void SetDead() 
    {
        m_bDead = true;
    }

    // 判断是否为已死亡状态
    bool IsDeadState()
    {
        return m_bDead;
    }

    // 查找雨滴的位置
    void FindMySelf()
    {
        int i,j;
        for(i=0;i<MaxMapRow;++i)
        {
            for(j=0;j<MaxMapColumn;++j)
            {
                if(m_Map[i][j]==CellRainDrop)
                {
                    m_MyPos.x = i;
                    m_MyPos.y = j;
                    SetEmpty(m_MyPos);
                }
            }
        }
    }

    CellState m_Map[MaxMapRow][MaxMapColumn]; // 地图状态
    bool m_bGold; // 雨滴是否为金色状态
    bool m_bDead; // 是否为死亡节点
    XPos m_MyPos; // 雨滴的位置
    int m_cntPearl; // 水珠的个数
    list<MoveDirection> m_MoveList; // 操作队列
};

MoveDirection g_LegalAct[] = {MoveLeft,MoveUp,MoveRight,MoveDown};
#define MAX_ACTION 4

// 规格化地图，将超出地图大小的部分全部设置为空，之后即可把地图的大小当成MaxMapRow*MaxMapColumn
void RegulateMap(CellState tar_map[MaxMapRow][MaxMapColumn],int iRow,int iCol)
{
    int i,j;
    for(i=iRow;i<MaxMapRow;++i)
    {
        for(j=0;j<MaxMapColumn;++j)
        {
            tar_map[i][j] = CellEmpty;
        }
    }
    for(j=iCol;j<MaxMapColumn;++j)
    {
        for(i=0;i<MaxMapRow;++i)
        {
            tar_map[i][j] = CellEmpty;
        }
    }
}

// 搜索XState vector中是否存在相同的记录
bool ExistState(const vector<XState>&state_vec,const XState &state)
{
    size_t i;
    for(i=0;i<state_vec.size();++i)
    {
        if(state_vec[i].IsEqual(state))
        {
            return true;
        }
    }
    return false;
}

// 复制地图
void CopyMap(CellState tar[MaxMapRow][MaxMapColumn], CellState src[MaxMapRow][MaxMapColumn])
{
    int i,j;
    for(i=0;i<MaxMapRow;++i)
    {
        for(j=0;j<MaxMapColumn;++j)
        {
            tar[i][j] = src[i][j];
        }
    }
}

PlayerLJX::PlayerLJX()
{

}

PlayerLJX::~PlayerLJX()
{

}

std::list<MoveDirection> PlayerLJX::solutionToMap( CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount )
{
    list<MoveDirection> moveList;
    vector<XState> stateVec; // 状态队列
    CellState regulated_map[MaxMapRow][MaxMapColumn];
    CopyMap(regulated_map,quellReflectMap);
    RegulateMap(regulated_map,rowCount,columnCount); // 规格化地图
    stateVec.push_back(XState(regulated_map)); 	// 起始状态入队
    size_t index;
    for(index = 0; index<stateVec.size(); ++index)
    {
        XState curState = stateVec[index];
        // 如果状态为死节点，continue跳过
        if(curState.IsDeadState())
        {
            continue;
        }
        XState newState(curState);
        MoveDirection curDirection;
        int i;
        for(i=0;i<MAX_ACTION;++i)
        {
            newState = curState;
            curDirection = g_LegalAct[i];
            // 更新当前状态
            UpdateRet ret = newState.Update(curDirection);
            if(ret == UPRET_SUCC)// 如果吃完水珠，结束，返回结果
            {
                newState.GetActions(moveList);
                return moveList;
            }
            else if(ret == UPRET_DEAD_LOOP)// 如果死循环，节点死亡，continue
            {
                continue;
            }
            // 否则为NORMAL

            if(ExistState(stateVec,newState))
            {
                //不管节点是否死亡，无须记录，continue
                continue;
            }
            // 判断此次状态是否为不可完成状态，如果是则标记死亡，但是依然需要记录状态
            if(newState.IsImpossible())
            {
                newState.SetDead();
            }
            // 记录状态
            stateVec.push_back(newState);
        }
    }
    XASSERT(false);
    return moveList;
}

std::string PlayerLJX::teamNameToDisplay()
{
    return "LJX";
}