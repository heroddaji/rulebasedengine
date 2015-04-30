#ifndef __FEATUREDESCRIPTORHOG_H__
#define __FEATUREDESCRIPTORHOG_H__

#include "Feature.hpp"
#include "DetectionWindow.hpp"

#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <ViNotion/Image.hpp>


// Forward declarations
class CImageSource;
class IClassifierSVM;


struct gPascal {
  std::string imgnum;
};


//! The interface class for the feature generator class.
class CFeatureGenHOG
{
public:

  //! Pixel size of detector.
  int m_detectorWidth;
	
  //! Pixel size of detector.
  int m_detectorHeight;

  //! Pixel size of context taken into account in the detector, to be removed 
  //! for writing of detected bounding boxes.
  /*!
   *	This denotes both the number of pixels on the left and the right side.
   *	So in the case of the default person-detector from Dalal, 8 pixels on 
   *	left and right should be used.
   */
  int m_detectorContextX;

  //! Pixel size of context taken into account in the detector, to be removed 
  //! for writing of detected bounding boxes.
  /*!
   *	This denotes both the number of pixels on the top and the bottom side.
   *	So in the case of the default person-detector from Dalal, 8 pixels on 
   *	top and bottom should be used.
   */
  int m_detectorContextY;

  //! Ssize (pixels) of a HOG bin (SIFT) horizontally.
  signed char m_cellSizeHor;

  //! Size (pixels) of a HOG bin (SIFT) vertically.
  signed char m_cellSizeVer;

  //! Number of feature dimensions for the detector window.
  int m_detectorDimensions;

  //! Threshold for the window-level classisifer (before mean-shift mode-finding).
  float m_thresholdWindowClassifier;

  //! Threshold for the final classisifer (after mean-shift mode-finding).
  float m_thresholdFinalClassifier;

  //! Do we apply mean-shift?
  bool m_useMeanShiftModeFinding;

  //! Timer objects.
  /*!
   *  Contains cumulative time for:
   *
   *  [0] Scaling
   *  [1] Calculate features
   *  [2] Sliding detection window
   */
  std::vector< Vi::Timer > timert;


private:

  //! Spatial cell index relative to current cell for all m_cellSizeHor.
  //! Values contain -1 for x values [0,m_cellSizeHor/2] and +1 otherwise.
  signed char * m_lutSpatialCellIndexRelative;

  //! Interpolation weights LUT.
  /*!
    m_lutSpatialInterpolation[y][x]   = interpolFactorHorSelf  * interpolFactorVerSelf;
    m_lutSpatialInterpolation[y][x+1] = interpolFactorHorSelf  * interpolFactorVerOther;
    m_lutSpatialInterpolation[y][x+2] = interpolFactorHorOther * interpolFactorVerSelf;
    m_lutSpatialInterpolation[y][x+3] = interpolFactorHorOther * interpolFactorVerOther;
  */
  float *** m_lutSpatialInterpolation;

  //! Orientation bin index for each gradient angle, for the other bin.
  unsigned char * m_lutOriBinIndexOther;

  //! Orientation interpolation weights LUT.
  float * m_lutOriInterpolation;

  //! SVM weights.
  float * m_svmWeights;

  //! SVM Bias value.
  float m_svmBias;

  //! SSE optimized array for feature vector (aligned at 16 byte boundaries).
  float * dataSSEFeatureVector;

  //! SSE optimized array for SVM weights vector 
  //! (aligned at 16 byte boundaries).
  float * dataSSESVMWeights;

  //! Number of orientation bins.
  int m_orientationBins;

  //! Use sign of the orientation (yes/no).
  //! Remember to adjust m_orientationbins accordingly.
  bool m_useOrientationSign;

  //! Cells per block (horizontally).
  //! Note that this value can only be '1' or '2'!
  signed char m_cellsPerBlockHor;

  //! Cells per block (vertically).
  //! Note that this value can only be '1' or '2'!
  signed char m_cellsPerBlockVer;

  //! The number of cells per image horizontally.
  //! Note, these values are automatically generated.
  int m_cellsPerImageHor;

  //! The number of cells per image horizontally.
  //! Note, these values are automatically generated.
  int m_cellsPerImageVer; 

  //! The number of cells per image allocated 
  //! (might be more than the current image).
  int m_cellsPerImageAllocatedHor;

  //! The number of cells per image allocated 
  //! (might be more than the current image).
  int m_cellsPerImageAllocatedVer;
	
  //! Number of cells margin taken around the input image while applying object 
  //! detection in x direction. 
  //! Note that at each image scale, this size in pixels increases.
  int m_detectorMarginCellX;

  //! Number of cells margin taken around the input image while applying object 
  //! detection in y direction. 
  //! Note that at each image scale, this size in pixels increases.
  int m_detectorMarginCellY;

  //! Scale factor in the image scale-space pyramid (default 1.2).
  float m_scaleFactor;

  //! Feature normalization applied.
  enum FeatureNormalization {NORM_L1, NORM_L1SQRT, NORM_L2, NORM_L2HYS};
  enum FeatureNormalization m_normalization;

  //! Epsilon constant for HOG normalization (small constant value, added 
  //! during normalization).
  float m_epsNormalization;


  ///////////////////////////////////////////////////////////////////////////
  // Mean-Shift Mode Finding Parameters.
  ///////////////////////////////////////////////////////////////////////////
	
  //! Mean-shift mode finding sigma for x (in pixels w.r.t. scale 1.0).
  float m_msSigmaX;
	
  //! Mean-shift mode finding sigma for y (in pixels w.r.t. scale 1.0).
  float m_msSigmaY;
	
  //! Mean-shift mode finding sigma for scale 
  //! (in scale-factor w.r.t. scale 1.0).
  float m_msSigmaScale;

  //! Mean-shift minimum distance between iterations before mode is found.
  float m_msEpsilon;


  ///////////////////////////////////////////////////////////////////////////
  // Threadiung related variables.
  ///////////////////////////////////////////////////////////////////////////
	
  //! Number of threads.
  int m_nrThreads;

  //! List with scales.
  std::list< float >	m_scaleList;
  //! Mutex for scale list m_scaleList. 
  boost::mutex		m_mutexScaleList;

  //! Condition for triggering that scales are availble.
  boost::condition	m_condWorkScalesAvailable;
  //! Boolean that should be set to true when scales are available 
  //! for processing.
  bool				m_boolWorkScalesAvailable;
  //! Mutex for checking the boolean m_boolWorkScalesAvailable.
  boost::mutex		m_mutexWorkScalesAvailable;
	
  //! Integer for counting how many scales have been processed.
  int					m_nrScalesProcessed;
  //! Mutex for m_mutexWorkScalesAvailable.
  boost::mutex		m_mutexNrScalesProcessed;
  //! Condition for triggering that scales are been processed.
  boost::condition	m_condWorkScalesDone;
  //! Mutex for m_condWorkScalesDone (not practically used).
  boost::mutex		m_mutexWorkScalesDone;
	
  //! Detection windows for an image.
  std::list< CDetectionWindow > m_detectionList;
  //! Mutex for detection windows list m_detections. 
  boost::mutex		m_mutexDetectionList;

  // Threads.
  boost::thread_group	m_threads;
  //! Boolean for stopping the threads, after setting to true, 
  //! m_condWorkScalesAvailable should be notified one more time.
  bool				m_boolStopProcessingThreads;
	
  //! List of features for hard examples.
  std::list< CFeatureResponseList > m_detectedHardExamples;
  //! Mutex for m_detectedHardExamples. 
  boost::mutex		m_mutexDetectedHardExamples;

  //! List of detection windows of all windows processed.
  std::list< CDetectionWindow > m_detectedWindowsAll;
  //! Mutex for m_detectedWindowsAll. 
  boost::mutex		m_mutexDetectedWindowsAll;

  //! List of detection windows of detected objects.
  std::list< CDetectionWindow > m_detectedWindowsObjects;
  //! Mutex for m_detectedWindowsObjects. 
  boost::mutex		m_mutexDetectedWindowsObjects;


  ///////////////////////////////////////////////////////////////////////////
  // Generated features.
  ///////////////////////////////////////////////////////////////////////////

  //! HOG Features.
  /*!
    Final histogram features.
    m_features[y][x][hist_dim] contains the float* object for the 
    cell at x,y and with normalization index hist_dim.
  */
  float * m_features;

  //! Normalized HOG features.
  //! \sa m_features.
  float * m_featuresNorm;


  ///////////////////////////////////////////////////////////////////////////
  // Detection (and training) related parameters.
  ///////////////////////////////////////////////////////////////////////////

  //! Classifier for detection.
  IClassifierSVM * m_classifier;

  //! Image pointer (for multiple threads).
  Vi::Image<>  * m_image;

  //! Offset in x-direction (for multiple threads).
  int			m_offsetX;
  //! Offset in y-direction (for multiple threads).
  int			m_offsetY;
  //! Should we store the hard examples? (for multiple threads).
  bool		m_storeHardExamples;

  //! Extra detector pointers (per-thread), nasty but simple for a quick 
  //! implementation fix.
  std::list< CFeatureGenHOG * > m_threadsDetectors;


public:

  //! Constructor.
  CFeatureGenHOG();

  //! Sets the threshold of the window-level classifiers.
  //! Note that an int is passed, which is first transfered to the float 
  //! threshold by dividing by 100 (so value '50' means '0.5f').
  bool setThresholdWindowClassifiers(int f_threshold);

  //! Sets the threshold of the window-level classifiers.
  //! Note that an int is passed, which is first transfered to the float 
  //! threshold by dividing by 100 (so value '50' means '0.5f').
  bool setThresholdFinalClassifiers(int f_threshold);
	
  //! Process an image and extract the features.
  bool generateFeatures(
    Vi::Image<>	  * f_image,
    int				f_offsetX = 0, 
    int				f_offsetY = 0);

  //! Applies feature generation and sliding window search at multiple scales.
  /*!
   *	@param[in]	f_offsetX offset in pixels, left and right at each scale-level.
   *	@param[in]	f_offsetY offset in pixels, top and bottom at each scale-level.
   *	@param[in]	f_scaleMax defines the maximum scale to search for (practical 
   *              for very large images).
   */
  bool detectObjects(
    Vi::Image<>							  * f_image,
    IClassifierSVM						  * f_classifier, 
    std::list< CFeatureResponseList >	  * f_hardExamples				= NULL,
    std::list< CDetectionWindow >		  * f_listDetectionPositions	= NULL,
    std::list< CDetectionWindow >		  * f_detections				= NULL,
    int										f_offsetX					= 0, 
    int										f_offsetY					= 0);

  //! Visualizes the SVM weights.
  bool showSVMWeights(
    float				f_factorScalingViz,
    int					f_showTime,
    IClassifierSVM	  * f_svm,
    const std::string		  & f_outputFilePos = std::string(""),
    const std::string		  & f_outputFileNeg = std::string(""));
		
  //! Show the histograms for the SVM weights.
  bool showCellHistogramsSVMWeights(
    IClassifierSVM * f_svm,
    float			f_factorScalingViz	= 1.0f,
    int				f_showTime			= 0,
    const std::string   & f_outputFile		= std::string(""));

  //! Visualize the histograms as made by generateFeatures().
  bool showCellHistograms(
    float			f_factorScalingViz	= 1.0f,
    int				f_showTime			= 0,
    const std::string   & f_outputFile		= std::string(""));

  //! Get the global feature vector of the overall object description.
  bool getFeatureVectorForWindow(
    int f_x1,
    int f_y1,
    int f_x2, 
    int f_y2,
    float * f_featureVector);

  //! Apply meanshift mode finding to group similar detections.
  bool meanShiftModeFinding(
    std::list< CDetectionWindow > * f_detectionsInput, 
    std::list< CDetectionWindow > * f_detectionsResult);

  //! Stores the HOG data in a .TXT file.
  bool saveHOGStoFile(
    std::string & f_outputFile);

  //! Create the threads and start them.
  bool createThreads();

  //! Stop the threads.
  bool stopThreads();

  //! Destructor.
  ~CFeatureGenHOG();


private:

  //! Create LUT for spatial interpolation weights.
  bool createLUTSpatialInterpolation();

  //! Delete the LUT for spatial interpolation weights.
  bool deleteLUTSpatialInterpolation();

  //! Create LUT for orientation interpolation weights.
  bool createLUTOriInterpolation();

  //! Delete the LUT for orientation interpolation weights.
  bool deleteLUTOriInterpolation();

  //! Allocates memory for the histograms of the image.
  bool allocateHistograms();

  //! Apply detection window using sliding approach and classify each window.
  /*! 
    If f_hardExamples is given, the feature vectors of detections are stored 
    in the vector to be used for retraining lateron.
  */
  bool applyDetectionWindow(
    float									                f_scale,
    unsigned int                          f_windowCounter,
    std::list< CFeatureResponseList >	  * f_hardExamples				    = NULL,
    std::list< CDetectionWindow >		    * f_listDetectionPositions  = NULL);

  //! Sets the classifier and the pointer to the linear SVM weights.
  bool setClassifier(
    IClassifierSVM * f_classifier);

  //! Inline code for SVM testing, much faster than actually calling the 
  //! SVM object's test() function
  inline float classifyFeatureVector(
    float * f_fv);

  //! Inline code for SVM testing using SSE, much faster than actually 
  //! calling the SVM object's test() function
  inline float classifyFeatureVectorSSE(
    float * f_fv);

  //! Generates a feature description from the IPL image.
  /*!
    Processing is started at an offset. When an offset is used, the 
    resulting cells are to be used normally, as if no offset has been used.
  */
  bool calculateFeatures(
    Vi::Image<>	  * f_image,
    int				f_offsetX = 0, 
    int				f_offsetY = 0);

  //! Applies feature generation and sliding window search at multiple scales.
  /*!
   *	@param[in]	f_scale defines the scale at which processing is applied.
   */
  bool detectObjectsSingleScale(
    Vi::Image<>  * f_image,
    float		f_scale);

  //! Normalizes all the cells in the image w.r.t. the block each cell 
  //! is part of.
  bool normalizeCells();

  //! Get memory index for certain position in the image.
  inline int getMemoryIndex(
    int cellIndexVer, 
    int cellIndexHor)
  {
    return (((cellIndexVer * m_cellsPerImageHor) + cellIndexHor) * m_orientationBins);
  }

  //! Sums the histogram values (squared for L2 norms, linear for L1 norms) 
  //! over all bins and returns this.
  inline float getHistSum(int f_index);
  inline float getHistSumNorm(int f_index);

  //! Calculates the normalization factor, given summed-value.
  inline float getNormalizationFactor(
    float f_histSum);

  //! Normalize a cell for 2nd pass normalization.
  inline void normalizeCellBlockPass2(
    int f_y,
    int f_x,
    int f_blockIndex,
    float f_normalization);

  //! Normalize a cell for 2nd pass normalization.
  inline void normalizeCellBlockPass1(
    int f_y,
    int f_x, 
    int f_blockIndex,
    float f_normalization);

  //! Get memory index for certain position in the image.
  inline int getMemoryIndexNorm(
    int f_cellIndexVer, 
    int f_cellIndexHor,
    int f_block)
  {
    int blocks = m_cellsPerBlockHor * m_cellsPerBlockVer;
    return 
      (((((f_cellIndexVer * m_cellsPerImageHor) + f_cellIndexHor) * blocks) 
        + f_block) * m_orientationBins);
  }
	
  //! Calculates the offsets for the border of the image.
  bool getImageOffsets(
    const int f_offsetX, 
    const int f_offsetY, 
    int &f_offsetSkipX, 
    int &f_offsetSkipY, 
    int &f_offsetBorderX, 
    int &f_offsetBorderY);


  ///////////////////////////////////////////////////////////////////////////
  // Threading functions.
  ///////////////////////////////////////////////////////////////////////////
	
  //! Get the processing scale and image to process (thread-safe).
  bool getProcessingScale(
    float		& f_scale);

  //! Store the detection results for this thread into the total set of 
  //! detections (thread-safe).
  bool storeDetectionResults(
    std::list< CDetectionWindow > * f_detections);

  //! Threading function.
  void scaleThread(
    CFeatureGenHOG * f_detector);

  //! Scale image using either libViNotion scaling or OpenCV scaler internally.
  /*!
   *  Note that the size() of the output image should already have been set,
   *  otherwise the scaler does not know where to scale to.
   *  Supported formats are only planar formats!
   */
  void scaleImage(
    Vi::Image<> * input, Vi::Image<> * output);

}; // class CFeatureGenHOG.

#endif //__FEATUREDESCRIPTORHOG_H__
