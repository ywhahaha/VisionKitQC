#pragma once

#include <QWidget>
#include "ui_MarkTheError.h"
#include <opencv2\core\core.hpp>
#include <qstring.h>
#include <qimage.h>

class MarkTheError : public QWidget
{
	Q_OBJECT

public:
	MarkTheError(QWidget *parent = nullptr);
	void ShowErrorInfo(cv::Mat InsideErrorImage, cv::Mat OutsideErrorImage, QString InsideErrorData, QString OutsideErrorData);

private:
	Ui::MarkTheErrorClass ui;
	QImage MatToQImage(const cv::Mat& mat);
	void ShowErrorImage(QLabel* label, const cv::Mat mat);
};
