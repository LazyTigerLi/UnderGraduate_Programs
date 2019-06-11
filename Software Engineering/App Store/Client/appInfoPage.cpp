#include "appInfoPage.h"
#include "client.h"
#include <QPixmap>
#include <QImage>
#include <QGridLayout>

AppInfoPage::AppInfoPage(Client *c, int appID, QString appName)
    :AppPage(c)
{
    state = AnalyzeReply;
    sock = client->socket;
    this->appID = appID;
    this->appName = appName;
    iconPath = QString("/home/linan/Client/images/") + appID + ".png";

    iconLabel = new QLabel(this);
    QPixmap pixmap;
    pixmap.convertFromImage(*(new QImage(iconPath)));
    iconLabel->setPixmap(pixmap);

    nameLabel = new QLabel(appName,this);
    introBrowser = new QTextBrowser(this);
    downloadButton = new QPushButton("Download");

    QGridLayout *midLayout = new QGridLayout;
    midLayout->addWidget(iconLabel,0,0,3,3);
    midLayout->addWidget(nameLabel,0,3,1,3);
    midLayout->addWidget(introBrowser,1,3,2,3);
    midLayout->addWidget(downloadButton,1,7,1,1);

    screenshots = new QListWidget(this);
    comments = new QListWidget(this);

    mainLayout->addLayout(midLayout);
    mainLayout->addWidget(screenshots);
    mainLayout->addWidget(comments);

    connect(backButton,SIGNAL(clicked(bool)),this,SLOT(backToHomePage()));
    connect(sock,SIGNAL(readyRead()),this,SLOT(analyzeReply()));

    getAppInfoRequest();
}

AppInfoPage::~AppInfoPage()
{}

void AppInfoPage::backToHomePage()
{
    setParent(nullptr);
    client->setCentralWidget(client->homePage);
    client->homePage->show();
    connect(sock,SIGNAL(readyRead()),client->homePage,SLOT(analyzeReply()));
    close();
}

void AppInfoPage::analyzeReply()
{
    if(state == AnalyzeReply)
    {
        rcvMsg = sock->readAll();
        qDebug("here");
        qDebug("%s",rcvMsg.mid(0,3).toStdString().data());
        if(QString::fromStdString(rcvMsg.mid(0,3).toStdString()) == "app")
        {
            state = GetAppInfo;
            getAppInfoReply();
        }
    }
    else if(state == GetAppInfo)
        getAppInfoReply();
}

void AppInfoPage::getAppInfoRequest()
{
    sock->write((QString("app info") + QString::number(appID)).toUtf8());
}

void AppInfoPage::getAppInfoReply()
{
    int bytes = 8;
    int introSize = std::stoi(rcvMsg.mid(bytes,4).toStdString());
    QString introduction = QString::fromStdString(rcvMsg.mid(bytes + 4,introSize).toStdString());
    introBrowser->setText(introduction);
}
