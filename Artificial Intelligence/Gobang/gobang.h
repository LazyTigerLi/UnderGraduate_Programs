#ifndef GOBANG_H
#define GOBANG_H

#include <utility>
#include <tuple>
#include <queue>

enum Role{ai = 0,hum = 1,empty = 2};
enum Direction{horizontal = 0,vertical = 1,diagonal1 = 2,diagonal2 = 3,all};

enum Pattern
{
    five = 100000,           //五
    four = 10000,           //活四
    three = 1000,          //活三
    two = 100,            //活二
    blockedFour = 1000,    //冲四
    blockedThree = 100,
    blockedTwo = 10     //这两个不知道怎么翻译，感觉网上的有些地方，相同的中文术语表示的是不同的意思
    //这里blocked代表只有一端堵住
    //因为两端都堵住的是不肯能成五的，对局势没有贡献
};

class Gobang
{
public:
    Gobang();
    ~Gobang();
    void alphaBetaSearch(int depth);
    void displayBoard();
    bool placeStone(std::pair<int,int> pos, Role role);
    Role isGameOver();

    int count;
    int evaluationOfPoints[2][15][15][4];           //存储ai和human的每个棋子在4个方向上的评分
    std::pair<int,int> nextStepOfAi;
private:
    int numOfChessman;
    Role board[15][15];
    bool top;                   //指示是否在最高的MAX节点
    std::pair<int,int> last;    //最后一次落子

    int evalute();             //全局评估函数
    void evaluateOfPoint(std::pair<int,int> pos,Role role,Direction dir);    //评估某个点对于role的分值
    std::tuple<int,int,int> analyzePoint(std::pair<int,int> pos,Role role,Direction dir);   //分析某个点在某个方向上的棋型

    std::queue<std::pair<int,int> > nextSteps(Role role);
    int maxValue(int alpha,int beta,int depth);
    int minValue(int alpha,int beta,int depth);

    void updateEvaluation(std::pair<int,int> pos);
    void removeStone(std::pair<int,int> pos);

};




#endif // GOBANG_H
