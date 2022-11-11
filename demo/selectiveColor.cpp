#include <cstdio>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "SelectiveColor.hpp"

using namespace std;
using namespace cv;

static string window_name = "Photo";
static Mat src;

static Mat adjust_mat;
static string adjust_window = "Selective Color";
static int color = 2;
SelectiveColor  selectiveColor;

vector<Point> points;

int   cyan;
int   magenta;
int   yellow;
int   black;
int   is_absolute;

static void invalidate()
{
	Mat dst;
	selectiveColor.adjust(src, dst);

	imshow(window_name, dst);
	imshow(adjust_window, adjust_mat);
}

static void channelRead(int which)
{
	color = which;

	SelectiveColorAdjust * current = NULL;
	if ( color >=0 && color <= 9)
		current = &(selectiveColor.colors[color]);
	if ( current == NULL ) return;

	cyan = (current->cyan < 0) ? (current->cyan + 1) * 100 : current->cyan * 100;
	magenta = (current->magenta < 0) ? (current->magenta + 1) * 100 : current->magenta * 100;
	yellow = (current->yellow < 0) ? (current->yellow + 1) * 100 : current->yellow * 100;
	black = (current->black < 0) ? (current->black + 1) * 100 : current->black * 100;

	if ( selectiveColor.isAbsolute )
		is_absolute = 1;
	else
		is_absolute = 0;

}

static void channelWrite()
{
	SelectiveColorAdjust * current = NULL;
    if ( color >=0 && color <= 9)
    	current = &(selectiveColor.colors[color]);
	if ( current == NULL ) return;

	current->cyan = (cyan - 100 ) / 100.0;
	current->magenta =  (magenta - 100 ) / 100.0;
	current->yellow =  (yellow - 100 ) / 100.0;
	current->black =  (black - 100 ) / 100.0;

	selectiveColor.isAbsolute = ( is_absolute == 1 );
	invalidate();
}


static void callbackAdjust(int , void *)
{
	channelWrite();
	invalidate();
}


static void callbackAdjustChannel(int , void *)
{
	channelRead(color);
	setTrackbarPos("cyan", adjust_window, cyan);
	setTrackbarPos("magenta", adjust_window, magenta);
	setTrackbarPos("yellow", adjust_window, yellow);
	setTrackbarPos("black", adjust_window, black);
	setTrackbarPos("Absolute", adjust_window, is_absolute);
	invalidate();
}

static void callbackMouseEvent(int mouseEvent, int x, int y, int flags, void* param)
{
	switch(mouseEvent) {
	case EVENT_LBUTTONDOWN:
		break;
	case EVENT_MOUSEMOVE:
		break;
	case EVENT_LBUTTONUP:
		points.push_back(Point(x, y));
		invalidate();
		break;
	case EVENT_LBUTTONDBLCLK:
		points.clear();
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
	setMouseCallback(window_name, callbackMouseEvent, NULL );


	//create window for levels
	namedWindow(adjust_window);
	adjust_mat = Mat::ones(100,400, CV_8UC3);
	adjust_mat.setTo( Scalar(255,255,255) );
	imshow(adjust_window, adjust_mat);

	channelRead(0);
	createTrackbar("Color", adjust_window, &color,  8, callbackAdjustChannel);
	createTrackbar("cyan", adjust_window, &cyan,  200, callbackAdjust);
	createTrackbar("magenta", adjust_window, &magenta,  200, callbackAdjust);
	createTrackbar("yellow", adjust_window, &yellow,  200, callbackAdjust);
	createTrackbar("black", adjust_window, &black,  200, callbackAdjust);
	createTrackbar("Absolute", adjust_window, &is_absolute,  1, callbackAdjust);

	waitKey();
	return 0;
}


