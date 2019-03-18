#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QPushButton>
#include "sudoku.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    sudoku *sudokuWidget;
    QMenu *gameMenu;
    QMenu *helpMenu;
    QAction *newAction;
    QAction *solveAction;
    QAction *exitAction;
    QAction *ruleAction;
    QAction *authorAction;

    QPushButton *solveSudoku;
    void createMenus();

public slots:
    void newGame();
    void solveGame();
    void ruleInfo();
    void authorInfo();
};

#endif // MAINWINDOW_H
