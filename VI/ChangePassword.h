#pragma once

#include <QWidget>
#include "ui_ChangePassword.h"
#include <mysql.h>
# pragma execution_character_set("utf-8")

class ChangePassword : public QWidget
{
	Q_OBJECT

public:
	ChangePassword(QWidget *parent = Q_NULLPTR);
	~ChangePassword();
	void GetLogInfoFromSQL();

protected:
	void closeEvent(QCloseEvent *event);


private:
	Ui::ChangePassword ui;

	MYSQL DatabaseVI;
	MYSQL_RES *result;//MYSQL查询的结果数据集
	MYSQL_ROW row;//结果数据集中的一行

	QString user;//注册时的账号
	QString password;//注册时的密码

signals:
	void UpdateSuccess();
};
