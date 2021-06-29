#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<opencv2/opencv.hpp>
#include<vector>
#include"main.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_button1_clicked()
{
    all();
}
