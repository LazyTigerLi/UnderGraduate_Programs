#ifndef BANK_H
#define BANK_H
#include "table.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class Bank : public Table
{
    Q_OBJECT

public:
    Bank(QSqlDatabase database,QVector<QString> tableName,QWidget *parent = nullptr);
    ~Bank();

    //父类Table的tableLayout下面可以再加窗口部件
private:
    QPushButton *searchButton;
    QLineEdit *name;
    QLineEdit *city;
    QLineEdit *fundMin;
    QLineEdit *fundMax;
    QLabel *nameLabel;
    QLabel *cityLabel;
    QLabel *fundLabel;

    QSqlQueryModel *queryModel;

private slots:
    void search();
};

#endif // BANK_H
