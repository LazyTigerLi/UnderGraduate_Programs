#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QTableWidget>
#include <tuple>

class Server;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(Server *s,QWidget *parent = 0);
    ~Widget();

private:
    QTextBrowser *connectionInfo;
    QTableWidget *appInfo;
    Server *server;

public slots:
    void update(QString req);
};

#endif // WIDGET_H
