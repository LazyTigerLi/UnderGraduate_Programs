#ifndef STATITICS_H
#define STATITICS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTableView>
#include <QVector>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QSqlQueryModel>

class Statistics : public QWidget
{
    Q_OBJECT

public:
    Statistics(QSqlDatabase database,QWidget *parent = nullptr);
    ~Statistics();

private:
    QSqlDatabase db;
    QTableView *savingView;
    QTableView *loanView;
    QSqlQueryModel *savingModel;
    QSqlQueryModel *loanModel;

    QHBoxLayout *buttonLayout;
    QVBoxLayout *tableLayout;

    QButtonGroup *time;
    QRadioButton *year;
    QRadioButton *month;
    QRadioButton *season;

    QString yearSaving;
    QString yearLoan;
    QString seasonSaving;
    QString seasonLoan;
    QString monthSaving;
    QString monthLoan;

    void initializeQuery();

private slots:
    void displayTable();
    void displayChart();
};

#endif // STATITICS_H
