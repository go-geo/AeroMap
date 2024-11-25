#ifndef PHOTO_H
#define PHOTO_H

#include <opencv2/opencv.hpp>	// OpenCV

#include "tiffio.h"
#include "Calc.h"
#include "Logger.h"
#include "XString.h"

class Photo
{
public:

    cv::Mat image;            // full image loaded into 2d array

public:
	
	Photo();
	Photo(XString file_name);
	~Photo();

    XString GetFileName();

    int GetWidth();
    int GetHeight();

    int GetExifWidth();
    int GetExifHeight();

    XString GetModel();
    XString GetMake();

    int GetOrientation();

    double GetLatitude();
    double GetLongitude();
    double GetAltitude();

    double GetYaw();
    double GetPitch();
    double GetRoll();

    XString GetBandName();

    double GetFNumber();
    int    GetISOSpeed();
    double GetExposureTime();
    double GetFocalLength();

    double GetFocalPlaneXResolution();
    double GetFocalPlaneYResolution();
    int    GetFocalPlaneResolutionUnit();

    double GetFocalRatio();
    XString GetCameraStr(bool opensfm);

    XString GetCaptureDate();
    __int64 GetCaptureEpoch();

    bool is_rgb();
    bool is_thermal();

    double GetGain();
    double GetGainAdjustment();
    double GetPhotometricExposure();
    double get_capture_megapixels();
    XString get_capture_id();
    XString get_utc_time();

	static SizeType find_largest_photo_dims(const std::vector<Photo*>& photos);
	static int      find_largest_photo_dim(const std::vector<Photo*>& photos);
	static Photo*   find_largest_photo(const std::vector<Photo*>& photos);

	static double get_mm_per_unit(int resolution_unit);

    static bool is_supported_format(XString file_name);

private:

    XString m_file_name;

	// Standard tags (virtually all photos have these)
	int m_width;
	int m_height;
	XString m_camera_make;
	XString m_camera_model;
	int m_orientation;

	// Geo tags
	double m_latitude;
	double m_longitude;
	double m_altitude;

	// Multi-band fields
	XString m_band_name;
	int m_band_index;
    XString m_capture_uuid;

	// Multi-spectral fields
	double m_fnumber;
	//m_radiometric_calibration = None
	//m_black_level = None
    double m_gain;
    double m_gain_adjustment;

    // Capture info
    double m_exposure_time;
    int m_iso_speed;
    //m_bits_per_sample = None
    //m_vignetting_center = None
    //m_vignetting_polynomial = None
    //m_spectral_irradiance = None
    //m_horizontal_irradiance = None
    //m_irradiance_scale_to_si = None
    //m_utc_time = None

    // OPK angles
    double m_yaw;
    double m_pitch;
    double m_roll;
    double m_omega;
    double m_phi;
    double m_kappa;

    // DLS
    //m_sun_sensor = None
    //m_dls_yaw = None
    //m_dls_pitch = None
    //m_dls_roll = None

    // Aircraft speed
    double m_speed_x;
    double m_speed_y;
    double m_speed_z;

    // Original image width/height at capture time (before possible resizes)
    int m_exif_width;
    int m_exif_height;

    // m_center_wavelength = None
    // m_bandwidth = None

    // RTK
    double m_gps_xy_stddev;     // Dilution of Precision X/Y
    double m_gps_z_stddev;      // Dilution of Precision Z

    // Misc SFM
    XString m_camera_projection;
    double m_focal_ratio;

    double m_focal_plane_xres;
    double m_focal_plane_yres;
    int m_focal_plane_res_unit;
    double m_focal_length;

    double m_spectral_irradiance;      // amount of energy that hits a surface per unit of time, per unit of area, and per unit of wavelength

    XString m_capture_date;     // capture date/time
    __int64 m_epoch;			// capture date/time, unix epoch

private:

    void parse_exif_values(XString file_name);
    void parse_tiff_values(XString file_name);
    
    double get_rational(TIFF* pTiff, uint32_t tag);
    uint32_t get_uint32(TIFF* pTiff, uint32_t tag);

    double CalcFocalRatio();
    void compute_opk();

    int yisleap(int year);
    int get_yday(int mon, int day, int year);
    __int64 CalcUnixEpoch(const char* dateTime);

    XString LoadXmpMeta(XString file_name);
};

#endif // #ifndef PHOTO_H
