#include "mainwindow.h"
#include <QApplication>
#include<opencv2/opencv.hpp>
#include<vector>
#include "ui_mainwindow.h"
using namespace cv;
using namespace std;
Mat Matrix(Mat temp);
QImage tr2Qimg(const cv::Mat &inMat);
vector<Point2f> cog(vector<Point2f> a);
void all(){//Ui::MainWindow *ui
    VideoCapture vc(10);

    Mat temp;

    vc>>temp;//输入视频信号
//    if(temp==NULL) {
//        cout<<"no camera!";
//        return void(0);
//    }
    //imshow("temp1",temp);
    //waitKey(200);
    Mat matrix=Matrix(temp);//得到变换矩阵
    cout<<"24"<<endl;
    namedWindow("play",WINDOW_NORMAL);
cout<<"26"<<endl;
    while(1){//播放
        Mat frame;
        vc>>frame;
        cout<<"play now"<<endl;
        warpPerspective(frame,frame,matrix,temp.size(),INTER_LINEAR, BORDER_CONSTANT);
        //ui->label1->setPixmap(QPixmap::fromImage(tr2Qimg(frame)));
        imshow("play",frame);
        waitKey(30);  
    }
}
//void cvt(Mat& image){
//    IplImage* hsv=CreateImage(GetSize(image),8,3);

//    cvtColor(image,hsv,COLOR_BGR2HSV);
//    int width=hsv->width;
//    int height=hsv->height;
//    for(int i=0;i<height;i++)
//        for(int j=0;j<width;j++){
//            Scalar s_hsv=Get2D(hsv,j,i);
//            Scalar s;
//            if (!(((s_hsv.val[0]>0)&&(s_hsv.val[0]<8)) || (s_hsv.val[0]>120)&&(s_hsv.val[0]<180)))
//			{
//				s.val[0] =0;
//				s.val[1]=0;
//				s.val[2]=0;
//				Set2D(hsv, i ,j, s);
//			}
//    img = CreateMat( hsv->height, hsv->width, CV_8UC3 );
//	Convert(hsv, img);
//	// cvNamedWindow("filter");
//	// cvShowImage("filter", hsv);
//	// waitKey(0);
//	// cvReleaseImage(&hsv);


//        }
//}
void HC(Mat& gray,vector<Vec3f>& Circles){
    int threshold=27;
    HoughCircles(gray,Circles,HOUGH_GRADIENT,1,20,100,threshold,0,40);
    while(Circles.size()!=4){
        if(Circles.size()<4) threshold--;
        else threshold++;
        HoughCircles(gray,Circles,HOUGH_GRADIENT,1,20,100,threshold,0,30);
    }
}
Mat Matrix(Mat temp){
//    cvt(temp);//
    int cols=temp.size().width;
    int rows=temp.size().height;
    Mat gray;vector<Point2f> srcPoint,dstPoint;

    cvtColor(temp,gray,COLOR_BGR2GRAY);
    GaussianBlur(gray,gray,Size(9,9),2,2);
    //imshow("gray",gray);waitKey(0);
    cout<<"col52"<<endl;
    vector<Vec3f> Circles;
    
    HC(gray,Circles);
   //HoughCircles(gray,Circles,HOUGH_GRADIENT,1,20,100,27,0,60);
    for(size_t i=0;i<Circles.size();i++){
        Point center(cvRound(Circles[i][0]),cvRound(Circles[i][1]));
        //Point center(Circles[i,0],Circles[i][1]);
        int radius =cvRound((Circles[i][2]));
        circle(temp,center,3,Scalar(0,255,0),-1,8,0);
        circle(temp,center,radius,Scalar(155,50,255),3,8,0);
        srcPoint.push_back(center);
    }
    imshow("temp",temp);waitKey(500);
    cout<<"67"<<endl;
    dstPoint.push_back(Point(0,0));dstPoint.push_back(Point(0,rows));dstPoint.push_back(Point(cols,0));dstPoint.push_back(Point(cols,rows));
    cout<<"beforeWarp"<<endl;
    Mat warpMatrix=getPerspectiveTransform(cog(srcPoint),dstPoint);
     cout<<"endWarp"<<endl;
    return warpMatrix;
}

QImage tr2Qimg(const cv::Mat &inMat){
    switch (inMat.type())
        {
            case CV_8UC4:		// 8-bit, 4 channel
            {
                QImage image(inMat.data,
                    inMat.cols, inMat.rows,
                    static_cast<int>(inMat.step),
                    QImage::Format_ARGB32);

                return image;
            }

            case CV_8UC3:		// 8-bit, 3 channel
            {
                QImage image(inMat.data,
                    inMat.cols, inMat.rows,
                    static_cast<int>(inMat.step),
                    QImage::Format_RGB888);

                return image.rgbSwapped();
            }


            case CV_8UC1:// 8-bit, 1 channel
            {
    #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
                QImage image(inMat.data,
                    inMat.cols, inMat.rows,
                    static_cast<int>(inMat.step),
                    QImage::Format_Grayscale8); //Format_Alpha8 and Format_Grayscale8 were added in Qt 5.5
    #else
                static QVector<QRgb>  sColorTable;

                // only create our color table the first time
                if (sColorTable.isEmpty())
                {
                    sColorTable.resize(256);

                    for (int i = 0; i < 256; ++i)
                    {
                        sColorTable[i] = qRgb(i, i, i);
                    }
                }

                QImage image(inMat.data,
                    inMat.cols, inMat.rows,
                    static_cast<int>(inMat.step),
                    QImage::Format_Indexed8);

                image.setColorTable(sColorTable);
    #endif
                return image;
            }

            default:
    //			qWarning() << "CVS::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
                break;
        }

        return QImage();
}

vector<Point2f> cog(vector<Point2f> a){//假设只有四个点
    int sx,sy;sx=sy=0;
    for(Point2f f:a){
        sx+=f.x;
        sy+=f.y;
    }
    sx/=4;sy/=4;
    Point2f aa[4];
     cout<<"beforecog"<<endl;
    for(Point2f f:a){
        if(f.x>sx&&f.y>sy) aa[3]=Point2f(f.x,f.y);
        if(f.x<sx&&f.y<sy) aa[0]=Point2f(f.x,f.y);
        if(f.y>sy&&f.x<sx) aa[1]=Point2f(f.x,f.y);
        if(f.x>sx&&f.y<sy) aa[2]=Point2f(f.x,f.y);

     }
     cout<<"endcog"<<endl;
     vector<Point2f> dst;
     for(int i=0;i<4;i++){
         dst.push_back(aa[i]);
         cout<<"push";
     }
     return dst;


}

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
