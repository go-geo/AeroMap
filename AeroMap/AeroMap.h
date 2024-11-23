#ifndef AEROMAP_H
#define AEROMAP_H

#include <assert.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <GL/glew.h>

#include <QtGui>
#include <QApplication>
#include <QComboBox>

#include "Version.h"
#include "MarkTypes.h"
#include "MarkLib.h"
#include "XString.h"
#include "Logger.h"
#include "Calc.h"
#include "Gis.h"

#include "Project.h"
#include "Tool.h"
#include "OutputWindow.h"

#define APP_NAME "AeroMap"

#define ORG_KEY "GorgeGeo"
#define APP_KEY "AeroMap"

#define PATH_APPDATA_KEY		"AppDataPath"
#define STARTUP_STATE_KEY		"StartupState"
#define LAST_FILE_KEY			"LastFile"			// most recently loaded project file

#define TERRAIN_SCALE_KEY		"TerrainScale"		// most recent color scale used in terrain view

#define CONTOUR_INTERVAL_KEY	"ContourInterval"
#define CONTOUR_START_KEY		"ContourStart"
#define CONTOUR_COLOR_KEY		"ContourColor"

#define SCENE_BACK_COLOR QColor(0xF0, 0xF0, 0xFF)	// default background color for graphics scenes

// connection macro for the following overload that:
//		1. asserts on failure
//		2. logs failure location
//
//static QMetaObject::Connection connect(const QObject *sender, const QMetaMethod &signal,
//	const QObject *receiver, const QMetaMethod &method,
//	Qt::ConnectionType type = Qt::AutoConnection);

#define verify_connect(parm_sender, parm_signal, parm_receiver, parm_method) \
{ \
	QMetaObject::Connection cn = connect(parm_sender, parm_signal, parm_receiver, parm_method); \
	if (cn == nullptr ) \
	{ \
		Logger::Write(__FUNCTION__, "Qt connection failed @ line %d.", __LINE__); \
		assert(false); \
	} \
}

class MainWindow;

class AeroMapApp : public QApplication
{
	Q_OBJECT

public:

	enum class ViewType
	{
		None,
		Drone,		// drone photogrammetry workspace
		Ortho,		// orthophoto viewer
		Terrain		// terrain models
	};

	Tool m_Tool;	// active tool

public:

	AeroMapApp(int& argc, char** argv);
	~AeroMapApp();

	XString GetAppDataPath() { return ms_AppDataPath; }
	XString GetProgLibPath() { return ms_ProgLibPath; }

	MainWindow* GetMainWindow();

	Project& GetProject() { return m_Project; }

	void ActivateView(AeroMapApp::ViewType view, int subItem = -1);
	ViewType GetActiveView();

	void SetOutputWindow(OutputWindow* pOutputWindow) { mp_OutputWindow = pOutputWindow; }
	OutputWindow* GetOutputWindow() { return mp_OutputWindow; }

	XString GetEnvValue(const char* envKey);
	void LogWrite(const char* text, ...);

	void DumpRaster(GDALDataset* pDS, const char* fileName);
	void DumpVector(GDALDataset* pDS, const char* fileName);

	static void SetComboByUserData(QComboBox* cbo, int userData);

signals:

	void view_change_sig(AeroMapApp::ViewType, int);	// emitted when active view changes, optional subitem to activate
	void tool_change_sig();								// emitted when selected tool changed

private:

	XString ms_AppDataPath;				// location of application data
	XString ms_ProgLibPath;				// path to ProgLib folder

	Project m_Project;					// active project
	ViewType m_ViewType;
	OutputWindow* mp_OutputWindow;

private:

	void LoadSettings();
	XString ResolveAppDataPath();
	XString ResolveProgLibPath(int& argc, char** argv);
};

inline AeroMapApp* GetApp()
{
	return static_cast<AeroMapApp*>(qApp);
}

inline Project& GetProject()
{ 
	return GetApp()->GetProject();
}

#endif // #ifndef AEROMAP_H
