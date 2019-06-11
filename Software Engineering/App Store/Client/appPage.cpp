#include "appPage.h"
#include "client.h"
#include "logindialog.h"
#include "signupdialog.h"
#include <QHBoxLayout>

AppPage::AppPage(Client *c, QMainWindow *parent)
    :QWidget(parent)
{
    //setWindowState(Qt::WindowMaximized);

    optionsButton = new QToolButton;
    userButton = new QToolButton;
    backButton = new QPushButton;
    searchButton = new QPushButton;

    options = new QMenu(tr("选项"));
    user = new QMenu(tr("用户"));

    searchBar = new QLineEdit;
    uploadAction = new QAction(tr("上传"));
    updateAction = new QAction(tr("更新"));
    loginAction = new QAction(tr("登录"));
    signUpAction = new QAction(tr("注册"));

    optionsButton->setMenu(options);
    userButton->setMenu(user);
    options->addAction(uploadAction);
    options->addAction(updateAction);
    user->addAction(loginAction);
    user->addAction(signUpAction);

    QHBoxLayout *upperLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    upperLayout->addWidget(backButton);
    upperLayout->addWidget(searchBar);
    upperLayout->addWidget(searchButton);
    upperLayout->addWidget(userButton);
    upperLayout->addWidget(optionsButton);
    mainLayout->addLayout(upperLayout);
    setLayout(mainLayout);

    client = c;

    connect(loginAction,SIGNAL(triggered(bool)),this,SLOT(login()));
    connect(signUpAction,SIGNAL(triggered(bool)),this,SLOT(signUp()));
}

AppPage::~AppPage()
{}

class Client;

void AppPage::login()
{
    new LoginDialog(this,client->socket,this);
    connect(client->socket,SIGNAL(readyRead()),this,SLOT(analyzeReply()));
}

void AppPage::signUp()
{
    new SignUpDialog(this,client->socket,this);
    connect(client->socket,SIGNAL(readyRead()),this,SLOT(analyzeReply()));
}
