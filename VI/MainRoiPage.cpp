#include "MainRoiPage.h"
#include <iostream>
#include <QDesktopWidget>

MainRoiPage::MainRoiPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	page1 = new RoiPage;
	page2 = new RoiPage;
	page3 = new RoiPage;

	page1->GetPageIndex(1);
	page2->GetPageIndex(2);
	page3->GetPageIndex(3);
	
	ui.stackedWidget->addWidget(page1);
	ui.stackedWidget->addWidget(page2);
	ui.stackedWidget->addWidget(page3);

	connect(ui.pushButton, SIGNAL(clicked(bool)), this, SLOT(SwitchThePage()));
	connect(ui.pushButton_2, SIGNAL(clicked(bool)), this, SLOT(SwitchThePage()));
	connect(ui.pushButton_3, SIGNAL(clicked(bool)), this, SLOT(SwitchThePage()));

	/*按钮互斥*/
	buttonbox = new QButtonGroup;
	buttonbox->setExclusive(true);
	buttonbox->addButton(ui.pushButton);
	buttonbox->addButton(ui.pushButton_2);
	buttonbox->addButton(ui.pushButton_3);

	setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);//禁止最大化窗口
	QDesktopWidget* desktop = QApplication::desktop();
	move(0,0);
}

MainRoiPage::~MainRoiPage()
{
}

void MainRoiPage::get_image(cv::Mat img1, cv::Mat img2, cv::Mat img3)
{
	page1->GetPageImage(img1);
	page2->GetPageImage(img2);
	page3->GetPageImage(img3);
}


void MainRoiPage::get_index(int index)
{
	ui.stackedWidget->setCurrentIndex(index + 1);//加1是因为按钮的索引是从2开始的

	switch (index)
	{
	case 1:
	{
		ui.pushButton->setChecked(true);
		break;
	}
	case 2:
	{
		ui.pushButton_2->setChecked(true);
		break;
	}
	case 3:
	{
		ui.pushButton_3->setChecked(true);
		break;
	}
	default:
	{
		break;
	}
	}
}


void MainRoiPage::SwitchThePage()
{
	QPushButton *button = qobject_cast<QPushButton*>(sender());
	if (button == ui.pushButton)
	{
		ui.stackedWidget->setCurrentIndex(2);
	}
	else if (button == ui.pushButton_2)
	{
		ui.stackedWidget->setCurrentIndex(3);
	}
	else if (button == ui.pushButton_3)
	{
		ui.stackedWidget->setCurrentIndex(4);
	}
}

void MainRoiPage::closeEvent(QCloseEvent *event)
{
	bool WindowIsOpened = false;
}