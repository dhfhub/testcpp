
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include "Filter.hpp"

using namespace std;
using namespace cv;

static Mat src;
static Mat dst;
static int radius = 10;

static void callbackAdjust(int , void *)
{

	Filter::HighPass(src, dst, radius / 10.0 );
	imshow("photo", dst);
}

int main(int argc, char** argv)
{
	cv::String img_path = argv[1];
	src = imread(img_path);

	if ( !src.data ) {
		cout << "error read image" << endl;
		return -1;
	}

	namedWindow("photo");
	createTrackbar("radius", "photo", &radius, 100, callbackAdjust);
	callbackAdjust(0, 0);

	waitKey();
	return 0;

}
