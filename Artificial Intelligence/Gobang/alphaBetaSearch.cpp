#include "gobang.h"
#include <limits.h>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>

struct cmp
{
    bool operator()(std::tuple<int,int,Gobang *> pos1,std::tuple<int,int,Gobang *> pos2)
    {
        int x1 = std::get<0>(pos1),y1 = std::get<1>(pos1),x2 = std::get<0>(pos2),y2 = std::get<1>(pos2);
        Gobang *gobang = std::get<2>(pos1);
        int evaluationAi1 = 0,evaluationAi2 = 0,evaluationHum1 = 0,evaluationHum2 = 0;
        for(int i = 0; i < 4; i++)
        {
            evaluationAi1 += gobang->evaluationOfPoints[ai][x1][y1][i];
            evaluationHum1 += gobang->evaluationOfPoints[hum][x1][y1][i];
            evaluationAi2 += gobang->evaluationOfPoints[ai][x2][y2][i];
            evaluationHum2 += gobang->evaluationOfPoints[hum][x2][y2][i];
        }
        if((evaluationAi1 >= evaluationHum1 ? evaluationAi1 : evaluationHum1) <
                (evaluationAi2 >= evaluationHum2 ? evaluationAi2 : evaluationHum2))return true;
     return false;
    }
};



void insertIntoQueue(std::set<std::pair<int,int> > &s,std::queue<std::pair<int,int> > &q)   //对容器的操作，应当是引用传参，要加&
{
    for(std::set<std::pair<int,int> >::iterator it = s.begin(); it != s.end(); it++)
        q.push(*it);
}


std::queue<std::pair<int, int> > Gobang::nextSteps(Role role)  //根据当前状态生成下一个可能的行棋
{
    int distance = 1;
    std::queue<std::pair<int,int> > steps;

    std::priority_queue<std::tuple<int,int,Gobang *>,std::vector<std::tuple<int,int,Gobang *> >,cmp> orderedSteps;
    std::set<std::pair<int,int> > fiveAi,fiveHum,fourAi,fourHum,blockedFourOrThreeAi,blockedFourOrThreeHum;
                                    //blockedThreeOrTwoAi,blockedThreeOrTwoHum,
                                    //blockedTwoAi,blockedTwoHum,littleUseAi,littleUseHum;
                                    //对于比较初级的棋型，感觉还不需要考虑进攻还是防守的问题
    //对于双方，将可能的落点按照生成的棋型进行分类
    //littleUse存储没什么价值的落点
    std::set<std::pair<int,int> > fiveHum_fourAi,fiveHum_blockedFourOrThreeAi,fourHum_fourAi,fourHum_blockedFourOrThreeAi,
                                blockedFourOrThreeHum_fourAi,blockedFourOrThreeHum_blockedFourOrThreeAi;
    //如果是ai落子，分别表示，能阻止hum成五并且自己成四的落点，等等
    std::set<std::pair<int,int> > fiveAi_fourHum,fiveAi_blockedFourOrThreeHum,fourAi_fourHum,fourAi_blockedFourOrThreeHum,
                                blockedFourOrThreeAi_fourHum,blockedFourOrThreeAi_blockedFourOrThreeHum;
    //如果是hum落子，分别表示，能阻止ai成五并且自己成四的落点，等等

    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
        {
            bool reasonable = false;
            if(board[i][j] == empty)
            {
                for(int m = ((i - distance > 0) ? i - distance : 0); m <= ((i + distance < 14) ? i + distance : 14); m++)
                {
                    if(reasonable)break;
                    for(int n = ((j - distance > 0) ? j - distance : 0); n <= ((j + distance < 14) ? j + distance : 14); n++)
                        if(board[m][n] != empty)
                        {
                            reasonable = true;
                            break;
                        }
                }
            }
            if(reasonable)
            {
                int evaluationAi = 0,evaluationHum = 0;
                for(int k = 0; k < 4; k++)
                {
                    evaluationAi += evaluationOfPoints[ai][i][j][k];
                    evaluationHum += evaluationOfPoints[hum][i][j][k];
                }

                //提升下面这些着法的优先级
                if(evaluationAi >= 2 * three && evaluationAi < four)evaluationAi = four;        //两个活三（冲四）相当于一个活四
                if(evaluationAi >= 2 * two && evaluationAi < three)evaluationAi = three;        //两个活二（冲三）相当于一个活三
                if(evaluationHum >= 2 * three && evaluationHum < four)evaluationHum = four;
                if(evaluationHum >= 2 * two && evaluationHum < three)evaluationHum = three;

                if(evaluationAi < three && evaluationHum < three)orderedSteps.push(std::make_tuple(i,j,this));
                if(evaluationAi >= five)fiveAi.insert(std::make_pair(i,j));
                else if(evaluationAi >= four)fourAi.insert(std::make_pair(i,j));
                else if(evaluationAi >= three)blockedFourOrThreeAi.insert(std::make_pair(i,j));

                if(evaluationHum >= five)fiveHum.insert(std::make_pair(i,j));
                else if(evaluationHum >= four)fourHum.insert(std::make_pair(i,j));
                else if(evaluationHum >= three)blockedFourOrThreeHum.insert(std::make_pair(i,j));
            }
        }

    std::set_intersection(fiveHum.begin(),fiveHum.end(),fourAi.begin(),fourAi.end(),
                          std::inserter(fiveHum_fourAi,fiveHum_fourAi.begin()));
    std::set_intersection(fiveHum.begin(),fiveHum.end(),blockedFourOrThreeAi.begin(),blockedFourOrThreeAi.end(),
                          std::inserter(fiveHum_blockedFourOrThreeAi,fiveHum_blockedFourOrThreeAi.begin()));
    std::set_intersection(fourHum.begin(),fourHum.end(),fourAi.begin(),fourAi.end(),
                          std::inserter(fourHum_fourAi,fourHum_fourAi.begin()));
    std::set_intersection(fourHum.begin(),fourHum.end(),blockedFourOrThreeAi.begin(),blockedFourOrThreeAi.end(),
                          std::inserter(fourHum_blockedFourOrThreeAi,fourHum_blockedFourOrThreeAi.begin()));
    std::set_intersection(blockedFourOrThreeHum.begin(),blockedFourOrThreeHum.end(),fourAi.begin(),fourAi.end(),
                          std::inserter(blockedFourOrThreeHum_fourAi,blockedFourOrThreeHum_fourAi.begin()));
    std::set_intersection(blockedFourOrThreeHum.begin(),blockedFourOrThreeHum.end(),blockedFourOrThreeAi.begin(),
                          blockedFourOrThreeAi.end(),std::inserter(blockedFourOrThreeHum_blockedFourOrThreeAi,
                                                                blockedFourOrThreeHum_blockedFourOrThreeAi.begin()));
    std::set_intersection(fiveAi.begin(),fiveAi.end(),fourHum.begin(),fourHum.end(),
                          std::inserter(fiveAi_fourHum,fiveAi_fourHum.begin()));
    std::set_intersection(fiveAi.begin(),fiveAi.end(),blockedFourOrThreeHum.begin(),blockedFourOrThreeHum.end(),
                          std::inserter(fiveAi_blockedFourOrThreeHum,fiveAi_blockedFourOrThreeHum.begin()));
    fourAi_fourHum = fourHum_fourAi;
    fourAi_blockedFourOrThreeHum = blockedFourOrThreeHum_fourAi;
    blockedFourOrThreeAi_fourHum = fourHum_blockedFourOrThreeAi;
    blockedFourOrThreeAi_blockedFourOrThreeHum = blockedFourOrThreeHum_blockedFourOrThreeAi;

    if(role == ai)
    {
        if(!fiveAi.empty())
        {
            steps.push(*(fiveAi.begin()));
            return steps;
        }
        //优先级如下：自己成五、阻止对面成五、自己成四、阻止对面成四、自己成三、阻止对面成三
        //如果存在比如能同时阻止对面成四并且自己成四的落点，则优先此落点
        insertIntoQueue(fiveHum_fourAi,steps);
        insertIntoQueue(fiveHum_blockedFourOrThreeAi,steps);
        insertIntoQueue(fiveHum,steps);           //阻止hum成五
        insertIntoQueue(fourHum_fourAi,steps);
        insertIntoQueue(fourAi,steps);
        insertIntoQueue(fourHum_blockedFourOrThreeAi,steps);
        insertIntoQueue(fourHum,steps);
        //insertIntoQueue(blockedFourOrThreeHum_fourAi,steps);
        //这种操作没有必要
        insertIntoQueue(blockedFourOrThreeHum_blockedFourOrThreeAi,steps);
        insertIntoQueue(blockedFourOrThreeAi,steps);
    }
    else
    {
        if(!fiveHum.empty())
        {
            steps.push(*(fiveHum.begin()));
            return steps;
        }
        //优先级如下：自己成五、阻止对面成五、自己成四、阻止对面成四、自己成三、阻止对面成三
        //如果存在比如能同时阻止对面成四并且自己成四的落点，则优先此落点
        insertIntoQueue(fiveAi_fourHum,steps);
        insertIntoQueue(fiveAi_blockedFourOrThreeHum,steps);
        insertIntoQueue(fiveAi,steps);           //阻止hum成五
        insertIntoQueue(fourAi_fourHum,steps);
        insertIntoQueue(fourHum,steps);
        insertIntoQueue(fourAi_blockedFourOrThreeHum,steps);
        insertIntoQueue(fourAi,steps);
        //insertIntoQueue(blockedFourOrThreeAi_fourHum,steps);
        //这种操作没有必要
        insertIntoQueue(blockedFourOrThreeAi_blockedFourOrThreeHum,steps);
        insertIntoQueue(blockedFourOrThreeHum,steps);
    }
    while(!orderedSteps.empty())
    {
        auto pos = orderedSteps.top();
        int x = std::get<0>(pos),y = std::get<1>(pos);
        //std::cout<<x<<','<<y<<'\t';
        steps.push(std::make_pair(x,y));
        orderedSteps.pop();
    }
    //std::cout<<'\n'<<'\n';
    return steps;
}

int Gobang::maxValue(int alpha,int beta,int depth)
{
    if(depth == 0)return evalute();       //截断
    int v = INT_MIN;
    bool isTop = top;
    std::queue<std::pair<int,int> > steps = nextSteps(ai);
    //if(top)std::cout<<steps.size()<<'\n';
    while(!steps.empty())
    {
        std::pair<int,int> pos = steps.front();
        steps.pop();

        //if(isTop)std::cout<<pos.first<<','<<pos.second<<'\t';
        placeStone(pos,ai);
        if(top)top = false;
        int min = minValue(alpha,beta,depth);
        removeStone(pos);
        //v = (v > min) ? v : min;                //这里应当记录所采取的着法
        if(v < min)
        {
            v = min;
            if(isTop)nextStepOfAi = pos;
        }
        if(v >= beta)
        {
            count++;
            return v;
        }
        alpha = (alpha > v) ? alpha : v;
    }
    //std::cout<<stepsOfAi.size()<<'\n';
    //if(isTop)std::cout<<'\n';
    return v;
}

int Gobang::minValue(int alpha,int beta,int depth)  //与maxValue高度相似
{
    if(depth == 0)return evalute();       //截断
    int v = INT_MAX;
    std::queue<std::pair<int,int> > steps = nextSteps(hum);
    while(!steps.empty())
    {
        std::pair<int,int> pos = steps.front();
        steps.pop();

        placeStone(pos,hum);
        int max = maxValue(alpha,beta,depth - 1);
        removeStone(pos);
        v = (v < max) ? v : max;
        if(v <= alpha)
        {
            count++;
            return v;
        }
        beta = (beta < v) ? beta : v;
    }
    return v;
}

void Gobang::alphaBetaSearch(int depth)
{
    //while(!stepsOfAi.empty())stepsOfAi.pop();
    top = true;
    count = 0;
    maxValue(INT_MIN,INT_MAX,depth);
}
