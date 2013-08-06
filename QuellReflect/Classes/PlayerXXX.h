#ifndef _PLAYERXXX_
#define _PLAYERXXX_

#include "Player.h"

class PlayerXXX : public Player
{
public:
    PlayerXXX();

    virtual ~PlayerXXX();

    std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

    std::string teamNameToDisplay();
};

#endif