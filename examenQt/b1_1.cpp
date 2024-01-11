#include "b1_1.h"

b1_1::b1_1()
{
}

// Билет 1 Задание 2
// поменять местами максимальный и минимальный элемент в каждой строке
void b1_1::task_1_1(int rows, int columns)
{
    qDebug() << "=================================================";
    qDebug() << "Билет 1 Задание 2";

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


