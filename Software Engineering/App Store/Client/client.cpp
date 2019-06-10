#include "client.h"
#include <QDebug>

Client::Client()
{
    serverIp.setAddress("127.0.0.1");
    socket = new QTcpSocket;
    connect(socket,SIGNAL(connected()),this,SLOT(hasConnected()));
    socket->connectToHost(serverIp,port);

    page = new AppHomePage(this,socket);
    page->show();
}

Client::~Client()
{}

void Client::hasConnected()
{
    qDebug("has connected!\n");
}
