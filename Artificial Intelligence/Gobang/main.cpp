#include <iostream>
#include "gobang.h"
#include <fstream>

int main()
{
    Gobang *gobang = new Gobang;        //必须要申请堆空间，否则栈大小不够会导致段错误，这个bug找了好久
    std::ofstream outfile("output.txt");
    outfile<<"AI\t\tME\n";

    std::cout<<"ai:8,8\n";
    gobang->placeStone(std::make_pair(7,7),ai);
    gobang->displayBoard();
    outfile<<"[8,8]\t";

    int x,y;
    do
    {
        std::cout<<"hum:";
        std::cin>>x>>y;
        if(!gobang->placeStone(std::make_pair(x - 1,y - 1),hum))continue;
        gobang->displayBoard();
        outfile<<'['<<x<<','<<y<<']'<<std::endl;
        if(gobang->isGameOver() == hum)
        {
            std::cout<<"Hum wins!\n";
            outfile<<"Hum wins!\n";
            break;
        }
        gobang->alphaBetaSearch(2);
        std::cout<<"ai:"<<gobang->nextStepOfAi.first + 1<<','<<gobang->nextStepOfAi.second + 1<<'\n';
        gobang->placeStone(gobang->nextStepOfAi,ai);
        gobang->displayBoard();
        outfile<<'['<<gobang->nextStepOfAi.first + 1<<','<<gobang->nextStepOfAi.second + 1<<"]\t";
        if(gobang->isGameOver() == ai)
        {
            std::cout<<"Ai wins!\n";
            outfile<<"\nAi wins!\n";
            break;
        }
    }while(true);
    outfile.close();
    return 0;
}
