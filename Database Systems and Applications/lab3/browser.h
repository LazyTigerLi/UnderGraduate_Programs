#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QTabWidget>
#include <table.h>

class Browser: public QWidget
{
    Q_OBJECT
public:
    Browser(QWidget *parent = nullptr);
    ~Browser();
    void generateWidgets();

    QSqlDatabase db;

private:
    QTabWidget *browser;
    Table *bank;
    Table *staff;
    Table *client;
    Table *account;
    Table *loan;
    //QWidget *loan;
    //QWidget *statistics;
};



#endif // BROWSER_H
