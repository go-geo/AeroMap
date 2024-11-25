/******************************************************************************
 * Copyright (c) 2019, Bradley J Chambers (brad.chambers@gmail.com)
 ****************************************************************************/

 // PDAL implementation of the nearest-neighbor reciprocity criterion presented
 // in T. Weyrich, M. Pauly, R. Keiser, S. Heinzle, S. Scandella, and M. Gross,
 // “Post-processing of Scanned 3D Surface Data,” Proc. Eurographics Symp.
 // Point-Based Graph. 2004, pp. 85–94, 2004.

#include "ReciprocityFilter.h"

#include <pdal/KDIndex.h>
#include <pdal/util/ProgramArgs.h>

#include <string>
#include <thread>
#include <vector>

namespace pdal
{
	using namespace Dimension;

	static StaticPluginInfo const s_info
	{
		"filters.reciprocity",
		"Returns the percentage of neighbors that do NOT have the query point as a "
		"neighbor",
		"http://pdal.io/stages/filters.reciprocity.html"
	};

	CREATE_STATIC_STAGE(ReciprocityFilter, s_info)

	std::string ReciprocityFilter::getName() const
	{
		return s_info.name;
	}

	void ReciprocityFilter::addArgs(ProgramArgs& args)
	{
		args.add("knn", "k-Nearest neighbors", m_knn, 8);
		args.add("threads", "Number of threads used to run this filter", m_threads,	1);
	}

	void ReciprocityFilter::addDimensions(PointLayoutPtr layout)
	{
		layout->registerDim(Id::Reciprocity);
	}

	void ReciprocityFilter::filter(PointView& view)
	{
		point_count_t nloops = view.size();
		std::vector<std::thread> threadList(m_threads);
		for (int t = 0; t < m_threads; t++)
		{
			threadList[t] = std::thread(std::bind(
				[&](const PointId start, const PointId end) {
				for (PointId i = start; i < end; i++)
					setReciprocity(view, i);
			},
				t * nloops / m_threads,
				(t + 1) == m_threads ? nloops : (t + 1) * nloops / m_threads));
		}
		for (auto& t : threadList)
			t.join();
	}

	void ReciprocityFilter::setReciprocity(PointView& view, const PointId& i)
	{
		// Find k-nearest neighbors of i.
		const KD3Index& kdi = view.build3dIndex();
		PointIdList ni = kdi.neighbors(i, m_knn + 1);

		// Initialize number of unidirectional neighbors to 0.
		point_count_t uni(0);

		// Visit each neighbor of i, finding its k-nearest neighbors. If i is
		// not a nearest neighbor of one of its neighbors, increment uni.
		for (PointId const& j : ni)
		{
			// The query point itself will always show up as a neighbor and can
			// be skipped.
			if (j == i)
				continue;

			// Find k-nearest neighbors of j.
			PointIdList nj = kdi.neighbors(j, m_knn + 1);

			// If i is not a neighbor of j, increment uni.
			if (std::find(nj.begin(), nj.end(), i) == nj.end())
				++uni;
		}

		// Compute reciprocity as percentage of neighbors that do NOT contain
		// id as a neighbor.
		double reciprocity = 100.0 * uni / m_knn;
		view.setField(Id::Reciprocity, i, reciprocity);
	}
}
