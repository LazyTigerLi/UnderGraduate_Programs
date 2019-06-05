#include "logindialog.h"
#include "browser.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSqlError>


LoginDialog::LoginDialog(QWidget *parent)
    :QDialog(parent)
{
    //this->setWindowModality(Qt::ApplicationModal);

    userNameLabel = new QLabel(tr("用户名"));
    passwdLabel = new QLabel(tr("密码"));
    userNameEdit = new QLineEdit;
    passwdEdit = new QLineEdit;
    passwdEdit->setEchoMode(QLineEdit::Password);
    connectButton = new QPushButton(tr("连接"));
    cancelButton = new QPushButton(tr("取消"));

    QGridLayout *upLayout = new QGridLayout;
    upLayout->addWidget(userNameLabel,0,0,1,1);
    upLayout->addWidget(userNameEdit,0,1,1,1);
    upLayout->addWidget(passwdLabel,1,0,1,1);
    upLayout->addWidget(passwdEdit,1,1,1,1);

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addWidget(connectButton);
    downLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(upLayout);
    mainLayout->addLayout(downLayout);

    setLayout(mainLayout);

    connect(connectButton,SIGNAL(clicked(bool)),this,SLOT(login()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}

LoginDialog::~LoginDialog()
{}

void LoginDialog::login()
{
    Browser *browser = new Browser;
    browser->db.setUserName(userNameEdit->text());
    browser->db.setPassword(passwdEdit->text());
    if(!browser->db.open())
    {
        QMessageBox::information(this, tr("银行管理系统"),browser->db.lastError().text());
        passwdEdit->clear();
        delete browser;
    }
    else
    {
        close();
        browser->generateWidgets();
        browser->show();
    }
}
