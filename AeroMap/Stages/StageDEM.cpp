// StageDEM.cpp
// Generate DTM/DSM.
//

#include "DEM.h"
#include "Gsd.h"
#include "Terrain.h"
#include "TiffFile.h"
#include "StageDEM.h"

int StageDEM::Run()
{
	// Inputs:
	// Outputs:
	//		+ odm_dem
	//			dtm.tif
	//			dsm.tif
	//

	int status = 0;

	BenchmarkStart("DEM");

	XString dem_input = tree.georef_model_laz;
	bool pc_model_found = AeroLib::FileExists(dem_input);
	bool ignore_resolution = false;
	bool pseudo_georeference = false;

	AeroLib::Georef georef = AeroLib::ReadGeoref();
	if (georef.is_valid == false)
	{
		Logger::Write(__FUNCTION__, "Not georeferenced, using ungeoreferenced point cloud.");
		ignore_resolution = true;
		pseudo_georeference = true;
	}

//TODO:
	bool reconstruction_has_gcp = false;
	double resolution = Gsd::cap_resolution(
		arg.dem_resolution,						// resolution
		tree.opensfm_reconstruction,			// reconstruction json
		0.1,									// error estimate
		1.0,									// gsd scaling
		arg.ignore_gsd,							// ignore gsd
		ignore_resolution && arg.ignore_gsd,	// ignore resolution
		reconstruction_has_gcp);				// has ground control points
		
	// define paths and create working directories
	AeroLib::CreateFolder(tree.dem_path);

	if (arg.pc_classify && pc_model_found)
	{
		//pc_classify_marker = XString::CombinePath(odm_dem_root, 'pc_classify_done.txt')

		//if not io.file_exists(pc_classify_marker) or self.rerun():
		//    log.ODM_INFO("Classifying {} using Simple Morphological Filter (1/2)".format(dem_input))
		//    commands.classify(dem_input,
		//                        arg.smrf_scalar,
		//                        arg.smrf_slope,
		//                        arg.smrf_threshold,
		//                        arg.smrf_window
		//                    )

		//    log.ODM_INFO("Classifying {} using OpenPointClass (2/2)".format(dem_input))
		//    classify(dem_input, arg.max_concurrency)

		//    with open(pc_classify_marker, 'w') as f:
		//        f.write('Classify: smrf\n')
		//        f.write('Scalar: {}\n'.format(arg.smrf_scalar))
		//        f.write('Slope: {}\n'.format(arg.smrf_slope))
		//        f.write('Threshold: {}\n'.format(arg.smrf_threshold))
		//        f.write('Window: {}\n'.format(arg.smrf_window))
	}

	if (arg.pc_rectify)
	{
		//    commands.rectify(dem_input)
	}

	// Do we need to process anything here?
	if ((arg.dsm || arg.dtm) && pc_model_found)
	{
		XString dsm_output_filename = XString::CombinePath(tree.dem_path, "dsm.tif");
		XString dtm_output_filename = XString::CombinePath(tree.dem_path, "dtm.tif");

		if ((arg.dtm && AeroLib::FileExists(dtm_output_filename) == false) ||
			(arg.dsm && AeroLib::FileExists(dsm_output_filename) == false) ||
			Rerun())
		{
			std::vector<XString> products;

			if (arg.dsm || (arg.dtm && arg.dem_euclidean_map))
				products.push_back("dsm");
			if (arg.dtm)
				products.push_back("dtm");

			std::vector<double> radius_steps = DEM::get_dem_radius_steps(tree.filtered_point_cloud_stats, arg.dem_gapfill_steps, resolution);

			for (XString product : products)
			{
				XString output_type = "max";
				if (product == "dtm")
					output_type = "idw";

				int max_tiles = GetProject().GetImageCount() / 2;

				DEM::create_dem(
					dem_input,                      // input point cloud
					product,                        // dem type
					radius_steps,
					output_type,
					arg.dem_gapfill_steps > 0,      // gap fill
					tree.dem_path,                  // output directory
					resolution / 100.0,             // TODO: why /100 here?
					arg.max_concurrency,
					4096,                           // max tile size
					arg.dem_decimation,
					arg.dem_euclidean_map,          // keep unfilled copy
					true,                           // apply smoothing
					max_tiles);

		            //dem_geotiff_path = XString::CombinePath(odm_dem_root, "{}.tif".format(product))
		            //bounds_file_path = XString::CombinePath(tree.odm_georeferencing, "odm_georeferenced_model.bounds.gpkg")

				if (arg.crop > 0.0 || arg.boundary.length() > 0)
				{
					// Crop DEM
					// Cropper.crop(bounds_file_path, dem_geotiff_path, utils.get_dem_vars(args), keep_original=not arg.optimize_disk_space)
				}

		        if (arg.dem_euclidean_map)
				{
					//    unfilled_dem_path = io.related_file_path(dem_geotiff_path, postfix=".unfilled")

					if (arg.crop > 0.0 || arg.boundary.length() > 0)
					{
						// Crop unfilled DEM
						// Cropper.crop(bounds_file_path, unfilled_dem_path, utils.get_dem_vars(args), keep_original=not arg.optimize_disk_space)
					}
					//    commands.compute_euclidean_map(unfilled_dem_path,
					//                        io.related_file_path(dem_geotiff_path, postfix=".euclideand"),
					//                        overwrite=True)
				}

		        //if pseudo_georeference:
		        //    pseudogeo.add_pseudo_georeferencing(dem_geotiff_path)

		        //if arg.tiles:
		        //    generate_dem_tiles(dem_geotiff_path, tree.path("%s_tiles" % product), arg.max_concurrency)

		        //if arg.cog:
		        //    convert_to_cogeo(dem_geotiff_path, max_workers=arg.max_concurrency)

				generate_terrain_model(product, resolution);
			}
		}
		else
		{
			Logger::Write(__FUNCTION__, "Found existing outputs in: %s", tree.dem_path.c_str());
		}
	}
	else
	{
		Logger::Write(__FUNCTION__, "DEM will not be generated");
	}

	BenchmarkStop();

	return status;
}

void StageDEM::generate_terrain_model(XString product, double resolution)
{
	// Using the tif file as input, generate an AeroMap
	// terrain model.
	//
	// Inputs:
	//		product = dtm, dsm
	//		resolution = capped dem resolution
	//

	XString tiff_file = XString::CombinePath(tree.dem_path, product + ".tif");
	if (AeroLib::FileExists(tiff_file))
	{
		// get geo extents of input tiff file
		RectD ext = AeroLib::GetRasterExtents(tiff_file);

		//TODO:
		//a couple things going on here:
		// 1. we scale down by totally arbitrary factor - prob should base on actual size vs desired size
		// 	(say largest dim < 1000)
		// 2. however we scale it, adjust the resolution value below to account for that

		int scale_percent = 10;
		XString tiff_file_small = XString::CombinePath(tree.dem_path, product + XString::Format(".%d.tif", scale_percent).c_str());

		// these can be huge, scale down to workable size
		// Scale to 10% size
		QStringList args;
		args.push_back("-co");
		args.push_back("NUM_THREADS=ALL_CPUS");
		args.push_back("-co");
		args.push_back("BIGTIFF=IF_SAFER");
		args.push_back("-outsize");
		args.push_back(XString::Format("%d%%", scale_percent).c_str());
		args.push_back("0");
		args.push_back(tiff_file.c_str());
		args.push_back(tiff_file_small.c_str());
		AeroLib::RunProgram(tree.prog_gdal_translate, args);

		cv::Mat mat = cv::imread(tiff_file_small.c_str(), cv::IMREAD_LOAD_GDAL);
		int rows = mat.rows;
		int cols = mat.cols;

		// we know this value because the input file created by us, but would be ideal to query it
		const float NODATA = -9999.0;
		if (mat.rows > 0 && mat.cols > 0)
		{
			XString terrain_file = XString::CombinePath(tree.dem_path, product + ".dat");
			AeroLib::RemoveFile(terrain_file);

			if (Terrain::Create(terrain_file.c_str(), rows, cols, resolution))
			{
				Terrain* pTerrain = new Terrain(terrain_file.c_str());
				pTerrain->SetGeoExtents(ext.x0, ext.y0, ext.x1, ext.y1);
				AeroLib::Georef georef = AeroLib::ReadGeoref();
				if (georef.is_valid)
					pTerrain->SetProjection(GIS::Projection::UTM, georef.utm_zone);

				// get actual terrain min/max values
				double min_elev = NODATA;
				double max_elev = NODATA;
				for (int col = 0; col < cols; ++col)
				{
					for (int row = 0; row < rows; ++row)
					{
						float elev = mat.at<float>(row, col);
						if (elev != NODATA)
						{
							if (min_elev == NODATA)
							{
								min_elev = elev;
								max_elev = elev;
							}
							else
							{
								if (min_elev > elev)
									min_elev = elev;
								if (max_elev < elev)
									max_elev = elev;
							}
						}
					}
				}

				// elevation to use outside valid region
				float elev_nodata = min_elev - 25.0;

				for (int col = 0; col < cols; ++col)
				{
					for (int row = 0; row < rows; ++row)
					{
						float elev = mat.at<float>(row, col);

						int dest_row = rows - row - 1;
						if (elev != NODATA)
						{
							pTerrain->SetHeight(dest_row, col, elev);
						}
						else
						{
							pTerrain->SetHeight(dest_row, col, elev_nodata);
						}
					}
				}

				pTerrain->SaveData();

				delete pTerrain;
			}
		}
	}
}