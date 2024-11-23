#ifndef TERRAIN_H
#define TERRAIN_H

#include "MarkTypes.h"
#include "MarkLib.h"
#include "XString.h"
#include "Logger.h"
#include "Calc.h"
#include "Gis.h"

class Terrain
{
public:

	// terrain vertex flags

	enum class Flags : UInt8
	{
		WATER = 0x01,
		SAND  = 0x02,
		DIRT  = 0x04,
		ROCK  = 0x08,
		SHORE = 0x10,
	};

	// construction/destruction

	Terrain(const char* mapFolder);
	virtual ~Terrain();

	double  GetHeight(int row, int col);											// get elevation at row/col, meters
	double  GetHeightNormal(double x, double y, VEC3* pNormal = nullptr);			// get elevation, face normal
	void    SetHeight(int row, int col, double heightMeters);

	void    SetFlags(int row, int col, UInt8 type);
	UInt8   GetFlags(int row, int col);
	bool	IsLand(int row, int col);

	VEC3    GetTerrainVertex(int row, int col);
	void	UpdateHeightRange();
	void    SetProjection(GIS::Projection proj, int zone = 0);
	GIS::Projection GetProjection(int* zone);
	// get geospatial extents of model (lat/lon or x/y depending on projection)
	void	SetGeoExtents(double sw_x, double sw_y, double ne_x, double ne_y);
	PointD	GetGeoCoordSW();
	RectD	GetGeoExents();

	void	SaveData();			// save elevation data
	void	Flush();			// ensure changes written to disk

	// coordinate conversion

	bool XYToRowCol(double x, double y, int& row, int& col);			// terrain-relative xy
	void RowColToXY(int row, int col, double& x, double& y);

	bool ProjToRowCol(double x, double y, int& row, int& col);			// projected xy
	void RowColToProj(int row, int col, double& x, double& y);

	bool LLToRowCol(double lat, double lon, int& row, int& col);		// lat/lon
	void RowColToLL(int row, int col, double& lat, double& lon);

	XString GetMetaData();

	virtual bool IsDirty() { return mb_IsDirty; }
	virtual void OnDataChanged();

	// contours

	void Contour(std::vector<double> z, PixelType pixColor, XString strScaleColor);			// calculate contours

	int    GetContourCount() { return (int)mv_ContourLines.size(); }						// # of contour lines
	double GetContourElev(int idx) { return mv_ContourLines[idx].elev; }					// elevation for contour line index
	bool   GetContourClosed(int idx) { return mv_ContourLines[idx].closed; }				// # of contour lines
	std::vector<PointD> GetContourPoints(int idx) { return mv_ContourLines[idx].pts; }		// ordered list of contour line points

	// measurement

	int AddDistancePoint(VEC2& pos);
	int GetDistancePointCount() const;
	const VEC2 GetDistancePoint(unsigned int index);
	double GetDistance(unsigned int index);
	void ClearDistancePoints();

	// in-lines

	XString GetFileName() { return ms_TerrainFile; }
	int GetRowCount() const { return static_cast<int>(m_Header.RowCount); }
	int GetColCount() const { return static_cast<int>(m_Header.ColCount); }
	int GetMaxDim() const { return (GetRowCount() > GetColCount() ? GetRowCount() : GetColCount()); }
	double GetRadius() const { return GetMaxDim() * GetPitch(); }
	double GetMinElev() const { return m_Header.MinHeight; }				// minimum terrain elevation, meters
	double GetMaxElev() const { return m_Header.MaxHeight; }				// maximum terrain elevation, meters
	double GetPitch() const { return m_Header.GridSpace; }
	double GetSizeX() const { return (m_Header.GridSpace*(m_Header.ColCount - 1)); }		// terrain size in E-W direction, meters
	double GetSizeY() const { return (m_Header.GridSpace*(m_Header.RowCount - 1)); }		// terrain size in N-S direction, meters
	VEC2 GetCenter() { return VEC2(GetSizeX() * 0.5, GetSizeY() * 0.5); }

	// static methods

	static bool Create(const char* fileName, int rowCount, int colCount, double pitchMeters);

	static const VEC3 GetNorth() { return VEC3(0, 1, 0); }
	static const VEC3 GetEast() { return VEC3(1, 0, 0); }
	static const VEC3 GetUp() { return VEC3(0, 0, 1); }

protected:

#pragma pack(1)		// byte align disk records

	struct TerrainHeaderDiskType	// generated terrain database header
	{
		UInt8  Marker[2];		// file marker, 'GG'
		UInt16 Version;         // terrain file version
		UInt16 VxSize;			// size of vertex record, bytes
		UInt16 RowCount;	    // # of rows in database
		UInt16 ColCount;	    // # of columns in database
		float  MinHeight;    	// minimum terrain height for entire file, signed feet msl
		float  MaxHeight;    	// maximum terrain height for entire file, signed feet msl
		double GridSpace;	    // horizontal grid spacing, meters
		double SWX;				// extents of map, interpretation depends on projection
		double SWY;
		double NEX;
		double NEY;
		UInt16 Projection;		// Gis::Projection enum value
		UInt16 ProjZone;		// zone for Projection, if applicable
	};

	struct TerrainVertexDiskType
	{
		float Height;		// terrain elevation, feet msl
		UInt8 Flags;		// terrain vertex flags
	};

#pragma pack()

	XString ms_TerrainFile;		// height data file

	std::vector<VEC2> mv_Distance;		// connected points defining distance measurement

	// "Contour Lines" are the vectorized results of processed 
	// mv_Contour/mv_ContourElev inputs
	struct ContourLineType
	{
		double elev;				// elevation for this line
		bool closed;				// last contour point connects to first
		PixelType color;			// color for this line
		std::vector<PointD> pts;	// ordered list of points defining contour polyline

		ContourLineType()
		{
			elev = 0.0;
			closed = false;
			color.SetGraySF(0.0F);
			pts.clear();
		}
	};
	std::vector<ContourLineType> mv_ContourLines;

	// contours are a list of line segments rather than a list of individual
	// points because they aren't constructed in order
	std::vector<double> mv_ContourElev;				// contour elevation levels, 0 = lowest
	std::vector<std::vector<RectD>> mv_Contour;		// all contour line segments, each "rect" is single line segment, index is level

protected:

	TerrainVertexDiskType GetVertex(int row, int col);
	inline bool IsValid(int row, int col);

private:

	TerrainHeaderDiskType m_Header;
	std::vector<TerrainVertexDiskType*> m_trn;

	bool mb_IsDirty;			// terrain data modified, only applies to data that can be saved
	int m_File;

private:

	void LoadData();			// load elevation data

	bool OpenDataFile();
	void CloseDataFile();

	void SortContours();

	inline UInt32 CalcByteOffset(int rowIdx, int colIdx);
};

#endif // #ifndef TERRAIN_H
