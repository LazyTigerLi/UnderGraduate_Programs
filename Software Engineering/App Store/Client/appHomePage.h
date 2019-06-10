#ifndef APPHOMEPAGE_H
#define APPHOMEPAGE_H

//#include <QScrollArea>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTcpSocket>
#include <QVector>
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QDataStream>
#include "appPage.h"

class AppHomePage : public AppPage
{
    Q_OBJECT


public:
    QTcpSocket *sock;
    AppHomePage(Client *c,QTcpSocket *socket);
    ~AppHomePage();

private:
    enum State{AnalyzeReply,ListApp};         //当接收到信息时作怎样的处理，相当于一个状态机

    QListWidget *appArea;

    QVector<int> appID;
    QVector<QString> appName;
    QVector<QString> icon;          //存放的是图标的地址
    QString iconPath = "/home/linan/Client/images/";
    QVector<QListWidgetItem *> item;
    //QScrollArea *appArea;
    void listAppRequest();                 //在主界面列出所有应用
    void listAppReply();
    State state;

    QByteArray rcvMsg;

private slots:
    void analyzeReply();
    void searchApp();
    void newAppInfoPage(QListWidgetItem *itemClicked);
};

#endif // APPHOMEPAGE_H
