#pragma once

#include <QWidget>
#include <QMenu>
#include "PaintRectRoi.h"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <opencv2\core\core.hpp>
#include "ui_RoiPage.h"

#define WINDOW_WIDTH 768		  //图像窗口的宽度
#define WINDOW_HEIGHT 512		  //图像窗口的高度

class RoiPage : public QWidget
{
	Q_OBJECT

public:
	RoiPage(QWidget *parent = Q_NULLPTR);
	~RoiPage();

	void GetPageImage(cv::Mat img);
	void GetPageIndex(int index);

private:
	Ui::ControlPanel ui;

	PaintRectRoi* rectroi;
	QImage Img;
	QLabel* ShowImage;

	int page_index;

private:
	void showToolTip(int index, int max, int min, int step);

public slots:
	void PaintRoiArea0();
	void PaintRoiArea1();
	void PaintRoiAreaUnion(bool status);
	void ShowControlPanel_0(QRect rect, int dec);
	void ShowControlPanel_1(QRect rect, int dec);
	void ShowControlPanel_union(QRect rect, int dec);
	void DealSpinBoxValue();

signals:
	void SpinBoxValue(int value, int number);
};
