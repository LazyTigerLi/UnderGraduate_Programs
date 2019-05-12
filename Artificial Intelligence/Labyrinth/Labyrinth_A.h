#ifndef LABRINTH_A_H
#define LABRINTH_A_H

#include <set>
#include <queue>
#include <vector>
#include <stack>
#include <cmath>
#include <sys/time.h>

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

class Labyrinth
{

public:

    Labyrinth();
    ~Labyrinth();
    void search();
    void output();

private:
    int height;
    int width;
    int **labyrinth;
    timeval start;
    timeval end;

    std::pair<int,int> goal;
    std::priority_queue<node*,std::vector<node*>,cmp> edge;             //优先队列，用于存储边缘节点
    std::set<std::pair<std::pair<int,int>,int> > positions;
    std::stack<direction> path;             //存放路径

    int calculateH(node *nd);
    void operate(direction dir,node *nodeToExtend);

};

#endif // LABRINTH_A_H
