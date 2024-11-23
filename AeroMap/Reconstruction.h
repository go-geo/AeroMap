#ifndef RECONSTRUCTION_H
#define RECONSTRUCTION_H

#include "Photo.h"

class Reconstruction
{
public:

	// multispectral data type, used for separating images
	// into lists by band name
	struct MultiType
	{
		XString band_name;
		std::vector<Photo*> photos;
	};

public:

	Reconstruction::Reconstruction(const std::vector<Photo*> photos);
	Reconstruction::~Reconstruction();

	const std::vector<MultiType>& GetMulti();

	bool is_georef();
	bool has_gcp();
	bool is_multi();

private:

	std::vector<MultiType> m_multi;
	std::vector<Photo*> m_photos;

private:

	void detect_multi_camera();
	int  get_band_index(XString band_name);
};

#endif // #ifndef RECONSTRUCTION_H

