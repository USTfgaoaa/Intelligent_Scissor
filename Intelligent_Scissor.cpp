#include "Intelligent_Scissor.h"
#include "ui_Intelligent_Scissor.h"
#include "function.h"
#include <QMouseEvent>

using namespace cv;
using namespace std;

QImage Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     return dest;
}

QImage Mat2QImage_mask(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     temp = src.clone();
     //cvtColor(src, temp,CV_BGR2GRAY); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Indexed8);
     dest.bits(); // enforce deep copy, see documentation
     return dest;
}

cv::Mat QImage2Mat(QImage const& src)
{
     cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result,CV_BGR2RGB);
     return result;
}


Intelligent_Scissor::Intelligent_Scissor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Intelligent_Scissor)
{
    ui->setupUi(this);

    ui->label->installEventFilter(this);
//    ui->label->setMouseTracking(true);
    ui->label->setMouseTracking(true);

    is_load = false;
    is_start = false;
    //is_finish = false;
    mouse_press = false;
    is_finished = false;
    mission_finish = false;
    is_cut = false;

    last_end_row = inf;
    last_end_col = inf;
    press_count = 0;
    zoom_coef = 1.0;

    seed_x = 60;
    seed_y = 60;

    //final_mask = cv::Mat(origin.rows, origin.cols, CV_8UC3, Scalar(0,0,0));
    image_history.clear();
    mask_history.clear();

}

Intelligent_Scissor::~Intelligent_Scissor()
{
    delete ui;
}

bool Intelligent_Scissor::eventFilter(QObject *obj, QEvent *event)
{

    if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
    //## TIPS: Now we have got the current mouse position coordinate, should add some limit to constrain it within the pic,
    //         and compensate the offset in Y axis.

    if(mouseEvent->pos().x() > col_up - 4 || mouseEvent->pos().y() > row_up -4 || mouseEvent->pos().x() < 1 || mouseEvent->pos().y() < 1)
        return false;

    if(!is_start)
        return false;

    if(is_finished)
        return false;

    //cout<<"mouse press :"<<mouse_press<<endl;
    //cout<<"mouse move ok ..."<<endl;
    end_col = mouseEvent->pos().x();
    end_row = mouseEvent->pos().y();

     if(abs(end_col - first_pt_col) < 5 && abs(end_row - first_pt_row) < 5 && press_count > 1)
        {
            //cout<<"find loop closure ..."<<endl;
            end_col = first_pt_col;
            end_row = first_pt_row;

            is_finished = true;
         }

    if(!mouse_press)
        return false;

    last_end_col = end_col;
    last_end_row = end_row;

   // cout<<"restart & start, now OK ... "<<endl;
    pair<cv::Mat, cv::Mat> result_mask = draw(end_row, end_col);
    cv::Mat result = result_mask.first;
    mask_to_find   = result_mask.second.clone();
    //cout<<"restart & start, now OK ... after drawing "<<endl;
    QPixmap new_pixmap = QPixmap::fromImage(Mat2QImage( result ));

    ui->label->setPixmap(new_pixmap);
    ui->label->show();

  }

  return false;
}


void Intelligent_Scissor::on_actionLoad_triggered()
{
    open();
   // test();
}


void Intelligent_Scissor::on_actionSave_triggered()
{
    save();
}

void Intelligent_Scissor::on_actionAbout_triggered()
{
     QMessageBox::information(this, tr("Information"),tr("Intelligent Scissor, made by GAO Fei"));
}

void Intelligent_Scissor::on_actionHelp_triggered()
{
    QMessageBox::information(this, tr("FYI ... "),tr("Intelligent Scissor, made by BBGF!"));
}

void Intelligent_Scissor::Zoom_in_out(int index)
{
    if(!is_load)
        return;

     tmp = src.clone();
     //dst = tmp.clone();
     //dst = tmp.resize((tmp.cols*index, tmp.rows*index));
     Mat dst(tmp.cols*index, tmp.rows*index, CV_8UC3, Scalar(0,0,255));

     cv::Mat new_mask, old_mask;
     old_mask = mask_history[mask_history.size()-1];

     if(index > 1)
     {
         pyrUp( tmp, dst, Size( tmp.cols*index, tmp.rows*index ) );
         pyrUp( old_mask, new_mask, Size( tmp.cols*index, tmp.rows*index ) );
     }
      else if(index < 1)
     {
         pyrDown( tmp, dst, Size( tmp.cols*index, tmp.rows*index ) );
         pyrUp( old_mask, new_mask, Size( tmp.cols*index, tmp.rows*index ) );
     }

     src = dst;
     zoom_coef = zoom_coef * index;

     //imshow( window_name, dst );

     row_up = src.rows;
     col_up = src.cols;
    //cout<<"New row and col linit: "<<row_up<<" , "<<col_up<<endl;

     image_history.clear();
     image_history.push_back(src);

     mask_history.clear();
     mask_history.push_back(new_mask);

     cost_graph = pre_process(src);

    QPixmap new_pixmap = QPixmap::fromImage(Mat2QImage( dst ));

     int w = new_pixmap.width();
     int h = new_pixmap.height();

     ui->label->setPixmap(new_pixmap);
     ui->label->resize(w,h);
     ui->label->show();

}

void Intelligent_Scissor::on_actionZoom_x1_2_triggered()
{
    if(!is_load)
        return;
   Zoom_in_out(0.5);
}

void Intelligent_Scissor::on_actionZoom_x1_triggered()
{
    if(!is_load)
        return;

    is_start = false;
    //is_finish = false;
    mouse_press = false;
    is_finished = false;
    mission_finish = false;
    press_count = 0;

    zoom_coef = 1.0;
    src = src_backup;
    //imshow( window_name, src );

    QPixmap new_pixmap = QPixmap::fromImage(Mat2QImage( src ));

    int w = new_pixmap.width();
    int h = new_pixmap.height();

    ui->label->setPixmap(new_pixmap.scaled(w,h,Qt::KeepAspectRatio));
    ui->label->show();
    Zoom_in_out(1);
}

void Intelligent_Scissor::on_actionZoom_x2_triggered()
{
    if(!is_load)
        return;
   Zoom_in_out(2);
}

void Intelligent_Scissor::save()
{
    if(!is_cut )
        return;

    QString path = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
    if(path.length() == 0)
    {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
            //path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
    }
    else
    {
            //QImage image_cut = Mat2QImage(output_backup);
            //image_cut.save(path);
        std::string utf8_path = path.toUtf8().constData();
         imwrite( utf8_path, output_backup );
    }
}

void Intelligent_Scissor::open()
{
        QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
        if(path.length() == 0)
        {
                QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
                //path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
        }
        else{
            cv::Mat image;

            std::string utf8_text = path.toUtf8().constData();
            image = imread(utf8_text,1);

            src = image;
            src_backup = image;

            //window_name = "Display window";
            //namedWindow( window_name, WINDOW_AUTOSIZE );// Create a window for display.
            //imshow( "Display window", image );

            QImage  image_in(path);
            image_hold = image_in;

            //QPixmap pixmap(path);
            QPixmap pixmap = QPixmap::fromImage(image_in);
            int w = pixmap.width();
            int h = pixmap.height();

            ui->label->setPixmap(pixmap);//,Qt::KeepAspectRatio));
            ui->label->resize(w,h);
            //ui->label->setPixmap(pixmap);
            ui->label->show();
            is_load = true;

            row_up = image.rows;
            col_up = image.cols;
            //cout<<"row and col limit: "<<row_up<<" , "<<col_up<<endl;

            image_history.push_back((image));
            cost_graph = pre_process(image);

            cv::Mat M(image.rows, image.cols, CV_8UC3, Scalar(0,0,0));   //result.clone();
            mask_to_find = M.clone();
            mask_history.push_back(mask_to_find);

        }
}

void Intelligent_Scissor::mousePressEvent(QMouseEvent *e)
{

   //ui->label_2->setText(tr("(%1,%2)").arg(e->x()).arg(e->y()));
   mouse_x = e->x();
   mouse_y = e->y();

   if(mission_finish)
    {
       seed_x = mouse_x;
       seed_y = mouse_y;
       press_count++;
       return;
   }

   if(!is_start)
       return;

   //cout<<"start and mouse press ..."<<endl;

   //printf("Now mouse position is: %d, %d\n",mouse_x, mouse_y);

   mouse_press = true;

   if(mouse_x > col_up || mouse_y - 25 > row_up)
       return;

       start_col = mouse_x;
       start_row = mouse_y - 25;

   press_count++;

   start_col_his.push_back(start_col);
   start_row_his.push_back(start_row);

   //cout<<"press count: "<<press_count<<endl;
   //cout<<"press_count:  "<<press_count<<endl;
   //cout<<"size of start_row history:  "<<start_col_his.size()<<endl;
   if(press_count == 1)
   {
       first_pt_col = start_col;
       first_pt_row = start_row;
       //cout<<"first start point ... "<<first_pt_row<<" , "<<first_pt_col<<endl;
   }
   else
   {
        image_history.push_back((result));
        mask_history.push_back(mask_to_find);
   }

    //cout<<"start point ... "<<start_row<<" , "<<start_col<<endl;
   if(abs(start_col - first_pt_col) < 8 && abs(start_row - first_pt_row) < 8 && press_count > 1)
        if(is_finished)
            is_start = false;

   process(start_row, start_col);
}

void Intelligent_Scissor::process(int start_row, int start_col)
{
    int x, y;

    priority_queue<Node> pq;

    cv::Mat image = src;//src_backup;//

    //Now do Initialize work :

    node_array.clear();

    for(int i = 0; i < image.rows ; ++i )
    {
        vector<Node> row_node;
        for(int j = 0; j < image.cols ; ++j)
        {
            Node node;
            node.row = i;
            node.col = j;
            node.state = INITIAL;
            node.totalCost = inf;
            node.prevNode = NULL;

            if(i ==0 || j==0 || i ==image.rows-1 || j ==image.cols-1)
            {

                node.linkCost[0] = inf;
                node.linkCost[1] = inf;
                node.linkCost[2] = inf;
                node.linkCost[3] = inf;
                node.linkCost[4] = inf;
                node.linkCost[5] = inf;
                node.linkCost[6] = inf;
                node.linkCost[7] = inf;
            }
            else
            {
            x = 3*i;
            y = 3*j;

            node.linkCost[0] = cost_graph.at<cv::Vec3b>(x, y+1)[0];
            node.linkCost[1] = cost_graph.at<cv::Vec3b>(x-1, y+1)[0];
            node.linkCost[2] = cost_graph.at<cv::Vec3b>(x-1, y)[0];
            node.linkCost[3] = cost_graph.at<cv::Vec3b>(x-1, y-1)[0];
            node.linkCost[4] = cost_graph.at<cv::Vec3b>(x, y-1)[0];
            node.linkCost[5] = cost_graph.at<cv::Vec3b>(x+1, y-1)[0];
            node.linkCost[6] = cost_graph.at<cv::Vec3b>(x+1, y)[0];
            node.linkCost[7] = cost_graph.at<cv::Vec3b>(x+1, y+1)[0];

            if(i == start_row && j == start_col)
            {
                node.totalCost = 0.0;
                node.state = ACTIVE;
                pq.push(node);
            }
            }

             row_node.push_back(node);
         }
        node_array.push_back(row_node);
    }

//    /cout<<"prepare path finding ... "<<endl;
    while (!pq.empty())
    {
        Node q = pq.top();
        pq.pop();
        int q_row = q.row, q_col = q.col;

        q.state = EXPANDED;
        assert(node_array[q_row][q_col].state=2);
        node_array[q_row][q_col].state = EXPANDED;
        assert(node_array[q_row][q_col].state=1);
        for(int i = -1; i < 2; i++)
        {
            for(int j = -1; j< 2; j++)
            {
                int link_idx;
                if(i == 0 && j ==1)
                    link_idx = 0;
                else if(i == -1 && j ==1)
                     link_idx = 1;
                else if(i == -1 && j ==0)
                     link_idx = 2;
                else if(i == -1 && j ==-1)
                     link_idx = 3;
                else if(i == 0 && j ==-1)
                     link_idx = 4;
                else if(i == 1 && j ==-1)
                     link_idx = 5;
                else if(i == 1 && j ==0)
                     link_idx = 6;
                else if(i == 1 && j ==1)
                     link_idx = 7;
                else if(i == 0 && j==0)
                    continue;

                if( q_row + i == 0 ||q_row + i== image.rows-1 || q_col + j == 0 || q_col + j == image.cols -1)
                     continue;
                Node r = node_array[q_row + i][ q_col + j];
                if(r.state != EXPANDED)
                {
                    assert(r.state != 1);
                    if(r.state == INITIAL)
                    {
                        r.prevNode = &node_array[q_row][q_col];
                        r.totalCost = q.totalCost + q.linkCost[link_idx];
                        r.state = ACTIVE;
                        if( !(r.row == 0 ||r.row == image.rows -1  || r.col == 0 || r.col == image.cols -1))
                        pq.push(r);

                        node_array[q_row + i][ q_col + j].prevNode = &node_array[q_row][ q_col];
                        node_array[q_row + i][ q_col + j].totalCost = q.totalCost + q.linkCost[link_idx];
                        node_array[q_row + i][ q_col + j].state = ACTIVE;
                    }
                    else if(r.state == ACTIVE)
                    {
                        if(q.totalCost + q.linkCost[link_idx] < r.totalCost )
                        {
                            r.prevNode = &node_array[q_row][ q_col];
                            r.totalCost = q.totalCost + q.linkCost[link_idx];

                            node_array[q_row + i][ q_col + j].prevNode = &node_array[q_row][ q_col];
                            node_array[q_row + i][ q_col + j].totalCost = q.totalCost + q.linkCost[link_idx];
                        }
                    }
                }
            }
        }
    }
     //is_finished = true;
     //cout<<"path finding finished ... "<<endl;
}

pair<cv::Mat, cv::Mat> Intelligent_Scissor::draw(int end_row, int end_col)
{
    //cout<<"draw function ,.."<<endl;
    result = image_history[image_history.size() -1].clone();//src_backup.clone(); // FIX ME: Store all image in image_history vector ...
    //cv::Mat M(result.rows, result.cols, CV_8UC3, Scalar(0,0,0));   //result.clone();

    mask_to_find = mask_history[mask_history.size() -1].clone();

    Node end_node = node_array[end_row][end_col];

    //cout<<"end row and col: "<<end_row<<" , "<<end_col<<endl;
    int link_node_row, link_node_col;
    link_node_row = end_row;
    link_node_col = end_col;

    while(!(link_node_row == start_row && link_node_col == start_col ))
        {
            for(int i = -1; i < 2; i++)
                for(int j = -1; j < 2; j++)
                    result.at<cv::Vec3b>(link_node_row + i, link_node_col + j) = cv::Vec3b(0,0,255);

            mask_to_find.at<cv::Vec3b>(link_node_row, link_node_col) = cv::Vec3b(255,255,255);
            end_node = *(end_node.prevNode);
            link_node_row = end_node.row;
            link_node_col = end_node.col;

    }
    cv::Point seed(start_col, start_row);
    cv::circle(result, seed, 3, cv::Scalar(128,128,128), 2);


    return make_pair(result, mask_to_find);
}

void Intelligent_Scissor::on_actionCost_Graph_triggered()
{
    std::string wn2 = "Cost Graph";
    namedWindow( wn2, WINDOW_AUTOSIZE );// Create a window for display.
    imshow("Cost Graph", cost_graph);

}
/*
void Intelligent_Scissor::on_pushButton_clicked()
{
    //priority_queue<int> qi;

        int a[5] = {3,5,9,6,2};

        priority_queue<int> qi;
        for(int i = 0; i < 5; i++)
            qi.push(a[i]);

        for(int i = 0; i < 5; i++)
        {
            cout<<qi.top()<<" "<<endl;
            qi.pop();
        }
}*/

void Intelligent_Scissor::on_Finish_clicked()
{
    if(is_finished)
    {
   // cout<<"finish ok .."<<endl;
    is_start = false;
    mission_finish = true;
    }
}

void Intelligent_Scissor::on_Start_clicked()
{
        is_start = true;
        mission_finish = false;
}

void Intelligent_Scissor::on_Undo_clicked()
{
    undo();
}

void Intelligent_Scissor::undo()
{
    if(start_row_his.size() - 1 < 1 || press_count < 1)
        return;

    if(image_history.empty())
        return;
    if(mission_finish)
        return;
    press_count --;
    image_history.pop_back();
    mask_history.pop_back();
    if(press_count > 0)
    {

        start_row = start_row_his[start_row_his.size() - 2 ];
        start_col = start_col_his[start_col_his.size() - 2];
        start_row_his.pop_back();
        start_col_his.pop_back();
        process(start_row, start_col);
    }
}
void Intelligent_Scissor::on_Restart_clicked()
{
    if(!is_load)
        return;
    //is_load = false;
    is_start = false;
    //is_finish = false;
    mouse_press = false;
    is_finished = false;
    press_count = 0;
    mission_finish = false;
    is_cut = false;

    start_row = inf;
    start_col = inf;
    press_count = 0;
    zoom_coef = 1.0;

    seed_x = 60;
    seed_y = 60;

    cv::Mat restart = image_history[0];
    cv::Mat restart_mask = mask_history[0];
    QPixmap new_pixmap = QPixmap::fromImage(Mat2QImage(restart));

    image_history.clear();
    mask_history.clear();
    image_history.push_back((restart));
    mask_history.push_back((restart_mask));

    ui->label->setPixmap(new_pixmap);
    ui->label->show();

    return;
}

void Intelligent_Scissor::on_actionExit_2_triggered()
{
        this->close();
}


void Intelligent_Scissor::on_Mask_clicked()
{
    if(!mission_finish)
        return;
    if(mask_history.empty())
        return;

    cv::Point seed(seed_x,seed_y);

    cv::Mat mask;

    cv::cvtColor(mask_to_find,mask,CV_BGR2GRAY);
    cv::copyMakeBorder(mask, mask, 1, 1, 1, 1, cv::BORDER_REPLICATE);

    uchar fillValue = 255;
    cv::floodFill(mask_to_find, mask, seed, cv::Scalar(255) ,0, cv::Scalar(), cv::Scalar(), 4 | cv::FLOODFILL_MASK_ONLY | (fillValue << 8));
    final_mask = mask.clone();
    //cv::imshow("Final Mask", final_mask);
}

/*
 * void Intelligent_Scissor::on_Debug_clicked()
{
    int i = 162;// col_y = 200;
    for(int j = 200; j< 209; j++)
    {
        int x= 3*i;
        int y= 3*j;
        printf("%d\n",cost_graph.at<cv::Vec3b>(x, y+1)[0]);
        printf("%d\n",cost_graph.at<cv::Vec3b>(x-1, y+1)[0]);
        printf("%d\n",cost_graph.at<cv::Vec3b>(x-1, y)[0]);
        printf("%d\n",cost_graph.at<cv::Vec3b>(x-1, y-1)[0]);
        printf("%d\n",cost_graph.at<cv::Vec3b>(x, y-1)[0]);
        printf("%d\n",cost_graph.at<cv::Vec3b>(x+1, y-1)[0]);
        printf("%d\n",cost_graph.at<cv::Vec3b>(x+1, y)[0]);
        printf("%d\n",cost_graph.at<cv::Vec3b>(x+1, y+1)[0]);
        cout<<"###################################"<<endl;
    }
        return;
}*/

void Intelligent_Scissor::on_Cut_clicked()
{
    if(!mission_finish)
        return;
    if(mask_history.empty())
        return;
    if(!is_finished)
        return;

    //cout<<"Now Ok here ..."<<endl;
    cv::Mat origin = src.clone();
    cv::Mat mask  = mask_history[mask_history.size() -1];


    //IplImage* output = cvCreateImage( cvSize(origin.rows,origin.cols), IPL_DEPTH_8U,3 );
    cv::Mat output = cv::Mat(origin.rows, origin.cols, CV_8UC3, Scalar(0,0,0));
    //origin.copyTo(mask,output);

    //imshow("origin pic", origin);

    for(int i = 0; i < final_mask.rows; i++ )
        for(int j =0; j< final_mask.cols; j++)
            //cout<<mask.at<cv::Vec3b>(i,j)<<endl;
            if(final_mask.at<uchar>(i,j) == 255)
                output.at<cv::Vec3b>(i,j) = origin.at<cv::Vec3b>(i,j);

    //imshow("cut pic", output);

    output_backup = output;
    QPixmap new_pixmap = QPixmap::fromImage(Mat2QImage( output));
    int w = new_pixmap.width();
    int h = new_pixmap.height();

    ui->label->setPixmap(new_pixmap);
    ui->label->resize(w,h);
    ui->label->show();

    is_cut = true;
}


void Intelligent_Scissor::on_SaveMask_clicked()
{
    if(!mission_finish)
        return;
    //if(is_start)
    //    return;

    QString path = QFileDialog::getSaveFileName(this, tr("Save Mask Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
    if(path.length() == 0)
    {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any path."));
            //path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
    }
    else
    {
    std::string utf8_path = path.toUtf8().constData();
     imwrite( utf8_path, final_mask );
    }
}

void Intelligent_Scissor::on_pushButton_2_clicked()
{
    //if()
    //    return;

    if(image_history.empty())
        return;
    QString path = QFileDialog::getSaveFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
    if(path.length() == 0)
    {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any path."));
            //path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png *.bmp)"));
    }
    else
    {
    std::string utf8_path = path.toUtf8().constData();
     imwrite( utf8_path, result );
    }
}
