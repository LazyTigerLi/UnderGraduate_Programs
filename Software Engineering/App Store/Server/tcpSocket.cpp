#include "tcpSocket.h"
#include <QDateTime>
#include <string>
#include <QDebug>

TcpSocket::TcpSocket(QSqlDatabase database)
{
    db = database;
    state = AnalyzeRequest;
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
    rcvMsg = readAll();
    //qDebug("%s",byteArray.toStdString().data());
    if(state == AnalyzeRequest)
    {
        if(QString::fromStdString(rcvMsg.mid(0,8).toStdString()) == "list all")
            listApp();
        else if(QString::fromStdString(rcvMsg.mid(0,4).toStdString()) == "list")
        {
            QString appName = QString::fromStdString(rcvMsg.mid(4).toStdString());
            listApp(appName);
        }
        else if(QString::fromStdString(rcvMsg.mid(0,8).toStdString()) == "app info")
        {
            int appID = std::stoi(rcvMsg.mid(8).toStdString());
            getAppInfo(appID);
        }
        else if(QString::fromStdString(rcvMsg.mid(0,8).toStdString()) == "login   ")
        {
            int userNameSize = std::stoi(rcvMsg.mid(8,1).toStdString(),0,16);
            QString userName = QString::fromStdString(rcvMsg.mid(9,userNameSize).toStdString());
            int passwordSize = std::stoi(rcvMsg.mid(9 + userNameSize,1).toStdString(),0,16);
            QString password = QString::fromStdString(rcvMsg.mid(10 + userNameSize,passwordSize).toStdString());
            login(userName,password);
        }
        else if(QString::fromStdString(rcvMsg.mid(0,8).toStdString()) == "signup  ")
        {
            int userNameSize = std::stoi(rcvMsg.mid(8,1).toStdString(),0,16);
            QString userName = QString::fromStdString(rcvMsg.mid(9,userNameSize).toStdString());
            int passwordSize = std::stoi(rcvMsg.mid(9 + userNameSize,1).toStdString(),0,16);
            QString password = QString::fromStdString(rcvMsg.mid(10 + userNameSize,passwordSize).toStdString());
            signUp(userName,password);
        }
        else if(QString::fromStdString(rcvMsg.mid(0,8).toStdString()) == "app down")
        {
            int appID = std::stoi(rcvMsg.mid(8).toStdString());
            download(appID);
        }
        else if(QString::fromStdString(rcvMsg.mid(0,8).toStdString()) == "app send")
        {
            fileSize = 0;
            rcvSize = 0;
            upload();
        }
    }
    else if(state == Upload)
        rcvFile();
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

void TcpSocket::login(QString userName, QString password)
{
    QByteArray dataToSend;
    QByteArray success;
    QByteArray isDeveloper;
    QByteArray errorMsg;
    QByteArray errorMsgSize;
    dataToSend.append("login   ");
    QSqlQuery query;
    query.exec("select `Client`.`Password`,`Client`.`Is Developer` from `app store`.`Client` where "
               "`Client`.`ID` = '" + userName + "'");
    if(!query.next())
    {
        success = QByteArray(1,'0');
        QString error = "The user does not exist!";
        errorMsg = error.toUtf8();
        errorMsgSize = QString::number(errorMsg.size(),16).toUtf8();
        dataToSend.append(success);
        dataToSend.append(errorMsgSize);
        dataToSend.append(errorMsg);
    }
    else if(query.value(0).toString() != password)
    {
        success = QByteArray(1,'0');
        QString error = "The password is wrong!";
        errorMsg = error.toUtf8();
        errorMsgSize = QString::number(errorMsg.size(),16).toUtf8();
        dataToSend.append(success);
        dataToSend.append(errorMsgSize);
        dataToSend.append(errorMsg);
    }
    else
    {
        success = QByteArray(1,'1');
        if(query.value(1).toInt() == 1)isDeveloper = QByteArray(1,'1');
        else isDeveloper = QByteArray(1,'0');
        dataToSend.append(success);
        dataToSend.append(isDeveloper);
        /*query.exec("update `App Store`.`Client` set `Client`.`Is Online` = 1 where "
                   "`Client`.`ID` = '" + userName + "'");*/
    }
    write(dataToSend);
}

void TcpSocket::signUp(QString userName, QString password)
{
    QByteArray dataToSend;
    QByteArray success;
    QByteArray errorMsg;
    QByteArray errorMsgSize;
    dataToSend.append("login   ");
    QSqlQuery query;
    query.exec("select * from `app store`.`Client` where "
               "`Client`.`ID` = '" + userName + "'");
    if(query.next())
    {
        success = QByteArray(1,'0');
        QString error = "The username has be used!";
        errorMsg = error.toUtf8();
        errorMsgSize = QString::number(errorMsg.size(),16).toUtf8();
        dataToSend.append(success);
        dataToSend.append(errorMsgSize);
        dataToSend.append(errorMsg);
    }
    else
    {
        success = QByteArray(1,'1');
        dataToSend.append(success);
        query.exec("insert into `app store`.`Client`(`Client`.`ID`,`Client`.`Password`,`Client`.`Is Developer`)"
                   " values('" + userName + "','" + password + "',0)");
    }
    write(dataToSend);
}

void TcpSocket::download(int appID)
{
    QByteArray dataToSend;
    dataToSend.append("app down");
    QSqlQuery query;
    query.exec("select `Application`.`Package` from `app store`.`Application` where "
               "`Application`.`App ID` = " + QString::number(appID));

    query.next();

    QByteArray appData = query.value(0).toByteArray();
    QByteArray appSizeData = QString::number(appData.size(),16).toUtf8();
    appSizeData.resize(8);

    dataToSend.append(appSizeData);

    int blockSize = 64 * 1024;
    int sendTimes = appData.size() / blockSize;
    for(int i = 0; i < sendTimes; i++)
    {
        dataToSend.append(appData.mid(blockSize * i,blockSize));
        write(dataToSend);
        qDebug("%d",dataToSend.size());
        dataToSend.clear();
    }
    if(sendTimes * blockSize < appData.size())
    {
        dataToSend.append(appData.mid(sendTimes * blockSize));
        write(dataToSend);
    }
}

void TcpSocket::upload()
{
    int bytes = 8;
    int nameSize = std::stoi(rcvMsg.mid(bytes,2).toStdString(),0,16);
    QString name = QString::fromStdString(rcvMsg.mid(bytes + 2,nameSize).toStdString());

    int appSize = std::stoi(rcvMsg.mid(bytes + 2 + nameSize,8).toStdString(),0,16);
    fileSize = appSize;

    QByteArray appData = rcvMsg.mid(bytes + 10 + nameSize);
    rcvSize = appData.size();

    QSqlQuery query;
    query.exec("select max(`Application`.`App ID`) from `app store`.`Application`");
    query.next();
    QString appID = QString::number(query.value(0).toInt());

    appFile = new QFile(appPath + appID);
    if(!appFile->exists(appPath + appID))
    {
        appFile->setFileName(appPath + appID);
        appFile->open(QIODevice::WriteOnly);
        appFile->write(appData);
        //使用QDataStream会在文件首部多4个字节，为文件大小
        if(appSize == rcvSize)
        {
            appFile->close();
            delete appFile;
            state = AnalyzeRequest;
            return;
        }
        else state = Upload;
        return;
    }
    state = AnalyzeRequest;
}

void TcpSocket::rcvFile()
{
    if(QString::fromStdString(rcvMsg.mid(0,8).toStdString()) == "cancelup")
    {
        fileSize = 0;
        rcvSize = 0;
        delete appFile;
        state = AnalyzeRequest;
        return;
    }
    QByteArray appData = rcvMsg;
    appFile->write(appData);
    rcvSize += appData.size();
    if(rcvSize == fileSize)     //所有数据都已接收
    {
        appFile->close();
        delete appFile;
        state = AnalyzeRequest;
    }
    else state = Upload;
}
