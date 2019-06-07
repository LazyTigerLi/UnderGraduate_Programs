#ifndef BROWSER_H
#define BROWSER_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QTabWidget>
#include <QSqlRelation>
#include "bank.h"
#include "account.h"
#include "client.h"
#include "staff.h"
#include "loan.h"
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
    Bank *bank;
    Staff *staff;
    Client *client;
    Account *account;
    Loan *loan;
    Statistics *stat;

    //void setForeignKey();
};



#endif // BROWSER_H
