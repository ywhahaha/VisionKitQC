#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <qbuttongroup.h>
#include "ui_MainRoiPage.h"
#include "RoiPage.h"
#include <opencv2\core\core.hpp>


class MainRoiPage : public QWidget
{
	Q_OBJECT

public:
	MainRoiPage(QWidget *parent = Q_NULLPTR);
	~MainRoiPage();

	void get_image(cv::Mat img1, cv::Mat img2, cv::Mat img3);
	void get_index(int index);
	bool WindowIsOpened = false;

protected:
	void closeEvent(QCloseEvent *event);

private:
	Ui::MainRoiPage ui;

	RoiPage* page1;
	RoiPage* page2;
	RoiPage* page3;

	QButtonGroup * buttonbox;

signals:
	void ThisWindowIsClosed();

private slots:
	void SwitchThePage();

};
