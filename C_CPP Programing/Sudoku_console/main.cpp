#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <ctime>
struct unit
{
    int num;
    std::set<int> candidate;
}sudoku[9][9];

int numOfUnitsUnsolved = 0;

void updateCandidate(int row,int col)
{
    int num = sudoku[row][col].num;
    for(int i = 0; i < 9; i++)
    {
        if(i != col)sudoku[row][i].candidate.erase(num);
        if(i != row)sudoku[i][col].candidate.erase(num);
    }
    for(int i = (row / 3) * 3; i < (row / 3) * 3 + 3; i++)
        for(int j = (col / 3) * 3; j < (col / 3) * 3 + 3; j++)
            if(i != row || j != col)
                sudoku[i][j].candidate.erase(num);
}

std::pair<int,int> getMinNumOfCandidatesUnit()
{
    int row = 0,col = 0;
    unsigned int minNumOfCandidates = 9;
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
        {
            if(sudoku[i][j].candidate.size() <= minNumOfCandidates && sudoku[i][j].num == 0)
            {
                row = i;
                col = j;
                minNumOfCandidates = sudoku[i][j].candidate.size();
            }
        }
    return std::pair<int,int>(row,col);
}

void solve()
{
    if(!numOfUnitsUnsolved)
    {
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
                std::cout<<sudoku[i][j].num<<'\t';
            std::cout<<std::endl;
        }
        return;
    }
    auto minNumOfCandidatesUnit = getMinNumOfCandidatesUnit();
    int row = minNumOfCandidatesUnit.first;
    int col = minNumOfCandidatesUnit.second;
    if(sudoku[row][col].candidate.size() == 0)return;
    for(auto it = sudoku[row][col].candidate.begin(); it != sudoku[row][col].candidate.end(); it++)
    {
        bool stateOfRow[8],stateOfCol[8],stateOfSubregion[8];
        int num = *it;
        for(int i = 0,index = 0; i < 9; i++)
            if(i != col)
            {
                if(sudoku[row][i].candidate.count(num))stateOfRow[index] = true;
                else stateOfRow[index] = false;
                index++;
            }
        for(int i = 0,index = 0; i < 9; i++)
            if(i != row)
            {
                if(sudoku[i][col].candidate.count(num))stateOfCol[index] = true;
                else stateOfCol[index] = false;
                index++;
            }
        for(int i = (row / 3) * 3,index = 0; i < (row / 3) * 3 + 3; i++)
            for(int j = (col / 3) * 3; j < (col / 3) * 3 + 3; j++)
                if(i != row || j != col)
                {
                    if(sudoku[i][j].candidate.count(num))stateOfSubregion[index] = true;
                    else stateOfSubregion[index] = false;
                    index++;
                }

        sudoku[row][col].num = num;
        updateCandidate(row,col);
        numOfUnitsUnsolved--;
        solve();
        numOfUnitsUnsolved++;
        sudoku[row][col].num = 0;
        for(int i = 0,index = 0; i < 9; i++)
            if(i != col)
            {
                if(stateOfRow[index])sudoku[row][i].candidate.insert(num);
                index++;
            }
        for(int i = 0,index = 0; i < 9; i++)
            if(i != row)
            {
                if(stateOfCol[index])sudoku[i][col].candidate.insert(num);
                index++;
            }
        for(int i = (row / 3) * 3,index = 0; i < (row / 3) * 3 + 3; i++)
            for(int j = (col / 3) * 3; j < (col / 3) * 3 + 3; j++)
                if((i != row || j != col))
                {
                    if(stateOfSubregion[index])sudoku[i][j].candidate.insert(num);
                    index++;
                }
    }
    //std::cout<<numOfUnitsUnsolved<<std::endl;
}

int main()
{
    std::set<int> candidateOfRow[9];
    std::set<int> candidateOfCol[9];
    std::set<int> candidateOfSubgrid[9];
    for(int i = 0; i < 9; i++)
    {
        candidateOfRow[i].insert({1,2,3,4,5,6,7,8,9});
        candidateOfCol[i].insert({1,2,3,4,5,6,7,8,9});
        candidateOfSubgrid[i].insert({1,2,3,4,5,6,7,8,9});
    }
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            std::cin>>sudoku[i][j].num;
            if(sudoku[i][j].num)sudoku[i][j].candidate.insert(sudoku[i][j].num);
            candidateOfRow[i].erase(sudoku[i][j].num);
            candidateOfCol[j].erase(sudoku[i][j].num);
            candidateOfSubgrid[(i / 3) * 3 + j / 3].erase((sudoku[i][j].num));
        }
    }
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
        {
            if(!sudoku[i][j].num)
            {
                numOfUnitsUnsolved++;
                std::set<int> temp;
                std::set_intersection(std::begin(candidateOfRow[i]),std::end(candidateOfRow[i]),
                                      std::begin(candidateOfCol[j]),std::end(candidateOfCol[j]),
                                      std::inserter(temp,std::begin(temp)));
                std::set_intersection(std::begin(candidateOfSubgrid[(i / 3) * 3 + j / 3]),
                                      std::end(candidateOfSubgrid[(i / 3) * 3 + j / 3]),
                                      std::begin(temp),std::end(temp),
                                      std::inserter(sudoku[i][j].candidate,std::begin(sudoku[i][j].candidate)));
                if(sudoku[i][j].candidate.size() == 1)
                {
                    sudoku[i][j].num = *sudoku[i][j].candidate.begin();
                    updateCandidate(i,j);
                }
            }
        }
    clock_t t1 = clock();
    solve();
    clock_t t2 = clock();
    std::cout<<t2 - t1<<std::endl;

    return 0;
}
