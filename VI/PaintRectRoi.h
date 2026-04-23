#pragma once

#include <QLabel>
#include "INI_Config.h"
#include <QMessageBox>
#include <QtWidgets/QSpinBox>

/* 缩放方向 */
enum EmDirection
{
	DIR_TOP = 0,
	DIR_BOTTOM,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_LEFTTOP,
	DIR_LEFTBOTTOM,
	DIR_RIGHTTOP,
	DIR_RIGHTBOTTOM,
	DIR_MIDDLE,
	DIR_NONE
};



#define EDGPADDING       10       //四周边缘可拉伸宽度
#define CORPADDING       10        //四角可拉伸宽度

#define MIN_WIDTH        5        //可拉伸的最小宽度
#define MIN_HEIGHT       5        //可拉伸的最小高度

#define POINT_WIDTH      10        //边缘9点的宽度
#define POINT_HEIGHT     10        //边缘9点的高度

#define EDGE_WIDTH       3        //边框的宽度
#define MIDDLELINE_WIDTH 2        //辅助线的宽度

#define DRAW_TEN_POINT            //绘制十个点

#define IMAGE_WINDOW_WIDTH 768		  //图像窗口的宽度
#define IMAGE_WINDOW_HEIGHT 512		  //图像窗口的高度

class QMenu;
class QAction;

class PaintRectRoi : public QLabel
{
	Q_OBJECT

public:
	PaintRectRoi(QWidget *parent = nullptr);
	~PaintRectRoi();
	QRect getRoiRect(int area) const;               //获取已经圈选的框 外部调用
	void startPaint(int area); //画图函数的入口	传入外部参数
	void ShowInitRoi(int index);//获取子区域ROI的初始参数
	void ShowInitUnionRoi();//获取联合区域ROI的初始参数
	void DeleteUnionRoi();//删除联合区域ROI
	void CreateUnionRoi();//生成联合区域ROI
	void ClearScreen();//清除画布

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void contextMenuEvent(QContextMenuEvent *ev);

private:
	void initViewer();                         //初始化
	void saveROIImage();                       //把ROI区域的图片存储下来
	EmDirection region(const QPoint &point);   //根据鼠标位置设置鼠标形状
	void scaleRect(const QPoint &mousePoint);  //缩放矩形
	void paintRect(const QPoint &mousePoint);  //绘制矩形
	void moveRect(const QPoint &mousePoint);   //拖动矩形
	QPoint MousePointConvert(QPoint ev_pos);  //将超出边界的鼠标坐标进行转换
	QRect ChangetoStdRoi(QRect stdroi);	  //将图形界面的ROI修改成相机可接受的ROI

private:
	bool m_bPainterPressed;        //是否正在绘制
	bool m_bMovedPressed;          //是否正在拖动
	bool m_bScalePressed;          //是否正在缩放大小
	bool m_bPainterAllowed = false;//是否允许绘制区域0,1的ROI
	bool m_bShowInitROI = false;   //是否显示已存储的ROI
	bool m_bShowUnionROI = false;  //是否允许绘制联合区域的ROI
	QPoint m_paintStartPoint;      //绘制的初始位置
	QPoint m_moveStartPoint;       //拖动的初始位置
	QRect m_roiRect[2] = {};       //绘制区域ROI
	QRect m_roiRect_union;       //绘制联合区域ROI

	EmDirection m_emCurDir;        //拖动的方向

	QMenu *m_pOptMenu;
	QAction *m_pDelAction;
	QAction *m_pSaveAction;

private:
	INI_Config *ini = new INI_Config();
	int page_index = 0;
	int OffsetX = 0;
	int OffsetY = 0;
	int ImageWidth = 0;
	int ImageHeight = 0;
	int decimation = 1;
	int AreaNumber = 1;

public slots:
	void GetSpinBoxChangedValue(int value, int number);//向控制面板传送实时数据
	void saveUnionROIImage();//保存联合区域ROI


signals:
	void getRoiRect0(QRect rect,int dec);//dec为下采样比例
	void getRoiRect1(QRect rect, int dec);//dec为下采样比例
	void getRoiRectUnion(QRect rect, int dec);//dec为下采样比例

};
