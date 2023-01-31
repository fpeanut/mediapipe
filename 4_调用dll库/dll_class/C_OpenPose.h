#pragma once
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include<openpose/headers.hpp>


using namespace std;
using namespace cv;

class C_OpenPose {
public:
	C_OpenPose(std::string _model_path);//变量
	bool InitOpenPose();//初始化IrisTracking
	bool m_Init_OpenPose_OK = false;//初始化成功标识符
	std::vector<std::vector<Point>> landmarks;
	bool getLandmarks(Mat img);//获得所有骨架点

private:
	std::string model_path;//模型路径
	op::Wrapper m_opWrapper{ op::ThreadManagerMode::Asynchronous };//定义骨架配置器

	int logging_level = 3;
	bool disable_multi_thread = false;
	int gpuNumber = -1;
	int gpuNumberStart = 0;
	int scalesNumber = 1;
	float scaleGap = 0.25;
	bool blendOriginalFrame = true;//是否在原图渲染
	float alphaKeypoint = 0.6;//渲染的透明度，范围0-1，越低越透明
	float alphaHeatMap = 0.7;//0-1之间，暂时无用
	int defaultPartToRender = 0;//0-4显示不同类图
	int numberPeopleMax = -1;
	float renderThreshold = 0.1;//渲染阈值,pose检测阈值

};

