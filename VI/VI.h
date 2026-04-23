#pragma once

#include <QtWidgets/QMainWindow>
#include "show_camera_config_window.h"
#include "MarkTheError.h"
#include "MainRoiPage.h"
#include "ui_VI.h"
#include "VI_Task.h"
#include <Qthread>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <qevent.h>
#include <QMenu>
#include <QDesktopWidget>
#include <QCheckBox>
#include "show_log.h"
#include "HistoryDataRecord.h"
#include "Setting.h"
#include "VI_Camera.h"
#include "VI_Modbus.h"
#include "GetLocalPCInfo.h"
#include <mysql.h>
#include <io.h>
#include <direct.h>
#include <QSqlQuery>
#include "INI_Config.h"
#include <QMetaType>
#include <deque>

class VI : public QMainWindow
{
	Q_OBJECT

public:
	VI(QWidget *parent = nullptr);
	~VI();

private:
	Ui::VIClass ui;

	/*创建多线程*/
	VI_Task* vi_task = new VI_Task;
	QThread* vi_thread = new QThread;
	GetLocalPCInfo* getlocalpcinfo = new GetLocalPCInfo;
	QThread* vi_thread_2 = new QThread;

	/*初始化窗口函数*/
	show_log* log_window = new show_log;
	MainRoiPage* PaintRoi_window = new MainRoiPage;
	Setting* settingwindow = new Setting;
	HistoryDataRecord* historydatawindow = new HistoryDataRecord;
	QMessageBox* PowerOff_Msgb = NULL;
	MarkTheError* MarkTheErrorWindow = new MarkTheError;

	/*初始化状态栏参数*/
	QLabel *SystemStatus = new QLabel;
	QLabel * bandwidth = new QLabel;
	QLabel *ProductionSpeed = new QLabel;
	QLabel *PLCStatus = new QLabel;
	QLabel * DiskDFreeSpace = new QLabel;
	QLabel * QCurrentSysTime = new QLabel;
	QCheckBox *DebugStatus = new QCheckBox;

	struct ErrorImageItem
	{
		cv::Mat InsideErrorImage;
		cv::Mat OutsideErrorImage;

		QString InsideErrorData;
		QString OutsideErrorData;
	};

	/*标志位*/
	bool start_grab_flag = true;
	int countReady = 0;
	bool runningFlag = 0;
	bool closeflag = false;
	bool InitWorkHasDone = false;

	/*定义存储生产数据的数组*/
	int Yield[3] = { 0,0,0 };
	int QualifiedProductNum[3] = { 0,0,0 };
	int UnqualifiedProductNum[3] = { 0,0,0 };
	int StatisticsRecord[3][2][2] = { { {0, 0}, { 0,0 }}, { {0,0},{0,0} }, { {0,0},{0,0} } };//1.检测点2.内外侧3.ok/NG

	/*UI图片*/
	cv::Mat show_image1, show_image2, show_image3;
	cv::Mat InsideImage1, InsideImage2, InsideImage3;
	cv::Mat OutsideImage2, OutsideImage3;

	/*绘制相机ROI页面的索引*/
	int page_index;

	void matToQimageLabelShow(QLabel* label, cv::Mat show_image);
	void ShowInitStatusBar();
	void ShowInitRecordBar();
	void CreateAndDeleteImageFiles();//系统初次启动时创建存放图片的文件夹，以及删除过期的图片文件
	void Led(QString color, int camera_type);
	void GetTodayStatistics();
	void PushbackDeque(ErrorImageItem item, int camera_type);

	/*UI界面图片的外框颜色*/
	QString BorderColorRed = "background-color: rgb(0, 0, 0);color: rgb(255, 255, 255); border-width:2px; border-style: solid;border-color:red;";
	QString BorderColorGreen = "background-color: rgb(0, 0, 0);color: rgb(255, 255, 255); border-width:2px; border-style: solid;border-color:green;";

	/*数据格式转换*/
	LPCWSTR string2LPCWSTR(std::string str);
	QImage MatToQImage(const cv::Mat& mat);

	/*防止子窗口重复打开*/
	int* windows1 = NULL;

	/*时间*/
	time_t nowtime;
	struct tm* p;

	/*MYSQL*/
	MYSQL DatabaseVI;

	/*调试*/
	int ExpiredImages = 2;
	bool SaveDataFlag = true;


	/*读取配置文件*/
	INI_Config *ini = new INI_Config();

	/*数据库插入数据*/
	void InsertDataToMYSQLDatabase(const char *data);

	deque<ErrorImageItem> ErrorImageDeque[3];

	#define ListWidgetItemNumber 12//日志栏日志条数

public slots:
	void StartVISystem(bool start_msg);
	void CloseVISystem();

	void ShowImage(cv::Mat show_image, int pos, int order);

	void begin_VI(bool status);
	void begin_VI2(bool status);
	void RestartRegularCheckSys(bool status);
	void show_Log_window(bool logstatus);
	void show_HistoryData_window();
	void show_PaintRoi_window();
	void show_PaintRoi_window(int index);
	void ShowSysStatus(int firststatus, int secondstatus);
	void ShowCameraStatus(int status);
	void ShowMarkTheErrorWindow(QModelIndex item);
	void RegularCheckSys(double ethernetbandwidth, double freespace);//定时查看系统部分参数
	void ResetRecordBar();
	void ShowSettingWindow();
	void InsertDataToMYSQLDatabase(string data);
	void PowerOff();
	void PackageErrorItem(cv::Mat InsideErrorImage, cv::Mat OutsideErrorImage, string InsideErrorData, string OutsideErrorData, int camera_type);

signals:
	void Begin_VI_signals();
	void Init_VI_signals();
	void Close_VI_signals();
	void GetCameraConnectStatus();
	void RestartRegularCheckSys();
};
