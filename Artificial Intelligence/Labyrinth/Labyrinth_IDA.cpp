#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include <ctime>
#define WIDTH 60   //25
#define HEIGHT 30   //18

enum direction{left,right,up,down,none};

struct node             //搜索树节点
{
    std::pair<int,int> pos;
    int g;
    int h;
    direction dir;
};

bool labyrinth[HEIGHT + 2][WIDTH + 2];
std::pair<int,int> goal;
std::set<std::pair<std::pair<int,int>,int> > positions;
std::stack<direction> path;

int calculateH(node *nd)                    //计算启发式函数的值，这里采用的是曼哈顿距离
{
    return abs(nd->pos.first - goal.first) + abs(nd->pos.second - goal.second);
}

node* operate(direction dir,node *nodeToExtend)  //对待扩展的节点进行操作，向dir方向移动
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

bool search(int threshold,node *currState)           //搜索函数，threshold为阈值
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
            if(search(threshold,nextState[i]))
            {
                path.push(nextState[i]->dir);
                return true;
            }
            else delete nextState[i];
        }
    }
    return false;
}

int main()
{
    for(int i = 0; i <= HEIGHT + 1; i++)
        for(int j = 0; j <= WIDTH + 1; j++)
        {
            if(i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1)labyrinth[i][j] = true;
            else std::cin>>labyrinth[i][j];
        }
    goal = std::make_pair(29,59);       //17,25     29,59
    node *startNode = new node;
    startNode->pos = std::make_pair(2,2);   //2,1       2,2
    startNode->g = 0;
    startNode->h = calculateH(startNode);
    startNode->dir = none;              //生成初始状态节点
    positions.insert(std::make_pair(startNode->pos,0));
    time_t t1 = time(0);
    int threshold = startNode->h;
    while(!search(threshold,startNode))
    {
        threshold += 2;
        positions.clear();
        positions.insert(std::make_pair(startNode->pos,0));
    }
    std::cout<<std::endl<<path.size()<<std::endl;
    while(!path.empty())
    {
        switch(path.top())
        {
        case left: std::cout<<'L';break;
        case right: std::cout<<'R';break;
        case up: std::cout<<'U';break;
        case down: std::cout<<'D';break;
        case none: break;
        }
        path.pop();
    }
    std::cout<<std::endl;
    time_t t2 = time(0);
    std::cout<<t2 - t1<<std::endl;
    return 0;
}
