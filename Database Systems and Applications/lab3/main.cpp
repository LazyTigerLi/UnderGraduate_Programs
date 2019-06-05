#include "browser.h"
#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginDialog *w = new LoginDialog;
    w->show();

    return a.exec();
}
