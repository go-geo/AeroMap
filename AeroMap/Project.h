#ifndef PROJECT_H
#define PROJECT_H

#include "exif.h"			// easy exif header
#include "Gis.h"
#include "XString.h"
#include "Photo.h"
#include "Reconstruction.h"

#include "gdal_priv.h"
#include "cpl_conv.h"		// for CPLMalloc()
#include "ogrsf_frmts.h"

#include <QObject>

#include <vector>

struct ArgType
{
	bool dsm;
	bool dtm;
	bool fast_orthophoto;
	int max_concurrency;

	double dem_resolution;
	int    dem_decimation;
	bool   dem_euclidean_map;
	int    dem_gapfill_steps;
	double ortho_resolution;

	int octree_depth;
	int mesh_size;

	double pc_filter;
	double pc_sample;
	bool   pc_classify;
	bool   pc_rectify;
	std::string pc_quality;			// unknown problem using XString in this context

	double crop;
	std::string boundary;
	bool auto_boundary;
	double auto_boundary_distance;

	bool use_3dmesh;
	bool skip_3dmodel;
	bool skip_report;
	bool skip_orthophoto;

	bool ignore_gsd;

	std::string primary_band;
	bool skip_band_alignment;

	std::string radiometric_calibration;
};
extern ArgType arg;

struct TreeType
{
	XString opensfm;
	XString opensfm_reconstruction;
	XString opensfm_reconstruction_nvm;

	XString openmvs;
	XString openmvs_model;

	XString filter_path;					// root output point filtering path
	XString filtered_point_cloud;
	XString filtered_point_cloud_stats;

	XString mesh_path;						// root output mesh path
	XString mesh;
	XString mesh_25d;

	XString texture_path;					// root output texture path
	XString texture_25d_path;
	XString textured_model_obj;				// root file name only
	XString textured_model_glb;				// root file name only

	XString georef_path;					// root output georef path
	XString georef_model_laz;
	XString georef_gcp;
	XString georef_coords;
	XString georef_gcp_utm;
	XString georef_proj;

	XString dem_path;						// root output dem path

	XString ortho_path;						// root output orthophoto path
	XString orthophoto_tif;
	XString orthophoto_render;
	XString orthophoto_log;
	XString orthophoto_corners;

	XString report_path;					// root output report path

	// executables

	XString prog_opensfm;
	XString prog_densify;
	XString prog_filter;
	XString prog_pdal;
	XString prog_poisson;
	XString prog_recon_mesh;
	XString prog_recon_tex;
	XString prog_dem2mesh;
	XString prog_dem2points;
	XString prog_odm_orthophoto;

	XString prog_gdal_buildvrt;
	XString prog_gdal_info;
	XString prog_gdal_translate;
	XString prog_gdal_warp;
};
extern TreeType tree;

class Project : public QObject
{
	Q_OBJECT

public:

	Project();
	~Project();

	int Load(const char* fileName);
	int Save(const char* fileName = nullptr);
	void Clear();
	void Update();
	bool IsDirty();

	int LoadImageList();
	int GetImageCount();
	const std::vector<Photo*>& GetImageList();

	XString GetFileName();
	XString GetName();

	XString GetDroneInputPath();
	void    SetDroneInputPath(XString path);
	XString GetDroneOutputPath();
	void    SetDroneOutputPath(XString path);

	XString GetCoordsFileName();

	Reconstruction* GetReconstruction();

	int GetUndistMaxImageSize();
	void SetUndistMaxImageSize(int max_size);

	static const char* GetDefaultExt();		// default file extension
	
signals:

	void projectChanged_sig();				// any change to project structure

private:

	std::vector<Photo*> m_ImageList;

	XString ms_FileName;			// project path/file name
	XString ms_ProjectName;			// project name

	XString ms_DroneInputPath;		// location of input images
	XString ms_DroneOutputPath;		// root folder that will receive outputs

	int m_ErrorCtr;					// # of errors in project file
	bool mb_IsDirty;				// unsaved changes

	Reconstruction* mp_Recon;

	int m_undist_image_max_size;		// undistorted max image size

private:

	int  GetIndentLevel(XString& str);
	void FreeResources();
	void LoadData();

	void InitArg();
	void InitTree();
};

#endif // #ifndef PROJECT_H
