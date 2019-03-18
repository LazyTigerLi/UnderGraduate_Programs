#include "mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    lineEdit = new QLineEdit(this);
    QString integer = "([1-9]\\d*|0)";
    QString fraction = "(([1-9]\\d*|0)\\.\\d*)";

    QString number = '(' + integer + '|' + fraction + ')';
    QString unaryOp = "(+|-|sin|cos|tan|log)";
    QString binaryOp = "(\\+|-|\\*|/|\\^)";
    QString expr = number + '(' + binaryOp + number + ")*";
    qDebug("%s",qPrintable(expr));
    rx.setPattern('^' + expr + '$');
    validator = new QRegExpValidator(rx,this);
    lineEdit->setValidator(validator);
}

MainWindow::~MainWindow()
{

}
