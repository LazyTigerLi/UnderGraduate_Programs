#include "table.h"
#include <QMessageBox>

Table::Table(QVector<QString> tableName, QWidget *parent)
    :QWidget(parent)
{
    mainLayout = new QHBoxLayout;
    for(int i = 0; i < tableName.size(); i++)
    {
        QTableView *newTable = new QTableView(this);
        QSqlRelationalTableModel *newModel = new QSqlRelationalTableModel(this);
        newModel->setTable(tableName[i]);
        newModel->select();
        newModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
        newTable->setModel(newModel);
        newTable->setItemDelegate(new QSqlRelationalDelegate(newTable));
        tableView.push_back(newTable);
        model.push_back(newModel);

        QPushButton *insertRow = new QPushButton(tr("insert"),this);
        QPushButton *deleteRow = new QPushButton(tr("delete"),this);
        QPushButton *submitAll = new QPushButton(tr("submit"),this);
        connect(insertRow,SIGNAL(clicked(bool)),this,SLOT(insertRow()));
        connect(deleteRow,SIGNAL(clicked(bool)),this,SLOT(deleteRow()));
        connect(submitAll,SIGNAL(clicked(bool)),this,SLOT(submitAll()));
        insertRowButton.push_back(insertRow);
        deleteRowButton.push_back(deleteRow);
        submitAllButton.push_back(submitAll);

        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addWidget(insertRow);
        btnLayout->addWidget(deleteRow);
        btnLayout->addWidget(submitAll);
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
    int i;
    for(i = 0; i < insertRowButton.size(); i++)
        if(insertRowButton[i] == sender)break;

    int row = model[i]->rowCount();
    model[i]->insertRow(row);
    QModelIndex insertIndex = model[i]->index(row,0);
    tableView[i]->setCurrentIndex(insertIndex);
    tableView[i]->edit(insertIndex);
}

void Table::deleteRow()
{
    QPushButton *sender = qobject_cast<QPushButton *>(QObject::sender());
    int i;
    for(i = 0; i < deleteRowButton.size(); i++)
        if(deleteRowButton[i] == sender)break;

    QModelIndexList currentSelection = tableView[i]->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); i++)
    {
        if (currentSelection.at(i).column() != 0)continue;
        model[i]->removeRow(currentSelection.at(i).row());
    }
}

void Table::submitAll()
{
    QPushButton *sender = qobject_cast<QPushButton *>(QObject::sender());
    int i;
    for(i = 0; i < submitAllButton.size(); i++)
        if(submitAllButton[i] == sender)break;

    model[i]->database().transaction();
    if(model[i]->submitAll())model[i]->database().commit();
    else
    {
        model[i]->database().rollback();
        QMessageBox::warning(this,"error",model[i]->lastError().text().toUtf8());
    }
}
