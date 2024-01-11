#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    b1_1 b1;
//    b1.task_1_1(10, 10);


}

MainWindow::~MainWindow()
{
    delete ui;
}

// Билет 2 Задание 2
void MainWindow::on_pushButton_2_1_clicked()
{
    qDebug() << "=================================================";
    qDebug() << "Билет 2 Задание 2";

    int rows = 3, columns = 5;

    int arr[rows][columns];
    for(int i = 0; i < rows; i++){
        QString print = "";
        for(int j = 0; j < columns; j++){
            arr[i][j] = QRandomGenerator::global()->bounded(0, 5);
            print += QString::number(arr[i][j]) + " ";
        }
        qDebug() << print;
    }
    qDebug() << "=================================================";

    int select;
    if(ui->comboBox_2_1->currentText() == "чет")
        select = 0;
    if(ui->comboBox_2_1->currentText() == "нечет")
        select = 1;

    int sum = 0;
    for(int i = 0; i < columns; i++){
        if(((i + 1) % 2) != select)
            continue;

        for(int j = 0; j < rows; j++)
            sum += arr[j][i];
    }

    ui->label_res_2_1->setText(QString::number(sum));
}

// Билет 3 Задание 2
void MainWindow::on_pushButton_3_1_clicked()
{
    qDebug() << "=================================================";
    qDebug() << "Билет 2 Задание 2";

    int k = ui->spinBox_3_1->value();
    int rows = 3, columns = 13;
    if(k > 13)
        ui->label_res_3_1->setText("k больше, чем число столбцов");

    int arr[rows][columns];
    for(int i = 0; i < rows; i++){
        QString print = "";
        for(int j = 0; j < columns; j++){
            arr[i][j] = QRandomGenerator::global()->bounded(0, 5);
            print += QString::number(arr[i][j]) + " ";
        }
        qDebug() << print;
    }
    qDebug() << "=================================================";

    int sum = 0, mult = 1;
    for(int i = 0; i < columns; i++){
        if(i != k - 1)
            continue;

        for(int j = 0; j < rows; j++){
            if(ui->comboBox_3_1->currentText() == "sum")
                sum += arr[j][i];

            else if(ui->comboBox_3_1->currentText() == "mult")
                mult *= arr[j][i];
        }
    }

    if(ui->comboBox_3_1->currentText() == "sum")
        ui->label_res_3_1->setText(QString::number(sum));
    else if(ui->comboBox_3_1->currentText() == "mult")
        ui->label_res_3_1->setText(QString::number(mult));
}

