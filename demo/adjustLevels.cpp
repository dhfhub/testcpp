/*
 * test_Levels
 *
 *  Created on: 2016年9月11日
 *      Author: Administrator
 */


#include <cstdio>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "Levels.hpp"

using namespace std;
using namespace cv;

static string window_name = "Photo";
static Mat src;

static Mat levels_mat;
static string levels_window = "Adjust Levels";
static int channel = 0;
Levels  levels;

int   Shadow;
int   Midtones = 100;
int   Highlight;
int   OutputShadow;
int   OutputHighlight;

static void invalidate()
{
	Mat dst;
	levels.adjust(src, dst);
	imshow(window_name, dst);

	imshow(levels_window, levels_mat);
}

static void channelRead(int which_channel)
{
	channel = which_channel;
	Level * CurrentChannel = NULL;
	switch (channel) {
	case 0: CurrentChannel = &levels.RGBChannel; break;
	case 1: CurrentChannel = &levels.RedChannel; break;
	case 2: CurrentChannel = &levels.GreenChannel; break;
	case 3: CurrentChannel = &levels.BlueChannel; break;
	}
	if ( CurrentChannel == NULL ) return;

	Shadow = CurrentChannel->Shadow;
	Midtones = int (CurrentChannel->Midtones * 100);
	Highlight = CurrentChannel->Highlight;
	OutputShadow = CurrentChannel->OutputShadow;
	OutputHighlight = CurrentChannel->OutputHighlight;

}

static void channelWrite()
{
	Level * CurrentChannel = NULL;
	switch (channel) {
	case 0: CurrentChannel = &levels.RGBChannel; break;
	case 1: CurrentChannel = &levels.RedChannel; break;
	case 2: CurrentChannel = &levels.GreenChannel; break;
	case 3: CurrentChannel = &levels.BlueChannel; break;
	}

	if ( CurrentChannel == NULL )
		return ;

	CurrentChannel->Shadow = Shadow;
	CurrentChannel->Midtones = Midtones / 100.0;
	CurrentChannel->Highlight = Highlight;
	CurrentChannel->OutputShadow = OutputShadow;
	CurrentChannel->OutputHighlight = OutputHighlight;

	invalidate();
}


static void callbackAdjust(int , void *)
{
	channelWrite();
	invalidate();
}


static void callbackAdjustChannel(int , void *)
{
	channelRead(channel);
	setTrackbarPos("Shadow", levels_window, Shadow);
	setTrackbarPos("Midtones", levels_window, Midtones);
	setTrackbarPos("Highlight", levels_window, Highlight);
	setTrackbarPos("OutShadow", levels_window, OutputShadow);
	setTrackbarPos("OutHighlight", levels_window, OutputHighlight);
	invalidate();
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


	//create window for levels
	namedWindow(levels_window);
	levels_mat = Mat::ones(100,400, CV_8UC3);
	levels_mat.setTo( Scalar(255,255,255) );
	imshow(levels_window, levels_mat);

	channelRead(0);
	createTrackbar("Channel", levels_window, &channel,  3, callbackAdjustChannel);
	createTrackbar("Shadow", levels_window, &Shadow,  255, callbackAdjust);
	createTrackbar("Midtones", levels_window, &Midtones,  200, callbackAdjust);
	createTrackbar("Highlight", levels_window, &Highlight,  255, callbackAdjust);
	createTrackbar("OutShadow", levels_window, &OutputShadow,  255, callbackAdjust);
	createTrackbar("OutHighlight", levels_window, &OutputHighlight,  255, callbackAdjust);

	waitKey();

	return 0;

}


