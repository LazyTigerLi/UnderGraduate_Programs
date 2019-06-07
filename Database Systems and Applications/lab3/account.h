#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "table.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class Account : public Table
{
    Q_OBJECT

public:
    Account(QSqlDatabase database,QVector<QString> tableName,QWidget *parent = nullptr);
    ~Account();

private:
    QPushButton *searchButton;
    QLineEdit *accountID;
    QLineEdit *clientID;
    QLineEdit *bank;
    QLineEdit *type;
    QLineEdit *balanceMin;
    QLineEdit *balanceMax;
    QLineEdit *dateBegin;
    QLineEdit *dateEnd;
    QLineEdit *clientName;

    QLabel *accountIDLabel;
    QLabel *clientIDLabel;
    QLabel *bankLabel;
    QLabel *typeLabel;
    QLabel *balanceLabel;
    QLabel *dateLabel;
    QLabel *clientNameLabel;

    QSqlQueryModel *queryModel;

private slots:
    void search();

};

#endif // ACCOUNT_H
