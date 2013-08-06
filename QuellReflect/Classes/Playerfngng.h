#ifndef _PLAYERFNGNG_
#define _PLAYERFNGNG_

#include "Player.h"

class Playerfngng : public Player
{
public:
    Playerfngng();

    virtual ~Playerfngng();

    std::list<MoveDirection> solutionToMap(CellState quellReflectMap[MaxMapRow][MaxMapColumn], int rowCount, int columnCount);

    std::string teamNameToDisplay();
};

#endif