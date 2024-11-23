#ifndef STAGESFM_H
#define STAGESFM_H

#include "AeroMap.h"		// application header
#include "exif.h"			// easy exif header
#include "Stage.h"			// base class

class StageSFM : Stage
{
public:

	virtual int Run() override;

private:

	struct ImageType
	{
		XString fileName;
		easyexif::EXIFInfo exif;
	};
	std::vector<ImageType> m_ImageList;

private:
	
	int Setup();

	void DetectFeatures();
	void MatchFeatures();
	void CreateTracks();
	void Reconstruct();
	void ComputeStats();
	void Undistort();
	void ExportGeocoords();
	void ExportPly();
	void ExportVisualSfm();

	int WriteExif();
	int WriteImageListText();
	int WriteCameraModelsJson();
	int WriteReferenceLLA();
	int WriteConfigYaml();

	int WriteCamerasJson();

	void UpdateConfig();
};

#endif //#ifndef STAGESFM_H
