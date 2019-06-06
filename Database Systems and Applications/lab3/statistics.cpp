#include "statistics.h"

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
    time->addButton(year);
    time->addButton(season);
    time->addButton(month);
    connect(year,SIGNAL(clicked(bool)),this,SLOT(displayTable()));
    connect(season,SIGNAL(clicked(bool)),this,SLOT(displayTable()));
    connect(month,SIGNAL(clicked(bool)),this,SLOT(displayTable()));

    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(year);
    buttonLayout->addWidget(season);
    buttonLayout->addWidget(month);

    savingView = new QTableView(this);
    savingModel = new QSqlQueryModel(savingView);
    savingModel->setQuery(QSqlQuery(yearSaving,db));
    savingView->setModel(savingModel);

    loanView = new QTableView(this);
    loanModel = new QSqlQueryModel(loanView);
    loanModel->setQuery(QSqlQuery(yearLoan,db));
    loanView->setModel(loanModel);

    tableLayout = new QVBoxLayout;
    tableLayout->addLayout(buttonLayout);
    tableLayout->addWidget(savingView);
    tableLayout->addWidget(loanView);

    setLayout(tableLayout);
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
}

void Statistics::displayChart()
{

}
