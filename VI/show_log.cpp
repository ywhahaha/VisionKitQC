#include "show_log.h"
#include <QMessageBox>
#include <iostream>
#include "ChangePassword.h"

show_log::show_log(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ChangePassword* ChangePasswordWindow = new ChangePassword;//修改密码窗口

	/*确定登录*/
	connect(ui.pushButton, &QPushButton::clicked, [=]()
	{
		if (ui.lineEdit->text() == user && ui.lineEdit_2->text() == password)
		{
			LoggingStatus = true;
			emit CloseLogWindow(true);
			close();
		}
		else
		{
			QMessageBox::critical(this, "", "账号或密码错误");
		}
	});

	/*取消登录*/
	connect(ui.pushButton_2, &QPushButton::clicked, [=]() 
	{
		close();
	});

	/*修改密码*/
	connect(ui.pushButton_3, &QPushButton::clicked, [=]()
	{
		ChangePasswordWindow->GetLogInfoFromSQL();
		ChangePasswordWindow->show();
	});

	/*修改密码成功后重新刷新数据*/
	connect(ChangePasswordWindow, &ChangePassword::UpdateSuccess, [=]()
	{
		MysqlQuery();
	});

	/*禁止缩放窗口*/
	setFixedSize(this->width(), this->height());
}

void show_log::closeEvent(QCloseEvent *event)
{

	/*记住密码*/
	if (!ui.checkBox->isChecked())
	{
		std::string str = "update account set user = '' where num = 2; ";
		mysql_query(&DatabaseVI, str.c_str());
		str = "update account set password = '' where num = 2; ";
		mysql_query(&DatabaseVI, str.c_str());
		str = "update account set remember = '0' where num = 2; ";
		mysql_query(&DatabaseVI, str.c_str());
	}
	else
	{
		QString str = QString("update account set user = '%0' where num = 2; ").arg(ui.lineEdit->text());
		mysql_query(&DatabaseVI, str.toStdString().c_str());
		str = QString("update account set password = '%0' where num = 2; ").arg(ui.lineEdit_2->text());
		mysql_query(&DatabaseVI, str.toStdString().c_str());
		str = QString("update account set remember = '1' where num = 2; ");
		mysql_query(&DatabaseVI, str.toStdString().c_str());
	}


	WindowIsOpened = false;

	mysql_free_result(result);     //释放结果集所占用的内存
	mysql_close(&DatabaseVI);          //关闭与mysql的连接
}

void show_log::GetLogInfoFromSQL()
{
	mysql_init(&DatabaseVI);   //初始化MYSQL变量

	if (!mysql_real_connect(&DatabaseVI, "127.0.0.1", "root", "123456", "VI", 3306, NULL, 0))  //连接到mysql
	{
		QMessageBox::critical(this, "", "MYSQL连接失败");
	}

	MysqlQuery();
}

void show_log::MysqlQuery()
{
	if (mysql_query(&DatabaseVI, "SELECT * FROM account") != 0)   //若查询成功返回0，失败返回随机数
	{
		QMessageBox::critical(this, "", "MYSQL查询失败");
	}

	result = mysql_store_result(&DatabaseVI);    //将查询到的结果集储存到result中

	/*获取注册时存储的账号密码*/
	row = mysql_fetch_row(result);
	user = QString(QLatin1String(row[1]));//账号
	password = QString(QLatin1String(row[2]));//密码

	/*获取记住时存储的账号密码*/
	row = mysql_fetch_row(result);
	user_Rem = QString(QLatin1String(row[1]));//账号
	password_Rem = QString(QLatin1String(row[2]));//密码
	RemFlag = QString(QLatin1String(row[3]));//是否记住密码

	ui.lineEdit->setText(user_Rem);
	ui.lineEdit_2->setText(password_Rem);
	if (RemFlag == '1')
	{
		ui.checkBox->setChecked(true);
	}
	else
	{
		ui.checkBox->setChecked(false);
	}
}

