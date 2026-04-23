#pragma once
#include "VI_Camera.h"
#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include <string>
#include "Get_Local_IP.h"
#include <direct.h>
#include <io.h>
#include "StateCode.h"

VI_Camera::VI_Camera()
{
}


/*建立相机连接*/
int VI_Camera::build_camera_connect(std::string net_ip, int camera_type, void** mhandle)
{
	int nRet = MV_OK;
	void* handle = NULL;
	switch (camera_type)
	{
	case 1:
	{
		LoadCameraParameters(camera_type, cameraparameters1);//载入相机初始属性
		break;
	}
	case 2:
	{
		LoadCameraParameters(camera_type, cameraparameters2);//载入相机初始属性
		break;
	}
	case 3:
	{
		LoadCameraParameters(camera_type, cameraparameters3);//载入相机初始属性
		break;
	}
	}

	MV_CC_DEVICE_INFO stDevInfo = { 0 };
	MV_GIGE_DEVICE_INFO stGigEDev = { 0 };

	/*需要连接的相机ip(根据实际填充)*/
	char nCurrentIp[128];
	strcpy(nCurrentIp, camera_ip(camera_type).c_str());
	/*相机对应的网卡ip(根据实际填充)*/
	char nNetExport[128];
	strcpy(nNetExport, net_ip.c_str());
	
	unsigned int nIp1, nIp2, nIp3, nIp4, nIp;

	sscanf_s(nCurrentIp, "%d.%d.%d.%d", &nIp1, &nIp2, &nIp3, &nIp4);
	nIp = (nIp1 << 24) | (nIp2 << 16) | (nIp3 << 8) | nIp4;
	stGigEDev.nCurrentIp = nIp;

	sscanf_s(nNetExport, "%d.%d.%d.%d", &nIp1, &nIp2, &nIp3, &nIp4);
	nIp = (nIp1 << 24) | (nIp2 << 16) | (nIp3 << 8) | nIp4;
	stGigEDev.nNetExport = nIp;

	stDevInfo.nTLayerType = MV_GIGE_DEVICE;// ch:仅支持GigE相机 | en:Only support GigE camera
	stDevInfo.SpecialInfo.stGigEInfo = stGigEDev;


	/*创建句柄*/
	nRet = MV_CC_CreateHandle(&handle, &stDevInfo);
	if (MV_OK != nRet)
	{
		return VI_E_CAMERA_CREATEHANDLE;
	}

	*mhandle = handle;
	mhandle = &handle;

	/*打开设备*/
	nRet = MV_CC_OpenDevice(handle);
	if (MV_OK != nRet)
	{
		return VI_E_CAMERA_OPEN;
	}

	switch (camera_type)
	{
	case 1:
	{
		SetCameraParameters(camera_type, handle, cameraparameters1);
		break;
	}
	case 2:
	{
		SetCameraParameters(camera_type, handle, cameraparameters2);
		break;
	}
	case 3:
	{
		SetCameraParameters(camera_type, handle, cameraparameters3);
		break;
	}
	}
	

	/*开始取流*/
	nRet = MV_CC_StartGrabbing(handle);
	if (MV_OK != nRet)
	{
		return VI_E_CAMERA_STARTGRAB;
	}

	return VI_CAMERA_READY;
}

/*抓取照片*/
int VI_Camera::grab_image(cv::Mat& image, void* handle)
{
	int nRet = MV_OK;
	cv::Mat Img;
	MV_SAVE_IMG_TO_FILE_PARAM stSaveFileParam;

	MV_FRAME_OUT stImageInfo = { 0 };

	memset(&stSaveFileParam, 0, sizeof(MV_SAVE_IMG_TO_FILE_PARAM));

	nRet = MV_CC_GetImageBuffer(handle, &stImageInfo, 1000);

	stSaveFileParam.enPixelType = stImageInfo.stFrameInfo.enPixelType;
	stSaveFileParam.nWidth = stImageInfo.stFrameInfo.nWidth;
	stSaveFileParam.nHeight = stImageInfo.stFrameInfo.nHeight;
	stSaveFileParam.nDataLen = stImageInfo.stFrameInfo.nFrameLen;
	stSaveFileParam.pData = stImageInfo.pBufAddr;
	Img = cv::Mat(stSaveFileParam.nHeight, stSaveFileParam.nWidth, CV_8UC1, stSaveFileParam.pData);

	nRet = MV_CC_FreeImageBuffer(handle, &stImageInfo);

	if (Img.empty())
	{
		return VI_E_CAMERA_IMAGEEMPTY;
	}

	image = Img.clone();
	return VI_CAMERA_PHOTO;
}


/*关闭相机连接*/
int VI_Camera::close_camera_connect(void* handle)
{
	int nRet = MV_OK;

	//停止取流
	nRet = MV_CC_StopGrabbing(handle);
	if (MV_OK != nRet)
	{
		return VI_E_CAMERA_STOPGRAB;
	}

	//关闭设备
	nRet = MV_CC_CloseDevice(handle);
	if (MV_OK != nRet)
	{
		return VI_E_CAMERA_CLOSE;
	}

	//销毁句柄
	nRet = MV_CC_DestroyHandle(handle);
	if (MV_OK != nRet)
	{
		return VI_E_CAMERA_DESTROYHANDLE;
	}

	return VI_CAMERA_EXIT;
}

/*查看相机连接状态*/
int VI_Camera::CheckCameraConnection(void* handle1, void* handle2, void* handle3)
{
	return MV_CC_IsDeviceConnected(handle1) + 2 * MV_CC_IsDeviceConnected(handle2) + 4 * MV_CC_IsDeviceConnected(handle3);
}

/*根据相机序号获取其IP地址*/
std::string VI_Camera::camera_ip(int camera_type)
{
	switch (camera_type)
	{
	case 1:
	{
		return cameraparameters1.ip;
	}
	case 2:
	{
		return cameraparameters2.ip;
	}
	case 3:
	{
		return cameraparameters3.ip;
	}
	default:
	{
		break;
	}
	}
}


void VI_Camera::LoadCameraParameters(int camera_type, CameraParameters& cameraparameters)
{
	INI_Config *ini = new INI_Config();

	ini->ReadINI("config.ini");//读.ini配置文件

	/*获取相机IP值*/
	cameraparameters.ip = ini->GetValue("Camera", "IP" + to_string(camera_type));

	/*设置心跳检测*/
	if (ini->GetValue("Camera", "SetGevHeartbeat") == "true")
	{
		cameraparameters.SetGevHeartbeat = true;
	}
	else
	{
		cameraparameters.SetGevHeartbeat = false;
	}

	/*设置心跳时间*/
	cameraparameters.GevHeartbeatTimeout = stoi(ini->GetValue("Camera", "GevHeartbeatTimeout"));

	/*设置Gev传输包获取方式*/
	if (ini->GetValue("Camera", "GetGevPacketSizeAuto") == "true")
	{
		cameraparameters.GetGevPacketSizeAuto = true;
	}
	else
	{
		cameraparameters.GetGevPacketSizeAuto = false;
	}
	
	/*设置Gev传输包大小*/
	cameraparameters.GevPacketSize = stoi(ini->GetValue("Camera", "GevPacketSize"));

	/*设置Gev包传输延时*/
	cameraparameters.GevSCPD = stoi(ini->GetValue("Camera", "GevSCPD"));

	/*设置帧率*/
	cameraparameters.AcqusitionFrameRate = stoi(ini->GetValue("Camera", "AcqusitionFrameRate" + to_string(camera_type)));
	

	/*设置曝光时间*/
	cameraparameters.ExposureTime = stoi(ini->GetValue("Camera", "ExposureTime" + to_string(camera_type)));

	/*设置增益*/
	cameraparameters.Gain = stof(ini->GetValue("Camera", "Gain" + to_string(camera_type)));

	/*设置是否开启黑电平*/
	if (ini->GetValue("Camera", "BlackLevelEnable" + to_string(camera_type)) == "true")
	{
		cameraparameters.BlackLevelEnable = true;
	}
	else
	{
		cameraparameters.BlackLevelEnable = false;
	}


	/*设置相机黑电平值*/
	cameraparameters.BlackLevel = stoi(ini->GetValue("Camera", "BlackLevel" + to_string(camera_type)));

	/*设置下采样比例*/
	cameraparameters.decimation = stoi(ini->GetValue("Camera", "Decimation_Camera" + to_string(camera_type)));
	cameraparameters.DecimationCamera = CameraParameters::Decimation_Camera(cameraparameters.decimation);//相机1下采样比例

	ini->Clear();
}


/*设置相机参数*/
int VI_Camera::SetCameraParameters(int camera_type, void* handle, CameraParameters cameraparameters)
{
	int nRet = MV_OK;
	INI_Config *ini = new INI_Config();

	/*设置相机是否开启心跳检测，true为关闭心跳检测*/
	nRet = MV_CC_SetBoolValue(handle, "GevGVCPHeartbeatDisable", cameraparameters.SetGevHeartbeat);
	if (MV_OK != nRet)
	{
		return VI_E_SET_HEARTBEAT;
	}

	/*设置相机心跳时间，在心跳时间内，若未收到 SDK 心跳回应，则将相机占用状态清楚*/
	if (!cameraparameters3.SetGevHeartbeat)
	{
		nRet = MV_CC_SetIntValue(handle, "GevHeartbeatTimeout", cameraparameters.GevHeartbeatTimeout);
		if (MV_OK != nRet)
		{
			return VI_E_SET_HEARTBEATTIME;
		}
	}

	/*设置相机传输包大小*/
	if (cameraparameters.GetGevPacketSizeAuto)
	{
		/*自动获取相机传输包大小*/
		/*探测网络最佳包大小(只对GigE相机有效)*/
		int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
		if (nPacketSize > 0)
		{
			nRet = MV_CC_SetIntValue(handle, "GevSCPSPacketSize", nPacketSize);
			if (nRet != MV_OK)
			{
				return VI_E_SET_PACKETSIZE;
			}
		}
		else
		{
			return VI_E_SET_PACKETSIZE;
		}
	}
	else
	{
		/*手动获取相机传输包大小*/
		nRet = MV_CC_SetIntValue(handle, "GevSCPSPacketSize", cameraparameters.GevPacketSize);
		if (MV_OK != nRet)
		{
			return VI_E_SET_PACKETSIZE;
		}
	}


	/*设置传输延迟时间*/
	nRet = MV_CC_SetIntValue(handle, "GevSCPD", cameraparameters.GevSCPD);
	if (MV_OK != nRet)
	{
		return VI_E_SET_SCPD;
	}


	/*设置float型参数-帧率*/
	nRet = MV_CC_SetFloatValue(handle, "AcquisitionFrameRate", cameraparameters.AcqusitionFrameRate);
	if (MV_OK != nRet)
	{
		return VI_E_SET_FPS;
	}

	/*设置曝光时间*/
	nRet = MV_CC_SetFloatValue(handle, "ExposureTime", cameraparameters.ExposureTime);
	if (MV_OK != nRet)
	{
		return VI_E_SET_EXPOSURETIME;
	}

	/*设置增益*/
	nRet = MV_CC_SetFloatValue(handle, "Gain", cameraparameters.Gain);
	if (MV_OK != nRet)
	{
		return VI_E_SET_GAIN;
	}

	/*设置相机是否开启黑电平，true为开启*/
	nRet = MV_CC_SetBoolValue(handle, "BlackLevelEnable", cameraparameters.BlackLevelEnable);
	if (MV_OK != nRet)
	{
		return VI_E_SET_BLACKLEVELENABLE;
	}

	/*设置相机黑电平值*/
	if (cameraparameters3.BlackLevelEnable)
	{
		nRet = MV_CC_SetIntValue(handle, "BlackLevel", cameraparameters.BlackLevel);
		if (MV_OK != nRet)
		{
			return VI_E_SET_BLACKLEVEL;
		}
	}


	/*设置相机下采样比例*/
	/**设置相机下采样水平方向比例**/
	nRet = MV_CC_SetEnumValue(handle, "DecimationHorizontal", cameraparameters.DecimationCamera);
	if (MV_OK != nRet)
	{
		return VI_E_SET_DECIMATIONHORIZONTAL;
	}

	/**设置相机下采样垂直方向比例**/
	nRet = MV_CC_SetEnumValue(handle, "DecimationVertical", cameraparameters.DecimationCamera);
	if (MV_OK != nRet)
	{
		return VI_E_SET_DECIMATIONVERTICAL;
	};
	
	/*显示相机当前最大分辨率的图像*/
	MVCC_INTVALUE WidthMax, HeightMax;
	/**获取相机当前最大分辨率**/
	nRet = MV_CC_GetIntValue(handle, "WidthMax", &WidthMax);
	if (MV_OK != nRet)
	{
		return VI_E_SET_WIDTHMAX;
	}
	nRet = MV_CC_GetIntValue(handle, "HeightMax", &HeightMax);
	if (MV_OK != nRet)
	{
		return VI_E_SET_HEIGHTMAX;
	}
	/**设置相机ROI区域水平偏移**/
	nRet = MV_CC_SetIntValue(handle, "OffsetX", 0);
	if (MV_OK != nRet)
	{
		return VI_E_SET_OFFSETX;
	}
	/**设置相机1ROI区域垂直偏移**/
	nRet = MV_CC_SetIntValue(handle, "OffsetY", 0);
	if (MV_OK != nRet)
	{
		return VI_E_SET_OFFSETY;
	}
	/**设置相机1ROI区域宽度**/
	nRet = MV_CC_SetIntValue(handle, "Width", WidthMax.nCurValue);
	if (MV_OK != nRet)
	{
		return VI_E_SET_WIDTHMAX;
	}
	/**设置相机1ROI区域高度**/
	nRet = MV_CC_SetIntValue(handle, "Height", HeightMax.nCurValue);
	if (MV_OK != nRet)
	{
		return VI_E_SET_HEIGHTMAX;
	}


	ini->ReadINI("config.ini");//读.ini配置文件

	/*设置相机ROI区域*/
	cameraparameters.OffsetX = stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_OffsetX_Union"));
	cameraparameters.OffsetY = stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_OffsetY_Union"));
	cameraparameters.ImageWidth = stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_Width_Union"));
	cameraparameters.ImageHeight = stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_Height_Union"));

	ini->Clear();

	/*设置相机ROI区域大小*/

	/**设置相机ROI区域宽度**/
	nRet = MV_CC_SetIntValue(handle, "Width", cameraparameters.ImageWidth);
	if (MV_OK != nRet)
	{
		return VI_E_SET_WIDTHMAX;
	}
	/**设置相机ROI区域高度**/
	nRet = MV_CC_SetIntValue(handle, "Height", cameraparameters.ImageHeight);
	if (MV_OK != nRet)
	{
		return VI_E_SET_HEIGHTMAX;
	}
	/**设置相机ROI区域水平偏移**/
	nRet = MV_CC_SetIntValue(handle, "OffsetX", cameraparameters.OffsetX);
	if (MV_OK != nRet)
	{
		return VI_E_SET_OFFSETX;
	}
	/**设置相机ROI区域垂直偏移**/
	nRet = MV_CC_SetIntValue(handle, "OffsetY", cameraparameters.OffsetY);
	if (MV_OK != nRet)
	{
		return VI_E_SET_OFFSETY;
	}

	return 0;
}

int VI_Camera::ResetROI(void* handle)
{
	int nRet = MV_OK;

	/*停止取流*/
	nRet = MV_CC_StopGrabbing(handle);
	if (MV_OK != nRet)
	{
		return VI_E_CAMERA_STOPGRAB;
	}

	/*显示相机当前最大分辨率的图像*/
	MVCC_INTVALUE WidthMax, HeightMax;
	/**获取相机当前最大分辨率**/
	nRet = MV_CC_GetIntValue(handle, "WidthMax", &WidthMax);
	if (MV_OK != nRet)
	{
		return VI_E_SET_WIDTHMAX;
	}
	nRet = MV_CC_GetIntValue(handle, "HeightMax", &HeightMax);
	if (MV_OK != nRet)
	{
		return VI_E_SET_HEIGHTMAX;
	}
	/**设置相机ROI区域水平偏移**/
	nRet = MV_CC_SetIntValue(handle, "OffsetX", 0);
	if (MV_OK != nRet)
	{
		return VI_E_SET_OFFSETX;
	}
	/**设置相机1ROI区域垂直偏移**/
	nRet = MV_CC_SetIntValue(handle, "OffsetY", 0);
	if (MV_OK != nRet)
	{
		return VI_E_SET_OFFSETY;
	}
	/**设置相机1ROI区域宽度**/
	nRet = MV_CC_SetIntValue(handle, "Width", WidthMax.nCurValue);
	if (MV_OK != nRet)
	{
		return VI_E_SET_WIDTHMAX;
	}
	/**设置相机1ROI区域高度**/
	nRet = MV_CC_SetIntValue(handle, "Height", HeightMax.nCurValue);
	if (MV_OK != nRet)
	{
		return VI_E_SET_HEIGHTMAX;
	}

	/*开始取流*/
	nRet = MV_CC_StartGrabbing(handle);
	if (MV_OK != nRet)
	{
		return VI_E_CAMERA_STARTGRAB;
	}

	return 0;
}
