#include "MarkTheError.h"
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <qdebug.h>
#include <Windows.h>

MarkTheError::MarkTheError(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);//禁止最大化窗口
	ui.frame->setAlignment(Qt::AlignCenter);
	ui.frame_2->setAlignment(Qt::AlignCenter);
}

void MarkTheError::ShowErrorInfo(cv::Mat InsideErrorImage, cv::Mat OutsideErrorImage, QString InsideErrorData, QString OutsideErrorData)
{
	ui.frame->setHidden(true);
	ui.frame_2->setHidden(true);
	if (!InsideErrorImage.empty() && InsideErrorData != "Success")
	{
		ui.frame->setHidden(false);
	}
	if (!OutsideErrorImage.empty() && OutsideErrorData != "Success")
	{
		ui.frame_2->setHidden(false);
	}

	if (!InsideErrorImage.empty())
	{
		ShowErrorImage(ui.frame, InsideErrorImage);
		ui.label->setText(InsideErrorData);
	}
	if (!OutsideErrorImage.empty())
	{
		ShowErrorImage(ui.frame_2, OutsideErrorImage);
		ui.label_2->setText(OutsideErrorData);
	}
}


QImage MarkTheError::MatToQImage(const cv::Mat& mat)
{
	cv::Mat mat_RGB;
	cv::cvtColor(mat, mat_RGB, CV_BGR2RGB);
	QImage image((const unsigned char *)(mat_RGB.data), mat_RGB.cols, mat_RGB.rows, mat_RGB.cols * 3, QImage::Format_RGB888);
	return image.rgbSwapped();
}

void MarkTheError::ShowErrorImage(QLabel* label, const cv::Mat mat)
{
	QImage Img = MatToQImage(mat);
	double label_height = label->height();
	double label_width = label->width();
	double label_ratio = label_height / label_width;
	double img_height = Img.height();
	double img_width = Img.width();
	double img_ratio = img_height / img_width;

	if (Img.isNull() || Img.height() == 0 || Img.width() == 0)
	{
		label->setText("无图像数据");
		return;
	}
	if (label_ratio > img_ratio)
	{
		label->setPixmap(QPixmap::fromImage(Img.scaled(label_width, img_height*label_width / img_width)));
	}
	else
	{
		label->setPixmap(QPixmap::fromImage(Img.scaled(img_width*label_height / img_height, label_height)));

	}

}