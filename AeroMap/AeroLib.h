#ifndef AEROLIB_H
#define AEROLIB_H

#include "AeroMap.h"
#include "exif.h"		// easy exif header

namespace AeroLib
{
	int  RunProgram(const char* prog, QStringList args, const char* output_file = nullptr);
	void InitRunLog();
	void WriteRunLog(const char* text);

	struct Georef
	{
		int utm_zone;
		char hemi;
		double x;
		double y;
		bool is_valid;

		Georef()
			: utm_zone(0)
			, hemi(' ')
			, x(0.0)
			, y(0.0)
			, is_valid(false)
		{

		}
	};

	Georef ReadGeoref();

	double  CalcFocalRatio(easyexif::EXIFInfo exif);
	__int64 CalcUnixEpoch(const char* dateTime);
	XString GetCameraString(easyexif::EXIFInfo exif, bool opensfm);

	double Median(std::vector<double>& v);
	double Mean(std::vector<double>& v);
	
	void CreateFolder(XString path);
	void DeleteFolder(XString path);
	void Replace(XString src_file, XString dest_file);

	bool FileExists(XString file_name);
	void RemoveFile(XString file_name);

	int get_depthmap_resolution(std::vector<Photo*> photos);
	RectD GetRasterExtents(XString file_name);
}

#endif // #ifndef AEROLIB_H
