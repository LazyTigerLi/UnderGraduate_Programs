#ifndef LOAN_H
#define LOAN_H

#include "table.h"

class Loan : public Table
{
    Q_OBJECT

public:
    Loan(QSqlDatabase database,QVector<QString> tableName,QWidget *parent = nullptr);
    ~Loan();

};

#endif // LOAN_H
