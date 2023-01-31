#include "hand_tracking_api.h"
#include "hand_tracking_detect.h"

using namespace GoogleMediapipeHandTrackingDetect;

HandTrackingDetect m_HandTrackingDetect;

EXPORT_API int HandTracking_Hand_Tracking_Init(const char* model_path)
{
    return m_HandTrackingDetect.InitGraph(model_path);
}


//非回调函数得到图像手部点
EXPORT_API float * HandTracking_Hand_Tracking_Detect_Frame_New(int image_width, int image_height, int image_channels, void* image_data, float *landmarks, int &size_, bool log_flag)
{
	std::vector<float> pp= m_HandTrackingDetect.DetectFrame_New(image_width, image_height, image_channels, image_data, log_flag);
	size_ = pp.size();
	if (size_ > 0)
	{
		landmarks = (float*)malloc(sizeof(float)*size_);
		for (int i = 0; i < size_; i++)
		{
			landmarks[i] = pp[i];
		}	
	}
	return landmarks;
}


EXPORT_API int HandTracking_Hand_Tracking_Release()
{
    return m_HandTrackingDetect.Release();
}
