#include "Labyrinth_IDA.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define WIDTH 60   //25
#define HEIGHT 30   //18

Labyrinth::Labyrinth()
{
    std::cout<<"Please input the width and the height of the labyrinth:";
    std::cin>> width>> height;
    labyrinth = new int *[height + 2];
    for(int i = 0; i < height + 2; i++)
        labyrinth[i] = new int[width + 2];

    std::cout<<"Please input the file name:";
    std::string filename;
    std::cin>>filename;
    std::ifstream infile(filename);

    if(infile.fail()){std::cout<<"fail";return;}

    int i = 1;
    std::string line;
    while (std::getline(infile,line))
    {
        int j = 1;
        std::stringstream ss;
        ss<<line;
        if(!ss.eof())
        {
            while(ss>>labyrinth[i][j])j++;
        }
        i++;
    }
    infile.close();
    for(int i = 0; i <= height + 1; i++)
    {
        labyrinth[i][0] = 1;
        labyrinth[i][width + 1] = 1;
    }
    for(int j = 0; j <= width + 1; j++)
    {
        labyrinth[0][j] = 1;
        labyrinth[height + 1][j] = 1;
    }

    std::cout<<"Please input the start and the end point:";
    int startX,startY,endX,endY;
    std::cin>>startX>>startY>>endX>>endY;                   //以上为数据读取
    goal = std::make_pair(endX + 1,endY + 1);

    startNode = new node;
    startNode->pos = std::make_pair(startX + 1,startY + 1);
    startNode->g = 0;
    startNode->h = calculateH(startNode);
    positions.insert(std::make_pair(startNode->pos,0));
    startNode->dir = none;              //生成初始状态节点
}


int Labyrinth::calculateH(node *nd)                    //计算启发式函数的值，这里采用的是曼哈顿距离
{
    return abs(nd->pos.first - goal.first) + abs(nd->pos.second - goal.second);
}

node* Labyrinth::operate(direction dir,node *nodeToExtend)  //对待扩展的节点进行操作，向dir方向移动
{
    int x = nodeToExtend->pos.first;
    int y = nodeToExtend->pos.second;
    direction lastDir = nodeToExtend->dir;
    node *newNode;
    switch(dir)
    {
        case left:
            if(labyrinth[x][y - 1] || (lastDir == right
                && !(labyrinth[x][y + 1] && labyrinth[x - 1][y] && labyrinth[x + 1][y])))return nullptr;
            //向左是墙，或者上一步就是向右且不是死胡同
            newNode = new node;
            newNode->pos = std::make_pair(x,y - 1);
            newNode->dir = left;
            break;
        case right:
            if(labyrinth[x][y + 1] || (lastDir == left
                && !(labyrinth[x][y - 1] && labyrinth[x + 1][y] && labyrinth[x - 1][y])))return nullptr;
            newNode = new node;
            newNode->pos = std::make_pair(x,y + 1);
            newNode->dir = right;
            break;
        case up:
            if(labyrinth[x - 1][y] || (lastDir == down
                && !(labyrinth[x + 1][y] && labyrinth[x][y - 1] && labyrinth[x][y + 1])))return nullptr;
            newNode = new node;
            newNode->pos = std::make_pair(x - 1,y);
            newNode->dir = up;
            break;
        case down:
            if(labyrinth[x + 1][y] || (lastDir == up
                && !(labyrinth[x - 1][y] && labyrinth[x][y - 1] && labyrinth[x][y + 1])))return nullptr;
            newNode = new node;
            newNode->pos = std::make_pair(x + 1,y);
            newNode->dir = down;
            break;
        case none:
            break;
    }
    newNode->g = nodeToExtend->g + 1;
    for(std::set<std::pair<std::pair<int,int>,int> >::iterator it = positions.begin(); it != positions.end(); it++)
    {
        if((*it).first.first == newNode->pos.first && (*it).first.second == newNode->pos.second)
        {
            if((*it).second <= newNode->g)     //判断条件应当是<=
            {
                delete newNode;
                return nullptr;
            }
            else
            {
                positions.erase(it);
                positions.insert(std::make_pair(newNode->pos,newNode->g));
                return newNode;
            }
        }
    }
    newNode->h = calculateH(newNode);
    positions.insert(std::make_pair(newNode->pos,newNode->g));
    return newNode;
}

bool Labyrinth::dfs(int threshold,node *currState)           //搜索函数，threshold为阈值
{
    if(currState->g + currState->h > threshold)return false;        //当前节点的f值超过阈值

    if(currState->pos.first == goal.first && currState->pos.second == goal.second)return true;

    node *nextState[4];
    direction dir[4] = {left,right,up,down};
    for(int i = 0; i < 4; i++)
    {
        nextState[i] = operate(dir[i],currState);
        if(nextState[i])
        {
            if(dfs(threshold,nextState[i]))
            {
                path.push(nextState[i]->dir);
                return true;
            }
            else delete nextState[i];
        }
    }
    return false;
}

void Labyrinth::search()
{
    gettimeofday(&start,nullptr);
    int threshold = startNode->h;
    while(true)
    {
        if(dfs(threshold,startNode))
        {
            gettimeofday(&end,nullptr);
            return;
        }
        threshold += 2;
        positions.clear();
        positions.insert(std::make_pair(startNode->pos,0));
    }
}

void Labyrinth::output()
{
    std::ofstream outfile("output.txt");
    outfile<<end.tv_sec - start.tv_sec + (double)(end.tv_usec - start.tv_usec) / 1000000;
    outfile<<std::endl;
    int size = path.size();
    while(!path.empty())
    {
        switch(path.top())
        {
        case left: outfile<<'L';break;
        case right: outfile<<'R';break;
        case up: outfile<<'U';break;
        case down: outfile<<'D';break;
        case none: break;
        }
        path.pop();
    }
    outfile<<std::endl;
    outfile<<size;
    outfile.close();
}


int main()
{
    Labyrinth *puzzle = new Labyrinth;
    puzzle->search();
    puzzle->output();
    return 0;
}
