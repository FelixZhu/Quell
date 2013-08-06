#ifndef _PLAYER000_
#define _PLAYER000_

#include "Player.h"

class PlayerOOO : public Player
{
public:
    PlayerOOO();

    virtual ~PlayerOOO();

    std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

    std::string teamNameToDisplay();
};

#endif