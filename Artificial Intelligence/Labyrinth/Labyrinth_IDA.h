#ifndef LABYRINTH_IDA_H
#define LABYRINTH_IDA_H

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
    direction dir;
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
    node *startNode;
    timeval start;
    timeval end;
    std::pair<int,int> goal;
    std::set<std::pair<std::pair<int,int>,int> > positions;
    std::stack<direction> path;             //存放路径

    bool dfs(int threshold,node *currState);
    int calculateH(node *nd);
    node* operate(direction dir,node *nodeToExtend);
};

#endif // LABYRINTH_IDA_H
