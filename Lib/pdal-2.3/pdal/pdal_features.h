/*
 * originally generated by automake, now hand-maintained
 *
 */
#pragma once

/*
 * version settings
 */
#define PDAL_VERSION_MAJOR 2
#define PDAL_VERSION_MINOR 3
#define PDAL_VERSION_PATCH 0

#define PDAL_VERSION "2.3.0"

/* (note this will look yucky until we get to major>=1) */
#define PDAL_VERSION_INTEGER ((PDAL_VERSION_MAJOR*100*100)+(PDAL_VERSION_MINOR*100)+PDAL_VERSION_PATCH)

//#cmakedefine PDAL_PLUGIN_INSTALL_PATH "@PDAL_PLUGIN_INSTALL_PATH@"
/*
 * availability of 3rd-party libraries
 */
//#cmakedefine PDAL_HAVE_LASZIP
//#cmakedefine PDAL_HAVE_LAZPERF
//#cmakedefine PDAL_HAVE_HDF5
//#cmakedefine PDAL_HAVE_ZSTD
//#cmakedefine PDAL_HAVE_ZLIB
//#cmakedefine PDAL_HAVE_LZMA
//#cmakedefine PDAL_HAVE_LIBXML2
#define PDAL_LAS_START

/*
 * Debug or Release build?
 */
#define PDAL_BUILD_TYPE "@PDAL_BUILD_TYPE@"

/*
 * built pdal app as application bundle on OSX?
 */
//#cmakedefine PDAL_APP_BUNDLE

