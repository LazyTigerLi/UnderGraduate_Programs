#ifndef CLIENT_H
#define CLIENT_H
#include "appHomePage.h"
#include <QHostAddress>
#include <QTcpSocket>
class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();

private:
    AppHomePage *page;
    bool hasLogin;

    QHostAddress serverIp;
    int port = 5678;
    QTcpSocket *socket;

private slots:
    void hasConnected();
};

#endif // CLIENT_H
