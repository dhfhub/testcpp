//
// Created by dhf on 2022/9/14.
//

#ifndef PRE_PROCESS_IMAGE_H
#define PRE_PROCESS_IMAGE_H

#include <string>
#include <iostream>
#include <algorithm>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "Curves.hpp"
#include "ColorSpace.hpp"
#include "Levels.hpp"
#include "Filter.hpp"
#include "HSL.hpp"

class PreProcessImage{
public:
    PreProcessImage();
    ~PreProcessImage();

    // virtual int initPreProcessImage(const char* modelPath);
    // virtual int getRedMap(const cv::Mat* InputImage, cv::Mat* RedMap);
    // virtual int getSpotSpical(const cv::Mat* InputImage, cv::Mat* SpotSpical);
    // virtual int getUVHigh(const cv::Mat* InputImage, cv::Mat* UVHigh);
    // virtual int getRedPredict(const cv::Mat* InputImage, int level, cv::Mat* RedPredict);
    // virtual int getSpotPredict(const cv::Mat* InputImage, int level, cv::Mat* RedMap);
    // int initPreProcessImage(const char* modelPath);
    // int getRedMap();
    // int getRedMap1();   // thinkview
    // int getRedMap2();   // visia
    // int getRedMap3();   // visia
    // int getBrownMap();
    // int getBrownMap1(); // thinkview
    // int getBrownMap2(); // thinkview
    // int getUVHigh();
    // int getUVHigh1();
    // int getUVHigh2();
    // int getRedPredict();
    // int getRedPredict1();
    // int getSpotPredict();

    int getRedPolarized(const cv::Mat* InputImage, cv::Mat* RedPolarized);
    int getRedMap(const cv::Mat* InputImage, cv::Mat* RedMap);
    int getBrownMap(const cv::Mat* InputImage, cv::Mat* BrownMap);
    int getUVHigh(const cv::Mat* InputImage, cv::Mat* UVHigh);
    int getRedPredict(const cv::Mat* InputImage, cv::Mat* RedPredict);
    int getSpotPredict(const cv::Mat* InputImage, cv::Mat* SpotPredict);
    
private:
    cv::Mat image_sun;
    cv::Mat image_cross;
    cv::Mat image_uv;

    Curves  curves;

public:
    cv::Mat image_RedPolarized;
    cv::Mat image_RedMap;
    cv::Mat image_BrownMap;
    cv::Mat image_UVHigh;
    cv::Mat image_RedPredict;
    cv::Mat image_SpotPredict;
};


#endif //PRE_PROCESS_IMAGE_H