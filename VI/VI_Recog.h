#pragma once
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <time.h>
#include <MvCameraControl.h>
#include <stdio.h>
#include <conio.h>
#include "INI_Config.h"
#include "RecogData.h"

using namespace std;

struct RecogErrorImageItem
{
	cv::Mat InsideErrorImage;
	cv::Mat OutsideErrorImage;

	string InsideErrorData;
	string OutsideErrorData;
};

class VI_Recog
{
public:
	void VI_Image_Recog(cv::Mat image, int& result, int camera_type);
	void GetROI(int camera_type);

	cv::Mat GetInsideImage(int camera_type);//外部调用,用于展示在UI界面
	cv::Mat GetOutsideImage(int camera_type);//外部调用,用于展示在UI界面
	RecogErrorImageItem GetErrorInfo(int camera_type);

	int GetCutImage(cv::Mat OriImage, cv::Mat& cutImage_A, cv::Mat& cutImage_B, int camera_type, int& result);
	void ShowInitImage(cv::Mat OriImage, cv::Mat& InsideImage, cv::Mat& OutsideImage, int camera_type);

private:
	void paper_detect(cv::Mat OriImage, int& result);
	void bottom_detect(cv::Mat OriImage, int& result);
	void lid_detect(cv::Mat OriImage, int& result);

	void paper_detect_cutImage(cv::Mat cutImage, cv::String WindowName, int& detect_result);
	void PaperSlotDetect(cv::Mat cutImage, cv::String WindowName, int& detect_result);
	void bottom_detect_cutImage(cv::Mat cutImage, cv::String WindowName, int& detect_result);
	void lid_detect_cutImage(cv::Mat cutImage, cv::String WindowName, int& detect_result);
	int GetMaxArea(vector<vector<cv::Point>>& contours, int& MaxArea, int MaxAreaThreshold, int MinAreaThreshold);
	int GetMaxArea(vector<vector<cv::Point>>& contours, int& MaxArea, int MaxAreaThreshold, int MinAreaThreshold, vector<vector<cv::Point>>& DiscardedContours, int DiscardedMaxValue);
	int GetContoursSize(vector<vector<cv::Point>>& contours, int MaxLengthThreshold, int MinLengthThreshold);
	int GetContoursSize(vector<vector<cv::Point>>& contours, int MaxLengthThreshold, int MinLengthThreshold, int ImageWidth, int ImageHeight);
	int ReturnCutImage(cv::Mat src, cv::Mat& dst, cv::RotatedRect rect, int Param1, int Param2, int Param3, int Param4);
	void EdgeFilter(int filterRadius, double sigma, vector<vector<cv::Point>>& contours);


	/*画图函数*/
	void drawGoldMark(cv::RotatedRect rect, double length, cv::Mat cutImage);


	const int INF = 0x3fffffff;

	double PaperGoldenMarkLengthThreshold = -1;//金标线宽，单位mm
	double PaperWidthUpThreshold = 3.2;//试纸条宽度误差，单位mm
	double PaperWidthDownThreshold = 2.8;//试纸条宽度误差，单位mm
	double PaperPositiveAngleThreshold = 2;//试纸条与x轴正方向的夹角为正
	double PaperNegativeAngleThreshold = 2;//试纸条与x轴正方向的夹角为负
	double PaperStainLengthThreshold = 1;//试纸条脏污长度，单位mm
	double PaperStainNumberThreshold = 1;//试纸条脏污数量
	double PaperStainColorDegreeThreshold = 50;//试纸条脏污明显程度，值越小表明所能容忍的脏污程度越大，单位灰度值，范围（0，255）
	double BoxPaperAngleLeftThreshold = 2;//试纸盒内纸条左倾角度上限(°)
	double BoxPaperAngleRightThreshold = 2;//试纸盒内纸条右倾角度上限(°)
	double BoxPaperPositionYUpThreshold = 332;//试纸盒内纸条垂直方向的位置上限
	double BoxPaperPositionYDownThreshold = 357;//试纸盒内纸条垂直方向的位置下限
	double GapAeraThreshold = 4;//缝隙面积，单位mm^2

	int PaperUpsidedownWarningTimes = 5;//大卡反了报警阈值
	int PaperUpsidedownWarningCounter = 0;//大卡反了报警计数

	int Camera1Result = -1;//设置相机1输出
	int Camera2Result = -1;//设置相机2输出
	int Camera3Result = -1;//设置相机3输出

	RecogData recogdata1;
	RecogData recogdata2;
	RecogData recogdata3;
	cv::Rect subroi[3][2] = {};//获取图像roi子区域

};

