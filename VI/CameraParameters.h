#pragma once
//#include <string.h>

/*相机参数*/
class CameraParameters
{
public:
	std::string ip;
	bool SetGevHeartbeat;
	int GevHeartbeatTimeout;
	bool GetGevPacketSizeAuto;
	int GevPacketSize;
	int GevSCPD;
	float AcqusitionFrameRate;
	float ExposureTime;
	float Gain;
	bool BlackLevelEnable;
	int BlackLevel;
	int decimation;//下采样比例
	enum Decimation_Camera
	{
		Decimation1 = 1,//下采样比例
		Decimation2 = 2,
		Decimation4 = 4
	}DecimationCamera;
	int OffsetX;
	int OffsetY;
	int ImageWidth;
	int ImageHeight;

};