// StageSFM.cpp
// Structure from motion.
//

#include "Gsd.h"
#include "Multispectral.h"
#include "TextFile.h"
#include "Thermal.h"
#include "StageSFM.h"

int StageSFM::Run()
{
	// Inputs:
	// Outputs:
	//		+ opensfm
	//			+ exif
	//				[file name*].exif		
	//			camera_models.json
	//			config.yaml					
	//			image_list.txt
	//			reference_lla.json
	//		cameras.json
	//

	int status = 0;

	BenchmarkStart("OpenSFM");

	Setup();

	// OpenSFM stages

	DetectFeatures();
	MatchFeatures();
	CreateTracks();
	Reconstruct();

	// Export reconstruction stats

	ComputeStats();
	ExportGeocoords();

	// Make sure undistorted max image size capped by the depthmap-resolution arg,
	// since the undistorted images are used for MVS
	int max_size_gsd = Gsd::image_max_size(GetProject().GetImageList(), arg.ortho_resolution, tree.opensfm_reconstruction, 0.5, arg.ignore_gsd, GetProject().GetReconstruction()->has_gcp());
	int max_size_depth = AeroLib::get_depthmap_resolution(GetProject().GetImageList());
	int undist_image_max_size = std::max(max_size_gsd, max_size_depth);
	// put in project so available globally
	GetProject().SetUndistMaxImageSize(undist_image_max_size);
	GetApp()->LogWrite("Undistorted max image size: %d", undist_image_max_size);
	
	// Undistorted images will be used for texturing / MVS

	UpdateConfig();

	if (GetProject().GetReconstruction()->is_multi())
	{
		Photo* largest_photo = Photo::find_largest_photo(GetProject().GetImageList());
		for (Photo* p : GetProject().GetImageList())
		{
			if (p->is_thermal())
				Thermal::resize_to_match(p, largest_photo);
		}
	}

	if (arg.radiometric_calibration != "none")
	{
		for (Photo* p : GetProject().GetImageList())
		{
			//if (p->is_thermal())
			//	thermal.dn_to_temperature(photo, image, tree.dataset_raw)
			//else
			//	multispectral.dn_to_reflectance(photo, image, use_sun_sensor = args.radiometric_calibration == "camera+sun")
		}
	}

	if (GetProject().GetReconstruction()->is_multi())
	{
		// Undistort only secondary bands
		// (why isn't primary band undistorted?)
		XString primary_band_name = Multispectral::get_primary_band_name(GetProject().GetReconstruction()->GetMulti(), arg.primary_band.c_str());
		
		std::vector<Photo*> image_list_override;
		for (auto p : GetProject().GetImageList())
		{
			// if not a primary band image
			if (primary_band_name.CompareNoCase(p->GetBandName()) == false)
			{
				image_list_override.push_back(p);
			}
		}

		// We backup the original reconstruction.json, tracks.csv
		// then we augment them by duplicating the primary band
		// camera shots with each band, so that exports, undistortion,
		// etc. include all bands
		// We finally restore the original files later

		//	s2p, p2s = None, None

		XString added_shots_file = XString::CombinePath(tree.opensfm, "added_shots_done.txt");
		if ((AeroLib::FileExists(added_shots_file) == false) || Rerun())
		{
			//s2p, p2s = multispectral.compute_band_maps(reconstruction.multi_camera, primary_band_name)

			if (arg.skip_band_alignment == false)
			{
				//	alignment_info = multispectral.compute_alignment_matrices(reconstruction.multi_camera, primary_band_name, tree.dataset_raw, s2p, p2s, max_concurrency=args.max_concurrency)
			}
			else
			{
				//	log.ODM_WARNING("Skipping band alignment")
				//	alignment_info = {}
			}

			Logger::Write(__FUNCTION__, "Adding shots to reconstruction");

			//octx.backup_reconstruction()
			//octx.add_shots_to_reconstruction(p2s)
			//octx.touch(added_shots_file)
		}

		// undistort_pipeline.append(align_to_primary_band)
	}

	Undistort();
	ExportVisualSfm();

	// Skip dense reconstruction if necessary and export
	// sparse reconstruction instead
	if (arg.fast_orthophoto)
	{
		XString output_file = XString::CombinePath(tree.opensfm, "reconstruction.ply");
		if ((AeroLib::FileExists(output_file) == false) || Rerun())
			ExportPly();
		else
			Logger::Write(__FUNCTION__, "Found a valid PLY reconstruction in '%s'", output_file.c_str());
	}

	BenchmarkStop();

	return status;
}

void StageSFM::DetectFeatures()
{
	GetApp()->LogWrite("OpenSFM: Detect features...");
	QStringList args;
	args.push_back("detect_features");
	args.push_back(tree.opensfm.c_str());
	AeroLib::RunProgram(tree.prog_opensfm, args);
	// cmd: opensfm detect_features "d:/test_odm/opensfm"
}

void StageSFM::MatchFeatures()
{
	GetApp()->LogWrite("OpenSFM: Match features...");
	QStringList args;
	args.push_back("match_features");
	args.push_back(tree.opensfm.c_str());
	AeroLib::RunProgram(tree.prog_opensfm, args);
	// cmd: opensfm match_features "d:/test_odm/opensfm"
}

void StageSFM::CreateTracks()
{
	GetApp()->LogWrite("OpenSFM: Create tracks...");
	QStringList args;
	args.push_back("create_tracks");
	args.push_back(tree.opensfm.c_str());
	AeroLib::RunProgram(tree.prog_opensfm, args);
	// cmd: opensfm create_tracks "d:/test_odm/opensfm"
}

void StageSFM::Reconstruct()
{
	GetApp()->LogWrite("OpenSFM: Reconstruct...");
	QStringList args;
	args.push_back("reconstruct");
	args.push_back(tree.opensfm.c_str());
	AeroLib::RunProgram(tree.prog_opensfm, args);
	// cmd: opensfm reconstruct "d:/test_odm/opensfm"
}

void StageSFM::ComputeStats()
{
	GetApp()->LogWrite("OpenSFM: Compute statistics...");
	QStringList args;
	args.push_back("compute_statistics");
	args.push_back("--diagram_max_points");
	args.push_back("100000");
	args.push_back(tree.opensfm.c_str());
	AeroLib::RunProgram(tree.prog_opensfm, args);
	// cmd: opensfm compute_statistics --diagram_max_points 100000 "d:/test_odm/opensfm"
}

void StageSFM::ExportGeocoords()
{
	AeroLib::Georef georef = AeroLib::ReadGeoref();
	if (georef.is_valid)
	{
		GetApp()->LogWrite("OpenSFM: Export geocoords...");
		QStringList args;
		args.push_back("export_geocoords");
		args.push_back("--reconstruction");
		args.push_back("--proj");
		args.push_back(XString::Format("+proj=utm +zone=%d +datum=WGS84 +units=m +no_defs +type=crs", georef.utm_zone).c_str());
		args.push_back("--offset-x");
		args.push_back(XString::Format("%0.1f", georef.x).c_str());
		args.push_back("--offset-y");
		args.push_back(XString::Format("%0.1f", georef.y).c_str());
		args.push_back(tree.opensfm.c_str());
		AeroLib::RunProgram(tree.prog_opensfm, args);
		// cmd: opensfm export_geocoords --reconstruction 
		//			--proj "+proj=utm +zone=32 +datum=WGS84 +units=m +no_defs +type=crs"
		//			--offset-x 322263.0 --offset-y 5157982.0 "d:/test_odm/opensfm"
	}
}

void StageSFM::ExportPly()
{
	GetApp()->LogWrite("OpenSFM: Export ply...");
	QStringList args;
	args.push_back("export_ply");
	args.push_back("--no-cameras");
	args.push_back("--point-num-views");
	args.push_back(tree.opensfm.c_str());
	AeroLib::RunProgram(tree.prog_opensfm, args);
	// cmd: opensfm export_ply --no-cameras --point-num-views "d:\test_aero\opensfm"
}

void StageSFM::ExportVisualSfm()
{
	GetApp()->LogWrite("OpenSFM: Export visualsfm...");
	QStringList args;
	args.push_back("export_visualsfm");
	args.push_back("--points");
	args.push_back(tree.opensfm.c_str());
	AeroLib::RunProgram(tree.prog_opensfm, args);
	// cmd: opensfm export_visualsfm --points "d:/test_odm/opensfm"
}

void StageSFM::Undistort()
{
	GetApp()->LogWrite("OpenSFM: Undistort...");
	QStringList args;
	args.push_back("undistort_aero");
	args.push_back(tree.opensfm.c_str());
	AeroLib::RunProgram(tree.prog_opensfm, args);
}

int StageSFM::Setup()
{
	int status = 0;

	WriteExif();
	WriteImageListText();
	WriteCameraModelsJson();
	WriteReferenceLLA();
	WriteConfigYaml();

	WriteCamerasJson();

	return status;
}

int StageSFM::WriteExif()
{
	// Create & populate opensfm/exif folder
	//

	int status = 0;

	XString exif_path = XString::CombinePath(tree.opensfm, "exif");
	AeroLib::CreateFolder(exif_path);

	for (Photo* image : GetProject().GetImageList())
	{
		// file names look like: 'IMG_0428.JPG.exif'
		XString file_name = XString::CombinePath(exif_path, image->GetFileName().GetFileName() + ".exif");
		FILE* pFile = fopen(file_name.c_str(), "wt");
		if (pFile)
		{
			fprintf(pFile, "{\n");
			fprintf(pFile, "    \"make\": \"%s\",\n", image->GetMake().c_str());
			fprintf(pFile, "    \"model\": \"%s\",\n", image->GetModel().c_str());
			fprintf(pFile, "    \"width\": %d,\n", image->GetWidth());
			fprintf(pFile, "    \"height\": %d,\n", image->GetHeight());
			fprintf(pFile, "    \"projection_type\": \"brown\",\n");			// in/derived-from exif?
			fprintf(pFile, "    \"focal_ratio\": %0.16f,\n", image->GetFocalRatio());
			fprintf(pFile, "    \"orientation\": %d,\n", image->GetOrientation());
			fprintf(pFile, "    \"capture_time\": %I64u.0,\n", image->GetCaptureEpoch());		//1299256936.0
			fprintf(pFile, "    \"gps\": {\n");
			fprintf(pFile, "        \"latitude\": %0.15f,\n", image->GetLatitude());		//46.553156600003355
			fprintf(pFile, "        \"longitude\": %0.15f,\n", image->GetLongitude());
			fprintf(pFile, "        \"altitude\": %0.12f,\n", image->GetAltitude());		//980.296992481203
			fprintf(pFile, "        \"dop\": 10.0\n");		//TODO: odm gets 10, i get 0
			fprintf(pFile, "    },\n");
			fprintf(pFile, "    \"camera\": \"%s\"\n", image->GetCameraStr(true).c_str());
			fprintf(pFile, "}\n");

			fclose(pFile);
		}
	}

	return status;
}

int StageSFM::WriteImageListText()
{
	// Write 'opensfm/image_list.txt'
	//

	int status = 0;

	XString file_name = XString::CombinePath(tree.opensfm, "image_list.txt");
	FILE* pFile = fopen(file_name.c_str(), "wt");
	if (pFile)
	{
		for (Photo* image : GetProject().GetImageList())
		{
			// full path name, opensfm uses this to find images
			fprintf(pFile, "%s\n", image->GetFileName().c_str());
		}

		fclose(pFile);
	}

	return status;
}

int StageSFM::WriteCameraModelsJson()
{
	// Write 'opensfm/camera_models.json'
	//
	// Believe intent is 1 entry for each camera detected in input
	// image dataset. Making simplifying assumption all images 
	// captured with same camera.
	//

	int status = 0;

	XString file_name = XString::CombinePath(tree.opensfm, "camera_models.json");
	FILE* pFile = fopen(file_name.c_str(), "wt");
	if (pFile)
	{
		Photo* image = GetProject().GetImageList()[0];

		fprintf(pFile, "{\n");
		fprintf(pFile, "    \"%s\": {\n", image->GetCameraStr(true).c_str());
		fprintf(pFile, "        \"projection_type\": \"brown\",\n");		//TODO: in/derived from exif?
		fprintf(pFile, "        \"width\": %d,\n", image->GetWidth());
		fprintf(pFile, "        \"height\": %d,\n", image->GetHeight());
		fprintf(pFile, "        \"focal_x\": %0.16f,\n", image->GetFocalRatio());
		fprintf(pFile, "        \"focal_y\": %0.16f,\n", image->GetFocalRatio());	// any scenario where x & y could differ?
		fprintf(pFile, "        \"c_x\": 0.0,\n");
		fprintf(pFile, "        \"c_y\": 0.0,\n");
		fprintf(pFile, "        \"k1\": 0.0,\n");	//TODO: how are these calculated
		fprintf(pFile, "        \"k2\": 0.0,\n");
		fprintf(pFile, "        \"p1\": 0.0,\n");
		fprintf(pFile, "        \"p2\": 0.0,\n");
		fprintf(pFile, "        \"k3\": 0.0\n");
		fprintf(pFile, "    }\n");
		fprintf(pFile, "}\n");

		fclose(pFile);
	}

	return status;
}

int StageSFM::WriteReferenceLLA()
{
	// Write 'opensfm/reference_lla.json'
	//
	// This is the center of the image field converted
	// back to lat/lon.
	//

	int status = 0;

	AeroLib::Georef georef = AeroLib::ReadGeoref();
	if (georef.is_valid)
	{
		XString file_name = XString::CombinePath(tree.opensfm, "reference_lla.json");
		FILE* pFile = fopen(file_name.c_str(), "wt");
		if (pFile)
		{
			double lat, lon;
			GIS::XYToLatLon_UTM(georef.utm_zone, georef.hemi, georef.x, georef.y, lat, lon, GIS::Ellipsoid::WGS_84);

			fprintf(pFile, "{\n");
			fprintf(pFile, "	\"latitude\": %0.14f,\n", lat);
			fprintf(pFile, "	\"longitude\": %0.14f, \n", lon);
			fprintf(pFile, "	\"altitude\": 0.0\n");
			fprintf(pFile, "}\n");

			fclose(pFile);
		}
	}

	return status;
}

int StageSFM::WriteConfigYaml()
{
	// Write 'opensfm/config.yaml'
	//

	int status = 0;

	XString file_name = XString::CombinePath(tree.opensfm, "config.yaml");
	FILE* pFile = fopen(file_name.c_str(), "wt");
	if (pFile)
	{
		//TODO:
		//understand source & meaning of each of these settings
		fprintf(pFile, "align_method: auto\n");
		fprintf(pFile, "align_orientation_prior: vertical\n");
		fprintf(pFile, "bundle_outlier_filtering_type: AUTO\n");
		fprintf(pFile, "feature_min_frames: 10000\n");
		fprintf(pFile, "feature_process_size: 2000\n");
		fprintf(pFile, "feature_type: SIFT_GPU\n");
		fprintf(pFile, "flann_algorithm: KDTREE\n");
		fprintf(pFile, "local_bundle_radius: 0\n");
		fprintf(pFile, "matcher_type: FLANN\n");
		fprintf(pFile, "matching_gps_distance: 0\n");
		fprintf(pFile, "matching_gps_neighbors: 0\n");
		fprintf(pFile, "matching_graph_rounds: 50\n");
		fprintf(pFile, "optimize_camera_parameters: true\n");
		fprintf(pFile, "processes: 16\n");
		fprintf(pFile, "reconstruction_algorithm: incremental\n");
		fprintf(pFile, "retriangulation_ratio: 2\n");
		fprintf(pFile, "sift_peak_threshold: 0.066\n");
		fprintf(pFile, "triangulation_type: ROBUST\n");
		fprintf(pFile, "undistorted_image_format: tif\n");
		fprintf(pFile, "undistorted_image_max_size: 4000\n");
		fprintf(pFile, "use_altitude_tag: true\n");
		fprintf(pFile, "use_exif_size: false\n");

		fclose(pFile);
	}

	return status;
}

int StageSFM::WriteCamerasJson()
{
	// Write 'cameras.json'
	//
	// Same format as opensfm/camera_models.json with
	// potentially differing data.
	//

	int status = 0;

	XString file_name = XString::CombinePath(GetProject().GetDroneOutputPath(), "cameras.json");
	FILE* pFile = fopen(file_name.c_str(), "wt");
	if (pFile)
	{
		Photo* image = GetProject().GetImageList()[0];

		fprintf(pFile, "{\n");
		fprintf(pFile, "    \"%s\": {\n", image->GetCameraStr(false).c_str());
		fprintf(pFile, "        \"projection_type\": \"brown\",\n");		//TODO: in/derived from exif?
		fprintf(pFile, "        \"width\": %d,\n", image->GetWidth());
		fprintf(pFile, "        \"height\": %d,\n", image->GetHeight());
		fprintf(pFile, "        \"focal_x\": %0.16f,\n", image->GetFocalRatio());
		fprintf(pFile, "        \"focal_y\": %0.16f,\n", image->GetFocalRatio());	// any scenario where x & y could differ?
		fprintf(pFile, "        \"c_x\": 0.0,\n");
		fprintf(pFile, "        \"c_y\": 0.0,\n");
		fprintf(pFile, "        \"k1\": 0.0,\n");	//TODO: how are these calculated
		fprintf(pFile, "        \"k2\": 0.0,\n");
		fprintf(pFile, "        \"p1\": 0.0,\n");
		fprintf(pFile, "        \"p2\": 0.0,\n");
		fprintf(pFile, "        \"k3\": 0.0\n");
		fprintf(pFile, "    }\n");
		fprintf(pFile, "}\n");

		fclose(pFile);
	}

	return status;
}

void StageSFM::UpdateConfig()
{
	// update config.yaml

	XString file_name = XString::CombinePath(tree.opensfm, "config.yaml");

	GetApp()->LogWrite("Updating '%s'...", file_name.c_str());

	TextFile textFile(file_name.c_str());

	FILE* pOutFile = fopen(file_name.c_str(), "wt");
	if (pOutFile)
	{
		bool undist_updated = false;
		for (int i = 0; i < (int)textFile.GetLineCount(); ++i)
		{
			XString line = textFile.GetLine(i).c_str();
			if (line.BeginsWithNoCase("undistorted_image_max_size"))
			{
				fprintf(pOutFile, "undistorted_image_max_size: %d\n", GetProject().GetUndistMaxImageSize());
				undist_updated = true;
			}
			else
			{
				fprintf(pOutFile, "%s\n", line.c_str());
			}
		}

		if (undist_updated == false)
			fprintf(pOutFile, "undistorted_image_max_size: %d\n", GetProject().GetUndistMaxImageSize());

		fclose(pOutFile);
	}
}
