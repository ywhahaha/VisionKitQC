#pragma once
#include <QWidget>
#include "ui_HistoryDataRecord.h"
#include <qstring.h>

class HistoryDataRecord : public QWidget
{
	Q_OBJECT

public:
	HistoryDataRecord(QWidget *parent = Q_NULLPTR);
	~HistoryDataRecord();
	bool WindowIsOpened = false;

protected:
	void closeEvent(QCloseEvent *event);

private:
	Ui::HistoryDataRecord ui;


	int PageMaxItem = 0;

signals:
	void ThisWindowIsClosed();

public slots:
	void QueryingHistory();
	void QueryingStatistics();
	void show_picture(QTableWidgetItem* item);
};
