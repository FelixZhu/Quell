#include "PlayerLJX.h"

#include <vector>

using namespace std;

#define XASSERT(x)  (x)

class XState;
// ���Ƶ�ͼ
void CopyMap(CellState tar[MaxMapRow][MaxMapColumn], CellState src[MaxMapRow][MaxMapColumn]);
// ����XState vector���Ƿ������ͬ�ļ�¼
bool ExistState(const vector<XState>&state_vec,const XState &state);
// ��񻯵�ͼ����������ͼ��С�Ĳ���ȫ������Ϊ�գ�֮�󼴿ɰѵ�ͼ�Ĵ�С����MaxMapRow*MaxMapColumn
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
        SetMap(reflect_map);  // ���Ƶ�ͼ����Ա����
        CountPearl(); // ����ˮ�����(���������)
        FindMySelf(); // ���Ҳ����ñ������꣬ͬʱ������α����ڵ�ͼ�еļ�¼
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

    // �б�״̬�Ƿ����
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

    // ��ȡ��������
    void GetActions(list<MoveDirection> &actions)const
    {
        actions = m_MoveList;
    }

    // ���ݷ������һ��λ�ƣ�������߽�����
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

    // ���ݸ����ķ��򣬸���״̬�����ظ��½��
    UpdateRet Update(MoveDirection direct)
    {
        // ��ȡdirect��ʾ�ķ��򣬸��µ�ͼ������״̬
        m_MoveList.push_back(direct); // ��¼����
        bool bLooped = false; // Ϊ���ʾ��ѭ����һ��
        XPos originalPos = m_MyPos; // ��ȡ����֮ǰ��ԭ��ַ�������ж���ѭ��
        XPos oldPos = m_MyPos; // �ƶ�һ��λ��֮ǰ��ԭ��ַ������ȡ��һ��λ��
        NextStep(m_MyPos,direct); // ��ָ�������ƶ�һ��
        while(1)
        {
            if(m_MyPos == originalPos)
            {
                if(bLooped) // 2�ε���ԭλ�ã��б�Ϊ��ѭ��
                {
                    return UPRET_DEAD_LOOP;
                }
                else // ��1�ε���ԭλ�ã������б�Ϊ��ѭ��
                {
                    bLooped = true;
                }
            }
            // �հף��������κ��ϰ���
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellEmpty )
            {
                oldPos = m_MyPos;
                NextStep(m_MyPos,direct);
                continue;
            }
            // ������ˮ�飬��ο��Գ�
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellNormalPearl )
            {
                SET_EMPTY_AND_CHECK_SUCC(m_MyPos.x,m_MyPos.y);
                oldPos = m_MyPos;
                NextStep(m_MyPos,direct);
                continue;
            }
            //��ɫ��ˮ�飬��γԹ��Ժ����ͨ����ɫ��ǽ��
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
            // ��ɫǽ��
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
            // ����
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellOpenGate)
            {
                m_Map[m_MyPos.x][m_MyPos.y] = CellNormalWall;
                oldPos = m_MyPos;
                NextStep(m_MyPos,direct);
                continue;
            }
            // ��ͨǽ��
            if(m_Map[m_MyPos.x][m_MyPos.y] == CellNormalWall)
            {
                m_MyPos = oldPos;
                return UPRET_NORMAL;
            }
            // ��������������հ�
            XASSERT(false);
            oldPos = m_MyPos;
            NextStep(m_MyPos,direct);
            continue;
        }
    }

    // ��ͼ���Ƿ�û��ˮ����
    bool IsNoDrop()const
    {
        return m_cntPearl==0;
    }

    // ����ͼ(x,y)����Ϊ�գ��������Ƿ�ɹ���������ˮ��
    bool SetEmpty(int x,int y)
    {
        if(m_Map[x][y] == CellNormalPearl || m_Map[x][y] == CellYellowPearl)
        {
            --m_cntPearl;
        }
        m_Map[x][y] = CellEmpty;
        return IsNoDrop();
    }

    // ����ͼ(x,y)����Ϊ�գ��������Ƿ�ɹ���������ˮ��
    bool SetEmpty(const XPos &pos)
    {
        return SetEmpty(pos.x,pos.y);
    }

    // �ж��Ƿ�Ϊ��������ɵ�״̬
    bool IsImpossible()const
    {
        // TO WRITE
        return false;
    }
    // ���״̬������
    void SetDead() 
    {
        m_bDead = true;
    }

    // �ж��Ƿ�Ϊ������״̬
    bool IsDeadState()
    {
        return m_bDead;
    }

    // ������ε�λ��
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

    CellState m_Map[MaxMapRow][MaxMapColumn]; // ��ͼ״̬
    bool m_bGold; // ����Ƿ�Ϊ��ɫ״̬
    bool m_bDead; // �Ƿ�Ϊ�����ڵ�
    XPos m_MyPos; // ��ε�λ��
    int m_cntPearl; // ˮ��ĸ���
    list<MoveDirection> m_MoveList; // ��������
};

MoveDirection g_LegalAct[] = {MoveLeft,MoveUp,MoveRight,MoveDown};
#define MAX_ACTION 4

// ��񻯵�ͼ����������ͼ��С�Ĳ���ȫ������Ϊ�գ�֮�󼴿ɰѵ�ͼ�Ĵ�С����MaxMapRow*MaxMapColumn
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

// ����XState vector���Ƿ������ͬ�ļ�¼
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

// ���Ƶ�ͼ
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
    vector<XState> stateVec; // ״̬����
    CellState regulated_map[MaxMapRow][MaxMapColumn];
    CopyMap(regulated_map,quellReflectMap);
    RegulateMap(regulated_map,rowCount,columnCount); // ��񻯵�ͼ
    stateVec.push_back(XState(regulated_map)); 	// ��ʼ״̬���
    size_t index;
    for(index = 0; index<stateVec.size(); ++index)
    {
        XState curState = stateVec[index];
        // ���״̬Ϊ���ڵ㣬continue����
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
            // ���µ�ǰ״̬
            UpdateRet ret = newState.Update(curDirection);
            if(ret == UPRET_SUCC)// �������ˮ�飬���������ؽ��
            {
                newState.GetActions(moveList);
                return moveList;
            }
            else if(ret == UPRET_DEAD_LOOP)// �����ѭ�����ڵ�������continue
            {
                continue;
            }
            // ����ΪNORMAL

            if(ExistState(stateVec,newState))
            {
                //���ܽڵ��Ƿ������������¼��continue
                continue;
            }
            // �жϴ˴�״̬�Ƿ�Ϊ�������״̬�����������������������Ȼ��Ҫ��¼״̬
            if(newState.IsImpossible())
            {
                newState.SetDead();
            }
            // ��¼״̬
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