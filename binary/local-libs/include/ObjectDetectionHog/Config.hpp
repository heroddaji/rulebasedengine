#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include "SimpleIni.h"
#include <ViNotion/Timer.hpp>
#include <vector>

//#############################################################################
// Defines
//#############################################################################

//! Write testbench data.
//#define TESTBENCH_HOG

//! Minimum processing scale, typically set to 0.7 to allow for limited amount
//! of upsampling (scale 1.0 = input res, scale 2.0 is 2x downsampled).
#define SCALE_MINIMUM 0.7f

//! Define this if detection results should be written to disk.
//#define DETECTOR_SAVE_RESULTS_DISK_JPEG

//! Define to use OpenCV for image scaling, instead of libViNotion scaling.
#define IMG_SCALING_OPENCV

//! Show the image and the detection results overlayed.
//#define DETECTOR_SHOW_RESULTS

//! Gradient calculation using Sobel filtering or simple gradient.
/*! 
 *  If GRADIENT_SOBEL is defined, the filter is used:
 *    [ -1 0 +1 ]
 *    [ -2 0 +2 ]
 *    [ -1 0 +1 ]
 *
 *  Else, the following simple filter is used:
 *    [ -1 0 +1 ]
 */
//#define GRADIENT_SOBEL

//! Clip the HOG gradient values to a maximum value to avoid problems with very 
//! high-contast images (like the black trees against the white background 
//! in panoramas).
//#define HOG_GRADIENT_CLIP_MAX
//! Maximum gradient value.
//#define HOG_GRADIENT_CLIP_MAX_VALUE     16.0f

//! Threshold the gradient values to remove noise in the gradient channel.
//! Values smaller than HOG_GRADIENT_CLIP_MIN_VALUE will be set to 0.0f.
//#define HOG_GRADIENT_CLIP_MIN
//! Minimum gradient value noise level.
//#define HOG_GRADIENT_CLIP_MIN_VALUE     8.0f

//! Define if only graylevel information should be used, or the colour channel 
//! with maximum edge should be selected per pixel.
//#define PROCESS_GRAYLEVEL

//! For certain applications, mode merging should move towards the largest scale
//! (e.g. for traffic sign detection), if not defined, normal meanshift is applied.
//#define MERGE_DETECTION_MEAN_SHIFT_MAX_SCALE

//! Define when the gradient information should be shown.
//! NOTE: Be carefull here! It seems that different threads cannot write to
//! the same windows!!!
//#define DEBUG_SHOW_GRADIENTS

//! HOG Features: use the maximum gradient channel.
#define FEAT_HOG_CHANNEL_ALLMAX
//! HOG Features: use only this colour channel 
//! You have to #undef FEAT_HOG_CHANNEL_ALLMAX
//#define FEAT_HOG_CHANNEL_SINGLE_NUMBER 1

//! Print time for scaling, features and detector.
//#define DEBUG_PRINT_TIME

//! Define this when std::cout printing should be used. 
//#define STD_COUT_PRINT

//! Maximum number of mean-shift detection-merging iterations 
//! (to avoid jumping between two or more solutions infinitely).
#define MS_MAX_ITERATIONS   2000

//#############################################################################


#include <vector>
#include <list>
#include <string>

#include <math.h> //sqrt
#define PI 3.141592653589793

// BOOST Serialization.
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>		// for serializing STL vectors.
#include <boost/shared_ptr.hpp> // Shared pointers


///////////////////////////////////////////////////////////////////////////////
// Object detection
///////////////////////////////////////////////////////////////////////////////

class CConfigObjectDetection
{
public:

	//! Show the detection results from the window-level classifier.
	bool m_showBoxesClassifierWindow;

	//! Show the results after the merging step.
	bool m_showBoxesClassifierMerged;

	//! Preprocess the image using sqrt-gamma correction (take sqrt of every
	//! pixel in every colour channel).
	bool m_preprocessRGBSqrt;

	//! Use colour information or just graylevel information.
	bool m_preprocessUseColour;

	//! Maximum scale to process, where 0.0 means no maximum, and 1.0 means 
	//! the scale of the input pixels.
	float m_scaleMax;

	//! Minimum scale to process, where (0.0 means default value of 1.0) 
	//! 1.0 means the scale of the input pixels,
	//! and larger values represent a larger minimum size of objects.
	float m_scaleMin;

	//! Cropping the image horizontally, start pixel value, 0 means no crop, -1 to disable cropping.
	int m_xMin;
	//! Cropping the image horizontally, end of cropped image (new width - m_xMin).
	int m_xMax;

	//! Cropping the image vertically, start pixel value, 0 means no crop, -1 to disable cropping.
	int m_yMin;
	//! Cropping the image vertically, end of cropped image (new height - m_yMin).
	int m_yMax;

	//! Detection results will be stored in a subdirectory of the dataset, 
	//! this parameter defines one level of subdirectory (e.g. "results_param1")
	std::string m_resultImagesDir;

	//! When true, detection metadata is written to disk, both for all 
	//! detection-windows and real detections.
	bool m_writeDiskDetections;
	
	//! For grabbing positive samples from the camera, the box is a multiple of  
	//! the detectorsize (as currently still specified in the hog features),
	//! factor 1.0 means detector resolution.
	float m_factorRectangleCapture;

	//! Where to store the images of the positive training samples.
	std::string m_directoryPosSamples;

	//! Where to store the images of the negative training samples.
	std::string m_directoryNegSamples;

	//! Number of random iterations for feature extraction from negative images.
	int m_randomSamplesPerNegImage;

	//! XviD decodes the image flipped, so when using videos compressed with XviD,
	//! set this variable to true!
	bool m_flipImageInternally;

  //! When generating training samples, also flip the positive sample images
  //! vertically (this means around the vertical axis, so e.g. for 
  //! sideviews of cars, left becomes right, right becomes left).
  bool m_flipPosSampleVer;

  //! When generating training samples, also flip the positive sample images
  //! horizontally (this means around the horizontal axis, so objects
  //! are upsidedown).
  bool m_flipPosSampleHor;

	//! Video source, where should we grab the video from? If empty, get camera.
	std::string m_videoInputFile;

	//! Where should we store the captured video file?
	std::string m_videoOutputFile;

	//! OpenCV Colour space.
	int m_colourSpace;

  //! Name for the object-class that is being detected.
  std::string m_className;

  /** 
   *  The factor for gamma compensation.
   *  y_out = 255.0f * pow(y_in/255.0f, 1/factor).
   *  Value 1.0f does do nothing, value 2.0f has a square-root compression
   *  behaviour. This means that black will be made lighter and light gets 
   *  compressed (default sqrt gamma compression).
   */
  float m_gammaCompensationFactor;
};


///////////////////////////////////////////////////////////////////////////////
// Classifiers.
///////////////////////////////////////////////////////////////////////////////

class CConfigClassifierSVM
{
public:
  //! Lambda for gradient descent.
  float       m_lambda;

  //! Number of iterations over the training set.
  int         m_epochs;

  //! Factor of samples to learn for dataset, set to 1 for all samples,
  //! 2 for half of the samples.
  float       m_factorDataset;

  //! Eta (gain) value for t=0 (default = 1).
  float       m_etaStart;
  
  //! SVM Implementation Library ("SVMLight", "libLinear", ..).
  std::string m_lib;

  float       m_costRatio;

public:
	CConfigClassifierSVM() :
	  m_lib("SVMLight"),
    m_costRatio(1.0f) {};
};


///////////////////////////////////////////////////////////////////////////////
// Feature generation configuration.
///////////////////////////////////////////////////////////////////////////////

class CConfigFeatureHOG
{
public:
	// Mean-shift mode-finding parameters.
	bool			m_useMeanShiftModeFinding;
	bool			m_msSigmaAuto;
	float			m_msSigmaX;
	float			m_msSigmaY;
	float			m_msSigmaScale;
	float			m_msEpsilon;

	// detector parameters.
	int				m_detectorWidth;
	int				m_detectorHeight;
	int				m_detectorContextX;
	int				m_detectorContextY;
	int				m_detectorMarginCellX;
	int				m_detectorMarginCellY;
	bool			m_detectorMarginAuto;
	
	// hog feature parameters.
	signed char		m_cellSizeHor;
	signed char		m_cellSizeVer;
	unsigned int	m_orientationBins;
	bool			    m_useOrientationSign;
	signed char		m_cellsPerBlockHor;
	signed char		m_cellsPerBlockVer;
	
	// classifier, final detector.
	float			m_thresholdWindowClassifier;
	float			m_thresholdFinalClassifier;
	float			m_scaleFactor;

	// normalization.
	int				m_normalization;
	float			m_epsNormalization;

public:
	CConfigFeatureHOG() :
		m_useMeanShiftModeFinding(false),
		m_msSigmaAuto(true),
		m_msSigmaX(8),
		m_msSigmaY(16),
		m_msSigmaScale(1.3f),
		m_msEpsilon(1e-4f), 
		m_detectorWidth(64),
		m_detectorHeight(128),
		m_detectorContextX(8),
		m_detectorContextY(8),
		m_detectorMarginCellX(1),
		m_detectorMarginCellY(1),
		m_detectorMarginAuto(true),
		m_cellSizeHor(8),
		m_cellSizeVer(8),
		m_orientationBins(9), 
		m_useOrientationSign(false), 
		m_cellsPerBlockHor(2),
		m_cellsPerBlockVer(2),
		m_thresholdWindowClassifier(0.0f),
		m_thresholdFinalClassifier(0.0f),
		m_scaleFactor(1.05f),
		m_normalization(0),
		m_epsNormalization(1e-4f) {}
};


class CConfigFeature 
{
public:
	
	CConfigFeatureHOG	hog;

	//! True if normalization should be applied to each extracted feature.
	bool				normalization; 
};




///////////////////////////////////////////////////////////////////////////////
// Serialization configuration.
///////////////////////////////////////////////////////////////////////////////
class CConfigSerialization
{
public:
	bool					binary;
	CConfigSerialization() : binary(false) {}
};



///////////////////////////////////////////////////////////////////////////////
// Classifiers
///////////////////////////////////////////////////////////////////////////////
class CConfigClassifier
{
public:
  CConfigClassifierSVM    	svm;
};



class CConfigCycloMedia 
{
public:
  std::string imageListFileName;
};


///////////////////////////////////////////////////////////////////////////////
// Global configuration.
///////////////////////////////////////////////////////////////////////////////
//! Configuration class.
/*!
 *	Every configuration value is stored in this object.
 *	There are several main variables that each provide configuration data
 *	for its task.
 */
class CConfig
{
public:

	//! Configuration information for the feature generators.
	CConfigFeature			feature;

	//! Configuration information for the classifier algorithms.
	CConfigClassifier		classifier;

	//! Object detection parameters.
	CConfigObjectDetection	objectDetection;

	//! Serialization information (binary or textual).
	CConfigSerialization	serialization;

  //! CycloMedia configuration.
  CConfigCycloMedia cycloMedia;

private:

  //! Simple ini file reader/writer.
  CSimpleIniA m_fileConfig;


public:

  //! Constructor.
  CConfig() {}

	//! Reads a config file and sets the configuration object parameters.
	bool read(char * f_fileName);

  //! Opens a file for reading, but does not read/write yet.
  bool open(const char * f_fileName);

  //! Stores the made changes and saves to the specified file.
  bool save(const char * f_fileName);

  //! Sets a specified value, false if could not be set.
  bool get(std::string f_param, int& f_value);

  //! Destructor.
  ~CConfig() {}


private:
	
	//! Read bool config value from config file.
	void getConfigValueBool(
	  const char  * f_section, 
    const char  * f_key, 
    bool        & f_value,
    const bool    f_valueDefault = false);

	//! Read int config value from config file.
	void getConfigValueInt(
	  const char  * f_section, 
    const char  * f_key, 
    int         & f_value,
    const int     f_valueDefault = 0);

	//! Read unsigned int config value from config file.
	void getConfigValueUInt(
	  const char    * f_section, 
    const char    * f_key, 
    unsigned int  & f_value,
    const unsigned int f_valueDefault = 0);

	//! Read float config value from config file.
	void getConfigValueFloat(
	  const char  * f_section, 
    const char  * f_key, 
    float       & f_value,
    const float   f_valueDefault = 0.0f);

	//! Read unsigned char value from config file.
	void getConfigValueChar(
	  const char  * f_section, 
    const char  * f_key, 
    signed char & f_value,
    const signed char f_valueDefault = '0');

	//! Read string from config file.
	void getConfigValueString(
		const char  * f_section, 
    const char  * f_key, 
    std::string	& f_value,
    const std::string	f_valueDefault = "");

};


extern CConfig globalConfig;
extern int gCounterImage;

#endif //__CONFIG_HPP__
