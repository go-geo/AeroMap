// Photo.cpp
// Port of odm photo.py
//

#include <opencv2/opencv.hpp>	// OpenCV

#include <iostream>
#include <fstream>

#include "exif.h"           // easy exif header
#include "TinyEXIF.h"
#include "tiffio.h"
#include "tiffiop.h"
#include "Photo.h"

//projections = ['perspective', 'fisheye', 'fisheye_opencv', 'brown', 'dual', 'equirectangular', 'spherical']

Photo::Photo()
    : Photo("")
{
}

Photo::Photo(XString file_name)
    : m_focal_ratio(0.0)
    , m_focal_plane_xres(0.0)
    , m_focal_plane_yres(0.0)
    , m_focal_plane_res_unit(0)
    , m_focal_length(0.0)
    , m_epoch(0)
    , m_exposure_time(0.0)
{
    m_file_name = file_name;

    image = cv::imread(file_name.c_str());

    //m_mask = None

    // Standard tags (virtually all photos have these)
    m_width = 0;
    m_height = 0;
    m_camera_make.Clear();
    m_camera_model.Clear();
    m_orientation = 1;

    // Geo tags
    m_latitude = 0.0;
    m_longitude = 0.0;
    m_altitude = 0.0;

    // Multi-band fields
    m_band_name = "RGB";
    m_band_index = 0;
    //m_capture_uuid = None

    // Multi-spectral fields
    m_fnumber = 0.0;
    //m_radiometric_calibration = None
    //m_black_level = None
    m_gain = 0.0;
    m_gain_adjustment = 0.0;

    // Capture info
    m_exposure_time = 0.0;
	m_iso_speed = 0;
	//m_bits_per_sample = None
	//m_vignetting_center = None
	//m_vignetting_polynomial = None
    m_spectral_irradiance = 0.0;
	//m_horizontal_irradiance = None
	//m_irradiance_scale_to_si = None
	//m_utc_time = None

	// OPK angles
	m_yaw = 0.0;
	m_pitch = 0.0;
	m_roll = 0.0;
	m_omega = 0.0;
	m_phi = 0.0;
	m_kappa = 0.0;

	// DLS
	//m_sun_sensor = None
	//m_dls_yaw = None
	//m_dls_pitch = None
	//m_dls_roll = None

	// Aircraft speed
	m_speed_x = 0.0;
	m_speed_y = 0.0;
	m_speed_z = 0.0;

	// Original image width/height at capture time (before possible resizes)
	m_exif_width = 0;
	m_exif_height = 0;

	// m_center_wavelength = None
	// m_bandwidth = None

	// RTK
	m_gps_xy_stddev = 0.0;  // Dilution of Precision X/Y
	m_gps_z_stddev = 0.0;   // Dilution of Precision Z

	// Misc SFM
	m_camera_projection = "brown";
	m_focal_ratio = 0.85;

	// parse values from metadata
	if (file_name.EndsWithNoCase(".jpg") || file_name.EndsWithNoCase(".jpeg"))
		parse_exif_values(file_name);
	else if (file_name.EndsWithNoCase(".tif") || file_name.EndsWithNoCase(".tiff"))
		parse_tiff_values(file_name);

	m_focal_ratio = CalcFocalRatio();

	m_epoch = CalcUnixEpoch(m_capture_date.c_str());
}

Photo::~Photo()
{

}

void Photo::parse_tiff_values(XString file_name)
{
    TIFF* pTiff = TIFFOpen(file_name.c_str(), "r");
    if (pTiff)
    {
        UInt32 width;
        UInt32 height;
        int status = TIFFGetField(pTiff, TIFFTAG_IMAGEWIDTH, &width);
        if (status == 1)
            status = TIFFGetField(pTiff, TIFFTAG_IMAGELENGTH, &height);

        if (status == 1)
        {
            m_width = width;
            m_height = height;

            m_exif_width = width;
            m_exif_height = height;
        }

        UInt16 orient;
        status = TIFFGetField(pTiff, TIFFTAG_ORIENTATION, &orient);
        if (status == 1)
            m_orientation = orient;

        char* p = nullptr;
        status = TIFFGetField(pTiff, TIFFTAG_MAKE, &p);
        if (status == 1)
            m_camera_make = p;
        status = TIFFGetField(pTiff, TIFFTAG_MODEL, &p);
        if (status == 1)
            m_camera_model = p;

        // read exif entries (changes current directory)

        toff_t offset = 0;
        if (TIFFGetField(pTiff, TIFFTAG_EXIFIFD, &offset) == 1)
        {
            if (TIFFReadEXIFDirectory(pTiff, offset) == 1)
            {
                m_exposure_time = get_rational(pTiff, EXIFTAG_EXPOSURETIME);
                m_fnumber = get_rational(pTiff, EXIFTAG_FNUMBER);

                // 0x9003 DateTimeOriginal string ExifIFD (date/time when original image was taken)
                status = TIFFGetField(pTiff, EXIFTAG_DATETIMEORIGINAL, &p);
                if (status == 1)
                    m_capture_date = p;

                // 0x920e FocalPlaneXResolution
                float f = 0.0;
                status = TIFFGetField(pTiff, EXIFTAG_FOCALPLANEXRESOLUTION, &f);
                if (status == 1)
                    m_focal_plane_xres = f;
                // 0x920f FocalPlaneYResolution
                status = TIFFGetField(pTiff, EXIFTAG_FOCALPLANEYRESOLUTION, &f);
                if (status == 1)
                    m_focal_plane_yres = f;
                // 0x9210 FocalPlaneResolutionUnit
                //      1 = None
                //      2 = inches
                //      3 = cm
                //      4 = mm
                //      5 = um
                uint32_t n = 0;
                status = TIFFGetField(pTiff, EXIFTAG_FOCALPLANERESOLUTIONUNIT, &n);
                if (status == 1)
                    m_focal_plane_res_unit = n;

                //0x8833
                //EXIFTAG_ISOSPEEDRATINGS
                //EXIFTAG_ISOSPEED
                //EXIFTAG_PHOTOGRAPHICSENSITIVITY
                //m_iso_speed = get_uint32(pTiff, EXIFTAG_ISOSPEEDRATINGS);
                //m_iso_speed = get_uint32(pTiff, EXIFTAG_ISOSPEED);
                //m_iso_speed = get_uint32(pTiff, EXIFTAG_PHOTOGRAPHICSENSITIVITY);
            }
        }

        TIFFClose(pTiff);

        XString xmp = LoadXmpMeta(file_name);

        TinyEXIF::EXIFInfo tiny_exif;
        if (tiny_exif.parseFromXMPSegmentXML(xmp.c_str(), xmp.GetLength()) == TinyEXIF::PARSE_SUCCESS)
        {
            m_band_name = tiny_exif.BandName.c_str();
            m_gain = tiny_exif.BandGain;
            m_gain_adjustment = tiny_exif.GainAdjustment;
            m_capture_uuid = tiny_exif.CaptureUUID.c_str();
            m_capture_uuid.MakeUpper();

            // these are raw ypr values - some models will require interpretation
            m_yaw = tiny_exif.GeoLocation.YawDegree;
            m_pitch = tiny_exif.GeoLocation.PitchDegree;
            m_roll = tiny_exif.GeoLocation.RollDegree;
        }
    }
}

uint32_t Photo::get_uint32(TIFF* pTiff, uint32_t tag)
{
    uint32_t n = 0;

    const TIFFField* fip = TIFFFieldWithTag(pTiff, tag);
    if (fip != nullptr)
    {
        uint32_t value_count = 1;
        if ((fip->field_readcount == TIFF_VARIABLE) || (fip->field_readcount == TIFF_VARIABLE2))
            value_count = 1;
        else if (fip->field_readcount == TIFF_SPP)
            value_count = pTiff->tif_dir.td_samplesperpixel;
        else
            value_count = fip->field_readcount;

        void* raw_data = nullptr;
        /// Rational2Double: For Rationals evaluate
        //  "set_field_type" to determine internal storage size.
        int tv_size = TIFFFieldSetGetSize(fip);
        raw_data = _TIFFmallocExt(pTiff, tv_size * value_count);
        if (TIFFGetField(pTiff, tag, raw_data) == 1)
        {
            if (fip->field_type == TIFF_LONG)
            {
                int tv_size = TIFFFieldSetGetSize(fip);
                if (tv_size == 4)
                    n = ((uint32_t*)raw_data)[0];
            }
            else
            {
                assert(false);
            }
        }
        _TIFFfreeExt(pTiff, raw_data);
    }

    return n;
}

double Photo::get_rational(TIFF* pTiff, uint32_t tag)
{
    double f = 0.0;

    const TIFFField* fip = TIFFFieldWithTag(pTiff, tag);
    if (fip != nullptr)
    {
        uint32_t value_count = 1;
        if ((fip->field_readcount == TIFF_VARIABLE) || (fip->field_readcount == TIFF_VARIABLE2))
            value_count = 1;
        //else if (fip->field_readcount == TIFF_SPP)
          //  value_count = td->td_samplesperpixel;
        else
            value_count = fip->field_readcount;

        void* raw_data = nullptr;
        /// Rational2Double: For Rationals evaluate
        //  "set_field_type" to determine internal storage size.
        int tv_size = TIFFFieldSetGetSize(fip);
        raw_data = _TIFFmallocExt(pTiff, tv_size * value_count);
        if (TIFFGetField(pTiff, tag, raw_data) == 1)
        {
            if ((fip->field_type == TIFF_RATIONAL) || (fip->field_type == TIFF_SRATIONAL))
            {
                int tv_size = TIFFFieldSetGetSize(fip);
                if (tv_size == 8)
                    f = ((double*)raw_data)[0];
                else
                    f = ((float*)raw_data)[0];
            }
            else
            {
                assert(false);
            }
        }
        _TIFFfreeExt(pTiff, raw_data);
    }

    return f;
}

void Photo::parse_exif_values(XString file_name)
{
    // first, try parsing with easy exif - it's easy :)

    FILE* fp = fopen(file_name.c_str(), "rb");
    if (!fp)
    {
        Logger::Write(__FUNCTION__, "Unable to open image file: '%s'", file_name.c_str());
        return;
    }
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    unsigned char* buf = new unsigned char[fsize];
    if (fread(buf, 1, fsize, fp) != fsize)
    {
        Logger::Write(__FUNCTION__, "Unable to open read file: '%s'", file_name.c_str());
        delete[] buf;
        return;
    }
    fclose(fp);

    easyexif::EXIFInfo exif;
    int code = exif.parseFrom(buf, fsize);
    delete[] buf;
    if (code)
    {
        Logger::Write(__FUNCTION__, "Error parsing EXIF: '%s' (code %d)", file_name.c_str(), code);
        return;
    }

    m_width = exif.ImageWidth;
    m_height = exif.ImageHeight;

    m_exif_width = exif.ImageWidth;
    m_exif_height = exif.ImageHeight;

    m_camera_make = exif.Make.c_str();
    m_camera_model = exif.Model.c_str();

    m_orientation = exif.Orientation;

    m_latitude = exif.GeoLocation.Latitude;
    m_longitude = exif.GeoLocation.Longitude;
    m_altitude = exif.GeoLocation.Altitude;

    m_fnumber = exif.FNumber;
    m_iso_speed = exif.ISOSpeedRatings;
    m_exposure_time = exif.ExposureTime;

    m_focal_plane_xres = exif.LensInfo.FocalPlaneXResolution;
    m_focal_plane_yres = exif.LensInfo.FocalPlaneYResolution;
    m_focal_plane_res_unit = exif.LensInfo.FocalPlaneResolutionUnit;
    m_focal_length = exif.FocalLength;

    m_capture_date = exif.DateTime.c_str();

    // second, parse using TinyExif - has better support for XMP

    std::ifstream istream(file_name.c_str(), std::ifstream::binary);
    TinyEXIF::EXIFInfo tinyEXIF(istream);

    if (tinyEXIF.Fields)
    {
        if (m_width == 0 && tinyEXIF.ImageWidth > 0)
            m_width = tinyEXIF.ImageWidth;
        if (m_height == 0 && tinyEXIF.ImageHeight > 0)
            m_height = tinyEXIF.ImageHeight;

        if (tinyEXIF.BandName.length() > 0)
            m_band_name = tinyEXIF.BandName.c_str();

        m_gain = tinyEXIF.BandGain;
        m_gain_adjustment = tinyEXIF.GainAdjustment;
    }

    // still no image dimensions, get them from opencv - be aware
    // it doesn't seem to respect orientation
    
    if (m_width == 0 || m_height == 0)
    {
        m_width = image.cols;
        m_height = image.rows;
    }

    m_exif_width = m_width;
    m_exif_height = m_height;

    //with open(_path_file, 'rb') as f:
    //    tags = exifread.process_file(f, details=True, extract_thumbnail=False)
    //    try:
    //        if 'Image Make' in tags:
    //            try:
    //                m_camera_make = tags['Image Make'].values
    //                m_camera_make = m_camera_make.strip()
    //            except UnicodeDecodeError:
    //                Logger::Write(__FUNCTION__, "EXIF Image Make might be corrupted")
    //                m_camera_make = "unknown"
    //        if 'Image Model' in tags:
    //            try:
    //                m_camera_model = tags['Image Model'].values
    //                m_camera_model = m_camera_model.strip()
    //            except UnicodeDecodeError:
    //                Logger::Write(__FUNCTION__, "EXIF Image Model might be corrupted")
    //                m_camera_model = "unknown"
    //        if 'GPS GPSAltitude' in tags:
    //            self.altitude = self.float_value(tags['GPS GPSAltitude'])
    //            if 'GPS GPSAltitudeRef' in tags and self.int_value(tags['GPS GPSAltitudeRef']) is not None and self.int_value(tags['GPS GPSAltitudeRef']) > 0:
    //                self.altitude *= -1
    //        if 'GPS GPSLatitude' in tags and 'GPS GPSLatitudeRef' in tags:
    //            self.latitude = self.dms_to_decimal(tags['GPS GPSLatitude'], tags['GPS GPSLatitudeRef'])
    //        elif 'GPS GPSLatitude' in tags:
    //            Logger::Write(__FUNCTION__, "GPS position for %s might be incorrect, GPSLatitudeRef tag is missing (assuming N)" % self.filename)
    //            self.latitude = self.dms_to_decimal(tags['GPS GPSLatitude'], GPSRefMock('N'))
    //        if 'GPS GPSLongitude' in tags and 'GPS GPSLongitudeRef' in tags:
    //            self.longitude = self.dms_to_decimal(tags['GPS GPSLongitude'], tags['GPS GPSLongitudeRef'])
    //        elif 'GPS GPSLongitude' in tags:
    //            Logger::Write(__FUNCTION__, "GPS position for %s might be incorrect, GPSLongitudeRef tag is missing (assuming E)" % self.filename)
    //            self.longitude = self.dms_to_decimal(tags['GPS GPSLongitude'], GPSRefMock('E'))
    //        if 'Image Orientation' in tags:
    //            self.orientation = self.int_value(tags['Image Orientation'])
    //    except (IndexError, ValueError) as e:
    //        Logger::Write(__FUNCTION__, "Cannot read basic EXIF tags for %s: %s" % (self.filename, str(e)))

    //    try:
    //        if 'Image Tag 0xC61A' in tags:
    //            self.black_level = self.list_values(tags['Image Tag 0xC61A'])
    //        elif 'BlackLevel' in tags:
    //            self.black_level = self.list_values(tags['BlackLevel'])
    //        elif 'Image BlackLevel' in tags:
    //            self.black_level = self.list_values(tags['Image BlackLevel'])

    //        if 'EXIF ExposureTime' in tags:
    //            self.exposure_time = self.float_value(tags['EXIF ExposureTime'])

    //        if 'EXIF FNumber' in tags:
    //            self.fnumber = self.float_value(tags['EXIF FNumber'])

    //        if 'EXIF ISOSpeed' in tags:
    //            self.iso_speed = self.int_value(tags['EXIF ISOSpeed'])
    //        elif 'EXIF PhotographicSensitivity' in tags:
    //            self.iso_speed = self.int_value(tags['EXIF PhotographicSensitivity'])
    //        elif 'EXIF ISOSpeedRatings' in tags:
    //            self.iso_speed = self.int_value(tags['EXIF ISOSpeedRatings'])

    //        if 'Image BitsPerSample' in tags:
    //            self.bits_per_sample = self.int_value(tags['Image BitsPerSample'])

    //        if 'EXIF DateTimeOriginal' in tags:
    //            str_time = tags['EXIF DateTimeOriginal'].values
    //            utc_time = datetime.strptime(str_time, "%Y:%m:%d %H:%M:%S")
    //            subsec = 0
    //            if 'EXIF SubSecTime' in tags:
    //                subsec = self.int_value(tags['EXIF SubSecTime'])
    //            negative = 1.0
    //            if subsec < 0:
    //                negative = -1.0
    //                subsec *= -1.0
    //            subsec = float('0.{}'.format(int(subsec)))
    //            subsec *= negative
    //            ms = subsec * 1e3
    //            utc_time += timedelta(milliseconds = ms)
    //            timezone = pytz.timezone('UTC')
    //            epoch = timezone.localize(datetime.utcfromtimestamp(0))
    //            self.utc_time = (timezone.localize(utc_time) - epoch).total_seconds() * 1000.0

    //        if 'MakerNote SpeedX' in tags and \
    //            'MakerNote SpeedY' in tags and \
    //            'MakerNote SpeedZ' in tags:
    //            self.speed_x = self.float_value(tags['MakerNote SpeedX'])
    //            self.speed_y = self.float_value(tags['MakerNote SpeedY'])
    //            self.speed_z = self.float_value(tags['MakerNote SpeedZ'])

    //        if 'EXIF ExifImageWidth' in tags and \
    //            'EXIF ExifImageLength' in tags:
    //            m_exif_width = self.int_value(tags['EXIF ExifImageWidth'])
    //            self.exif_height = self.int_value(tags['EXIF ExifImageLength'])

    //    except Exception as e:
    //        Logger::Write(__FUNCTION__, "Cannot read extended EXIF tags for %s: %s" % (self.filename, str(e)))

    //    # Warn if GPS coordinates are suspiciously wrong
    //    if self.latitude is not None and self.latitude == 0 and \
    //        self.longitude is not None and self.longitude == 0:
    //        Logger::Write(__FUNCTION__, "%s has GPS position (0,0), possibly corrupted" % self.filename)


    //    # Extract XMP tags
    //    f.seek(0)
    //    xmp = self.get_xmp(f)

    //    for xtags in xmp:
    //        try:
    //            band_name = self.get_xmp_tag(xtags, ['Camera:BandName', '@Camera:BandName', 'FLIR:BandName'])
    //            if band_name is not None:
    //                self.band_name = band_name.replace(" ", "")

    //            self.set_attr_from_xmp_tag('band_index', xtags, [
    //                'DLS:SensorId', # Micasense RedEdge
    //                '@Camera:RigCameraIndex', # Parrot Sequoia, Sentera 21244-00_3.2MP-GS-0001
    //                'Camera:RigCameraIndex', # MicaSense Altum
    //            ])

    //            self.set_attr_from_xmp_tag('radiometric_calibration', xtags, [
    //                'MicaSense:RadiometricCalibration',
    //            ])

    //            self.set_attr_from_xmp_tag('vignetting_center', xtags, [
    //                'Camera:VignettingCenter',
    //                'Sentera:VignettingCenter',
    //            ])

    //            self.set_attr_from_xmp_tag('vignetting_polynomial', xtags, [
    //                'Camera:VignettingPolynomial',
    //                'Sentera:VignettingPolynomial',
    //            ])

    //            self.set_attr_from_xmp_tag('horizontal_irradiance', xtags, [
    //                'Camera:HorizontalIrradiance'
    //            ], float)

    //            self.set_attr_from_xmp_tag('irradiance_scale_to_si', xtags, [
    //                'Camera:IrradianceScaleToSIUnits'
    //            ], float)

    //            self.set_attr_from_xmp_tag('sun_sensor', xtags, [
    //                'Camera:SunSensor',
    //            ], float)

    //            self.set_attr_from_xmp_tag('spectral_irradiance', xtags, [
    //                'Camera:SpectralIrradiance',
    //                'Camera:Irradiance',
    //            ], float)

    //            self.set_attr_from_xmp_tag('capture_uuid', xtags, [
    //                '@drone-dji:CaptureUUID', # DJI
    //                'MicaSense:CaptureId', # MicaSense Altum
    //                '@Camera:ImageUniqueID', # sentera 6x
    //                '@Camera:CaptureUUID', # Parrot Sequoia
    //            ])

    //            # Camera make / model for some cameras is stored in the XMP
    //            if m_camera_make == '':
    //                self.set_attr_from_xmp_tag('camera_make', xtags, [
    //                    '@tiff:Make'
    //                ])
    //            if m_camera_model == '':
    //                self.set_attr_from_xmp_tag('camera_model', xtags, [
    //                    '@tiff:Model'
    //                ])

    //            # DJI GPS tags
    //            self.set_attr_from_xmp_tag('longitude', xtags, [
    //                '@drone-dji:Longitude'
    //            ], float)
    //            self.set_attr_from_xmp_tag('latitude', xtags, [
    //                '@drone-dji:Latitude'
    //            ], float)
    //            self.set_attr_from_xmp_tag('altitude', xtags, [
    //                '@drone-dji:AbsoluteAltitude'
    //            ], float)

    //            # Phantom 4 RTK
    //            if '@drone-dji:RtkStdLon' in xtags:
    //                y = float(self.get_xmp_tag(xtags, '@drone-dji:RtkStdLon'))
    //                x = float(self.get_xmp_tag(xtags, '@drone-dji:RtkStdLat'))
    //                self.gps_xy_stddev = max(x, y)

    //                if '@drone-dji:RtkStdHgt' in xtags:
    //                    self.gps_z_stddev = float(self.get_xmp_tag(xtags, '@drone-dji:RtkStdHgt'))
    //            else
    //                self.set_attr_from_xmp_tag('gps_xy_stddev', xtags, [
    //                    '@Camera:GPSXYAccuracy',
    //                    'GPSXYAccuracy'
    //                ], float)
    //                self.set_attr_from_xmp_tag('gps_z_stddev', xtags, [
    //                    '@Camera:GPSZAccuracy',
    //                    'GPSZAccuracy'
    //                ], float)

    //            # DJI Speed tags
    //            if '@drone-dji:FlightXSpeed' in xtags and \
    //                '@drone-dji:FlightYSpeed' in xtags and \
    //                '@drone-dji:FlightZSpeed' in xtags:
    //                self.set_attr_from_xmp_tag('speed_x', xtags, [
    //                    '@drone-dji:FlightXSpeed'
    //                ], float)
    //                self.set_attr_from_xmp_tag('speed_y', xtags, [
    //                    '@drone-dji:FlightYSpeed',
    //                ], float)
    //                self.set_attr_from_xmp_tag('speed_z', xtags, [
    //                    '@drone-dji:FlightZSpeed',
    //                ], float)

    //            # Account for over-estimation
    //            if self.gps_xy_stddev is not None:
    //                self.gps_xy_stddev *= 2.0
    //            if self.gps_z_stddev is not None:
    //                self.gps_z_stddev *= 2.0

    //            if 'DLS:Yaw' in xtags:
    //                self.set_attr_from_xmp_tag('dls_yaw', xtags, ['DLS:Yaw'], float)
    //                self.set_attr_from_xmp_tag('dls_pitch', xtags, ['DLS:Pitch'], float)
    //                self.set_attr_from_xmp_tag('dls_roll', xtags, ['DLS:Roll'], float)

    //            camera_projection = self.get_xmp_tag(xtags, ['@Camera:ModelType', 'Camera:ModelType'])
    //            if camera_projection is not None:
    //                camera_projection = camera_projection.lower()

    //                # Parrot Sequoia's "fisheye" model maps to "fisheye_opencv"
    //                if camera_projection == "fisheye" and m_camera_make.lower() == "parrot" and m_camera_model.lower() == "sequoia":
    //                    camera_projection = "fisheye_opencv"

    //                if camera_projection in projections:
    //                    m_camera_projection = camera_projection

    //            # OPK
    //            self.set_attr_from_xmp_tag('yaw', xtags, ['@drone-dji:FlightYawDegree', '@Camera:Yaw', 'Camera:Yaw'], float)
    //            self.set_attr_from_xmp_tag('pitch', xtags, ['@drone-dji:GimbalPitchDegree', '@Camera:Pitch', 'Camera:Pitch'], float)
    //            self.set_attr_from_xmp_tag('roll', xtags, ['@drone-dji:GimbalRollDegree', '@Camera:Roll', 'Camera:Roll'], float)

    //            # Normalize YPR conventions (assuming nadir camera)
    //            # Yaw: 0 --> top of image points north
    //            # Yaw: 90 --> top of image points east
    //            # Yaw: 270 --> top of image points west
    //            # Pitch: 0 --> nadir camera
    //            # Pitch: 90 --> camera is looking forward
    //            # Roll: 0 (assuming gimbal)
    //            if self.has_ypr():
    //                if m_camera_make.lower() in ['dji', 'hasselblad']:
    //                    self.pitch = 90 + self.pitch

    //                if m_camera_make.lower() == 'sensefly':
    //                    self.roll *= -1

    //        except Exception as e:
    //            Logger::Write(__FUNCTION__, "Cannot read XMP tags for %s: %s" % (self.filename, str(e)))

    //        # self.set_attr_from_xmp_tag('center_wavelength', xtags, [
    //        #     'Camera:CentralWavelength'
    //        # ], float)

    //        # self.set_attr_from_xmp_tag('bandwidth', xtags, [
    //        #     'Camera:WavelengthFWHM'
    //        # ], float)

    // Special case band handling for AeroVironment Quantix images
    // for some reason, they don't store band information in EXIFs
    if (m_camera_make.CompareNoCase("AeroVironment") &&  m_camera_model.CompareNoCase("Quantix"))
    {
        //    matches = re.match("IMG_(\d+)_(\w+)\.\w+", self.filename, re.IGNORECASE)
        //    if matches:
        //        band_aliases = {
        //            'GRN': 'Green',
        //            'NIR': 'Nir',
        //            'RED': 'Red',
        //            'RGB': 'RedGreenBlue',
        //        }
        //        self.capture_uuid = matches.group(1)
        //        self.band_name = band_aliases.get(matches.group(2), matches.group(2))
    }

    // Sanitize band name since we use it in folder paths
    //self.band_name = re.sub('[^A-Za-z0-9]+', '', self.band_name)

    //self.compute_focal(tags, xtags)
    //self.compute_opk()
}

SizeType Photo::find_largest_photo_dims(const std::vector<Photo*>& photos)
{
    int max_mp = 0;
    SizeType max_dims = { 0, 0 };

    for (Photo* p : photos)
    {
        if (p->GetWidth() == 0 || p->GetHeight() == 0)
            continue;
        int mp = p->GetWidth() * p->GetHeight();
        if (mp > max_mp)
        {
            max_mp = mp;
            max_dims = { p->GetWidth(), p->GetHeight() };
        }
    }

    return max_dims;
}

int Photo::find_largest_photo_dim(const std::vector<Photo*>& photos)
{
	int max_dim = 0;

    for (Photo* p : photos)
    {
        if (p->GetWidth() == 0 || p->GetHeight() == 0)
            continue;
        max_dim = std::max(max_dim, std::max(p->GetWidth(), p->GetHeight()));
    }

	return max_dim;
}

Photo* Photo::find_largest_photo(const std::vector<Photo*>& photos)
{
	Photo* max_p = nullptr;

    int max_area = 0;
    for (Photo* p : photos)
    {
        if (p->GetWidth() == 0 || p->GetHeight() == 0)
            continue;
        int area = p->GetWidth() * p->GetHeight();

        if (area > max_area)
        {
            max_area = area;
            max_p = p;
        }
    }

	return max_p;
}

double Photo::get_mm_per_unit(int resolution_unit)
{
	// Length of a resolution unit in millimeters.
	//
	// Uses the values from the EXIF specs in
	// https://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/EXIF.html
	//
	// Inputs:
	//		resolution_unit	= resolution unit value given in the EXIF
	//

	double mm = 0.0;

	switch (resolution_unit) {
	case 2:			// inch
		mm = 25.4;
		break;
	case 3:			// cm
		mm = 10.0;
		break;
	case 4:			// mm
		mm = 1.0;
		break;
	case 5:			// um
		mm = 0.001;
		break;
	default:
		Logger::Write(__FUNCTION__, "Unknown EXIF resolution unit value: %d", resolution_unit);
	}

	return mm;
}

//    def set_mask(self, mask):
//        self.mask = mask

//    def update_with_geo_entry(self, geo_entry):
//        self.latitude = geo_entry.y
//        self.longitude = geo_entry.x
//        self.altitude = geo_entry.z
//        if geo_entry.yaw is not None and geo_entry.pitch is not None and geo_entry.roll is not None:
//            self.yaw = geo_entry.yaw
//            self.pitch = geo_entry.pitch
//            self.roll = geo_entry.roll
//            self.dls_yaw = geo_entry.yaw
//            self.dls_pitch = geo_entry.pitch
//            self.dls_roll = geo_entry.roll
//        self.gps_xy_stddev = geo_entry.horizontal_accuracy
//        self.gps_z_stddev = geo_entry.vertical_accuracy

//    def compute_focal(self, tags, xtags):
//        try:
//            self.focal_ratio = self.extract_focal(m_camera_make, m_camera_model, tags, xtags)
//        except (IndexError, ValueError) as e:
//            Logger::Write(__FUNCTION__, "Cannot extract focal ratio for %s: %s" % (self.filename, str(e)))

//    def set_attr_from_xmp_tag(self, attr, xmp_tags, tags, cast=None):
//        v = self.get_xmp_tag(xmp_tags, tags)
//        if v is not None:
//            if cast is None:
//                setattr(self, attr, v)
//            else
//                # Handle fractions
//                if (cast == float or cast == int) and "/" in v:
//                    v = self.try_parse_fraction(v)
//                setattr(self, attr, cast(v))

//    def get_xmp_tag(self, xmp_tags, tags):
//        if isinstance(tags, str):
//            tags = [tags]
//
//        for tag in tags:
//            if tag in xmp_tags:
//                t = xmp_tags[tag]
//
//                if isinstance(t, string_types):
//                    return str(t)
//                elif isinstance(t, dict):
//                    items = t.get('rdf:Seq', {}).get('rdf:li', {})
//                    if items:
//                        if isinstance(items, string_types):
//                            return items
//                        return " ".join(items)
//                elif isinstance(t, int) or isinstance(t, float):
//                    return t

//    # From https://github.com/mapillary/OpenSfM/blob/master/opensfm/exif.py
//    def get_xmp(self, file):
//        img_bytes = file.read()
//        xmp_start = img_bytes.find(b'<x:xmpmeta')
//        xmp_end = img_bytes.find(b'</x:xmpmeta')
//
//        if xmp_start < xmp_end:
//            xmp_str = img_bytes[xmp_start:xmp_end + 12].decode('utf8')
//            try:
//                xdict = x2d.parse(xmp_str)
//            except ExpatError as e:
//                from bs4 import BeautifulSoup
//                xmp_str = str(BeautifulSoup(xmp_str, 'xml'))
//                xdict = x2d.parse(xmp_str)
//                Logger::Write(__FUNCTION__, "%s has malformed XMP XML (but we fixed it)" % self.filename)
//            xdict = xdict.get('x:xmpmeta', {})
//            xdict = xdict.get('rdf:RDF', {})
//            xdict = xdict.get('rdf:Description', {})
//            if isinstance(xdict, list):
//                return xdict
//            else
//                return [xdict]
//        else
//            return []
//    def float_values(self, tag):
//        if isinstance(tag.values, list):
//            result = []
//            for v in tag.values:
//                if isinstance(v, int):
//                    result.append(float(v))
//                elif isinstance(v, tuple) and len(v) == 1 and isinstance(v[0], float):
//                    result.append(v[0])
//                elif v.den != 0:
//                    result.append(float(v.num) / float(v.den))
//                else
//                    result.append(None)
//            return result
//        elif hasattr(tag.values, 'den'):
//            return [float(tag.values.num) / float(tag.values.den) if tag.values.den != 0 else None]
//        else
//            return [None]

//    def float_value(self, tag):
//        v = self.float_values(tag)
//        if len(v) > 0:
//            return v[0]

//    def int_values(self, tag):
//        if isinstance(tag.values, list):
//            return [int(v) for v in tag.values]
//        elif isinstance(tag.values, str) and tag.values == '':
//            return []
//        else
//            return [int(tag.values)]

//    def int_value(self, tag):
//        v = self.int_values(tag)
//        if len(v) > 0:
//            return v[0]

//    def list_values(self, tag):
//        return " ".join(map(str, tag.values))

//    def try_parse_fraction(self, val):
//        parts = val.split("/")
//        if len(parts) == 2:
//            try:
//                num, den = map(float, parts)
//                return num / den if den != 0 else val
//            except ValueError:
//                pass
//        return val

//    def get_radiometric_calibration(self):
//        if isinstance(self.radiometric_calibration, str):
//            parts = self.radiometric_calibration.split(" ")
//            if len(parts) == 3:
//                return list(map(float, parts))
//
//        return [None, None, None]

//    def get_dark_level(self):
//        if self.black_level:
//            levels = np.array([float(v) for v in self.black_level.split(" ")])
//            return levels.mean()

//    def get_gain(self):
//        if self.gain is not None:
//            return self.gain
//        elif self.iso_speed:
//            #(gain = ISO/100)
//            return self.iso_speed / 100.0

//    def get_vignetting_center(self):
//        if self.vignetting_center:
//            parts = self.vignetting_center.split(" ")
//            if len(parts) == 2:
//                return list(map(float, parts))
//        return [None, None]

//    def get_vignetting_polynomial(self):
//        if self.vignetting_polynomial:
//            parts = self.vignetting_polynomial.split(" ")
//            if len(parts) > 0:
//                coeffs = list(map(float, parts))
//
//                # Different camera vendors seem to use different ordering for the coefficients
//                if m_camera_make != "Sentera":
//                    coeffs.reverse()
//
//                return coeffs

XString Photo::get_utc_time()
{
    //if (m_utc_time.IsEmpty() == false)
      //  return datetime.fromtimestamp(self.utc_time / 1000, timezone.utc);

    return "";
}

double Photo::GetPhotometricExposure()
{
    double pe = 0.0;
    // H ~= (exposure_time) / (f_number^2)
    if (m_fnumber > 0.0 && m_exposure_time > 0.0)
        pe = m_exposure_time / (m_fnumber * m_fnumber);
    return pe;
}

//    def get_horizontal_irradiance(self):
//        if self.horizontal_irradiance is not None:
//            scale = 1.0 # Assumed
//            if self.irradiance_scale_to_si is not None:
//                scale = self.irradiance_scale_to_si
//
//            return self.horizontal_irradiance * scale
//        elif m_camera_make == "DJI" and self.spectral_irradiance is not None:
//            # Phantom 4 Multispectral saves this value in @drone-dji:Irradiance
//            return self.spectral_irradiance

//    def get_sun_sensor(self):
//        if self.sun_sensor is not None:
//            # TODO: Presence of XMP:SunSensorExposureTime
//            # and XMP:SunSensorSensitivity might
//            # require additional logic. If these two tags are present,
//            # then sun_sensor is not in physical units?
//            return self.sun_sensor / 65535.0 # normalize uint16 (is this correct?)
//        elif self.spectral_irradiance is not None:
//            scale = 1.0 # Assumed
//            if self.irradiance_scale_to_si is not None:
//                scale = self.irradiance_scale_to_si
//
//            return self.spectral_irradiance * scale

//    def get_dls_pose(self):
//        if self.dls_yaw is not None:
//            return [self.dls_yaw, self.dls_pitch, self.dls_roll]
//        return [0.0, 0.0, 0.0]

//    def get_bit_depth_max(self):
//        if self.bits_per_sample:
//            return float(2 ** self.bits_per_sample)
//        else
//            # If it's a JPEG, this must be 256
//            _, ext = os.path.splitext(self.filename)
//            if ext.lower() in [".jpeg", ".jpg"]:
//                return 256.0
//
//        return None

XString Photo::get_capture_id()
{
    // Use capture UUID first, capture time as fallback
    if (m_capture_uuid.IsEmpty() == false)
        return m_capture_uuid;

    return get_utc_time();
}

//    def get_gps_dop(self):
//        val = -9999
//        if self.gps_xy_stddev is not None:
//            val = self.gps_xy_stddev
//        if self.gps_z_stddev is not None:
//            val = max(val, self.gps_z_stddev)
//        if val > 0:
//            return val
//
//        return None

//    def override_gps_dop(self, dop):
//        self.gps_xy_stddev = self.gps_z_stddev = dop

//    def override_camera_projection(self, camera_projection):
//        if camera_projection in projections:
//            m_camera_projection = camera_projection

bool Photo::is_rgb()
{
    if (m_band_name.CompareNoCase("RGB") || m_band_name.CompareNoCase("REDGREENBLUE"))
        return true;

    return false;
}

bool Photo::is_thermal()
{
    // Added for support M2EA camera sensor
    if (m_camera_make.CompareNoCase("DJI") && m_camera_model.CompareNoCase("MAVIC2-ENTERPRISE-ADVANCED") && m_width == 640 && m_height == 512)
        return true;

    // Added for support DJI H20T camera sensor
    if (m_camera_make.CompareNoCase("DJI") && m_camera_model.CompareNoCase("ZH20T") && m_width == 640 && m_height == 512)
        return true;

    if (m_band_name.CompareNoCase("LWIR"))   // TODO: more ?
        return true;

    return false;
}

//    def camera_id(self):
//        return " ".join(
//                [
//                    "v2",
//                    m_camera_make.strip(),
//                    m_camera_model.strip(),
//                    str(int(m_width)),
//                    str(int(m_height)),
//                    m_camera_projection,
//                    str(float(self.focal_ratio))[:6],
//                ]
//            ).lower()

//    def to_opensfm_exif(self, rolling_shutter = False, rolling_shutter_readout = 0):
//        capture_time = 0.0
//        if self.utc_time is not None:
//            capture_time = self.utc_time / 1000.0
//
//        gps = {}
//        has_gps = self.latitude is not None and self.longitude is not None
//        if has_gps:
//            gps['latitude'] = self.latitude
//            gps['longitude'] = self.longitude
//            if self.altitude is not None:
//                gps['altitude'] = self.altitude
//            else
//                gps['altitude'] = 0.0
//
//            dop = self.get_gps_dop()
//            if dop is None:
//                dop = 10.0 # Default
//
//            gps['dop'] = dop
//
//        d = {
//            "make": m_camera_make,
//            "model": m_camera_model,
//            "width": m_width,
//            "height": m_height,
//            "projection_type": m_camera_projection,
//            "focal_ratio": self.focal_ratio,
//            "orientation": self.orientation,
//            "capture_time": capture_time,
//            "gps": gps,
//            "camera": m_camera_id()
//        }
//
//        if self.has_opk():
//            d['opk'] = {
//                'omega': self.omega,
//                'phi': self.phi,
//                'kappa': self.kappa
//            }
//
//        # Speed is not useful without GPS
//        if self.has_speed() and has_gps:
//            d['speed'] = [self.speed_y, self.speed_x, self.speed_z]
//
//        if rolling_shutter:
//            d['rolling_shutter'] = get_rolling_shutter_readout(self, rolling_shutter_readout)
//
//        return d

//    def has_ypr(self):
//        return self.yaw is not None and \
//            self.pitch is not None and \
//            self.roll is not None

//    def has_opk(self):
//        return self.omega is not None and \
//            self.phi is not None and \
//            self.kappa is not None

//    def has_speed(self):
//        return self.speed_x is not None and \
//                self.speed_y is not None and \
//                self.speed_z is not None

//    def has_geo(self):
//        return self.latitude is not None and \
//            self.longitude is not None

void Photo::compute_opk()
{
    //if self.has_ypr() and self.has_geo():
    {
        //    y, p, r = math.radians(self.yaw), math.radians(self.pitch), math.radians(self.roll)

    //    # Ref: New Calibration and Computing Method for Direct
    //    # Georeferencing of Image and Scanner Data Using the
    //    # Position and Angular Data of an Hybrid Inertial Navigation System
    //    # by Manfred Bäumker

    //    # YPR rotation matrix
    //    cnb = np.array([[ math.cos(y) * math.cos(p), math.cos(y) * math.sin(p) * math.sin(r) - math.sin(y) * math.cos(r), math.cos(y) * math.sin(p) * math.cos(r) + math.sin(y) * math.sin(r)],
    //                    [ math.sin(y) * math.cos(p), math.sin(y) * math.sin(p) * math.sin(r) + math.cos(y) * math.cos(r), math.sin(y) * math.sin(p) * math.cos(r) - math.cos(y) * math.sin(r)],
    //                    [ -math.sin(p), math.cos(p) * math.sin(r), math.cos(p) * math.cos(r)],
    //                   ])

    //    # Convert between image and body coordinates
    //    # Top of image pixels point to flying direction
    //    # and camera is looking down.
    //    # We might need to change this if we want different
    //    # camera mount orientations (e.g. backward or sideways)

    //    # (Swap X/Y, flip Z)
    //    cbb = np.array([[0, 1, 0],
    //                    [1, 0, 0],
    //                    [0, 0, -1]])

    //    delta = 1e-7

    //    alt = self.altitude if self.altitude is not None else 0.0
    //    p1 = np.array(ecef_from_lla(self.latitude + delta, self.longitude, alt))
    //    p2 = np.array(ecef_from_lla(self.latitude - delta, self.longitude, alt))
    //    xnp = p1 - p2
    //    m = np.linalg.norm(xnp)

    //    if m == 0:
    //        Logger::Write(__FUNCTION__, "Cannot compute OPK angles, divider = 0")
    //        return

    //    # Unit vector pointing north
    //    xnp /= m

    //    znp = np.array([0, 0, -1]).T
    //    ynp = np.cross(znp, xnp)

    //    cen = np.array([xnp, ynp, znp]).T

    //    # OPK rotation matrix
    //    ceb = cen.dot(cnb).dot(cbb)

    //    self.omega = math.degrees(math.atan2(-ceb[1][2], ceb[2][2]))
    //    self.phi = math.degrees(math.asin(ceb[0][2]))
    //    self.kappa = math.degrees(math.atan2(-ceb[0][1], ceb[0][0]))
    }
}

double Photo::get_capture_megapixels()
{
    double mp = 0.0;

    if (m_exif_width > 0 && m_exif_height)
    {
        // Accurate so long as resizing / postprocess software
        // did not fiddle with the tags
        mp = m_exif_width * m_exif_height / 1E6;
    }
    else if (m_width > 0 && m_height > 0)
    {
        // Fallback, might not be accurate since the image
        // could have been resized
        mp = m_width * m_height / 1E6;
    }
    
    return mp;
}

XString Photo::GetFileName()
{
    return m_file_name;
}

int Photo::GetWidth()
{
    return m_width;
}

int Photo::GetHeight()
{
    return m_height;
}

int Photo::GetExifWidth()
{
    return m_exif_width;
}

int Photo::GetExifHeight()
{
    return m_exif_height;
}

XString Photo::GetModel()
{
    return m_camera_model;
}

XString Photo::GetMake()
{
    return m_camera_make;
}

int Photo::GetOrientation()
{
    return m_orientation;
}

double Photo::GetLatitude()
{
    return m_latitude;
}

double Photo::GetLongitude()
{
    return m_longitude;
}

double Photo::GetAltitude()
{
    return m_altitude;
}

double Photo::GetYaw()
{
    return m_yaw;
}

double Photo::GetPitch()
{
    return m_pitch;
}

double Photo::GetRoll()
{
    return m_roll;
}

XString Photo::GetBandName()
{
    return m_band_name;
}

double Photo::GetFNumber()
{
    return m_fnumber;
}

int Photo::GetISOSpeed()
{
    return m_iso_speed;
}

double Photo::GetExposureTime()
{
    return m_exposure_time;
}

double Photo::GetGain()
{
    return m_gain;
}

double Photo::GetGainAdjustment()
{
    return m_gain_adjustment;
}

double Photo::GetFocalLength()
{
    return m_focal_length;
}

double Photo::GetFocalRatio()
{
    return m_focal_ratio;
}

double Photo::GetFocalPlaneXResolution()
{
    return m_focal_plane_xres;
}

double Photo::GetFocalPlaneYResolution()
{
    return m_focal_plane_yres;
}

int Photo::GetFocalPlaneResolutionUnit()
{
    return m_focal_plane_res_unit;
}

XString Photo::GetCaptureDate()
{
    return m_capture_date;
}

__int64 Photo::GetCaptureEpoch()
{
    return m_epoch;
}

XString Photo::GetCameraStr(bool opensfm)
{
    // Return camera id string.
    //

    XString camera_str = XString::Format("%s %s %d %d brown %0.4f",
        m_camera_make.c_str(), m_camera_model.c_str(),
        m_width, m_height,
        m_focal_ratio);

    // opensfm and odm have slightly different formats
    if (opensfm)
        camera_str.Insert(0, "v2 ");

    camera_str.MakeLower();

    return camera_str;
}

double Photo::CalcFocalRatio()
{
    double focal_ratio = 0.85;

    double sensor_width = 0.0;
    if (m_focal_plane_xres > 0.0 && m_focal_plane_yres > 0.0)
    {
        double mm_per_unit = get_mm_per_unit(m_focal_plane_res_unit);
        if (mm_per_unit > 0.0)
        {
            double pixels_per_unit = m_focal_plane_xres;
            if (pixels_per_unit <= 0.0 && m_focal_plane_yres > 0.0)
                pixels_per_unit = m_focal_plane_yres;

            if ((pixels_per_unit > 0.0) && (m_width > 0))
            {
                double units_per_pixel = 1.0 / pixels_per_unit;
                sensor_width = (double)m_width * units_per_pixel * mm_per_unit;
            }
        }
    }

    //focal_35 = None
    double focal = 0.0;
    //if "EXIF FocalLengthIn35mmFilm" in tags:
    //	focal_35 = self.float_value(tags["EXIF FocalLengthIn35mmFilm"])
    if (m_focal_length > 0.0)
        focal = m_focal_length;
    //if focal is None and "@aux:Lens" in xtags:
    //	lens = self.get_xmp_tag(xtags, ["@aux:Lens"])
    //	matches = re.search('([\d\.]+)mm', str(lens))
    //	if matches:
    //		focal = float(matches.group(1))

    //if focal_35 is not None and focal_35 > 0:
    //	focal_ratio = focal_35 / 36.0  # 35mm film produces 36x24mm pictures.
    //else
    //	if not sensor_width:
    //		sensor_width = sensor_data().get(sensor_string, None)
    if ((sensor_width > 0.0) && (focal > 0.0))
        focal_ratio = focal / sensor_width;

    return focal_ratio;
}

int Photo::yisleap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Photo::get_yday(int mon, int day, int year)
{
    static const int days[2][13] = {
        {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
        {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
    };
    int leap = yisleap(year);

    return days[leap][mon] + day;
}

__int64 Photo::CalcUnixEpoch(const char* dateTime)
{
    // Return seconds since Jan 1, 1950.
    //
    // Inputs:
    //		dateTime = "YYYY:MM:SS hh:mm:ss"
    // Outputs:
    //		return = seconds since Jan 1, 1970
    //

    __int64 epoch = 0;

    XString str = dateTime;
    if (str.GetLength() == 19)
    {
        __int64 tm_year = str.Mid(0, 4).GetLong();
        __int64 tm_month = str.Mid(5, 2).GetLong();
        __int64 tm_day = str.Mid(8, 2).GetLong();
        __int64 tm_hour = str.Mid(11, 2).GetLong();
        __int64 tm_min = str.Mid(14, 2).GetLong();
        __int64 tm_sec = str.Mid(17, 2).GetLong();

        __int64 tm_yday = get_yday((int)tm_month, (int)tm_day, (int)tm_year);

        // yday is 1-based
        --tm_yday;

        tm_year -= 1900;

        epoch = tm_sec + tm_min * 60 + tm_hour * 3600 + tm_yday * 86400 +
            (tm_year - 70) * 31536000 + ((tm_year - 69) / 4) * 86400 -
            ((tm_year - 1) / 100) * 86400 + ((tm_year + 299) / 400) * 86400;
    }

    return epoch;
}

bool Photo::is_supported_format(XString file_name)
{
    // Looking at extenions only, return true if it's
    // a supported image formate.
    //

    if ((file_name.EndsWithNoCase(".jpg") == false) && (file_name.EndsWithNoCase(".jpeg") == false))
        return true;

    if ((file_name.EndsWithNoCase(".tif") == false) && (file_name.EndsWithNoCase(".tiff") == false))
        return true;

    return false;
}

XString Photo::LoadXmpMeta(XString file_name)
{
    // Brute force method of finding & returning the xmp xml.
    //

//only applies to tif files...

    XString xml;

    FILE* fp = fopen(file_name.c_str(), "rb");
    if (!fp)
    {
        Logger::Write(__FUNCTION__, "Unable to open image file: '%s'", file_name.c_str());
        return "";
    }
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    unsigned char* buf = new unsigned char[fsize];
    if (fread(buf, 1, fsize, fp) != fsize)
    {
        Logger::Write(__FUNCTION__, "Unable to open read file: '%s'", file_name.c_str());
        delete[] buf;
        return "";
    }
    fclose(fp);

    // scan for the opening XMP tag

    unsigned char* pstart = nullptr;
    int start_len = (int)strlen("<x:xmpmeta");
    int end_len = (int)strlen("</x:xmpmeta>");
    for (unsigned char* p = buf; p < buf + fsize - start_len; ++p)
    {
        if (pstart == nullptr)
        {
            if (memcmp(p, "<x:xmpmeta", start_len) == 0)
            {
                pstart = p;

                xml = (const char*)pstart;
                int pos = xml.Find("</x:xmpmeta>");
                if (pos > -1)
                    xml = xml.Left(pos + end_len);
            }
        }
    }

    delete[] buf;

    return xml;
}
