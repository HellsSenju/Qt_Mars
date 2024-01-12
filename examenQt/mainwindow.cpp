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


    // Билет 2 Задание 3 - заполнение листа и листВиджет
    for(int i = 0; i < 10; i++){
       list_2_3_1 << QRandomGenerator::global()->bounded(0, 10);
       list_2_3_2 << QRandomGenerator::global()->bounded(0, 10);
    }

    it = list_2_3_1.begin();
    while(it != list_2_3_1.end()){
        ui->listWidget_2_first->addItem(QString::number(*it) + " ");
        it ++;
    }

    it = list_2_3_2.begin();
    while(it != list_2_3_2.end()){
        ui->listWidget_2_second->addItem(QString::number(*it) + " ");
        it ++;
    }
    //

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
    if(!file_A.exists() && !file_B.exists() && !file_C.exists())
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
// найти суммы элементов всех четных или нечетных столбцов
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


// Билет 2 Задание 3
// заполнить два списка случайными элементами. добавление элемента в первый или второй или в оба(чекбокс)
void MainWindow::b_2_3()
{
    QString input = ui->lineEdit_2_input->text();

    if(ui->checkBox_2_to_both->isChecked()){
        list_2_3_1.append(input.toInt());
        list_2_3_2.append(input.toInt());

        ui->listWidget_2_first->clear();
        ui->listWidget_2_second->clear();

        QList<int>::iterator it = list_2_3_1.begin();
        while(it != list_2_3_1.end()){
            ui->listWidget_2_first->addItem(QString::number(*it) + " ");
            it ++;
        }

        it = list_2_3_2.begin();
        while(it != list_2_3_2.end()){
            ui->listWidget_2_second->addItem(QString::number(*it) + " ");
            it ++;
        }

        return;
    }

    if(ui->checkBox_2_to_first->isChecked()){
        list_2_3_1.append(input.toInt());
        ui->listWidget_2_first->clear();

        QList<int>::iterator it = list_2_3_1.begin();
        while(it != list_2_3_1.end()){
            ui->listWidget_2_first->addItem(QString::number(*it) + " ");
            it ++;
        }
    }

    if(ui->checkBox_2_to_second->isChecked()){
        list_2_3_2.append(input.toInt());
        ui->listWidget_2_second->clear();

        QList<int>::iterator it = list_2_3_2.begin();
        while(it != list_2_3_2.end()){
            ui->listWidget_2_second->addItem(QString::number(*it) + " ");
            it ++;
        }
    }
}


// Билет 2 Задание 4
// найти наибольший элемент, наименьшеий на четной позиции, наибольший модуль на нечетной позиции, разность первого и последнее
void MainWindow::b_2_4()
{
    QFile file_f("file_f.txt");

    if(!file_f.exists())
    {
        qDebug() << "Файл не существует";
        return;
    }

    if(file_f.open(QIODevice::ReadOnly))
    {
        int max = -10000;
        int minChet = 100000;
        int maxNechet = -100000;
        int first = 0;
        int last = 0;
        int sub;

        int i = 1;
        while (!file_f.atEnd()) {
            int num = file_f.readLine().toInt();

            if(i == 1)
                first = num;

            if(file_f.atEnd())
                last = num;

            if(num > max)
                max = num;

            if((i % 2 == 0) && num < minChet)
                minChet = num;

            if((i % 2 != 0) && abs(num) > maxNechet)
                maxNechet = num;

            i++;
        }

        sub = first - last;
        qDebug() << "max = " << max;
        qDebug() << "minChet = " << minChet;
        qDebug() << "maxNechet = " << maxNechet;
        qDebug() << "Sub = " << sub;

        file_f.close();
    }
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


void MainWindow::on_pushButton_2_do_clicked()
{
    b_2_3();
}

