#ifndef APPPAGE_H
#define APPPAGE_H

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QAction>
#include <QMenu>
#include <QVBoxLayout>
#include <QPushButton>

class Client;

class AppPage : public QWidget
{
    Q_OBJECT

public:
    AppPage(Client *c,QWidget *parent = 0);
    ~AppPage();

protected:
    QToolButton *optionsButton;
    QToolButton *userButton;
    QPushButton *backButton;
    QPushButton *searchButton;
    QMenu *options;
    QMenu *user;
    QLineEdit *searchBar;
    QAction *uploadAction;
    QAction *updateAction;
    QAction *loginAction;
    QAction *signUpAction;

    QVBoxLayout *mainLayout;

public:
    Client *client;

private slots:
    void login();
    void signUp();
    virtual void analyzeReply() = 0;
};

#endif // APPPAGE
