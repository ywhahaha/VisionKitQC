#pragma once
#include "VI.h"
#include <qlabel.h>
#include <qframe.h>
#include <QtWidgets/QHBoxLayout>
#include "StateCode.h"
#include <Windows.h>
#include"Get_Local_IP.h"
#include <qdebug.h>

# pragma execution_character_set("utf-8")
bool DebugFlag = false;
#define MULTILINE(...) #__VA_ARGS__

VI::VI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	/*增加connect传输格式*/
	qRegisterMetaType< cv::Mat >("cv::Mat");
	qRegisterMetaType<string>("string");

	ShowInitStatusBar();//构建状态栏
	ShowInitRecordBar();//显示记录栏初始值

	vi_task->moveToThread(vi_thread);
	getlocalpcinfo->moveToThread(vi_thread_2);
	
	vi_thread->start();
	vi_thread_2->start();

	/*设置UI控件格式*/
	setWindowState(Qt::WindowFullScreen);
	log_window->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
	PaintRoi_window->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
	settingwindow->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
	historydatawindow->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
	MarkTheErrorWindow->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

	ui.frame->setAlignment(Qt::AlignCenter);
	ui.frame_2->setAlignment(Qt::AlignCenter);
	ui.frame_3->setAlignment(Qt::AlignCenter);
	ui.frame_5->setAlignment(Qt::AlignCenter);
	ui.frame_6->setAlignment(Qt::AlignCenter);

	/*初始化数据库*/
	mysql_init(&DatabaseVI);
	mysql_real_connect(&DatabaseVI, "127.0.0.1", "root", "123456", "VI", 3306, NULL, 0);
	mysql_query(&DatabaseVI, "set character set gbk");

	/*建立连接*/
	connect(this, SIGNAL(Init_VI_signals()), vi_task, SLOT(Init_VI()));
	connect(vi_task, SIGNAL(Reconnect_VI_signals(bool)), this, SLOT(StartVISystem(bool)));
	connect(vi_task, SIGNAL(FinishInit(bool)), this, SLOT(begin_VI2(bool)));
	connect(vi_task, SIGNAL(FinishInit(bool)), this, SLOT(begin_VI(bool)));
	connect(this, SIGNAL(Begin_VI_signals()), vi_task, SLOT(VI()));
	connect(vi_task, SIGNAL(Restart(bool)), this, SLOT(begin_VI(bool)));

	/*断开连接*/
	connect(this, SIGNAL(Close_VI_signals()), vi_task, SLOT(Close_VI()));
	connect(vi_task, SIGNAL(FinishClose()), this, SLOT(CloseVISystem()));
	
	/*采集到图片后发送至UI界面展示*/
	connect(vi_task, SIGNAL(image_msg(cv::Mat,int, int)), this, SLOT(ShowImage(cv::Mat, int, int)));

	/*获取系统状态*/
	connect(vi_task, SIGNAL(SysStatus(int, int)), this, SLOT(ShowSysStatus(int, int)));

	/*获取相机连接状态*/
	connect(this, SIGNAL(GetCameraConnectStatus()), vi_task, SLOT(IsCameraConnect()));
	connect(vi_task, SIGNAL(CameraConnectStatus(int)), this, SLOT(ShowCameraStatus(int)));

	/*定时获取特定系统信息*/
	connect(this, SIGNAL(RestartRegularCheckSys()), getlocalpcinfo, SLOT(GetCurrentInfo()));
	connect(getlocalpcinfo, SIGNAL(sendCurrentInfo(double, double)), this, SLOT(RegularCheckSys(double, double)));
	connect(getlocalpcinfo, SIGNAL(Restart(bool)), this, SLOT(RestartRegularCheckSys(bool)));

	/*传递MYSQL数据*/
	connect(vi_task, SIGNAL(MYSQLData(string)), this, SLOT(InsertDataToMYSQLDatabase(string)));

	/*传递错误标记数据*/
	connect(vi_task, SIGNAL(ErrorInfo(cv::Mat, cv::Mat, string, string, int)), this, SLOT(PackageErrorItem(cv::Mat, cv::Mat, string, string, int)));

	/*登录成功后获得权限*/
	connect(log_window, &show_log::CloseLogWindow, [=](bool LogStatus)
	{
		if (LogStatus)
		{
			ui.action_Roi->setEnabled(true);
			ui.action_setting->setEnabled(true);
			DebugStatus->setVisible(true);
			ui.action_login->setChecked(true);
			vi_task->StopReconnect = true;
		}
	});
	

	/*打开调试模式*/
	connect(DebugStatus, &QCheckBox::clicked, [=]()
	{
		DebugFlag = DebugStatus->isChecked();
		if (!DebugFlag)
		{
			int ExpiredImages = 2;
			bool SaveDataFlag = true;
		}
	});
	/*启动程序后自动运行程序*/
	CreateAndDeleteImageFiles();
	GetTodayStatistics();
	StartVISystem(true);
}

VI::~VI()
{}


void VI::StartVISystem(bool start_msg)
{
	if (DebugFlag)
	{
		ini->ReadINI("config.ini");//读.ini配置文件

		if (ini->GetValue("DeveloperOptions", "SaveData") == "true")
		{
			SaveDataFlag = true;
		}
		else
		{
			SaveDataFlag = false;
		}

		ini->Clear();
	}

	if (start_msg)
	{
		ui.action_stop->setEnabled(false);
		DebugStatus->setEnabled(false);
		start_grab_flag = true;
		closeflag = false;

		emit Init_VI_signals();
		emit RestartRegularCheckSys();
	}
	else
	{
		ui.action_stop->setEnabled(false);
		ui.action_stop->setToolTip("运行");
		start_grab_flag = false;
	}
}

/*字符格式转换*/
LPCWSTR VI::string2LPCWSTR(std::string str)
{
	size_t size = str.length();
	int wLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t *buffer = new wchar_t[wLen + 1];
	memset(buffer, 0, (wLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), size, (LPWSTR)buffer, wLen);
	return buffer;
}

QImage VI::MatToQImage(const cv::Mat& mat)
{
	QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
	// Set the color table (used to translate colour indexes to qRgb values)
	image.setColorCount(256);
	for (int i = 0; i < 256; i++)
	{
		image.setColor(i, qRgb(i, i, i));
	}
	// Copy input Mat
	uchar* pSrc = mat.data;
	for (int row = 0; row < mat.rows; row++)
	{
		uchar* pDest = image.scanLine(row);
		memcpy(pDest, pSrc, mat.cols);
		pSrc += mat.step;
	}
	return image;
}


void VI::CreateAndDeleteImageFiles()
{
	/*根据日期创建图片文件夹*/
	time(&nowtime);
	p = localtime(&nowtime);

	ini->ReadINI("config.ini");
	ExpiredImages = stoi(ini->GetValue("DeveloperOptions", "ExpiredImages"));

	ini->Clear();

	char prefix[256] = {};
	sprintf_s(prefix, 256, "D:\\VI\\figs\\%04d%02d%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday);

	if (_access(prefix, 0) == -1)	//如果文件夹不存在
		_mkdir(prefix);				//则创建

	for (int i = 0; i < 3; i++)
	{
		sprintf_s(prefix, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, i + 1);

		if (_access(prefix, 0) == -1)	//如果文件夹不存在
			_mkdir(prefix);				//则创建

		sprintf_s(prefix, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\Success", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, i + 1);
		if (_access(prefix, 0) == -1)	//如果文件夹不存在
			_mkdir(prefix);				//则创建

		sprintf_s(prefix, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\False", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, i + 1);
		if (_access(prefix, 0) == -1)	//如果文件夹不存在
			_mkdir(prefix);				//则创建

		sprintf_s(prefix, 256, "D:\\VI\\figs\\%04d%02d%02d\\debug", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday);
		if (_access(prefix, 0) == -1)	//如果文件夹不存在
			_mkdir(prefix);				//则创建
	}


	/*删除过期文件夹（删除两天前的良品图片）*/
	time_t _tm = mktime(p);//时间转换

	for (int day = ExpiredImages;day < 90;day++)
	{
		_tm -= day * 60 * 60 * 24;
		p = localtime(&_tm);

		char RemoveFilename[256] = {};
		for (int i = 0; i < 3; i++)
		{
			sprintf_s(RemoveFilename, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\Success", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, i + 1);
			if (_access(RemoveFilename, 0) == -1)break;

			string RemoveFilename_str = RemoveFilename;
			SHFILEOPSTRUCT FileOp;
			FileOp.fFlags = FOF_NOCONFIRMATION;
			FileOp.hNameMappings = NULL;
			FileOp.hwnd = NULL;
			FileOp.lpszProgressTitle = NULL;
			FileOp.pFrom = string2LPCWSTR(RemoveFilename_str);
			FileOp.pTo = NULL;
			FileOp.wFunc = FO_DELETE;
			SHFileOperation(&FileOp);
		}

		for (int i = 0; i < 3; i++)
		{
			sprintf_s(RemoveFilename, 256, "D:\\VI\\figs\\%04d%02d%02d\\camera%d\\debug", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, i + 1);
			if (_access(RemoveFilename, 0) == -1)break;

			string RemoveFilename_str = RemoveFilename;
			SHFILEOPSTRUCT FileOp;
			FileOp.fFlags = FOF_NOCONFIRMATION;
			FileOp.hNameMappings = NULL;
			FileOp.hwnd = NULL;
			FileOp.lpszProgressTitle = NULL;
			FileOp.pFrom = string2LPCWSTR(RemoveFilename_str);
			FileOp.pTo = NULL;
			FileOp.wFunc = FO_DELETE;
			SHFileOperation(&FileOp);
		}
	}
}



void VI::ShowInitStatusBar()
{
	time(&nowtime);
	p = localtime(&nowtime);

	SystemStatus->setText("   "); 
	SystemStatus->setMinimumWidth(100);
	bandwidth->setText("带宽: 0.0Mbps");
	ProductionSpeed->setText("生产速度: 0个/秒");
	PLCStatus->setText("PLC连接状态: 未连接"); 
	DiskDFreeSpace->setText("D盘可用空间：--G");
	QCurrentSysTime->setText(QString("%0/%1/%2 %3:%4").arg(p->tm_year + 1900, 4, 10, QLatin1Char('0')).arg(p->tm_mon+1, 2, 10, QLatin1Char('0')).arg(p->tm_mday, 2, 10, QLatin1Char('0')).arg(p->tm_hour, 2, 10, QLatin1Char('0')).arg(p->tm_min, 2, 10, QLatin1Char('0')));
	DebugStatus->setText("调试模式");
	DebugStatus->setChecked(false);
	DebugStatus->setEnabled(false);
	DebugStatus->setVisible(false);
	// 将初始化的标签添加到底部状态栏上
	ui.statusBar->addWidget(SystemStatus);
	ui.statusBar->addWidget(bandwidth);
	ui.statusBar->addWidget(ProductionSpeed);
	ui.statusBar->addWidget(PLCStatus);
	ui.statusBar->addWidget(DiskDFreeSpace);

	ui.statusBar->addPermanentWidget(DebugStatus);
	ui.statusBar->addPermanentWidget(QCurrentSysTime);


}

void VI::ShowInitRecordBar()
{
	for (int i = 1; i < 4; i++)
	{
		/*记录栏 总数*/
		QString ControlsName = QString("lineEdit_sum%0").arg(i);
		auto Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls->setText("0");

		/*记录栏 合格数*/
		ControlsName = QString("lineEdit_OK%0").arg(i);
		Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls->setText("0");

		/*记录栏 NG数*/
		ControlsName = QString("lineEdit_NG%0").arg(i);
		Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls->setText("0");

		/*记录栏 合格率*/
		ControlsName = QString("lineEdit_acceptRate%0").arg(i);
		Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls->setText("0.0%");
	}
}



void VI::CloseVISystem()
{
	/*成功断开连接*/
	ui.action_stop->setChecked(false);
	ui.action_stop->setEnabled(true);
	DebugStatus->setEnabled(true);
}

void VI::begin_VI2(bool status)
{
	ui.action_stop->setEnabled(true);
}


void VI::begin_VI(bool status)
{
	if (status == true && start_grab_flag == true)
	{
		emit Begin_VI_signals();
	}
	else if((status == false && closeflag == false)|| (status == true && start_grab_flag == false && closeflag == false))
	{
		closeflag = true;
		emit Close_VI_signals();
	}
	
}

void VI::RestartRegularCheckSys(bool status)
{
	emit RestartRegularCheckSys();
}



//参数1-显示图像的Label，参数2-要显示的Mat
void VI::matToQimageLabelShow(QLabel* label, const cv::Mat mat)
{
	QImage Img = MatToQImage(mat);
	double label_height = label->size().height();
	double label_width = label->size().width();
	double label_ratio = label_height / label_width;
	double img_height = Img.height();
	double img_width = Img.width();
	double img_ratio = img_height / img_width;
	if (Img.isNull()|| Img.height() == 0 || Img.width() == 0)
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
		label->setPixmap(QPixmap::fromImage(Img.scaled(img_width*label_height/ img_height, label_height)));
		
	}
}


void VI::ShowImage(cv::Mat show_image, int pos, int order)
{
	if (pos == 0)
	{
		switch (order)
		{
		case 1:
		{
			show_image1 = show_image.clone();
			break;
		}
		case 2:
		{
			show_image2 = show_image.clone();
			break;
		}
		case 3:
		{
			show_image3 = show_image.clone();
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (pos == 1)
	{
		switch (order)
		{
		case 1:
		{
			InsideImage1 = show_image.clone();
			matToQimageLabelShow(ui.frame, InsideImage1);
			break;
		}
		case 2:
		{
			InsideImage2 = show_image.clone();
			matToQimageLabelShow(ui.frame_2, InsideImage2);
			break;
		}
		case 3:
		{
			InsideImage3 = show_image.clone();
			matToQimageLabelShow(ui.frame_3, InsideImage3);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (pos == 2)
	{
		switch (order)
		{
		case 2:
		{
			OutsideImage2 = show_image.clone();
			matToQimageLabelShow(ui.frame_5, OutsideImage2);
			break;
		}
		case 3:
		{
			OutsideImage3 = show_image.clone();
			matToQimageLabelShow(ui.frame_6, OutsideImage3);
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void VI::PackageErrorItem(cv::Mat InsideErrorImage, cv::Mat OutsideErrorImage, string InsideErrorData, string OutsideErrorData, int camera_type)
{
	ErrorImageItem item;
	item.InsideErrorImage = InsideErrorImage.clone();
	item.OutsideErrorImage = OutsideErrorImage.clone();
	
	item.InsideErrorData = QString(QString::fromLocal8Bit(InsideErrorData.c_str()));
	item.OutsideErrorData = QString(QString::fromLocal8Bit(OutsideErrorData.c_str()));

	PushbackDeque(item, camera_type);
}

void VI::PushbackDeque(ErrorImageItem item, int camera_type)
{
	if (ErrorImageDeque[camera_type - 1].size() < ListWidgetItemNumber)
	{
		ErrorImageDeque[camera_type - 1].push_back(item);
	}
	else
	{
		ErrorImageDeque[camera_type - 1].pop_front();
		ErrorImageDeque[camera_type - 1].push_back(item);
	}
}


void VI::PowerOff()
{
	PowerOff_Msgb = new QMessageBox;
	PowerOff_Msgb->setIcon(QMessageBox::Question);
	PowerOff_Msgb->setWindowTitle(" ");
	PowerOff_Msgb->setText(QString("确认关机"));
	PowerOff_Msgb->addButton(QMessageBox::Yes)->setText("确定");
	PowerOff_Msgb->addButton(QMessageBox::Cancel)->setText("取消");
	PowerOff_Msgb->setDefaultButton(QMessageBox::Cancel);
	int ret = PowerOff_Msgb->exec();
	if (ret == QMessageBox::Yes)
	{
		ui.listWidget->addItem("关机");
		system("shutdown -s -t 05");
		exit(0);
	}
}

void VI::show_PaintRoi_window()
{
	if (!PaintRoi_window->WindowIsOpened)
	{
		PaintRoi_window->get_index(1);
		PaintRoi_window->get_image(show_image1, show_image2, show_image3);
		PaintRoi_window->show();
	}
}

void VI::show_PaintRoi_window(int index)
{
	if (!PaintRoi_window->WindowIsOpened)
	{
		PaintRoi_window->get_index(index);
		PaintRoi_window->get_image(show_image1, show_image2, show_image3);
		PaintRoi_window->show();
	}
}

void VI::ShowSettingWindow()
{
	if (!settingwindow->WindowIsOpened)
	{
		settingwindow->show();
	}
}

void VI::show_Log_window(bool logstatus)
{
	ui.action_login->setChecked(false);
	if (!log_window->WindowIsOpened)
	{
		if (logstatus)
		{
			log_window->WindowIsOpened = true;
			log_window->GetLogInfoFromSQL();
			log_window->show();
		}
		else
		{
			ui.action_Roi->setEnabled(false);
			ui.action_setting->setEnabled(false);
			DebugStatus->setVisible(false);
			DebugStatus->setChecked(false);
			DebugFlag = false;
			vi_task->StopReconnect = false;
		}
	}

}

void VI::ShowMarkTheErrorWindow(QModelIndex item)
{
	QListWidget *listwidget = qobject_cast<QListWidget*>(sender());
	int listnumber = listwidget->objectName().right(1).toInt();
	if (ErrorImageDeque[listnumber - 1].size() < item.row() + 1)
	{
		MarkTheErrorWindow->hide();
		return;
	}

	cv::Mat Image1 = ErrorImageDeque[listnumber - 1].at(item.row()).InsideErrorImage;
	cv::Mat Image2 = ErrorImageDeque[listnumber - 1].at(item.row()).OutsideErrorImage;
	QString para1 = ErrorImageDeque[listnumber - 1].at(item.row()).InsideErrorData;
	QString para2 = ErrorImageDeque[listnumber - 1].at(item.row()).OutsideErrorData;
	
	if (para1 == "IrrelevantInfo")
	{
		MarkTheErrorWindow->hide();
		return;
	}

	/*不打开合格的照片*/
	if (Image1.empty() && Image2.empty())
	{
		return;
	}

	MarkTheErrorWindow->show();
	MarkTheErrorWindow->ShowErrorInfo(Image1, Image2, para1, para2);
	
}

void VI::show_HistoryData_window()
{
	if (!historydatawindow->WindowIsOpened)
	{
		historydatawindow->show();
	}
}

void VI::ResetRecordBar()
{
	/*获取信号发送者*/
	QPushButton* button = qobject_cast<QPushButton*>(sender());
	QString num = button->objectName().at(button->objectName().size() - 1);

	StatisticsRecord[num.toInt() - 1][0][0] = 0;
	StatisticsRecord[num.toInt() - 1][0][1] = 0;
	StatisticsRecord[num.toInt() - 1][1][0] = 0;
	StatisticsRecord[num.toInt() - 1][1][1] = 0;


	/*清空记录栏数据*/
	/*总数*/
	QString ControlsName = "lineEdit_sum" + num;
	auto Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
	Controls->setText("0");
	Yield[num.toInt()-1] = 0;

	/*合格数*/
	ControlsName = "lineEdit_OK" + num;
	Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
	Controls->setText("0");
	QualifiedProductNum[num.toInt()-1] = 0;

	/*NG数*/
	ControlsName = "lineEdit_NG" + num;
	Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
	Controls->setText("0");

	/*OK比例*/
	ControlsName = "lineEdit_acceptRate" + num;
	Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
	Controls->setText("0.0%");

	/*识别结果*/
	ControlsName = "lineEdit_recog" + num;
	Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
	Controls->setText("");

	/*清空日志栏*/
	ControlsName = "listWidget" + num;
	auto Controls2 = ui.centralWidget->findChild<QListWidget *>(ControlsName);
	Controls2->clear();

	/*清空标定错误数据集*/
	ErrorImageDeque[num.toInt() - 1].clear();
}


void VI::ShowSysStatus(int firststatus, int secondstatus)
{
	QTime SysCurrentTime = QTime::currentTime();//获取系统当前时间
	QString SysTime = QString("%0: %1  ").arg(SysCurrentTime.hour(), 2, 10, QLatin1Char('0')).arg(SysCurrentTime.minute(), 2, 10, QLatin1Char('0'));

	/******日志栏和错误栏******/
	switch (firststatus)
	{
	case VI_E_PLC_TCP:
	{
		if (secondstatus == 0)
		{
			ui.listWidget->addItem(SysTime + "TCP创建失败");
		}
		else if(secondstatus == -1)
		{
			ui.listWidget->addItem(SysTime + "PLC重连失败，请检查PLC连接状态");
		}
		else
		{
			ui.listWidget->addItem(SysTime + "TCP创建失败，进行第" + QString::number(secondstatus) + "次重连");
		}
		
		break;
	}
	case VI_E_PLC_DEBUG:
	{
		if (secondstatus == 0)
		{
			ui.listWidget->addItem(SysTime + "PLC调试模式设置失败");
		}
		else if (secondstatus == -1)
		{
			ui.listWidget->addItem(SysTime + "PLC重连失败，请检查PLC连接状态");
		}
		else
		{
			ui.listWidget->addItem(SysTime + "PLC调试模式设置失败，进行第" + QString::number(secondstatus) + "次重连");
		}
		break;
	}
	case VI_E_PLC_BUILD:
	{
		if (secondstatus == 0)
		{
			ui.listWidget->addItem(SysTime + "PLC连接失败");
		}
		else if (secondstatus == -1)
		{
			ui.listWidget->addItem(SysTime + "PLC重连失败，请检查PLC连接状态");
		}
		else
		{
			ui.listWidget->addItem(SysTime + "PLC连接失败，进行第" + QString::number(secondstatus) + "次重连");
		}
		
		break;
	}
	case VI_E_PLC_WRITE:
	{
		ui.listWidget->addItem(QString(SysTime + "写入PLC寄存器D%0失败").arg(900 + secondstatus));
		
		break;
	}
	case VI_E_PLC_READ:
	{
		ui.listWidget->addItem(QString(SysTime + "读取PLC寄存器D%0失败").arg(900 + secondstatus));
		
		break;
	}
	case VI_E_PLC_MALLOC:
	{
		ui.listWidget->addItem(SysTime + "分配内存空间失败");
		
		break;
	}
	case VI_E_PLC_REGISTERSPILE:
	{
		ui.listWidget->addItem(SysTime + "PLC数据包记录溢出");
		
		break;
	}
	case VI_E_PLC_UNKNOW:
	{
		ui.listWidget->addItem(SysTime + "PLC发生未知错误");
		
		break;
	}
	case VI_E_CAMERA_CREATEHANDLE:
	{
		ui.listWidget->addItem(SysTime + QString("创建相机%0句柄失败").arg(secondstatus));
		
		break;
	}
	case VI_E_CAMERA_DESTROYHANDLE:
	{
		ui.listWidget->addItem(SysTime + QString("摧毁相机%0句柄失败").arg(secondstatus));
		
		break;
	}
	case VI_E_CAMERA_OPEN:
	{
		ui.listWidget->addItem(SysTime + QString("相机%0打开失败").arg(secondstatus));

		Led("red", secondstatus);
		
		break;
	}
	case VI_E_CAMERA_CLOSE:
	{
		ui.listWidget->addItem(SysTime + QString("相机%0关闭失败").arg(secondstatus));
		
		break;
	}
	case VI_E_CAMERA_STARTGRAB:
	{
		ui.listWidget->addItem(SysTime + QString("相机%0取流失败").arg(secondstatus));
		
		break;
	}
	case VI_E_CAMERA_STOPGRAB:
	{
		ui.listWidget->addItem(SysTime + QString("相机%0停止取流失败").arg(secondstatus));
		
		break;
	}
	case VI_E_CAMERA_SAVE:
	{
		ui.listWidget->addItem(SysTime + QString("相机%0存图失败").arg(secondstatus));
		
		break;
	}
	case VI_E_CAMERA_IMAGEEMPTY:
	{
		ui.listWidget->addItem(SysTime + QString("相机%0读取到的图片为空").arg(secondstatus));
		
		break;
	}
	case VI_E_CAMERA_CALIBRATEROI:
	{
		ui.listWidget->addItem(SysTime + QString("相机1初始化校准失败，请清空纸槽"));
		
		break;
	}
	case VI_E_SET_HEARTBEAT:
	{
		ui.listWidget->addItem(SysTime + QString("启用相机%0心跳设置失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_HEARTBEATTIME:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0心跳时间失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_PACKETSIZE:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0最佳包失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_SCPD:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0传输延时失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_FPS:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0帧率失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_EXPOSURETIME:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0曝光时间失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_BLACKLEVELENABLE:
	{
		ui.listWidget->addItem(SysTime + QString("启用相机%0黑电平设置失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_BLACKLEVEL:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0黑电平值失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_DECIMATIONHORIZONTAL:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0水平方向下采样比例失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_DECIMATIONVERTICAL:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0垂直方向下采样比例失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_WIDTHMAX:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0图像最大宽度失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_HEIGHTMAX:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0图像最大高度失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_OFFSETX:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0X方向偏移量失败").arg(secondstatus));
		
		break;
	}
	case VI_E_SET_OFFSETY:
	{
		ui.listWidget->addItem(SysTime + QString("设置相机%0Y方向偏移量失败").arg(secondstatus));

		break;
	}

	case VI_SIGNAL_TOPHOTO:
	{
		//PLC发出拍照指令
		break;
	}
	case VI_SIGNAL_TOPROCESS:
	{
		//PLC发出处理图像指令
		break;
	}
	case VI_SIGNAL_PHOTOOVER:
	{
		//相机返回拍照结束信号
		break;
	}
	case VI_SIGNAL_PROCESSOVER:
	{
		//相机返回处理结束信号
		break;
	}
	case VI_PLC_ON:
	{
		ui.listWidget->addItem(SysTime + "PLC已建立连接");
		PLCStatus->setText("PLC连接状态: 已连接");//状态栏
		break;
	}
	case VI_PLC_OFF:
	{
		ui.listWidget->addItem(SysTime + "PLC已断开连接");
		PLCStatus->setText("PLC连接状态: 未连接");//状态栏
		break;
	}
	case VI_CAMERA_READY:
	{
		QString ControlsName = QString("listWidget%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QListWidget *>(ControlsName);
		Controls->addItem(SysTime + "相机已就绪");
		Led("green", secondstatus);

		ErrorImageItem item;
		item.InsideErrorData = "IrrelevantInfo";
		PushbackDeque(item, secondstatus);

		break;
	}
	case VI_CAMERA_PHOTO:
	{
		QString ControlsName = QString("listWidget%0").arg(secondstatus);//mytest
		auto Controls = ui.centralWidget->findChild<QListWidget *>(ControlsName);
		Controls->addItem(SysTime + "相机完成拍照");

		ErrorImageItem item;
		item.InsideErrorData = "IrrelevantInfo";
		PushbackDeque(item, secondstatus);

		break;
	}
	case VI_CAMERA_IMAGESAVE:
	{
		QString ControlsName = QString("listWidget%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QListWidget *>(ControlsName);
		Controls->addItem(SysTime + "图片存储成功");

		ErrorImageItem item;
		item.InsideErrorData = "IrrelevantInfo";
		PushbackDeque(item, secondstatus);

		break;
	}
	case VI_CAMERA_EXIT:
	{
		QString ControlsName = QString("listWidget%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QListWidget *>(ControlsName);
		Controls->addItem(SysTime + "相机已退出");
		Led("red", secondstatus);

		ErrorImageItem item;
		item.InsideErrorData = "IrrelevantInfo";
		PushbackDeque(item, secondstatus);

		break;
	}
	case VI_RECOG_AERR_BERR:
	{
		QString ControlsName = QString("listWidget%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QListWidget *>(ControlsName);
		if (secondstatus == 1)
		{
			Controls->addItem(SysTime + "工位A异常");
			Controls->item(Controls->count()-1)->setForeground(QBrush(QColor(255, 0, 0)));
		}
		else
		{
			Controls->addItem(SysTime + "工位AB异常");
			Controls->item(Controls->count() - 1)->setForeground(QBrush(QColor(255, 0, 0)));
		}

		ControlsName = QString("lineEdit_recog%0").arg(secondstatus);
		auto Controls2 = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		if (secondstatus == 1)
		{
			Controls2->setText("A:异常");
		}
		else
		{
			Controls2->setText("A:异常 B：异常");
		}

		if (!SaveDataFlag)
		{
			break;
		}

		/*设置边框颜色*/
		switch (secondstatus)
		{
		case 1:
		{
			ui.frame->setStyleSheet(BorderColorRed);
			StatisticsRecord[0][0][1]++;
			break;
		}
		case 2:
		{
			ui.frame_2->setStyleSheet(BorderColorRed);
			ui.frame_5->setStyleSheet(BorderColorRed);
			StatisticsRecord[1][0][1]++;
			StatisticsRecord[1][1][1]++;
			break;
		}
		case 3:
		{
			ui.frame_3->setStyleSheet(BorderColorRed);
			ui.frame_6->setStyleSheet(BorderColorRed);
			StatisticsRecord[2][0][1]++;
			StatisticsRecord[2][1][1]++;
			break;
		}
		default:
		{
			break;
		}
		}


		break;
	}
	case VI_RECOG_AOK_BERR:
	{
		QString ControlsName = QString("listWidget%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QListWidget *>(ControlsName);
		if (secondstatus == 1)
		{
			Controls->addItem(SysTime + "工位A正常");
			Controls->item(Controls->count() - 1)->setForeground(QBrush(QColor(0, 0, 0)));

			ErrorImageItem item;
			item.InsideErrorData = "IrrelevantInfo";
			PushbackDeque(item, secondstatus);
		}
		else
		{
			Controls->addItem(SysTime + "工位A正常 工位B异常");
			Controls->item(Controls->count() - 1)->setForeground(QBrush(QColor(255, 0, 0)));
		}

		ControlsName = QString("lineEdit_recog%0").arg(secondstatus);
		auto Controls2 = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		if (secondstatus == 1)
		{
			Controls2->setText("A:正常");
		}
		else
		{
			Controls2->setText("A:正常 B：异常");
		}

		if (!SaveDataFlag)
		{
			break;
		}

		/*设置边框颜色*/
		switch (secondstatus)
		{
		case 1:
		{
			ui.frame->setStyleSheet(BorderColorGreen);
			StatisticsRecord[0][0][0]++;
			break;
		}
		case 2:
		{
			ui.frame_2->setStyleSheet(BorderColorGreen);
			ui.frame_5->setStyleSheet(BorderColorRed);
			StatisticsRecord[1][0][0]++;
			StatisticsRecord[1][1][1]++;
			break;
		}
		case 3:
		{
			ui.frame_3->setStyleSheet(BorderColorGreen);
			ui.frame_6->setStyleSheet(BorderColorRed);
			StatisticsRecord[2][0][0]++;
			StatisticsRecord[2][1][1]++;
			break;
		}
		default:
		{
			break;
		}
		}

		break;
	}
	case VI_RECOG_AERR_BOK:
	{
		QString ControlsName = QString("listWidget%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QListWidget *>(ControlsName);
		Controls->addItem(SysTime + "工位A异常 工位B正常");
		Controls->item(Controls->count() - 1)->setForeground(QBrush(QColor(255, 0, 0)));

		ControlsName = QString("lineEdit_recog%0").arg(secondstatus);
		auto Controls2 = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls2->setText("A:异常 B：正常");

		if (!SaveDataFlag)
		{
			break;
		}

		/*设置边框颜色*/
		switch (secondstatus)
		{
		case 2:
		{
			ui.frame_2->setStyleSheet(BorderColorRed);
			ui.frame_5->setStyleSheet(BorderColorGreen);
			StatisticsRecord[1][0][1]++;
			StatisticsRecord[1][1][0]++;
			break;
		}
		case 3:
		{
			ui.frame_3->setStyleSheet(BorderColorRed);
			ui.frame_6->setStyleSheet(BorderColorGreen);
			StatisticsRecord[2][0][1]++;
			StatisticsRecord[2][1][0]++;
			break;
		}
		default:
		{
			break;
		}
		}

		break;
	}
	case VI_RECOG_AOK_BOK:
	{
		QString ControlsName = QString("listWidget%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QListWidget *>(ControlsName);
		Controls->addItem(SysTime + "工位AB正常");
		Controls->item(Controls->count() - 1)->setForeground(QBrush(QColor(0, 0, 0)));

		ControlsName = QString("lineEdit_recog%0").arg(secondstatus);
		auto Controls2 = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls2->setText("A:正常 B：正常");

		ErrorImageItem item;
		item.InsideErrorData = "IrrelevantInfo";
		PushbackDeque(item, secondstatus);

		if (!SaveDataFlag)
		{
			break;
		}

		/*设置边框颜色*/
		switch (secondstatus)
		{
		case 2:
		{
			ui.frame_2->setStyleSheet(BorderColorGreen);
			ui.frame_5->setStyleSheet(BorderColorGreen);
			StatisticsRecord[1][0][0]++;
			StatisticsRecord[1][1][0]++;
			break;
		}
		case 3:
		{
			ui.frame_3->setStyleSheet(BorderColorGreen);
			ui.frame_6->setStyleSheet(BorderColorGreen);
			StatisticsRecord[2][0][0]++;
			StatisticsRecord[2][1][0]++;
			break;
		}
		default:
		{
			break;
		}
		}

		break;
	}
	case VI_RECOG_PAPERUPSIDEDOWN:
	{
		StatisticsRecord[0][0][1]++;
		ui.listWidget->addItem(SysTime + QString("大卡放反了"));
		ui.frame->setStyleSheet(BorderColorRed);
		break;
	}
	case VI_RECOG_PAPERSLOTUNCLEAR:
	{
		StatisticsRecord[0][0][1]++;
		ui.listWidget1->addItem(SysTime + QString("纸槽未清空"));
		ui.listWidget1->item(ui.listWidget1->count() - 1)->setForeground(QBrush(QColor(255, 0, 0)));
		ui.frame->setStyleSheet(BorderColorRed);
		break;
	}
	default:
	{
		ui.listWidget->addItem(SysTime + QString("未知错误:%0").arg(firststatus));
		break;
	}
	}
	
	if (ui.listWidget->count() > abs(ListWidgetItemNumber/* - 5*/))
	{
		ui.listWidget->takeItem(0);;
	}
	if (ui.listWidget1->count() > ListWidgetItemNumber)
	{
		ui.listWidget1->takeItem(0);;
	}
	if (ui.listWidget2->count() > ListWidgetItemNumber)
	{
		ui.listWidget2->takeItem(0);;
	}
	if (ui.listWidget3->count() > ListWidgetItemNumber)
	{
		ui.listWidget3->takeItem(0);;
	}

	ui.listWidget->scrollToBottom();
	ui.listWidget1->scrollToBottom();
	ui.listWidget2->scrollToBottom();
	ui.listWidget3->scrollToBottom();

	/******记录栏******/
	/*记录栏 总数*/
	if (firststatus >= VI_RECOG_AERR_BERR && firststatus <= VI_RECOG_PAPERSLOTUNCLEAR && SaveDataFlag)
	{
		Yield[secondstatus - 1] += min(2, secondstatus);//相机1拍一次生产1个，相机2和3拍一次生产2个

		QString ControlsName = QString("lineEdit_sum%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls->setText(QString::number(Yield[secondstatus - 1]));
	}

	/*记录栏 合格数*/
	if (firststatus >= VI_RECOG_AOK_BERR && firststatus <= VI_RECOG_AERR_BOK && SaveDataFlag)
	{
		++QualifiedProductNum[secondstatus - 1];
		QString ControlsName = QString("lineEdit_OK%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls->setText(QString::number(QualifiedProductNum[secondstatus - 1]));
	}
	else if (firststatus == VI_RECOG_AOK_BOK && SaveDataFlag)
	{
		QualifiedProductNum[secondstatus - 1] += min(2, secondstatus);//相机1拍一次生产1个，相机2和3拍一次生产2个
		
		QString ControlsName = QString("lineEdit_OK%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls->setText(QString::number(QualifiedProductNum[secondstatus - 1]));
	}

	/*记录栏 NG数*/
	if (firststatus >= VI_RECOG_AERR_BERR && firststatus <= VI_RECOG_AOK_BOK || firststatus == VI_RECOG_PAPERUPSIDEDOWN || firststatus == VI_RECOG_PAPERSLOTUNCLEAR && SaveDataFlag)
	{
		QString ControlsName = QString("lineEdit_NG%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls->setText(QString::number(Yield[secondstatus - 1] - QualifiedProductNum[secondstatus - 1]));
	}

	/*记录栏 合格率*/
	if (firststatus >= VI_RECOG_AERR_BERR && firststatus <= VI_RECOG_AOK_BOK && SaveDataFlag)
	{
		QString ControlsName = QString("lineEdit_acceptRate%0").arg(secondstatus);
		auto Controls = ui.centralWidget->findChild<QLineEdit *>(ControlsName);
		Controls->setText(QString::number(((double)QualifiedProductNum[secondstatus - 1] / (double)Yield[secondstatus - 1])*100)+"%");
	}

	/******状态栏******/
	/*状态栏 系统状态*/
	if (firststatus / 1000 == 1)
	{
		SystemStatus->setText("发生错误");
	}
	else if (firststatus == VI_CAMERA_READY)
	{
		countReady++;
		if (countReady == 3)
		{
			countReady = 0;
			runningFlag = true;
			SystemStatus->setText("运行中 ");
		}
	}
	else if (runningFlag && firststatus == VI_PLC_OFF)
	{
		SystemStatus->setText("已暂停 ");
	}

}

void VI::Led(QString color, int secondstatus)
{
	QString ControlsName = QString("led%0").arg(secondstatus);
	auto Controls = ui.centralWidget->findChild<QLabel *>(ControlsName);
	Controls->setStyleSheet("min-width:20px;min-height:20px;max-width:20px;max-height:20px;border-radius:10px;border:1px solid black;background:" + color);
}

void VI::ShowCameraStatus(int status)
{
	/*相机1连接状态*/
	if (status & 1 == 1)
	{
		Led("green", 1);
	}
	else
	{
		Led("red", 1);
	}

	/*相机2连接状态*/
	if (status & 2 == 2)
	{
		Led("green", 2);
	}
	else
	{
		Led("red", 2);
	}

	/*相机3连接状态*/
	if (status & 4  == 4)
	{
		Led("green", 3);
	}
	else
	{
		Led("red", 3);
	}
}


void VI::RegularCheckSys(double ethernetbandwidth, double freespace)
{
	/*获取相机连接状态*/
	emit GetCameraConnectStatus();

	/*状态栏 带宽*/
	bandwidth->setText(QString("带宽: %0Mbps").arg(ethernetbandwidth));

	/*状态栏 生产速度*/
	static int LastYield = Yield[2];
	ProductionSpeed->setText(QString("生产速度: %0个/秒").arg((double)(Yield[2] - LastYield) / 5));
	LastYield = Yield[2];

	/*状态栏 D盘剩余空间*/
	DiskDFreeSpace->setText(QString("D盘可用空间：%0G").arg(freespace));

	if (freespace < 20)
	{
		QMessageBox::question(this, "", QString("D盘可用空间已不到%0，请尽快处理").arg(freespace));
	}

	time_t nowtime1 = time(NULL);
	struct tm* p1;
	p1 = localtime(&nowtime1);

	/*状态栏 系统时间*/
	QCurrentSysTime->setText(QString("%0/%1/%2 %3:%4").arg(p1->tm_year + 1900, 4, 10, QLatin1Char('0')).arg(p1->tm_mon + 1, 2, 10, QLatin1Char('0')).arg(p1->tm_mday, 2, 10, QLatin1Char('0')).arg(p1->tm_hour, 2, 10, QLatin1Char('0')).arg(p1->tm_min, 2, 10, QLatin1Char('0')));
	/*数据统计 时间（天）-检测点-工位-检测结果-数量*/
	char MYSQLStr[512] = {};
	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < 2;j++)
		{
			for (int k = 0;k < 2;k++)
			{
				sprintf_s(MYSQLStr, 512, "replace into `statistics` values('%04d%02d%02d','%d','%d','%d','%d');", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, i, j, k, StatisticsRecord[i][j][k]);
				InsertDataToMYSQLDatabase(MYSQLStr);
			}
		}
	}
}

/*向MYSQL数据库插入数据*/
void VI::InsertDataToMYSQLDatabase(const char *data)
{
	mysql_query(&DatabaseVI, data);
}

void VI::InsertDataToMYSQLDatabase(string data)
{
	mysql_query(&DatabaseVI, data.c_str());
}
void VI::GetTodayStatistics()
{
	QSqlQuery query;
	time_t nowtime2 = time(NULL);
	p = localtime(&nowtime2);
	QString sqlstr = QString("select * from statistics where date = %0%1%2; ").arg(p->tm_year + 1900, 4, 10, QLatin1Char('0')).arg(p->tm_mon + 1, 2, 10, QLatin1Char('0')).arg(p->tm_mday, 2, 10, QLatin1Char('0'));//查询语句
	query.prepare(sqlstr);//准备

	if (query.exec())
	{
		while (query.next())
		{
			int SQLValue[4] = { query.value(1).toInt() ,query.value(2).toInt() ,query.value(3).toInt() ,query.value(4).toInt() };
			if (SQLValue[0] == 0)
			{
				if (SQLValue[1] == 0)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[0][0][0] = SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[0][0][1] = SQLValue[3];
					}
				}
			}
			else if (SQLValue[0] == 1)
			{
				if (SQLValue[1] == 0)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[1][0][0] = SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[1][0][1] = SQLValue[3];
					}
				}
				else if (SQLValue[1] == 1)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[1][1][0] = SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[1][1][1] = SQLValue[3];
					}
				}
			}
			else if (SQLValue[0] == 2)
			{
				if (SQLValue[1] == 0)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[2][0][0] = SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[2][0][1] = SQLValue[3];
					}
				}
				else if (SQLValue[1] == 1)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[2][1][0] = SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[2][1][1] = SQLValue[3];
					}
				}
			}
		}
	}

	for (int i = 0;i < 3;i++)
	{
		Yield[i] = StatisticsRecord[i][0][0] + StatisticsRecord[i][0][1] + StatisticsRecord[i][1][0] + StatisticsRecord[i][1][1];
		QualifiedProductNum[i] = StatisticsRecord[i][0][0] + StatisticsRecord[i][1][0];
	}
}

