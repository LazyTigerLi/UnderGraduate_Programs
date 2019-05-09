#include "gobang.h"

Gobang::Gobang()
{
    numOfChessman = 0;
    top = true;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            board[i][j] = empty;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            for(int k = 0; k < 4; k++)
            {
                evaluationOfPoints[ai][i][j][k] = 0;
                evaluationOfPoints[hum][i][j][k] = 0;
            }
}

Gobang::~Gobang()
{}
