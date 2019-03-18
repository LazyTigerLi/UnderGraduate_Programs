#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExpValidator>
#include <QRegExp>
#include <QLineEdit>
#include <QString>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QLineEdit *lineEdit;
    QRegExp rx;
    QRegExpValidator *validator;
};

#endif // MAINWINDOW_H
