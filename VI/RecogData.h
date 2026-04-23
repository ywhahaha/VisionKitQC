#pragma once
#include <opencv2\core\core.hpp>

class RecogData
{
public:
	cv::Mat cutImage_A;//切割图，A是转盘内侧工装图像，B是转盘外侧工装图像,C是剪纸模块两个试纸条之间的空间
	cv::Mat cutImage_B;

	cv::Mat InsideImage;//用于展示在UI界面//内侧图片
	cv::Mat OutsideImage;//用于展示在UI界面//外侧图片

	cv::Mat ErrorImage_A;//错误位置图片
	string ErrorData_A;//错误位置信息
	cv::Mat ErrorImage_B;//错误位置图片
	string ErrorData_B;//错误位置信息
};