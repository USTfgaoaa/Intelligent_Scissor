#ifndef FUNCTION
#define FUNCTION

#include <QMainWindow>
#include <stdio.h>
#include <math.h>
#include <QtGui>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <QLabel>

#include <QImage>
#include <QPixmap>
#include <QColor>
#include <QCursor>

#include <opencv2/imgproc/types_c.h>

// #include <Eigen/Eigen>

#include <queue>
#include <vector>
#include "Intelligent_Scissor.h"

#define INITIAL 0
#define EXPANDED 1
#define ACTIVE 2


const int inf = 1 << 30;

using namespace std;

void test();

QColor get_RGB(int, int, QImage);

bool operator<( Node, Node );

double select_max(double, double, double, double, double, double, double, double, double);

//void process(cv::Mat, cv::Mat, int, int, int, int, Node[10000][10000]);
cv::Mat pre_process(cv::Mat);
cv::Mat draw(int, int );
//cv::Mat process(cv::Mat, cv::Mat, int, int, int, int);
#endif // FUNCTION
