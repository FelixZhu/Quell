#ifndef _PLAYERLJX_
#define _PLAYERLJX_

#include "Player.h"

class PlayerLJX : public Player
{
public:
    PlayerLJX();

    virtual ~PlayerLJX();

    std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

    std::string teamNameToDisplay();
};

#endif