#include "tcpSocket.h"
#include <QDateTime>
#include <string>
#include <QDebug>

TcpSocket::TcpSocket(QSqlDatabase database)
{
    db = database;
    req.ip = peerAddress();
    connect(this,SIGNAL(readyRead()),this,SLOT(analyzeRequest()));
    connect(this,SIGNAL(disconnected()),this,SLOT(clientDisconnectedSlot()));
}

TcpSocket::~TcpSocket()
{}

void TcpSocket::clientDisconnectedSlot()
{
    emit clientDisconnected(socketDescriptor());
}

void TcpSocket::analyzeRequest()
{
    /*QString msg = QDateTime::currentDateTime().toString() + '\t' + req.userId + '\t' + req.appId
            + '\t' + req.appName + '\t' + req.isDeveloper + '\n';
    emit(newMsg(msg));*/
    QByteArray byteArray = readAll();
    qDebug("%s",byteArray.toStdString().data());
    if(QString::fromStdString(byteArray.mid(0,8).toStdString()) == "list all")
        listApp();
    else if(QString::fromStdString(byteArray.mid(0,4).toStdString()) == "list")
    {
        QString appName = QString::fromStdString(byteArray.mid(4).toStdString());
        listApp(appName);
    }
    else if(QString::fromStdString(byteArray.mid(0,8).toStdString()) == "app info")
    {
        int appID = std::stoi(byteArray.mid(8).toStdString());
        getAppInfo(appID);
    }
}

void TcpSocket::listApp()
{
    QByteArray dataToSend;
    dataToSend.append("list all");
    QSqlQuery query;
    query.exec("select `app store`.`Application`.`App ID`,`app store`.`Application`.`App Name`,"
               "`app store`.`Application`.`Icon` from `app store`.`Application`");
    //qDebug("%d",s);

    while(query.next())
    {
        QByteArray idData = QString::number(query.value(0).toInt()).toUtf8();
        idData.resize(8);
        //qDebug("%s",idData.toStdString().data());

        QByteArray nameData = (query.value(1).toString()).toUtf8();
        //qDebug("%s",nameData.toStdString().data());

        QByteArray nameSizeData = QString::number(nameData.size(),16).toUtf8();
        nameSizeData.resize(2);

        QByteArray iconData = query.value(2).toByteArray();
        //qDebug("%d",iconData.size());

        QByteArray iconSizeData = QString::number(iconData.size(),16).toUtf8();
        iconSizeData.resize(4);

        dataToSend.append(idData);      //8B
        dataToSend.append(nameSizeData);        //1B
        dataToSend.append(nameData);
        dataToSend.append(iconSizeData);    //4B
        dataToSend.append(iconData);

        /*qDebug("%s",dataToSend.mid(0,8).data());
        qDebug("%d",std::stoi(dataToSend.mid(8,8).toStdString()));
        qDebug("%s",dataToSend.mid(16,10).data());
        qDebug("%d",std::stoi(dataToSend.mid(26,4).toStdString(),0,16));*/
    }
    write(dataToSend);
}

void TcpSocket::listApp(QString appName)
{
    QByteArray dataToSend;
    dataToSend.append("list one");
    QSqlQuery query;
    query.exec("select `app store`.`Application`.`App ID`,`app store`.`Application`.`App Name`,"
               "`app store`.`Application`.`Icon` from `app store`.`Application` where `Application`.`App Name` "
               "like '%" + appName + "%'");

    while(query.next())
    {
        QByteArray idData = QString::number(query.value(0).toInt()).toUtf8();
        idData.resize(8);

        QByteArray nameData = (query.value(1).toString()).toUtf8();

        QByteArray nameSizeData = QString::number(nameData.size(),16).toUtf8();
        nameSizeData.resize(2);

        QByteArray iconData = query.value(2).toByteArray();

        QByteArray iconSizeData = QString::number(iconData.size(),16).toUtf8();
        iconSizeData.resize(4);

        dataToSend.append(idData);      //8B
        dataToSend.append(nameSizeData);        //1B
        dataToSend.append(nameData);
        dataToSend.append(iconSizeData);    //4B
        dataToSend.append(iconData);
    }
    write(dataToSend);
}

void TcpSocket::getAppInfo(int appID)
{
    //qDebug("%d",appID);
    QByteArray dataToSend;
    dataToSend.append("app info");
    QSqlQuery query;
    query.exec("select `app store`.`Application`.`Introduction` from `app store`.`Application` where "
               "`Application`.`App ID` = " + QString::number(appID));
    while(query.next())
    {
        QByteArray introData = (query.value(0).toString()).toUtf8();

        QByteArray introSizeData = QString::number(introData.size()).toUtf8();
        introSizeData.resize(4);
        //qDebug("%d",introData.size());
        //qDebug("%s",introData.toStdString().data());
        dataToSend.append(introSizeData);      //4B
        dataToSend.append(introData);
    }
    write(dataToSend);
}
