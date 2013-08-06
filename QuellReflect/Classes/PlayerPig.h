#ifndef _PLAYERPIG_
#define _PLAYERPIG_

#include "Player.h"

class PlayerPig : public Player
{
public:
    PlayerPig();

    virtual ~PlayerPig();

    std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

    std::string teamNameToDisplay();
};

#endif