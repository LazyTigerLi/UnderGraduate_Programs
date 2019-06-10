#include "logindialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>


LoginDialog::LoginDialog(AppPage *page, QTcpSocket *socket, QWidget *parent)
    :QDialog(parent)
{
    sock = socket;
    this->page = page;

    userNameLabel = new QLabel(tr("用户名"));
    passwdLabel = new QLabel(tr("密码"));
    userNameEdit = new QLineEdit;
    passwdEdit = new QLineEdit;
    passwdEdit->setEchoMode(QLineEdit::Password);
    connectButton = new QPushButton(tr("连接"));
    cancelButton = new QPushButton(tr("取消"));

    QGridLayout *upLayout = new QGridLayout;
    upLayout->addWidget(userNameLabel,0,0,1,1);
    upLayout->addWidget(userNameEdit,0,1,1,1);
    upLayout->addWidget(passwdLabel,1,0,1,1);
    upLayout->addWidget(passwdEdit,1,1,1,1);

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addWidget(connectButton);
    downLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(upLayout);
    mainLayout->addLayout(downLayout);

    setLayout(mainLayout);
    show();

    connect(connectButton,SIGNAL(clicked(bool)),this,SLOT(loginRequest()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(closeDialog()));
    disconnect(sock,SIGNAL(readyRead()),0,0);
    connect(sock,SIGNAL(readyRead()),this,SLOT(loginReply()));
}

LoginDialog::~LoginDialog()
{}

void LoginDialog::loginRequest()
{
    QByteArray dataToSend;
    QByteArray reqData = QString("login   ").toUtf8();
    QByteArray userName = userNameEdit->text().toUtf8();
    QByteArray password = passwdEdit->text().toUtf8();
    QByteArray userNameSize = QString::number(userName.size(),16).toUtf8();
    userNameSize.resize(1);
    QByteArray passwordSize = QString::number(password.size(),16).toUtf8();
    passwordSize.resize(1);

    dataToSend.append(reqData);
    dataToSend.append(userNameSize);
    dataToSend.append(userName);
    dataToSend.append(passwordSize);
    dataToSend.append(password);

    sock->write(dataToSend);
}

void LoginDialog::loginReply()
{
    QByteArray rcvMsg = sock->readAll();
    int bytes = 8;

    if(rcvMsg.mid(bytes,1).toStdString().data()[0] == '1')
    {
        disconnect(sock,SIGNAL(readyRead()),this,SLOT(loginReply()));
        close();
    }
    else
    {
        int errorSize = std::stoi(rcvMsg.mid(bytes + 1,2).toStdString(),0,16);
        QString error = QString::fromStdString(rcvMsg.mid(bytes + 3,errorSize).toStdString());
        QMessageBox::information(this, tr("登录失败"),error);
    }
}

void LoginDialog::closeDialog()
{
    disconnect(sock,SIGNAL(readyRead()),this,SLOT(loginReply()));
    close();
}
