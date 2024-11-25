/******************************************************************************
* Copyright (c) 2014, Peter J. Gadomski (pete.gadomski@gmail.com)
****************************************************************************/

#include "SbetWriter.h"

#include <pdal/PointView.h>
#include <pdal/util/ProgramArgs.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace pdal
{
	static StaticPluginInfo const s_info
	{
		"writers.sbet",
		"SBET Writer",
		"http://pdal.io/stages/writers.sbet.html",
		{ "sbet" }
	};

	CREATE_STATIC_STAGE(SbetWriter, s_info)

	std::string SbetWriter::getName() const { return s_info.name; }

	void SbetWriter::addArgs(ProgramArgs& args)
	{
		args.add("filename", "Output filename", m_filename).setPositional();
		args.add("angles_are_degrees", "Angles coming into the writer are in degrees", m_anglesAreDegrees, true);
	}

	void SbetWriter::ready(PointTableRef)
	{
		m_stream.reset(new OLeStream(m_filename));
	}

	void SbetWriter::write(const PointViewPtr view)
	{
		auto degreesToRadians = [](double degrees) {
			return degrees * M_PI / 180.0;
		};
		Dimension::IdList dims = sbet::fileDimensions();
		for (PointId idx = 0; idx < view->size(); ++idx)
		{
			for (auto di = dims.begin(); di != dims.end(); ++di)
			{
				// If a dimension doesn't exist, write 0.
				Dimension::Id dim = *di;
				double value = (view->hasDim(dim) ? view->getFieldAs<double>(dim, idx) : 0.0);
				if (m_anglesAreDegrees && sbet::isAngularDimension(dim)) {
					value = degreesToRadians(value);
				}
				*m_stream << value;
			}
		}
	}

	void SbetWriter::done(PointTableRef table)
	{
		getMetadata().addList("filename", m_filename);
	}
}
