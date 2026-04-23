#pragma once
#include "qobject.h"
#include"VI_Modbus.h"
#include"VI_Camera.h"
#include"VI_Recog.h"
#include"TaskData.h"
#include <string.h>
#include "INI_Config.h"
#include <qimage>

class VI_Task :
	public QObject
{
	Q_OBJECT
		
public:
	int PLCPacketLoss[3] = { 0,0,0 };//PLC丢包数
	int GetPLCPacketNum[3] = { 0,0,0 };//PLC数据包数
	bool StopReconnect = false;//用户登录可暂停重连

private:
	VI_Modbus m_vi_modbus;
	VI_Camera m_vi_camera;
	VI_Recog m_vi_recog;
	TaskData taskdata1;
	TaskData taskdata2;
	TaskData taskdata3;
	std::thread* task1;
	std::thread* task2;
	std::thread* task3;

	bool change_flag = true;//从寄存器读到的值发生变化时，将标志位置位
	bool execute_flag = true;//判断能否执行函数的标志位
	int ReconnectFlag = 0;//PLC重连次数
	int systemstatus = 0;//系统状态
	int ReconnectedTimes = 0;//已自动重连次数
	int AllowReconnectTimes = 30;//可自动重连次数
	bool CurrentSaveImageFlag = false;//判断当前轮次是否存图
	clock_t start_time = 0;

	int OpenCamera(int camera_type, void*& handle, cv::Mat& img);
	void CloseCamera(int camera_type, void* handle, cv::Mat img);
	void TaskGrabImage(int camera_type, void* handle, cv::Mat& img, bool& flag);//拍照任务
	void TaskRecog(int camera_type, cv::Mat img, int& result, bool& flag);//图像处理任务
	void TaskOther(int camera_type, cv::Mat img, const int result);//其他任务，包括：发送图像至UI界面，存图，打包MYSQL信息
	std::string DataToString(int camer_type, const int result);
	void GetSaveImageFrequency(string str, int& Frequency);
	void GetSettingConfig();

	/*设置项参数*/
	int DelayTime = 0;
	bool SaveImageFlag = true;
	bool SaveDataFlag = true;
	int SaveImageFrequency = 1;
	bool ShowCameraInfo = true;
	bool ShowResultInfo = true;

	/*读取配置文件*/
	INI_Config *ini = new INI_Config();

	const int INF = 0x3fffffff;

signals:
	void image_msg(cv::Mat image_msg,int pos, int order);
	void Restart(bool status);
	void SysStatus(int firststatus, int secondstatus);
	void FinishInit(bool status);
	void FinishClose();
	void CameraConnectStatus(int status);
	void MYSQLData(string data);
	void Reconnect_VI_signals(bool status);
	void ErrorInfo(cv::Mat InsideErrorImage, cv::Mat OutsideErrorImage, string InsideErrorData, string OutsideErrorData, int camera_type);

public slots:
	void VI();
	void Init_VI();
	void Close_VI();
	void IsCameraConnect();

};

