// StageMesh.cpp
//

#include "DEM.h"
#include "Mesh.h"
#include "StageMesh.h"

int StageMesh::Run()
{
	// Inputs:
	// Outputs:
	//		+ odm_meshing
	//			odm_25dmesh.mvs
	//			odm_25dmesh.ply
	//			odm_mesh.mvs
	//			odm_mesh.ply
	//

	int status = 0;

	BenchmarkStart("Mesh");

	AeroLib::CreateFolder(tree.mesh_path);

	CreateMesh3D();
	CreateMesh25D();

	BenchmarkStop();

	return status;
}

int StageMesh::CreateMesh3D()
{
	int status = 0;

	// Create full 3D model unless --skip-3dmodel is set
	if (arg.skip_3dmodel == false)
	{
		if ((AeroLib::FileExists(tree.mesh) == false) || Rerun())
		{
			//		Logger::Write(__FUNCTION__, "Writing ODM Mesh file in: %s' % tree.odm_mesh)

			Mesh::screened_poisson_reconstruction(
				tree.filtered_point_cloud, 
				tree.mesh, arg.octree_depth, 
				1.0, 
				arg.mesh_size, 
				4.0, 
				arg.max_concurrency - 1);		// poissonrecon can get stuck on some machines if --threads == all cores
		}
		else
		{
			Logger::Write("Found existing mesh file: '%s'", tree.mesh.c_str());
		}
	}

	return status;
}

int StageMesh::CreateMesh25D()
{
	int status = 0;

	// Always generate a 2.5D mesh
	// unless --use-3dmesh is set.
	if (arg.use_3dmesh == false)
	{
		if ((AeroLib::FileExists(tree.mesh_25d) == false) || Rerun())
		{
			Logger::Write(__FUNCTION__, "Writing 2.5D mesh file: '%s'", tree.mesh_25d.c_str());

			double multiplier = PI / 2.0;
			std::vector<double> radius_steps = DEM::get_dem_radius_steps(tree.filtered_point_cloud_stats, 3, arg.ortho_resolution, multiplier);
			double dsm_resolution = radius_steps[0] / multiplier;

			Logger::Write(__FUNCTION__, "2.5D DSM resolution: %0.2f", dsm_resolution);

			if (arg.fast_orthophoto)
				dsm_resolution *= 8.0;

			XString method;
			if (arg.fast_orthophoto)
				method = "poisson";
			else
				method = "gridded";

			int max_tiles = GetProject().GetImageCount() / 2;

			// is this supposed to be a parm?
			double samples_per_node = 1.0;

			Mesh::create_25dmesh(tree.filtered_point_cloud, tree.mesh_25d, radius_steps, dsm_resolution, arg.octree_depth, samples_per_node, 
				arg.mesh_size, arg.max_concurrency, method, true, max_tiles);
		}
		else
		{
			Logger::Write("Found existing 2.5D mesh file: '%s'", tree.mesh_25d.c_str());
		}
	}

	return status;
}
