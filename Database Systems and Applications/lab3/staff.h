#ifndef STAFF_H
#define STAFF_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include "table.h"

class Staff : public Table
{
    Q_OBJECT

public:
    Staff(QSqlDatabase database,QVector<QString> tableName,QWidget *parent = nullptr);
    ~Staff();

private:
    QPushButton *searchButton;
    QLineEdit *id;
    QLineEdit *name;
    QCheckBox *isManager;
    QLineEdit *department;
    QLineEdit *employDateStart;
    QLineEdit *employDateEnd;
    QLineEdit *bank;

    QLabel *idLabel;
    QLabel *nameLabel;
    QLabel *departmentLabel;
    QLabel *employDateLabel;
    QLabel *bankLabel;

    QSqlQueryModel *queryModel;

private slots:
    void search();
};

#endif // STAFF_H
