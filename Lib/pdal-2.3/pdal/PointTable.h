#ifndef POINTTABLE_H
#define POINTTABLE_H

#include <algorithm>
#include <list>
#include <vector>

#include "pdal/SpatialReference.h"
#include "pdal/Dimension.h"
#include "pdal/PointContainer.h"
#include "pdal/PointLayout.h"
#include "pdal/Metadata.h"

namespace pdal
{
	class ArtifactManager;

	class BasePointTable : public PointContainer
	{
		FRIEND_TEST(PointTable, srs);
		friend class PointView;

	protected:
		BasePointTable(PointLayout& layout);

	public:
		virtual ~BasePointTable();

		// Layout operations.
		virtual PointLayoutPtr layout() const
		{
			return &m_layoutRef;
		}

		// Metadata operations.
		MetadataNode metadata()
		{
			return m_metadata->getNode();
		}
		virtual void finalize()
		{
			m_layoutRef.finalize();
		}
		void setSpatialReference(const SpatialReference& srs)
		{
			clearSpatialReferences();
			addSpatialReference(srs);
		}
		void clearSpatialReferences()
		{
			m_spatialRefs.clear();
		}
		void addSpatialReference(const SpatialReference& srs);
		bool spatialReferenceUnique() const
		{
			return m_spatialRefs.size() <= 1;
		}
		SpatialReference spatialReference() const
		{
			return spatialReferenceUnique() ? anySpatialReference() :
				SpatialReference();
		}
		SpatialReference anySpatialReference() const
		{
			return m_spatialRefs.size() ?
				*m_spatialRefs.begin() : SpatialReference();
		}
		virtual bool supportsView() const
		{
			return false;
		}
		MetadataNode privateMetadata(const std::string& name);
		MetadataNode toMetadata() const;
		ArtifactManager& artifactManager();

	private:
		// Point data operations.
		virtual PointId addPoint() = 0;
		virtual char* getDimension(const Dimension::Detail* d, PointId idx) = 0;

	protected:
		virtual char* getPoint(PointId idx) = 0;

	protected:
		MetadataPtr m_metadata;
		std::list<SpatialReference> m_spatialRefs;
		PointLayout& m_layoutRef;
		std::unique_ptr<ArtifactManager> m_artifactManager;
	};
	typedef BasePointTable& PointTableRef;
	typedef BasePointTable const& ConstPointTableRef;

	class SimplePointTable : public BasePointTable
	{

	protected:
		SimplePointTable(PointLayout& layout) : BasePointTable(layout)
		{}

	protected:
		std::size_t pointsToBytes(point_count_t numPts) const
		{
			return m_layoutRef.pointSize() * numPts;
		}

	private:
		virtual void setFieldInternal(Dimension::Id id, PointId idx, const void* value);
		virtual void getFieldInternal(Dimension::Id id, PointId idx, void* value) const;

		char* getDimension(const Dimension::Detail* d, PointId idx)
		{
			return getPoint(idx) + d->offset();
		}

		const char* getDimension(const Dimension::Detail* d, PointId idx) const
		{
			SimplePointTable* ncThis = const_cast<SimplePointTable*>(this);
			return ncThis->getPoint(idx) + d->offset();
		}
	};

	// This provides a context for processing a set of points and allows the library
	// to be used to process multiple point sets simultaneously.
	class RowPointTable : public SimplePointTable
	{
	private:
		// Point storage.
		std::vector<char*> m_blocks;
		point_count_t m_numPts;

		// Make sure this is power-of-2 to facilitate fast div and mod ops.
		static const point_count_t m_blockPtCnt = 65536;

	public:
		RowPointTable() : SimplePointTable(m_layout), m_numPts(0)
		{}
		virtual ~RowPointTable();
		virtual bool supportsView() const
		{
			return true;
		}

	protected:
		virtual char* getPoint(PointId idx);

	private:
		// Point data operations.
		virtual PointId addPoint();

		PointLayout m_layout;
	};
	using PointTable = RowPointTable;

	// This provides a context for processing a set of points and allows the library
	// to be used to process multiple point sets simultaneously.
	class ColumnPointTable : public SimplePointTable
	{
	private:
		// Point storage.
		using DimBlockList = std::vector<char*>;
		using MemBlocks = std::vector<DimBlockList>;

		// List of dimension memory block lists.
		MemBlocks m_blocks;
		point_count_t m_numPts;

		// Make sure this is power-of-2 to facilitate fast div and mod ops.
		static const point_count_t m_blockPtCnt = 16384;

	public:
		ColumnPointTable() 
			: SimplePointTable(m_layout)
			, m_numPts(0)
		{
		}
		virtual ~ColumnPointTable();
		virtual bool supportsView() const
		{
			return true;
		}
		virtual void finalize();
		virtual char* getPoint(PointId /*idx*/)
		{
			return nullptr;
		}

	private:
		virtual void setFieldInternal(Dimension::Id id, PointId idx, const void* value);
		virtual void getFieldInternal(Dimension::Id id, PointId idx, void* value) const;

		virtual PointId addPoint();

		// Hide base class calls for now.
		const char* getDimension(const Dimension::Detail* d, PointId idx) const;
		char* getDimension(const Dimension::Detail* d, PointId idx);

		PointLayout m_layout;
	};

	/// A StreamPointTable must provide storage for point data up to its capacity.
	/// It must implement getPoint() which returns a pointer to a buffer of
	/// sufficient size to contain a point's data.  The minimum size required
	/// is constant and can be determined by calling pointsToBytes(1) in the
	/// finalize() method.
	class StreamPointTable : public SimplePointTable
	{
	protected:
		StreamPointTable(PointLayout& layout, point_count_t capacity)
			: SimplePointTable(layout)
			, m_capacity(capacity)
			, m_numPoints(0)
			, m_skips(m_capacity, false)
		{}

	public:
		/// Called when a new point should be added.  Probably a no-op for
		/// streaming.
		virtual PointId addPoint()
		{
			return 0;
		}

		/// Called when execute() is started.  Typically used to set buffer size
		/// when all dimensions are known.
		virtual void finalize()
		{}

		void clear(point_count_t count)
		{
			if (!count)
				return;

			m_numPoints = count;
			reset();
			std::fill(m_skips.begin(), m_skips.end(), false);
		}

		/// Returns true if a point in the table was filtered out and should be
		/// considered omitted.
		bool skip(PointId n) const
		{
			return m_skips[n];
		}
		void setSkip(PointId n)
		{
			m_skips[n] = true;
		}

		point_count_t capacity() const
		{
			return m_capacity;
		}

		/// During a given call to reset(), this indicates the number of points
		/// populated in the table.  This value will always be less then or equal
		/// to capacity(), and also includes skipped points.
		point_count_t numPoints() const
		{
			return m_numPoints;
		}

	protected:
		/// Called when the contents of StreamPointTable have been consumed and
		/// the point data will be potentially overwritten.
		virtual void reset()
		{}

	private:
		point_count_t m_capacity;
		point_count_t m_numPoints;
		std::vector<bool> m_skips;
	};

	class FixedPointTable : public StreamPointTable
	{
	public:
		FixedPointTable(point_count_t capacity)
			: StreamPointTable(m_layout, capacity)
		{}

		virtual void finalize()
		{
			if (!m_layout.finalized())
			{
				BasePointTable::finalize();
				m_buf.resize(pointsToBytes(capacity() + 1));
			}
		}

	protected:
		virtual void reset()
		{
			std::fill(m_buf.begin(), m_buf.end(), 0);
		}

		virtual char* getPoint(PointId idx)
		{
			return m_buf.data() + pointsToBytes(idx);
		}

	private:
		std::vector<char> m_buf;
		PointLayout m_layout;
	};
}

#endif // #ifndef POINTTABLE_H
