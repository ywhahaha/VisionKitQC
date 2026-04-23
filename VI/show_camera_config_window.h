#pragma once

#include <QMainWindow>
#include "ui_show_camera_config_window.h"
#include "MvCameraControl.h"
#include "INI_Config.h"

class show_camera_config_window : public QMainWindow
{
	Q_OBJECT

public:
	show_camera_config_window(QWidget *parent = nullptr);
	~show_camera_config_window();

private:
	Ui::show_camera_config_windowClass ui;

	void PLC_Config();
	void Camera_Config();
	void Show_Config();

	int nRet = MV_OK;
	void* handle = NULL;

	INI_Config *ini = new INI_Config();

public slots:
	void save_config();
	void disable_GevHeartbeatTimeout_items(bool toggled);
	void disable_GevPacketSize_items(bool toggled);
	void disable_BlackLevel1_items(bool toggled);
	void disable_BlackLevel2_items(bool toggled);
	void disable_BlackLevel3_items(bool toggled);
};
