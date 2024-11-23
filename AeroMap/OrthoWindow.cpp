// OrthoWindow.cpp
// // Orthophoto viewer.
//

#include "AeroLib.h"
#include "MetaDataDlg.h"
#include "TextFile.h"
#include "MainWindow.h"
#include "OrthoWindow.h"

#include <QStatusBar>

OrthoWindow::OrthoWindow(QWidget* parent)
	: mp_actionProp(nullptr)
	, mp_actionClear(nullptr)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setMinimumSize(50, 50);

	mp_Parent = static_cast<MainWindow*>(parent);
	assert(mp_Parent != nullptr);

	setMouseTracking(true);
	setAcceptDrops(true);
	grabKeyboard();

	// set up fonts
	m_Font.setFamily("Consolas");
	m_Font.setPointSize(10.0);
	m_Font.setItalic(false);

	CreateActions();

	LoadOrthophoto();
}

OrthoWindow::~OrthoWindow()
{
	delete mp_actionProp;
	delete mp_actionClear;
}

void OrthoWindow::LoadOrthophoto()
{
	if (AeroLib::FileExists(tree.orthophoto_tif) == false)
		return;

	QPixmap pixmap;
	if (pixmap.load(tree.orthophoto_tif.c_str()) == false)
	{
		Logger::Write(__FUNCTION__, "Unable to load '%s'", tree.orthophoto_tif.c_str());
		return;
	}

	if (AeroLib::FileExists(tree.orthophoto_corners) == false)
	{
		Logger::Write(__FUNCTION__, "File not found: '%s'", tree.orthophoto_corners.c_str());
	}

	TextFile corner_file(tree.orthophoto_corners.c_str());
	if (corner_file.GetLineCount() < 1)
	{
		Logger::Write(__FUNCTION__, "Unable to load '%s'", tree.orthophoto_corners.c_str());
	}

	XString corners = corner_file.GetLine(0).c_str();
	int token_count = corners.Tokenize(" \t");
	if (token_count >= 4)
	{
		m_corners.x0 = corners.GetToken(0).GetDouble();
		m_corners.y0 = corners.GetToken(1).GetDouble();
		m_corners.x1 = corners.GetToken(2).GetDouble();
		m_corners.y1 = corners.GetToken(3).GetDouble();
	}

	m_image_rect = pixmap.rect();

	QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
	m_scene.addItem(item);

	m_scene.setBackgroundBrush(QBrush(SCENE_BACK_COLOR));

	setScene(&m_scene);

	m_pen_distance.setColor(QColor(10, 200, 10));
	m_pen_distance.setWidthF(GetPenWidth());
	m_pen_distance.setStyle(Qt::PenStyle::SolidLine);
}

void OrthoWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
	// This event handler can be reimplemented in a subclass to receive mouse 
	// double click events for the widget.
	// The default implementation generates a normal mouse press event.
	// Note: The widget will also receive mouse press and mouse release events in addition to 
	// the double click event. It is up to the developer to ensure that the application interprets 
	// these events correctly.

	Q_UNUSED(event);

	update();
}

void OrthoWindow::mousePressEvent(QMouseEvent* event)
{
	// This event handler can be reimplemented in a subclass to receive mouse press events 
	// for the widget.
	// 
	// If you create new widgets in the mousePressEvent() the mouseReleaseEvent() may not 
	// end up where you expect, depending on the underlying window system (or X11 window manager), 
	// the widgets' location and maybe more.
	// 
	// The default implementation implements the closing of popup widgets when you click outside 
	// the window. For other widget types it does nothing.
	//

	switch (GetApp()->m_Tool.GetTool()) {
	case Tool::ToolType::Select:
	case Tool::ToolType::ViewZoom:
	case Tool::ToolType::ViewPan:
		m_ptAnchor.x = event->x();
		m_ptAnchor.y = event->y();
		break;
	case Tool::ToolType::Distance:
		{
			if (event->button() == Qt::LeftButton)
			{
				VEC2 pos;
				ScreenToWorld(event->x(), event->y(), pos.x, pos.y);
				if (m_corners.Contains(pos.x, pos.y))
					AddDistancePoint(pos);
			}
		}
		break;
	case Tool::ToolType::Area:
		break;
	case Tool::ToolType::Volume:
		break;
	default:   // no active tool
		break;
	}

	update();

	m_ptLastMouse.x = event->x();  // last recorded mouse position
	m_ptLastMouse.y = event->y();
}

void OrthoWindow::mouseReleaseEvent(QMouseEvent* /* event */)
{
	// This event handler can be reimplemented in a subclass to receive 
	// mouse release events for the widget.
	//

	switch (GetApp()->m_Tool.GetTool()) {
	case Tool::ToolType::Select:   // finished selecting
		break;
	default:
		break;
	}

	update();
}

void OrthoWindow::mouseMoveEvent(QMouseEvent* event)
{
	// This event handler can be reimplemented in a subclass to receive mouse move 
	// events for the widget.
	// 
	// If mouse tracking is switched off, mouse move events only occur if a mouse button
	// is pressed while the mouse is being moved. If mouse tracking is switched on, mouse 
	// move events occur even if no mouse button is pressed.
	// 
	// QMouseEvent::pos() reports the position of the mouse cursor, relative to this widget. 
	// For press and release events, the position is usually the same as the position of the 
	// last mouse move event, but it might be different if the user's hand shakes. This is a 
	// feature of the underlying window system, not Qt.
	// 
	// If you want to show a tooltip immediately, while the mouse is moving (e.g., to get the
	// mouse coordinates with QMouseEvent::pos() and show them as a tooltip), you must first 
	// enable mouse tracking as described above. Then, to ensure that the tooltip is updated 
	// immediately, you must call QToolTip::showText() instead of setToolTip() in your 
	// implementation of mouseMoveEvent().
	//

	if (event->buttons() & Qt::LeftButton)		// left button is depressed
	{
		switch (GetApp()->m_Tool.GetTool()) {
		case Tool::ToolType::Select:
		case Tool::ToolType::ViewZoom:
			break;
		case Tool::ToolType::ViewPan:
			break;
		case Tool::ToolType::None:     // no tool selected
			break;
		default:
			break;
		}

		update();
	}       // if left button

	double x, y;
	int xp, yp;
	ScreenToWorld(event->x(), event->y(), x, y);
	WorldToImage(x, y, xp, yp);

	XString str = XString::Format("Mouse: %d,%d  Image: %d,%d UTM: %0.1f,%0.1f Distance: %d", 
		event->x(), event->y(), xp, yp, x, y, GetDistancePointCount());
	mp_Parent->statusBar()->showMessage(str.c_str());

	m_ptLastMouse.x = event->x();
	m_ptLastMouse.y = event->y();
}

void OrthoWindow::wheelEvent(QWheelEvent* event)
{
	//	This event handler, for event event, can be reimplemented in a subclass to receive wheel events
	//	for the widget.
	//
	//	If you reimplement this handler, it is very important that you ignore() the event if you do not
	//	handle it, so that the widget's parent can interpret it.
	//
	//	The default implementation ignores the event.
	//

	double zoomInFactor = 1.25;
	double zoomOutFactor = 1.0 / zoomInFactor;

	int delta = event->angleDelta().y();
	if (delta > 0)
	{
		scale(zoomOutFactor, zoomOutFactor);
	}
	else if (delta < 0)
	{
		scale(zoomInFactor, zoomInFactor);
	}
	else
	{
		event->ignore();
	}

	// scale distance lines 
	m_pen_distance.setWidthF(GetPenWidth());
	for (int i = 1; i < GetDistancePointCount(); ++i)
	{
		m_Distance[i].pLine->setPen(m_pen_distance);
		m_Distance[i].pText->setScale(GetTextScale());
	}

	update();
}

void OrthoWindow::keyPressEvent(QKeyEvent* event)
{
	// This event handler can be reimplemented in a subclass to receive key press 
	// events for the widget.
	// 
	// A widget must call setFocusPolicy() to accept focus initially and have focus 
	// in order to receive a key press event.
	// 
	// If you reimplement this handler, it is very important that you call the base 
	// class implementation if you do not act upon the key.
	// 
	// The default implementation closes popup widgets if the user presses Esc. 
	// Otherwise the event is ignored, so that the widget's parent can interpret it.
	// 
	// Note that QKeyEvent starts with isAccepted() == true, so you do not need to 
	// call QKeyEvent::accept() - just do not call the base class implementation if
	// you act upon the key.
	//

	switch (event->key()) {
	case Qt::Key_D:
		break;
	case Qt::Key_S:
		break;
	case Qt::Key_W:
		break;
	}

	update();

	__super::keyPressEvent(event);
}

void OrthoWindow::keyReleaseEvent(QKeyEvent* event)
{
	// This event handler can be reimplemented in a subclass to receive key release 
	// events for the widget.
	// 
	// A widget must accept focus initially and have focus in order to receive a key 
	// release event.
	// 
	// If you reimplement this handler, it is very important that you call the base 
	// class implementation if you do not act upon the key.
	// 
	// The default implementation ignores the event, so that the widget's parent can 
	// interpret it.
	// 
	// Note that QKeyEvent starts with isAccepted() == true, so you do not need to 
	// call QKeyEvent::accept() - just do not call the base class implementation if
	// you act upon the key.
	//

	__super::keyReleaseEvent(event);
}

void OrthoWindow::showEvent(QShowEvent* event)
{
	Q_UNUSED(event);

	// scale to fit in window

	static bool init = false;
	if (init == false)
	{
		FitToWindow();

		init = true;
	}
}

 void OrthoWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
}

void OrthoWindow::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);
	menu.addAction(mp_actionClear);
	menu.addAction(mp_actionProp);
	menu.exec(event->globalPos());
}

void OrthoWindow::CreateActions()
{
	mp_actionProp = new QAction(QIcon(""), tr("Properties"), this);
	mp_actionProp->setStatusTip(tr("Show properties"));
	connect(mp_actionProp, SIGNAL(triggered()), this, SLOT(OnProp()));

	mp_actionClear = new QAction(QIcon(""), tr("Clear"), this);
	mp_actionClear->setStatusTip(tr("Clear edit artifacts"));
	connect(mp_actionClear, SIGNAL(triggered()), this, SLOT(OnClear()));
}

void OrthoWindow::ResetView()
{
	FitToWindow();
	//QTransform t = transform();
	//if (t.isScaling())
	//{
	//	double sf = t.m11();
	//	scale(1.0 / sf, 1.0 / sf);
	//}

	update();
}

int OrthoWindow::AddDistancePoint(VEC2& pos)
{
	DistanceType dt;
	dt.pt = pos;
	if (GetDistancePointCount() > 0)
	{
		double x0 = m_Distance.back().pt.x;
		double y0 = m_Distance.back().pt.y;

		double x1 = pos.x;
		double y1 = pos.y;

		// map to image coordinats which are the same as scene coordinates
		int xp0, yp0; 
		int xp1, yp1;
		WorldToImage(x0, y0, xp0, yp0);
		WorldToImage(x1, y1, xp1, yp1);

		dt.pLine = m_scene.addLine(xp0, yp0, xp1, yp1, m_pen_distance);
		dt.pLine->show();

		double distance = Distance(x0, y0, x1, y1);
		XString str = XString::Format("%0.2f", distance);
		dt.pText = m_scene.addSimpleText(str.c_str(), m_Font);
		dt.pText->setScale(GetTextScale());
		dt.pText->setPos(xp1, yp1);
		dt.pText->show();
	}
	m_Distance.push_back(dt);
	return static_cast<int>(m_Distance.size());
}

int OrthoWindow::GetDistancePointCount()
{
	return static_cast<int>(m_Distance.size());
}

VEC2 OrthoWindow::GetDistancePoint(int index)
{
	if (index < m_Distance.size())
		return m_Distance[index].pt;

	return VEC2();
}

void OrthoWindow::ClearDistancePoints()
{
	return m_Distance.clear();
}

double OrthoWindow::GetDistance(int index)
{
	// return distance from m_Distance[index-1] to m_Distance[index]

	if (index > 0 && index < m_Distance.size())
		return (m_Distance[index].pt - m_Distance[index - 1].pt).Magnitude();

	return 0.0;
}

void OrthoWindow::ScreenToWorld(int xp, int yp, double& x, double& y)
{
	// Map window x/y values to world xy values.
	//
	// Inputs:
	//		xp, yp = 2d pixel values
	//
	// Outputs:
	//		x, y = 2d world values
	//

	// this gives the pixel in the image
	QPointF pt = mapToScene(xp, yp);
	double px = pt.x();
	double py = pt.y();

	// calc pixel offset into window as factor from 0.0-1.0
	
	double dx = px / (double)m_image_rect.width();
	double dy = 1.0 - (py / (double)m_image_rect.height());

	x = m_corners.x0 + dx * m_corners.DX();
	y = m_corners.y0 + dy * m_corners.DY();
}

bool OrthoWindow::WorldToImage(double x, double y, int& xp, int& yp)
{
	// Map world (x,y) position to image pixel coordinates,
	// where (0,0) is upper left corner.
	//
	// Inputs:
	//		x, y = 2d world values
	//
	// Outputs:
	//		xp, yp = 2d pixel values
	//		return = true if coordinates are inside image
	//

	xp = 0;
	yp = 0;

	// offset into image
	double dx = (x - m_corners.x0) / m_corners.DX();
	double dy = (m_corners.y1 - y) / m_corners.DY();

	if ((dx >= 0.0 && dx <= 1.0) && (dy >= 0.0 && dy <= 1.0))
	{
		// get image pixel coords
		xp = m_image_rect.left() + (dx * m_image_rect.width());
		yp = m_image_rect.top() + (dy * m_image_rect.height());

		return true;
	}

	return false;
}

double OrthoWindow::GetPenWidth()
{
	// Return width of pens based on current scale factor.
	//

	double pen_width = 16.0;

	QTransform t = transform();
	if (t.isScaling())
	{
		double sf = t.m11();
		pen_width = 1.0 / sf * 2.5;
	}

	return pen_width;
}

double OrthoWindow::GetTextScale()
{
	double text_scale = 1.0;

	QTransform t = transform();
	if (t.isScaling())
	{
		double sf = t.m11();
		text_scale = 1.0 / sf * 1.25;
	}

	return text_scale;
}

void OrthoWindow::OnProp()
{
	XString meta = XString::Format("File Name: %s\n", tree.orthophoto_tif.c_str());
	meta += XString::Format("X Extents: %0.3f-%0.3f\n", m_corners.x0, m_corners.x1);
	meta += XString::Format("Y Extents: %0.3f-%0.3f\n", m_corners.y0, m_corners.y1);

	MetaDataDlg dlg(this, "Orthophoto");
	dlg.SetMetaData(meta);
	dlg.exec();
}

void OrthoWindow::OnClear()
{
	// clear distance measurements
	for (int i = 1; i < GetDistancePointCount(); ++i)
	{
		m_scene.removeItem(m_Distance[i].pLine);
		m_scene.removeItem(m_Distance[i].pText);
	}
	m_Distance.clear();
}

void OrthoWindow::FitToWindow()
{
	// first, back out the current scale factor
	double cur_sf = transform().m11();
	scale(1.0 / cur_sf, 1.0 / cur_sf);

	// now scale it to fit within the window
	double xf = (double)width() / (double)m_image_rect.width();
	double yf = (double)height() / (double)m_image_rect.height();

	double sf = xf < yf ? xf : yf;
	scale(sf, sf);
}
