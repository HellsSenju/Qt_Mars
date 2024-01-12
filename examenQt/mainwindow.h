#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QList>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <cstdlib>

#include "b1_1.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void b_1_2();
    void b_1_3();
    void b_1_4();

    void b_2_2();
    void b_2_3();
    void b_2_4();

    void b_3_2();

    void b_4_2();
    void b_6_2();

private:
    QList<int> list_1_3;
    QList<int> list_2_3_1;
    QList<int> list_2_3_2;


private slots:
    void on_pushButton_2_1_clicked();

    void on_pushButton_3_1_clicked();

    void on_pushButton_1_3_clicked();

    void on_pushButton_2_do_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
