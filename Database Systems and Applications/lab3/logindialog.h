#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

class LoginDialog: public QDialog
{
    Q_OBJECT
public:
    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    QString userName;
    QString passwd;

    QLabel *userNameLabel;
    QLabel *passwdLabel;
    QLineEdit *userNameEdit;
    QLineEdit *passwdEdit;
    QPushButton *connectButton;
    QPushButton *cancelButton;

private slots:
    void login();
};

#endif // LOGINDIALOG_H
