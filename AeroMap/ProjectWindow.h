#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include "AeroMap.h"        // application header
#include "LightWindow.h"

#include <QWidget>
#include <QTreeWidget>
#include <QGridLayout>

class ProjectWindow : public QWidget
{
	Q_OBJECT

public:

	explicit ProjectWindow(QWidget* parent = nullptr);
	~ProjectWindow();

	void SaveState();

public slots:

	void OnProjectChanged();

protected:

	void closeEvent(QCloseEvent* event) override;

private:

	enum class ItemType
	{
		None,

		ProjectRoot,	// root node for project

		DroneRoot,		// root node for photogrammetry workspace
		DroneInput,		// location of input images
		DroneOutput,	// root folder that will receive outputs

		Output,			// root node for outputs
		OutputOrtho,	// orthophoto
		OutputDTM,		// digital terrain model
		OutputDSM		// digital surface model
	};

	QTreeWidget* mp_Tree;
	QTreeWidgetItem* mp_ItemRoot;			// root tree node
	QTreeWidgetItem* mp_ItemDroneRoot;
	QTreeWidgetItem* mp_ItemDroneInput;
	QTreeWidgetItem* mp_ItemDroneOutput;
	QTreeWidgetItem* mp_ItemOutput;			// root node for outputs
	QTreeWidgetItem* mp_ItemOutputOrtho;
	QTreeWidgetItem* mp_ItemOutputDTM;
	QTreeWidgetItem* mp_ItemOutputDSM;

	QGridLayout* mp_gridLayout;

	QAction* mp_actionActivate;			// activate selected view
	QAction* mp_actionAddFile;			// general add file action
	QAction* mp_actionOpenFolder;		// general open containing folder
	QAction* mp_actionProperties;		// item properties

	LightWindow* mp_LightWindow;

	bool m_ItemChangeMask;				// mask out change events

private slots:

	void OnItemClicked(QTreeWidgetItem* pItem, int column);
	void OnItemDoubleClicked(QTreeWidgetItem* pItem, int column);

	// context menu handlers
	void OnActivateView();
	void OnAddFile();
	void OnLoadFile();

	void OnProperties();
	void OnOpenFolder();

private:

	// QWidget
	void contextMenuEvent(QContextMenuEvent* event) override;

	void CreateActions();
	void DisplayProperties(QTreeWidgetItem* pItem);
	XString SelectFolder();
	void UpdateUI();
};

#endif // #ifndef PROJECTWINDOW_H
