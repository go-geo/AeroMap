#ifndef THERMAL_H
#define THERMAL_H

#include "Photo.h"

class Thermal
{
public:

	static Photo* resize_to_match(Photo* image, Photo* match_photo = nullptr);
	static Photo* dn_to_temperature(Photo* photo, Photo* image, XString images_path);
};

#endif // #ifndef THERMAL_H
