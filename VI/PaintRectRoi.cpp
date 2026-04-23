#pragma once
#include "PaintRectRoi.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QStandardPaths>
#include <QFileDialog>
#include <iostream>
#include <string.h>
#include <qdebug.h>
#include "messagetips.h"

PaintRectRoi::PaintRectRoi(QWidget *parent)
	: QLabel(parent)
{
	this->initViewer();
}


PaintRectRoi::~PaintRectRoi()
{
}

void PaintRectRoi::initViewer()
{
	m_bPainterPressed = false;
	m_bMovedPressed = false;
	m_bScalePressed = false;
	m_roiRect[0] = QRect(0, 0, 0, 0);
	m_roiRect[1] = QRect(0, 0, 0, 0);
	m_roiRect_union = QRect(0, 0, 0, 0);
	m_emCurDir = EmDirection::DIR_NONE;

	this->setMouseTracking(true);
	this->setFocusPolicy(Qt::StrongFocus);

	m_pOptMenu = new QMenu(this);
	m_pDelAction = new QAction("删除", this);
	connect(m_pDelAction, &QAction::triggered, this, [&]() { m_roiRect[AreaNumber] = QRect(0, 0, 0, 0); });
	m_pSaveAction = new QAction("保存", this);
	connect(m_pSaveAction, &QAction::triggered, this, &PaintRectRoi::saveROIImage);

	m_pOptMenu->addAction(m_pDelAction);
	m_pOptMenu->addAction(m_pSaveAction);
}

/**
 * @brief				保存子区域的roi图形
 *
 * @return				void
 */
void PaintRectRoi::saveROIImage()
{
	if (m_roiRect[AreaNumber].width() * 4 / decimation < 188 || m_roiRect[AreaNumber].height() * 4 / decimation < 160)
	{
		QMessageBox::critical(this, "", "子区域的ROI区域的宽度必须大于376pt，高度必须大于320pt");
		return;
	}

	ini->ReadINI("config.ini");

	ini->SetValue("Camera", "Camera" + to_string(page_index) + "_Roi_OffsetX_" + to_string(AreaNumber), to_string(m_roiRect[AreaNumber].x() * 4 / decimation));
	ini->SetValue("Camera", "Camera" + to_string(page_index) + "_Roi_OffsetY_" + to_string(AreaNumber), to_string(m_roiRect[AreaNumber].y() * 4 / decimation));
	ini->SetValue("Camera", "Camera" + to_string(page_index) + "_Roi_Width_" + to_string(AreaNumber), to_string(m_roiRect[AreaNumber].width() * 4 / decimation));
	ini->SetValue("Camera", "Camera" + to_string(page_index) + "_Roi_Height_" + to_string(AreaNumber), to_string(m_roiRect[AreaNumber].height() * 4 / decimation));

	ini->WriteINI("config.ini");
	ini->Clear();
}

/**
 * @brief				保存联合区域roi图形
 *
 * @return				void
 */
void PaintRectRoi::saveUnionROIImage()
{
	/*联合区域的ROI区域的宽度必须大于376pt，高度必须大于320pt*/
	if (m_roiRect_union.width() * 4 / decimation < 376 || m_roiRect_union.height() * 4 / decimation < 320)
	{
		QMessageBox::critical(this, "", "联合区域的ROI区域的宽度必须大于376pt，高度必须大于320pt");
		return;
	}

	/*不允许子区域超出联合区域*/
	if ((m_roiRect[0] & m_roiRect_union) != m_roiRect[0] || (m_roiRect[1] & m_roiRect_union) != m_roiRect[1])
	{
		QMessageBox::critical(this, "", "不允许子区域超出联合区域");
		return;
	}

	ini->ReadINI("config.ini");

	ini->SetValue("Camera", "Camera" + to_string(page_index) + "_Roi_OffsetX_Union", to_string(m_roiRect_union.x() * 4 / decimation));
	ini->SetValue("Camera", "Camera" + to_string(page_index) + "_Roi_OffsetY_Union", to_string(m_roiRect_union.y() * 4 / decimation));
	ini->SetValue("Camera", "Camera" + to_string(page_index) + "_Roi_Width_Union", to_string(m_roiRect_union.width() * 4 / decimation));
	ini->SetValue("Camera", "Camera" + to_string(page_index) + "_Roi_Height_Union", to_string(m_roiRect_union.height() * 4 / decimation));

	ini->WriteINI("config.ini");
	ini->Clear();

	MessageTips *mMessageTips = new MessageTips("成功保存", this);
	mMessageTips->show();
}

void PaintRectRoi::GetSpinBoxChangedValue(int value, int number)
{
	value = value / 4 * decimation;

	QRect temproiRect[2] = { m_roiRect[0],m_roiRect[1] };

	switch (number)
	{
	case 1:
	{
		m_roiRect[0].setX(value);
		m_roiRect[0].setWidth(temproiRect[0].width());
		break;
	}
	case 2:
	{
		m_roiRect[0].setY(value);
		m_roiRect[0].setHeight(temproiRect[0].height());
		break;
	}
	case 3:
	{
		m_roiRect[0].setWidth(value);
		break;
	}
	case 4:
	{
		m_roiRect[0].setHeight(value);
		break;
	}
	case 5:
	{
		m_roiRect[1].setX(value);
		m_roiRect[1].setWidth(temproiRect[1].width());

		break;
	}
	case 6:
	{
		m_roiRect[1].setY(value);
		m_roiRect[1].setHeight(temproiRect[1].height());
		break;
	}
	case 7:
	{
		m_roiRect[1].setWidth(value);
		break;
	}
	case 8:
	{
		m_roiRect[1].setHeight(value);
		break;
	}
	default:
	{
		break;
	}
	}

	update();
	
}


/**
 * @brief				获取绘制的矩形
 *
 * @return				绘制的矩形
 */
QRect PaintRectRoi::getRoiRect(int area) const
{
	return m_roiRect[area];
}


/**
 * @brief				展示子区域的初始的ROI
 * @param img        	页面编号index
 *
 * @return				void
 */
void PaintRectRoi::ShowInitRoi(int index)
{
	/*获取保存的ROI区域*/
	ini->ReadINI("config.ini");

	decimation = stoi(ini->GetValue("Camera", "Decimation_Camera" + to_string(index)));
	for (int i = 0; i < 2; i++)
	{
		OffsetX = stoi(ini->GetValue("Camera", "Camera" + to_string(index) + "_Roi_OffsetX_" + to_string(i))) / 4 * decimation;
		OffsetY = stoi(ini->GetValue("Camera", "Camera" + to_string(index) + "_Roi_OffsetY_" + to_string(i))) / 4 * decimation;
		ImageWidth = stoi(ini->GetValue("Camera", "Camera" + to_string(index) + "_Roi_Width_" + to_string(i))) / 4 * decimation;
		ImageHeight = stoi(ini->GetValue("Camera", "Camera" + to_string(index) + "_Roi_Height_" + to_string(i))) / 4 * decimation;

		m_roiRect[i] = QRect(OffsetX, OffsetY, ImageWidth, ImageHeight);
	}

	ini->Clear();
	/*获取传入的页面编号*/
	page_index = index;

	/*允许展示ROI*/
	m_bShowInitROI = true;
}


/**
 * @brief				展示联合区域的初始的ROI
 * @param img        	void
 *
 * @return				void
 */
void PaintRectRoi::ShowInitUnionRoi()
{
	/*获取保存的ROI区域*/
	ini->ReadINI("config.ini");
	
	OffsetX = stoi(ini->GetValue("Camera", "Camera" + to_string(page_index) + "_Roi_OffsetX_Union")) / 4 * decimation;
	OffsetY = stoi(ini->GetValue("Camera", "Camera" + to_string(page_index) + "_Roi_OffsetY_Union")) / 4 * decimation;
	ImageWidth = stoi(ini->GetValue("Camera", "Camera" + to_string(page_index) + "_Roi_Width_Union")) / 4 * decimation;
	ImageHeight = stoi(ini->GetValue("Camera", "Camera" + to_string(page_index) + "_Roi_Height_Union")) / 4 * decimation;

	m_roiRect_union = QRect(OffsetX, OffsetY, ImageWidth, ImageHeight);

	ini->Clear();

	/*允许展示ROI*/
	m_bShowUnionROI = true;
}

/**
 * @brief				删除联合区域的ROI
 * @param img        	void
 *
 * @return				void
 */
void PaintRectRoi::DeleteUnionRoi()
{
	m_roiRect_union = QRect(0, 0, 0, 0);

	/*取消允许展示ROI*/
	m_bShowUnionROI = false;

	update();
}

void PaintRectRoi::ClearScreen()
{
	m_roiRect[0] = QRect(0, 0, 0, 0);
	m_roiRect[1] = QRect(0, 0, 0, 0);
	m_roiRect_union = QRect(0, 0, 0, 0);

	m_bShowInitROI = false;
	update();
}


/**
 * @brief				画图函数的入口
 * @param img        	绘制区域area
 *
 * @return				void
 */
void PaintRectRoi::startPaint(int area)
{
	AreaNumber = area;
	m_bPainterAllowed = true;
}


/**
 * @brief				生成联合区域ROI
 * @param img        	void
 *
 * @return				void
 */
void PaintRectRoi::CreateUnionRoi()
{
	m_bShowUnionROI = true;

	/*完成子区域的绘制后才允许生成联合区域*/
	if (m_roiRect[0] == QRect(0, 0, 0, 0) || m_roiRect[1] == QRect(0, 0, 0, 0))
	{
		QMessageBox::critical(this, "", "请完成子区域的绘制后再生成联合区域");
	}

	/*先生成原始尺寸的ROI，转换成相机可接受的参数后，再转换成展示界面尺寸的ROI*/
	m_roiRect_union.setX(min(m_roiRect[0].x(), m_roiRect[1].x()) * 4 / decimation);
	m_roiRect_union.setY(min(m_roiRect[0].y(), m_roiRect[1].y()) * 4 / decimation);
	m_roiRect_union.setWidth(max(m_roiRect[0].x() + m_roiRect[0].width(), m_roiRect[1].x() + m_roiRect[1].width()) * 4 / decimation - m_roiRect_union.x());
	m_roiRect_union.setHeight(max(m_roiRect[0].y() + m_roiRect[0].height(), m_roiRect[1].y() + m_roiRect[1].height()) * 4 / decimation - m_roiRect_union.y());
	
	m_roiRect_union.setX(m_roiRect_union.x() - m_roiRect_union.x() % 2);
	m_roiRect_union.setY(m_roiRect_union.y() - m_roiRect_union.y() % 2);
	m_roiRect_union.setWidth((m_roiRect_union.width() + 7) / 8 * 8);
	m_roiRect_union.setHeight((m_roiRect_union.height() + 7) / 8 * 8);
	
	int temp_width = m_roiRect_union.width();//临时保存宽高，防止改变x的时候宽高也跟着变化
	int temp_height = m_roiRect_union.height();

	m_roiRect_union.setX(m_roiRect_union.x() / 4 * decimation);
	m_roiRect_union.setY(m_roiRect_union.y() / 4 * decimation);
	m_roiRect_union.setWidth(temp_width / 4 * decimation);
	m_roiRect_union.setHeight(temp_height / 4 * decimation);

	update();
}


/**
 * @brief				绘制矩形
 * @param event         绘图事件
 *
 * @return				void
 */
void PaintRectRoi::paintEvent(QPaintEvent * event)
{
	QLabel::paintEvent(event);

	if (!m_bShowInitROI)return;

	if (m_roiRect[AreaNumber].x() < 0)
	{
		m_roiRect[AreaNumber].setWidth(m_roiRect[AreaNumber].width() - m_roiRect[AreaNumber].x());
		m_roiRect[AreaNumber].setX(0);
	}
	if (m_roiRect[AreaNumber].y() < 0)
	{
		m_roiRect[AreaNumber].setHeight(m_roiRect[AreaNumber].height() - m_roiRect[AreaNumber].y());
		m_roiRect[AreaNumber].setY(0);
	}

	//m_roiRect[AreaNumber].setX(m_roiRect[AreaNumber].x() - m_roiRect[AreaNumber].x() % 2);
	//m_roiRect[AreaNumber].setY(m_roiRect[AreaNumber].y() - m_roiRect[AreaNumber].y() % 2);

	emit(getRoiRect0(m_roiRect[0], decimation));//把矩阵信息发送给控制面板，实现实时更新数据
	emit(getRoiRect1(m_roiRect[1], decimation));
	emit(getRoiRectUnion(m_roiRect_union, decimation));


	/*设置画笔参数*/
	QPainter painter;
	painter.begin(this);
	QPen pen;
	QFont font;


	/*绘制当前操作的区域*/
	for (int i = 0; i < 2; i++)
	{
		if (m_roiRect[i] != QRect(0, 0, 0, 0))
		{
			pen.setColor(QColor(255, 255, 0, 190));
			pen.setWidth(EDGE_WIDTH);
			painter.setPen(pen);

			font.setPixelSize(16);
			font.setBold(true);
			painter.setFont(font);

			painter.drawRect(m_roiRect[i]);

			painter.drawRect(m_roiRect[i].topLeft().x() - POINT_WIDTH / 2, m_roiRect[i].topLeft().y() - POINT_HEIGHT / 2, POINT_WIDTH, POINT_HEIGHT); //左上角
			painter.drawRect(m_roiRect[i].bottomLeft().x() - POINT_WIDTH / 2, m_roiRect[i].bottomLeft().y() - POINT_WIDTH / 2, POINT_WIDTH, POINT_HEIGHT); //左下角
			painter.drawRect(m_roiRect[i].topRight().x() - POINT_WIDTH / 2, m_roiRect[i].topRight().y() - POINT_HEIGHT / 2, POINT_WIDTH, POINT_HEIGHT); //右上角
			painter.drawRect(m_roiRect[i].bottomRight().x() - POINT_WIDTH / 2, m_roiRect[i].bottomRight().y() - POINT_WIDTH / 2, POINT_WIDTH, POINT_HEIGHT); //右下角点

			/*增加文字提示*/
			//QString strPoint = QString("X:%0, Y:%1").arg(m_roiRect[i].x() * 4 / decimation).arg(m_roiRect[i].y() * 4 / decimation);         //位置信息
			//QString strSize = QString("W:%0, H:%1").arg(m_roiRect[i].width() * 4 / decimation).arg(m_roiRect[i].height() * 4 / decimation);   //大小信息
			QString strArea = "区域" + QString("%0").arg(i);

			painter.drawText(m_roiRect[i].topLeft().x()+4, m_roiRect[i].topLeft().y() + 19, strArea);
			//painter.drawText(m_roiRect[i].bottomLeft().x(), m_roiRect[i].bottomLeft().y() + 28, strPoint);
		}
	}

	if (m_bShowUnionROI)
	{
		pen.setStyle(Qt::DotLine);
		pen.setColor(QColor(0, 255, 0, 255));
		pen.setWidth(EDGE_WIDTH);
		painter.setPen(pen);
		painter.drawRect(m_roiRect_union);

		QString strArea = "联合区域";
		painter.drawText(m_roiRect_union.center().x()- painter.fontMetrics().width(strArea)/2, m_roiRect_union.center().y(), strArea);
	}

	painter.end();
}

/**
 * @brief				鼠标按下事件 用于拖动绘制缩放起始
 * @param ev	        鼠标事件
 *
 * @return				void
 */
void PaintRectRoi::mousePressEvent(QMouseEvent * ev)
{
	QPoint mousepoint = MousePointConvert(ev->pos());

	if (ev->buttons() & Qt::LeftButton && m_bPainterAllowed)
	{
		EmDirection dir = region(mousepoint);     //获取鼠标当前的位置

		if (dir == DIR_MIDDLE)
		{
			//鼠标在矩形中心位置
			this->setCursor(Qt::ClosedHandCursor);
			m_moveStartPoint.setX(mousepoint.x());
			m_moveStartPoint.setY(mousepoint.y());
			m_bMovedPressed = true;
		}
		else if (dir == DIR_NONE)
		{
			//鼠标在矩形外部
			this->setCursor(Qt::ArrowCursor);
			m_bPainterPressed = true;
			m_paintStartPoint.setX(mousepoint.x());
			m_paintStartPoint.setY(mousepoint.y());

		}
		else
		{
			//矩形在矩形边缘
			m_moveStartPoint.setX(mousepoint.x());
			m_moveStartPoint.setY(mousepoint.y());
			m_bScalePressed = true;
			m_emCurDir = dir;
		}
	}
}

/**
 * @brief				鼠标移动事件
 * @param ev         	鼠标事件
 *
 * @return				void
 */
void PaintRectRoi::mouseMoveEvent(QMouseEvent * ev)
{
	QPoint mousepoint = MousePointConvert(ev->pos());
	if (m_bPainterAllowed)
	{
		if (ev->buttons() & Qt::LeftButton)
		{
			if (m_bPainterPressed)
			{
				//正在绘制状态
				paintRect(mousepoint);
			}
			else if (m_bMovedPressed)
			{
				//正在移动状态
				moveRect(mousepoint);

			}
			else if (m_bScalePressed)
			{
				//正在缩放大小状态
				scaleRect(mousepoint);

			}

			//更新界面
			update();
			return;
		}

		//根据鼠标的位置设置当前的鼠标形状
		EmDirection dir = region(mousepoint);

		if (dir == DIR_NONE)
		{
			setCursor(Qt::ArrowCursor);
		}
		else if (dir == DIR_MIDDLE)
		{
			setCursor(Qt::OpenHandCursor);
		}
	}
}

/**
 * @brief				鼠标松开事件
 * @param ev	        鼠标事件
 *
 * @return				void
 */
void PaintRectRoi::mouseReleaseEvent(QMouseEvent * ev)
{
	QPoint mousepoint = MousePointConvert(ev->pos());

	//判断鼠标是否在矩形中
	if (m_roiRect[AreaNumber].contains(mousepoint))
	{
		//松开鼠标前是否正在拖放
		if (m_bMovedPressed)
		{
			this->setCursor(Qt::OpenHandCursor);
		}
		else
		{
			this->setCursor(Qt::ArrowCursor);
		}
	}

	m_paintStartPoint = QPoint();
	m_bMovedPressed = false;
	m_bPainterPressed = false;
	m_bScalePressed = false;
}

/**
 * @brief				右键菜单
 * @param ev	        菜单事件
 *
 * @return				void
 */
void PaintRectRoi::contextMenuEvent(QContextMenuEvent * ev)
{
	QPoint mousePos = ev->pos();

	if (m_roiRect[AreaNumber].contains(mousePos))
	{
		m_pOptMenu->exec(QCursor::pos());
	}

	ev->accept();
}

/**
 * @brief				判断鼠标的位置
 * @param point         鼠标的位置
 *
 * @return				EmDirection 方向
 */
EmDirection PaintRectRoi::region(const QPoint & point)
{
	int mouseX = point.x();
	int mouseY = point.y();

	QPoint roiTopLeft = m_roiRect[AreaNumber].topLeft();
	QPoint roiBottomRight = m_roiRect[AreaNumber].bottomRight();

	EmDirection dir = DIR_NONE;

	if (mouseX <= roiTopLeft.x() + CORPADDING && mouseX >= roiTopLeft.x() && mouseY <= roiTopLeft.y() + CORPADDING && mouseY >= roiTopLeft.y())
	{
		//左上角
		this->setCursor(Qt::SizeFDiagCursor);
		dir = DIR_LEFTTOP;
	}
	else if (mouseX >= roiBottomRight.x() - CORPADDING && mouseX < roiBottomRight.x() && mouseY <= roiTopLeft.y() + CORPADDING && mouseY >= roiTopLeft.y())
	{
		//右上角
		this->setCursor(Qt::SizeBDiagCursor);
		dir = DIR_RIGHTTOP;
	}
	else if (mouseX <= roiTopLeft.x() + CORPADDING && mouseX >= roiTopLeft.x() && mouseY >= roiBottomRight.y() - CORPADDING && mouseY <= roiBottomRight.y())
	{
		//左下角
		this->setCursor(Qt::SizeBDiagCursor);
		dir = DIR_LEFTBOTTOM;
	}
	else if (mouseX >= roiBottomRight.x() - CORPADDING && mouseX < roiBottomRight.x() && mouseY >= roiBottomRight.y() - CORPADDING && mouseY <= roiBottomRight.y())
	{
		//右下角
		this->setCursor(Qt::SizeFDiagCursor);
		dir = DIR_RIGHTBOTTOM;
	}
	else if (mouseX >= roiBottomRight.x() - EDGPADDING && mouseX <= roiBottomRight.x() && mouseY >= roiTopLeft.y() && mouseY <= roiBottomRight.y())
	{
		//右边
		this->setCursor(Qt::SizeHorCursor);
		dir = DIR_RIGHT;
	}
	else if (mouseY <= roiTopLeft.y() + EDGPADDING && mouseY >= roiTopLeft.y() && mouseX >= roiTopLeft.x() && mouseX <= roiBottomRight.x())
	{
		//上边
		this->setCursor(Qt::SizeVerCursor);
		dir = DIR_TOP;
	}
	else if (mouseY >= roiBottomRight.y() - EDGPADDING && mouseY <= roiBottomRight.y() && mouseX >= roiTopLeft.x() && mouseX <= roiBottomRight.x())
	{
		//下边
		this->setCursor(Qt::SizeVerCursor);
		dir = DIR_BOTTOM;
	}
	else if (mouseX <= roiTopLeft.x() + EDGPADDING && mouseX >= roiTopLeft.x() && mouseY >= roiTopLeft.y() && mouseY <= roiBottomRight.y())
	{
		//左边
		this->setCursor(Qt::SizeHorCursor);
		dir = DIR_LEFT;
	}
	else if (m_roiRect[AreaNumber].contains(point))
	{
		//中间
		dir = DIR_MIDDLE;
	}
	else
	{
		dir = DIR_NONE;
	}

	return dir;
}

/**
 * @brief				缩放矩形
 * @param mousePoint    鼠标的位置
 *
 * @return				void
 */
void PaintRectRoi::scaleRect(const QPoint & mousePoint)
{
	QRect newRect(m_roiRect[AreaNumber].topLeft(), m_roiRect[AreaNumber].bottomRight());
	int width = mousePoint.x() - m_moveStartPoint.x();   //移动的宽度
	int height = mousePoint.y() - m_moveStartPoint.y();  //移动的高度

	//根据当前的缩放状态来改变矩形的位置大小信息
	switch (m_emCurDir)
	{
	case DIR_LEFT:
		newRect.setLeft(mousePoint.x());
		break;
	case DIR_RIGHT:
		newRect.setRight(mousePoint.x());
		break;
	case DIR_TOP:
		newRect.setTop(mousePoint.y());
		break;
	case DIR_BOTTOM:
		newRect.setBottom(mousePoint.y());
		break;
	case DIR_LEFTTOP:
		newRect.setTopLeft(mousePoint);
		break;
	case DIR_LEFTBOTTOM:
		newRect.setBottomLeft(mousePoint);
		break;
	case DIR_RIGHTTOP:
		newRect.setTopRight(mousePoint);
		break;
	case DIR_RIGHTBOTTOM:
		newRect.setBottomRight(mousePoint);
		break;
	}

	if (newRect.width() < MIN_WIDTH || newRect.height() < MIN_HEIGHT)
	{
		//缩放的大小限制
		return;
	}

	m_roiRect[AreaNumber] = newRect;
	m_moveStartPoint = mousePoint;  //更新鼠标的起始位置
}

/**
 * @brief				绘制矩形
 * @param mousePoint    鼠标的位置
 *
 * @return				void
 */
void PaintRectRoi::paintRect(const QPoint &mousePoint)
{
	this->setCursor(Qt::ArrowCursor);                    //设置鼠标为指针形状

	int width = mousePoint.x() - m_paintStartPoint.x();  //移动的宽度
	int height = mousePoint.y() - m_paintStartPoint.y(); //移动的高度

	if (width < 0 && height < 0)
	{
		//鼠标向左上角移动
		m_roiRect[AreaNumber].setX(mousePoint.x());
		m_roiRect[AreaNumber].setY(mousePoint.y());
	}
	else if (width < 0)
	{
		//鼠标向负X位置移动
		m_roiRect[AreaNumber].setX(mousePoint.x());
		m_roiRect[AreaNumber].setY(m_paintStartPoint.y());
	}
	else if (height < 0)
	{
		//鼠标向负Y位置移动
		m_roiRect[AreaNumber].setX(m_paintStartPoint.x());
		m_roiRect[AreaNumber].setY(mousePoint.y());
	}
	else
	{
		//正常  向右下移动
		m_roiRect[AreaNumber].setX(m_paintStartPoint.x());
		m_roiRect[AreaNumber].setY(m_paintStartPoint.y());
	}

	/*width和height的步进值为8*/
	/*设置矩形大小 绝对值 避免反方向的产生的负值*/
	width = abs(width) - abs(width) % (2 * decimation);
	height = abs(height) - abs(height) % (2 * decimation);
	m_roiRect[AreaNumber].setSize(QSize(abs(width), abs(height)));
}

/**
 * @brief				拖动矩形
 * @param mousePoint    鼠标的位置
 *
 * @return				void
 */
void PaintRectRoi::moveRect(const QPoint & mousePoint)
{
	this->setCursor(Qt::ClosedHandCursor);

	int width, height;
	if (mousePoint.x() > 0)
	{
		width = mousePoint.x() - m_moveStartPoint.x();
	}
	else
	{
		width = 0;
	}
	if (mousePoint.y() > 0)
	{
		height = mousePoint.y() - m_moveStartPoint.y();
	}
	else
	{
		height = 0;
	}

	QRect ret;
	if (m_roiRect[AreaNumber].x() + m_roiRect[AreaNumber].width() + width <= IMAGE_WINDOW_WIDTH)
	{
		ret.setX(m_roiRect[AreaNumber].x() + width);
	}
	else
	{
		ret.setX(IMAGE_WINDOW_WIDTH - m_roiRect[AreaNumber].width());
	}

	if (m_roiRect[AreaNumber].y() + m_roiRect[AreaNumber].height() + height <= IMAGE_WINDOW_HEIGHT)
	{
		ret.setY(m_roiRect[AreaNumber].y() + height);
	}
	else
	{
		ret.setY(IMAGE_WINDOW_HEIGHT - m_roiRect[AreaNumber].height());
	}


	ret.setSize(m_roiRect[AreaNumber].size());

	m_roiRect[AreaNumber] = ret;
	m_moveStartPoint = mousePoint;
}

QPoint PaintRectRoi::MousePointConvert(QPoint ev_pos)
{
	QPoint mousepoint = ev_pos;
	if (mousepoint.x() <= 0)
	{
		mousepoint.setX(0);
	}
	else if (mousepoint.x() > IMAGE_WINDOW_WIDTH)
	{
		mousepoint.setX(IMAGE_WINDOW_WIDTH);
	}

	if (mousepoint.y() <= 0)
	{
		mousepoint.setY(0);
	}
	else if (mousepoint.y() > IMAGE_WINDOW_HEIGHT)
	{
		mousepoint.setY(IMAGE_WINDOW_HEIGHT);
	}
	return mousepoint;
}

QRect PaintRectRoi::ChangetoStdRoi(QRect stdroi)
{
	QRect newroi = stdroi;
	newroi.setX(m_roiRect_union.x() * 4 / decimation - (m_roiRect_union.x() * 4 / decimation) % 2);
	newroi.setY(m_roiRect_union.y() * 4 / decimation - (m_roiRect_union.y() * 4 / decimation) % 2);
	newroi.setWidth(m_roiRect_union.width() * 4 / decimation - (m_roiRect_union.width() * 4 / decimation) % 8 + 8);
	newroi.setHeight(m_roiRect_union.height() * 4 / decimation - (m_roiRect_union.height() * 4 / decimation) % 8 + 8);

	return newroi;
}
