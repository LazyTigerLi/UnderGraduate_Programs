#include <iostream>
#include <stdlib.h>
#include <queue>
#include <vector>

enum direction{left,right,up,down,none};
int goalState[3][3] = {{1,2,3},{4,0,5},{6,7,8}};
int h1[3][3][9] = {{{0,0,2,3,2,4,3,4,5},{0,2,0,2,3,3,4,3,4},{0,3,2,0,4,2,5,4,3}},
{{0,2,3,4,0,3,2,3,4},{0,3,2,3,2,2,3,2,3},{0,4,3,2,3,0,4,3,2}},
{{0,3,4,5,2,4,0,2,3},{0,4,3,4,3,3,2,0,2},{0,5,4,3,4,2,3,2,0}}
};          //suboptimal

int h2[3][3][9] = {{{0,0,1,2,1,3,2,3,4},{0,1,0,1,2,2,3,2,3},{0,2,1,0,3,1,4,3,2}},
{{0,1,2,3,0,2,1,2,3},{0,2,1,2,1,1,2,1,2},{0,3,2,1,2,0,3,2,1}},
{{0,2,3,4,1,3,0,1,2},{0,3,2,3,2,2,1,0,1},{0,4,3,2,3,1,2,1,0}}
};          //optimal

struct node
{
    int state[3][3];
    int g;
    int h;
    std::vector<direction> path;
};

class cmp
{
    public:
        bool operator()(const node *p1,const node *p2)
        {
            if(p1->g + p1->h > p2->g + p2->h)return true;
            else return false;
        }
};

std::priority_queue<node*,std::vector<node*>,cmp> edge;

int calculateH(int state[][3])
{
    int h = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            h += h2[i][j][state[i][j]];
    return h;
}

void operate(direction dir,int x,int y,node *nodeToExtend)
{
    struct node *newNode = new node;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            newNode->state[i][j] = nodeToExtend->state[i][j];
    direction lastDir = nodeToExtend->path[nodeToExtend->path.size() - 1];
    switch(dir)
    {
        case left:
            if(y == 2 || lastDir == right)
            {
                delete newNode;
                return;
            }
            newNode->state[x][y] = nodeToExtend->state[x][y + 1];
            newNode->state[x][y + 1] = 0;
            newNode->path.assign(nodeToExtend->path.begin(),nodeToExtend->path.end());
            newNode->path.push_back(left);
            break;
        case right:
            if(y == 0 || lastDir == left)
            {
                delete newNode;
                return;
            }
            newNode->state[x][y] = nodeToExtend->state[x][y - 1];
            newNode->state[x][y - 1] = 0;
            newNode->path.assign(nodeToExtend->path.begin(),nodeToExtend->path.end());
            newNode->path.push_back(right);
            break;
        case up:
            if(x == 2 || lastDir == down)
            {
                delete newNode;
                return;
            }
            newNode->state[x][y] = nodeToExtend->state[x + 1][y];
            newNode->state[x + 1][y] = 0;
            newNode->path.assign(nodeToExtend->path.begin(),nodeToExtend->path.end());
            newNode->path.push_back(up);
            break;
        case down:
            if(x == 0 || lastDir == up)
            {
                delete newNode;
                return;
            }
            newNode->state[x][y] = nodeToExtend->state[x - 1][y];
            newNode->state[x - 1][y] = 0;
            newNode->path.assign(nodeToExtend->path.begin(),nodeToExtend->path.end());
            newNode->path.push_back(down);
            break;
        case none:
            return;
    }
    int h = calculateH(newNode->state);
    newNode->g = nodeToExtend->g + 1;
    newNode->h = h;
    edge.push(newNode);
}

void search()
{
    while(!edge.empty())
    {
        node *nodeToExtend = edge.top();
        edge.pop();
        //std::cout<<edge.size()<<'\t';
        //if(path1.size() <= nodeToExtend->g)path1.push_back(nodeToExtend->dir);
        //else path1[nodeToExtend->g] = nodeToExtend->dir;
        //for(int i = nodeToExtend->g + 1; i < path1.size(); i++)
            //path1[i] = none;
        bool success = true;
        for(int i = 0; i < 3 && success; i++)
            for(int j = 0; j < 3; j++)
                if(nodeToExtend->state[i][j] == goalState[i][j])continue;
                else
                {
                    success = false;
                    break;
                }
        if(success)
        {
            std::cout<<nodeToExtend->path.size()<<std::endl;
            for(int i = 0; i < nodeToExtend->path.size(); i++)
                std::cout<<nodeToExtend->path[i]<<'\t';
            delete nodeToExtend;
            return;
        }

        int zeropx,zeropy;
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if(nodeToExtend->state[i][j] == 0)
                {
                    zeropx = i;
                    zeropy = j;
                    break;
                }
        operate(left,zeropx,zeropy,nodeToExtend);
        operate(right,zeropx,zeropy,nodeToExtend);
        operate(up,zeropx,zeropy,nodeToExtend);
        operate(down,zeropx,zeropy,nodeToExtend);
        delete nodeToExtend;
    }
}

int main()
{
    srand(0);
    int startState[3][3] = {{1,2,3},{4,0,5},{6,7,8}};
    for(int i = 0; i < 5; i++)
    {
        int p1 = rand() % 9;
        int p2 = rand() % 9;
        int temp = startState[p1 / 3][p1 % 3];
        startState[p1 / 3][p1 % 3] = startState[p2 / 3][p2 % 3];
        startState[p2 / 3][p2 % 3] = temp;
    }

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
            std::cout<<startState[i][j]<<'\t';
        std::cout<<std::endl;
    }

    node *startNode = new node;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            startNode->state[i][j] = startState[i][j];
    startNode->g = 0;
    startNode->path.push_back(none);
    startNode->h = calculateH(startState);
    edge.push(startNode);
    search();

    //for(int i = 0; i < path1.size(); i++)
      //  std::cout<<path1[i]<<'\t';
    //std::cout<<std::endl<<path1.size();
    return 0;
}
