#pragma once

#include <QWidget>
#include "ui_Setting.h"
#include "MvCameraControl.h"
#include "INI_Config.h"

class Setting : public QWidget
{
	Q_OBJECT

public:
	Setting(QWidget *parent = Q_NULLPTR);
	~Setting();
	bool WindowIsOpened = false;

protected:
	void closeEvent(QCloseEvent *event);

private:
	Ui::Setting ui;

	void CameraConfig();//相机属性配置界面
	void IPConfig();//IP配置界面
	void AdjustProductGrade();//产品瑕疵调整界面
	void OtherSettings();//其他设置界面
	void DeveloperOptions();//开发者选项界面
	void ShowInitialInterface();//显示初始设置界面
	int nRet = MV_OK;
	void* handle = NULL;

	INI_Config *ini = new INI_Config();

signals:
	void ThisWindowIsClosed();

private slots:
	void save_config();
	void disable_GevHeartbeatTimeout_items(bool toggled);
	void disable_GevPacketSize_items(bool toggled);
	void disable_BlackLevel1_items(bool toggled);
	void disable_BlackLevel2_items(bool toggled);
	void disable_BlackLevel3_items(bool toggled);
	void OpenDateAndTimeSettingPanel();

};