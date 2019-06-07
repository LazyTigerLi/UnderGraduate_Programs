#include "statistics.h"
#include <QMap>
#include <QList>
#include <QLegend>
#include <QLegendMarker>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QDebug>

Statistics::Statistics(QSqlDatabase database, QWidget *parent)
    :QWidget(parent)
{
    initializeQuery();

    db = database;

    time = new QButtonGroup(this);
    year = new QRadioButton("year",this);
    year->setChecked(true);
    season = new QRadioButton("season",this);
    month = new QRadioButton("month",this);
    //month->setChecked(true);
    time->addButton(year);
    time->addButton(season);
    time->addButton(month);
    refresh = new QPushButton(tr("refresh"),this);
    connect(year,SIGNAL(clicked(bool)),this,SLOT(displayTable()));
    connect(season,SIGNAL(clicked(bool)),this,SLOT(displayTable()));
    connect(month,SIGNAL(clicked(bool)),this,SLOT(displayTable()));
    connect(refresh,SIGNAL(clicked(bool)),this,SLOT(displayTable()));
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(year);
    buttonLayout->addWidget(season);
    buttonLayout->addWidget(month);
    buttonLayout->addWidget(refresh);

    savingView = new QTableView(this);
    savingModel = new QSqlQueryModel(savingView);
    savingView->setModel(savingModel);
    loanView = new QTableView(this);
    loanModel = new QSqlQueryModel(loanView);
    loanView->setModel(loanModel);
    tableLayout = new QVBoxLayout;
    tableLayout->addLayout(buttonLayout);
    tableLayout->addWidget(savingView);
    tableLayout->addWidget(loanView);


    type = new QButtonGroup(this);
    money = new QRadioButton("money",this);
    client = new QRadioButton("client",this);
    money->setChecked(true);
    type->addButton(money);
    type->addButton(client);
    connect(money,SIGNAL(clicked(bool)),this,SLOT(displayChart()));
    connect(client,SIGNAL(clicked(bool)),this,SLOT(displayChart()));
    buttonLayout2 = new QHBoxLayout;
    buttonLayout2->addWidget(money);
    buttonLayout2->addWidget(client);

    savingChart = new QChart;
    loanChart = new QChart;
    savingChartView = new QChartView(this);
    loanChartView = new QChartView(this);
    chartLayout = new QVBoxLayout;
    chartLayout->addLayout(buttonLayout2);
    chartLayout->addWidget(savingChartView);
    chartLayout->addWidget(loanChartView);


    displayTable();
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(tableLayout);
    mainLayout->addLayout(chartLayout);
    mainLayout->setStretchFactor(tableLayout,1);
    mainLayout->setStretchFactor(chartLayout,1);
    setLayout(mainLayout);
}

Statistics::~Statistics()
{}

void Statistics::initializeQuery()
{
    yearSaving = "select * from `Year Saving Statistics`";
    yearLoan = "select * from `Year Loan Statistics`";
    seasonSaving = "select * from `Season Saving Statistics`";
    seasonLoan = "select * from `Season Loan Statistics`";
    monthSaving = "select * from `Month Saving Statistics`";
    monthLoan = "select * from `Month Loan Statistics`";
}

void Statistics::displayTable()
{
    if(year->isChecked())
    {
        savingModel->setQuery(QSqlQuery(yearSaving,db));
        loanModel->setQuery(QSqlQuery(yearLoan,db));
    }
    else if(season->isChecked())
    {
        savingModel->setQuery(QSqlQuery(seasonSaving,db));
        loanModel->setQuery(QSqlQuery(seasonLoan,db));
    }
    else
    {
        savingModel->setQuery(QSqlQuery(monthSaving,db));
        loanModel->setQuery(QSqlQuery(monthLoan,db));
    }
    displayChart();
}

void Statistics::displayChart()
{
    if(!savingChart)delete savingChart;
    if(!loanChart)delete loanChart;
    savingChart = new QChart;
    loanChart = new QChart;
    savingChartView->setChart(savingChart);
    loanChartView->setChart(loanChart);

    QMap<QString,int> bankIndexSaving,bankIndexLoan;                //银行的索引号
    int indexSaving = 0,indexLoan = 0;
    QVector<QLineSeries *> lineSeriesSaving,lineSeriesLoan;
    QVector<QMap<QDateTime,qreal>> pointSaving,pointLoan;                  //所有的数据点

    QDateTimeAxis *axisXSaving = new QDateTimeAxis;
    QValueAxis *axisYSaving = new QValueAxis;
    QDateTimeAxis *axisXLoan = new QDateTimeAxis;
    QValueAxis *axisYLoan = new QValueAxis;
    /*QDateTime minDate,maxDate;
    minDate.setDate(QDate(2050,1,1));
    maxDate.setDate(QDate(1950,1,1));
    axisXSaving->setMax(maxDate);
    axisXSaving->setMin(minDate);
    axisXLoan->setMax(maxDate);
    axisXLoan->setMin(minDate);*/
    axisYLoan->setRange(0,0);
    axisYSaving->setRange(0,0);

    savingChart->addAxis(axisXSaving,Qt::AlignBottom);
    savingChart->addAxis(axisYSaving,Qt::AlignLeft);
    loanChart->addAxis(axisXLoan,Qt::AlignBottom);
    loanChart->addAxis(axisYLoan,Qt::AlignLeft);

    if(money->isChecked())
    {
        for(int i = 0; i < savingModel->rowCount(); i++)
        {
            QString time = savingModel->data(savingModel->index(i,0)).toString();
            QList<QString> date = time.split('-');
            int y,m = 1,d = 1;
            y = date[0].toInt();
            if(date.size() > 1)m = date[1].toInt();
            QDateTime dateTime;          //获取横坐标
            dateTime.setDate(QDate(y,m,d));
            QString bankName = savingModel->data(savingModel->index(i,1)).toString();
            qreal money = savingModel->data(savingModel->index(i,2)).toReal();      //获取纵坐标
            auto it = bankIndexSaving.find(bankName);
            if(it == bankIndexSaving.end())             //添加新的系列
            {
                bankIndexSaving.insert(bankName,indexSaving);
                QLineSeries *newSeries = new QLineSeries;
                QMap<QDateTime,qreal> newPoints;
                newSeries->setName(bankName);
                savingChart->addSeries(newSeries);
                pointSaving.push_back(newPoints);
                newSeries->attachAxis(axisXSaving);
                newSeries->attachAxis(axisYSaving);
                lineSeriesSaving.push_back(newSeries);
                if(indexSaving == 0)
                {
                    if(year->isChecked())axisXSaving->setRange(dateTime,dateTime.addYears(1));
                    else if(season->isChecked())axisXSaving->setRange(dateTime,dateTime.addMonths(3));
                    else axisXSaving->setRange(dateTime,dateTime.addMonths(1));
                }
                indexSaving++;
            }
            axisXSaving->setMin((axisXSaving->min() > dateTime) ? dateTime : axisXSaving->min());
            axisXSaving->setMax((axisXSaving->max() < dateTime) ? dateTime : axisXSaving->max());
            axisYSaving->setMin((axisYSaving->min() > money) ? money : axisYSaving->min());
            axisYSaving->setMax((axisYSaving->max() < money) ? money : axisYSaving->max());
            //qDebug("%s",axisXSaving->min().toString("yyyy:mm:dd"));
            pointSaving[bankIndexSaving[bankName]].insert(dateTime,money);
        }
        auto i = axisXSaving->min();
        while(i <= axisXSaving->max())
        {
            for(auto it = bankIndexSaving.begin(); it != bankIndexSaving.end(); it++)
            {
                int index = bankIndexSaving[it.key()];
                auto value = pointSaving[index].find(i);
                if(value == pointSaving[index].end())lineSeriesSaving[index]->append(i.toMSecsSinceEpoch(),0);
                else lineSeriesSaving[index]->append(i.toMSecsSinceEpoch(),value.value());
            }
            if(year->isChecked())i = i.addYears(1);
            else if(season->isChecked())i = i.addMonths(3);
            else i = i.addMonths(1);
        }

        for(int i = 0; i < loanModel->rowCount(); i++)
        {
            QString time = loanModel->data(loanModel->index(i,0)).toString();
            QList<QString> date = time.split('-');
            int y,m = 1,d = 1;
            y = date[0].toInt();
            if(date.size() > 1)m = date[1].toInt();
            QDateTime dateTime;          //获取横坐标
            dateTime.setDate(QDate(y,m,d));
            QString bankName = loanModel->data(loanModel->index(i,1)).toString();
            qreal money = loanModel->data(loanModel->index(i,2)).toReal();      //获取纵坐标
            auto it = bankIndexLoan.find(bankName);
            if(it == bankIndexLoan.end())             //添加新的系列
            {
                bankIndexLoan.insert(bankName,indexLoan);
                QLineSeries *newSeries = new QLineSeries;
                QMap<QDateTime,qreal> newPoints;
                newSeries->setName(bankName);
                loanChart->addSeries(newSeries);
                pointLoan.push_back(newPoints);
                newSeries->attachAxis(axisXLoan);
                newSeries->attachAxis(axisYLoan);
                lineSeriesLoan.push_back(newSeries);
                if(indexLoan == 0)
                {
                    if(year->isChecked())axisXLoan->setRange(dateTime,dateTime.addYears(1));
                    else if(season->isChecked())axisXLoan->setRange(dateTime,dateTime.addMonths(3));
                    else axisXLoan->setRange(dateTime,dateTime.addMonths(1));
                }
                indexLoan++;
            }
            axisXLoan->setMin((axisXLoan->min() > dateTime) ? dateTime : axisXLoan->min());
            axisXLoan->setMax((axisXLoan->max() < dateTime) ? dateTime : axisXLoan->max());
            axisYLoan->setMin((axisYLoan->min() > money) ? money : axisYLoan->min());
            axisYLoan->setMax((axisYLoan->max() < money) ? money : axisYLoan->max());
            pointLoan[bankIndexLoan[bankName]].insert(dateTime,money);
        }
        i = axisXLoan->min();
        while(i <= axisXLoan->max())
        {
            for(auto it = bankIndexLoan.begin(); it != bankIndexLoan.end(); it++)
            {
                int index = bankIndexLoan[it.key()];
                auto value = pointLoan[index].find(i);
                if(value == pointLoan[index].end())lineSeriesLoan[index]->append(i.toMSecsSinceEpoch(),0);
                else lineSeriesLoan[index]->append(i.toMSecsSinceEpoch(),value.value());
            }
            if(year->isChecked())i = i.addYears(1);
            else if(season->isChecked())i = i.addMonths(3);
            else i = i.addMonths(1);
        }
    }
    else
    {
        for(int i = 0; i < savingModel->rowCount(); i++)
        {
            QString time = savingModel->data(savingModel->index(i,0)).toString();
            QList<QString> date = time.split('-');
            int y,m = 1,d = 1;
            y = date[0].toInt();
            if(date.size() > 1)m = date[1].toInt();
            QDateTime dateTime;          //获取横坐标
            dateTime.setDate(QDate(y,m,d));
            QString bankName = savingModel->data(savingModel->index(i,1)).toString();
            qreal client = savingModel->data(savingModel->index(i,3)).toReal();      //获取纵坐标
            auto it = bankIndexSaving.find(bankName);
            if(it == bankIndexSaving.end())             //添加新的系列
            {
                bankIndexSaving.insert(bankName,indexSaving);
                QLineSeries *newSeries = new QLineSeries;
                QMap<QDateTime,qreal> newPoints;
                newSeries->setName(bankName);
                savingChart->addSeries(newSeries);
                pointSaving.push_back(newPoints);
                newSeries->attachAxis(axisXSaving);
                newSeries->attachAxis(axisYSaving);
                lineSeriesSaving.push_back(newSeries);
                if(indexSaving == 0)
                {
                    if(year->isChecked())axisXSaving->setRange(dateTime,dateTime.addYears(1));
                    else if(season->isChecked())axisXSaving->setRange(dateTime,dateTime.addMonths(3));
                    else axisXSaving->setRange(dateTime,dateTime.addMonths(1));
                }
                indexSaving++;
            }
            axisXSaving->setMin((axisXSaving->min() > dateTime) ? dateTime : axisXSaving->min());
            axisXSaving->setMax((axisXSaving->max() < dateTime) ? dateTime : axisXSaving->max());
            axisYSaving->setMin((axisYSaving->min() > client) ? client : axisYSaving->min());
            axisYSaving->setMax((axisYSaving->max() < client) ? client : axisYSaving->max());
            //qDebug("%s",axisXSaving->min().toString("yyyy:mm:dd"));
            pointSaving[bankIndexSaving[bankName]].insert(dateTime,client);
        }
        auto i = axisXSaving->min();
        while(i <= axisXSaving->max())
        {
            for(auto it = bankIndexSaving.begin(); it != bankIndexSaving.end(); it++)
            {
                int index = bankIndexSaving[it.key()];
                auto value = pointSaving[index].find(i);
                if(value == pointSaving[index].end())lineSeriesSaving[index]->append(i.toMSecsSinceEpoch(),0);
                else lineSeriesSaving[index]->append(i.toMSecsSinceEpoch(),value.value());
            }
            if(year->isChecked())i = i.addYears(1);
            else if(season->isChecked())i = i.addMonths(3);
            else i = i.addMonths(1);
        }

        for(int i = 0; i < loanModel->rowCount(); i++)
        {
            QString time = loanModel->data(loanModel->index(i,0)).toString();
            QList<QString> date = time.split('-');
            int y,m = 1,d = 1;
            y = date[0].toInt();
            if(date.size() > 1)m = date[1].toInt();
            QDateTime dateTime;          //获取横坐标
            dateTime.setDate(QDate(y,m,d));
            QString bankName = loanModel->data(loanModel->index(i,1)).toString();
            qreal client = loanModel->data(loanModel->index(i,3)).toReal();      //获取纵坐标
            auto it = bankIndexLoan.find(bankName);
            if(it == bankIndexLoan.end())             //添加新的系列
            {
                bankIndexLoan.insert(bankName,indexLoan);
                QLineSeries *newSeries = new QLineSeries;
                QMap<QDateTime,qreal> newPoints;
                newSeries->setName(bankName);
                loanChart->addSeries(newSeries);
                pointLoan.push_back(newPoints);
                newSeries->attachAxis(axisXLoan);
                newSeries->attachAxis(axisYLoan);
                lineSeriesLoan.push_back(newSeries);
                if(indexLoan == 0)
                {
                    if(year->isChecked())axisXLoan->setRange(dateTime,dateTime.addYears(1));
                    else if(season->isChecked())axisXLoan->setRange(dateTime,dateTime.addMonths(3));
                    else axisXLoan->setRange(dateTime,dateTime.addMonths(1));
                }
                indexLoan++;
            }
            axisXLoan->setMin((axisXLoan->min() > dateTime) ? dateTime : axisXLoan->min());
            axisXLoan->setMax((axisXLoan->max() < dateTime) ? dateTime : axisXLoan->max());
            axisYLoan->setMin((axisYLoan->min() > client) ? client : axisYLoan->min());
            axisYLoan->setMax((axisYLoan->max() < client) ? client : axisYLoan->max());
            pointLoan[bankIndexLoan[bankName]].insert(dateTime,client);
        }
        i = axisXLoan->min();
        while(i <= axisXLoan->max())
        {
            for(auto it = bankIndexLoan.begin(); it != bankIndexLoan.end(); it++)
            {
                int index = bankIndexLoan[it.key()];
                auto value = pointLoan[index].find(i);
                if(value == pointLoan[index].end())lineSeriesLoan[index]->append(i.toMSecsSinceEpoch(),0);
                else lineSeriesLoan[index]->append(i.toMSecsSinceEpoch(),value.value());
            }
            if(year->isChecked())i = i.addYears(1);
            else if(season->isChecked())i = i.addMonths(3);
            else i = i.addMonths(1);
        }
    }
}
