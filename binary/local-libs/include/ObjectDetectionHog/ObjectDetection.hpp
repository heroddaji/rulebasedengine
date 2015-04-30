#ifndef __OBJECT_DETECTION_HPP__
#define __OBJECT_DETECTION_HPP__

#include "DetectionWindow.hpp"

#include <ViNotion/Image.hpp>
#include <ViNotion/Compat.hpp>
#include <ViNotion/Timer.hpp>

#include <list>
#include <string>

// Forward declarations.
class CFeatureGenHOG;
class IClassifierSVM;
class CTrainingSet;

// How many training samples should be kept in memory between different 
// classifier training iterations.
#define KEEP_NEG_SAMPLES 0 // NOTE: Currently not used, since classifier removes all samples from trainingset.
#define KEEP_POS_SAMPLES 0 // NOTE: Currently not used, since classifier removes all samples from trainingset. Positive samples are re-generated for each training iteration.

//! Object Detection functionality class.
class EXPORT_TYPE CObjectDetection
{
private:

	//! Pointer to object detector for settings threshold.
	CFeatureGenHOG * m_hog;

  //! Image on which the detection or extraction of new samples is applied.
  Vi::Image<> * m_image;

  //! Classifier.
  IClassifierSVM * m_classifier;

  //! True if classifier has been learned with data.
  bool m_bClassifierLearned;

  //! True if window-level detections should be merged.
  bool m_bMergeDetections;

  //! Maximum scale for detector on current image.
  float m_maxScale;

  //! Detections by the HOG detector in center format.
  std::list< CDetectionWindow > m_detections;
  std::list< CDetectionWindow > m_detectionsMeanShift;

  //! Detections by the HOG detector in left-top format.
  std::list< CDetectionWindow > m_detectionsLT;
  std::list< CDetectionWindow > m_detectionsLTMeanShift;

public:

  //! Training samples.
  CTrainingSet * m_trainSet;


private:

  //! Calculates the overlap between two detections, one CDetection and 
  //! one CDetectionWindow. Note that the returned overlap is the minimum of
  //! the intersection of the two, normalized to the size of each window.
  float getMinimalOverlap(
    CDetection        * f_a,
    CDetectionWindow  * f_b);

  //! Convert detections from internal 'center' format (value 0) to 
  //! 'external', 'left-top' format (value 1).
  bool convertDetections(
    std::list< CDetectionWindow > * f_detC,
    std::list< CDetectionWindow > * f_detLT);


public:

	//! Constructor.
  CObjectDetection(std::string f_configFileName = "");

  //! Adds a negative feature vector to the training set.
  void addFeatureVectorNegative( CFeatureResponseList & f_features);
  
  //! Adds a positive feature vector to the training set.
  void addFeatureVectorPositive( CFeatureResponseList & f_features);

  //! Returns the detector specifications.
  bool getDetectorSpecs(
    int & f_width, 
    int & f_height, 
    int & f_offset,
    float & m_maxScale);

  //! Sets the image that the detector will work on, typically, trainNewSample()
  //! or detectObjects() is called next.
  //! The image is assumed to stay alive until the next setImage() call.
  bool setImage(const Vi::Image<> * f_image);

  //! Gets the number of detections from the previous call to detectObjects().
  bool getNrDetections(int & f_nrDetections);

  //! Get the detections as copies on the list.
  /*!
   *  @param[in]  f_format species the interpretation of the center coordinate.
   *              value 0 means 'center' format, where x,y represents the 
   *              center, and value 1 means left-top format, where x,y 
   *              represents the left-top.
   */
  bool getDetections(
    std::list< CDetectionWindow > & f_detections,
    int                             f_format = 0);

  //! Set the detection threshold for the HOG detector.
  //! integer value is converted to float by: ((x - 50) / 100).
  void setThresholdWindowClassifier(int f_threshold);

  //! Specify if window-level detections should be merged by f_shouldMerge.
  /*!
   *  If the value of f_shouldMerge is changed to true, merging is done 
   *  by calling meanShiftModeFinding().
   */
  bool setMergeDetections(bool f_shouldMerge);

  //! Set the minimum scale to process, including this scale.
  //! Value 0.6f means that all scales starting from and including 0.6 will 
  //! be processed.
  bool setScaleMin(float f_scaleMin);
  
  //! Set the maximum scale to process, including this scale.
  //! Value 2.0f means that all scales up to and including 2.0 will 
  //! be processed.
  bool setScaleMax(float f_scaleMax);

  //! Load a pre-trained classifier from file.
  bool loadClassifier(std::string f_fileName);

  //! Store a trained classifier to file.
  bool storeClassifier(std::string f_fileName);

  //! Get timer for HOG detector.
  std::vector<Vi::Timer> * getTimer();

  //! Train classifier using supplied training samples.
  bool trainClassifier(
    std::list<CDetection> f_objects);

  //! Compares all previous detections (before merging) with the given
  //! training samples. If detections do not overlap significantly,
  //! the detection is added to the negative training set.
  //! The number of positive objects are counted and the number of objects 
  //! that have been detected are calculated.
  bool generateFalseDetections(
    std::list<CDetection>     f_trainingSamples,
    int                     & f_nrFalseDetections,
    int                     & f_nrObjects,
    int                     & f_nrObjectsDetected);

  //! Removes all training samples from the m_trainSet.
  bool clearTrainSamples();

  //! Objects are detected on the image set by setImage(), and detected objects
  //! are stored in the list f_objects. 
  /*!
   *  Retrieve detected objects using getDetections().
   */
  bool detectObjects();

	//! Destructor.
	~CObjectDetection();

}; // class CObjectDetection.

#endif // __OBJECT_DETECTION_HPP__
