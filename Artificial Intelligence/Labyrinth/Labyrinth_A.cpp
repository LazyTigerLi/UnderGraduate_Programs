#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include <ctime>
#define WIDTH 25
#define HEIGHT 18

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
            //向左是墙，或者上一步就是向右且不是死胡同
            newNode = new node;
            newNode->pos = std::make_pair(x,y - 1);
            newNode->dir = left;
            break;
        case right:
            if(labyrinth[x][y + 1] || (lastDir == left 
                && !(labyrinth[x][y - 1] && labyrinth[x + 1][y] && labyrinth[x - 1][y])))return;
            newNode = new node;
            newNode->pos = std::make_pair(x,y + 1);
            newNode->dir = right;
            break;
        case up:
            if(labyrinth[x - 1][y] || (lastDir == down 
                && !(labyrinth[x + 1][y] && labyrinth[x][y - 1] && labyrinth[x][y + 1])))return;
            newNode = new node;
            newNode->pos = std::make_pair(x - 1,y);
            newNode->dir = up;
            break;
        case down:
            if(labyrinth[x + 1][y] || (lastDir == up
                && !(labyrinth[x - 1][y] && labyrinth[x][y - 1] && labyrinth[x][y + 1])))return;
            newNode = new node;
            newNode->pos = std::make_pair(x + 1,y);
            newNode->dir = down;
            break;
    }
    newNode->g = nodeToExtend->g + 1;
    newNode->h = calculateH(newNode);
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
    goal = std::make_pair(17,25);
    node *startNode = new node;
    startNode->pos = std::make_pair(2,1);
    startNode->g = 0;
    startNode->h = calculateH(startNode);
    startNode->last = nullptr;
    startNode->dir = none;              //生成初始状态节点
    edge.push(startNode);
    time_t t1 = time(0);
    search();
    time_t t2 = time(0);
    std::cout<<t2 - t1<<std::endl;
    return 0;
}
