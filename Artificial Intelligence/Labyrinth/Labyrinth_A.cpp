#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include <ctime>
#define WIDTH 60    //25
#define HEIGHT 30   //18

enum direction{left,right,up,down,none};

struct node             //搜索树节点
{
    std::pair<int,int> pos;
    int g;
    int h;
    node *last;
    direction dir;
};

class cmp                               //优先队列的比较函数
{
    public:
        bool operator()(const node *p1,const node *p2)
        {
            if(p1->g + p1->h > p2->g + p2->h)return true;
            else return false;
        }
};


bool labyrinth[HEIGHT + 2][WIDTH + 2];
std::pair<int,int> goal;
std::priority_queue<node*,std::vector<node*>,cmp> edge;             //优先队列，用于存储边缘节点
std::set<std::pair<std::pair<int,int>,int> > positions;

int calculateH(node *nd)                    //计算启发式函数的值，这里采用的是曼哈顿距离
{
    return abs(nd->pos.first - goal.first) + abs(nd->pos.second - goal.second);
}

void operate(direction dir,node *nodeToExtend)  //对待扩展的节点进行操作，向dir方向移动
{
    int x = nodeToExtend->pos.first;
    int y = nodeToExtend->pos.second;
    direction lastDir = nodeToExtend->dir;
    node *newNode;
    switch(dir)
    {
        case left:
            if(labyrinth[x][y - 1] || (lastDir == right 
                && !(labyrinth[x][y + 1] && labyrinth[x - 1][y] && labyrinth[x + 1][y])))return;
            //if(positions.find(std::make_pair(x,y - 1)) != positions.end())return;
            //else positions.insert(std::make_pair(x,y - 1));
            //向左是墙，或者上一步就是向右且不是死胡同
            newNode = new node;
            newNode->pos = std::make_pair(x,y - 1);
            newNode->dir = left;
            break;
        case right:
            if(labyrinth[x][y + 1] || (lastDir == left 
                && !(labyrinth[x][y - 1] && labyrinth[x + 1][y] && labyrinth[x - 1][y])))return;
            //if(positions.find(std::make_pair(x,y + 1)) != positions.end())return;
            //else positions.insert(std::make_pair(x,y + 1));
            newNode = new node;
            newNode->pos = std::make_pair(x,y + 1);
            newNode->dir = right;
            break;
        case up:
            if(labyrinth[x - 1][y] || (lastDir == down 
                && !(labyrinth[x + 1][y] && labyrinth[x][y - 1] && labyrinth[x][y + 1])))return;
            //if(positions.find(std::make_pair(x - 1,y)) != positions.end())return;
            //else positions.insert(std::make_pair(x - 1,y));
            newNode = new node;
            newNode->pos = std::make_pair(x - 1,y);
            newNode->dir = up;
            break;
        case down:
            if(labyrinth[x + 1][y] || (lastDir == up
                && !(labyrinth[x - 1][y] && labyrinth[x][y - 1] && labyrinth[x][y + 1])))return;
            //if(positions.find(std::make_pair(x + 1,y)) != positions.end())return;
            //else positions.insert(std::make_pair(x + 1,y));
            newNode = new node;
            newNode->pos = std::make_pair(x + 1,y);
            newNode->dir = down;
            break;
        case none:
            break;
    }
    newNode->g = nodeToExtend->g + 1;
    newNode->h = calculateH(newNode);
    for(std::set<std::pair<std::pair<int,int>,int> >::iterator it = positions.begin(); it != positions.end(); it++)
    {
        if((*it).first.first == newNode->pos.first && (*it).first.second == newNode->pos.second)
        {
            if((*it).second <= newNode->g)     //判断条件应当是<=
                delete newNode;
            else
            {
                positions.erase(it);
                positions.insert(std::make_pair(newNode->pos,newNode->g));
                newNode->last = nodeToExtend;
                edge.push(newNode);
            }
            return;
        }
    }
    positions.insert(std::make_pair(newNode->pos,newNode->g));
    newNode->last = nodeToExtend;
    edge.push(newNode);
}

void search()           //搜索函数
{
    while(!edge.empty())
    {
        node *nodeToExtend = edge.top();
        edge.pop();
        
        if(nodeToExtend->pos.first == goal.first && nodeToExtend->pos.second == goal.second)    //到达目标状态，输出路径
        {
            std::stack<direction> path;
            while(nodeToExtend->last)
            {
                path.push(nodeToExtend->dir);
                nodeToExtend = nodeToExtend->last;
            }
            int steps = path.size();
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
            std::cout<<std::endl<<steps<<std::endl;
            return;
        }
        operate(left,nodeToExtend);
        operate(right,nodeToExtend);
        operate(up,nodeToExtend);
        operate(down,nodeToExtend);
        //delete nodeToExtend;
    }
}

int main()
{
    for(int i = 0; i <= HEIGHT + 1; i++)
        for(int j = 0; j <= WIDTH + 1; j++)
        {
            if(i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1)labyrinth[i][j] = true;
            else std::cin>>labyrinth[i][j];
        }
    goal = std::make_pair(29,59);       //17,25
    node *startNode = new node;
    startNode->pos = std::make_pair(2,2);   //2,1
    startNode->g = 0;
    startNode->h = calculateH(startNode);
    positions.insert(std::make_pair(startNode->pos,0));
    startNode->last = nullptr;
    startNode->dir = none;              //生成初始状态节点
    edge.push(startNode);
    time_t t1 = time(0);
    search();
    time_t t2 = time(0);
    std::cout<<t2 - t1<<std::endl;
    return 0;
}
