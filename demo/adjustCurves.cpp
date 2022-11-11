#include <cstdio>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "Curves.hpp"
 
using namespace std;
using namespace cv;
 
static string window_name = "Photo";
static Mat src;
 
static string curves_window = "Adjust Curves";
static Mat curves_mat;
static int channel = 0;
Curves  curves;
 
static void invalidate()
{
	curves.draw(curves_mat);
	imshow(curves_window, curves_mat);
 
	Mat dst;
	curves.adjust(src, dst);
	imshow(window_name, dst);
 
	int y, x;
	uchar *p;
 
	y = 150; x = 50;
	p = dst.ptr<uchar>(y) + x * 3;
	cout << "(" << int(p[2]) << ", " << int(p[1]) << ", " << int(p[0]) << ")  ";
 
	y = 150; x = 220;
	p = dst.ptr<uchar>(y) + x * 3;
	cout << "(" << int(p[2]) << ", " << int(p[1]) << ", " << int(p[0]) << ")  ";
 
	y = 150; x = 400;
	p = dst.ptr<uchar>(y) + x * 3;
	cout << "(" << int(p[2]) << ", " << int(p[1]) << ", " << int(p[0]) << ")  " << endl;
}
 
static void callbackAdjustChannel(int , void *)
{
	switch (channel) {
	case 3:
		curves.CurrentChannel = &curves.BlueChannel;
		break;
	case 2:
		curves.CurrentChannel = &curves.GreenChannel;
		break;
	case 1:
		curves.CurrentChannel = &curves.RedChannel;
		break;
	default:
		curves.CurrentChannel = &curves.RGBChannel;
		break;
	}
 
 
	invalidate();
}
 
static void callbackMouseEvent(int mouseEvent, int x, int y, int flags, void* param)
{
	switch(mouseEvent) {
	case EVENT_LBUTTONDOWN:
		curves.mouseDown(x, y);
		invalidate();
		break;
	case EVENT_MOUSEMOVE:
		if ( curves.mouseMove(x, y) )
			invalidate();
		break;
	case EVENT_LBUTTONUP:
		curves.mouseUp(x, y);
		invalidate();
		break;
	}
	return;
}
 
 
int main(int argc, char** argv)
{
	//read image file
    cv::String img_path = argv[1];
	src = imread(img_path);
	if ( !src.data ) {
		cout << "error read image" << endl;
		return -1;
	}
 
	//create window
	namedWindow(window_name);
	imshow(window_name, src);
 
	//create Mat for curves
	curves_mat = Mat::ones(256, 256, CV_8UC3);
 
	//create window for curves
	namedWindow(curves_window);
	setMouseCallback(curves_window, callbackMouseEvent, NULL );
	createTrackbar("Channel", curves_window, &channel,  3, callbackAdjustChannel);
 
 
// 范例：用程序代码在RedChannel中定义一条曲线
//	curves.RedChannel.clearPoints();
//	curves.RedChannel.addPoint( Point(10,  10) );
//	curves.RedChannel.addPoint( Point(240, 240) );
//	curves.RedChannel.addPoint( Point(127, 127) );
 
	invalidate();
 
	waitKey();
 
	return 0;
}
 