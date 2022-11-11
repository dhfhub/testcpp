#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "preProcessImage.h"
#include "OpenCVWindowExt.h"

using namespace cv;
using namespace std;


float scale=1.0;

void zoomInAndOut(const float scale, const Mat srcImg, Mat &dstImg)
{
    Mat M=Mat::eye(3,3,CV_32FC1);
    int imgHeight=srcImg.rows;
    int imgWidth=srcImg.cols;

    uchar* pSrcData = (uchar*)srcImg.data;
    uchar* pDstData = (uchar*)dstImg.data;

    Point2f center(imgWidth / 2.0, imgHeight / 2.0);
    //计算仿射矩阵
    M.at<float>(0, 0) = scale;
    M.at<float>(0, 2) = (1 - scale)*center.x;
    M.at<float>(1, 1) = scale;
    M.at<float>(1, 2) = (1 - scale)*center.y;

    float a11 = M.at<float>(0, 0);
    float a12 = M.at<float>(0, 1);
    float a13 = M.at<float>(0, 2);
    float a21 = M.at<float>(1, 0);
    float a22 = M.at<float>(1, 1);
    float a23 = M.at<float>(1, 2);
    float a31 = M.at<float>(2, 0);
    float a32 = M.at<float>(2, 1);
    float a33 = M.at<float>(2, 2);

    float bx = a11*a22 - a21*a12;
    float by = a12*a21 - a11*a22;
    if ( abs(bx) > 1e-3 && abs(by) > 1e-3)
    {
        bx = 1.0 / bx;
        by = 1.0 / by;
        float cx = a13*a22 - a23*a12;
        float cy = a13*a21 - a23*a11;

        for (int j =0; j < imgHeight; j++)
        {
            for (int i = 0; i < imgWidth; i++)
            {
                float u = (a22*i - a12*j - cx) *bx;
                float v = (a21*i - a11*j - cy) *by;

                int u0 = floor(u);
                int v0 = floor(v);
                int u1 = floor(u0 + 1);
                int v1 = floor(v0 + 1);
                if (u0 >= 0 && v0 >= 0 && u1 < imgWidth && v1 < imgHeight)
                {
                    float dx = u - u0;
                    float dy = v - v0;
                    float weight1 = (1 - dx)*(1 - dy);
                    float weight2 = dx*(1 - dy);
                    float weight3 = (1 - dx)*dy;
                    float weight4 = dx*dy;

                    for (int k=0; k<srcImg.channels(); k++)
                    {
                        pDstData[j*imgWidth * 3 + i * 3 + k] = weight1*pSrcData[v0*imgWidth * 3 + u0 * 3 + k] +
                        weight2*pSrcData[v0*imgWidth * 3 + u1 * 3 + k] +
                        weight3*pSrcData[v1*imgWidth * 3 + u0 * 3 + k] +
                        weight4*pSrcData[v1*imgWidth * 3 + u1 * 3 + k];
                    }
                }
                else
                {
                    for (int k=0; k<srcImg.channels(); k++)
                    {
                        pDstData[j*imgWidth * 3 + i * 3 + k] = 0;
                    }
                }
            }
        }
    }
}

void onMouse(int event, int x, int y, int flags,  void* )
{
    double value;
    float step=0.02;
    switch (event)
    {
    case EVENT_MOUSEWHEEL:
        value = getMouseWheelDelta(flags);
        if (value>0)
            scale -=step;
        else if(value<0)
            scale +=step;
        break;
    default:
        break;
    }
}


int main(int argc, char *argv[])
{
    string cmd = argv[1];
    cv::String img_path = argv[2];
    cv::Mat input, display;

    input = imread(img_path);
    PreProcessImage preProcessImage;
    if (cmd == "red")
    {
        preProcessImage.getRedPolarized(&input, &display);
    }
    // 
    // preProcessImage.getRedMap3();
    // cv::Mat display = preProcessImage.image_RedMap;

    cv::namedWindow("img", cv::WINDOW_NORMAL);
    cv::resizeWindow("img", 700, 1000);

    cv::Mat dstImg = display.clone();
    dstImg.setTo(0);
    // imshow("img", display);
    // waitKey(10);

    setMouseCallback("img", onMouse, NULL);
    float scaleMin=1;
    float scaleMax=8;

    while (true)
    {
        scale = (scale<scaleMin)? (scaleMin): scale;
        scale = (scale>scaleMax)? (scaleMax): scale;
        zoomInAndOut(scale, display, dstImg);
        imshow("img", dstImg);
        if(cv::waitKey(27) == 113)
        {
            cv::destroyAllWindows();
            break;
        }
    }
    return 0;
}
