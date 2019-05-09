#include "gobang.h"

//该函数在某个方向上搜索这样的棋型：起止为对手的棋子或边界或空格，中间均为己方的棋子，或最多有一个空格
//调用该函数的前提是，在pos处落的是role方的棋子或者为空
std::tuple<int,int,int> Gobang::analyzePoint(std::pair<int, int> pos, Role role, Direction dir)
{
    int x = pos.first,y = pos.second;
    int numOfMyStones = 1,isBlocked = 0,emptyPos = -1;
    //numOfMyStones为己方棋子的数量，isBlocked表示两端是否被堵住(0：没有堵住，1：一端堵住，2：另一端堵住，3：两端都堵住)
    //只有边界或者对方的棋子会被认为是堵住
    //emptyPos表示空格在该棋型中的相对位置
    switch(dir)
    {
    case horizontal:
        for(int i = y + 1; i <= 15; i++)    //向右扫描      这里一开始写成了i = x + 1，下面的vertical也写错了
        {
            if(i == 15)     //  另一端是边界，相当于被堵死
            {
                isBlocked = 1;
                break;
            }
            if(board[x][i] == empty)
            {
                if(empty == -1 && i < 14 && board[x][i + 1] == role)emptyPos = numOfMyStones;
                else break;
            }
            else if(board[x][i] == role)numOfMyStones++;
            else
            {
                isBlocked = 1;
                break;
            }
        }
        for(int i = y - 1; i >= -1; i++)    //向左扫描      这里一开始写成了i = x - 1
        {
            if(i == -1)     //  另一端是边界，相当于被堵死
            {
                if(isBlocked == 1)isBlocked = 3;
                else isBlocked = 2;
                break;
            }
            if(board[x][i] == empty)
            {
                if(empty == -1 && i > 0 && board[x][i - 1] == role)emptyPos = 0;
                else break;
            }
            else if(board[x][i] == role)
            {
                numOfMyStones++;
                if(emptyPos != -1)emptyPos++;
            }
            else
            {
                if(isBlocked == 1)isBlocked = 3;
                else isBlocked = 2;
                break;
            }
        }
        break;
    case vertical:
        for(int i = x + 1; i <= 15; i++)    //向下扫描
        {
            if(i == 15)     //  另一端是边界，相当于被堵死
            {
                isBlocked = 1;
                break;
            }
            if(board[i][y] == empty)
            {
                if(empty == -1 && i < 14 && board[i + 1][y] == role)emptyPos = numOfMyStones;
                else break;
            }
            else if(board[i][y] == role)numOfMyStones++;
            else
            {
                isBlocked = 1;
                break;
            }
        }
        for(int i = x - 1; i >= -1; i++)    //向上扫描
        {
            if(i == -1)     //  另一端是边界，相当于被堵死
            {
                if(isBlocked == 1)isBlocked = 3;
                else isBlocked = 2;
                break;
            }
            if(board[i][y] == empty)
            {
                if(empty == -1 && i > 0 && board[i - 1][y] == role)emptyPos = 0;
                else break;
            }
            else if(board[i][y] == role)
            {
                numOfMyStones++;
                if(emptyPos != -1)emptyPos++;
            }
            else
            {
                if(isBlocked == 1)isBlocked = 3;
                else isBlocked = 2;
                break;
            }
        }
        break;
    //以下两个对角线方向的分析均类似处理
    case diagonal1:
        for(int i = x + 1,j = y + 1; i <= 15 && j <= 15; i++,j++)
        {
            if(i == 15 || j == 15)
            {
                isBlocked = 1;
                break;
            }
            if(board[i][j] == empty)
            {
                if(empty == -1 && i < 14 && j < 14 && board[i + 1][j + 1] == role)emptyPos = numOfMyStones;
                else break;
            }
            else if(board[i][j] == role)numOfMyStones++;
            else
            {
                isBlocked = 1;
                break;
            }
        }
        for(int i = x - 1,j = y - 1; i >= -1 && j >= -1; i--,j--)
        {
            if(i == -1 || j == -1)
            {
                if(isBlocked == 1)isBlocked = 3;
                else isBlocked = 2;
                break;
            }
            if(board[i][j] == empty)
            {
                if(empty == -1 && i > 0 && j > 0 && board[i - 1][j - 1] == role)emptyPos = 0;
                else break;
            }
            else if(board[i][j] == role)
            {
                numOfMyStones++;
                if(emptyPos != -1)emptyPos++;
            }
            else
            {
                if(isBlocked == 1)isBlocked = 3;
                else isBlocked = 2;
                break;
            }
        }
        break;
    case diagonal2:
        for(int i = x + 1,j = y - 1; i <= 15 && j >= -1; i++,j--)
        {
            if(i == 15 || j == -1)
            {
                isBlocked = 1;
                break;
            }
            if(board[i][j] == empty)
            {
                if(empty == -1 && i < 14 && board[i + 1][j - 1] == role)emptyPos = numOfMyStones;
                else break;
            }
            else if(board[i][j] == role)numOfMyStones++;
            else
            {
                isBlocked = 1;
                break;
            }
        }
        for(int i = x - 1,j = y + 1; i >= -1 && j <= 15; i--,j++)
        {
            if(i == -1 || j == 15)
            {
                if(isBlocked == 1)isBlocked = 3;
                else isBlocked = 2;
                break;
            }
            if(board[i][j] == empty)
            {
                if(empty == -1 && i > 0 && j < 14 && board[i - 1][j + 1] == role)emptyPos = 0;
                else break;
            }
            else if(board[i][j] == role)
            {
                numOfMyStones++;
                if(emptyPos != -1)emptyPos++;
            }
            else
            {
                if(isBlocked == 1)isBlocked = 3;
                else isBlocked = 2;
                break;
            }
        }
        break;
    default:
        break;
    }
    return std::make_tuple(numOfMyStones,isBlocked,emptyPos);
}

//此函数虽然较长，但主要均为if-else这样的分支语句，开销并不大
void Gobang::evaluateOfPoint(std::pair<int, int> pos,Role role,Direction dir)
{
    int x = pos.first,y = pos.second;
    if(board[x][y] != role && board[x][y] != empty)
    {
        evaluationOfPoints[role][x][y][horizontal] = 0;
        evaluationOfPoints[role][x][y][vertical] = 0;
        evaluationOfPoints[role][x][y][diagonal1] = 0;
        evaluationOfPoints[role][x][y][diagonal2] = 0;
        return;
    }
    //pos处的落子是对方的，所以该位置对role的价值为0

    Direction direct[4] = {horizontal,vertical,diagonal1,diagonal2};
    for(int i = 0; i < 4; i++)
    {
        if(dir != all && direct[i] != dir)continue;    //当前方向不是我要评价的方向
        evaluationOfPoints[role][x][y][direct[i]] = 0;
        auto situation = analyzePoint(pos,role,direct[i]);
        int numOfMyStones = std::get<0>(situation),
                isBlocked = std::get<1>(situation),
                emptyPos = std::get<2>(situation);
        if(isBlocked == 0)  //两端没有堵死
        {
            if(numOfMyStones >= 9)      //我方棋子数大于等于9，则必定成五
            {
                evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            }
            switch(numOfMyStones)
            {
            case 8:
                if(emptyPos == 4)                //XXXXOXXXX
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 7:
                if(emptyPos == 3 || emptyPos == 4)            //XXXOXXXX,XXXXOXXX
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 6:
                if(emptyPos == 2 || emptyPos == 4)            //XXOXXXX,XXXXOXX
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else if(emptyPos == 3)          //XXXOXXX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;       //这里的分值有待商榷
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 5:
                if(emptyPos == 2 || emptyPos == 3)                //XXOXXX,XXXOXX
                    evaluationOfPoints[role][x][y][direct[i]] += three;
                else if(emptyPos == 1 || emptyPos == 4)         //XOXXXX,XXXXOX
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 4:
                if(emptyPos == 2)                //XXOXX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;      //这里的分值有待商榷
                else if(emptyPos == 1 || emptyPos == 3)         //XOXXX,XXXOX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else evaluationOfPoints[role][x][y][direct[i]] += four;
                break;
            case 3:
                if(emptyPos == 1 || emptyPos ==2)                //XOXX,XXOX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedThree;
                else evaluationOfPoints[role][x][y][direct[i]] += three;
                break;
            case 2:
                if(emptyPos == 1)           //XOX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedTwo;
                else evaluationOfPoints[role][x][y][direct[i]] += two;
                break;
            default:
                break;
            }
        }
        else if(isBlocked == 1) //末端堵死
        {
            if(numOfMyStones >= 9)      //我方棋子数大于等于9，则必定成五
            {
                evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            }
            switch(numOfMyStones)
            {
            case 8:
                if(emptyPos == 4)                //XXXXOXXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 7:
                if(emptyPos == 3)            //XXXOXXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else if(emptyPos == 4)          //XXXXOXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 6:
                if(emptyPos == 2)            //XXOXXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else if(emptyPos == 4)          //XXXXOXX|
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else if(emptyPos == 3)          //XXXOXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;       //这里的分值有待商榷
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 5:
                if(emptyPos == 2)                //XXOXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedThree;
                else if(emptyPos == 3)          //XXXOXX|
                    evaluationOfPoints[role][x][y][direct[i]] += three;
                else if(emptyPos == 1)         //XOXXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else if(emptyPos == 4)          //XXXXOX|
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 4:
                if(emptyPos == 2)                //XXOXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;      //这里的分值有待商榷
                else if(emptyPos == 1)         //XOXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else if(emptyPos == 3)           //XXXOX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else evaluationOfPoints[role][x][y][direct[i]] += blockedFour;        //XXXX|
                break;
            case 3:
                if(emptyPos == 1 || emptyPos ==2)                //XOXX|,XXOX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedThree;
                else evaluationOfPoints[role][x][y][direct[i]] += blockedThree;       //XXX|
                break;
            case 2:
                if(emptyPos == 1)                   //XOX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedTwo;
                else evaluationOfPoints[role][x][y][direct[i]] += blockedTwo;     //XX|
                break;
            default:
                break;
            }
        }
        else if(isBlocked == 2) //起始端堵死
        {
            if(numOfMyStones >= 9)      //我方棋子数大于等于9，则必定成五
            {
                evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            }
            switch(numOfMyStones)
            {
            case 8:
                if(emptyPos == 4)                //|XXXXOXXXX
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 7:
                if(emptyPos == 3)            //|XXXOXXXX
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else if(emptyPos == 4)          //|XXXXOXXX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 6:
                if(emptyPos == 2)            //|XXOXXXX
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else if(emptyPos == 4)          //|XXXXOXX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else if(emptyPos == 3)          //|XXXOXXX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;       //这里的分值有待商榷
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 5:
                if(emptyPos == 2)                //|XXOXXX
                    evaluationOfPoints[role][x][y][direct[i]] += three;
                else if(emptyPos == 3)          //|XXXOXX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedThree;
                else if(emptyPos == 1)         //|XOXXXX
                    evaluationOfPoints[role][x][y][direct[i]] += four;
                else if(emptyPos == 4)          //|XXXXOX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 4:
                if(emptyPos == 2)                //|XXOXX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;      //这里的分值有待商榷
                else if(emptyPos == 1)         //|XOXXX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else if(emptyPos == 3)           //|XXXOX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else evaluationOfPoints[role][x][y][direct[i]] += blockedFour;        //|XXXX
                break;
            case 3:
                if(emptyPos == 1 || emptyPos ==2)                //|XOXX,|XXOX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedThree;
                else evaluationOfPoints[role][x][y][direct[i]] += blockedThree;       //|XXX
                break;
            case 2:
                if(emptyPos == 1)                   //|XOX
                    evaluationOfPoints[role][x][y][direct[i]] += blockedTwo;
                else evaluationOfPoints[role][x][y][direct[i]] += blockedTwo;     //|XX
                break;
            default:
                break;
            }
        }

        else                    //两端都被堵死
        {
            if(numOfMyStones >= 9)      //我方棋子数大于等于9，则必定成五
            {
                evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            }
            switch(numOfMyStones)
            {
            case 8:
                if(emptyPos == 4)                //|XXXXOXXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 7:
                if(emptyPos == 3 || emptyPos == 4)            //|XXXOXXXX|,|XXXXOXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 6:
                if(emptyPos == 2 || emptyPos == 4)            //|XXOXXXX|,|XXXXOXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else if(emptyPos == 3)          //|XXXOXXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;       //这里的分值有待商榷
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 5:
                if(emptyPos == 2 || emptyPos == 3)                //|XXOXXX|,|XXXOXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else if(emptyPos == 1 || emptyPos == 4)         //|XOXXXX|,|XXXXOX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else evaluationOfPoints[role][x][y][direct[i]] += five;
                break;
            case 4:
                if(emptyPos == 2)                //|XXOXX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;      //这里的分值有待商榷
                else if(emptyPos == 1 || emptyPos == 3)         //|XOXXX|,|XXXOX|
                    evaluationOfPoints[role][x][y][direct[i]] += blockedFour;
                else break;        //|XXXX|
                break;
            case 3:             //不可能成五
                break;
            case 2:             //不可能成五
                break;
            default:
                break;
            }
        }
    }
}

int Gobang::evalute()              //计算整个棋盘对于role的评估值
{
    int evalutionAi = 0,evalutionHum = 0;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
        {
            evaluateOfPoint(std::make_pair(i,j),ai,all);
            evaluateOfPoint(std::make_pair(i,j),hum,all);
            for(int k = 0; k < 4; k++)
            {
                evalutionAi += evaluationOfPoints[ai][i][j][k];
                evalutionHum += evaluationOfPoints[hum][i][j][k];
            }
        }
    return evalutionAi - evalutionHum;
}
