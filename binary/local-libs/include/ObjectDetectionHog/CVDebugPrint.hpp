#ifndef __CVDEBUGPRINT_HPP__
#define __CVDEBUGPRINT_HPP__


#include <cv.h>


//! Writes a 32-bit image to disk, for 8-bit images use the OpenCV-
//! function cvSaveImage() instead.
/*! 
	*	@param[in]	f_fileName	defines the filename of the file to be written.
	*							Note that the extention of the graphical file
	*							must be added to the filename and be supported
	*							by OpenCV.
	*	@param[in]	f_image		the IplImage that is to be saved.
	*	@param[in]	f_scaleToUnit	should be set to false if the image should
	*							be scaled to the range [0.0 - 1.0] instead of
	*							to the minimum and maximum values in the image.
	*/
void debugWriteImage(	
		const char* f_fileName, 
		IplImage  *	f_image,
		bool		f_scaleToUnit = false);


//! Shows a 32-bit image on the HighGUI window specified, for 8-bit images,
//! use the function cvShowImage() directly.
void debugShowImage(
		char	  * f_window,
		IplImage  * f_image,
		bool		f_scaleToUnit = false);

						
#endif // __CVDEBUGPRINT_HPP__
