#ifndef HAND_TRACKING_DETECT_H
#define HAND_TRACKING_DETECT_H

#include <cstdlib>
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/rect.pb.h"


namespace GoogleMediapipeHandTrackingDetect {
    class HandTrackingDetect
    {
    public:
        HandTrackingDetect();
        virtual ~HandTrackingDetect();

    public:
        int InitGraph(const char* model_path);
		//非回调函数得到图像手部点
		std::vector<float> DetectFrame_New(int image_width, int image_height, int image_channels, void* image_data, bool log_flag);
        int Release();

    private:
        absl::Status Mediapipe_InitGraph(const char* model_path);
        absl::Status Mediapipe_ReleaseGraph();


    private:
        bool m_bIsInit;
        bool m_bIsRelease;

        const char* m_kInputStream;
        const char* m_kOutputStream;
        const char* m_kWindowName;
        const char* m_kOutputLandmarks;

        mediapipe::CalculatorGraph m_Graph;

        std::unique_ptr<mediapipe::OutputStreamPoller> m_pPoller;
        std::unique_ptr<mediapipe::OutputStreamPoller> m_pPoller_landmarks;

    };

}


#endif // HAND_TRACKING_DETECT_H

