#ifndef INTELLIGENT_SCISSOR_H
#define INTELLIGENT_SCISSOR_H

#include <QMainWindow>
#include <stdio.h>
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
#include <opencv2/opencv.hpp>

#include <opencv/cv.h>

#include <QImage>
#include <QPixmap>
#include <QColor>
#include <QImageWriter>
#include <QCursor>

#include <opencv2/imgproc/types_c.h>

//#include <Eigen/Eigen>

#include <queue>

using namespace std;

namespace Ui {
class Intelligent_Scissor;

}

struct Node{
    double linkCost[8];
    int state;
    double totalCost;
    Node *prevNode;
    int col, row;
};


class Intelligent_Scissor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Intelligent_Scissor(QWidget *parent = 0);
    ~Intelligent_Scissor();

private slots:
    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_actionAbout_triggered();

    void on_actionHelp_triggered();

    void on_actionZoom_x1_triggered();

    void on_actionZoom_x2_triggered();

    void on_actionZoom_x1_2_triggered();

    void Zoom_in_out(int index);

    void on_actionCost_Graph_triggered();

    void on_pushButton_clicked();

    void on_Finish_clicked();

    void on_Start_clicked();

    void on_Undo_clicked();

    void on_Restart_clicked();

    void on_actionExit_2_triggered();

    void on_Mask_clicked();

    void on_Debug_clicked();

    void on_Cut_clicked();

private:
    Ui::Intelligent_Scissor *ui;

    cv::Mat src, dst, tmp, src_backup;
    cv::Mat cost_graph;
    cv::Mat result;
    cv::Mat mask_to_find;
    cv::Mat final_mask, output_backup;

    vector<cv::Mat> image_history;
    vector<cv::Mat> mask_history;

    QImage image_hold;
    //Eigen::MatrixXd img_process;
    int mouse_x, mouse_y;

    bool is_load;
    bool is_start;
    bool is_finish;
    bool mouse_press;
    bool is_finished;
    bool mission_finish;

    int press_count;
    int row_up;
    int col_up;
    int seed_x, seed_y;

    double zoom_coef;

    void open();
    void save();
    bool eventFilter(QObject *obj, QEvent *event);

    std::string window_name;


    //Node node_array[200][200];

    vector< vector<Node> > node_array;
    int start_row, start_col, end_row, end_col;
    int last_end_row, last_end_col;

    vector<int> start_col_his, start_row_his;

    void process(int, int);

    int first_pt_row, first_pt_col;

pair<cv::Mat, cv::Mat> draw(int, int);
protected:
    void mousePressEvent(QMouseEvent *);

};

#endif // INTELLIGENT_SCISSOR_H

