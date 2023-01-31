#include <vector>
#include "hand_tracking_detect.h"


GoogleMediapipeHandTrackingDetect::HandTrackingDetect::HandTrackingDetect()
{
    m_bIsInit = false;
    m_bIsRelease = false;
    m_kInputStream = "input_video";
    m_kOutputStream = "output_video";
    m_kWindowName = "MediaPipe";
    m_kOutputLandmarks = "landmarks";
}

GoogleMediapipeHandTrackingDetect::HandTrackingDetect::~HandTrackingDetect()
{
    if (!m_bIsRelease)
    {
        Release();
    }
}

int GoogleMediapipeHandTrackingDetect::HandTrackingDetect::InitGraph(const char* model_path)
{
    absl::Status run_status = Mediapipe_InitGraph(model_path);
    if (!run_status.ok()) 
    {
        return 0;
    }
    m_bIsInit = true;
    return  1;
}


//非回调函数得到图像手部点
std::vector<float> GoogleMediapipeHandTrackingDetect::HandTrackingDetect::DetectFrame_New(int image_width, int image_height, int image_channels, void* image_data, bool log_flag){
	//初始化手部点
	std::vector<float> hand_landmarks;
	hand_landmarks.clear();
	
	if (!m_bIsInit){
		printf("m_bIsInit False Before Run Graph. \n");
		return hand_landmarks;
	}
	if (log_flag)
	{
		//std::cout << "1. data to cv::Mat" << std::endl;
		printf("1. data to cv::Mat.\n");
	}

	// 构造cv::Mat对象
	cv::Mat camera_frame;
	if (image_channels == 3){
		cv::Mat camera_frame0(cv::Size(image_width, image_height), CV_8UC3, (uchar*)image_data);
		cv::cvtColor(camera_frame0, camera_frame, cv::COLOR_BGR2RGB);
	}

	else if (image_channels == 1){
		cv::Mat camera_frame0(cv::Size(image_width, image_height), CV_8UC1, (uchar*)image_data);
		cv::cvtColor(camera_frame0, camera_frame, cv::COLOR_GRAY2RGB);
	}
	//cv::flip(camera_frame, camera_frame, /*flipcode=HORIZONTAL*/ 1);

	if (log_flag)
	{
		printf("2. Wrap Mat into an ImageFrame.\n");
	}
	
    // Wrap Mat into an ImageFrame.
    auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
        mediapipe::ImageFormat::SRGB, camera_frame.cols, camera_frame.rows,
        mediapipe::ImageFrame::kDefaultAlignmentBoundary);
    cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
    camera_frame.copyTo(input_frame_mat);
    //std::cout << "Wrap Mat into an ImageFrame." << std::endl;

	if (log_flag)
	{
		printf("3. Send image packet into the graph.\n");
	}

    // Send image packet into the graph.
    size_t frame_timestamp_us = 
        (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;

	m_Graph.AddPacketToInputStream(
		m_kInputStream, mediapipe::Adopt(input_frame.release())
		.At(mediapipe::Timestamp(frame_timestamp_us)));

    // Get the graph result packet, or stop if that fails.
    mediapipe::Packet packet;
    mediapipe::Packet packet_landmarks;
	if (!m_pPoller->Next(&packet)) {
		if (log_flag)
		{
			printf("4. get the graph result packet: failed!!!\n");
		}
		return hand_landmarks;
	}

    if (m_pPoller_landmarks->QueueSize() > 0) 
    {
        if (m_pPoller_landmarks->Next(&packet_landmarks))
        {

            std::vector<mediapipe::NormalizedLandmarkList> output_landmarks = packet_landmarks.Get<std::vector<mediapipe::NormalizedLandmarkList>>();

            //int* hand_gesture_recognition_result = new int[output_landmarks.size()];

            for (int m = 0; m < output_landmarks.size(); ++m)
            {
                mediapipe::NormalizedLandmarkList single_hand_NormalizedLandmarkList = output_landmarks[m];

                for (int i = 0; i < single_hand_NormalizedLandmarkList.landmark_size(); ++i)
                {
                    const mediapipe::NormalizedLandmark landmark = single_hand_NormalizedLandmarkList.landmark(i);
                    float x = landmark.x() * camera_frame.cols;
                    float y = landmark.y() * camera_frame.rows;
                    hand_landmarks.push_back(x);
                    hand_landmarks.push_back(y);
                }
			}
			int landmarks_size = hand_landmarks.size();

			if (landmarks_size != 0)
			{
				if (log_flag)
				{
					printf("5. hand_landmarks.size(): [ %d ],First [%.6f],Last [%.6f]\n", landmarks_size, hand_landmarks[0], hand_landmarks[landmarks_size - 1]);
				}
				return hand_landmarks;
			}
			else
			{
				printf("5. hand_landmarks.size(): [ 0 ],First [None],Last [None]\n");
				return hand_landmarks;
			}
		}
	else
		{
			if (log_flag)
			{
				printf("5. No packet_landmarks detected!!!---END \n");
				
			}
			return hand_landmarks;
		}
	}
	else
	{
		if (log_flag)
		{
			printf("5. no m_ppoller_landmarks detected!!!---end \n");

		}
		return hand_landmarks;
	}
	
}



int GoogleMediapipeHandTrackingDetect::HandTrackingDetect::Release()
{
    absl::Status run_status = Mediapipe_ReleaseGraph();
    if (!run_status.ok()) {
        return 0;
    }
    m_bIsRelease = true;
    return 1;
}


absl::Status GoogleMediapipeHandTrackingDetect::HandTrackingDetect::Mediapipe_InitGraph(const char* model_path)
{
    std::string calculator_graph_config_contents;
    MP_RETURN_IF_ERROR(mediapipe::file::GetContents(model_path, &calculator_graph_config_contents));

    mediapipe::CalculatorGraphConfig config =
        mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(
            calculator_graph_config_contents);

    MP_RETURN_IF_ERROR(m_Graph.Initialize(config));

    // 添加video输出流
    auto sop = m_Graph.AddOutputStreamPoller(m_kOutputStream);
    assert(sop.ok());
    m_pPoller = std::make_unique<mediapipe::OutputStreamPoller>(std::move(sop.value()));

    // 添加landmarks输出流
    mediapipe::StatusOrPoller sop_landmark = m_Graph.AddOutputStreamPoller(m_kOutputLandmarks);
    assert(sop_landmark.ok());
    m_pPoller_landmarks = std::make_unique<mediapipe::OutputStreamPoller>(std::move(sop_landmark.value()));

    MP_RETURN_IF_ERROR(m_Graph.StartRun({}));

    return absl::OkStatus();
}


absl::Status GoogleMediapipeHandTrackingDetect::HandTrackingDetect::Mediapipe_ReleaseGraph()
{
    MP_RETURN_IF_ERROR(m_Graph.CloseInputStream(m_kInputStream));
    return m_Graph.WaitUntilDone();
}

