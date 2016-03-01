#include "Intelligent_Scissor.h"
#include <QApplication>
#include <QtGui>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QLabel>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Intelligent_Scissor w;
    w.show();

    //Mat image;

    return a.exec();
}
