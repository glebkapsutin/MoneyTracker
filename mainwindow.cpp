#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Настройка таблицы
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"Описание", "Сумма", "Тип"});

    // Настройка комбо-бокса
    ui->typeCombo->addItems({"Доход", "Расход"});

    // Создаем папку Reports в папке проекта, если она не существует
    QDir dir;
    QString reportsPath = "/Users/glebkapustin/Desktop/Cods/MoneyTracker/Reports";
    if (!dir.exists(reportsPath)) {
        dir.mkpath(reportsPath);
        qDebug() << "Создана папка:" << reportsPath;
    }

    // Загрузка данных из файла
    QString filePath = reportsPath + "/transactions.txt";
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 3) {
                Transaction t = {parts[0], parts[1].toDouble(), parts[2]};
                transactions.append(t);
            }
        }
        file.close();
        qDebug() << "Данные загружены из:" << filePath;
        updateTable();
    } else {
        qDebug() << "Не удалось открыть файл для чтения:" << filePath;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calculateSummary()
{
    double totalIncome = 0.0;
    double totalExpense = 0.0;

    for (const auto& t : transactions) {
        if (t.type == "Доход") {
            totalIncome += t.amount;
        } else if (t.type == "Расход") {
            totalExpense += t.amount;
        }
    }

    ui->incomeLabel->setText(QString("Доходы: %1").arg(totalIncome, 0, 'f', 2));
    ui->expenseLabel->setText(QString("Расходы: %1").arg(totalExpense, 0, 'f', 2));
}

void MainWindow::updateTable()
{
    ui->tableWidget->setRowCount(transactions.size());
    for (int i = 0; i < transactions.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(transactions[i].description));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(transactions[i].amount)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(transactions[i].type));
    }
    calculateSummary();
}

void MainWindow::on_addButton_clicked()
{
    QString description = ui->descriptionEdit->text();
    double amount = ui->amountEdit->text().toDouble();
    QString type = ui->typeCombo->currentText();

    if (!description.isEmpty() && amount > 0) {
        Transaction t = {description, amount, type};
        transactions.append(t);
        updateTable();
        ui->descriptionEdit->clear();
        ui->amountEdit->clear();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row >= 0) {
        transactions.remove(row);
        updateTable();
    }
}

void MainWindow::on_saveButton_clicked()
{
    QString reportsPath = "/Users/glebkapustin/Desktop/Cods/MoneyTracker/Reports";
    QString filePath = reportsPath + "/transactions.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл для записи:" << filePath;
        return;
    }
    QTextStream out(&file);
    for (const auto& t : transactions) {
        out << t.description << "," << t.amount << "," << t.type << "\n";
    }
    file.close();
    qDebug() << "Файл сохранен в:" << filePath;
}
