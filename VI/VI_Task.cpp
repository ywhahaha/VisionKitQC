#pragma once
#include "VI_Task.h"
#include<Windows.h>
#include <stdio.h>
#include"Get_Local_IP.h"
#include "StateCode.h"
#include <qstring.h>
extern bool DebugFlag;
clock_t counttime[3] = {0,0,0};
double num[3] = { 0,0,0 };
double sum[3] = { 0,0,0 };
double num2[3] = { 0,0,0 };
double sum2[3] = { 0,0,0 };

void VI_Task::VI()
{
	/*执行相机1任务*/
	if (taskdata1.execute_flag)
	{
		/*读D901寄存器*/
		systemstatus = m_vi_modbus.Read_PLC_Register(1);
		if (systemstatus == VI_E_PLC_UNKNOW || systemstatus == VI_E_PLC_READ)emit SysStatus(systemstatus, 1);//返回PLC读取错误状态
		
		/*执行拍照或图像处理命令*/
		taskdata1.execute_flag = false;
		if (systemstatus == VI_SIGNAL_TOPHOTO)
		{
			task1 = new std::thread([&] {TaskGrabImage(1, taskdata1.handle, taskdata1.image, taskdata1.execute_flag);});
		}
		else if (systemstatus == VI_SIGNAL_TOPROCESS)
		{
			task1 = new std::thread([&] {TaskRecog(1, taskdata1.image, taskdata1.result, taskdata1.execute_flag);});
		}
		else
		{
			taskdata1.execute_flag = true;
		}
	}
	
	/*执行相机2任务*/
	if (taskdata2.execute_flag)
	{
		/*读D902寄存器*/
		systemstatus = m_vi_modbus.Read_PLC_Register(2);
		if (systemstatus == VI_E_PLC_UNKNOW || systemstatus == VI_E_PLC_READ)emit SysStatus(systemstatus, 2);//返回PLC读取错误状态

		/*执行拍照或图像处理命令*/
		taskdata2.execute_flag = false;
		if (systemstatus == VI_SIGNAL_TOPHOTO)
		{
			task2 = new std::thread([&] {TaskGrabImage(2, taskdata2.handle, taskdata2.image, taskdata2.execute_flag);});
		}
		else if (systemstatus == VI_SIGNAL_TOPROCESS)
		{
			task2 = new std::thread([&] {TaskRecog(2, taskdata2.image, taskdata2.result, taskdata2.execute_flag);});
		}
		else
		{
			taskdata2.execute_flag = true;
		}
	}

	/*执行相机3任务*/
	if (taskdata3.execute_flag)
	{
		/*读D903寄存器*/
		systemstatus = m_vi_modbus.Read_PLC_Register(3);
		if (systemstatus == VI_E_PLC_UNKNOW || systemstatus == VI_E_PLC_READ)emit SysStatus(systemstatus, 3);//返回PLC读取错误状态

		/*执行拍照或图像处理命令*/
		taskdata3.execute_flag = false;
		if (systemstatus == VI_SIGNAL_TOPHOTO)
		{
			task3 = new std::thread([&] {TaskGrabImage(3, taskdata3.handle, taskdata3.image, taskdata3.execute_flag);});
		}
		else if (systemstatus == VI_SIGNAL_TOPROCESS)
		{
			task3 = new std::thread([&] {TaskRecog(3, taskdata3.image, taskdata3.result, taskdata3.execute_flag);});
		}
		else
		{
			taskdata3.execute_flag = true;
		}
	}

	Sleep(20);
	
	emit Restart(true);
}

/*拍照任务*/
void VI_Task::TaskGrabImage(int camera_type, void* handle, cv::Mat& img, bool& flag)
{
	if (camera_type != 1)
	{
		Sleep(20);
	}
	/*进行拍照*/
	systemstatus = m_vi_camera.grab_image(img, handle);
	if (ShowCameraInfo)	emit SysStatus(systemstatus, camera_type);
	if (systemstatus == VI_E_CAMERA_IMAGEEMPTY)
	{
		flag = true;
		return;
	}

	/*将拍摄完成信号反馈给PLC*/
	systemstatus = m_vi_modbus.Write_PLC_Register(camera_type, PhotoIsOver);
	if (systemstatus != -1)
	{
		emit SysStatus(systemstatus, camera_type);
	}
	if (systemstatus == VI_E_PLC_WRITE)
	{
		flag = true;
		return;
	}

	flag = true;
}

/*图像处理任务*/
void VI_Task::TaskRecog(int camera_type, cv::Mat img, int& result, bool& flag)
{
	/*图像处理*/
	cv::Mat Img = img.clone();
	counttime[camera_type - 1] = clock();
	m_vi_recog.VI_Image_Recog(Img, result, camera_type);
	systemstatus = result;
	if (ShowResultInfo)	emit SysStatus(systemstatus, camera_type);
	sum[camera_type - 1] += (double)(clock() - counttime[camera_type - 1]) / CLOCKS_PER_SEC;
	num[camera_type - 1]++;
	std::cout << "相机" << camera_type << "图像处理第" << num[camera_type - 1] << "次耗时" << sum[camera_type - 1]/ num[camera_type - 1] << endl;

	/*向PLC反馈图像处理结果*/
	systemstatus = m_vi_modbus.Write_PLC_Register(camera_type, result);
	if (systemstatus != -1)
	{
		emit SysStatus(systemstatus, camera_type);
	}
	if (systemstatus == VI_E_PLC_WRITE)
	{
		flag = true;
		return;
	}


	TaskOther(camera_type, Img, result);
	if (DebugFlag)
	{
		Sleep(DelayTime);	
	}

	flag = true;
}

/*其他任务，包括：发送图像至UI界面，存图，打包MYSQL信息*/
void VI_Task::TaskOther(int camera_type, cv::Mat img, const int result)
{
	//counttime[camera_type - 1] = clock();
	/*将拍照的图片发送到UI界面*/
	emit image_msg(m_vi_recog.GetInsideImage(camera_type), 1, camera_type);
	if (camera_type != 1)
	{
		emit image_msg(m_vi_recog.GetOutsideImage(camera_type), 2, camera_type);
	}

	/*将结果数据转换成string字符*/
	std::string ResultStr = DataToString(camera_type, result);

	/*生成图片文件名*/
	time_t nowtime;
	struct tm* p;
	time(&nowtime);//获取当前时间tam
	p = localtime(&nowtime);
	char FileStr[256] = {};
	char ImageType[10] = "png";//相比于jpg格式，png图片存储占用空间更多，但存图耗时少

	/*常规模式*/
	if (!DebugFlag)
	{
		/*常规模式-生成路径*/
		switch (camera_type)
		{
		case 1:
		{
			if (result == 1)
			{
				taskdata1.errornumber = 0;
				if (taskdata1.productionnumber%SaveImageFrequency == 0 && SaveImageFrequency != INF)
				{
					sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\Success\\T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, p->tm_hour, p->tm_min, p->tm_sec, taskdata1.productionnumber, ImageType);
					taskdata1.CurrentSaveImageFlag = true;
				}
				taskdata1.productionnumber++;
			}
			else
			{
				sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\False\\%s_T%02d%02d%02d_NO%05d_%02d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, ResultStr.c_str(), p->tm_hour, p->tm_min, p->tm_sec, taskdata1.productionnumber, taskdata1.errornumber++, ImageType);
				taskdata1.CurrentSaveImageFlag = true;
				emit ErrorInfo(m_vi_recog.GetErrorInfo(1).InsideErrorImage, m_vi_recog.GetErrorInfo(1).OutsideErrorImage, m_vi_recog.GetErrorInfo(1).InsideErrorData, (string)"success", 1);
			}
			break;
		}
		case 2:
		{
			if (result == 3)
			{
				if (taskdata2.productionnumber%SaveImageFrequency == 0 && SaveImageFrequency != INF)
				{
					sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\Success\\T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, p->tm_hour, p->tm_min, p->tm_sec, taskdata2.productionnumber, ImageType);
					taskdata2.CurrentSaveImageFlag = true;
				}
			}
			else
			{
				sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\False\\%s_T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, ResultStr.c_str(), p->tm_hour, p->tm_min, p->tm_sec, taskdata2.productionnumber, ImageType);
				taskdata2.CurrentSaveImageFlag = true;
				emit ErrorInfo(m_vi_recog.GetErrorInfo(2).InsideErrorImage, m_vi_recog.GetErrorInfo(2).OutsideErrorImage, m_vi_recog.GetErrorInfo(2).InsideErrorData, m_vi_recog.GetErrorInfo(2).OutsideErrorData, 2);
			}
			taskdata2.productionnumber++;
			break;
		}
		case 3:
		{
			if (result == 3)
			{
				if (taskdata3.productionnumber%SaveImageFrequency == 0 && SaveImageFrequency != INF)
				{
					sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\Success\\T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, p->tm_hour, p->tm_min, p->tm_sec, taskdata3.productionnumber, ImageType);
					taskdata3.CurrentSaveImageFlag = true;
				}
			}
			else
			{
				sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\False\\%s_T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, ResultStr.c_str(), p->tm_hour, p->tm_min, p->tm_sec, taskdata3.productionnumber, ImageType);
				taskdata3.CurrentSaveImageFlag = true;
				emit ErrorInfo(m_vi_recog.GetErrorInfo(3).InsideErrorImage, m_vi_recog.GetErrorInfo(3).OutsideErrorImage, m_vi_recog.GetErrorInfo(3).InsideErrorData, m_vi_recog.GetErrorInfo(3).OutsideErrorData, 3);
			}
			taskdata3.productionnumber++;
			break;
		}
		}

		/*常规模式-保存相机图片至本地文件夹*/
		if (!img.empty() && !DebugFlag)
		{
			if (taskdata1.CurrentSaveImageFlag && camera_type == 1)
			{
				cv::imwrite(FileStr, img);
				taskdata1.CurrentSaveImageFlag = false;
			}
			else if (taskdata2.CurrentSaveImageFlag && camera_type == 2)
			{
				cv::imwrite(FileStr, img);
				taskdata2.CurrentSaveImageFlag = false;
			}
			else if (taskdata3.CurrentSaveImageFlag && camera_type == 3)
			{
				cv::imwrite(FileStr, img);
				taskdata3.CurrentSaveImageFlag = false;
			}
		}
	}
	

	/*Debug模式*/
	if (DebugFlag)
	{
		/*Debug模式-生成路径*/
		switch (camera_type)
		{
		case 1:
		{
			if (result == 1)
			{
				taskdata1.errornumber = 0;
				if (taskdata1.productionnumber%SaveImageFrequency == 0 && SaveImageFrequency != INF)
				{
					sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\debug\\debug_camera%d_T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, p->tm_hour, p->tm_min, p->tm_sec, taskdata1.productionnumber, ImageType);
					taskdata1.CurrentSaveImageFlag = true;
				}
				taskdata1.productionnumber++;
			}
			else
			{
				sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\debug\\debug_camera%d_T%02d%02d%02d_NO%05d_%02d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, p->tm_hour, p->tm_min, p->tm_sec, taskdata1.productionnumber, taskdata1.errornumber++, ImageType);
				taskdata1.CurrentSaveImageFlag = true;
			}
			emit ErrorInfo(m_vi_recog.GetErrorInfo(1).InsideErrorImage, m_vi_recog.GetErrorInfo(1).OutsideErrorImage, m_vi_recog.GetErrorInfo(1).InsideErrorData, (string)"", 1);
			break;
		}
		case 2:
		{
			if (result == 3)
			{
				if (taskdata2.productionnumber%SaveImageFrequency == 0 && SaveImageFrequency != INF)
				{
					sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\debug\\debug_camera%d_T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, p->tm_hour, p->tm_min, p->tm_sec, taskdata2.productionnumber, ImageType);
					taskdata2.CurrentSaveImageFlag = true;
				}
			}
			else
			{
				sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\debug\\debug_camera%d_T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, p->tm_hour, p->tm_min, p->tm_sec, taskdata2.productionnumber, ImageType);
				taskdata2.CurrentSaveImageFlag = true;
			}
			taskdata2.productionnumber++;
			break;
		}
		case 3:
		{
			if (result == 3)
			{
				if (taskdata3.productionnumber%SaveImageFrequency == 0 && SaveImageFrequency != INF)
				{
					sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\debug\\debug_camera%d_T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, p->tm_hour, p->tm_min, p->tm_sec, taskdata3.productionnumber, ImageType);
					taskdata3.CurrentSaveImageFlag = true;
				}
			}
			else
			{
				sprintf_s(FileStr, 256, "D:\\VI\\figs\\%04d%02d%02d\\debug\\debug_camera%d_T%02d%02d%02d_NO%05d.%s", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, camera_type, p->tm_hour, p->tm_min, p->tm_sec, taskdata3.productionnumber, ImageType);
				taskdata3.CurrentSaveImageFlag = true;
			}
			taskdata3.productionnumber++;
			break;
		}
		}

		/*Debug模式-存图*/
		if (!img.empty() && SaveImageFlag)
		{
			if (taskdata1.CurrentSaveImageFlag && camera_type == 1)
			{
				cv::imwrite(FileStr, img);
				taskdata1.CurrentSaveImageFlag = false;
			}
			else if (taskdata2.CurrentSaveImageFlag && camera_type == 2)
			{
				cv::imwrite(FileStr, img);
				taskdata2.CurrentSaveImageFlag = false;
			}
			else if (taskdata3.CurrentSaveImageFlag && camera_type == 3)
			{
				cv::imwrite(FileStr, img);
				taskdata3.CurrentSaveImageFlag = false;
			}
		}
	}



	if (SaveDataFlag)
	{
		/*插入'\'*/
		string PathStrReplace = FileStr;
		for (string::size_type pos(0); pos != string::npos; pos += 3) {
			if ((pos = PathStrReplace.find("\\", pos)) != string::npos) {
				PathStrReplace.insert(pos, "\\");
			}
			else break;
		}

		/*打包MYSQL信息发送给主线程*/
		char MYSQLchar[512] = {};
		sprintf_s(MYSQLchar, 512, "insert into `historydata` values('%04d%02d%02d%02d%02d%02d-%d','%d','%s','%s');", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, camera_type, camera_type, ResultStr.c_str(), PathStrReplace.c_str());
		string MYSQLStr = MYSQLchar;

		emit MYSQLData(MYSQLStr);
	}

	//sum2[camera_type - 1] += (double)(clock() - counttime[camera_type - 1]) / CLOCKS_PER_SEC;
	//num2[camera_type - 1]++;
	//std::cout << "相机" << camera_type << "其他处理第" << num2[camera_type - 1] << "次耗时" << sum2[camera_type - 1] / num2[camera_type - 1] << endl;
	//

	return;
}


/*建立PLC和相机连接，以及初始化程序*/
void VI_Task::Init_VI()
{
	GetSettingConfig();
	
	taskdata1.execute_flag = true;
	taskdata2.execute_flag = true;
	taskdata3.execute_flag = true;
	/*建立PLC通讯连接，失败返回错误码*/
	systemstatus = m_vi_modbus.build_comm_connect();
	emit SysStatus(systemstatus, ReconnectedTimes);
	if (systemstatus != VI_PLC_ON)
	{
		ReconnectedTimes++;
		Sleep(5000);

		if(StopReconnect || ReconnectedTimes > AllowReconnectTimes)
		{
			emit SysStatus(systemstatus, -1);
			emit FinishInit(false);
		}
		else if (ReconnectedTimes <= AllowReconnectTimes)
		{
			emit Reconnect_VI_signals(true);
		}
		return;
	}

	std::thread* Openth1;
	std::thread* Openth2;
	
	Openth1 = new std::thread([&] 
	{
		if (OpenCamera(1, taskdata1.handle, taskdata1.image) == 0)return;
	});
	Openth2 = new std::thread([&]
	{
		if (OpenCamera(2, taskdata2.handle, taskdata2.image) == 0)return;
	});
	if (OpenCamera(3, taskdata3.handle, taskdata3.image) == 0)return;

	(*Openth1).join();
	(*Openth2).join();

	/*发送初始化完成信号*/
	emit FinishInit(true);
}

int VI_Task::OpenCamera(int camera_type, void*& handle, cv::Mat& img)
{
	/*建立相机通讯连接，失败返回错误码*/
	systemstatus = m_vi_camera.build_camera_connect(getEthernetIp(), camera_type, &handle);
	emit SysStatus(systemstatus, camera_type);
	if (systemstatus != VI_CAMERA_READY)
	{
		emit FinishInit(false);
		return 0;
	}

	m_vi_recog.GetROI(camera_type);

	/*拍照，此处拍照是因为不知道PLC的初始命令是什么，若是跳过拍照环节直接进行图像处理
	，会导致图像为空，从而出错*/
	systemstatus = m_vi_camera.grab_image(img, handle);
	if(ShowCameraInfo)	emit SysStatus(systemstatus, camera_type);
	if (systemstatus == VI_E_CAMERA_IMAGEEMPTY)
	{
		emit FinishInit(false);
		return 0;
	}

	/*将拍照的图片发送到UI界面*/
	cv::Mat InsideImage, OutsideImage;
	emit image_msg(img, 0, camera_type);
	m_vi_recog.ShowInitImage(img, InsideImage, OutsideImage, camera_type);
	emit image_msg(InsideImage, 1, camera_type);
	if (camera_type != 1)
	{
		emit image_msg(OutsideImage, 2, camera_type);
	}

	return 1;
}


/*安全断开视觉系统的通讯连接，否则需要等待相机后自动重启*/
void VI_Task::Close_VI()
{
	Sleep(2000);//确保线程已全部结束

	taskdata1.execute_flag = true;
	taskdata2.execute_flag = true;
	taskdata3.execute_flag = true;
	ReconnectedTimes = 0;

	/*若PLC建立失败，则相机未建立连接，不用尝试关闭相机直接退出即可*/
	if (taskdata1.handle == NULL && taskdata2.handle == NULL && taskdata3.handle == NULL)
	{
		emit FinishClose();
		return;
	}

	std::thread* Closeth1;
	std::thread* Closeth2;

	Closeth1 = new std::thread([&]
	{
		CloseCamera(1, taskdata1.handle, taskdata1.image);
	});
	Closeth2 = new std::thread([&]
	{
		CloseCamera(2, taskdata2.handle, taskdata2.image);
		
	});
	CloseCamera(3, taskdata3.handle, taskdata3.image);

	(*Closeth1).join();
	(*Closeth2).join();
	/*关闭PLC连接*/
	systemstatus = m_vi_modbus.close_comm_connect();
	emit SysStatus(systemstatus, 1);

	/*发送已断开通讯连接的信号*/
	emit FinishClose();
}

void VI_Task::CloseCamera(int camera_type, void* handle, cv::Mat img)
{
	if (camera_type == 1)Sleep(200);//依次拍照，防止网络传输过载，图片产生黑纹
	if (camera_type == 2)Sleep(400);

	m_vi_camera.ResetROI(handle);
	m_vi_camera.grab_image(img, handle);
	emit image_msg(img, 0, camera_type);

	//Sleep(1000);
	/*关闭相机连接*/
	systemstatus = m_vi_camera.close_camera_connect(handle);
	emit SysStatus(systemstatus, camera_type);
}

void VI_Task::IsCameraConnect()
{
	emit CameraConnectStatus(m_vi_camera.CheckCameraConnection(taskdata1.handle, taskdata2.handle, taskdata3.handle));
}

/*把数据结果转义成图片标题文字*/
std::string VI_Task::DataToString(int camer_type, const int result)
{
	switch (camer_type)
	{
	case 1:
	{
		switch (result)
		{
		case 0:
		{
			return "下";
			break;
		}
		case 1:
		{
			return "正常";
			break;
		}
		case 10:
		{
			return "试纸条反了";
			break;
		}
		case 12:
		{
			return "纸槽未清空";
			break;
		}
		}
		break;
	}
	case 2:
	{
		switch (result)
		{
		case 0:
		{
			return "左右";
			break;
		}
		case 1:
		{
			return "右";
			break;
		}
		case 2:
		{
			return "左";
			break;
		}
		default:
		{
			return "正常";
			break;
		}
		}
		break;
	}
	case 3:
	{
		switch (result)
		{
		case 0:
		{
			return "左右";
			break;
		}
		case 1:
		{
			return "左";
			break;
		}
		case 2:
		{
			return "右";
			break;
		}
		default:
		{
			return "正常";
			break;
		}
		}
		break;
	}
	default:
	{
		break;
	}
	}

	return "未知错误";
}

void VI_Task::GetSaveImageFrequency(string str, int& Frequency)
{
	if (str == "save_all_image")
	{
		Frequency = 1;
	}
	else if (str == "every_5")
	{
		Frequency = 5;
	}
	else if (str == "every_10")
	{
		Frequency = 10;
	}
	else if (str == "every_20")
	{
		Frequency = 20;
	}
	else if (str == "every_50")
	{
		Frequency = 50;
	}
	else if (str == "every_100")
	{
		Frequency = 100;
	}
	else if (str == "every_200")
	{
		Frequency = 200;
	}
	else if (str == "never_save_image")
	{
		Frequency = INF;
	}
}

void VI_Task::GetSettingConfig()
{
	SaveImageFlag = true;
	SaveDataFlag = true;

	ini->ReadINI("config.ini");//读.ini配置文件

	GetSaveImageFrequency(ini->GetValue("Other", "SaveFrequency"), SaveImageFrequency);

	if (ini->GetValue("Other", "CameraInfo") == "true")
	{
		ShowCameraInfo = true;
	}
	else
	{
		ShowCameraInfo = false;
	}

	if (ini->GetValue("Other", "ResultInfo") == "true")
	{
		ShowResultInfo = true;
	}
	else
	{
		ShowResultInfo = false;
	}

	if (DebugFlag)
	{
		DelayTime = stoi(ini->GetValue("DeveloperOptions", "DelayTime"));

		if (ini->GetValue("DeveloperOptions", "SaveImage") == "true")
		{
			SaveImageFlag = true;
		}
		else
		{
			SaveImageFlag = false;
		}

		if (ini->GetValue("DeveloperOptions", "SaveData") == "true")
		{
			SaveDataFlag = true;
		}
		else
		{
			SaveDataFlag = false;
		}
	}

	ini->Clear();
}

