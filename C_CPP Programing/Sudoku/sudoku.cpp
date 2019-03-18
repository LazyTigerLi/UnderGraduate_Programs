#include "sudoku.h"

sudoku::sudoku(QWidget *parent)
{
    setParent(parent);
    setGeometry(50,50,500,500);
    setStyleSheet("QComboBox::drop-down{"
                  "border-style:none;}");
    int size = ((height() < width()) ? height() : width()) / 9;
    QStringList initList = {"","1","2","3","4","5","6","7","8","9"};
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
        {
            data[i][j].box = new QComboBox(this);
            data[i][j].box->setGeometry(size * j,size * i,size,size);
            data[i][j].box->insertItems(0,initList);
            connect(data[i][j].box,SIGNAL(currentTextChanged(QString)),this,SLOT(updateComboBox()));
            data[i][j].num = 0;
            data[i][j].candidate.insert({1,2,3,4,5,6,7,8,9});
        }
    numOfUnitsUnsolved = 0;
    numOfAnswers = 0;
    for(int i = 0; i < 9; i++)
    {
        candidateOfRow[i].insert({1,2,3,4,5,6,7,8,9});
        candidateOfCol[i].insert({1,2,3,4,5,6,7,8,9});
        candidateOfSubgrid[i].insert({1,2,3,4,5,6,7,8,9});
    }
}

sudoku::~sudoku()
{}

std::pair<int,int> sudoku::getMinNumOfCandidatesUnit()
{
    int row = 0,col = 0;
    unsigned int minNumOfCandidates = 9;
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
        {
            if(data[i][j].candidate.size() <= minNumOfCandidates && data[i][j].num == 0)
            {
                row = i;
                col = j;
                minNumOfCandidates = data[i][j].candidate.size();
            }
        }
    return std::pair<int,int>(row,col);
}

void sudoku::updateCandidate(int row, int col)
{
    int num = data[row][col].num;
    for(int i = 0; i < 9; i++)
    {
        if(i != col)data[row][i].candidate.erase(num);
        if(i != row)data[i][col].candidate.erase(num);
    }
    for(int i = (row / 3) * 3; i < (row / 3) * 3 + 3; i++)
        for(int j = (col / 3) * 3; j < (col / 3) * 3 + 3; j++)
            if(i != row || j != col)
                data[i][j].candidate.erase(num);
}

void sudoku::updateComboBox()
{
    int row,col,num;
    bool find = false;
    for(row = 0; row < 9; row++)
    {
        for(col = 0; col < 9; col++)
        {
            num = data[row][col].box->currentText().toInt();
            if(num != data[row][col].num)
            {
                find = true;
                break;
            }
        }
        if(find)break;
    }
    if(int num = data[row][col].num)
    {
        candidateOfRow[row].insert(num);
        candidateOfCol[col].insert(num);
        candidateOfSubgrid[(row / 3) * 3 + col / 3].insert(num);
    }
    candidateOfRow[row].erase(num);
    candidateOfCol[col].erase(num);
    candidateOfSubgrid[(row / 3) * 3 + col / 3].erase(num);
    for(int i = 0; i < 9; i++)
    {
        if(i != col)
        {
            std::set<int> temp1,temp2;
            disconnect(data[row][i].box,SIGNAL(currentTextChanged(QString)),this,SLOT(updateComboBox()));
            data[row][i].box->clear();
            data[row][i].box->addItem("");
            std::set_intersection(std::begin(candidateOfRow[row]),std::end(candidateOfRow[row]),
                                  std::begin(candidateOfCol[i]),std::end(candidateOfCol[i]),
                                  std::inserter(temp1,std::begin(temp1)));
            std::set_intersection(std::begin(candidateOfSubgrid[(row / 3) * 3 + i / 3]),
                                  std::end(candidateOfSubgrid[(row / 3) * 3 + i / 3]),
                                  std::begin(temp1),std::end(temp1),
                                  std::inserter(temp2,std::begin(temp2)));
            if(data[row][i].num)temp2.insert(data[row][i].num);
            for(auto it = temp2.begin(); it != temp2.end(); it++)
                data[row][i].box->addItem(QString::number(*it));
            if(data[row][i].num)data[row][i].box->setCurrentText(QString::number(data[row][i].num));
            connect(data[row][i].box,SIGNAL(currentTextChanged(QString)),this,SLOT(updateComboBox()));
        }
        if(i != row)
        {
            std::set<int> temp1,temp2;
            disconnect(data[i][col].box,SIGNAL(currentTextChanged(QString)),this,SLOT(updateComboBox()));
            data[i][col].box->clear();
            data[i][col].box->addItem("");
            std::set_intersection(std::begin(candidateOfRow[i]),std::end(candidateOfRow[i]),
                                  std::begin(candidateOfCol[col]),std::end(candidateOfCol[col]),
                                  std::inserter(temp1,std::begin(temp1)));
            std::set_intersection(std::begin(candidateOfSubgrid[(i / 3) * 3 + col / 3]),
                                  std::end(candidateOfSubgrid[(i / 3) * 3 + col / 3]),
                                  std::begin(temp1),std::end(temp1),
                                  std::inserter(temp2,std::begin(temp2)));
            if(data[i][col].num)temp2.insert(data[i][col].num);
            for(auto it = temp2.begin(); it != temp2.end(); it++)
                data[i][col].box->addItem(QString::number(*it));
            if(data[i][col].num)data[i][col].box->setCurrentText(QString::number(data[i][col].num));
            connect(data[i][col].box,SIGNAL(currentTextChanged(QString)),this,SLOT(updateComboBox()));
        }
    }
    for(int i = (row / 3) * 3; i < (row / 3) * 3 + 3; i++)
        for(int j = (col / 3) * 3; j < (col / 3) * 3 + 3; j++)
            if(i != row || j != col)
            {
                std::set<int> temp1,temp2;
                disconnect(data[i][j].box,SIGNAL(currentTextChanged(QString)),this,SLOT(updateComboBox()));
                data[i][j].box->clear();
                data[i][j].box->addItem("");
                std::set_intersection(std::begin(candidateOfRow[i]),std::end(candidateOfRow[i]),
                                      std::begin(candidateOfCol[j]),std::end(candidateOfCol[j]),
                                      std::inserter(temp1,std::begin(temp1)));
                std::set_intersection(std::begin(candidateOfSubgrid[(i / 3) * 3 + j / 3]),
                                      std::end(candidateOfSubgrid[(i / 3) * 3 + j / 3]),
                                      std::begin(temp1),std::end(temp1),
                                      std::inserter(temp2,std::begin(temp2)));
                if(data[i][j].num)temp2.insert(data[i][j].num);
                for(auto it = temp2.begin(); it != temp2.end(); it++)
                    data[i][j].box->addItem(QString::number(*it));
                if(data[i][j].num)data[i][j].box->setCurrentText(QString::number(data[i][j].num));
                connect(data[i][j].box,SIGNAL(currentTextChanged(QString)),this,SLOT(updateComboBox()));
            }
    data[row][col].num = num;
}

void sudoku::solveSudoku()
{
    if(numOfAnswers)return;
    if(!numOfUnitsUnsolved)
    {
        for(int i = 0; i < 9; i++)
            for(int j = 0; j < 9; j++)
                answer[i][j] = data[i][j].num;
        numOfAnswers++;
        return;
    }
    auto minNumOfCandidatesUnit = getMinNumOfCandidatesUnit();
    int row = minNumOfCandidatesUnit.first;
    int col = minNumOfCandidatesUnit.second;
    if(data[row][col].candidate.size() == 0)return;
    for(auto it = data[row][col].candidate.begin(); it != data[row][col].candidate.end(); it++)
    {
        bool stateOfRow[8],stateOfCol[8],stateOfSubregion[8];
        int num = *it;
        for(int i = 0,index = 0; i < 9; i++)
            if(i != col)
            {
                if(data[row][i].candidate.count(num))stateOfRow[index] = true;
                else stateOfRow[index] = false;
                index++;
            }
        for(int i = 0,index = 0; i < 9; i++)
            if(i != row)
            {
                if(data[i][col].candidate.count(num))stateOfCol[index] = true;
                else stateOfCol[index] = false;
                index++;
            }
        for(int i = (row / 3) * 3,index = 0; i < (row / 3) * 3 + 3; i++)
            for(int j = (col / 3) * 3; j < (col / 3) * 3 + 3; j++)
                if(i != row || j != col)
                {
                    if(data[i][j].candidate.count(num))stateOfSubregion[index] = true;
                    else stateOfSubregion[index] = false;
                    index++;
                }

        data[row][col].num = num;
        updateCandidate(row,col);
        numOfUnitsUnsolved--;
        solveSudoku();
        numOfUnitsUnsolved++;
        data[row][col].num = 0;
        for(int i = 0,index = 0; i < 9; i++)
            if(i != col)
            {
                if(stateOfRow[index])data[row][i].candidate.insert(num);
                index++;
            }
        for(int i = 0,index = 0; i < 9; i++)
            if(i != row)
            {
                if(stateOfCol[index])data[i][col].candidate.insert(num);
                index++;
            }
        for(int i = (row / 3) * 3,index = 0; i < (row / 3) * 3 + 3; i++)
            for(int j = (col / 3) * 3; j < (col / 3) * 3 + 3; j++)
                if((i != row || j != col))
                {
                    if(stateOfSubregion[index])data[i][j].candidate.insert(num);
                    index++;
                }
    }
}

void sudoku::solve()
{
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
        {
            if(!data[i][j].num)
            {
                numOfUnitsUnsolved++;
                std::set<int> temp;
                std::set_intersection(std::begin(candidateOfRow[i]),std::end(candidateOfRow[i]),
                                      std::begin(candidateOfCol[j]),std::end(candidateOfCol[j]),
                                      std::inserter(temp,std::begin(temp)));
                std::set_intersection(std::begin(candidateOfSubgrid[(i / 3) * 3 + j / 3]),
                                      std::end(candidateOfSubgrid[(i / 3) * 3 + j / 3]),
                                      std::begin(temp),std::end(temp),
                                      std::inserter(data[i][j].candidate,std::begin(data[i][j].candidate)));
                if(data[i][j].candidate.size() == 1)
                {
                    data[i][j].num = *data[i][j].candidate.begin();
                    updateCandidate(i,j);
                }
            }
            solveSudoku();
        }
    for(int i = 0;i < 9; i++)
        for(int j = 0; j < 9; j++)
            if(data[i][j].box->currentIndex() == 0)data[i][j].box->setCurrentText(QString::number(answer[i][j]));
}
