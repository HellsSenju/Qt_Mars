#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    b1_1 b1;
    b1.doTask(10, 10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

