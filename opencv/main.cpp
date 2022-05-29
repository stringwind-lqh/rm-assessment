#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main()
{
    Mat src = imread("../assets/src.jpg");
    //图像灰度化，阈值处理
    Mat grey, dst0, dst1;
    cvtColor(src, grey, COLOR_BGR2GRAY);
    threshold(grey, dst0, 190, 255,THRESH_BINARY);

    //形态学滤波
    Mat k = getStructuringElement(MORPH_RECT, Size(2, 2));
    Mat big_element = getStructuringElement(MORPH_RECT, Size(8,8));
    morphologyEx(dst0, dst1, MORPH_OPEN, k, Point(-1,-1), 5);
    morphologyEx(dst1, dst0, MORPH_CLOSE, big_element, Point(-1,-1), 3);

    //图像轮廓提取
    vector<vector<Point>> contours;
    vector<Vec4i> hierachy;
    findContours(dst0, contours, hierachy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


    //拟合多边形
    vector<vector<Point>> approx(contours.size());
    for(int i=0;i<contours.size();i++)
    {
        approxPolyDP(contours[i], approx[i], contours.size(),true );
    }


    
    vector<Point> cache;
    cache = approx[8];
    int constant = 50;
    Mat final= Mat::zeros(constant*17, constant*9, CV_8UC3);
    // 取原图四个顶点
    vector<Point2f> before(4);
    before[0] = cache[0];
    before[1] = cache[3];
    before[2] = cache[1];
    before[3] = cache[2];
    // 取原图四个顶点在目标图像上的坐标
    vector<Point2f> after(4);
    after[0] = Point(0,0);
    after[1] = Point(452, 0);
    after[2] = Point(0, 850);
    after[3] = Point(452, 850);

    //透视变换
    Mat TransImage;
    TransImage = getPerspectiveTransform(before, after);
    warpPerspective(src, final, TransImage, final.size());

    namedWindow("结果图", WINDOW_NORMAL);
    imshow("结果图", final);
    waitKey(0);
    return 0;
}
