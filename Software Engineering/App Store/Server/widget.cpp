#include "widget.h"
#include <QVBoxLayout>
#include <QString>

Widget::Widget(Server *s, QWidget *parent)
    : QWidget(parent)
{
    connectionInfo = new QTextBrowser;
    appInfo = new QTableWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(connectionInfo);
    mainLayout->addWidget(appInfo);
    setLayout(mainLayout);

    server = s;
}

Widget::~Widget()
{

}

void Widget::update(QString req)
{
    connectionInfo->append(req);
}
