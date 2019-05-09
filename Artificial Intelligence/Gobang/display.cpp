#include "gobang.h"
#include <iostream>
void Gobang::displayBoard()
{
    std::cout<<"    ";
    for(int i = 0; i < 15; i++)
    {
        std::cout<<i + 1;
        if(i < 9)std::cout<<"  ";
        else std::cout<<" ";
    }
    std::cout<<'\n';
    for(int i = 0; i < 15; i++)
    {
        std::cout<<i + 1;
        if(i < 9)std::cout<<"   ";
        else std::cout<<"  ";
        for(int j = 0; j < 15; j++)
        {
            if(board[i][j] == hum)
                std::cout<<'X'<<"  ";
            else if(board[i][j] == ai)
                std::cout<<'O'<<"  ";
            else std::cout<<'+'<<"  ";
        }
        std::cout<<'\n';
    }
    std::cout<<'\n';
}
