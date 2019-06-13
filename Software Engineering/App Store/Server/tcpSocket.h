#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QString>
//#include <tuple>
#include <QHostAddress>
#include <QByteArray>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

struct Request
{
    QHostAddress ip;
    int userId;
    int appId;
    QString appName;
    bool isDeveloper;
};

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket(QSqlDatabase database);
    ~TcpSocket();
    Request req;

private:
    enum State{AnalyzeRequest,Upload};

    QSqlDatabase db;
    void listApp();
    void listApp(QString appName);
    void getAppInfo(int appID);
    void login(QString userName,QString password);
    void signUp(QString userName,QString password);
    void download(int appID);
    void upload();
    void rcvFile();

    QString appPath = "/home/linan/Server/app/";
    int fileSize;
    int rcvSize;
    QFile *appFile = nullptr;

    State state;
    QByteArray rcvMsg;

private slots:
    void clientDisconnectedSlot();
    void analyzeRequest();

signals:
    void newMsg(QString);
    void clientDisconnected(int);
};

#endif // TCPSOCKET_H
