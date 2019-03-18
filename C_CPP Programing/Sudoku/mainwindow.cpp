#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenus();
    sudokuWidget = new sudoku(this);
    solveSudoku = new QPushButton("&solve",this);
    connect(solveSudoku,SIGNAL(clicked(bool)),sudokuWidget,SLOT(sudoku::solve()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenus()
{
    newAction = new QAction(tr("&new"),this);
    connect(newAction,SIGNAL(triggered(bool)),this,SLOT(newGame()));
    solveAction = new QAction(tr("&solve"),this);
    connect(solveAction,SIGNAL(triggered(bool)),this,SLOT(solveGame()));
    exitAction = new QAction(tr("e&xit"),this);
    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));
    gameMenu = menuBar()->addMenu(tr("&game"));
    gameMenu->addAction(newAction);
    gameMenu->addAction(solveAction);
    gameMenu->addAction(exitAction);

    ruleAction = new QAction(tr("about rule"),this);
    connect(ruleAction,SIGNAL(triggered(bool)),this,SLOT(ruleInfo()));
    authorAction = new QAction(tr("about author"),this);
    connect(authorAction,SIGNAL(triggered(bool)),this,SLOT(authorInfo()));
    helpMenu = menuBar()->addMenu(tr("&help"));
    helpMenu->addAction(ruleAction);
    helpMenu->addAction(authorAction);
}

void MainWindow::newGame()
{}

void MainWindow::solveGame()
{}

void MainWindow::ruleInfo()
{}

void MainWindow::authorInfo()
{}
