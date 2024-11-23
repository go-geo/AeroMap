// GcpFile.cpp
// GCP file manager based on odm gcp.py
//
// GCP file format:
//
//		<projection>	; proj string, epsg, WGS84 UTM <zone>[N|S]
//		geo_x geo_y geo_z im_x im_y image_name [gcp_name]
//		...
//
// Example:
//
//		+proj=utm +zone=10 +ellps=WGS84 +datum=WGS84 +units=m +no_defs
//		544256.7 5320919.9 5 3044 2622 IMG_0525.jpg
//		544157.7 5320899.2 5 4193 1552 IMG_0585.jpg
//		544033.4 5320876.0 5 1606 2763 IMG_0690.jpg
//

#include "Logger.h"
#include "TextFile.h"
#include "GcpFile.h"

GcpFile::GcpFile(XString file_name)
	: m_file_name(file_name)
{
	read();
}

GcpFile::~GcpFile()
{

}

void GcpFile::read()
{
	TextFile textFile(m_file_name.c_str(), true);
	if (textFile.GetLineCount() == 0)
	{
		Logger::Write(__FUNCTION__, "Invalid GCP file: '%s'", m_file_name.c_str());
		return;
	}

	// Strip eventual BOM characters
	//contents = contents.replace('\ufeff', '')
	
	m_raw_srs = textFile.GetLine(0).c_str();		// SRS
	//self.srs = location.parse_srs_header(self.raw_srs)
	
	for (int i = 1; i < textFile.GetLineCount(); ++i)
	{
		XString line = textFile.GetLine(i).c_str();
		line.TruncateAt('#');
		line.Trim();

		if (line.GetLength() > 0)
		{
			int token_count = line.Tokenize(" \t,");
			if (token_count >= 6)
			{
				GcpType gcp;
				gcp.x = line.GetToken(0).GetDouble();
				gcp.y = line.GetToken(1).GetDouble();
				gcp.z = line.GetToken(2).GetDouble();
				gcp.px = line.GetToken(3).GetInt();
				gcp.py = line.GetToken(4).GetInt();
				gcp.file_name = line.GetToken(5);
				if (token_count > 6)
					gcp.name = line.GetToken(6);
				m_gcps.push_back(gcp);
			}
			else
			{
				Logger::Write(__FUNCTION__, "Malformed GCP line: '%s;", line.c_str());
			}
		}
	}
}

GcpFile::GcpType GcpFile::get_gcp(int n)
{
	GcpType gcp;
	if (n < get_gcp_count())
		gcp = m_gcps[n];
	return gcp;
}

int GcpFile::get_gcp_count()
{
	return (int)m_gcps.size();
}

//    def check_entries(self):
//        coords = {}
//        gcps = {}
//        errors = 0
//
//        for entry in self.iter_entries():
//            k = entry.coords_key()
//            coords[k] = coords.get(k, 0) + 1
//            if k not in gcps:
//                gcps[k] = []
//            gcps[k].append(entry)
//
//        for k in coords:
//            if coords[k] < 3:
//                description = "insufficient" if coords[k] < 2 else "not ideal"
//                for entry in gcps[k]:
//                    log.ODM_WARNING(str(entry))
//                Logger::Write(__FUNCTION__, "The number of images where the GCP %s has been tagged are %s" % (k, description))
//                Logger::Write(__FUNCTION__, "You should tag at least %s more images" % (3 - coords[k]))
//                Logger::Write(__FUNCTION__, "=====================================")
//                errors += 1
//        if len(coords) < 3:
//            Logger::Write(__FUNCTION__, "Low number of GCPs detected (%s). For best results use at least 5." % (3 - len(coords)))
//            Logger::Write(__FUNCTION__, "=====================================")
//            errors += 1
//
//        if errors > 0:
//            Logger::Write(__FUNCTION__, "Some issues detected with GCPs (but we're going to process this anyway)")

//    def make_resized_copy(self, gcp_file_output, ratio):
//        """
//        Creates a new resized GCP file from an existing GCP file. If one already exists, it will be removed.
//        :param gcp_file_output output path of new GCP file
//        :param ratio scale GCP coordinates by this value
//        :return path to new GCP file
//        """
//        output = [self.raw_srs]
//
//        for entry in self.iter_entries():
//            entry.px *= ratio
//            entry.py *= ratio
//            output.append(str(entry))
//
//        with open(gcp_file_output, 'w') as f:
//            f.write('\n'.join(output) + '\n')
//
//        return gcp_file_output

//    def wgs84_utm_zone(self):
//        """
//        Finds the UTM zone where the first point of the GCP falls into
//        :return utm zone string valid for a coordinates header
//        """
//        if self.entries_count() > 0:
//            entry = self.get_entry(0)
//            longlat = CRS.from_epsg("4326")
//            lon, lat = location.transform2(self.srs, longlat, entry.x, entry.y)
//            utm_zone, hemisphere = location.get_utm_zone_and_hemisphere_from(lon, lat)
//            return "WGS84 UTM %s%s" % (utm_zone, hemisphere)

//    def create_utm_copy(self, gcp_file_output, filenames=None, rejected_entries=None, include_extras=True):
//        """
//        Creates a new GCP file from an existing GCP file
//        by optionally including only filenames and reprojecting each point to
//        a UTM CRS. Rejected entries can recorded by passing a list object to
//        rejected_entries.
//        """
//        if os.path.exists(gcp_file_output):
//            os.remove(gcp_file_output)
//
//        output = [self.wgs84_utm_zone()]
//        target_srs = location.parse_srs_header(output[0])
//        transformer = location.transformer(self.srs, target_srs)
//
//        for entry in self.iter_entries():
//            if filenames is None or entry.filename in filenames:
//                entry.x, entry.y, entry.z = transformer.TransformPoint(entry.x, entry.y, entry.z)
//                if not include_extras:
//                    entry.extras = ''
//                output.append(str(entry))
//            elif isinstance(rejected_entries, list):
//                rejected_entries.append(entry)
//
//        with open(gcp_file_output, 'w') as f:
//            f.write('\n'.join(output) + '\n')
//
//        return gcp_file_output

//    def make_filtered_copy(self, gcp_file_output, images_dir, min_images=3):
//        """
//        Creates a new GCP file from an existing GCP file includes
//        only the points that reference images existing in the images_dir directory.
//        If less than min_images images are referenced, no GCP copy is created.
//        :return gcp_file_output if successful, None if no output file was created.
//        """
//        if not self.exists() or not os.path.exists(images_dir):
//            return None
//
//        if os.path.exists(gcp_file_output):
//            os.remove(gcp_file_output)
//
//        files = list(map(os.path.basename, glob.glob(os.path.join(images_dir, "*"))))
//
//        output = [self.raw_srs]
//        files_found = 0
//
//        for entry in self.iter_entries():
//            if entry.filename in files:
//                output.append(str(entry))
//                files_found += 1
//
//        if files_found >= min_images:
//            with open(gcp_file_output, 'w') as f:
//                f.write('\n'.join(output) + '\n')
//
//            return gcp_file_output
