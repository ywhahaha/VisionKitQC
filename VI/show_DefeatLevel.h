#pragma once

#include <QWidget>
#include "ui_show_DefeatLevel.h"

class show_DefeatLevel : public QWidget
{
	Q_OBJECT

public:
	show_DefeatLevel(QWidget *parent = Q_NULLPTR);
	~show_DefeatLevel();

private:
	Ui::show_DefeatLevel ui;
};
