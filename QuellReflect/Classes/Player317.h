#ifndef _PLAYER317_
#define _PLAYER317_

#include "Player.h"

class Player317 : public Player
{
public:
    Player317();

    virtual ~Player317();

    std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

    std::string teamNameToDisplay();
};

#endif