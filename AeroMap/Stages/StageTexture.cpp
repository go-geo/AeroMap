// StageTexture.cpp
// Apply textures
//

#include "StageTexture.h"

int StageTexture::Run()
{
	// Inputs:
	// Outputs:
	//		+ odm_texturing
	//			odm_textured_model_geo.conf
	//			odm_textured_model_geo.mtl
	//			odm_textured_model_geo.obj
	//		+ odm_texturing_25d
	//			odm_textured_model_geo.conf
	//			odm_textured_model_geo.mtl
	//			odm_textured_model_geo.obj
	//

	//TODO:
	//'intermediate': '--no_intermediate_results' if (r['labeling_file'] or not reconstruction.multi_camera) else '',

	int status = 0;

	BenchmarkStart("Texture");

	// odm used reconstruction image list
	int max_dim = Photo::find_largest_photo_dim(GetProject().GetImageList());
	m_max_texture_size = 8 * 1024;	// default

	if (max_dim > 8000)
	{
		m_max_texture_size *= 3;
		Logger::Write(__FUNCTION__, "Large input images (%d pixels), increasing maximum texture size to %d.", max_dim, m_max_texture_size);
	}

	//if reconstruction.multi_camera:

	//	for band in reconstruction.multi_camera:
	//		primary = band['name'] == get_primary_band_name(reconstruction.multi_camera, args.primary_band)
	//		nvm_file = os.path.join(tree.opensfm, "undistorted", "reconstruction_%s.nvm" % band['name'].lower())
	//		add_run(nvm_file, primary, band['name'].lower())

	//	# Sort to make sure primary band is processed first
	//	nonloc.runs.sort(key=lambda r: r['primary'], reverse=True)
	//else:
	//	add_run(tree.opensfm_reconstruction_nvm)

	TextureModel3D();
	TextureModel25D();

	BenchmarkStop();

	return status;
}

int StageTexture::TextureModel3D()
{
	int status = 0;
	
	AeroLib::CreateFolder(tree.texture_path);

	XString textured_model_obj = XString::CombinePath(tree.texture_path, tree.textured_model_obj);
	if ((AeroLib::FileExists(textured_model_obj) == false) || Rerun())
	{
		GetApp()->LogWrite("Writing MVS textured file: '%s'", textured_model_obj.c_str());

		XString out_prefix = XString::CombinePath(tree.texture_path, "odm_textured_model_geo");

		QStringList args;
		args.push_back(tree.opensfm_reconstruction_nvm.c_str());		// input scene
		args.push_back(tree.mesh.c_str());								// input mesh
		args.push_back(out_prefix.c_str());								// output prefix
		args.push_back("-d");					// Data term: {area, gmi}
		args.push_back("gmi");
		args.push_back("-o");					// Photometric outlier (pedestrians etc.) removal method: {none, gauss_damping, gauss_clamping}
		args.push_back("gauss_clamping");		
		args.push_back("-t");
		args.push_back("none");					// Tone mapping method: {none, gamma}
		args.push_back("--no_intermediate_results");
		args.push_back(XString::Format("--max_texture_size=%d", m_max_texture_size).c_str());
		AeroLib::RunProgram(tree.prog_recon_tex, args);
		// cmd: texrecon 
		//			"d:/test_odm/opensfm/undistorted/reconstruction.nvm" 
		//			"d:/test_odm/odm_meshing/odm_mesh.ply" 
		//			"d:/test_odm/odm_texturing/odm_textured_model_geo" 
		//			-d gmi -o gauss_clamping -t none --no_intermediate_results --max_texture_size=8192
	}
	else
	{
		Logger::Write(__FUNCTION__, "Found existing textured model: '%s'", textured_model_obj.c_str());
	}
	
	return status;
}

int StageTexture::TextureModel25D()
{
	int status = 0;

	AeroLib::CreateFolder(tree.texture_25d_path);

	XString textured_model_obj = XString::CombinePath(tree.texture_25d_path, tree.textured_model_obj);
	if ((AeroLib::FileExists(textured_model_obj) == false) || Rerun())
	{
		GetApp()->LogWrite("Writing MVS textured file: '%s'", textured_model_obj.c_str());

		XString out_prefix = XString::CombinePath(tree.texture_25d_path, "odm_textured_model_geo");

		QStringList args;
		args.push_back(tree.opensfm_reconstruction_nvm.c_str());		// input scene
		args.push_back(tree.mesh_25d.c_str());							// input mesh
		args.push_back(out_prefix.c_str());								// output prefix
		args.push_back("-d");
		args.push_back("gmi");
		args.push_back("-o");
		args.push_back("gauss_clamping");
		args.push_back("-t");
		args.push_back("none");
		args.push_back("--nadir_mode");
		args.push_back("--no_intermediate_results");
		args.push_back(XString::Format("--max_texture_size=%d", m_max_texture_size).c_str());
		AeroLib::RunProgram(tree.prog_recon_tex, args);
		// cmd: texrecon 
		//			"d:/test_odm/opensfm/undistorted/reconstruction.nvm" 
		//			"d:/test_odm/odm_meshing/odm_25dmesh.ply" 
		//			"d:/test_odm/odm_texturing_25d/odm_textured_model_geo" 
		//			-d gmi -o gauss_clamping -t none --no_intermediate_results
		//		    --nadir_mode
		//			--max_texture_size=8192
	}
	else
	{
		Logger::Write(__FUNCTION__, "Found existing textured model: '%s'", textured_model_obj.c_str());
	}

	return status;
}
