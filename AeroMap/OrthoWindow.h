#ifndef ORTHOWINDOW_H
#define ORTHOWINDOW_H

#include "AeroMap.h"	        // application header

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenu>

class MainWindow;

class OrthoWindow : public QGraphicsView
{
    Q_OBJECT

public:
	
	OrthoWindow(QWidget* parent);
	~OrthoWindow();

	void ResetView();

protected:

	// QWidget
	virtual void closeEvent(QCloseEvent* event) override;
	virtual void contextMenuEvent(QContextMenuEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;
	virtual void showEvent(QShowEvent* event) override;

private:

	struct DistanceType
	{
		VEC2 pt;
		QGraphicsLineItem* pLine;
		QGraphicsSimpleTextItem* pText;
	};
	std::vector<DistanceType> m_Distance;		// connected points defining distance measurement
	QPen m_pen_distance;

	QGraphicsScene m_scene;			// backing scene
	RectD m_corners;				// corners of orthophoto, world units
	QRect m_image_rect;				// image size, pixels

	QFont m_Font;					// default font

	SizeType	m_winSize;			// client area dimensions
	PointType  	m_ptAnchor;			// anchor point for current operation
	PointType	m_ptLastMouse;		// last recorded mouse position
	RectType   	m_rctSel;			// current 2d selection

	MainWindow* mp_Parent;

	QAction* mp_actionProp;
	QAction* mp_actionClear;

private slots:

	void OnProp();
	void OnClear();

private:

	void CreateActions();
	void LoadOrthophoto();

	// measurement

	int    AddDistancePoint(VEC2& pos);
	int    GetDistancePointCount();
	VEC2   GetDistancePoint(int index);
	void   ClearDistancePoints();
	double GetDistance(int index);

	// coordinate mapping
	void ScreenToWorld(int xp, int yp, double& x, double& y);
	bool WorldToImage(double x, double y, int& xp, int& yp);

	double GetPenWidth();
	double GetTextScale();
	void FitToWindow();
};

#endif // #ifndef ORTHOWINDOW_H
