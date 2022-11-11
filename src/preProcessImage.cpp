#include "preProcessImage.h"

PreProcessImage::PreProcessImage()
{
    
}

PreProcessImage::~PreProcessImage()
{

}

int PreProcessImage::getRedPolarized(const cv::Mat* InputImage, cv::Mat* RedPolarized)
{
    double t = (double)cv::getTickCount();

    cv::Mat Mat1 = image_cross;
    cv::Mat Mat1_lab, Mat1_hsv;
    cv::Mat Mat1_lab_f, Mat1_hsv_f;
    std::vector<cv::Mat> Mat1_lab_vector;
    std::vector<cv::Mat> Mat1_hsv_vector;

    cv::cvtColor(Mat1, Mat1_lab, cv::COLOR_BGR2Lab);
    cv::cvtColor(Mat1, Mat1_hsv, cv::COLOR_BGR2HSV);
    Mat1_lab.convertTo(Mat1_lab_f, CV_32F, 1/255.0);
    Mat1_hsv.convertTo(Mat1_hsv_f, CV_32F, 1/255.0);
    cv::split(Mat1_lab_f, Mat1_lab_vector);
    cv::split(Mat1_hsv_f, Mat1_hsv_vector);

    cv::Mat Mat1_lab_a = Mat1_lab_vector.at(1);
    cv::Mat Mat1_hsv_v = Mat1_hsv_vector.at(2);
    
    // 取Mat1_lab_b图像中的最大最小像素值
    // float min = 0.45784314;
    // float max = 0.6549;
    cv::Mat Mat1_lab_b = Mat1_lab_vector.at(2);
    float min,max;
    max = *std::max_element(Mat1_lab_b.begin<float>(), Mat1_lab_b.end<float>());
    min = *std::min_element(Mat1_lab_b.begin<float>(), Mat1_lab_b.end<float>());
    min = 0.45784314;
    max = 0.6549;

    
    int nr = Mat1_lab_a.rows;
    int nc = Mat1_lab_a.cols;
    cv::Mat outImage;
    outImage.create(Mat1_lab_a.size(), Mat1_lab_a.type());
    if(Mat1_lab_a.isContinuous() && outImage.isContinuous())
    {
        nr = 1;
        nc = nc*Mat1_lab_a.rows*Mat1_lab_a.channels();
        std::cout << "test"<< std::endl;
    }
    for(int i=0;i<nr;i++)
    {
        const float* inData = Mat1_lab_a.ptr<float>(i);
        float* outData = outImage.ptr<float>(i);
        for(int j=0; j<nc; j++)
        {
            if (*inData < min)
            {
                *outData = 0;
            }
            else if (*inData > max)
            {
                *outData = 1;
            }
            else
            {
                *outData = (*inData - min)/(max-min);
            }
            outData ++;
            inData ++;
        }
    }


    // hsv增加亮度
    nr = Mat1_hsv_v.rows;
    nc = Mat1_hsv_v.cols;
    cv::Mat outImage_hsv_v;
    outImage_hsv_v.create(Mat1_hsv_v.size(), Mat1_hsv_v.type());
    if(Mat1_hsv_v.isContinuous() && outImage_hsv_v.isContinuous())
    {
        nr = 1;
        nc = nc*Mat1_hsv_v.rows*Mat1_hsv_v.channels();
        std::cout << "test" << std::endl;
    }
    for(int i=0;i<nr;i++)
    {
        const float* inData_v = Mat1_hsv_v.ptr<float>(i);
        float* outData_v = outImage_hsv_v.ptr<float>(i);
        for(int j=0; j<nc; j++)
        {
            // if (*inData_v>0.098039216)
            // {
            //     *outData_v = *inData_v + 0.2;
            // }

            *outData_v = *inData_v;
            
            if (*outData_v > 1)
            {
                *outData_v = 1;
            }
            
            outData_v ++;
            inData_v ++;
        }
    }
    
    // 创建空白单通道图片
    cv::Mat Mat1_zero;
    cv::Mat Mat1_a_temp;
    cv::Mat Mat1_v_temp;
    cv::Mat Mat1A;
    cv::Mat Mat1A_rgb;
    std::vector<cv::Mat> channels;
    Mat1_zero.create(outImage.size(), CV_8UC1);
    cv::Mat kernel(outImage.size(), CV_8UC1, cv::Scalar(0));
    outImage.convertTo(Mat1_a_temp, CV_8U, 255); 
    Mat1_hsv_v.convertTo(Mat1_v_temp, CV_8U, 255);
    // outImage_hsv_v.convertTo(Mat1_v_temp, CV_8U, 255);
    channels.push_back(kernel);
    channels.push_back(Mat1_a_temp);
    channels.push_back(Mat1_v_temp);
    cv::merge(channels, Mat1A);
    cv::cvtColor(Mat1A, Mat1A_rgb, cv::COLOR_HSV2BGR);

    // 曲线调色
    curves.RGBChannel.clearPoints();
    // curves.RedChannel.clearPoints();
    // curves.GreenChannel.clearPoints();
    // curves.BlueChannel.clearPoints();
    // curves.RGBChannel.addPoint( Point(0, 0) );
	curves.RGBChannel.addPoint( Point(38,  36) );
	curves.RGBChannel.addPoint( Point(129, 170) );
    // curves.RGBChannel.addPoint( Point(255, 255) );

    cv::Mat curves_dst;
	curves.adjust(Mat1A_rgb, curves_dst);
    


    image_RedPredict = Mat1A_rgb;


    // 保存图像
    std::vector<int> compression_params;
	compression_params.push_back(16);
	compression_params.push_back(0);
    cv::imwrite("redp.png", image_RedPredict, compression_params);  











    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency()*1000; //获得时间，单位是毫秒
    std::cout << "data指针遍历:" << t << "ms" << std::endl;
}

int PreProcessImage::getRedMap(const cv::Mat* InputImage, cv::Mat* RedMap)
{

}

int PreProcessImage::getBrownMap(const cv::Mat* InputImage, cv::Mat* BrownMap)
{

}

int PreProcessImage::getUVHigh(const cv::Mat* InputImage, cv::Mat* UVHigh)
{

}

int PreProcessImage::getRedPredict(const cv::Mat* InputImage, cv::Mat* RedPredict)
{

}

int PreProcessImage::getRedPredict(const cv::Mat* InputImage, cv::Mat* RedPredict)
{

}
