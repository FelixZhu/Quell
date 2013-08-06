#ifndef _PLAYERBIRD_
#define _PLAYERBIRD_

#include "Player.h"

class PlayerBird : public Player
{
public:
    PlayerBird();

    virtual ~PlayerBird();

    std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

    std::string teamNameToDisplay();
};

#endif