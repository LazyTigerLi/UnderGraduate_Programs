#ifndef SUDOKU_H
#define SUDOKU_H

#include <QComboBox>
#include <QGridLayout>
#include <set>
#include <iostream>
#include <algorithm>
#include <QDebug>
struct unit
{
    int num;
    QComboBox *box;
    std::set<int> candidate;
};

class sudoku : public QWidget
{
    Q_OBJECT
public:
    sudoku(QWidget *parent = 0);
    ~sudoku();

public slots:
    void solve();

private:
    unit data[9][9];
    int answer[9][9];
    int numOfUnitsUnsolved;
    int numOfAnswers;
    std::set<int> candidateOfRow[9];
    std::set<int> candidateOfCol[9];
    std::set<int> candidateOfSubgrid[9];
    //These three variables are only used when updating the comboboxes.

    std::pair<int,int> getMinNumOfCandidatesUnit();
    void updateCandidate(int row,int col);
    void solveSudoku();
private slots:
    void updateComboBox();
};



#endif // SUDOKU_H
