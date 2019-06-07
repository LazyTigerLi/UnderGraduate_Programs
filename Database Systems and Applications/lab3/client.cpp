#include "client.h"

Client::Client(QSqlDatabase database, QVector<QString> tableName, QWidget *parent)
    :Table(database,tableName,parent)
{
    searchButton = new QPushButton("search",this);
    id = new QLineEdit(this);
    name = new QLineEdit(this);
    telephone = new QLineEdit(this);
    address = new QLineEdit(this);
    staffID = new QLineEdit(this);
    staffName = new QLineEdit(this);

    idLabel = new QLabel("ID",this);
    nameLabel = new QLabel("name",this);
    telephoneLabel = new QLabel("telephone",this);
    addressLabel = new QLabel("address",this);
    staffIDLabel = new QLabel("staff ID",this);
    staffNameLabel = new QLabel("staff name",this);
    connect(searchButton,SIGNAL(clicked(bool)),this,SLOT(search()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(idLabel);
    layout->addWidget(id);
    layout->addWidget(nameLabel);
    layout->addWidget(name);
    layout->addWidget(telephoneLabel);
    layout->addWidget(telephone);
    layout->addWidget(addressLabel);
    layout->addWidget(address);
    layout->addWidget(staffIDLabel);
    layout->addWidget(staffID);
    layout->addWidget(staffNameLabel);
    layout->addWidget(staffName);
    layout->addWidget(searchButton);
    tableLayout[0]->addLayout(layout);
    setLayout(mainLayout);

    queryModel = new QSqlQueryModel(tableView[0]);
}

Client::~Client()
{}

void Client::search()
{
    QString idQuery,nameQuery,telephoneQuery,addressQuery,
            staffIDQuery,staffNameQuery;
    if(id->text() == "")idQuery = "";
    else idQuery = " `Client`.`ID` = '" + id->text() + "' and ";
    if(name->text() == "")nameQuery = "";
    else nameQuery = " `Client`.`Name` = '" + name->text() + "' and ";
    if(telephone->text() == "")telephoneQuery = "";
    else telephoneQuery = " `Client`.`telephone` = '" + telephone->text() + "' and ";
    if(address->text() == "")addressQuery = "";
    else addressQuery = " `Client`.`Address` like '%" + address->text() + "%' and ";
    if(staffID->text() == "")staffIDQuery = "";
    else staffIDQuery = " `Client`.`Staff ID` = '" + staffID->text() + "' and ";
    if(staffName->text() == "")staffNameQuery = "";
    else staffNameQuery = " `Staff`.`Name` = '" + staffName->text() + "' and ";

    QString query;
    if(staffID->text() == "" && staffName->text() == "")
        query = "select * from `mydb`.`Client` where " + idQuery + nameQuery + telephoneQuery
                + addressQuery + " `Client`.`ID` is not null";
    else query = "select `Client`.`ID`,`Client`.`Name`,`Client`.`Telephone`,`Client`.`Address`"
                 ",`Client`.`Contact Name`,`Client`.`Contact Telephone`,`Client`.`Contact Email`"
                 ",`Client`.`Relationship with Contact`,`Client`.`Staff ID`,`Staff`.`Name`"
                 ",`Client`.`Relationship with Staff` from `mydb`.`Client`,`mydb`.`Staff` where"
                 " `Client`.`Staff ID` = `Staff`.`ID` and " + idQuery + nameQuery + telephoneQuery
                + addressQuery + staffIDQuery + staffNameQuery + " `Client`.`ID` is not null";
    qDebug("%s",qPrintable(query));
    queryModel->setQuery(QSqlQuery(query,db));
    tableView[0]->setModel(queryModel);
}
