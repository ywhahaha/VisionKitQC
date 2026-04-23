#pragma once
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2/core/utils/logger.hpp>
#include "MvCameraControl.h"
#include "show_camera_config_window.h"
#include <mysql.h>
#include "CameraParameters.h"

class VI_Camera
{
public:
	VI_Camera();
	int build_camera_connect(std::string net_ip, int camera_type, void** handle);
	int grab_image(cv::Mat& image, void* handle);
	int close_camera_connect(void* handle);

	int ResetROI(void* handle);//还原相机ROI区域
	int CheckCameraConnection(void* handle1, void* handle2, void* handle3);	//查看相机连接状态

private:
	CameraParameters cameraparameters1;
	CameraParameters cameraparameters2;
	CameraParameters cameraparameters3;

	std::string camera_ip(int camera_type);
	void LoadCameraParameters(int camera_type, CameraParameters& cameraparameters);
	int SetCameraParameters(int camera_type, void* handle, CameraParameters cameraparameters);
};