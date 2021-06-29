#ifndef MAIN_H
#define MAIN_H

#include "mainwindow.h"
#include <QApplication>
#include<opencv2/opencv.hpp>
#include<vector>
#include "ui_mainwindow.h"
using namespace cv;
Mat Matrix(Mat temp);
QImage tr2Qimg(const cv::Mat &inMat);
void all();//Ui::MainWindow *ui
int main(int argc, char *argv[]);
#endif // MAIN_H
