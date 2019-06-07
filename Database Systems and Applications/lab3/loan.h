#ifndef LOAN_H
#define LOAN_H

#include "table.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class Loan : public Table
{
    Q_OBJECT

public:
    Loan(QSqlDatabase database,QVector<QString> tableName,QWidget *parent = nullptr);
    ~Loan();

private:
    QPushButton *searchButton;
    QLineEdit *loanID;
    QLineEdit *clientID;
    QLineEdit *bank;
    QLineEdit *status;
    QLineEdit *loanMin;
    QLineEdit *loanMax;
    QLineEdit *clientName;

    QLabel *loanIDLabel;
    QLabel *clientIDLabel;
    QLabel *bankLabel;
    QLabel *statusLabel;
    QLabel *loanLabel;
    QLabel *clientNameLabel;

    QSqlQueryModel *queryModel;

private slots:
    void search();

};

#endif // LOAN_H
