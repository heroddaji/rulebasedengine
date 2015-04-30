/** \file
 * Object detector HOG prototype.
 *
 * $Id$
 */

#ifndef OBJECT_DETECTOR_HOG_HPP
#define OBJECT_DETECTOR_HOG_HPP

#include <Settings/Settings.hpp>
#include <Features/FeatureSpaceHOG.hpp>

#include "ObjectDetector.hpp"
#include "ClassifierHOG.hpp"

/// Uncomment to enable SSE calculations (will be undef'ed for ICC).
#define OBJECT_DETECTOR_HOG_SSE

// For the Intel compiler we don't want SSE
#ifdef __INTEL_COMPILER
#undef OBJECT_DETECTOR_HOG_SSE
#endif

// Detect 64-bit OS for double SSE loop unrolling (otherwise double)
#ifdef __x86_64
#define OBJECT_DETECTOR_HOG_SSE_QUAD
#endif

namespace Vi
{
  /**
   * Detection type, just a wrapper around a bounding box.
   */
  class DetectionHOG
  {
  public:

    Vi::Bbox<int> mBbox;  ///< The detection bounding box.
    Vi::Frac<int> mScale; ///< The detection scale.
    float mScore;         ///< The detection score [apples/pear].
  };

  /**
   * Object detector HOG prototype.
   *
   * \todo Non-SSE calculations don't need the alignment stuff and can there-
   *        fore skip all the copies to aligned memory (still happen). This
   *        would also need some special way to get the pointers to feature
   *        data in a linear way (right now, it is a `block' inside an area,
   *        which is non-linear).
   * \todo (Related to the previous point) Constrain the orientation bin
   *        setting to powers of two, to be able to load the object features
   *        in aligned memory directly, this saves us the copy to the SSE
   *        vector in the applyDetectionWindow() function.
   */
  class EXPORT_TYPE ObjectDetectorHOG
  {
  public:

    /**
     * Constructor.
     *
     * \param[in] fFeatureSpace Pointer to the HOG feature space.
     * \param[in]   fClassifier Pointer to the HOG classifier.
     */
    ObjectDetectorHOG(Vi::FeatureSpaceHOG *fFeatureSpace,
                      Vi::ClassifierHOG *fClassifier = NULL);

    /**
     * Destructor.
     */
    ~ObjectDetectorHOG();

    void detect(const Vi::Image<> &fFrame,
                const Vi::Image<> &fObjectMask,
                std::vector<Vi::Bbox<int> > &fDetections);

    /// Overloaded function.
    void detect(const Vi::Image<> &fFrame,
                const Vi::Image<> &fObjectMask,
                std::vector<Vi::DetectionHOG> &fDetections);

    /**
     * Set classifier.
     *
     * \param[in] fClassifier Pointer to the HOG classifier.
     */
    void setClassifier(Vi::ClassifierHOG *fClassifier);

    /**
     * Set run-time configuration.
     *
     * \param[in] fConfig The configuration settings.
     */
    void configure(const Vi::Settings &fConfig);

    /**
     * Enable mean shift merging.
     *
     * \param[in] fState True to enable merging, false for disable.
     */
    void setMergeDetections(bool fState);

  private:

    /**
     * Initialize class state.
     */
    void initialize();

    /**
     * Check compatibility between the classifier and the feature space.
     *
     * Throws an error on classifier/feature space settings inequality.
     */
    void checkCompatibility();

    /**
     * Allocate aligned vectors for SSE and pre-load the classifier data into
     *  an array that is properly aligned for use in SSE calculations.
     */
    void initializeVectorsSSE();

    /**
     * Apply the detection window for a single scale/window.
     *
     * \param[in] fFeatureVec The feature vector for the scale to search.
     * \param[in]    fOffsetX The offset in the X-direction [cells].
     * \param[in]    fOffsetY The offset in the Y-direction [cells].
     *
     * \return The result of the inproduct between detection window/classifier.
     */
    float applyDetectionWindow(const Vi::FeatureVectorHOG &fFeatureVec,
                               unsigned int fOffsetX,
                               unsigned int fOffsetY);

    /**
     * Mean shift mode finder for merging detections.
     *
     * \todo This function needs some optimization TLC and smart thinking.
     *
     * \param[in,out] fDetections The vector of detections.
     */
    void findMeanShiftModes(std::vector<DetectionHOG> &fDetections);

    Vi::FeatureSpaceHOG *mFeatureSpace; ///< Pointer to the feature space class.
    Vi::ClassifierHOG *mClassifier;     ///< Classifier.

    float *mFeatureDataSSE;    ///< Aligned feature data for SSE.
    float *mClassifierDataSSE; ///< Aligned classifier data for SSE.

    unsigned int mStrideLenCell; ///< Stride length in floats per cell.
    unsigned int mStrideLenLine; ///< Stride length in floats per detector cell line.

    float mDetectionThreshold;        ///< The detection threshold.
    bool mEnableMeanShiftModeFinding; ///< Mean shift mode finding for merging detections.
    bool mMeanShiftSigmaAuto;         ///< Automatic mean shift variance calculation.
    float mMeanShiftSigmaX;           ///< Mean shift X-coordinate variance.
    float mMeanShiftSigmaY;           ///< Mean shift Y-coordinate variance.
    float mMeanShiftSigmaScale;       ///< Mean shift scale variance.
    float mMeanShiftEpsilon;          ///< Mean shift epsilon threshold.
  };
}

#endif // OBJECT_DETECTOR_HOG_HPP
