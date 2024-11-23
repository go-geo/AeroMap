#ifndef GCPFILE_H
#define GCPFILE_H

#include <vector>

#include "XString.h"

class GcpFile
{
public:

	struct GcpType
	{
		double x, y, z;				// world coordinates
		int px, py;					// pixel coordinates
		XString name;				// optional name for this gcp
		XString file_name;			// image file

		GcpType()
		{
			this->x = this->y = this->z = 0.0;
			this->px = this->py;
		}
		GcpType(double x, double y, double z, int px, int py, XString file_name, XString name = "")
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->px = px;
			this->py = py;
			this->file_name = file_name;
			this->name = name;
		}
	};

//    def coords_key(self):
//        return "{} {} {}".format(self.x, self.y, self.z)
//
//    def __str__(self):
//        return "{} {} {} {} {} {} {}".format(self.x, self.y, self.z,
//                                             self.px, self.py,
//                                             self.filename,
//                                             self.extras).rstrip()

public:

	GcpType get_gcp(int n);
	int get_gcp_count();

private:

	XString m_file_name;
	XString m_raw_srs;

	std::vector<GcpType> m_gcps;

public:

	GcpFile(XString file_name);
	~GcpFile();

private:

	void read();
};

#endif // #ifndef GCPFILE_H
