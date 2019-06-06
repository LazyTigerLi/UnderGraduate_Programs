#include "table.h"
#include <QMessageBox>

Table::Table(QVector<QString> tableName, QWidget *parent)
    :QWidget(parent)
{
    mainLayout = new QHBoxLayout;
    for(int i = 0; i < tableName.size(); i++)
    {
        table.push_back(tableName[i]);
        QTableView *newTable = new QTableView(this);
        QSqlRelationalTableModel *newModel = new QSqlRelationalTableModel(this);
        newModel->setTable(tableName[i]);
        newModel->select();
        newModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
        newTable->setModel(newModel);
        //newTable->setItemDelegate(new QSqlRelationalDelegate(newTable));

        //newTable->setUpdatesEnabled(true);

        tableView.push_back(newTable);
        model.push_back(newModel);

        QPushButton *insertRow = new QPushButton(tr("insert"),this);
        QPushButton *deleteRow = new QPushButton(tr("delete"),this);
        QPushButton *submitAll = new QPushButton(tr("submit"),this);
        QPushButton *refreshTable = new QPushButton(tr("refresh"),this);
        connect(insertRow,SIGNAL(clicked(bool)),this,SLOT(insertRow()));
        connect(deleteRow,SIGNAL(clicked(bool)),this,SLOT(deleteRow()));
        connect(submitAll,SIGNAL(clicked(bool)),this,SLOT(submitAll()));
        connect(refreshTable,SIGNAL(clicked(bool)),this,SLOT(refresh()));
        insertRowButton.push_back(insertRow);
        deleteRowButton.push_back(deleteRow);
        submitAllButton.push_back(submitAll);
        refreshButton.push_back(refreshTable);

        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addWidget(insertRow);
        btnLayout->addWidget(deleteRow);
        btnLayout->addWidget(submitAll);
        btnLayout->addWidget(refreshTable);
        QVBoxLayout *tblLayout = new QVBoxLayout;
        tblLayout->addWidget(newTable);
        tblLayout->addLayout(btnLayout);
        buttonLayout.push_back(btnLayout);
        tableLayout.push_back(tblLayout);

        mainLayout->addLayout(tblLayout);
    }
    setLayout(mainLayout);
}

Table::~Table()
{}

void Table::insertRow()
{
    QPushButton *sender = qobject_cast<QPushButton *>(QObject::sender());
    int index;
    for(index = 0; index < insertRowButton.size(); index++)
        if(insertRowButton[index] == sender)break;

    int row = model[index]->rowCount();
    model[index]->insertRow(row);
    QModelIndex insertIndex = model[index]->index(row,0);
    tableView[index]->setCurrentIndex(insertIndex);
    tableView[index]->edit(insertIndex);
}

void Table::deleteRow()
{
    QPushButton *sender = qobject_cast<QPushButton *>(QObject::sender());
    int index;
    for(index = 0; index < deleteRowButton.size(); index++)
        if(deleteRowButton[index] == sender)break;

    int row = tableView[index]->currentIndex().row();
    model[index]->removeRow(row);
}

void Table::submitAll()
{
    QPushButton *sender = qobject_cast<QPushButton *>(QObject::sender());
    int index;
    for(index = 0; index < submitAllButton.size(); index++)
        if(submitAllButton[index] == sender)break;

    /*model[index]->database().transaction();
    if(model[index]->submitAll())model[index]->database().commit();
    else
    {
        model[index]->database().rollback();
        QMessageBox::warning(this,"error",model[index]->lastError().text().toUtf8());
    }*/
    if(!model[index]->submitAll())
    {
        QMessageBox::warning(this,"error",model[index]->lastError().text().toUtf8());
        model[index]->revertAll();
    }
    //tableView[index]->setModel(model[index]);
}

void Table::refresh()
{
    QPushButton *sender = qobject_cast<QPushButton *>(QObject::sender());
    int index;
    for(index = 0; index < refreshButton.size(); index++)
        if(refreshButton[index] == sender)break;
    model[index]->clear();
    model[index]->setTable(table[index]);
    model[index]->select();
    model[index]->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    tableView[index]->setModel(model[index]);
}
