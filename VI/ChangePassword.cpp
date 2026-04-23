#include "ChangePassword.h"
#include <QMessageBox>

ChangePassword::ChangePassword(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	/*确认修改密码*/
	connect(ui.pushButton, &QPushButton::clicked, [=]()
	{
		if (ui.lineEdit->text() != user || ui.lineEdit_2->text() != password)
		{
			QMessageBox::critical(this, "", "账号或密码错误");
		}
		else if (ui.lineEdit_3->text() != ui.lineEdit_4->text())
		{
			QMessageBox::critical(this, "", "两次输入密码值不同");

		}
		else
		{
			QString str = QString("update account set password = '%0' where num = 1; ").arg(ui.lineEdit_3->text());
			mysql_query(&DatabaseVI, str.toStdString().c_str());
			QMessageBox::information(this, "", "修改密码成功");
			emit UpdateSuccess();

			close();
		}

	});

	/*取消修改*/
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));

	/*禁止缩放窗口*/
	setFixedSize(this->width(), this->height());

}

ChangePassword::~ChangePassword()
{
}

void ChangePassword::GetLogInfoFromSQL()
{
	mysql_init(&DatabaseVI);   //初始化MYSQL变量

	if (!mysql_real_connect(&DatabaseVI, "127.0.0.1", "root", "123456", "VI", 3306, NULL, 0))  //连接到mysql
	{
		QMessageBox::critical(this, "", "MYSQL连接失败");
	}

	if (mysql_query(&DatabaseVI, "SELECT * FROM account") != 0)   //若查询成功返回0，失败返回随机数
	{
		QMessageBox::critical(this, "", "MYSQL查询失败");
	}

	result = mysql_store_result(&DatabaseVI);    //将查询到的结果集储存到result中

	/*获取注册时存储的账号密码*/
	row = mysql_fetch_row(result);
	user = QString(QLatin1String(row[1]));//账号
	password = QString(QLatin1String(row[2]));//密码

}


void ChangePassword::closeEvent(QCloseEvent *event)
{
	mysql_free_result(result);     //释放结果集所占用的内存
	mysql_close(&DatabaseVI);          //关闭与mysql的连接
}