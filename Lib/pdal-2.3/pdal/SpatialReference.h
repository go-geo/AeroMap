#ifndef SPATIALREFERENCE_H
#define SPATIALREFERENCE_H

#include <pdal/pdal_internal.h>

namespace pdal
{
	class BOX3D;
	class MetadataNode;

	/// A SpatialReference defines a model of the earth that is used to describe
	/// the location of points.
	/// A SpatialReference is part of input data and is automatically loaded
	/// into PDAL by readers, or it's provided explicitly by a user through an
	/// option.  All points in a point view share a common spatial reference.  When
	/// a stage finishes processing point view, the point view takes on the
	/// spatial reference of that stage, if it had one.
	/// A point table tracks the spatial references of the views currently being
	/// processed by a stage.  If a point table being processed by a stage has
	/// more than one spatial reference, PointTable::spatialReference() will
	/// return an empty spatial reference and PointTable::spatialReferenceUnique()
	/// will return false.
	class SpatialReference
	{
	public:
		/**
		  Constructor.  Create an empty SRS.
		*/
		SpatialReference()
		{}

		/**
		  Construct a spatial reference from well-known text.

		  \param wkt  Well-known text from which to construct SRS.
		*/
		SpatialReference(const std::string& wkt);

		/**
		  Construct a spatial reference from well-known text.

		  \param wkt  Well-known text from which to construct SRS.
		*/
		SpatialReference(const char* wkt);


		/**
		  Determine if this spatial reference is the same as another.

		  \param other  SRS to compare with this one.
		  \return  \c true if the SRSs match
		*/
		bool equals(const SpatialReference& other) const;

		/**
		  See \ref equals.
		*/
		bool operator==(const SpatialReference& other) const;

		/**
		  Determine if this spatial reference is different from another.

		  \param other  SRS to compare with this one.
		  \return \c true if the SRSs don't match.
		*/
		bool operator!=(const SpatialReference& other) const;

		/**
		   Determine if the well-known text representation of this SRS is
		   lexographically less than that of another.

		   \param other  SRS to compare with this one.
		   \return  \c true if this SRS is lexographically less.
		*/
		bool operator<(const SpatialReference& other) const
		{
			return m_wkt < other.m_wkt;
		}

		/**
		  Returns true iff the object doesn't contain a valid srs.

		  \return  Whether the SRS is empty.
		*/
		bool empty() const;


		// Returns true of OSR can validate the SRS
		bool valid() const;

		std::string getWKT() const;
		std::string getWKT1() const;

		/// Parse the string starting at position `pos` as a spatial reference.
		/// \param s    String to parse.
		/// \param pos  Position to start parsing string.
		void parse(const std::string& s, std::string::size_type& pos);

		/// Sets the SRS from a string representation.  WKT is saved as
		/// provided.
		/// \param v - a string containing the definition (filename, proj4,
		///    wkt, etc).
		void set(std::string v);

		/// Returns the Proj.4 string describing the Spatial Reference System.
		/// If GDAL is linked, it uses GDAL's operations and methods to determine
		/// the Proj.4 string -- otherwise, if libgeotiff is linked, it uses
		/// that.  Note that GDAL's operations are much more mature and
		/// support more coordinate systems and descriptions.
		std::string getProj4() const;

		std::string getHorizontal() const;
		std::string getHorizontalUnits() const;
		std::string getVertical() const;
		std::string getVerticalUnits() const;

		/// Attempt to identify an EPSG code from the spatial reference.  Returns
		/// an empty string if a code could not be identified.
		std::string identifyHorizontalEPSG() const;
		std::string identifyVerticalEPSG() const;

		/// Returns UTM zone **if** the coordinate system is actually UTM.
		/// The method simply forwards down to OSRGetUTMZone
		int getUTMZone() const;

		void dump() const;
		MetadataNode toMetadata() const;

		bool isGeographic() const;
		bool isGeocentric() const;
		bool isProjected() const;

		std::vector<int> getAxisOrdering() const;

		int computeUTMZone(const BOX3D& box) const;

		const std::string& getName() const;
		static int calculateZone(double lon, double lat);
		static SpatialReference wgs84FromZone(int zone);
		static bool isWKT(const std::string& wkt);
		static std::string prettyWkt(const std::string& wkt);

	private:
		std::string m_wkt;
		mutable std::string m_horizontalWkt;
		friend std::ostream& operator<<(std::ostream& ostr, const SpatialReference& srs);
		friend std::istream& operator>>(std::istream& istr, SpatialReference& srs);
	};

	std::ostream& operator<<(std::ostream& ostr, const SpatialReference& srs);
	std::istream& operator>>(std::istream& istr, SpatialReference& srs);
}

#endif // #ifndef SPATIALREFERENCE_H