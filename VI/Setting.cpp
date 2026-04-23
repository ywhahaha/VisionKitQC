#include "Setting.h"

Setting::Setting(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);//禁止最大化

	/*保存或取消设置*/
	connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(save_config()));
	connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_return, SIGNAL(clicked()), this, SLOT(close()));

	/*上级选项开启后，禁用下级选项*/
	connect(ui.checkBox, SIGNAL(toggled(bool)), this, SLOT(disable_GevHeartbeatTimeout_items(bool)));
	connect(ui.checkBox_2, SIGNAL(toggled(bool)), this, SLOT(disable_GevPacketSize_items(bool)));
	connect(ui.checkBox_3, SIGNAL(toggled(bool)), this, SLOT(disable_BlackLevel1_items(bool)));
	connect(ui.checkBox_4, SIGNAL(toggled(bool)), this, SLOT(disable_BlackLevel2_items(bool)));
	connect(ui.checkBox_5, SIGNAL(toggled(bool)), this, SLOT(disable_BlackLevel3_items(bool)));

	ui.tabWidget->setCurrentIndex(0);

	ShowInitialInterface();
}

Setting::~Setting()
{
}


void Setting::ShowInitialInterface()
{
	ini->ReadINI("config.ini");

	/******相机选项卡******/

	/*显示心跳设置状态，true为关闭心跳检测*/
	if (ini->GetValue("Camera", "SetGevHeartbeat") == "true")
	{
		ui.checkBox->setChecked(true);
		ui.label_2->setEnabled(false);
		ui.spinBox->setEnabled(false);
	}
	else
	{
		ui.checkBox->setChecked(false);
		ui.label_2->setEnabled(true);
		ui.spinBox->setEnabled(true);
	}


	ui.spinBox->setValue(stoi(ini->GetValue("Camera", "GevHeartbeatTimeout")));//显示心跳时间

	/*显示GEV传输包大小的获取方式（自动/手动）*/
	if (ini->GetValue("Camera", "GetGevPacketSizeAuto") == "true")
	{
		ui.checkBox_2->setChecked(true);
		ui.label_20->setEnabled(false);
		ui.spinBox_2->setEnabled(false);
	}
	else
	{
		ui.checkBox_2->setChecked(false);
		ui.label_20->setEnabled(true);
		ui.spinBox_2->setEnabled(true);
	}

	ui.spinBox_2->setValue(stoi(ini->GetValue("Camera", "GevPacketSize")));//显示GEV传输包大小
	ui.spinBox_3->setValue(stoi(ini->GetValue("Camera", "GevSCPD")));//显示GEV传输包延时
	ui.spinBox_4->setValue(stoi(ini->GetValue("Camera", "AcqusitionFrameRate1")));//显示相机1帧率
	ui.spinBox_6->setValue(stoi(ini->GetValue("Camera", "AcqusitionFrameRate2")));//显示相机2帧率
	ui.spinBox_7->setValue(stoi(ini->GetValue("Camera", "AcqusitionFrameRate3")));//显示相机3帧率
	ui.spinBox_8->setValue(stoi(ini->GetValue("Camera", "ExposureTime1")));//显示相机1曝光时间
	ui.spinBox_5->setValue(stoi(ini->GetValue("Camera", "ExposureTime2")));//显示相机2曝光时间
	ui.spinBox_9->setValue(stoi(ini->GetValue("Camera", "ExposureTime3")));//显示相机3曝光时间
	ui.spinBox_12->setValue(stoi(ini->GetValue("Camera", "Gain1")));//显示相机1增益
	ui.spinBox_30->setValue(stoi(ini->GetValue("Camera", "Gain2")));//显示相机2增益
	ui.spinBox_31->setValue(stoi(ini->GetValue("Camera", "Gain3")));//显示相机3增益
	/*显示相机1黑电平是否启动*/
	if (ini->GetValue("Camera", "BlackLevelEnable1") == "true")
	{
		ui.checkBox_3->setChecked(true);
		ui.label_32->setEnabled(true);
		ui.spinBox_10->setEnabled(true);
	}
	else
	{
		ui.checkBox_3->setChecked(false);
		ui.label_32->setEnabled(false);
		ui.spinBox_10->setEnabled(false);
	}

	/*显示相机2黑电平是否启动*/
	if (ini->GetValue("Camera", "BlackLevelEnable2") == "true")
	{
		ui.checkBox_4->setChecked(true);
		ui.label_33->setEnabled(true);
		ui.spinBox_11->setEnabled(true);
	}
	else
	{
		ui.checkBox_4->setChecked(false);
		ui.label_33->setEnabled(false);
		ui.spinBox_11->setEnabled(false);
	}

	/*显示相机3黑电平是否启动*/
	if (ini->GetValue("Camera", "BlackLevelEnable3") == "true")
	{
		ui.checkBox_5->setChecked(true);
		ui.label_36->setEnabled(true);
		ui.spinBox_13->setEnabled(true);
	}
	else
	{
		ui.checkBox_5->setChecked(false);
		ui.label_36->setEnabled(false);
		ui.spinBox_13->setEnabled(false);
	}

	ui.spinBox_10->setValue(stoi(ini->GetValue("Camera", "BlackLevel1")));//显示相机1黑电平值
	ui.spinBox_11->setValue(stoi(ini->GetValue("Camera", "BlackLevel2")));//显示相机2黑电平值
	ui.spinBox_13->setValue(stoi(ini->GetValue("Camera", "BlackLevel3")));//显示相机3黑电平值
	ui.comboBox->setCurrentText(QString::fromStdString(ini->GetValue("Camera", "Decimation_Camera1")));//显示相机1下采样比例
	ui.comboBox_2->setCurrentText(QString::fromStdString(ini->GetValue("Camera", "Decimation_Camera2")));//显示相机2下采样比例
	ui.comboBox_4->setCurrentText(QString::fromStdString(ini->GetValue("Camera", "Decimation_Camera3")));//显示相机3下采样比例

	/******IP选项卡******/
	ui.comboBox_5->setCurrentText(QString::fromStdString(ini->GetValue("Ethernet", "Port_Number")));//显示 网卡接口1名称
	ui.lineEdit_7->setText(QString::fromStdString(ini->GetValue("PLC", "IP")));//显示PLC  IP地址
	ui.lineEdit_2->setText(QString::fromStdString(ini->GetValue("PLC", "Port_Number")));//显示PLC 端口号
	ui.lineEdit_3->setText(QString::fromStdString(ini->GetValue("Camera", "IP1")));//显示相机1 IP地址
	ui.lineEdit_4->setText(QString::fromStdString(ini->GetValue("Camera", "IP2")));//显示相机2 IP地址
	ui.lineEdit_5->setText(QString::fromStdString(ini->GetValue("Camera", "IP3")));//显示相机3 IP地址


	/******产品瑕疵选项卡******/
	ui.doubleSpinBox->setValue(stod(ini->GetValue("Flaw", "PaperWidthUpperLimit")));//显示 试纸条宽度上限(mm)
	ui.doubleSpinBox_2->setValue(stod(ini->GetValue("Flaw", "PaperWidthLowerLimit")));//显示 试纸条宽度下限(mm)
	ui.doubleSpinBox_6->setValue(stod(ini->GetValue("Flaw", "PaperAngleUpperLimit")));//显示 试纸条角度上限(mm)
	ui.doubleSpinBox_7->setValue(stod(ini->GetValue("Flaw", "PaperAngleLowerLimit")));//显示 试纸条角度下限(mm)
	ui.doubleSpinBox_8->setValue(stod(ini->GetValue("Flaw", "PaperStainLength")));//显示 蛋白条污点长度(mm)
	ui.spinBox_15->setValue(stod(ini->GetValue("Flaw", "PaperStainNumber")));//显示 蛋白条污点数量
	ui.spinBox_16->setValue(stod(ini->GetValue("Flaw", "PaperStainColorDegree")));//显示 蛋白条脏污程度
	ui.doubleSpinBox_9->setValue(stod(ini->GetValue("Flaw", "PaperGoldenMarkLength")));//显示 金标线宽(mm)
	
	ui.doubleSpinBox_4->setValue(stod(ini->GetValue("Flaw", "BoxPaperAngleLeftUpperLimit")));//显示 试纸盒内纸条左倾角度(度)
	ui.doubleSpinBox_5->setValue(stod(ini->GetValue("Flaw", "BoxPaperAngleRightUpperLimit")));//显示 试纸盒内纸条右倾角度(度)
	ui.spinBox_17->setValue(stod(ini->GetValue("Flaw", "BoxPaperYPositionUpperLimit")));//显示 试纸盒内纸条偏移上限(mm)
	ui.spinBox_18->setValue(stod(ini->GetValue("Flaw", "BoxPaperYPositionLowerLimit")));//显示 试纸盒内纸条偏移下限(mm)

	ui.doubleSpinBox_3->setValue(stod(ini->GetValue("Flaw", "BoxGapAera")));//显示 试纸盒纸条缝隙面积(mm^2)

	/******其他选项卡******/
	ui.comboBox_15->setCurrentText(QString::fromStdString(ini->GetValue("Other", "SaveFrequency")));//显示 存图频次
	ui.spinBox_29->setValue(stoi(ini->GetValue("Other", "RetentionDay")));//显示 图片保留天数
	if (ini->GetValue("Other", "ResultInfo") == "true")
	{
		ui.checkBox_16->setChecked(true);
	}
	else
	{
		ui.checkBox_16->setChecked(false);
	}

	if (ini->GetValue("Other", "CameraInfo") == "true")
	{
		ui.checkBox_17->setChecked(true);
	}
	else
	{
		ui.checkBox_17->setChecked(false);
	}

	/******开发者选项卡******/
	ui.lineEdit->setText(QString::fromStdString(ini->GetValue("DeveloperOptions", "IP")));//显示 PLC测试机IP
	ui.lineEdit_6->setText(QString::fromStdString(ini->GetValue("DeveloperOptions", "Port_Number")));//显示 PLC测试机端口号
	ui.spinBox_14->setValue(stoi(ini->GetValue("DeveloperOptions", "DelayTime")));//显示 延迟时间(mm)
	ui.comboBox_6->setCurrentText(QString::fromStdString(ini->GetValue("DeveloperOptions", "Camera1Result")));//显示 相机1设定结果
	ui.comboBox_7->setCurrentText(QString::fromStdString(ini->GetValue("DeveloperOptions", "Camera2Result")));//显示 相机2设定结果
	ui.comboBox_8->setCurrentText(QString::fromStdString(ini->GetValue("DeveloperOptions", "Camera3Result")));//显示 相机3设定结果
	
	/*显示存图设置状态，true为存图*/
	if (ini->GetValue("DeveloperOptions", "SaveImage") == "true")
	{
		ui.checkBox_6->setChecked(true);
	}
	else
	{
		ui.checkBox_6->setChecked(false);
	}

	/*显示存数据设置状态，true为存数据*/
	if (ini->GetValue("DeveloperOptions", "SaveData") == "true")
	{
		ui.checkBox_7->setChecked(true);
	}
	else
	{
		ui.checkBox_7->setChecked(false);
	}

	ini->Clear();
}

/*保存属性*/
void Setting::save_config()
{
	ini->ReadINI("config.ini");
	CameraConfig();
	IPConfig();
	AdjustProductGrade();
	OtherSettings();
	DeveloperOptions();
	ini->WriteINI("config.ini");
}

void Setting::CameraConfig()
{
	/*设置心跳检测，true为关闭心跳检测*/
	if (ui.checkBox->isChecked())
	{
		ini->SetValue("Camera", "SetGevHeartbeat", "true");
	}
	else
	{
		ini->SetValue("Camera", "SetGevHeartbeat", "false");
	}

	/*设置相机心跳时间，在心跳时间内，若未收到 SDK 心跳回应，则将相机占用状态清除*/
	ini->SetValue("Camera", "GevHeartbeatTimeout", to_string(ui.spinBox->value()));

	/*设置GEV传输包大小的获取方式（自动/手动）*/
	if (ui.checkBox_2->isChecked())
	{
		ini->SetValue("Camera", "GetGevPacketSizeAuto", "true");
	}
	else
	{
		ini->SetValue("Camera", "GetGevPacketSizeAuto", "false");
	}

	ini->SetValue("Camera", "GevPacketSize", to_string(ui.spinBox_2->value()));
	ini->SetValue("Camera", "GevSCPD", to_string(ui.spinBox_3->value()));
	ini->SetValue("Camera", "AcqusitionFrameRate1", to_string(ui.spinBox_4->value()));
	ini->SetValue("Camera", "AcqusitionFrameRate2", to_string(ui.spinBox_6->value()));
	ini->SetValue("Camera", "AcqusitionFrameRate3", to_string(ui.spinBox_7->value()));
	ini->SetValue("Camera", "ExposureTime1", to_string(ui.spinBox_8->value()));
	ini->SetValue("Camera", "ExposureTime2", to_string(ui.spinBox_5->value()));
	ini->SetValue("Camera", "ExposureTime3", to_string(ui.spinBox_9->value()));
	ini->SetValue("Camera", "Gain1", to_string(ui.spinBox_12->value()));
	ini->SetValue("Camera", "Gain2", to_string(ui.spinBox_30->value()));
	ini->SetValue("Camera", "Gain3", to_string(ui.spinBox_31->value()));
	/*设置相机1黑电平是否启动*/
	if (ui.checkBox_3->isChecked())
	{
		ini->SetValue("Camera", "BlackLevelEnable1", "true");
	}
	else
	{
		ini->SetValue("Camera", "BlackLevelEnable1", "false");
	}

	/*设置相机2黑电平是否启动*/
	if (ui.checkBox_4->isChecked())
	{
		ini->SetValue("Camera", "BlackLevelEnable2", "true");
	}
	else
	{
		ini->SetValue("Camera", "BlackLevelEnable2", "false");
	}

	/*设置相机3黑电平是否启动*/
	if (ui.checkBox_5->isChecked())
	{
		ini->SetValue("Camera", "BlackLevelEnable3", "true");
	}
	else
	{
		ini->SetValue("Camera", "BlackLevelEnable3", "false");
	}

	ini->SetValue("Camera", "BlackLevel1", to_string(ui.spinBox_10->value()));
	ini->SetValue("Camera", "BlackLevel2", to_string(ui.spinBox_11->value()));
	ini->SetValue("Camera", "BlackLevel3", to_string(ui.spinBox_13->value()));
	ini->SetValue("Camera", "Decimation_Camera1", ui.comboBox->currentText().toStdString());
	ini->SetValue("Camera", "Decimation_Camera2", ui.comboBox_2->currentText().toStdString());
	ini->SetValue("Camera", "Decimation_Camera3", ui.comboBox_4->currentText().toStdString());

}

void Setting::IPConfig()
{
	/*设置使用哪个网卡*/
	ini->SetValue("Ethernet", "Port_Number", ui.comboBox_5->currentText().toStdString());
	
	/*设置PLC地址*/
	ini->SetValue("PLC", "IP", ui.lineEdit_7->text().toStdString());
	ini->SetValue("PLC", "Port_Number", ui.lineEdit_2->text().toStdString());

	/*设置相机IP地址*/
	ini->SetValue("Camera", "IP1", ui.lineEdit_3->text().toStdString());
	ini->SetValue("Camera", "IP2", ui.lineEdit_4->text().toStdString());
	ini->SetValue("Camera", "IP3", ui.lineEdit_5->text().toStdString());
}

void Setting::AdjustProductGrade()
{
	ini->SetValue("Flaw", "PaperWidthUpperLimit", to_string(ui.doubleSpinBox->value()));
	ini->SetValue("Flaw", "PaperWidthLowerLimit", to_string(ui.doubleSpinBox_2->value()));
	ini->SetValue("Flaw", "PaperAngleUpperLimit", to_string(ui.doubleSpinBox_6->value()));
	ini->SetValue("Flaw", "PaperAngleLowerLimit", to_string(ui.doubleSpinBox_7->value()));
	ini->SetValue("Flaw", "PaperStainLength", to_string(ui.doubleSpinBox_8->value()));
	ini->SetValue("Flaw", "PaperStainNumber", to_string(ui.spinBox_15->value()));
	ini->SetValue("Flaw", "PaperStainColorDegree", to_string(ui.spinBox_16->value()));
	ini->SetValue("Flaw", "PaperGoldenMarkLength", to_string(ui.doubleSpinBox_9->value()));

	ini->SetValue("Flaw", "BoxPaperAngleLeftUpperLimit", to_string(ui.doubleSpinBox_4->value()));
	ini->SetValue("Flaw", "BoxPaperAngleRightUpperLimit", to_string(ui.doubleSpinBox_5->value()));
	ini->SetValue("Flaw", "BoxPaperYPositionUpperLimit", to_string(ui.spinBox_17->value()));
	ini->SetValue("Flaw", "BoxPaperYPositionLowerLimit", to_string(ui.spinBox_18->value()));

	ini->SetValue("Flaw", "BoxGapAera", to_string(ui.doubleSpinBox_3->value()));
}

void Setting::OtherSettings()
{
	ini->SetValue("Other", "SaveFrequency", ui.comboBox_15->currentText().toStdString());
	ini->SetValue("Other", "RetentionDay", to_string(ui.spinBox_29->value()));

	if (ui.checkBox_16->isChecked())
	{
		ini->SetValue("Other", "ResultInfo", "true");
	}
	else
	{
		ini->SetValue("Other", "ResultInfo", "false");
	}

	if (ui.checkBox_17->isChecked())
	{
		ini->SetValue("Other", "CameraInfo", "true");
	}
	else
	{
		ini->SetValue("Other", "CameraInfo", "false");
	}
}

void Setting::DeveloperOptions()
{
	ini->SetValue("DeveloperOptions", "IP", ui.lineEdit->text().toStdString());
	ini->SetValue("DeveloperOptions", "Port_Number", ui.lineEdit_6->text().toStdString());
	ini->SetValue("DeveloperOptions", "DelayTime", to_string(ui.spinBox_14->value()));
	ini->SetValue("DeveloperOptions", "Camera1Result", ui.comboBox_6->currentText().toStdString());
	ini->SetValue("DeveloperOptions", "Camera2Result", ui.comboBox_7->currentText().toStdString());
	ini->SetValue("DeveloperOptions", "Camera3Result", ui.comboBox_8->currentText().toStdString());

	if (ui.checkBox_6->isChecked())
	{
		ini->SetValue("DeveloperOptions", "SaveImage", "true");
	}
	else
	{
		ini->SetValue("DeveloperOptions", "SaveImage", "false");
	}

	if (ui.checkBox_7->isChecked())
	{
		ini->SetValue("DeveloperOptions", "SaveData", "true");
	}
	else
	{
		ini->SetValue("DeveloperOptions", "SaveData", "false");
	}

}

/*上级选项开启后，禁用下级选项*/
void Setting::disable_GevHeartbeatTimeout_items(bool toggled)
{
	if (toggled)
	{
		ui.label_2->setEnabled(false);
		ui.spinBox->setEnabled(false);
	}
	else
	{
		ui.label_2->setEnabled(true);
		ui.spinBox->setEnabled(true);
	}
}

/*上级选项开启后，禁用下级选项*/
void Setting::disable_GevPacketSize_items(bool toggled)
{
	if (toggled)
	{
		ui.label_20->setEnabled(false);
		ui.spinBox_2->setEnabled(false);
	}
	else
	{
		ui.label_20->setEnabled(true);
		ui.spinBox_2->setEnabled(true);
	}
}

/*上级选项开启后，开启下级选项*/
void Setting::disable_BlackLevel1_items(bool toggled)
{
	if (toggled)
	{
		ui.label_32->setEnabled(true);
		ui.spinBox_10->setEnabled(true);
	}
	else
	{
		ui.label_32->setEnabled(false);
		ui.spinBox_10->setEnabled(false);
	}
}

/*上级选项开启后，开启下级选项*/
void Setting::disable_BlackLevel2_items(bool toggled)
{
	if (toggled)
	{
		ui.label_33->setEnabled(true);
		ui.spinBox_11->setEnabled(true);
	}
	else
	{
		ui.label_33->setEnabled(false);
		ui.spinBox_11->setEnabled(false);
	}
}

/*上级选项开启后，开启下级选项*/
void Setting::disable_BlackLevel3_items(bool toggled)
{
	if (toggled)
	{
		ui.label_36->setEnabled(true);
		ui.spinBox_13->setEnabled(true);
	}
	else
	{
		ui.label_36->setEnabled(false);
		ui.spinBox_13->setEnabled(false);
	}
}

void Setting::closeEvent(QCloseEvent *event)
{
	bool WindowIsOpened = false;
}

void Setting::OpenDateAndTimeSettingPanel()
{
	system("start ms-settings:dateandtime");
}