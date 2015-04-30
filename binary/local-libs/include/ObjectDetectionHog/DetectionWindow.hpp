#ifndef __DETECTION_WINDOW_HPP__
#define __DETECTION_WINDOW_HPP__

#include "Feature.hpp"

#include <ViNotion/Compat.hpp>

#include <string>

//! Describes a detection window.
class EXPORT_TYPE CDetectionWindow
{
public:

	float x;
	float y;
  float width;
  float height;
	float score;
	float scale;
	std::string imgnum; // PASCAL VOC setting.
  CFeatureResponseList features;

public:
	//! Constructor.
	CDetectionWindow() : 
	  x(0.0f), y(0.0f), score(0.0f), scale(0.0f) {} 

	//! Copy constructor.
	CDetectionWindow(const CDetectionWindow &a);

	//! Assignment operator.
	CDetectionWindow& operator=(const CDetectionWindow &a);

	//! Destructor.
	~CDetectionWindow();

	bool operator<(const CDetectionWindow &a) const;

};

//! Detection window.
class CDetection
{
public:
  //! Left coordinate of object region.
  int x;
  //! Top coordinate of object region.
  int y;
  //! Width of object region.
  int width;
  //! Height of object region.
  int height;
  //! Object class.
  int objectClass;

};



#endif //__DETECTION_WINDOW_HPP__
