#ifndef HAND_TRACKING_API_H
#define HAND_TRACKING_API_H

#define EXPORT

/* 定义动态链接库dll的导出 */
#include <malloc.h>
#ifdef _WIN32
    #ifdef EXPORT
        #define EXPORT_API __declspec(dllexport)
    #else
        #define EXPORT_API __declspec(dllimport)
    #endif
#else
#include <stdlib.h>

    #ifdef EXPORT
        #define EXPORT_API __attribute__((visibility ("default")))
    #else
    #endif

#endif


#ifdef __cplusplus
extern "C" {
#endif 

#ifndef EXPORT_API
#define EXPORT_API
#endif

    /*
    @brief 初始化Google Mediapipe
    @param[in] model_path 需要加载的模型路径
    @return 返回操作成功或者失败
        0 失败
        1 成功
    */
    EXPORT_API int HandTracking_Hand_Tracking_Init(const char* model_path);


	//非回调函数得到图像手部点
	EXPORT_API float* HandTracking_Hand_Tracking_Detect_Frame_New(int image_width, int image_height, int image_channels, void* image_data, float *landmarks, int &size_, bool log_flag);

    /*
    @brief Google Mediapipe释放
    @return 返回操作成功或者失败
        0 失败
        1 成功
    */
    EXPORT_API int HandTracking_Hand_Tracking_Release();


#ifdef __cplusplus
}
#endif 



#endif // !HAND_TRACKING_API_H

