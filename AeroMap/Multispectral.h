#ifndef MULTISPECTRAL_H
#define MULTISPECTRAL_H

#include "AeroMap.h"		// application header
#include "Reconstruction.h"

class Multispectral
{
public:

    static Photo* dn_to_radiance(Photo* photo);
    static double compute_irradiance(Photo* photo, bool use_sun_sensor = true);
    static XString get_primary_band_name(std::vector<Reconstruction::MultiType> multi_camera, XString user_band_name);
    static VEC3 vignette_map(Photo* photo);
};

#endif // #ifndef MULTISPECTRAL_H
