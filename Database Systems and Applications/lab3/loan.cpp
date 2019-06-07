#include "loan.h"

Loan::Loan(QSqlDatabase database, QVector<QString> tableName, QWidget *parent)
    :Table(database,tableName,parent)
{
    searchButton = new QPushButton("search",this);

    loanID = new QLineEdit(this);
    clientID = new QLineEdit(this);
    bank = new QLineEdit(this);
    status = new QLineEdit(this);
    loanMin = new QLineEdit(this);
    loanMax = new QLineEdit(this);
    clientName = new QLineEdit(this);

    clientNameLabel = new QLabel("Client Name",this);
    loanIDLabel = new QLabel("Loan ID",this);
    clientIDLabel = new QLabel("Client ID",this);
    bankLabel = new QLabel("Bank",this);
    statusLabel = new QLabel("Status",this);
    loanLabel = new QLabel("Loan",this);
    connect(searchButton,SIGNAL(clicked(bool)),this,SLOT(search()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(loanIDLabel);
    layout->addWidget(loanID);
    layout->addWidget(clientIDLabel);
    layout->addWidget(clientID);
    layout->addWidget(clientNameLabel);
    layout->addWidget(clientName);
    layout->addWidget(bankLabel);
    layout->addWidget(bank);
    layout->addWidget(statusLabel);
    layout->addWidget(status);
    layout->addWidget(loanLabel);
    layout->addWidget(loanMin);
    layout->addWidget(loanMax);
    layout->addWidget(searchButton);

    QVBoxLayout *mainLyt = new QVBoxLayout;
    mainLyt->addLayout(mainLayout);
    mainLyt->addLayout(layout);
    setLayout(mainLyt);
    //tableLayout[0]->addLayout(layout);
    queryModel = new QSqlQueryModel(tableView[0]);
}

Loan::~Loan()
{}

void Loan::search()
{
    QString loanIDQuery,clientIDQuery,clientNameQuery,bankQuery,statusQuery,
            loanMinQuery,loanMaxQuery;

    if(loanID->text() == "")loanIDQuery = "";
    else loanIDQuery = " `Loan`.`Loan ID` = '" + loanID->text() + "' and ";
    if(clientID->text() == "")clientIDQuery = "";
    else clientIDQuery = " `Client_has_Loan`.`ID` = '" + clientID->text() + "' and ";
    if(clientName->text() == "")clientNameQuery = "";
    else clientNameQuery = " `Client`.`Name` = '" + clientName->text() + "' and ";
    if(bank->text() == "")bankQuery = "";
    else bankQuery = " `Loan`.`Bank` = '" + bank->text() + "' and ";
    if(status->text() == "")statusQuery = "";
    else statusQuery = " `Loan`.`Status` = '" + status->text() + "' and ";
    if(loanMin->text() == "")loanMinQuery = "";
    else loanMinQuery = " `Loan`.`Loan` >= " + loanMin->text() + " and ";
    if(loanMax->text() == "")loanMaxQuery = "";
    else loanMaxQuery = " `Loan`.`Loan` <= " + loanMax->text() + " and ";

    QString query = "select `Loan`.`Loan ID`,`Client`.`ID`,`Client`.`Name`,"
                    "`Loan`.`Bank`,`Loan`.`Status`,`Loan`.`Loan` from `mydb`.`Client`,"
                    "`mydb`.`Loan`,`mydb`.`Client_has_Loan` where " + loanIDQuery
                    + clientIDQuery + clientNameQuery + bankQuery + statusQuery
                    + loanMinQuery + loanMaxQuery + " `Client_has_Loan`.`ID` = `Client`.`ID`"
                    " and `Client_has_Loan`.`Loan ID` = `Loan`.`Loan ID`";

    qDebug("%s",qPrintable(query));
    queryModel->setQuery(QSqlQuery(query,db));
    tableView[0]->setModel(queryModel);
}
