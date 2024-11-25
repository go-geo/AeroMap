/******************************************************************************
* Copyright (c) 2014,  Hobu Inc., hobu@hobu.co
****************************************************************************/

#include <pdal/DbWriter.h>
#include <pdal/util/Utils.hpp>

namespace pdal
{
	void DbWriter::addArgs(ProgramArgs& args)
	{
		args.add("output_dims", "Output dimensions", m_outputDims);
		m_scaling.addArgs(args);
	}

	// Build the list of dimensions for the output schema.
	// Placing this here allows validation of dimensions before execution begins.
	void DbWriter::prepared(PointTableRef table)
	{
		using namespace Dimension;

		PointLayoutPtr layout = table.layout();

		if (m_outputDims.empty())
		{
			for (auto& dimType : layout->dimTypes())
				m_dbDims.push_back(XMLDim(dimType, layout->dimName(dimType.m_id)));
			return;
		}

		DimTypeList dims;
		for (std::string& s : m_outputDims)
		{
			DimType dt = layout->findDimType(s);
			if (dt.m_id == Id::Unknown)
			{
				std::ostringstream oss;
				oss << "Invalid dimension '" << s << "' specified for "
					"'output_dims' option.";
				throw pdal_error(oss.str());
			}
			m_dbDims.push_back(XMLDim(dt, layout->dimName(dt.m_id)));
		}
	}

	void DbWriter::ready(PointTableRef /*table*/)
	{
		using namespace Dimension;

		// Determine if X, Y and Z values should be written as Signed32 along with
		// a scale factor and offset instead of being written as Double.
		m_locationScaling = m_scaling.nonstandard();

		auto cmp = [](const XMLDim& d1, const XMLDim& d2) -> bool
		{
			long id1 = Utils::toNative(d1.m_dimType.m_id);
			long id2 = Utils::toNative(d2.m_dimType.m_id);

			const auto isXyz([](long native)->bool
			{
				const Id e(static_cast<Id>(native));
				return (e == Id::X || e == Id::Y || e == Id::Z);
			});

			// Put X, Y and Z at the end of the list.
			if (isXyz(id1))
				id1 += 1000000;
			if (isXyz(id2))
				id2 += 1000000;
			return id1 < id2;
		};

		// Sort the dimensions so that X, Y & Z are at the end.
		std::sort(m_dbDims.begin(), m_dbDims.end(), cmp);

		// Suck the dimTypes out of the dbDims so that they can be used to
		// retrieve data from the point table.
		// Set the packed type into the dbDim if necessary and save off the
		// index of X, Y and Z for location scaling.
		m_dimTypes.clear();
		m_xOffsets = std::make_pair(-1, -1);
		m_yOffsets = std::make_pair(-1, -1);
		m_zOffsets = std::make_pair(-1, -1);
		m_packedPointSize = 0;
		m_dbPointSize = 0;
		for (auto& xmlDim : m_dbDims)
		{
			m_dimTypes.push_back(xmlDim.m_dimType);
			DimType& dt = m_dimTypes.back();
			// Dim types are stored in the map to allow fast access in readField.
			m_dimMap[Utils::toNative(dt.m_id)] = dt;

			if (m_locationScaling)
			{
				if (xmlDim.m_dimType.m_id == Id::X)
				{
					xmlDim.m_dimType.m_xform = m_scaling.m_xXform;
					xmlDim.m_dimType.m_type = Type::Signed32;
					m_xOffsets =
						std::make_pair((int)m_packedPointSize, (int)m_dbPointSize);
				}
				if (xmlDim.m_dimType.m_id == Id::Y)
				{
					xmlDim.m_dimType.m_xform = m_scaling.m_yXform;
					xmlDim.m_dimType.m_type = Type::Signed32;
					m_yOffsets =
						std::make_pair((int)m_packedPointSize, (int)m_dbPointSize);
				}
				if (xmlDim.m_dimType.m_id == Id::Z)
				{
					xmlDim.m_dimType.m_xform = m_scaling.m_zXform;
					xmlDim.m_dimType.m_type = Type::Signed32;
					m_zOffsets =
						std::make_pair((int)m_packedPointSize, (int)m_dbPointSize);
				}
			}
			m_packedPointSize += Dimension::size(dt.m_type);
			m_dbPointSize += Dimension::size(xmlDim.m_dimType.m_type);
		}
	}

	/// Make sure that computed offsets are stored in the schema.
	void DbWriter::setAutoXForm(const PointViewPtr view)
	{
		using namespace Dimension;

		m_scaling.setAutoXForm({ view });
		for (auto& xmlDim : m_dbDims)
		{
			if (xmlDim.m_dimType.m_id == Id::X)
				xmlDim.m_dimType.m_xform = m_scaling.m_xXform;
			if (xmlDim.m_dimType.m_id == Id::Y)
				xmlDim.m_dimType.m_xform = m_scaling.m_yXform;
			if (xmlDim.m_dimType.m_id == Id::Z)
				xmlDim.m_dimType.m_xform = m_scaling.m_zXform;
		}
	}

	/// Read a field from a PointView and write its value as formatted for output
	/// to the DB schema to the location as requested.
	/// \param[in] view     PointView to read from.
	/// \param[in] pos      Location in which to store field value.
	/// \param[in] id       ID of the dimension to read.
	/// \param[in] idx      Index of point to read.
	/// \return  Size of field as read.
	size_t DbWriter::readField(const PointView& view, char* pos,
		Dimension::Id id, PointId idx)
	{
		using namespace Dimension;

		DimType& dt = m_dimMap[(int)id];
		size_t size = Dimension::size(dt.m_type);

		// Using the ID instead of a dimType as the arugment hides the complication
		// of the "type" of the dimension to retrieve in the case of location
		// scaling.
		view.getField(pos, id, dt.m_type, idx);

		auto iconvert = [pos](const XForm& xform, Dimension::Id dim)
		{
			double d;
			int32_t i;

			memcpy(&d, pos, sizeof(double));

			d = xform.toScaled(d);
			if (!Utils::numericCast(d, i))
			{
				std::ostringstream oss;
				oss << "Unable to convert double to int32 for packed DB output: ";
				oss << Dimension::name(dim) << ": (" << d << ").";
				throw pdal_error(oss.str());
			}
			memcpy(pos, &i, sizeof(int32_t));
		};

		if (m_locationScaling)
		{
			// For X, Y or Z.
			if (id == Id::X)
			{
				iconvert(m_scaling.m_xXform, Id::X);
				size = sizeof(int32_t);
			}
			else if (id == Id::Y)
			{
				iconvert(m_scaling.m_yXform, Id::Y);
				size = sizeof(int32_t);
			}
			else if (id == Id::Z)
			{
				iconvert(m_scaling.m_zXform, Id::Z);
				size = sizeof(int32_t);
			}
		}
		return size;
	}

	/// Read a point's data packed into a buffer.
	/// \param[in] view  PointView to read from.
	/// \param[in] idx  Index of point to read.
	/// \param[in] outbuf  Buffer to write to.
	/// \return  Number of bytes written to buffer.
	size_t DbWriter::readPoint(const PointView& view, PointId idx, char* outbuf)
	{
		using namespace Dimension;

		// Read the data for the output dimensions from the view into the outbuf.
		view.getPackedPoint(m_dimTypes, idx, outbuf);

		auto iconvert = [](const XForm& xform, Id dim,
			const char* inpos, char* outpos)
		{
			double d;
			int32_t i;

			memcpy(&d, inpos, sizeof(double));
			d = xform.toScaled(d);
			if (!Utils::numericCast(d, i))
			{
				std::ostringstream oss;
				oss << "Unable to convert double to int32 for packed DB output: ";
				oss << Dimension::name(dim) << ": (" << d << ").";
				throw pdal_error(oss.str());
			}
			memcpy(outpos, &i, sizeof(int32_t));
		};

		if (m_xOffsets.first >= 0)
			iconvert(m_scaling.m_xXform, Id::X, outbuf + (size_t)m_xOffsets.first,
				outbuf + (size_t)m_xOffsets.second);
		if (m_yOffsets.first >= 0)
			iconvert(m_scaling.m_yXform, Id::Y, outbuf + (size_t)m_yOffsets.first,
				outbuf + (size_t)m_yOffsets.second);
		if (m_zOffsets.first >= 0)
			iconvert(m_scaling.m_zXform, Id::Z, outbuf + (size_t)m_zOffsets.first,
				outbuf + (size_t)m_zOffsets.second);
		return m_dbPointSize;
	}
}
