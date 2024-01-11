#include "b1_1.h"

b1_1::b1_1()
{
}

//  поменять местами максимальный и минимальный элемент в каждой строке
void b1_1::doTask(int rows, int columns)
{
    int arr[rows][columns];
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            arr[i][j] = QRandomGenerator::global()->bounded(-100, 100);
            qDebug().noquote() << arr[i][j];
        }
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

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            qDebug().noquote() << arr[i][j];
        }
    }
}


