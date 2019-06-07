#ifndef CLIENT_H
#define CLIENT_H

#include "table.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class Client : public Table
{
    Q_OBJECT

public:
    Client(QSqlDatabase database,QVector<QString> tableName,QWidget *parent = nullptr);
    ~Client();

private:
    QPushButton *searchButton;
    QLineEdit *id;
    QLineEdit *name;
    QLineEdit *telephone;
    QLineEdit *address;
    QLineEdit *staffID;
    QLineEdit *staffName;

    QLabel *idLabel;
    QLabel *nameLabel;
    QLabel *telephoneLabel;
    QLabel *addressLabel;
    QLabel *staffIDLabel;
    QLabel *staffNameLabel;

    QSqlQueryModel *queryModel;

private slots:
    void search();

};

#endif // CLIENT_H
