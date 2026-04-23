#pragma once
#include "VI_Recog.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\types_c.h>
#include <time.h>
#include <opencv2/core/utils/logger.hpp>
//#include "MvCameraControl.h"
#include <stdio.h>
#include <conio.h>
#include<functional>
#include "StateCode.h"
extern bool DebugFlag;

VI_Recog vi_recog;

/*根据相机序号来进行相应的图像识别*/
void VI_Recog::VI_Image_Recog(cv::Mat image, int& result, int camera_type)
{
	;
	switch (camera_type)
	{
	case 1:
	{
		paper_detect(image, result);
		if (DebugFlag && Camera1Result != -1)
		{
			result = Camera1Result;
		}
		break;
	}
	case 2:
	{
		bottom_detect(image, result);
		if (DebugFlag&& Camera2Result != -1)
		{
			result = Camera2Result;
		}
		break;
	}
	case 3:
	{
		lid_detect(image, result);
		if (DebugFlag&& Camera3Result != -1)
		{
			result = Camera3Result;
		}
		break;
	}
	default:
	{
		break;
	}
	}
}


/*读取图像Roi子区域*/
void VI_Recog::GetROI(int camera_type)
{
	INI_Config *ini = new INI_Config;

	ini->ReadINI("config.ini");//读.ini配置文件

	for (int i = 0; i < 2; i++)
	{
		subroi[camera_type - 1][i].x = stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_OffsetX_" + to_string(i))) - stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_OffsetX_Union"));
		subroi[camera_type - 1][i].y = stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_OffsetY_" + to_string(i))) - stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_OffsetY_Union"));
		subroi[camera_type - 1][i].width = stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_Width_" + to_string(i)));
		subroi[camera_type - 1][i].height = stoi(ini->GetValue("Camera", "Camera" + to_string(camera_type) + "_Roi_Height_" + to_string(i)));
	}	
		
	PaperWidthUpThreshold = stod(ini->GetValue("Flaw", "PaperWidthUpperLimit"));//试纸条宽度误差，单位mm
	PaperWidthDownThreshold = stod(ini->GetValue("Flaw", "PaperWidthLowerLimit"));//试纸条宽度误差，单位mm
	PaperPositiveAngleThreshold = stod(ini->GetValue("Flaw", "PaperAngleUpperLimit"));//试纸条与x轴正方向的夹角为正
	PaperNegativeAngleThreshold = stod(ini->GetValue("Flaw", "PaperAngleLowerLimit"));//试纸条与x轴正方向的夹角为负
	PaperStainLengthThreshold = stod(ini->GetValue("Flaw", "PaperStainLength"));//试纸条脏污长度，单位mm
	PaperStainNumberThreshold = stod(ini->GetValue("Flaw", "PaperStainNumber"));//试纸条脏污数量
	PaperStainColorDegreeThreshold = stod(ini->GetValue("Flaw", "PaperStainColorDegree"));//试纸条脏污明显程度，值越小表明所能容忍的脏污程度越大，单位灰度值，范围（0，255）
	PaperGoldenMarkLengthThreshold = stod(ini->GetValue("Flaw", "PaperGoldenMarkLength"));//金标线宽，单位mm

	BoxPaperAngleLeftThreshold = stod(ini->GetValue("Flaw", "BoxPaperAngleLeftUpperLimit"));//试纸盒内纸条左倾角度上限(°)
	BoxPaperAngleRightThreshold = stod(ini->GetValue("Flaw", "BoxPaperAngleRightUpperLimit"));//试纸盒内纸条右倾角度上限(°)
	BoxPaperPositionYUpThreshold = stod(ini->GetValue("Flaw", "BoxPaperYPositionUpperLimit"));//试纸盒内纸条垂直方向的位置上限
	BoxPaperPositionYDownThreshold = stod(ini->GetValue("Flaw", "BoxPaperYPositionLowerLimit"));//试纸盒内纸条垂直方向的位置下限

	GapAeraThreshold = stod(ini->GetValue("Flaw", "BoxGapAera"));//缝隙面积，单位mm^2

	if (DebugFlag)
	{ 
		auto cameraresult = ini->GetValue("DeveloperOptions", "Camera1Result");
		if (cameraresult == "-")
		{
			Camera1Result = -1;
		}
		else if(cameraresult == "0")
		{
			Camera1Result = 0;
		}
		else if (cameraresult == "1")
		{
			Camera1Result = 1;
		}
		else if (cameraresult == "10")
		{
			Camera1Result = 10;
		}
		else if (cameraresult == "12")
		{
			Camera1Result = 12;
		}

		cameraresult = ini->GetValue("DeveloperOptions", "Camera2Result");
		if (cameraresult == "-")
		{
			Camera2Result = -1;
		}
		else if (cameraresult == "0")
		{
			Camera2Result = 0;
		}
		else if (cameraresult == "1")
		{
			Camera2Result = 1;
		}
		else if (cameraresult == "2")
		{
			Camera2Result = 2;
		}
		else if (cameraresult == "3")
		{
			Camera2Result = 3;
		}

		cameraresult = ini->GetValue("DeveloperOptions", "Camera3Result");
		if (cameraresult == "-")
		{
			Camera3Result = -1;
		}
		else if (cameraresult == "0")
		{
			Camera3Result = 0;
		}
		else if (cameraresult == "1")
		{
			Camera3Result = 1;
		}
		else if (cameraresult == "2")
		{
			Camera3Result = 2;
		}
		else if (cameraresult == "3")
		{
			Camera3Result = 3;
		}
	}

	ini->Clear();
}

cv::Mat VI_Recog::GetInsideImage(int camera_type)
{
	switch (camera_type)
	{
	case 1:
	{
		recogdata1.InsideImage = recogdata1.cutImage_A.clone();
		return recogdata1.InsideImage;
	}
	case 2:
	{
		transpose(recogdata2.cutImage_A, recogdata2.InsideImage);
		flip(recogdata2.InsideImage, recogdata2.InsideImage, 0);
		return recogdata2.InsideImage;
	}
	case 3:
	{
		transpose(recogdata3.cutImage_A, recogdata3.InsideImage);
		flip(recogdata3.InsideImage, recogdata3.InsideImage, 1);
		return recogdata3.InsideImage;
	}
	default:
	{
		break;
	}
	}
	cv::Mat EmptyImage;
	return EmptyImage;
}

cv::Mat VI_Recog::GetOutsideImage(int camera_type)
{
	switch (camera_type)
	{
	case 2:
	{
		transpose(recogdata2.cutImage_B, recogdata2.OutsideImage);
		flip(recogdata2.OutsideImage, recogdata2.OutsideImage, 0);
		return recogdata2.OutsideImage;
	}
	case 3:
	{
		transpose(recogdata3.cutImage_B, recogdata3.OutsideImage);
		flip(recogdata3.OutsideImage, recogdata3.OutsideImage, 1);
		return recogdata3.OutsideImage;
	}
	default:
	{
		break;
	}
	}
	cv::Mat EmptyImage;
	return EmptyImage;
}

int VI_Recog::GetCutImage(cv::Mat OriImage, cv::Mat& cutImage_A, cv::Mat& cutImage_B, int camera_type, int& result)
{
	cv::Rect rect;//用于存放截取的坐标点，并用来判断rect函数是否为空
	cv::Mat Img = OriImage.clone();
	if (Img.empty())
	{
		result = 0;
		return 0;
	}

	cv::Rect ImageRect;

	ImageRect.x = subroi[camera_type - 1][1].x, ImageRect.y = subroi[camera_type - 1][1].y, ImageRect.width = subroi[camera_type - 1][1].width, ImageRect.height = subroi[camera_type - 1][1].height;
	ImageRect &= cv::Rect(0, 0, Img.cols, Img.rows);
	cutImage_A = Img(ImageRect).clone();

	ImageRect.x = subroi[camera_type - 1][0].x, ImageRect.y = subroi[camera_type - 1][0].y, ImageRect.width = subroi[camera_type - 1][0].width, ImageRect.height = subroi[camera_type - 1][0].height;
	ImageRect &= cv::Rect(0, 0, Img.cols, Img.rows);
	cutImage_B = Img(ImageRect).clone();

	return 1;
}

RecogErrorImageItem VI_Recog::GetErrorInfo(int camera_type)
{
	RecogErrorImageItem item;
	
	if (camera_type == 1)
	{
		item.InsideErrorImage = recogdata1.ErrorImage_A;
		item.InsideErrorData = recogdata1.ErrorData_A;
	}
	else if (camera_type == 2)
	{
		item.InsideErrorImage = recogdata2.ErrorImage_A;
		item.InsideErrorData = recogdata2.ErrorData_A;
		item.OutsideErrorImage = recogdata2.ErrorImage_B;
		item.OutsideErrorData = recogdata2.ErrorData_B;
	}
	else if (camera_type == 3)
	{
		item.InsideErrorImage = recogdata3.ErrorImage_A;
		item.InsideErrorData = recogdata3.ErrorData_A;
		item.OutsideErrorImage = recogdata3.ErrorImage_B;
		item.OutsideErrorData = recogdata3.ErrorData_B;
	}

	return item;
}


void VI_Recog::ShowInitImage(cv::Mat OriImage, cv::Mat& InsideImage, cv::Mat& OutsideImage, int camera_type)
{
	cv::Rect rect;//用于存放截取的坐标点，并用来判断rect函数是否为空
	cv::Mat cutImage_A, cutImage_B;
	if (OriImage.empty())
	{
		return;
	}

	cv::Rect ImageRect;

	ImageRect.x = subroi[camera_type - 1][1].x, ImageRect.y = subroi[camera_type - 1][1].y, ImageRect.width = subroi[camera_type - 1][1].width, ImageRect.height = subroi[camera_type - 1][1].height;
	ImageRect &= cv::Rect(0, 0, OriImage.cols, OriImage.rows);
	cutImage_A = OriImage(ImageRect);

	ImageRect.x = subroi[camera_type - 1][0].x, ImageRect.y = subroi[camera_type - 1][0].y, ImageRect.width = subroi[camera_type - 1][0].width, ImageRect.height = subroi[camera_type - 1][0].height;
	ImageRect &= cv::Rect(0, 0, OriImage.cols, OriImage.rows);
	cutImage_B = OriImage(ImageRect);

	if (camera_type == 1)
	{
		InsideImage = cutImage_A.clone();
	}
	else if (camera_type == 2)
	{

		transpose(cutImage_B, InsideImage);
		transpose(cutImage_A, OutsideImage);
		flip(InsideImage, InsideImage, 1);
		flip(OutsideImage, OutsideImage, 0);
	}
	else if (camera_type == 3)
	{
		transpose(cutImage_A, InsideImage);
		transpose(cutImage_B, OutsideImage);
		flip(InsideImage, InsideImage, 0);
		flip(OutsideImage, OutsideImage, 1);
	}
}


/*剪纸检测*/
void VI_Recog::paper_detect(cv::Mat OriImage, int& result)
{
	if(GetCutImage(OriImage, recogdata1.cutImage_A, recogdata1.cutImage_B, 1, result) == 0)return;

	int paper_detect_Result_A = 0;//检测结果0或1
	int paper_detect_Result_B = 0;

	paper_detect_cutImage(recogdata1.cutImage_A, "A", paper_detect_Result_A);
	if (paper_detect_Result_A == 0)
	{
		PaperSlotDetect(recogdata1.cutImage_B, "纸槽", paper_detect_Result_B);
	}

	result = paper_detect_Result_A * (1 - paper_detect_Result_B) + paper_detect_Result_B * 12;//根据IPC与PLC的交互协议一共有00，01，10，11四种传输数据（uint16_t），十位数字表示工装A的结果，个位数字表示工装B的结果。

}

/*纸槽纸条检测*/
void VI_Recog::PaperSlotDetect(cv::Mat cutImage, cv::String WindowName, int& detect_result)
{
	vector<vector<cv::Point> > contours;//轮廓
	vector<cv::Vec4i> hierarchy;//向量
	int MaxArea = 0;

	cv::Mat binImage;
	threshold(cutImage, binImage, 100, 255, CV_THRESH_BINARY);
	morphologyEx(binImage, binImage, cv::MORPH_OPEN, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

	if (countNonZero(binImage) > 40000)
	{
		detect_result = 1;
		findContours(binImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		cvtColor(cutImage, cutImage, CV_GRAY2BGR);
		drawContours(cutImage, contours, -1, (0, 0, 255), 2);
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "纸槽有纸";
	}
	else
	{
		detect_result = 0;
	}
}

/*压片检测*/
void VI_Recog::bottom_detect(cv::Mat OriImage, int& result)
{
	if(GetCutImage(OriImage, recogdata2.cutImage_B, recogdata2.cutImage_A, 2, result) == 0)return;;

	cv::Mat rotMat = cv::getRotationMatrix2D(cv::Point(recogdata2.cutImage_A.cols / 2, recogdata2.cutImage_A.rows / 2), 180, 1);
	warpAffine(recogdata2.cutImage_A, recogdata2.cutImage_A, rotMat, recogdata2.cutImage_A.size());

	int bottom_detect_Result_A = 0;//检测结果0或1
	int bottom_detect_Result_B = 0;

	thread th(std::bind(&VI_Recog::bottom_detect_cutImage, this, recogdata2.cutImage_A, "A", ref(bottom_detect_Result_A)));
	bottom_detect_cutImage(recogdata2.cutImage_B, "B", bottom_detect_Result_B);
	th.join();

	result = bottom_detect_Result_A + bottom_detect_Result_B * 2;//根据IPC与PLC的交互协议一共有00，01，10，11四种传输数据（uint16_t），十位数字表示工装A的结果，个位数字表示工装B的结果。

}

/*合盖检测*/
void VI_Recog::lid_detect(cv::Mat OriImage, int& result)
{
	if(GetCutImage(OriImage, recogdata3.cutImage_A, recogdata3.cutImage_B, 3, result) == 0)return;;

	cv::Mat rotMat = cv::getRotationMatrix2D(cv::Point(recogdata3.cutImage_A.cols / 2, recogdata3.cutImage_A.rows / 2), 180, 1);
	warpAffine(recogdata3.cutImage_A, recogdata3.cutImage_A, rotMat, recogdata3.cutImage_A.size());

	int lid_detect_Result_A = 0;//检测结果0或1
	int lid_detect_Result_B = 0;

	thread th(std::bind(&VI_Recog::lid_detect_cutImage, this, recogdata3.cutImage_A, "A", ref(lid_detect_Result_A)));
	lid_detect_cutImage(recogdata3.cutImage_B, "B", lid_detect_Result_B);
	th.join();


	result = lid_detect_Result_A + lid_detect_Result_B * 2;//根据IPC与PLC的交互协议一共有00，01，10，11四种传输数据（uint16_t），十位数字表示工装A的结果，个位数字表示工装B的结果。
}


void VI_Recog::paper_detect_cutImage(cv::Mat cutImage, cv::String WindowName, int& detect_result)
{
	cv::Mat binImage;//二值图
	cv::Mat contourImage;//用于存放canny算子检测结果的轮廓图像
	cv::Mat morphImage;//存放“形态学操作”处理的图像，在寻找轮廓之前进行开操作可以减少轮廓不闭合的概率。
	cv::Mat middlepaperImage;//试纸条中间段图像
	cv::Mat floodFillImage;//水漫填充图像
	cv::Mat WatchImage;

	cv::Rect floodFill_rect;//水漫填充的最小矩形边界

	vector<vector<cv::Point> > contours;//轮廓
	vector<cv::Vec4i> hierarchy;//向量
	vector<cv::Vec2f> lines;//线段

	int MaxArea = 0;

	morphologyEx(cutImage, morphImage, cv::MORPH_OPEN, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
	threshold(morphImage, binImage, 100, 255, CV_THRESH_BINARY);

	findContours(binImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	GetContoursSize(contours, INF, 500);
	if (contours.size() > 1)contours[0].insert(contours[0].end(), contours[1].begin(), contours[1].end());

	if (GetMaxArea(contours, MaxArea = 0, INF, 3000) == 0)
	{
		cvtColor(cutImage, cutImage, CV_GRAY2BGR);
		drawContours(cutImage, contours, -1, (0, 0, 255), 2);
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "空试纸条";
		detect_result = 0;
		return;
	}

	if (MaxArea > 115000)
	{
		cvtColor(cutImage, cutImage, CV_GRAY2BGR);
		drawContours(cutImage, contours, -1, (0, 0, 255), 2);
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "纸槽有纸";
		detect_result = 0;
		return;
	}

	EdgeFilter(10, 5, contours);
	cv::RotatedRect PaperRect = minAreaRect(contours[0]);
	/*试纸条歪斜*/

	double PaperAngle = PaperRect.angle;
	if (abs(PaperAngle) > PaperNegativeAngleThreshold && abs(PaperAngle) < 90 - PaperPositiveAngleThreshold)
	{
		cvtColor(cutImage, cutImage, CV_GRAY2BGR);
		drawContours(cutImage, contours, -1, (0, 0, 255), 2);
		recogdata1.ErrorImage_A = cutImage.clone();
		if (PaperAngle > 45)
		{
			recogdata1.ErrorData_A = "试纸条倾斜，倾斜角度为：" + to_string(90 - PaperAngle);
		}
		else
		{
			recogdata1.ErrorData_A = "试纸条倾斜，倾斜角度为：" + to_string(PaperAngle);
		}
		detect_result = 0;
		return;
	}

	double PaperWidth = (min(PaperRect.size.width, PaperRect.size.height) - 5) * 45 / 1029;//减0.2是进行补偿
	if (PaperWidth > PaperWidthUpThreshold)
	{
		cvtColor(cutImage, cutImage, CV_GRAY2BGR);
		drawContours(cutImage, contours, -1, (0, 0, 255), 2);
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "试纸条宽度超过阈值，试纸条宽度为：" + to_string(PaperWidth);
		detect_result = 0;
		return;
	}
	else if (PaperWidth < PaperWidthDownThreshold)
	{
		cvtColor(cutImage, cutImage, CV_GRAY2BGR);
		drawContours(cutImage, contours, -1, (0, 0, 255), 2);
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "试纸条宽度小于阈值，试纸条宽度为：" + to_string(PaperWidth);
		detect_result = 0;
		return;
	}

	floodFillImage = morphImage.clone();
	cv::blur(floodFillImage, floodFillImage, cv::Size(5, 5));
	int floodFill_Diff = 1;
	int floodFill_color = 255;
	cv::floodFill(floodFillImage, cv::Point(PaperRect.center.x, PaperRect.center.y), cv::Scalar(floodFill_color, 0, 0), &floodFill_rect, cv::Scalar(floodFill_Diff, 0, 0), cv::Scalar(floodFill_Diff, 0, 0));
	if (PaperRect.center.x - 40 > 0)
	{
		cv::floodFill(floodFillImage, cv::Point(PaperRect.center.x - 40, PaperRect.center.y), cv::Scalar(floodFill_color, 0, 0), &floodFill_rect, cv::Scalar(floodFill_Diff, 0, 0), cv::Scalar(floodFill_Diff, 0, 0));
	}
	threshold(floodFillImage, binImage, 254, 255, CV_THRESH_BINARY/* | CV_THRESH_OTSU*/);
	int floodFillImageArea = countNonZero(binImage);

	if (floodFillImageArea > 40000)
	{
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "试纸条上下反了";

		PaperUpsidedownWarningCounter++;
		if (PaperUpsidedownWarningCounter >= PaperUpsidedownWarningTimes)
		{
			detect_result = 10;
		}
		else
		{
			detect_result = 0;
		}
		return;
	}
	else
	{
		PaperUpsidedownWarningCounter = 0;
	}



	/*重新进行阈值分割，识别海绵条缺失情况*/
	threshold(morphImage, binImage, 240, 255, CV_THRESH_BINARY/* | CV_THRESH_OTSU*/);

	int SpongeBarArea = countNonZero(binImage);

	if (SpongeBarArea > 4000)
	{
		findContours(binImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		GetMaxArea(contours, MaxArea = 0, INF, 0);
		cvtColor(cutImage, cutImage, CV_GRAY2BGR);
		drawContours(cutImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "试纸条海绵条缺失";
		detect_result = 0;
		return;
	}

	/*重新进行阈值分割，将试纸条左侧和中间部分分离*/
	threshold(morphImage, binImage, 210, 255, CV_THRESH_BINARY/* | CV_THRESH_OTSU*/);
	int PaperArea = countNonZero(binImage);

	/*获取试纸条左侧部分*/
	findContours(binImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	if (GetMaxArea(contours, MaxArea = 0, 30000, 500) == 0)
	{
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "试纸条水平倾斜";
		detect_result = 0;
		return;
	}

	cv::RotatedRect LeftRect = minAreaRect(contours[0]);
	/*试纸条左右正反*/
	double LeftRect_center_x = LeftRect.center.x / cutImage.cols;
	if (LeftRect_center_x > 0.5 * cutImage.cols)
	{
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "试纸条左右反了";

		PaperUpsidedownWarningCounter++;
		if (PaperUpsidedownWarningCounter >= PaperUpsidedownWarningTimes)
		{
			detect_result = 10;
		}
		else
		{
			detect_result = 0;
		}
		return;
	}
	else
	{
		PaperUpsidedownWarningCounter = 0;
	}

	/*获取试纸条中间部分*/
	findContours(binImage, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	if (GetMaxArea(contours, MaxArea = 0, INF, 2000) == 0)
	{
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "未知错误，错误参数为：" + to_string(MaxArea);
		detect_result = 0;
		return;
	}

	cv::RotatedRect MiddleRect = minAreaRect(contours[0]);

	/*金标线宽*/
	double goldenmarklength = abs(29 - max(MiddleRect.size.height, MiddleRect.size.width) * 45 / 1029);
	if (goldenmarklength < PaperGoldenMarkLengthThreshold)
	{
		drawGoldMark(MiddleRect, goldenmarklength, cutImage);
		recogdata1.ErrorData_A = "金标线宽低于阈值，金标线宽为：" + to_string(PaperAngle);
		detect_result = 0;
		return;
	}

	/*裁剪蛋白条图像*/
	cv::Mat MiddlePaperImage;
	if (ReturnCutImage(cutImage, MiddlePaperImage, MiddleRect, 15, 2, 137 - goldenmarklength * 1029 / 45, 2) == 0)
	{
		cvtColor(cutImage, cutImage, CV_GRAY2BGR);
		recogdata1.ErrorImage_A = cutImage.clone();
		recogdata1.ErrorData_A = "未知错误，错误参数为：" + to_string(MaxArea);
		detect_result = 0;
		return;
	}

	threshold(MiddlePaperImage, binImage, PaperStainColorDegreeThreshold, 255, CV_THRESH_BINARY_INV);
	findContours(binImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/*蛋白条脏污*/
	int StainNumber = GetContoursSize(contours, INF, (int)PaperStainLengthThreshold * 1029 / 45, MiddlePaperImage.cols, MiddlePaperImage.rows);
	if (StainNumber >= PaperStainNumberThreshold)
	{
		cvtColor(MiddlePaperImage, MiddlePaperImage, CV_GRAY2BGR);
		drawContours(MiddlePaperImage, contours, -1, (0, 0, 255), 2);
		recogdata1.ErrorImage_A = MiddlePaperImage.clone();
		recogdata1.ErrorData_A = "蛋白条脏污，脏污轮廓数量：" + to_string(StainNumber);
		detect_result = 0;
		return;
	}

	recogdata1.ErrorData_A = "Success";
	recogdata1.ErrorData_B = "Success";

	detect_result = 1;
}

void VI_Recog::bottom_detect_cutImage(cv::Mat cutImage, cv::String WindowName, int& detect_result)
{
	cv::Mat binImage;//二值图
	cv::Mat contourImage;//轮廓图像
	cv::Mat bottomImage;//包含试纸盒的最小矩形图像
	cv::Mat bottompaperImage;//包含试纸条的最小矩形图像
	cv::Mat morphImage;//存放开操作处理的图像，在寻找轮廓之前进行开操作可以减少轮廓不闭合的概率。
	cv::Mat floodFillImage;//水漫填充图像
	cv::Mat middlepaperImage;//试纸条中段图像

	cv::Rect rect;//用于存放截取的坐标点，并用来判断rect函数是否为空
	cv::Rect floodFill_rect;//水漫填充的最小矩形边界

	cv::Point2f RectPoint[4];//存放最小矩阵的四个顶点

	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	int floodFill_Diff;//水漫填充范围参数
	int floodFill_color;//水漫填充颜色参数

	int MaxArea = 0;

	/*寻找底壳：二值化后通过搜索最大外轮廓的方法定位*/
	threshold(cutImage, binImage, 80, 255, CV_THRESH_BINARY);
	morphologyEx(binImage, binImage, cv::MORPH_OPEN, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
	morphologyEx(binImage, binImage, cv::MORPH_OPEN, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

	findContours(binImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	if (GetMaxArea(contours, MaxArea, 205000, 170000) == 0)
	{
		cvtColor(cutImage, cutImage, CV_GRAY2BGR);
		drawContours(cutImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = cutImage.clone();
			recogdata2.ErrorData_A = "未找到底壳区域，无底壳";
		}
		else
		{
			recogdata2.ErrorImage_B = cutImage.clone();
			recogdata2.ErrorData_B = "未找到底壳区域，无底壳";
		}
		detect_result = 0;
		return;
	}

	cv::RotatedRect BottomRect = minAreaRect(contours[0]);
	if (ReturnCutImage(cutImage, bottomImage, BottomRect, 20, 20, 20, 20) == 0)
	{
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "未知错误，底壳搜索出错";
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "未知错误，底壳搜索出错";
		}
		detect_result = 0;
		return;
	}


	/*计算底壳上部轮廓数量用以剔除底壳前后反了的情况*/
	//morphImage = bottomImage(cv::Rect(10, 220, 40, 100));
	//cv::blur(morphImage, morphImage, cv::Size(3, 3));
	//Canny(morphImage, contourImage, 35, 105, 3);
	//findContours(contourImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	//int AddAllContoursLength = 0;
	//if (contours.size() >= 1)
	//{
	//	for (int i = 0; i < contours.size(); i++)
	//	{
	//		AddAllContoursLength += arcLength(contours[i], false);
	//	}
	//}

	///************************/
	///******底壳前后反了******/
	//if (AddAllContoursLength < 200)
	//{
	//	if (WindowName == "A")
	//	{
	//		recogdata2.ErrorImage_A = bottomImage.clone();
	//		recogdata2.ErrorData_A = "底壳前后反了";
	//	}
	//	else
	//	{
	//		recogdata2.ErrorImage_B = bottomImage.clone();
	//		recogdata2.ErrorData_B = "底壳前后反了";
	//	}
	//	detect_result = 0;
	//	return;
	//}

	/*试纸条上下偏移以及试纸条轻微的角度偏移*/
	floodFillImage = bottomImage.clone();
	morphologyEx(floodFillImage, floodFillImage, cv::MORPH_OPEN, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

	/**选择三个起始点进行水漫填充是为了消除纸条污损对算法的影响**/
	floodFill_Diff = 2;
	floodFill_color = 0;
	if (floodFillImage.rows > bottomImage.rows / 2 + 30)
	{
		cv::floodFill(floodFillImage, cv::Point(bottomImage.cols / 2, bottomImage.rows / 2 - 30), cv::Scalar(floodFill_color, 0, 0), &floodFill_rect, cv::Scalar(floodFill_Diff, 0, 0), cv::Scalar(floodFill_Diff, 0, 0));
		cv::floodFill(floodFillImage, cv::Point(bottomImage.cols / 2, bottomImage.rows / 2), cv::Scalar(floodFill_color, 0, 0), &floodFill_rect, cv::Scalar(floodFill_Diff, 0, 0), cv::Scalar(floodFill_Diff, 0, 0));
		cv::floodFill(floodFillImage, cv::Point(bottomImage.cols / 2, bottomImage.rows / 2 + 30), cv::Scalar(floodFill_color, 0, 0), &floodFill_rect, cv::Scalar(floodFill_Diff, 0, 0), cv::Scalar(floodFill_Diff, 0, 0));
	}
	else
	{
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "未知错误，水漫填充错误";
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "未知错误，水漫填充错误";
		}
		detect_result = 0;
		return;
	}
	//imshow(WindowName, floodFillImage);
	threshold(floodFillImage, binImage, 1, 255, CV_THRESH_BINARY_INV);
	//imshow(WindowName, binImage);
	/**此处待优化，应该用凸包检测裁剪掉凸包再计算试纸条中间段的中心位置**/
	morphologyEx(binImage, binImage, cv::MORPH_ERODE, getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2)));
	//imshow(WindowName, binImage);
	findContours(binImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	if (GetMaxArea(contours, MaxArea = 0, 10000, 100) == 0)
	{
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "试纸歪了，空试纸条";
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "试纸歪了，空试纸条";
		}
		detect_result = 0;
		return;
	}

	cv::RotatedRect MiddlePaperRect = minAreaRect(contours[0]);
	MiddlePaperRect.points(RectPoint);

	/**************************/
	/******试纸条上下偏移******/
	if (MiddlePaperRect.center.y < BoxPaperPositionYUpThreshold || MiddlePaperRect.center.y > BoxPaperPositionYDownThreshold)
	{
		cvtColor(bottomImage, bottomImage, CV_GRAY2BGR);
		drawContours(bottomImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "试纸条垂直偏移，蛋白条中心位置为："+ to_string(MiddlePaperRect.center.y);
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "试纸条垂直偏移，蛋白条中心位置为：" + to_string(MiddlePaperRect.center.y);
		}
		detect_result = 0;
		return;
	}
	/**************************/
	/******试纸条角度偏移******/
	else if (abs(MiddlePaperRect.angle) > BoxPaperAngleRightThreshold && abs(MiddlePaperRect.angle) < 90 - BoxPaperAngleLeftThreshold)
	{
		cvtColor(bottomImage, bottomImage, CV_GRAY2BGR);
		drawContours(bottomImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (MiddlePaperRect.angle > 45)MiddlePaperRect.angle = 90 - MiddlePaperRect.angle;
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "试纸条角度偏移，角度偏移为：" + to_string(MiddlePaperRect.angle);
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "试纸条角度偏移，角度偏移为：" + to_string(MiddlePaperRect.angle);
		}
		detect_result = 0;
		return;
	}
	/**************************/
	/******试纸条左右偏移******/
	else if (abs(MiddlePaperRect.center.x - 0.5 * bottomImage.cols) > 10)
	{
		cvtColor(bottomImage, bottomImage, CV_GRAY2BGR);
		drawContours(bottomImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "试纸条水平偏移，水平偏移量为：" + to_string(abs(MiddlePaperRect.center.x - 0.5 * bottomImage.cols));
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "试纸条水平偏移，水平偏移量为：" + to_string(abs(MiddlePaperRect.center.x - 0.5 * bottomImage.cols));
		}
		detect_result = 0;
		return;
	}
	/**********************/
	/******试纸条反了******/
	else if (MiddlePaperRect.center.y > 0.5 * bottomImage.rows)
	{
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "试纸条反了：";
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "试纸条反了：";
		}
		detect_result = 0;
		return;
	}

	/*海绵条缺失或者海绵条偏移*/
	cv::Mat SpongeBarImage = bottomImage(cv::Rect(50, 540, 100, 150));
	morphologyEx(SpongeBarImage, morphImage, cv::MORPH_OPEN, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
	Canny(morphImage, contourImage, 26, 101, 3);
	findContours(contourImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	drawContours(SpongeBarImage, contours, -1, (100, 0, 0), 1);
	cv::GaussianBlur(SpongeBarImage, SpongeBarImage, cv::Size(3, 3), 3, 3);

	floodFillImage = SpongeBarImage.clone();
	cv::blur(floodFillImage, floodFillImage, cv::Size(3, 3));
	if (floodFillImage.cols > floodFillImage.cols / 2 && floodFillImage.rows > 20)
	{
		floodFill_Diff = 3;
		floodFill_color = 255;
		cv::floodFill(floodFillImage, cv::Point(floodFillImage.cols / 2, floodFillImage.rows / 2), cv::Scalar(floodFill_color, 0, 0), &floodFill_rect, cv::Scalar(floodFill_Diff, 0, 0), cv::Scalar(floodFill_Diff, 0, 0));
		cv::floodFill(floodFillImage, cv::Point(floodFillImage.cols / 2, floodFillImage.rows / 2 + 20), cv::Scalar(floodFill_color, 0, 0), &floodFill_rect, cv::Scalar(floodFill_Diff, 0, 0), cv::Scalar(floodFill_Diff, 0, 0));
	}
	else
	{
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "未知错误，水漫填充错误：";
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "未知错误，水漫填充错误：";
		}
		detect_result = 0;
		return;
	}


	threshold(floodFillImage, binImage, 254, 255, CV_THRESH_BINARY);
	findContours(binImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/**********************/
	/******海绵条缺失******/
	if (countNonZero(binImage) < 2500)
	{
		cvtColor(bottomImage, bottomImage, CV_GRAY2BGR);
		drawContours(bottomImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "海绵条缺失" ;
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "海绵条缺失";
		}
		detect_result = 0;
		return;
	}

	if (GetMaxArea(contours, MaxArea = 0, INF, 100) == 0)
	{
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "未知错误，未找到海绵条区域" ;
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "未知错误，未找到海绵条区域";
		}
		detect_result = 0;
		return;
	}

	cv::RotatedRect DownPaperRect = minAreaRect(contours[0]);
	DownPaperRect.points(RectPoint);

	//cout << "海绵条角度" << DownPaperRect.angle << endl;
	/**************************/
	/******海绵条偏移角度******/
	if (abs(DownPaperRect.angle) > 5 && abs(DownPaperRect.angle) < 85)
	{
		cvtColor(bottomImage, bottomImage, CV_GRAY2BGR);
		drawContours(bottomImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (DownPaperRect.angle > 45)DownPaperRect.angle = 90 - DownPaperRect.angle;
		if (WindowName == "A")
		{
			recogdata2.ErrorImage_A = bottomImage.clone();
			recogdata2.ErrorData_A = "海绵条角度偏移，角度偏移为：" + to_string(DownPaperRect.angle);
		}
		else
		{
			recogdata2.ErrorImage_B = bottomImage.clone();
			recogdata2.ErrorData_B = "海绵条角度偏移，角度偏移为：" + to_string(DownPaperRect.angle);
		}
		detect_result = 0;
		return;
	}

	if (WindowName == "A")
	{
		recogdata2.ErrorData_A = "Success";
	}
	else
	{
		recogdata2.ErrorData_B = "Success";
	}

	detect_result = 1;
}


void VI_Recog::lid_detect_cutImage(cv::Mat cutImage, cv::String WindowName, int& detect_result)
{
	cv::Mat binImage;//二值图
	cv::Mat morphImage;//存放开操作处理的图像，在寻找轮廓之前进行开操作可以减少轮廓不闭合的概率。
	cv::Mat contourImage;//用于存放canny算子检测结果的轮廓图像
	cv::Mat libImage;//包含试纸盒的最小矩形图像
	cv::Mat floodFillImage;//水漫填充图像

	cv::Rect rect;//用于存放截取的坐标点，并用来判断rect函数是否为空
	cv::Rect floodFill_rect;//水漫填充的最小矩形边界

	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	int MaxArea = 0;

	/*寻找顶盖：二值化后通过搜索最大外轮廓的方法定位*/
	threshold(cutImage, binImage, 100, 255, CV_THRESH_BINARY);
	morphologyEx(binImage, binImage, cv::MORPH_OPEN, getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
	findContours(binImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	/*未找到顶盖区域，底壳有无，图像扭曲，顶盖歪斜*/
	if (GetMaxArea(contours, MaxArea = 0, INF, 170000) == 0)
	{
		if (WindowName == "A")
		{
			recogdata3.ErrorImage_A = cutImage.clone();
			recogdata3.ErrorData_A = "无底壳";
		}
		else
		{
			recogdata3.ErrorImage_B = cutImage.clone();
			recogdata3.ErrorData_B = "无底壳";
		}
		detect_result = 0;
		return;
	}
	if (MaxArea > 205000)
	{
		if (WindowName == "A")
		{
			recogdata3.ErrorImage_A = cutImage.clone();
			recogdata3.ErrorData_A = "顶盖反了或有物体超出边界";
		}
		else
		{
			recogdata3.ErrorImage_B = cutImage.clone();
			recogdata3.ErrorData_B = "顶盖反了或有物体超出边界";
		}
		detect_result = 0;
		return;
	}

	cv::RotatedRect LibRect = minAreaRect(contours[0]);
	if (ReturnCutImage(cutImage, libImage, LibRect, 10, 10, 10, 10) == 0)
	{
		if (WindowName == "A")
		{
			recogdata3.ErrorImage_A = cutImage.clone();
			recogdata3.ErrorData_A = "未知错误";
		}
		else
		{
			recogdata3.ErrorImage_B = cutImage.clone();
			recogdata3.ErrorData_B = "未知错误";
		}
		detect_result = 0;
		return;
	}

	morphologyEx(libImage, binImage, cv::MORPH_CLOSE, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
	threshold(binImage, binImage, 245, 255, CV_THRESH_BINARY);
	findContours(binImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	if (GetMaxArea(contours, MaxArea = 0, INF, 800) == 0)
	{
		cvtColor(libImage, libImage, CV_GRAY2BGR);
		drawContours(libImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (WindowName == "A")
		{
			recogdata3.ErrorImage_A = libImage.clone();
			recogdata3.ErrorData_A = "纸条缺失、顶盖缺失，底壳反了";
		}
		else
		{
			recogdata3.ErrorImage_B = libImage.clone();
			recogdata3.ErrorData_B = "纸条缺失、顶盖缺失，底壳反了";
		}
		detect_result = 0;
		return;
	}

	if (MaxArea < 3500)
	{
		cvtColor(libImage, libImage, CV_GRAY2BGR);
		drawContours(libImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (WindowName == "A")
		{
			recogdata3.ErrorImage_A = libImage.clone();
			recogdata3.ErrorData_A = "纸条偏移、纸条反了，顶盖反了、顶盖未压紧";
		}
		else
		{
			recogdata3.ErrorImage_B = libImage.clone();
			recogdata3.ErrorData_B = "纸条偏移、纸条反了，顶盖反了、顶盖未压紧";
		}
		detect_result = 0;
		return;
	}

	cv::RotatedRect MiddlePaperRect = minAreaRect(contours[0]);
	findContours(binImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	if (GetMaxArea(contours, MaxArea = 0, 2000, 300) == 1)
	{
		cvtColor(libImage, libImage, CV_GRAY2BGR);
		drawContours(libImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (WindowName == "A")
		{
			recogdata3.ErrorImage_A = libImage.clone();
			recogdata3.ErrorData_A = "纸条海绵条缺失，试纸条反了，顶盖反了，试纸条有划痕";
		}
		else
		{
			recogdata3.ErrorImage_B = libImage.clone();
			recogdata3.ErrorData_B = "纸条海绵条缺失，试纸条反了，顶盖反了，试纸条有划痕";
		}
		detect_result = 0;
		return;
	}

	/*裁剪蛋白条图像*/
	cv::Mat MiddlePaperImage;
	if (ReturnCutImage(libImage, MiddlePaperImage, MiddlePaperRect, -20, -10, -20, -10) == 0)
	{
		if (WindowName == "A")
		{
			recogdata3.ErrorImage_A = libImage.clone();
			recogdata3.ErrorData_A = "未知错误";
		}
		else
		{
			recogdata3.ErrorImage_B = libImage.clone();
			recogdata3.ErrorData_B = "未知错误";
		}
		detect_result = 0;
		return;
	}

	threshold(MiddlePaperImage, binImage, 100, 255, CV_THRESH_BINARY_INV);
	double StainArea = (double)countNonZero(binImage) * 4 * 45 * 45 / 1029 / 1029;
	if (StainArea > 0)
	{
		cvtColor(MiddlePaperImage, MiddlePaperImage, CV_GRAY2BGR);
		drawContours(MiddlePaperImage, contours, -1, cv::Scalar(0, 0, 255), 2);
		if (WindowName == "A")
		{
			recogdata3.ErrorImage_A = MiddlePaperImage.clone();
			recogdata3.ErrorData_A = "试纸条有脏污，脏污面积为：" + to_string(StainArea);
		}
		else
		{
			recogdata3.ErrorImage_B = MiddlePaperImage.clone();
			recogdata3.ErrorData_B = "试纸条有脏污，脏污面积为：" + to_string(StainArea);
		}
		detect_result = 0;
		return;
	}

	threshold(MiddlePaperImage, binImage, 175, 255, CV_THRESH_BINARY_INV);

	double GapAera = 0;
	GapAera = (double)countNonZero(binImage) * 4 * 45 * 45 / 1029 / 1029;
	if (GapAera > GapAeraThreshold)
	{
		findContours(binImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		cvtColor(MiddlePaperImage, MiddlePaperImage, CV_GRAY2BGR);
		cv::Mat MiddlePaperImageClone = MiddlePaperImage.clone();
		drawContours(MiddlePaperImage, contours, -1, (0, 0, 255), 1);
		cv::Mat merged_img(MiddlePaperImage.rows, MiddlePaperImageClone.cols + MiddlePaperImage.cols + 1,
			MiddlePaperImageClone.type(), cv::Scalar(0));
		MiddlePaperImageClone.copyTo(merged_img.colRange(0, MiddlePaperImageClone.cols));
		MiddlePaperImage.copyTo(merged_img.colRange(MiddlePaperImage.cols + 1, merged_img.cols));
		if (WindowName == "A")
		{
			recogdata3.ErrorImage_A = merged_img.clone();
			recogdata3.ErrorData_A = "试纸条有缝隙，缝隙面积为：" + to_string(GapAera);
		}
		else
		{
			recogdata3.ErrorImage_B = merged_img.clone();
			recogdata3.ErrorData_B = "试纸条有缝隙，缝隙面积为：" + to_string(GapAera);
		}
		detect_result = 0;
		return;
	}

	if (WindowName == "A")
	{
		recogdata3.ErrorData_A = "Success";
	}
	else
	{
		recogdata3.ErrorData_B = "Success";
	}

	detect_result = 1;
	return;
}

/*
@brief 获取最大轮廓围成的面积。根据设定阈值筛选轮廓

@param contours，输入轮廓
@param MaxArea，最大轮廓面积
@param MaxAreaThreshold，面积高阈值
@param MinAreaThreshold，面积低阈值
*/
int VI_Recog::GetMaxArea(vector<vector<cv::Point>>& contours, int& MaxArea, int MaxAreaThreshold, int MinAreaThreshold)
{
	int AreaValue = 0;

	if (contours.size() > 1)
	{
		vector<vector<cv::Point> >::iterator itc = contours.begin();
		while (itc != contours.end())
		{
			AreaValue = contourArea(*itc);
			if (AreaValue < MaxArea || AreaValue > MaxAreaThreshold || AreaValue < MinAreaThreshold)
			{
				itc = contours.erase(itc);
			}
			else
			{
				MaxArea = AreaValue;
				if (itc != contours.begin())
				{
					itc = contours.erase(itc - 1);
				}
				else
				{
					++itc;
				}
			}
		}
	}
	else if (contours.size() == 1)
	{
		MaxArea = contourArea(contours[0]);
		if (MaxArea < MinAreaThreshold || MaxArea > MaxAreaThreshold)return 0;
	}

	/**空试纸条**/
	if (contours.size() == 0)
	{
		return 0;
	}
	return 1;
}

/*
@brief 获取最大轮廓围成的面积。根据设定阈值筛选轮廓

@param contours，输入轮廓
@param MaxArea，最大轮廓面积
@param MaxAreaThreshold，面积高阈值
@param MinAreaThreshold，面积低阈值
@param MinAreaThreshold，被丢弃的数值的最大值
@param MinAreaThreshold，被丢弃的数值的轮廓
*/
int VI_Recog::GetMaxArea(vector<vector<cv::Point>>& contours, int& MaxArea, int MaxAreaThreshold, int MinAreaThreshold, vector<vector<cv::Point>>& DiscardedContours, int DiscardedMaxValue)
{
	int AreaValue = 0;

	if (contours.size() > 1)
	{
		vector<vector<cv::Point> >::iterator itc = contours.begin();
		while (itc != contours.end())
		{
			AreaValue = contourArea(*itc);
			if (AreaValue < MaxArea || AreaValue > MaxAreaThreshold || AreaValue < MinAreaThreshold)
			{
				if (DiscardedMaxValue > AreaValue)
				{
					DiscardedContours.push_back(contours[0]);
					DiscardedMaxValue = AreaValue;
				}
				itc = contours.erase(itc);
			}
			else
			{
				MaxArea = AreaValue;
				if (itc != contours.begin())
				{
					itc = contours.erase(itc - 1);
				}
				else
				{
					++itc;
				}
			}
		}
	}
	else if (contours.size() == 1)
	{
		MaxArea = contourArea(contours[0]);
		if (MaxArea < MinAreaThreshold || MaxArea > MaxAreaThreshold)return 0;
	}

	/**空试纸条**/
	if (contours.size() == 0)
	{
		return 0;
	}
	return 1;
}

/*
@brief 计算轮廓数量。根据设定阈值筛选轮廓

@param contours，输入轮廓
@param MaxLengthThreshold，轮廓高阈值
@param MinLengthThreshold，轮廓低阈值
*/
int VI_Recog::GetContoursSize(vector<vector<cv::Point>>& contours, int MaxLengthThreshold, int MinLengthThreshold)
{
	vector<vector<cv::Point> >::iterator itc = contours.begin();
	if (contours.size() >= 1)
	{
		while (itc != contours.end())
		{
			if (arcLength(*itc, false) > MaxLengthThreshold || arcLength(*itc, false) < MinLengthThreshold)
			{
				itc = contours.erase(itc);
			}
			else
			{
				++itc;
			}
		}
	}

	return contours.size();
}

/*
@brief 计算轮廓数量。根据设定阈值筛选轮廓，并忽略图像边界处的边缘

@param contours，输入轮廓
@param MaxLengthThreshold，轮廓高阈值
@param MinLengthThreshold，轮廓低阈值
@param ImageWidth，图像宽度
@param ImageHeight，图像宽度
*/
int VI_Recog::GetContoursSize(vector<vector<cv::Point>>& contours, int MaxLengthThreshold, int MinLengthThreshold, int ImageWidth, int ImageHeight)
{
	vector<vector<cv::Point> >::iterator itc = contours.begin();
	if (contours.size() >= 1)
	{
		while (itc != contours.end())
		{
			if (arcLength(*itc, false) > MaxLengthThreshold || arcLength(*itc, false) < MinLengthThreshold)
			{
				itc = contours.erase(itc);
			}
			else
			{
				++itc;
			}
		}
	}

	if (contours.size() >= 1 && contours.size() <= 3)
	{
		//cout << arcLength(contours[0], false) << endl;
		itc = contours.begin();
		while (itc != contours.end())
		{
			cv::RotatedRect Rect = minAreaRect(*itc);
			if (Rect.center.x > 3 && Rect.center.x < ImageWidth - 3 && Rect.center.y > 3 && Rect.center.y < ImageHeight - 3)
			{
				++itc;
			}
			else
			{
				itc = contours.erase(itc);
			}
		}
	}

	return contours.size();
}


/*
@brief 根据旋转矩阵裁剪图像

@param src，输入图像
@param dst，输出图像
@param rect，旋转矩阵
@param ParamLeft，调节图像左侧间距
@param ParamTop，调节图像顶部间距
@param ParamRight，调节图像右侧间距
@param ParamBottom，调节图像底部间距
*/
int VI_Recog::ReturnCutImage(cv::Mat src, cv::Mat& dst, cv::RotatedRect rect, int ParamLeft, int ParamTop, int ParamRight, int ParamBottom)
{
	cv::Point2f RectPoint[4];//存放最小矩阵的四个顶点
	cv::Point2f T_L, T_R, B_L, B_R;//存放最小矩阵的左上角，右上角，左下角。左上角作为切割最小矩阵的原点，将T_R.x - T_L.x 作为矩阵x方向的边长，将B_L.y - T_L.y作为矩阵y方向的边长。
	cv::Rect ImageRect;//用于存放截取的坐标点，并用来判断rect函数是否为空
	cv::Mat RotImage;
	cv::Mat src_clone = src.clone();
	double XLength, YLength = 0;//x,y方向的边长
	rect.points(RectPoint);

	cv::Mat RotMat;
	if (rect.angle > 45)
	{
		RotMat = cv::getRotationMatrix2D(rect.center, rect.angle - 90, 1.0);//求旋转矩阵
		XLength = rect.size.height;
		YLength = rect.size.width;
	}
	else
	{
		RotMat = cv::getRotationMatrix2D(rect.center, rect.angle, 1.0);//求旋转矩阵
		XLength = rect.size.width;
		YLength = rect.size.height;
	}
	cv::warpAffine(src_clone, RotImage, RotMat, src_clone.size());
	//imshow("1", RotImage);

	ImageRect.x = rect.center.x - XLength / 2 + ParamLeft; ImageRect.y = rect.center.y - YLength / 2 + ParamTop; ImageRect.width = XLength - ParamLeft - ParamRight; ImageRect.height = YLength - ParamTop - ParamBottom;
	ImageRect &= cv::Rect(0, 0, src_clone.cols, src_clone.rows);
	if (ImageRect.width <= 0 || ImageRect.height <= 0)
	{
		return 0;
	}

	dst = RotImage(ImageRect);

	return 1;
}

/*
@brief 对边缘进行滤波，注：contours数量应为1

@param filterRadius，高斯滤波参数
@param sigma，高斯滤波参数
@param contours，轮廓
*/
void VI_Recog::EdgeFilter(int filterRadius, double sigma, vector<vector<cv::Point>>& contours)
{
	int filterSize = 2 * filterRadius + 1;

	size_t len = contours[0].size() + 2 * filterRadius;
	size_t idx = (contours[0].size() - filterRadius);
	vector<float> x, y;
	for (size_t i = 0; i < len; i++)
	{
		x.push_back(contours[0][(idx + i) % contours[0].size()].x);
		y.push_back(contours[0][(idx + i) % contours[0].size()].y);
	}

	vector<float> xFilt, yFilt;
	cv::GaussianBlur(x, xFilt, cv::Size(filterSize, filterSize), sigma, sigma);
	cv::GaussianBlur(y, yFilt, cv::Size(filterSize, filterSize), sigma, sigma);

	int ContoursSize = contours[0].size();
	contours[0].clear();
	for (size_t i = filterRadius; i < ContoursSize + filterRadius; i++)
	{
		contours[0].push_back(cv::Point(xFilt[i], yFilt[i]));
	}

}

void VI_Recog::drawGoldMark(cv::RotatedRect rect, double length, cv::Mat cutImage)
{
	cv::Point2f RectPoint[4];//存放最小矩阵的四个顶点
	rect.points(RectPoint);
	cv::Mat lineImage = cutImage.clone();

	int rectlength = length * 1029 / 45;
	if (rect.angle > 45)
	{
		RectPoint[0].x = RectPoint[1].x + rectlength;
		RectPoint[0].y = RectPoint[1].y;
		RectPoint[3].x = RectPoint[2].x + rectlength;
		RectPoint[3].y = RectPoint[2].y;

	}
	else
	{
		RectPoint[0].x = RectPoint[3].x + rectlength;
		RectPoint[0].y = RectPoint[3].y;
		RectPoint[1].x = RectPoint[2].x + rectlength;
		RectPoint[1].y = RectPoint[2].y;
	}

	for (int j = 0; j < 4; j++)
	{
		cv::line(lineImage, RectPoint[j], RectPoint[(j + 1) % 4], cv::Scalar(0, 0, 255), 3, 8);  //绘制最小外接矩形每条边
	}

	cvtColor(lineImage, lineImage, CV_GRAY2BGR);
	recogdata1.ErrorImage_A = lineImage.clone();
}