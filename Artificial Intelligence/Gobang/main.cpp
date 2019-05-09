#include <iostream>
#include "gobang.h"

int main()
{
    Gobang *gobang = new Gobang;        //必须要申请堆空间，否则栈大小不够会导致段错误，这个bug找了好久
    int x,y;
    //gobang.alphaBetaSearch(3);
    //cout<<gobang.nextStep.first<<' '<<gobang.nextStep.second<<endl;
    do
    {
        std::cout<<"hum:";
        std::cin>>x>>y;
        //std::pair<int,int> pos = std::make_pair(x - 1,y - 1);
        if(gobang->placeStone(std::make_pair(x - 1,y - 1),hum) != empty)
        {
            gobang->displayBoard();
            break;
        }
        gobang->displayBoard();
        gobang->alphaBetaSearch(2);
        //auto stepOfAi = gobang.stepsOfAi.top();
        //std::cout<<gobang.stepsOfAi.size()<<'\n';
        std::cout<<"prun "<<gobang->count<<'\n';
        std::cout<<"ai:"<<gobang->nextStepOfAi.first + 1<<','<<gobang->nextStepOfAi.second + 1<<'\n';
        if(gobang->placeStone(gobang->nextStepOfAi,ai) != empty)
        {
            gobang->displayBoard();
            break;
        }
        gobang->displayBoard();
    }while(true);
    return 0;
}
