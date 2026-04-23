#include "HistoryDataRecord.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <qdebug.h>
#include <qsqlerror.h>
#include <string>
#include <qmessagebox.h>
#include <iostream>
# pragma execution_character_set("utf-8")

QSqlDatabase db;
QSqlQuery query;

HistoryDataRecord::HistoryDataRecord(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//********数据库设置***********//
	//创建一个自己的数据库 然后利用QT与数据库建立连接
	db = QSqlDatabase::addDatabase("QMYSQL");//使用的数据库类型 MYSQL
	db.setHostName("localhost");//默认
	db.setPort(3306);//默认
	db.setDatabaseName("VI");//数据库名 需自己在MYSQL创建一个名为test的database
	db.setUserName("root");//用户名 根据安装MYSQL来
	db.setPassword("123456");//密码  根据安装MYSQL来

	if (db.open())
	{
		query = QSqlQuery(db);//将query指令与数据库绑定
	}
	else 
	{
		QString SQLlastError = db.lastError().text();
		QMessageBox::critical(this, "", "Error:" + SQLlastError);
	}


	this->showMaximized();
	setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);//禁止最大化窗口

	ui.frame_2->setAlignment(Qt::AlignCenter);

	ui.dateTimeEdit->setDate(QDate::currentDate());
	ui.dateTimeEdit_2->setDate(QDate::currentDate().addDays(1));
	ui.dateTimeEdit->setCalendarPopup(true);
	ui.dateTimeEdit_2->setCalendarPopup(true);

}

HistoryDataRecord::~HistoryDataRecord()
{
}


void HistoryDataRecord::QueryingHistory()
{
	QString SelectDate = "";
	QString SelectCheckpoint = "";
	QString SelectResult = "";

	/*日期筛选*/
	QString StartData = ui.dateTimeEdit->text();
	QString EndData = ui.dateTimeEdit_2->text();

	char array[] = { ':',' ','/' }; //需要去除的字符
	int length = sizeof(array) / sizeof(char);
	for (int i = 0; i < length; i++)
	{
		QString tmp = QString(array[i]);
		if (StartData.contains(tmp))
		{
			StartData = StartData.replace(tmp, "");
		}
		if (EndData.contains(tmp))
		{
			EndData = EndData.replace(tmp, "");
		}
	}
	StartData.append("00");
	EndData.append("00");

	SelectDate = QString("where date > %0 and date < %1").arg(StartData).arg(EndData);


	/*检测点筛选*/
	switch (ui.comboBox_5->currentIndex())
	{
	case 0:
	{
		SelectCheckpoint = " and checkpoint = '1'";
		break;
	}
	case 1:
	{
		SelectCheckpoint = " and checkpoint = '2'";
		break;
	}
	case 2:
	{
		SelectCheckpoint = " and checkpoint = '3'";
		break;
	}
	default:
	{
		SelectCheckpoint = " ";
		break;
	}
	}


	/*结果筛选*/
	switch (ui.comboBox_5->currentIndex())
	{
	case 0:
	{
		switch (ui.comboBox_6->currentIndex())
		{
		case 0:
		{
			SelectResult = " and result = '正常';";
			break;
		}
		case 1:
		{
			SelectResult = " and result = '下';";
			break;
		}
		default:
		{
			SelectResult = "?;";//超出界限则不返回结果
			break;
		}
		}
		break;
	}
	case 1:
	{
		switch (ui.comboBox_6->currentIndex())
		{
		case 0:
		{
			SelectResult = " and result = '正常';";
			break;
		}
		case 1:
		{
			SelectResult = " and result = '右';";
			break;
		}
		case 2:
		{
			SelectResult = " and result = '左';";
			break;
		}
		case 3:
		{
			SelectResult = " and result = '左右';";
			break;
		}
		default:
		{
			SelectResult = "?;";
			break;
		}
		}
		break;
	}	
	case 2:
	{
		switch (ui.comboBox_6->currentIndex())
		{
		case 0:
		{
			SelectResult = " and result = '正常';";
			break;
		}
		case 1:
		{
			SelectResult = " and result = '左';";
			break;
		}
		case 2:
		{
			SelectResult = " and result = '右';";
			break;
		}
		case 3:
		{
			SelectResult = " and result = '左右';";
			break;
		}
		default:
		{
			SelectResult = "?;";
			break;
		}
		}
		break;
	}
	}

	QString sqlstr ="select date, checkpoint, result, filename from historydata " + SelectDate + SelectCheckpoint + SelectResult;//查询语句
	query.prepare(sqlstr);//准备
	int i = 0;

	ui.tableWidget->clear();
	if (query.exec())
	{
		//依次读取查询到的记录
		while (query.next())
		{
			ui.tableWidget->setRowCount(i + 1);//设置表格行数
			QString date = query.value(0).toString();
			date.insert(4, "-");
			date.insert(7, "-");
			date.insert(10, " ");
			date.insert(13, ":");
			date = date.left(date.size() - 4);
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(date));
			ui.tableWidget->item(i, 0)->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
			  
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
			ui.tableWidget->item(i, 1)->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
			  
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
			ui.tableWidget->item(i, 2)->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
			  
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
			ui.tableWidget->item(i, 3)->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

			i++;
		}
	}
	if (ui.tableWidget->rowCount() == 0)
	{
		QMessageBox::question(this, "", "未查询到数据");
	}
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //x先自适应宽度
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);     //然后设置要根据内容使用宽度的列
}


void HistoryDataRecord::QueryingStatistics()
{
	QString SelectDate = "";
	int StatisticsRecord[3][2][2] = { { {0, 0}, { 0,0 }}, { {0,0},{0,0} }, { {0,0},{0,0} } };//1.检测点2.内外侧3.ok/NG

	/*日期筛选*/
	QString StartData = ui.dateTimeEdit->text();
	QString EndData = ui.dateTimeEdit_2->text();

	char array[] = { ':',' ','/' }; //需要去除的字符
	int length = sizeof(array) / sizeof(char);
	for (int i = 0; i < length; i++)
	{
		QString tmp = QString(array[i]);
		if (StartData.contains(tmp))
		{
			StartData = StartData.replace(tmp, "");
		}
		if (EndData.contains(tmp))
		{
			EndData = EndData.replace(tmp, "");
		}
	}

	StartData = StartData.left(StartData.size() - 4);
	EndData = EndData.left(EndData.size() - 4);

	SelectDate = QString("where date >= %0 and date < %1").arg(StartData).arg(EndData);

	QString sqlstr = "select * from statistics " + SelectDate;//查询语句
	query.prepare(sqlstr);//准备

	ui.textBrowser->clear();
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
						StatisticsRecord[0][0][0] += SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[0][0][1] += SQLValue[3];
					}
				}
				else if (SQLValue[1] == 1)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[0][1][0] += SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[0][1][1] += SQLValue[3];
					}
				}
			}
			else if (SQLValue[0] == 1)
			{
				if (SQLValue[1] == 0)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[1][0][0] += SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[1][0][1] += SQLValue[3];
					}
				}
				else if (SQLValue[1] == 1)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[1][1][0] += SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[1][1][1] += SQLValue[3];
					}
				}
			}
			else if (SQLValue[0] == 2)
			{
				if (SQLValue[1] == 0)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[2][0][0] += SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[2][0][1] += SQLValue[3];
					}
				}
				else if (SQLValue[1] == 1)
				{
					if (SQLValue[2] == 0)
					{
						StatisticsRecord[2][1][0] += SQLValue[3];
					}
					else if (SQLValue[2] == 1)
					{
						StatisticsRecord[2][1][1] += SQLValue[3];
					}
				}
			}
		}
	}
	int data = 0;
	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < 2;j++)
		{
			for (int k = 0;k < 2;k++)
			{
				data += StatisticsRecord[i][j][k];
			}
		}
	}
	if (data == 0)
	{
		ui.textBrowser->append("该时间段无数据记录");
		return;
	}

	ui.textBrowser->append(QString("%0至%1期间的数据如下").arg(ui.dateTimeEdit->text()).arg(ui.dateTimeEdit_2->text()));
	for (int i = 0;i < 3;i++)
	{
		ui.textBrowser->append(QString("检测点%0生产总数：%1").arg(i + 1).arg(StatisticsRecord[i][0][0] + StatisticsRecord[i][0][1] + StatisticsRecord[i][1][0] + StatisticsRecord[i][1][1]));
		ui.textBrowser->append(QString("检测点%0良品率：%1%").arg(i + 1).arg((double)(StatisticsRecord[i][0][0] + StatisticsRecord[i][1][0])/ (StatisticsRecord[i][0][0] + StatisticsRecord[i][0][1] + StatisticsRecord[i][1][0] + StatisticsRecord[i][1][1]) * 100));
		for (int j = 0;j < 2;j++)
		{
			ui.textBrowser->append(QString("工位%0(OK)：%1").arg(j + 1).arg(StatisticsRecord[i][j][0]));
			ui.textBrowser->append(QString("工位%0(NG)：%1").arg(j + 1).arg(StatisticsRecord[i][j][1]));
			ui.textBrowser->append(QString("工位%0良品率：%1%").arg(j + 1).arg((double)StatisticsRecord[i][j][0] / (StatisticsRecord[i][j][0] + StatisticsRecord[i][j][1]) * 100));

			if (i == 0)
			{
				break;
			}
		}
		ui.textBrowser->append("");
	}
}


void HistoryDataRecord::show_picture(QTableWidgetItem* item)
{
	int rows = item->row();
	QString Imagepath = ui.tableWidget->item(rows, 3)->text();

	QImage *image = new QImage(Imagepath);
	double label_height = ui.frame_2->size().height();
	double label_width = ui.frame_2->size().width();
	double label_ratio = label_height / label_width;
	double img_height = image->height();
	double img_width = image->width();
	double img_ratio = img_height / img_width;

	if (image->isNull())
	{
		ui.frame_2->setText("图像已丢失");
		return;
	}

	if (label_ratio > img_ratio)
	{
		ui.frame_2->setPixmap(QPixmap::fromImage(image->scaled(label_width, img_height*label_width / img_width)));
	}
	else
	{
		ui.frame_2->setPixmap(QPixmap::fromImage(image->scaled(img_width*label_height / img_height, label_height)));
	}
}

void HistoryDataRecord::closeEvent(QCloseEvent *event)
{
	bool WindowIsOpened = false;
}
