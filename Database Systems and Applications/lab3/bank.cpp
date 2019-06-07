#include "bank.h"
//#include <QDebug>

Bank::Bank(QSqlDatabase database, QVector<QString> tableName, QWidget *parent)
    :Table(database,tableName,parent)
{
    searchButton = new QPushButton("search",this);
    name = new QLineEdit(this);
    city = new QLineEdit(this);
    fundMax = new QLineEdit(this);
    fundMin = new QLineEdit(this);
    nameLabel = new QLabel("name",this);
    cityLabel = new QLabel("city",this);
    fundLabel = new QLabel("fund",this);
    connect(searchButton,SIGNAL(clicked(bool)),this,SLOT(search()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(nameLabel);
    layout->addWidget(name);
    layout->addWidget(cityLabel);
    layout->addWidget(city);
    layout->addWidget(fundLabel);
    layout->addWidget(fundMin);
    layout->addWidget(fundMax);
    layout->addWidget(searchButton);
    tableLayout[0]->addLayout(layout);
    setLayout(mainLayout);

    queryModel = new QSqlQueryModel(tableView[0]);
}

Bank::~Bank()
{}

void Bank::search()
{
    QString nameQuery,cityQuery,fundQuery1,fundQuery2;
    if(name->text() == "")nameQuery = "";
    else nameQuery = "`Bank`.`Name` = '" + name->text() + "' and ";
    if(city->text() == "")cityQuery = "";
    else cityQuery = "`Bank`.`City` = '" + city->text() + "' and ";
    if(fundMin->text() == "")fundQuery1 = "";
    else fundQuery1 = "`Bank`.`Fund` >= '" + fundMin->text() + "' and ";
    if(fundMax->text() == "")fundQuery2 = "";
    else fundQuery2 = "`Bank`.`Fund` <= '" + fundMax->text() + "' and ";

    QString query = "select * from `mydb`.`Bank` where " + nameQuery + cityQuery
            + fundQuery1 + fundQuery2 + " `Bank`.`Name` is not null";

    //qDebug("%s",qPrintable(query));
    queryModel->setQuery(QSqlQuery(query,db));
    tableView[0]->setModel(queryModel);
}
