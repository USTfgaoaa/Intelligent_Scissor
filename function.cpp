#include "function.h"

void test()
{

}

bool operator<( Node a, Node b ){
    return a.totalCost> b.totalCost;
}

double select_max(double max_old, double a0, double a1, double a2, double a3, double a4, double a5, double a6, double a7)
{
    double max_value = max_old;

    if(a0 > max_old)
        max_value = a0;
    if(a1 > max_old)
        max_value = a1;
    if(a2 > max_old)
        max_value = a2;
    if(a3 > max_old)
        max_value = a3;
    if(a4 > max_old)
        max_value = a4;
    if(a5 > max_old)
        max_value = a5;
    if(a6 > max_old)
        max_value = a6;
    if(a7 > max_old)
        max_value = a7;

    return max_value;
}


QColor get_RGB(int x, int y, QImage image_input)
{

    QRgb tempColorRgb = image_input.pixel(QPoint(x,y));
    QColor tempColor(tempColorRgb);
    return tempColor;

}

cv::Mat pre_process(cv::Mat cv_image)
{

    int row = cv_image.rows; // row is height
    int col = cv_image.cols; // col is width

    //result.resize( 3*w, 3*h );

    cv::Mat cost_graph(3*row, 3*col, CV_8UC3, cv::Scalar(0,0,0) );

    double dr0, dr1, dr2, dr3, dr4, dr5, dr6, dr7;
    double dg0, dg1, dg2, dg3, dg4, dg5, dg6, dg7;
    double db0, db1, db2, db3, db4, db5, db6, db7;
    double d0, d1, d2, d3, d4, d5, d6, d7;
    double cos0, cos1, cos2, cos3, cos4, cos5, cos6, cos7;
    double d_max = 0;
    int x, y;

    cv::Vec3b pix_rgb0, pix_rgb1, pix_rgb2, pix_rgb3, pix_rgb4, pix_rgb5, pix_rgb6, pix_rgb7;

    for(int i = 1; i < row-1; ++ i)
    {
        for(int j = 1; j < col-1; ++j)
        {
            pix_rgb0 = cv_image.at<cv::Vec3b>(i,j+1);
            pix_rgb1 = cv_image.at<cv::Vec3b>(i-1,j+1);
            pix_rgb2 = cv_image.at<cv::Vec3b>(i-1 ,j);
            pix_rgb3 = cv_image.at<cv::Vec3b>(i-1,j-1);
            pix_rgb4 = cv_image.at<cv::Vec3b>(i,j-1  );
            pix_rgb5 = cv_image.at<cv::Vec3b>(i+1,j-1);
            pix_rgb6 = cv_image.at<cv::Vec3b>(i+1  ,j);
            pix_rgb7 = cv_image.at<cv::Vec3b>(i+1,j+1);

                dr0 = abs( pix_rgb2[0]  + pix_rgb1[0] - pix_rgb6[0] - pix_rgb7[0] )/4;
                dg0 = abs( pix_rgb2[1]  + pix_rgb1[1] - pix_rgb6[1] - pix_rgb7[1] )/4;
                db0 = abs( pix_rgb2[2]  + pix_rgb1[2] - pix_rgb6[2] - pix_rgb7[2] )/4;
                d0  =  sqrt( (dr0*dr0 + dg0*dg0 + db0*db0) /3 );

                dr2 = abs( pix_rgb3[0]  + pix_rgb4[0] - pix_rgb1[0] - pix_rgb0[0]   )/4;
                dg2 = abs( pix_rgb3[1]  + pix_rgb4[1] - pix_rgb1[1] - pix_rgb0[1] )/4;
                db2 = abs( pix_rgb3[2]  + pix_rgb4[2] - pix_rgb1[2] - pix_rgb0[2]  )/4;
                d2  =  sqrt( (dr2*dr2 + dg2*dg2 + db2*db2) /3 );

                dr4 = abs( pix_rgb2[0]  + pix_rgb3[0] - pix_rgb5[0] - pix_rgb6[0]   )/4;
                dg4 = abs( pix_rgb2[1]  + pix_rgb3[1] - pix_rgb5[1] - pix_rgb6[1] )/4;
                db4 = abs( pix_rgb2[2]  + pix_rgb3[2] - pix_rgb5[2] - pix_rgb6[2]  )/4;
                d4  =  sqrt( (dr4*dr4 + dg4*dg4 + db4*db4) /3 );

                dr6 = abs( pix_rgb4[0]  + pix_rgb5[0] - pix_rgb0[0] - pix_rgb7[0]   )/4;
                dg6 = abs( pix_rgb4[1]  + pix_rgb5[1] - pix_rgb0[1] - pix_rgb7[1] )/4;
                db6 = abs( pix_rgb4[2]  + pix_rgb5[2] - pix_rgb0[2] - pix_rgb7[2]  )/4;
                d6  =  sqrt( (dr6*dr6 + dg6*dg6 + db6*db6) /3 );

        //################################### D(RGB) of 45 degree direction ###############################################
                dr1 = abs( pix_rgb2[0] - pix_rgb0[0]  )/sqrt(2);
                dg1 = abs( pix_rgb2[1] - pix_rgb0[1])/sqrt(2);
                db1 = abs( pix_rgb2[2] - pix_rgb0[2] )/sqrt(2);
                d1  =  sqrt( (dr1*dr1 + dg1*dg1 + db1*db1) /3 );

                dr3 = abs( pix_rgb2[0] - pix_rgb4[0]  )/sqrt(2);
                dg3 = abs( pix_rgb2[1] - pix_rgb4[1])/sqrt(2);
                db3 = abs( pix_rgb2[2] - pix_rgb4[2] )/sqrt(2);
                d3  =  sqrt( (dr3*dr3 + dg3*dg3 + db3*db3) /3 );

                dr5 = abs( pix_rgb4[0] - pix_rgb6[0]  )/sqrt(2);
                dg5 = abs( pix_rgb4[1] - pix_rgb6[1])/sqrt(2);
                db5 = abs( pix_rgb4[2] - pix_rgb6[2] )/sqrt(2);
                d5  =  sqrt( (dr5*dr5 + dg5*dg5 + db5*db5) /3 );

                dr7 = abs( pix_rgb6[0] - pix_rgb0[0]  )/sqrt(2);
                dg7 = abs( pix_rgb6[1] - pix_rgb0[1])/sqrt(2);
                db7 = abs( pix_rgb6[2] - pix_rgb0[2] )/sqrt(2);
                d7  =  sqrt( (dr7*dr7 + dg7*dg7 + db7*db7) /3 );
                //printf("RGB components of the pixel selected: %d %d %d\n", pixel_rgb_color[0], pixel_rgb_color[1], pixel_rgb_color[2]);
            //result
                d_max = select_max(d_max, d0, d1, d2, d3, d4, d5, d6, d7);

                x = 3*i;
                y = 3*j;
                cost_graph.at<cv::Vec3b>(x, y) = cv_image.at<cv::Vec3b>(i,j);

                cost_graph.at<cv::Vec3b>(x, y+1)   = cv::Vec3b(d0, d0, d0);
                cost_graph.at<cv::Vec3b>(x-1, y+1) = cv::Vec3b(d1, d1, d1);
                cost_graph.at<cv::Vec3b>(x-1, y)   = cv::Vec3b(d2, d2, d2);
                cost_graph.at<cv::Vec3b>(x-1, y-1) = cv::Vec3b(d3, d3, d3);
                cost_graph.at<cv::Vec3b>(x, y-1)   = cv::Vec3b(d4, d4, d4);
                cost_graph.at<cv::Vec3b>(x+1, y-1) = cv::Vec3b(d5, d5, d5);
                cost_graph.at<cv::Vec3b>(x+1, y)   = cv::Vec3b(d6, d6, d6);
                cost_graph.at<cv::Vec3b>(x+1, y+1) = cv::Vec3b(d7, d7, d7);

        }
    }

        //cout<<d_max<<endl;
    double tmp_d0, tmp_d1, tmp_d2, tmp_d3, tmp_d4, tmp_d5, tmp_d6, tmp_d7;

    for(int i = 1; i < row-1; ++ i)
    {
        for(int j = 1; j < col-1; ++j)
        {
            x = 3*i;
            y = 3*j;
            //cost_graph.at<cv::Vec3b>(x, y) = cv_image.at<cv::Vec3b>(i,j);


            tmp_d0 = cost_graph.at<cv::Vec3b>(x, y+1)[0];
            tmp_d1 = cost_graph.at<cv::Vec3b>(x-1, y+1)[0];
            tmp_d2 = cost_graph.at<cv::Vec3b>(x-1,   y)[0];
            tmp_d3 = cost_graph.at<cv::Vec3b>(x-1, y-1)[0];
            tmp_d4 = cost_graph.at<cv::Vec3b>(x, y-1)[0];
            tmp_d5 = cost_graph.at<cv::Vec3b>(x+1, y-1)[0];
            tmp_d6 = cost_graph.at<cv::Vec3b>(x+1, y)[0];
            tmp_d7 = cost_graph.at<cv::Vec3b>(x+1, y+1)[0];

            cos0 =  d_max - tmp_d0;
            cos1 = (d_max - tmp_d1)*sqrt(2);
            cos2 =  d_max - tmp_d2;
            cos3 = (d_max - tmp_d3)*sqrt(2);
            cos4 =  d_max - tmp_d4;
            cos5 = (d_max - tmp_d5)*sqrt(2);
            cos6 =  d_max - tmp_d6;
            cos7 = (d_max - tmp_d7)*sqrt(2);

            cost_graph.at<cv::Vec3b>(x, y+1)[0] = cos0;
            cost_graph.at<cv::Vec3b>(x, y+1)[1] = cos0;
            cost_graph.at<cv::Vec3b>(x, y+1)[2] = cos0;

            cost_graph.at<cv::Vec3b>(x-1, y+1)[0] = cos1;
            cost_graph.at<cv::Vec3b>(x-1, y+1)[1] = cos1;
            cost_graph.at<cv::Vec3b>(x-1, y+1)[2] = cos1;

            cost_graph.at<cv::Vec3b>(x-1, y)[0] = cos2;
            cost_graph.at<cv::Vec3b>(x-1, y)[1] = cos2;
            cost_graph.at<cv::Vec3b>(x-1, y)[2] = cos2;

            cost_graph.at<cv::Vec3b>(x-1, y-1)[0] = cos3;
            cost_graph.at<cv::Vec3b>(x-1, y-1)[1] = cos3;
            cost_graph.at<cv::Vec3b>(x-1, y-1)[2] = cos3;

            cost_graph.at<cv::Vec3b>(x, y-1)[0] = cos4;
            cost_graph.at<cv::Vec3b>(x, y-1)[1] = cos4;
            cost_graph.at<cv::Vec3b>(x, y-1)[2] = cos4;

            cost_graph.at<cv::Vec3b>(x+1, y-1)[0] = cos5;
            cost_graph.at<cv::Vec3b>(x+1, y-1)[1] = cos5;
            cost_graph.at<cv::Vec3b>(x+1, y-1)[2] = cos5;

            cost_graph.at<cv::Vec3b>(x+1, y)[0] = cos6;
            cost_graph.at<cv::Vec3b>(x+1, y)[1] = cos6;
            cost_graph.at<cv::Vec3b>(x+1, y)[2] = cos6;

            cost_graph.at<cv::Vec3b>(x+1, y+1)[0] = cos7;
            cost_graph.at<cv::Vec3b>(x+1, y+1)[1] = cos7;
            cost_graph.at<cv::Vec3b>(x+1, y+1)[2] = cos7;

        }
    }

    return cost_graph;

}

