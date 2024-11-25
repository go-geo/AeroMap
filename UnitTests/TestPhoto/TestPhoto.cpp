// TestPhoto.cpp
//
// CppUnitLite test harness for Photo class
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "CppUnitLite/TestHarness.h"	// CppUnitLite library
#include "../Common/UnitTest.h"			// unit test helpers
#include "Photo.h"						// interface to class under test

int main(int argc, char* argv[])
{
	// output name of executable
	if (argc > 0 && argv[0])
		printf("%s\n", argv[0]);
	
	SetDataPath(argv[0]);

	TestResult tr;
	TestRegistry::runAllTests(tr);

	// always pause if errors
	int failureCount = tr.GetFailureCount();
	if (failureCount > 0)
		getc(stdin);

	return failureCount;
}


//----------------------------------------------------------------------------
TEST(Photo, RGB)
{
	// Test a basic RBG image.
	//

	XString file_name = XString::CombinePath(gs_DataPath, "IMG_0428.JPG");

	{
		Photo photo(file_name.c_str());

		CHECK(file_name == photo.GetFileName());

		LONGS_EQUAL(4000, photo.GetWidth());
		LONGS_EQUAL(3000, photo.GetHeight());
		DOUBLES_EQUAL(12.0, photo.get_capture_megapixels(), 0.0);

		int orientation = photo.GetOrientation();
		LONGS_EQUAL(6, orientation);

		CHECK(photo.GetMake() == "Canon");
		CHECK(photo.GetModel() == "Canon DIGITAL IXUS 120 IS");

		CHECK(photo.GetBandName() == "RGB");
		CHECK(photo.is_rgb() == true);
		CHECK(photo.is_thermal() == false);

		double fnumber = photo.GetFNumber();
		DOUBLES_EQUAL(2.8, fnumber, 1E-6);
		int iso = photo.GetISOSpeed();
		LONGS_EQUAL(125, iso);
		double focal_length = photo.GetFocalLength();
		DOUBLES_EQUAL(5.0, focal_length, 0.0);
		double exposure_time = photo.GetExposureTime();
		DOUBLES_EQUAL(0.002, exposure_time, 0.0);

		DOUBLES_EQUAL(0.0, photo.GetGain(), 0.0);
		DOUBLES_EQUAL(0.0, photo.GetGainAdjustment(), 0.0);

		double focal_ratio = photo.GetFocalRatio();
		DOUBLES_EQUAL(0.80676390860978453, focal_ratio, 0.0);

		XString camera_str_osfm = photo.GetCameraStr(true);
		XString camera_str_odm = photo.GetCameraStr(false);

		CHECK(camera_str_odm == "canon canon digital ixus 120 is 4000 3000 brown 0.8068");
		CHECK(camera_str_osfm == "v2 canon canon digital ixus 120 is 4000 3000 brown 0.8068");

		XString capture_date = photo.GetCaptureDate();
		CHECK(capture_date == "2011:03:04 16:42:16");

		__int64 epoch = photo.GetCaptureEpoch();
		LONGS_EQUAL(1299256936, (long)epoch);
	}
}

//----------------------------------------------------------------------------
TEST(Photo, Thermal)
{
	// Test an infrared image.
	//

	XString file_name = XString::CombinePath(gs_DataPath, "DJI_0058.JPG");

	{
		Photo photo(file_name.c_str());

		CHECK(file_name == photo.GetFileName());

		LONGS_EQUAL(640, photo.GetWidth());
		LONGS_EQUAL(512, photo.GetHeight());
		DOUBLES_EQUAL(0.32768, photo.get_capture_megapixels(), 0.0);

		int orientation = photo.GetOrientation();
		//LONGS_EQUAL(6, orientation);

		CHECK(photo.GetMake() == "DJI");
		CHECK(photo.GetModel() == "FLIR");

		CHECK(photo.GetBandName() == "LWIR");
		CHECK(photo.is_rgb() == false);
		CHECK(photo.is_thermal() == true);

		double fnumber = photo.GetFNumber();
		DOUBLES_EQUAL(1.4, fnumber, 1E-6);
		int iso = photo.GetISOSpeed();
		//LONGS_EQUAL(125, iso);
		double focal_length = photo.GetFocalLength();
		DOUBLES_EQUAL(9.0, focal_length, 0.0);

		DOUBLES_EQUAL(0.0, photo.GetGain(), 0.0);
		DOUBLES_EQUAL(0.0, photo.GetGainAdjustment(), 0.0);

		double focal_ratio = photo.GetFocalRatio();
		DOUBLES_EQUAL(0.8500, focal_ratio, 1E-6);

		XString camera_str_osfm = photo.GetCameraStr(true);
		XString camera_str_odm = photo.GetCameraStr(false);
		CHECK(camera_str_odm == "dji flir 640 512 brown 0.8500");
		CHECK(camera_str_osfm == "v2 dji flir 640 512 brown 0.8500");

		XString capture_date = photo.GetCaptureDate();
		CHECK(capture_date == "2019:02:05 13:49:04");

		__int64 epoch = photo.GetCaptureEpoch();
		LONGS_EQUAL(1549374544, (long)epoch);
	}
}

//----------------------------------------------------------------------------
TEST(Photo, Multi)
{
	// Test multispectral band stored at tiff file.
	//

	XString file_name = XString::CombinePath(gs_DataPath, "IMG_180518_130857_0000_GRE.TIF");

	{
		Photo photo(file_name.c_str());

		CHECK(file_name == photo.GetFileName());

		LONGS_EQUAL(1280, photo.GetWidth());
		LONGS_EQUAL(960, photo.GetHeight());
		DOUBLES_EQUAL(1.2288, photo.get_capture_megapixels(), 0.0);

		LONGS_EQUAL(1280, photo.GetExifWidth());
		LONGS_EQUAL(960, photo.GetExifHeight());

		LONGS_EQUAL(1, photo.GetOrientation());

		CHECK(photo.GetMake() == "Parrot");
		CHECK(photo.GetModel() == "Sequoia");

		CHECK(photo.GetBandName() == "Green");
		CHECK(photo.is_rgb() == false);
		CHECK(photo.is_thermal() == false);

		DOUBLES_EQUAL(0.000969230, photo.GetExposureTime(), 1E-9);

		DOUBLES_EQUAL(-145.547256, photo.GetYaw(), 1E-6);
		DOUBLES_EQUAL(11.779017, photo.GetPitch(), 1E-6);
		DOUBLES_EQUAL(-6.244004, photo.GetRoll(), 1E-6);

		double pe = photo.GetPhotometricExposure();
		DOUBLES_EQUAL(0.00020025425517780185, pe, 1E-9);
		DOUBLES_EQUAL(1.0, photo.GetGain(), 0.0);
		DOUBLES_EQUAL(0.0, photo.GetGainAdjustment(), 0.0);

		DOUBLES_EQUAL(266.6666559, photo.GetFocalPlaneXResolution(), 1E-6);
		DOUBLES_EQUAL(266.6666559, photo.GetFocalPlaneYResolution(), 1E-6);
		LONGS_EQUAL(4, photo.GetFocalPlaneResolutionUnit());
		
		double focal_ratio = photo.GetFocalRatio();
		DOUBLES_EQUAL(0.8500, focal_ratio, 1E-6);

		XString capture_date = photo.GetCaptureDate();
		CHECK(capture_date == "2018:05:18 13:08:57");
		XString uuid = photo.get_capture_id();
		CHECK(uuid == "0900EB3C0598921532B04A6C40340E0C");

		XString camera_str_osfm = photo.GetCameraStr(true);
		XString camera_str_odm = photo.GetCameraStr(false);
		CHECK(camera_str_osfm == "v2 parrot sequoia 1280 960 brown 0.8500");
		CHECK(camera_str_odm == "parrot sequoia 1280 960 brown 0.8500");
	}
}
