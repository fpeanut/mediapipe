#pragma once

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "DynamicModuleLoader.h"

using namespace DynamicModuleLoaderSpace;

using namespace std;
using namespace cv;


class C_IrisTracking
{
public:
	//_dll_name,_pbtxt_name可以采用默认的名字，也可以用自定义的名字，但要和实际名字对应
	C_IrisTracking(std::string _model_path, std::string _dll_name = "iris_tracking_dll.dll", std::string _pbtxt_name = "iris_tracking_cpu.pbtxt");
	~C_IrisTracking();
	bool m_Init_IrisTracking_OK = false;//初始化成功标识符
	bool InitIrisTracking();//初始化IrisTracking
	std::vector<Point> landmarks;
	bool getLandmarks(Mat img);

private:
	std::string model_path;//dll和pbtxt文件的路径
	std::string dll_name;//模型名称
	std::string pbtxt_name;//pbtxt名称

	typedef int(*FunC_IrisTracking_Iris_Tracking_Init)(const char* model_path);//初始化模型和配置文件
	typedef float* (*FunC_IrisTracking_Iris_Tracking_Detect_Frame_New)(int image_width, int image_height, int image_channels, void* image_data, float *landmarks, int &size_, int bool_flag);//获取landmarks
	typedef int(*FunC_IrisTracking_Iris_Tracking_Release)();//释放资源

	FunC_IrisTracking_Iris_Tracking_Init IrisTracking_Iris_Tracking_Init = nullptr;
	FunC_IrisTracking_Iris_Tracking_Release IrisTracking_Iris_Tracking_Release = nullptr;
	FunC_IrisTracking_Iris_Tracking_Detect_Frame_New IrisTracking_Iris_Tracking_Detect_Frame_New = nullptr;

	DynamicModuleLoader dllLoader;

};