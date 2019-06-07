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
#include <QPushButton>
#include <QButtonGroup>
#include <QSqlQueryModel>
#include <QChart>
#include <QChartView>
#include <QLineSeries>


using namespace QT_CHARTS_NAMESPACE;

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
    QHBoxLayout *buttonLayout2;
    QVBoxLayout *chartLayout;

    QButtonGroup *time;
    QButtonGroup *type;
    QRadioButton *year;
    QRadioButton *month;
    QRadioButton *season;
    QRadioButton *money;
    QRadioButton *client;
    QPushButton *refresh;

    QChart *savingChart;
    QChart *loanChart;
    QChartView *savingChartView;
    QChartView *loanChartView;

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
