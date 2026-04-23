#pragma once
#include <opencv2\core\core.hpp>

class TaskData
{
public:
	void* handle = NULL;		//相机句柄
	cv::Mat image;				//相机图像
	int result = 0;				//图像处理结果
	bool execute_flag = true;	//判断能否执行函数的标志位
	int errornumber = 0;		//相机1连续生产错误序号
	int productionnumber = 0;	//生产序号
	bool CurrentSaveImageFlag = false;//判断当前轮次是否存图
};