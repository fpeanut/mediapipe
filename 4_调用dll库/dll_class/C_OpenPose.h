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
	C_OpenPose(std::string _model_path);//����
	bool InitOpenPose();//��ʼ��IrisTracking
	bool m_Init_OpenPose_OK = false;//��ʼ���ɹ���ʶ��
	std::vector<std::vector<Point>> landmarks;
	bool getLandmarks(Mat img);//������йǼܵ�

private:
	std::string model_path;//ģ��·��
	op::Wrapper m_opWrapper{ op::ThreadManagerMode::Asynchronous };//����Ǽ�������

	int logging_level = 3;
	bool disable_multi_thread = false;
	int gpuNumber = -1;
	int gpuNumberStart = 0;
	int scalesNumber = 1;
	float scaleGap = 0.25;
	bool blendOriginalFrame = true;//�Ƿ���ԭͼ��Ⱦ
	float alphaKeypoint = 0.6;//��Ⱦ��͸���ȣ���Χ0-1��Խ��Խ͸��
	float alphaHeatMap = 0.7;//0-1֮�䣬��ʱ����
	int defaultPartToRender = 0;//0-4��ʾ��ͬ��ͼ
	int numberPeopleMax = -1;
	float renderThreshold = 0.1;//��Ⱦ��ֵ,pose�����ֵ

};

