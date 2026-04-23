#pragma once

#include <QWidget>
#include "ui_login.h"
#include <QCloseEvent>
#include <mysql.h>


class show_log : public QWidget
{
	Q_OBJECT

public:
	show_log(QWidget *parent = Q_NULLPTR);
	void GetLogInfoFromSQL();
	bool WindowIsOpened = false;

private:
	Ui::login ui;
	bool LoggingStatus;//登录状态

	MYSQL DatabaseVI;
	MYSQL_RES *result;//MYSQL查询的结果数据集
	MYSQL_ROW row;//结果数据集中的一行

	QString user;//注册时的账号
	QString user_Rem;//记住的账号
	QString password;//注册时的密码
	QString password_Rem;//记住的密码
	QString RemFlag;//是否记住密码

	void MysqlQuery();

protected:
	void closeEvent(QCloseEvent *event);

signals:
	void CloseLogWindow(bool status);
	void ThisWindowIsClosed();
};
