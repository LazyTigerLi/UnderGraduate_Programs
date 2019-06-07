#include "browser.h"
#include <QVBoxLayout>

Browser::Browser(QWidget *parent)
    :QWidget(parent)
{
    resize(1000,800);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("mydb");
    db.setPort(3306);
}

Browser::~Browser()
{}

void Browser::generateWidgets()
{
    browser = new QTabWidget(this);
    bank = new Bank(db,QVector<QString>{"Bank"});
    staff = new Staff(db,QVector<QString>{"Staff"});
    client = new Client(db,QVector<QString>{"Client"});
    account = new Account(db,QVector<QString>{"Account","Client_has_Account"});
    loan = new Loan(db,QVector<QString>{"Loan","LoanDistribution","Client_has_Loan"});
    stat = new Statistics(db,this);
    //setForeignKey();
    account->mainLayout->setStretchFactor(account->tableLayout[0],3);
    account->mainLayout->setStretchFactor(account->tableLayout[1],1);
    loan->mainLayout->setStretchFactor(loan->tableLayout[0],5);
    loan->mainLayout->setStretchFactor(loan->tableLayout[1],3);
    loan->mainLayout->setStretchFactor(loan->tableLayout[2],2);

    browser->addTab(bank,tr("支行管理"));
    browser->addTab(staff,tr("员工管理"));
    browser->addTab(client,tr("客户管理"));
    browser->addTab(account,tr("账户管理"));
    browser->addTab(loan,tr("贷款管理"));
    browser->addTab(stat,tr("业务统计"));

    browser->show();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(browser);
    setLayout(mainLayout);
}

/*void Browser::setForeignKey()
{
    account->model[0]->setRelation(5,QSqlRelation("Bank","Name","Name"));
    account->model[0]->setRelation(1,QSqlRelation("Client","ID","ID"));
    account->model[1]->setRelation(0,QSqlRelation("Client","ID","ID"));
    account->model[1]->setRelation(1,QSqlRelation("Account","Account ID","Account ID"));
    account->model[1]->setRelation(2,QSqlRelation("Account","Type","Type"));
    account->model[1]->setRelation(3,QSqlRelation("Account","Bank","Bank"));
    staff->model[0]->setRelation(8,QSqlRelation("Bank","Name","Name"));
    client->model[0]->setRelation(8,QSqlRelation("Staff","ID","ID"));
    loan->model[0]->setRelation(1,QSqlRelation("Bank","Name","Name"));
    loan->model[0]->setRelation(4,QSqlRelation("Client","ID","ID"));
    loan->model[2]->setRelation(1,QSqlRelation("Loan","Loan ID","Loan ID"));
    loan->model[2]->setRelation(0,QSqlRelation("Client","ID","ID"));
    loan->model[1]->setRelation(2,QSqlRelation("Loan","Loan ID","Loan ID"));

}*/
