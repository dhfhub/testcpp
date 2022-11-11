#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
 
#include "BlackWhite.hpp"

using namespace std;
using namespace cv;
 
 
#define BASE 200
 
static string window_name = "photo";
static Mat src;
static int red     = 40 + BASE;
static int yellow  = 60 + BASE;
static int green   = 40 + BASE;
static int magenta = 60 + BASE;
static int blue    = 20 + BASE;
static int cyan    = 80 + BASE;
 
static void callbackAdjust(int , void *)
{
	Mat dst;
	BlackWhite b;
 
	//set params
	b.red = (red - BASE) / 100.0;
	b.yellow = (yellow - BASE) / 100.0;
	b.green = (green - BASE) / 100.0;
	b.magenta = (magenta - BASE) / 100.0;
	b.blue = (blue - BASE) / 100.0;
	b.cyan = (cyan - BASE) / 100.0;
 
	//adjust Black White
	b.adjust(src, dst);
 
	imshow(window_name, dst);
}
 
 
int main(int argc, char** argv)
{
	cv::String img_path = argv[1];
	src = imread(img_path);
 
	if ( !src.data ) {
		cout << "error read image" << endl;
		return -1;
	}
 
	namedWindow(window_name);
 
	//create trackbars
	createTrackbar("red", window_name, &red, 500, callbackAdjust);
	createTrackbar("yellow", window_name, &yellow, 500, callbackAdjust);
	createTrackbar("green", window_name, &green, 500, callbackAdjust);
	createTrackbar("cyan", window_name, &cyan, 500, callbackAdjust);
	createTrackbar("blue", window_name, &blue, 500, callbackAdjust);
	createTrackbar("magenta", window_name, &magenta, 500, callbackAdjust);
 
	callbackAdjust(0, 0);
 
	waitKey();
	return 0;
}
 