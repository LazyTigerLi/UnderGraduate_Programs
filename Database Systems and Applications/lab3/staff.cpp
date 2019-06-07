#include "staff.h"

Staff::Staff(QSqlDatabase database, QVector<QString> tableName, QWidget *parent)
    :Table(database,tableName,parent)
{
    searchButton = new QPushButton("search",this);
    id = new QLineEdit(this);
    name = new QLineEdit(this);
    department = new QLineEdit(this);
    employDateStart = new QLineEdit(this);
    employDateEnd = new QLineEdit(this);
    bank = new QLineEdit(this);
    isManager = new QCheckBox("is manager",this);

    idLabel = new QLabel("ID",this);
    nameLabel = new QLabel("name",this);
    departmentLabel = new QLabel("deparment",this);
    employDateLabel = new QLabel("employed date",this);
    bankLabel = new QLabel("bank",this);
    connect(searchButton,SIGNAL(clicked(bool)),this,SLOT(search()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(idLabel);
    layout->addWidget(id);
    layout->addWidget(nameLabel);
    layout->addWidget(name);
    layout->addWidget(isManager);
    layout->addWidget(departmentLabel);
    layout->addWidget(department);
    layout->addWidget(employDateLabel);
    layout->addWidget(employDateStart);
    layout->addWidget(employDateEnd);
    layout->addWidget(bankLabel);
    layout->addWidget(bank);
    layout->addWidget(searchButton);
    tableLayout[0]->addLayout(layout);
    setLayout(mainLayout);

    queryModel = new QSqlQueryModel(tableView[0]);
}

Staff::~Staff()
{}

void Staff::search()
{
    QString idQuery,nameQuery,isManagerQuery,departmentQuery,
            employDateStartQuery,employDateEndQuery,bankQuery;
    if(id->text() == "")idQuery = "";
    else idQuery = " `Staff`.`ID` = '" + id->text() + "' and ";
    if(name->text() == "")nameQuery = "";
    else nameQuery = " `Staff`.`Name` = '" + name->text() + "' and ";
    if(!isManager->isChecked())isManagerQuery = "";
    else isManagerQuery = " `Staff`.`Is Manager` = '是' and ";
    if(department->text() == "")departmentQuery = "";
    else departmentQuery = " `Staff`.`Department` = '" + department->text() + "' and ";
    if(employDateStart->text() == "")employDateStartQuery = "";
    else employDateStartQuery = " `Staff`.`Employed Date` >= '" + employDateStart->text() + "' and";
    if(employDateEnd->text() == "")employDateEndQuery = "";
    else employDateEndQuery = " `Staff`.`Employed Date` <= '" + employDateEnd->text() + "' and";
    if(bank->text() == "")bankQuery = "";
    else bankQuery = " `Staff`.`Bank` = '" + bank->text() + "' and ";

    QString query = "select * from `mydb`.`Staff` where " + idQuery + nameQuery + isManagerQuery
            + departmentQuery + employDateStartQuery + employDateEndQuery + bankQuery
            + " `Staff`.`ID` is not null";

    qDebug("%s",qPrintable(query));
    queryModel->setQuery(QSqlQuery(query,db));
    tableView[0]->setModel(queryModel);
}
