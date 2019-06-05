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
    bank = new Table(QVector<QString>{"Bank"});
    staff = new Table(QVector<QString>{"Staff"});
    client = new Table(QVector<QString>{"Client"});
    account = new Table(QVector<QString>{"Account","Client_has_Account"});
    loan = new Table(QVector<QString>{"Loan","LoanDistribution","Client_has_Loan"});
    //statistics = new QWidget;

    browser->addTab(bank,tr("支行管理"));
    browser->addTab(staff,tr("员工管理"));
    browser->addTab(client,tr("客户管理"));
    browser->addTab(account,tr("账户管理"));
    browser->addTab(loan,tr("贷款管理"));
    //browser->addTab(loan,tr("贷款管理"));
    //browser->addTab(statistics,tr("业务统计"));

    browser->show();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(browser);
    setLayout(mainLayout);
}
