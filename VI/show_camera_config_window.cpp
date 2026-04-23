#pragma once
#include "show_camera_config_window.h"
#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include <string>
#include"Get_Local_IP.h"
#include <direct.h>
#include <io.h>

show_camera_config_window::show_camera_config_window(QWidget *parent)
	: QMainWindow(parent)
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

	Show_Config();
}

show_camera_config_window::~show_camera_config_window()
{}

/*显示读取的属性*/
void show_camera_config_window::Show_Config()
{
	ini->ReadINI("config.ini");

	ui.comboBox_3->setCurrentText(QString::fromStdString(ini->GetValue("PLC", "IP")));//显示PLC  IP地址
	ui.lineEdit_2->setText(QString::fromStdString(ini->GetValue("PLC", "Port_Number")));//显示PLC 端口号
	ui.lineEdit_3->setText(QString::fromStdString(ini->GetValue("Camera", "IP1")));//显示相机1 IP地址
	ui.lineEdit_4->setText(QString::fromStdString(ini->GetValue("Camera", "IP2")));//显示相机2 IP地址
	ui.lineEdit_5->setText(QString::fromStdString(ini->GetValue("Camera", "IP3")));//显示相机3 IP地址

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
}

/*保存属性*/
void show_camera_config_window::save_config()
{
	PLC_Config();
	Camera_Config();
	ini->WriteINI("config.ini");
}



/*写PLC IP地址到config.ini文件中*/
void show_camera_config_window::PLC_Config()
{
	ini->SetValue("PLC", "IP", ui.comboBox_3->currentText().toStdString());
	ini->SetValue("PLC", "Port_Number", ui.lineEdit_2->text().toStdString());
}

/*写相机属性到config.ini文件中*/
void show_camera_config_window::Camera_Config()
{
	/*设置相机IP地址*/
	ini->SetValue("Camera", "IP1", ui.lineEdit_3->text().toStdString());
	ini->SetValue("Camera", "IP2", ui.lineEdit_4->text().toStdString());
	ini->SetValue("Camera", "IP3", ui.lineEdit_5->text().toStdString());

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

/*上级选项开启后，禁用下级选项*/
void show_camera_config_window::disable_GevHeartbeatTimeout_items(bool toggled)
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
void show_camera_config_window::disable_GevPacketSize_items(bool toggled)
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
void show_camera_config_window::disable_BlackLevel1_items(bool toggled)
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
void show_camera_config_window::disable_BlackLevel2_items(bool toggled)
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
void show_camera_config_window::disable_BlackLevel3_items(bool toggled)
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

