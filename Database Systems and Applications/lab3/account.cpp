#include "account.h"

Account::Account(QSqlDatabase database, QVector<QString> tableName, QWidget *parent)
    :Table(database,tableName,parent)
{
    searchButton = new QPushButton("search",this);

    accountID = new QLineEdit(this);
    clientID = new QLineEdit(this);
    bank = new QLineEdit(this);
    type = new QLineEdit(this);
    balanceMin = new QLineEdit(this);
    balanceMax = new QLineEdit(this);
    dateBegin = new QLineEdit(this);
    dateEnd = new QLineEdit(this);
    clientName = new QLineEdit(this);

    clientNameLabel = new QLabel("Client Name",this);
    accountIDLabel = new QLabel("Account ID",this);
    clientIDLabel = new QLabel("Client ID",this);
    bankLabel = new QLabel("Bank",this);
    typeLabel = new QLabel("Type",this);
    balanceLabel = new QLabel("Balance",this);
    dateLabel = new QLabel("Date",this);
    connect(searchButton,SIGNAL(clicked(bool)),this,SLOT(search()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(accountIDLabel);
    layout->addWidget(accountID);
    layout->addWidget(clientIDLabel);
    layout->addWidget(clientID);
    layout->addWidget(clientNameLabel);
    layout->addWidget(clientName);
    layout->addWidget(bankLabel);
    layout->addWidget(bank);
    layout->addWidget(typeLabel);
    layout->addWidget(type);
    layout->addWidget(balanceLabel);
    layout->addWidget(balanceMin);
    layout->addWidget(balanceMax);
    layout->addWidget(dateLabel);
    layout->addWidget(dateBegin);
    layout->addWidget(dateEnd);
    layout->addWidget(searchButton);

    QVBoxLayout *mainLyt = new QVBoxLayout;
    mainLyt->addLayout(mainLayout);
    mainLyt->addLayout(layout);
    setLayout(mainLyt);
    //tableLayout[0]->addLayout(layout);
    queryModel = new QSqlQueryModel(tableView[0]);
}

Account::~Account()
{}

void Account::search()
{
    QString accountIDQuery,clientIDQuery,clientNameQuery,bankQuery,typeQuery,
            balanceMinQuery,balanceMaxQuery,dateBeginQuery,dateEndQuery;
    if(accountID->text() == "")accountIDQuery = "";
    else accountIDQuery = " `Account`.`Account ID` = '" + accountID->text() + "' and ";
    if(clientID->text() == "")clientIDQuery = "";
    else clientIDQuery = " `Client_has_Account`.`Client ID` = '" + clientID->text() + "' and ";
    if(clientName->text() == "")clientNameQuery == "";
    else clientNameQuery = " `Client`.`Name` = '" + clientName->text() + "' and ";
    if(bank->text() == "")bankQuery = "";
    else bankQuery = " `Account`.`Bank` = '" + bank->text() + "' and ";
    if(type->text() == "")typeQuery = "";
    else typeQuery = " `Account`.`Type` = '" + type->text() + "' and ";
    if(balanceMin->text() == "")balanceMinQuery = "";
    else balanceMinQuery = " `Account`.`Balance` >= " + balanceMin->text() + " and ";
    if(balanceMax->text() == "")balanceMaxQuery = "";
    else balanceMaxQuery = " `Account`.`Balance` <= " + balanceMax->text() + " and ";
    if(dateBegin->text() == "")dateBeginQuery = "";
    else dateBeginQuery = " `Account`.`Date` >= '" + dateBegin->text() + "' and";
    if(dateEnd->text() == "")dateEndQuery = "";
    else dateEndQuery = " `Account`.`Date` >= '" + dateEnd->text() + "' and";

    QString query = "select `Account`.`Account ID`,`Client_has_Account`.`Client ID`,`Client`.`Name`,"
                    "`Account`.`Bank`,`Account`.`Type`,`Account`.`Balance`,`Account`.`Date`"
                    " from `mydb`.`Client`,`mydb`.`Account`,`mydb`.`Client_has_Account`"
                    " where " + accountIDQuery + clientIDQuery + clientNameQuery + bankQuery
                    + typeQuery + balanceMinQuery + balanceMaxQuery + dateBeginQuery
                    + dateEndQuery + " `Account`.`Account ID` = `Client_has_Account`.`Account ID` "
                    "and `Client_has_Account`.`Client ID` = `Client`.`ID`";
    qDebug("%s",qPrintable(query));
    queryModel->setQuery(QSqlQuery(query,db));
    tableView[0]->setModel(queryModel);
}
