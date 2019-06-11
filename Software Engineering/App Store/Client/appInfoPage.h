#ifndef APPINFOPAGE_H
#define APPINFOPAGE_H

#include "appPage.h"
#include "appHomePage.h"
#include <QLabel>
#include <QTextBrowser>

class AppInfoPage : public AppPage
{
    Q_OBJECT

public:
    AppInfoPage(Client *c,int appID,QString appName);
    ~AppInfoPage();

private:
    enum State{AnalyzeReply,GetAppInfo,DownLoad};

    State state;
    QTcpSocket *sock;
    int appID;
    QString appName;
    QString iconPath;

    QLabel *iconLabel;
    QLabel *nameLabel;
    QTextBrowser *introBrowser;
    QPushButton *downloadButton;          //中间的布局

    QListWidget *screenshots;
    QListWidget *comments;

    QByteArray rcvMsg;
    void getAppInfoRequest();
    void getAppInfoReply();

private slots:
    void analyzeReply();
    void backToHomePage();

};

#endif // APPINFOPAGE_H
