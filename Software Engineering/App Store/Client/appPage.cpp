#include "appPage.h"
#include <QHBoxLayout>

AppPage::AppPage(Client *c, QWidget *parent)
    :QWidget(parent)
{
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

    optionsButton->setMenu(options);
    userButton->setMenu(user);
    options->addAction(uploadAction);
    options->addAction(updateAction);
    user->addAction(loginAction);

    QHBoxLayout *upperLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    upperLayout->addWidget(backButton);
    upperLayout->addWidget(searchBar);
    upperLayout->addWidget(searchButton);
    upperLayout->addWidget(optionsButton);
    upperLayout->addWidget(userButton);
    mainLayout->addLayout(upperLayout);
    setLayout(mainLayout);

    client = c;
}

AppPage::~AppPage()
{

}

void AppPage::login()
{

}
