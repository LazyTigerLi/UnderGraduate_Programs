#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>

class Table : public QWidget
{
    Q_OBJECT
public:
    Table(QVector<QString> tableName,QWidget *parent = nullptr);
    ~Table();
    QVector<QSqlRelationalTableModel*> model;
    QVector<QTableView*> tableView;

private:
    QVector<QPushButton*> insertRowButton;
    QVector<QPushButton*> deleteRowButton;
    QVector<QPushButton*> submitAllButton;

    QVector<QVBoxLayout*> tableLayout;
    QVector<QHBoxLayout*> buttonLayout;
    QHBoxLayout *mainLayout;

private slots:
    void insertRow();
    void deleteRow();
    void submitAll();
};

#endif // TABLE_H
