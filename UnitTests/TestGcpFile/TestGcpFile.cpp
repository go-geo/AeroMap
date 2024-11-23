// TestGcpFile.cpp
// CppUnitLite test harness for GcpFile class
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>

#include "CppUnitLite/TestHarness.h"		// CppUnitLite library
#include "../Common/UnitTest.h"				// unit test helpers
#include "GcpFile.h"						// interface to class under test

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
TEST(GcpFile, test1)
{
	XString file_name = XString::CombinePath(gs_DataPath, "gcp_list.txt");

	GcpFile gcpFile(file_name.c_str());
	LONGS_EQUAL(3, gcpFile.get_gcp_count());

	{
		GcpFile::GcpType gcp = gcpFile.get_gcp(0);
		//544256.7 5320919.9 5 3044 2622 IMG_0525.jpg
		DOUBLES_EQUAL(544256.7, gcp.x, 0.0);
		DOUBLES_EQUAL(5320919.9, gcp.y, 0.0);
		DOUBLES_EQUAL(5.0, gcp.z, 0.0);
		LONGS_EQUAL(3044, gcp.px);
		LONGS_EQUAL(2622, gcp.py);
		CHECK(gcp.file_name == "IMG_0525.jpg");
	}
	{
		GcpFile::GcpType gcp = gcpFile.get_gcp(1);
		//544157.7 5320899.2 5 4193 1552 IMG_0585.jpg
		DOUBLES_EQUAL(544157.7, gcp.x, 0.0);
		DOUBLES_EQUAL(5320899.2, gcp.y, 0.0);
		DOUBLES_EQUAL(5.0, gcp.z, 0.0);
		LONGS_EQUAL(4193, gcp.px);
		LONGS_EQUAL(1552, gcp.py);
		CHECK(gcp.file_name == "IMG_0585.jpg");
	}
	{
		GcpFile::GcpType gcp = gcpFile.get_gcp(2);
		//544033.4 5320876.0 5 1606 2763 IMG_0690.jpg
		DOUBLES_EQUAL(544033.4, gcp.x, 0.0);
		DOUBLES_EQUAL(5320876.0, gcp.y, 0.0);
		DOUBLES_EQUAL(5.0, gcp.z, 0.0);
		LONGS_EQUAL(1606, gcp.px);
		LONGS_EQUAL(2763, gcp.py);
		CHECK(gcp.file_name == "IMG_0690.jpg");
	}
}
