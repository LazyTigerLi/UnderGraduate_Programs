#include "loan.h"

Loan::Loan(QSqlDatabase database, QVector<QString> tableName, QWidget *parent)
    :Table(database,tableName,parent)
{
    setLayout(mainLayout);
}

Loan::~Loan()
{}
