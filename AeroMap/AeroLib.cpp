// AeroLib.cpp
// Common routines for AeroMap.
//

#include "TextFile.h"
#include "AeroLib.h"

constexpr char* RUN_LOG = "run_log.txt";

namespace AeroLib
{
	int RunProgram(const char* prog, QStringList args, const char* output_file)
	{
		// Run external program, setting environment variables as needed.
		//
		// Inputs:
		//		prog = executable, full path with extension
		//		args = arguments
		//		output_file = optional file to capture output
		//
		// *** This call will block ***
		//

		int status = 0;

		if (QFile::exists(prog) == false)
		{
			Logger::Write(__FUNCTION__, "Program file not found: '%s'", prog);
			assert(false);
			return -1;
		}

		// verify parameters
		for (XString arg : args)
		{
			if (arg.BeginsWith(" ") || arg.EndsWith(" "))
			{
				Logger::Write(__FUNCTION__, "Invalid parameter format: '%s'", arg.c_str());
				assert(false);
			}
		}

		// log program being executed
		XString run_log = XString::CombinePath(GetProject().GetDroneOutputPath(), RUN_LOG);
		FILE* pFile = fopen(run_log.c_str(), "at");
		if (pFile)
		{
			fprintf(pFile, "Running: %s", prog);
			for (XString arg : args)
				fprintf(pFile, " %s", arg.c_str());
			fprintf(pFile, "\n");
			fclose(pFile);
		}

		// The normal scenario starts from the current environment by calling QProcessEnvironment::systemEnvironment() and then proceeds to adding, 
		// changing, or removing specific variables. The resulting variable roster can then be applied to a QProcess with setProcessEnvironment().

		XString lib_path = GetApp()->GetProgLibPath();

		// python env config file must be updated
		XString pyenv_cfg = XString::CombinePath(lib_path, "venv/pyvenv.cfg");
		pFile = fopen(pyenv_cfg.c_str(), "wt");
		if (pFile)
		{
			fprintf(pFile, "home = %s\n", XString::CombinePath(lib_path, "venv/Scripts").c_str());
			fprintf(pFile, "include-system-site-packages = false");
			fclose(pFile);
		}

		// update opencv python config

		XString file_name = XString::CombinePath(lib_path, "venv/Lib/site-packages/cv2/config-3.8.py");
		pFile = fopen(file_name.c_str(), "wt");
		if (pFile)
		{
			fprintf(pFile, "PYTHON_EXTENSIONS_PATHS = [r'''%s/venv/lib/site-packages/cv2/python-3.8'''] + PYTHON_EXTENSIONS_PATHS", lib_path.c_str());
			fclose(pFile);
		}
		file_name = XString::CombinePath(lib_path, "venv/Lib/site-packages/cv2/config.py");
		pFile = fopen(file_name.c_str(), "wt");
		if (pFile)
		{
			fprintf(pFile, "BINARIES_PATHS = [r\"%s/bin\"] + BINARIES_PATHS", lib_path.c_str());
			fclose(pFile);
		}

		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		env.insert("GDAL_DATA", XString::CombinePath(lib_path, "venv/Lib/site-packages/osgeo/data/gdal").c_str());
		env.insert("GDAL_DRIVER_PATH", XString::CombinePath(lib_path, "venv/Lib/site-packages/osgeo/gdalplugins").c_str());
		env.insert("OSFMBASE", XString::CombinePath(lib_path, "bin/opensfm/bin").c_str());
		
		XString path_var = XString::CombinePath(lib_path, "venv/Scripts") + ";";
		path_var += XString::CombinePath(lib_path, "venv/Lib/site-packages/osgeo") + ";";
		path_var += XString::CombinePath(lib_path, "bin") + ";";
		path_var += XString::CombinePath(lib_path, "bin/opensfm/bin") + ";";
		env.insert("Path", path_var.c_str());

		env.insert("PDAL_DRIVER_PATH", XString::CombinePath(lib_path, "bin").c_str());
		env.insert("PROJ_LIB", XString::CombinePath(lib_path, "venv/Lib/site-packages/osgeo/data/proj").c_str());
		env.insert("PYENVCFG", pyenv_cfg.c_str());
		env.insert("PYTHONPATH", XString::CombinePath(lib_path, "venv").c_str());
		env.insert("PYTHONPYCACHEPREFIX", "C:/ProgramData/AeroMap/pycache");
		env.insert("SBBIN", XString::CombinePath(lib_path, "bin").c_str());
		env.insert("VIRTUAL_ENV", XString::CombinePath(lib_path, "venv").c_str());

		//env.insert("GDAL_DATA", "C:/ODM-3.2.0/venv/Lib/site-packages/osgeo/data/gdal");
		//env.insert("GDAL_DRIVER_PATH", "C:/ODM-3.2.0/venv/Lib/site-packages/osgeo/gdalplugins");
		//env.insert("ODMBASE", "C:/ODM-3.2.0/");
		//env.insert("OSFMBASE", "C:/ODM-3.2.0/SuperBuild/install/bin/opensfm/bin");
		//env.insert("Path", "C:/ODM-3.2.0/venv/Scripts;C:/ODM-3.2.0/venv/Lib/site-packages/osgeo;C:/ODM-3.2.0/SuperBuild/install/bin;C:/ODM-3.2.0/SuperBuild/install/bin/opensfm/bin");
		//env.insert("PDAL_DRIVER_PATH", "C:/ODM-3.2.0/SuperBuild/install/bin");
		//env.insert("PROJ_LIB", "C:/ODM-3.2.0/venv/Lib/site-packages/osgeo/data/proj");
		//env.insert("PYENVCFG", "C:/ODM-3.2.0/venv/pyvenv.cfg");
		//env.insert("PYTHONPATH", "C:/ODM-3.2.0/venv");
		//env.insert("PYTHONPYCACHEPREFIX", "C:/ProgramData/ODM/pycache");
		//env.insert("SBBIN", "C:/ODM-3.2.0/SuperBuild/install/bin");
		//env.insert("VIRTUAL_ENV", "C:/ODM-3.2.0/venv");

		//PROMPT=(venv) $P$G
		//_OLD_VIRTUAL_PATH=C:/ODM-3.2.0/venv/Lib/site-packages/osgeo;C:/ODM-3.2.0/SuperBuild/install/bin;C:/ODM-3.2.0/SuperBuild/install/bin/opensfm/bin
		//_OLD_VIRTUAL_PROMPT=$P$G

		QProcess* pProc = new QProcess();
		pProc->setProcessEnvironment(env);
		pProc->start(prog, args);

		// block until program completes
		bool wait_status = pProc->waitForFinished(-1);
		if (wait_status == false)
		{
			Logger::Write(__FUNCTION__, "waitForFinished() failed");
		}
			
		QProcess::ExitStatus exit_status = pProc->exitStatus();
		if (exit_status != QProcess::NormalExit)
		{
			Logger::Write(__FUNCTION__, "Program returned exit status: %d", (int)exit_status);
			assert(false);
			return -2;
		}

		XString std_out = QString(pProc->readAllStandardOutput());
		XString std_err = QString(pProc->readAllStandardError());

		if (std_err.GetLength() > 0)
		{
			std_err.Replace("\r\n", "\n");

			FILE* pFile = fopen(run_log.c_str(), "at");
			if (pFile)
			{
				fprintf(pFile, "%s", std_err.c_str());
				fclose(pFile);
			}
		}

		if (output_file != nullptr)
		{
			if (std_out.GetLength() > 0)
			{
				std_out.Replace("\r\n", "\n");

				FILE* pFile = fopen(output_file, "wt");
				if (pFile)
				{
					fprintf(pFile, "%s", std_out.c_str());
					fclose(pFile);
				}
			}
		}

		delete pProc;

		return status;
	}

	void InitRunLog()
	{
		XString file_name = XString::CombinePath(GetProject().GetDroneOutputPath(), RUN_LOG);
		FILE* pFile = fopen(file_name.c_str(), "wt");
		fclose(pFile);
	}

	void WriteRunLog(const char* text)
	{
		// Write text directly to run log.
		//

		if (text == nullptr)
			return;

		XString run_log = XString::CombinePath(GetProject().GetDroneOutputPath(), RUN_LOG);
		FILE* pFile = fopen(run_log.c_str(), "at");
		if (pFile)
		{
			fprintf(pFile, "%s\n", text);
			fclose(pFile);
		}
	}

	Georef ReadGeoref()
	{
		// Return georeference information from 'coords.txt'
		//

		Georef georef;

		georef.is_valid = false;

		if (QFile::exists(GetProject().GetCoordsFileName().c_str()))
		{
			TextFile textFile(GetProject().GetCoordsFileName().c_str());
			if (textFile.GetLineCount() >= 2)
			{
				// WGS84 UTM 32N
				// 322263 5157982
				XString line0 = textFile.GetLine(0).c_str();
				if (line0.Tokenize(" \t") == 3)
				{
					XString line1 = textFile.GetLine(1).c_str();
					if (line1.Tokenize(" \t") == 2)
					{
						XString s = line0.GetToken(2);
						if (s.EndsWith("N") || s.EndsWith("S"))
						{
							georef.hemi = s.Right(1)[0];

							s.DeleteRight(1);
							georef.utm_zone = s.GetInt();
							if (georef.utm_zone >= 1 && georef.utm_zone <= 60)
							{
								georef.x = line1.GetToken(0).GetDouble();
								georef.y = line1.GetToken(1).GetDouble();

								georef.is_valid = true;
							}
						}
					}
				}
			}
		}

		return georef;
	}

	int yisleap(int year)
	{
		return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	}

	int get_yday(int mon, int day, int year)
	{
		static const int days[2][13] = {
			{0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
			{0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
		};
		int leap = yisleap(year);

		return days[leap][mon] + day;
	}

	__int64 CalcUnixEpoch(const char* dateTime)
	{
		// Return seconds since Jan 1, 1950.
		//
		// Inputs:
		//		dateTime = "YYYY:MM:SS hh:mm:ss"
		// Outputs:
		//		return = seconds since Jan 1, 1970
		//

		int epoch = 0;

		XString str = dateTime;
		if (str.GetLength() == 19)
		{
			__int64 tm_year = str.Mid(0, 4).GetLong();
			__int64 tm_month = str.Mid(5, 2).GetLong();
			__int64 tm_day = str.Mid(8, 2).GetLong();
			__int64 tm_hour = str.Mid(11, 2).GetLong();
			__int64 tm_min = str.Mid(14, 2).GetLong();
			__int64 tm_sec = str.Mid(17, 2).GetLong();

			__int64 tm_yday = get_yday(tm_month, tm_day, tm_year);

			// yday is 1-based
			--tm_yday;

			tm_year -= 1900;

			epoch = tm_sec + tm_min * 60 + tm_hour * 3600 + tm_yday * 86400 +
				(tm_year - 70) * 31536000 + ((tm_year - 69) / 4) * 86400 -
				((tm_year - 1) / 100) * 86400 + ((tm_year + 299) / 400) * 86400;
		}

		return epoch;
	}

	double CalcFocalRatio(easyexif::EXIFInfo exif)
	{
		double focal_ratio = 0.0;
		
		double sensor_width = 0.0;
		if (exif.LensInfo.FocalPlaneResolutionUnit > 0 && exif.LensInfo.FocalPlaneXResolution > 0.0)
		{
			int resolution_unit = exif.LensInfo.FocalPlaneResolutionUnit;
			double mm_per_unit = 0.0;
			if (resolution_unit == 2)		// inch
				mm_per_unit = 25.4;
			else if (resolution_unit == 3)		// cm
				mm_per_unit = 10;
			else if (resolution_unit == 4)		// mm
				mm_per_unit = 1;
			else if (resolution_unit == 5)		// um
				mm_per_unit = 0.001;
			else
			{
				Logger::Write(__FUNCTION__, "Unknown EXIF resolution unit: %d", resolution_unit);
				assert(false);
				return focal_ratio;
			}

			if (mm_per_unit > 0.0)
			{
				double pixels_per_unit = exif.LensInfo.FocalPlaneXResolution;
				if (pixels_per_unit <= 0.0 && exif.LensInfo.FocalPlaneYResolution > 0.0)
					pixels_per_unit = exif.LensInfo.FocalPlaneYResolution;
	
				if ((pixels_per_unit > 0.0) && (exif.ImageWidth > 0))
				{
					double units_per_pixel = 1.0 / pixels_per_unit;
					sensor_width = (double)exif.ImageWidth * units_per_pixel * mm_per_unit;
				}
			}
		}

		//focal_35 = None
		double focal;
		//if "EXIF FocalLengthIn35mmFilm" in tags:
		//	focal_35 = self.float_value(tags["EXIF FocalLengthIn35mmFilm"])
		if (exif.FocalLength > 0.0)
			focal = exif.FocalLength;
		//if focal is None and "@aux:Lens" in xtags:
		//	lens = self.get_xmp_tag(xtags, ["@aux:Lens"])
		//	matches = re.search('([\d\.]+)mm', str(lens))
		//	if matches:
		//		focal = float(matches.group(1))

		//if focal_35 is not None and focal_35 > 0:
		//	focal_ratio = focal_35 / 36.0  # 35mm film produces 36x24mm pictures.
		//else:
		//	if not sensor_width:
		//		sensor_width = sensor_data().get(sensor_string, None)
		if ((sensor_width > 0.0) && (focal > 0.0))
			focal_ratio = focal / sensor_width;
		else
			focal_ratio = 0.85;

		return focal_ratio;
	}

	XString GetCameraString(easyexif::EXIFInfo exif, bool opensfm)
	{
		// Return camera id string.
		//
		
		double focal_ratio = CalcFocalRatio(exif);

		XString camera_str = XString::Format("%s %s %d %d brown %0.4f",
			exif.Make.c_str(), exif.Model.c_str(),
			exif.ImageWidth, exif.ImageHeight, 
			focal_ratio);

		// opensfm and odm have slightly different formats
		if (opensfm)
			camera_str.Insert(0, "v2 ");

		camera_str.MakeLower();

		return camera_str;
	}

	double Median(std::vector<double>& v)
	{
		// Return median of v.
		// 
		// After returning, the elements in v may be reordered 
		// and the resulting order is implementation defined.
		//

		if (v.empty())
		{
			return 0.0;
		}
		auto n = v.size() / 2;
		nth_element(v.begin(), v.begin() + n, v.end());
		auto med = v[n];
		if (!(v.size() & 1))	// if size is even
		{
			auto max_it = max_element(v.begin(), v.begin() + n);
			med = (*max_it + med) / 2.0;
		}
		return med;
	}

	double Mean(std::vector<double>& v)
	{
		double mean = 0.0;

		// size of vector 
		int n = (int)v.size();
		if (n > 0)
		{
			// sum of the vector elements 
			double sum = accumulate(v.begin(), v.end(), 0.0);

			// average of the vector elements 
			mean = sum / (double)n;
		}

		return mean;
	}

	void CreateFolder(XString path)
	{
		QDir dir(path.c_str());
		if (dir.exists() == false)
			dir.mkpath(path.c_str());
	}

	void DeleteFolder(XString path)
	{
		QDir dir(path.c_str());
		if (dir.exists())
			dir.removeRecursively();
	}

	void Replace(XString src_file, XString dest_file)
	{
		// Drop-in replacement for python os.replace(),
		// for files only.
		//

		if (QFile::exists(src_file.c_str()))
		{
			if (QFile::exists(dest_file.c_str()))
				QFile::remove(dest_file.c_str());

			QFile::rename(src_file.c_str(), dest_file.c_str());
		}
	}

	bool FileExists(XString file_name)
	{
		return QFile::exists(file_name.c_str());
	}

	void RemoveFile(XString file_name)
	{
		if (FileExists(file_name))
			QFile::remove(file_name.c_str());
	}

	int get_depthmap_resolution(std::vector<Photo*> photos)
	{
		SizeType max_dims = Photo::find_largest_photo_dims(photos);
		int min_dim = 320;		// Never go lower than this

		if ((max_dims.cx > 0) && (max_dims.cy > 0))
		{
			int w = max_dims.cx;
			int h = max_dims.cy;
			int max_dim = std::max(w, h);

			double megapixels = (w * h) / 1E6;
			double multiplier = 1.0;

			if (megapixels < 6.0)
				multiplier = 2.0;
			else if (megapixels > 42.0)
				multiplier = 0.5;

			double quality = 0.125;
			if (arg.pc_quality.compare("ultra") == 0)
				quality = 0.5;
			else if (arg.pc_quality.compare("high") == 0)
				quality = 0.25;
			else if (arg.pc_quality.compare("medium") == 0)
				quality = 0.125;
			else if (arg.pc_quality.compare("low") == 0)
				quality = 0.0675;
			else if (arg.pc_quality.compare("lowest") == 0)
				quality = 0.03375;

			return std::max(min_dim, int(max_dim * quality * multiplier));
		}
		else
		{
			Logger::Write(__FUNCTION__, "Cannot compute max image dimensions, going with default depthmap_resolution of 640");
			return 640;	// Sensible default
		}
	}

	RectD GetRasterExtents(XString file_name)
	{
		// Get extents of raster file.
		//

		RectD ext;

		XString out_file = XString::CombinePath(GetProject().GetDroneOutputPath(), "raster_ext.txt");

		QStringList args;
		args.push_back(file_name.c_str());
		AeroLib::RunProgram(tree.prog_gdal_info, args, out_file);

		if (FileExists(out_file))
		{
			//Upper Left  (  321901.221, 5158246.971) (  6d40'35.77"E, 46d33'15.08"N)
			//Lower Left  (  321901.221, 5157669.421) (  6d40'36.57"E, 46d32'56.38"N)
			//Upper Right (  322616.571, 5158246.971) (  6d41' 9.34"E, 46d33'15.76"N)
			//Lower Right (  322616.571, 5157669.421) (  6d41'10.14"E, 46d32'57.06"N)

			TextFile textFile(out_file.c_str(), true);
			for (int i = 0; i < (int)textFile.GetLineCount(); ++i)
			{
				XString line = textFile.GetLine(i).c_str();
				if (line.BeginsWithNoCase("Lower Left"))
				{
					int token_count = line.Tokenize(" ,()");
					if (token_count >= 4)
					{
						ext.x0 = line.GetToken(2).GetDouble();
						ext.y0 = line.GetToken(3).GetDouble();
					}
				}
				if (line.BeginsWithNoCase("Upper Right"))
				{
					int token_count = line.Tokenize(" ,()");
					if (token_count >= 4)
					{
						ext.x1 = line.GetToken(2).GetDouble();
						ext.y1 = line.GetToken(3).GetDouble();
					}
				}
			}
		}

		return ext;
	}
}
