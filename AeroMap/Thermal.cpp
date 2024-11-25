// Thermal.cpp
// Port of odm thermal.py
//

#include "Thermal.h"

Photo* Thermal::resize_to_match(Photo* photo, Photo* match_photo)
{
	// Resize images to match the dimension of another photo
	// 
	// Inputs:
	//		photo		= photo to resize
	//		match_photo = photo whose dimensions should be used for resize
	// Outputs:
	//		return		= resized image
	//    
	
	if (match_photo != nullptr)
	{
		int w = photo->GetWidth();
		int h = photo->GetHeight();
		if (w != match_photo->GetWidth() || h != match_photo->GetHeight())
		{
			double fx = (double)match_photo->GetWidth() / (double)w;
			double fy = (double)match_photo->GetHeight() / (double)h;
			cv::resize(photo->image, photo->image, cv::Size(), fx, fy, cv::INTER_LANCZOS4);
		}
	}

	return photo;
}

Photo* Thermal::dn_to_temperature(Photo* photo, Photo* image, XString images_path)
{
	// Convert Digital Number values to temperature (C) values
	// 
	// Inputs:
	//    :param photo ODM_Photo
	//    :param image numpy array containing image data
	//    :param images_path path to original source image to read data using PIL for DJI thermal photos
	// Outputs:
	//    :return numpy array with temperature (C) image values
	//

	// Handle thermal bands
	if (photo->is_thermal())
	{
		// Every camera stores thermal information differently
		// The following will work for MicaSense Altum cameras
		// but not necessarily for others
		if (photo->GetMake() == "MicaSense" && photo->GetModel() == "Altum")
		{
			//    image = image.astype("float32")
			//    image -= (273.15 * 100.0) # Convert Kelvin to Celsius
			//    image *= 0.01
		}
		else if (photo->GetMake() == "DJI" && photo->GetModel() == "ZH20T")
		{
			//    filename, file_extension = os.path.splitext(photo.filename)
		//    # DJI H20T high gain mode supports measurement of -40~150 celsius degrees
		//    if file_extension.lower() in [".tif", ".tiff"] and image.min() >= 23315: # Calibrated grayscale tif
		//        image = image.astype("float32")
		//        image -= (273.15 * 100.0) # Convert Kelvin to Celsius
		//        image *= 0.01
		}
		else if (photo->GetMake() == "DJI" && photo->GetModel() == "MAVIC2-ENTERPRISE-ADVANCED")
		{
			//    image = dji_unpack.extract_temperatures_dji(photo, image, images_path)
			//    image = image.astype("float32")
		}
		else
		{
			//    try:
			//        params, image = extract_raw_thermal_image_data(os.path.join(images_path, photo.filename))
			//        image = sensor_vals_to_temp(image, **params)
			//    except Exception as e:
			//        log.ODM_WARNING("Cannot radiometrically calibrate %s: %s" % (photo.filename, str(e)))

			//    image = image.astype("float32")
		}
	}
	else
	{
		//image = image.astype("float32")
		//log.ODM_WARNING("Tried to radiometrically calibrate a non-thermal image with temperature values (%s)" % photo.filename)
	}

	return image;
}
