#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QTabWidget>
#include <QSqlRelation>
#include <table.h>
#include <statistics.h>

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
    Statistics *stat;

    void setForeignKey();
};



#endif // BROWSER_H
