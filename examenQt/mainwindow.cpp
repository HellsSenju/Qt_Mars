#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Билет 1 Задание 3 - заполнение листа и листВиджет
    for(int i = 0; i < 10; i++)
       list_1_3 << QRandomGenerator::global()->bounded(0, 10);

    QList<int>::iterator it = list_1_3.begin();
    while(it != list_1_3.end()){
        ui->listWidget_1_3->addItem(QString::number(*it) + " ");
        it ++;
    }
    //

    b_1_4();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Билет 1 Задание 2
// поменять местами максимальный и минимальный элемент в каждой строке
void MainWindow::b_1_2()
{
    qDebug() << "=================================================";
    qDebug() << "Билет 1 Задание 2";

    int rows = 3, columns = 5;

    int arr[rows][columns];
    for(int i = 0; i < rows; i++){
        QString print = "";
        for(int j = 0; j < columns; j++){
            arr[i][j] = QRandomGenerator::global()->bounded(-100, 100);
            print += QString::number(arr[i][j]) + " ";
        }
        qDebug() << print;
    }


    for(int i = 0; i < rows; i++){
        int max = -1000, min = 1000, maxIndex = -1, minIndex = -1;

        for(int j = 0; j < columns; j++){
            if(arr[i][j] > max){
                max = arr[i][j];
                maxIndex = j;
            }

            if(arr[i][j] < min){
                min = arr[i][j];
                minIndex = j;
            }
        }

        arr[i][maxIndex] = min;
        arr[i][minIndex] = max;
    }

    qDebug() << "Билет 1 Задание 2 результат: ";
    for(int i = 0; i < rows; i++){
        QString print = "";
        for(int j = 0; j < columns; j++){
            print += QString::number(arr[i][j]) + " ";
        }
        qDebug() << print;
    }

    qDebug() << "=================================================";

}

// Билет 1 Задание 3
//заполнить список рандомно. доавбление в конец, удаление с начала
void MainWindow::b_1_3()
{
    if(ui->radioButton_1_3_end->isChecked()){
        list_1_3.push_back(QRandomGenerator::global()->bounded(0, 10));
        ui->listWidget_1_3->addItem(QString::number(list_1_3.last()) + " ");
    }

    else if(ui->radioButton_1_3_beginning->isChecked()){
        list_1_3.pop_front();
        ui->listWidget_1_3->clear();

        QList<int>::iterator it = list_1_3.begin();
        while(it != list_1_3.end()){
            ui->listWidget_1_3->addItem(QString::number(*it) + " ");
            it ++;
        }
    }
}

// Билет 1 Задание 4
// 1. найти сумму компонент файла А и записать в файл В
// 2. последний компонент файла А и 32 запишите его в файл С
void MainWindow::b_1_4()
{
    QFile file_A("file_A.txt");
    QFile file_B("file_B.txt");
    QFile file_C("file_C.txt");
    if(!file_A.exists("file_A.txt") && !file_B.exists("file_B.txt") && !file_C.exists("file_C.txt"))
    {
        qDebug() << "Файл не существует";
        return;
    }

    if(file_A.open(QIODevice::ReadOnly) &&
       file_B.open(QIODevice::WriteOnly | QIODevice::Text) &&
       file_C.open(QIODevice::WriteOnly| QIODevice::Text))
    {

        int sum = 0;
        int last = 32;
        while (!file_A.atEnd()) {
            QByteArray line = file_A.readLine();
            if(file_A.atEnd())
                last += line.toInt();

            sum += line.toInt();
        }

        QTextStream writeStream(&file_B);
        writeStream << QString::number(sum);

        QTextStream writeStream_C(&file_C);
        writeStream_C << QString::number(last);

        file_A.close();
        file_B.close();
        file_C.close();
    }
}

// Билет 2 Задание 2
void MainWindow::b_2_2()
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
void MainWindow::b_3_2()
{
    qDebug() << "=================================================";
    qDebug() << "Билет 3 Задание 2";

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

// Билет 4 Задание 2
// по каждой строке найти кол-во элементов, больших ср ариф всех элем строки
void MainWindow::b_4_2()
{
    qDebug() << "=================================================";
    qDebug() << "Билет 4 Задание 2";

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

    for(int i = 0; i < rows; i++){
        double srznach = 0;
        int kol = 0;
        for(int j = 0; j < columns; j++)
            srznach += arr[i][j];

        srznach /= columns;
        for(int j = 0; j < columns; j++)
            if(arr[i][j] > srznach)
                kol++;
        qDebug() << "в строке - " << i+1 << ", среднее арифм - " << srznach << ", количество - " << kol;
    }
}

// Билет 6 Задание 2
// дан интовый массив, прибавить к четным элементам первый элемент, первый и послдений элементы не трогать
void MainWindow::b_6_2()
{
    qDebug() << "=================================================";
    qDebug() << "Билет 6 Задание 2";

    int size = 10;

    int arr[size];
    QString print = "";
    for(int i = 0; i < size; i++){
        arr[i] = QRandomGenerator::global()->bounded(0, 5);
        print += QString::number(arr[i]) + " ";
    }
    qDebug() << print;

    for(int i = 1; i < size - 1; i++){
        if(arr[i] % 2 == 0)
            arr[i] += arr[0];
    }

    qDebug() << "Результат:";
    print = "";
    for(int i = 0; i < size; i++){
        print += QString::number(arr[i]) + " ";
    }
    qDebug() << print;
    qDebug() << "=================================================";
}


void MainWindow::on_pushButton_2_1_clicked()
{
    b_2_2();
}


void MainWindow::on_pushButton_3_1_clicked()
{
    b_3_2();
}


void MainWindow::on_pushButton_1_3_clicked()
{
    b_1_3();
}

