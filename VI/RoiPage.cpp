#include "RoiPage.h"
#include <QPainter>
#include <qdebug.h>
#include <QHelpEvent>
#include <QToolTip>
# pragma execution_character_set("utf-8")

RoiPage::RoiPage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ShowImage = new QLabel;
	ShowImage->setAlignment(Qt::AlignCenter);
	ui.gridLayout_2->addWidget(ShowImage, 0, 1, 1, 1);

	rectroi = new PaintRectRoi;
	rectroi->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);
	ui.gridLayout_2->addWidget(rectroi, 0, 1, 1, 1);

	connect(rectroi, SIGNAL(getRoiRect0(QRect, int)), this, SLOT(ShowControlPanel_0(QRect, int)));//接收ROI的实时数据
	connect(rectroi, SIGNAL(getRoiRect1(QRect, int)), this, SLOT(ShowControlPanel_1(QRect, int)));
	connect(rectroi, SIGNAL(getRoiRectUnion(QRect, int)), this, SLOT(ShowControlPanel_union(QRect, int)));

	/*控制面板区域0*/
	connect(ui.spinBox_26, SIGNAL(editingFinished()), this, SLOT(DealSpinBoxValue()));//完成修改spinbox值后对数据进行分发
	connect(ui.spinBox_25, SIGNAL(editingFinished()), this, SLOT(DealSpinBoxValue()));
	connect(ui.spinBox_27, SIGNAL(editingFinished()), this, SLOT(DealSpinBoxValue()));
	connect(ui.spinBox_28, SIGNAL(editingFinished()), this, SLOT(DealSpinBoxValue()));

	connect(ui.pushButton_5, SIGNAL(clicked(bool)), this, SLOT(PaintRoiArea0()));//开始绘制ROI

	/*控制面板区域1*/
	connect(ui.spinBox_29, SIGNAL(editingFinished()), this, SLOT(DealSpinBoxValue()));//完成修改spinbox值后对数据进行分发
	connect(ui.spinBox_30, SIGNAL(editingFinished()), this, SLOT(DealSpinBoxValue()));
	connect(ui.spinBox_31, SIGNAL(editingFinished()), this, SLOT(DealSpinBoxValue()));
	connect(ui.spinBox_32, SIGNAL(editingFinished()), this, SLOT(DealSpinBoxValue()));

	connect(ui.pushButton_6, SIGNAL(clicked(bool)), this, SLOT(PaintRoiArea1()));//开始绘制ROI

	/*控制面板联合区域*/
	connect(ui.pushButton_7, SIGNAL(clicked(bool)), this, SLOT(PaintRoiAreaUnion(bool)));//生成或删除联合区域ROI
	connect(ui.pushButton_8, SIGNAL(clicked()), rectroi, SLOT(saveUnionROIImage()));//保存联合区域ROI

	connect(this, SIGNAL(SpinBoxValue(int, int)), rectroi, SLOT(GetSpinBoxChangedValue(int, int)));//手动修改spinbox值会改变roi区域大小

}

RoiPage::~RoiPage()
{
}

void RoiPage::GetPageImage(cv::Mat img)
{
	if (img.empty())
	{
		ShowImage->setText("无图像数据，请建立连接后再打开此页面");
		ShowImage->setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		ShowImage->setMaximumSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else if (img.rows % 512 != 0)
	{
		rectroi->ClearScreen();
		Img = QImage((const uchar*)(img.data), img.cols, img.rows, img.cols * img.channels(), QImage::Format_Grayscale8);
		Img = Img.scaled(WINDOW_WIDTH, WINDOW_HEIGHT);
		ShowImage->setPixmap(QPixmap::fromImage(Img));
	}
	else
	{
		Img = QImage((const uchar*)(img.data), img.cols, img.rows, img.cols * img.channels(), QImage::Format_Grayscale8);
		Img = Img.scaled(WINDOW_WIDTH, WINDOW_HEIGHT);
		ShowImage->setPixmap(QPixmap::fromImage(Img));
		rectroi->ShowInitRoi(page_index);
		rectroi->ShowInitUnionRoi();

		ui.spinBox_26->setEnabled(true);
		ui.spinBox_25->setEnabled(true);
		ui.spinBox_27->setEnabled(true);
		ui.spinBox_28->setEnabled(true);
		ui.pushButton_5->setEnabled(true);
		ui.spinBox_29->setEnabled(true);
		ui.spinBox_30->setEnabled(true);
		ui.spinBox_31->setEnabled(true);
		ui.spinBox_32->setEnabled(true);
		ui.pushButton_6->setEnabled(true);
		ui.pushButton_7->setEnabled(true);
	}
}

void RoiPage::GetPageIndex(int index)
{
	page_index = index;
}

/*画区域0*/
void RoiPage::PaintRoiArea0()
{
	rectroi->startPaint(0);
}

/*画区域1*/
void RoiPage::PaintRoiArea1()
{
	rectroi->startPaint(1);
}

/*画联合区域*/
void RoiPage::PaintRoiAreaUnion(bool status)
{
	if (status)
	{
		ui.pushButton_7->setText("删除ROI");
		rectroi->CreateUnionRoi();
		ui.pushButton_8->setEnabled(true);//未生成ROI禁止保存
	}
	else
	{
		ui.pushButton_7->setText("生成ROI");
		rectroi->DeleteUnionRoi();
		ui.pushButton_8->setEnabled(false);//未生成ROI禁止保存
	}
}


/*控制面板的区域0*/
void RoiPage::ShowControlPanel_0(QRect rect, int dec)
{
	rect = QRect(rect.x() * 4 / dec, rect.y() * 4 / dec, rect.width() * 4 / dec, rect.height() * 4 / dec);

	int image_width = WINDOW_WIDTH * 4 / dec;
	int image_height = WINDOW_HEIGHT * 4 / dec;

	ui.spinBox_26->setMaximum(image_width - rect.width());//OffsetX
	ui.spinBox_25->setMaximum(image_height - rect.height());//OffsetY
	ui.spinBox_27->setMaximum(image_width - rect.x());//width
	ui.spinBox_28->setMaximum(image_height - rect.y());//height
	ui.spinBox_26->setSingleStep(4 / dec);//设置步进值
	ui.spinBox_25->setSingleStep(4 / dec);
	ui.spinBox_27->setSingleStep(4 / dec);
	ui.spinBox_28->setSingleStep(4 / dec);


	ui.spinBox_26->setValue(rect.x());
	ui.spinBox_25->setValue(rect.y());
	ui.spinBox_27->setValue(rect.width());
	ui.spinBox_28->setValue(rect.height());
}

/*控制面板的区域1*/
void RoiPage::ShowControlPanel_1(QRect rect, int dec)
{
	rect = QRect(rect.x() * 4 / dec, rect.y() * 4 / dec, rect.width() * 4 / dec, rect.height() * 4 / dec);

	int image_width = WINDOW_WIDTH * 4 / dec;
	int image_height = WINDOW_HEIGHT * 4 / dec;

	ui.spinBox_29->setMaximum(image_width - rect.width());//OffsetX
	ui.spinBox_30->setMaximum(image_height - rect.height());//OffsetY
	ui.spinBox_31->setMaximum(image_width - rect.x());//width
	ui.spinBox_32->setMaximum(image_height - rect.y());//height

	ui.spinBox_29->setValue(rect.x());
	ui.spinBox_30->setValue(rect.y());
	ui.spinBox_31->setValue(rect.width());
	ui.spinBox_32->setValue(rect.height());
}

/*控制面板的联合区域*/
void RoiPage::ShowControlPanel_union(QRect rect, int dec)
{
	rect = QRect(rect.x() * 4 / dec, rect.y() * 4 / dec, rect.width() * 4 / dec, rect.height() * 4 / dec);

	int image_width = WINDOW_WIDTH * 4 / dec;
	int image_height = WINDOW_HEIGHT * 4 / dec;

	ui.spinBox_34->setMaximum(image_width - rect.width());//OffsetX
	ui.spinBox_35->setMaximum(image_height - rect.height());//OffsetY
	ui.spinBox_36->setMaximum(image_width - rect.x());//width
	ui.spinBox_33->setMaximum(image_height - rect.y());//height

	ui.spinBox_34->setValue(rect.x());
	ui.spinBox_35->setValue(rect.y());
	ui.spinBox_36->setValue(rect.width());
	ui.spinBox_33->setValue(rect.height());
}



void RoiPage::DealSpinBoxValue()
{
	QSpinBox *spinbox = qobject_cast<QSpinBox*>(sender());

	if (spinbox->objectName() == "spinBox_26")
	{
		showToolTip(1, ui.spinBox_26->maximum(), ui.spinBox_26->minimum(), ui.spinBox_26->singleStep());
		emit SpinBoxValue(ui.spinBox_26->value(), 1);
	}
	else if (spinbox->objectName() == "spinBox_25")
	{
		showToolTip(2, ui.spinBox_25->maximum(), ui.spinBox_25->minimum(), ui.spinBox_25->singleStep());
		emit SpinBoxValue(ui.spinBox_25->value(), 2);
	}
	else if (spinbox->objectName() == "spinBox_27")
	{
		showToolTip(3, ui.spinBox_27->maximum(), ui.spinBox_27->minimum(), ui.spinBox_27->singleStep());
		emit SpinBoxValue(ui.spinBox_27->value(), 3);
	}
	else if (spinbox->objectName() == "spinBox_28")
	{
		showToolTip(4, ui.spinBox_28->maximum(), ui.spinBox_28->minimum(), ui.spinBox_28->singleStep());
		emit SpinBoxValue(ui.spinBox_28->value(), 4);
	}
	else if (spinbox->objectName() == "spinBox_29")
	{
		showToolTip(5, ui.spinBox_29->maximum(), ui.spinBox_29->minimum(), ui.spinBox_29->singleStep());
		emit SpinBoxValue(ui.spinBox_29->value(), 5);
	}
	else if (spinbox->objectName() == "spinBox_30")
	{
		showToolTip(6, ui.spinBox_30->maximum(), ui.spinBox_30->minimum(), ui.spinBox_30->singleStep());
		emit SpinBoxValue(ui.spinBox_30->value(), 6);
	}
	else if (spinbox->objectName() == "spinBox_31")
	{
		showToolTip(7, ui.spinBox_31->maximum(), ui.spinBox_31->minimum(), ui.spinBox_31->singleStep());
		emit SpinBoxValue(ui.spinBox_31->value(), 7);
	}
	else if (spinbox->objectName() == "spinBox_32")
	{
		showToolTip(8, ui.spinBox_32->maximum(), ui.spinBox_32->minimum(), ui.spinBox_32->singleStep());
		emit SpinBoxValue(ui.spinBox_32->value(), 8);
	}
}

void RoiPage::showToolTip(int index, int max, int min, int step)
{
	QString str = QString("最大值：%0；最小值：%1；步进值：%2").arg(max).arg(min).arg(step);
	QToolTip::showText(QCursor::pos(), str);
}