#include "gobang.h"

Role Gobang::isGameOver(std::pair<int, int> pos)
{
    int x = pos.first,y = pos.second;
    Role role = board[x][y];
    int numOfMyChessmen = 1;
    for(int i = y - 1; i >= 0; i--)
    {
        if(board[x][i] == role)numOfMyChessmen++;
        else break;
    }
    for(int i = y + 1; i < 15; i++)
    {
        if(board[x][i] == role)numOfMyChessmen++;
        else break;
    }
    if(numOfMyChessmen >= 5)return role;

    numOfMyChessmen = 1;
    for(int i = x - 1; i >= 0; i--)
    {
        if(board[i][y] == role)numOfMyChessmen++;
        else break;
    }
    for(int i = x + 1; i < 15; i++)
    {
        if(board[i][y] == role)numOfMyChessmen++;
        else break;
    }
    if(numOfMyChessmen >= 5)return role;

    numOfMyChessmen = 1;
    for(int i = x - 1,j = y - 1; i >= 0 && j >= 0; i--,j--)
    {
        if(board[i][j] == role)numOfMyChessmen++;
        else break;
    }
    for(int i = x + 1,j = y + 1; i < 15 && j < 15; i++,j++)
    {
        if(board[i][j] == role)numOfMyChessmen++;
        else break;
    }
    if(numOfMyChessmen >= 5)return role;

    numOfMyChessmen = 1;
    for(int i = x - 1,j = y + 1; i >= 0 && j < 15; i--,j++)
    {
        if(board[i][j] == role)numOfMyChessmen++;
        else break;
    }
    for(int i = x + 1,j = y - 1; i < 15 && j >= 0; i++,j--)
    {
        if(board[i][j] == role)numOfMyChessmen++;
        else break;
    }
    if(numOfMyChessmen >= 5)return role;

    return empty;
}

void Gobang::updateEvaluation(std::pair<int, int> pos)
{
    int x = pos.first,y = pos.second;
    //落子最多导致某个方向上的8个棋子受到影响，如|XXXOXXXX|和|XXXOXXXX的区别
    for(int i = 0; i <= 8; i++)
    {
        if(y - i >= 0)
        {
            evaluateOfPoint(std::make_pair(x,y - i),ai,horizontal);
            evaluateOfPoint(std::make_pair(x,y - i),hum,horizontal);
        }
        if(y + i < 15)
        {
            evaluateOfPoint(std::make_pair(x,y + i),ai,horizontal);
            evaluateOfPoint(std::make_pair(x,y + i),hum,horizontal);
        }
        if(x - i >= 0)
        {
            evaluateOfPoint(std::make_pair(x - i,y),ai,vertical);
            evaluateOfPoint(std::make_pair(x - i,y),hum,vertical);
        }
        if(x + i < 15)
        {
            evaluateOfPoint(std::make_pair(x + i,y),ai,vertical);
            evaluateOfPoint(std::make_pair(x + i,y),hum,vertical);
        }
        if(x - i >= 0 && y - i < 15)
        {
            evaluateOfPoint(std::make_pair(x - i,y - i),ai,diagonal1);
            evaluateOfPoint(std::make_pair(x - i,y - i),hum,diagonal1);
        }
        if(x + i < 15 && y + i < 15)
        {
            evaluateOfPoint(std::make_pair(x + i,y + i),ai,diagonal1);
            evaluateOfPoint(std::make_pair(x + i,y + i),hum,diagonal1);
        }
        if(x - i >= 0 && y + i < 15)
        {
            evaluateOfPoint(std::make_pair(x - i,y + i),ai,diagonal2);
            evaluateOfPoint(std::make_pair(x - i,y + i),hum,diagonal2);
        }
        if(x + i < 15 && y - i >= 0)
        {
            evaluateOfPoint(std::make_pair(x + i,y - i),ai,diagonal2);
            evaluateOfPoint(std::make_pair(x + i,y - i),hum,diagonal2);
        }
    }
}

Role Gobang::placeStone(std::pair<int, int> pos,Role role)
{
    int x = pos.first,y = pos.second;
    board[x][y] = role;
    numOfChessman++;
    updateEvaluation(pos);
    return isGameOver(pos);         //每次落子之后应当判断游戏是否结束
}

void Gobang::removeStone(std::pair<int, int> pos)
{
    int x = pos.first,y = pos.second;
    board[x][y] = empty;
    numOfChessman--;
    updateEvaluation(pos);
}
