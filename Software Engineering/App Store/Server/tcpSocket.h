#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QString>
//#include <tuple>
#include <QHostAddress>
#include <QByteArray>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>

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
    QSqlDatabase db;
    void listApp();
    void listApp(QString appName);
    void getAppInfo(int appID);

private slots:
    void clientDisconnectedSlot();
    void analyzeRequest();

signals:
    void newMsg(QString);
    void clientDisconnected(int);

};

#endif // TCPSOCKET_H
