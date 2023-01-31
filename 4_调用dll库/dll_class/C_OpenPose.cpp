#include "C_OpenPose.h"


C_OpenPose::C_OpenPose(std::string _model_path) {
	model_path = _model_path;
	if (InitOpenPose()) {
		m_Init_OpenPose_OK = true;
		cout << "Init OpenPose: True" << endl;
	}
	else {
		m_Init_OpenPose_OK = false;
		cout << "Init OpenPose: False" << endl;
	}
};


bool C_OpenPose::InitOpenPose() {//骨架初始化
	op::opLog("Starting OpenPose demo...", op::Priority::High);
	const auto opTimer = op::getTimerInit();
	// Configuring OpenPose
	op::opLog("Configuring OpenPose...", op::Priority::High);
	try
	{
		// Configuring OpenPose
		// logging_level
		op::checkBool(
			0 <= logging_level && logging_level <= 255, "Wrong logging_level value.", __LINE__, __FUNCTION__, __FILE__);
		op::ConfigureLog::setPriorityThreshold((op::Priority)logging_level);
		op::Profiler::setDefaultX(1000);

		// 初始化pose参数
		const auto outputSize = op::flagsToPoint(op::String("-1x-1"), "-1x-1");// outputSize
		const auto netInputSize = op::flagsToPoint(op::String("256x368"), "-1x368");// netInputSize
		const auto poseMode = op::flagsToPoseMode(1);// poseMode
		const auto poseModel = op::flagsToPoseModel(op::String("BODY_25"));// poseModel
		const auto keypointScaleMode = op::flagsToScaleMode(0);// keypointScaleMode
		// heatmaps to add
		const auto heatMapTypes = op::flagsToHeatMaps(false, false, false);
		const auto heatMapScaleMode = op::flagsToHeatMapScaleMode(2);
		const auto multipleView = (false || -1 > 1);// >1 camera view?
		const bool enableGoogleLogging = true;// Enabling Google Logging

		// pose骨架配置
		const op::WrapperStructPose wrapperStructPose{
			poseMode, netInputSize, outputSize, keypointScaleMode, gpuNumber, gpuNumberStart,scalesNumber, scaleGap, op::flagsToRenderMode(-1, multipleView),
			poseModel, blendOriginalFrame, alphaKeypoint, alphaHeatMap,defaultPartToRender, op::String(model_path), heatMapTypes, heatMapScaleMode, false,
			renderThreshold, numberPeopleMax, false, -1,op::String(""), op::String(""),(float)0, enableGoogleLogging };
		m_opWrapper.configure(wrapperStructPose);
		//禁用多线程
		if (disable_multi_thread)
			m_opWrapper.disableMultiThreading();
	}
	catch (const std::exception& e)
	{
		op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
		return false;
	}
	// Starting OpenPose
	op::opLog("Starting thread(s)...", op::Priority::High);
	m_opWrapper.start();
	return true;
}

bool C_OpenPose::getLandmarks(Mat img)
{
	landmarks.clear();
	try
	{
		// Process and display image
		const cv::Mat cvImageToProcess = img;
		const op::Matrix imageToProcess = OP_CV2OPCONSTMAT(cvImageToProcess);
		auto datumProcessed = m_opWrapper.emplaceAndPop(imageToProcess);
		if (datumProcessed != nullptr)
		{
			//img = OP_OP2CVCONSTMAT(datumProcessed->at(0)->cvOutputData);//cuda骨架渲染
			//变量定义
			int pose_number = datumProcessed->at(0)->poseScores.getVolume();//获取检测骨架数量
			vector<cv::Point2i> solo_keypoints;//单人骨架点容器
			//存储关键点
			for (int i = 0; i < pose_number; i++) {
				//剔除非人骨架
				if (datumProcessed->at(0)->poseKeypoints[i * 75 + 0] && datumProcessed->at(0)->poseKeypoints[i * 75 + 1]) {
					solo_keypoints.clear();
					for (int j = 0; j < 25; j++) {
						int p_x = datumProcessed->at(0)->poseKeypoints[i * 75 + 3 * j];
						int p_y = datumProcessed->at(0)->poseKeypoints[i * 75 + 3 * j + 1];
						solo_keypoints.push_back(cv::Point2i(p_x, p_y));
					}
					landmarks.push_back(solo_keypoints);
				}
			}
			return true;

		}
		else {
			op::opLog("Image could not be processed.", op::Priority::High);
			return false;
		}
	}
	catch (const std::exception&)
	{
		return false;
	}
}