#ifndef CLIENT_H
#define CLIENT_H
#include "appHomePage.h"
#include "appInfoPage.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QMainWindow>

class Client : public QMainWindow
{
    Q_OBJECT
public:
    Client();
    ~Client();
    QTcpSocket *socket;
    AppHomePage *homePage;
    AppInfoPage *infoPage;

private:
    bool hasLogin;

    QHostAddress serverIp;
    int port = 5678;


private slots:
    void hasConnected();
};

#endif // CLIENT_H
