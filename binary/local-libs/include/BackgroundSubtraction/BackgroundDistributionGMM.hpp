/** \file
 * GMM background prototype.
 *
 * $Id: BackgroundDistributionGMM.hpp 1976 2011-01-17 11:00:30Z kris $
 */

#ifndef BACKGROUND_DISTRIBUTION_GMM_HPP
#define BACKGROUND_DISTRIBUTION_GMM_HPP

#include <ViNotion/Image.hpp>

#include <Settings/Settings.hpp>

#include "PixelDistributionGMM.hpp"

/**
 * Gaussian Mixture Model for the background reference image.
 */
class BackgroundDistributionGMM
{
public:

  /**
   * Constructor.
   *
   * \param[in] fSize The number of pixels in the image.
   */
  BackgroundDistributionGMM(unsigned int fSize);
  
  /**
   * Destructor.
   */
  ~BackgroundDistributionGMM();

  /**
   * Update.
   */
  void update();

  /**
   * Process - Object level feedback.
   *
   * \note Loop on the pixel and call the processing function for each pixel distribution.
   * \note Update is done taking segmentation results into account.
   * \note Build the background/foreground segmentation mask.
   *
   * \param[in]       fFrameRGB The current frame.
   * \param[in]  fObjectMaskYUV The object segmentation mask (from previous frame).
   * \param[out]       fMaskYUV The background/foreground segmentation mask in YUV.
   */
  void process(const Vi::Image<> &fFrameRGB,
               const Vi::Image<> &fObjectMaskYUV,
               Vi::Image<> &fMaskYUV);
 
  /**
   * Run-time configuration function.
   */
  void configure(const Vi::Settings &fConfig);

  /**
   * The probability distribution for each pixels.
   *
   * \note Has to be public to build the background image.
   */
  PixelDistributionGMM** mGMMs;
 
private:

  /**
   * Allocate pixel distribution memory.
   */
  void allocatePixelDistribution();

  /**
   * Deallocate pixel distribution memory.
   */
  void deallocatePixelDistribution();

  /// The size of the image = the number of GMMs.
  unsigned int mSize;

  /// The current pixel color values (for each channel).
  float* mPixelValues;

  // --------------------------------------------------------------------------
  // \{ \name Class run-time parameters

  /// The maximum number of components in the mixture.
  uint32_t mMaxNbComponents;

  /// The background update factor alpha.
  float mAlpha;

  /// The threshold (i.e. factor on the variance) for including a pixel on the background.
  float mThresholdModeBackground;
  
  /// The threshold to tell if a mode should be considered as part of the background model.
  float mThresholdBackgroundModel;
  
  /// The threshold (i.e. factor on the variance) for the mode update.
  float mThresholdModeUpdate;

  /// The prior cT.
  float mCT;

  /// The intial value for the variance of the gaussians in the GMM.
  float mInitialVariance;
};

#endif // BACKGROUND_DISTRIBUTION_GMM_HPP
