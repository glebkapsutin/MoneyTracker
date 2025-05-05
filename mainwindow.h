#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    struct Transaction {
        QString description;
        double amount;
        QString type; // "Доход" или "Расход"
    };

    QVector<Transaction> transactions;

    void updateTable();
    void calculateSummary(); // Новый метод для расчета доходов и расходов

private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_saveButton_clicked();
};

#endif // MAINWINDOW_H
