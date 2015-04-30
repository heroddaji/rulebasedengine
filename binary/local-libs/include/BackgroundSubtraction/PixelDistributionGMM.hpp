/** \file
 * GMM pixel distribution prototype.
 *
 * $Id: PixelDistributionGMM.hpp 2020 2011-01-31 09:00:06Z guillaume $
 */

#ifndef PIXEL_DISTRIBUTION_GMM_HPP
#define PIXEL_DISTRIBUTION_GMM_HPP

#include <stdint.h>

#include "IsotropicGaussian.hpp"

/**
 * Gaussian Mixture Model associated to a single pixel.
 */
class PixelDistributionGMM
{
public:
  
  /**
   * Constructor.
   *
   * \param[in]          fMaxNbComponents The maximum number of components in the mixture.
   * \param[in]                    fAlpha The background update factor alpha.
   * \param[in]  fThresholdModeBackground The threshold (i.e. factor on the variance) for
   *                                        including a pixel on the background.
   * \param[in] fThresholdBackgroundModel The threshold to tell if a mode should be
   *                                        considered as part of the background model.
   * \param[in]      fThresholdModeUpdate The threshold (i.e. factor on the variance) for
   *                                        the mode update.
   * \param[in]                       fCT The prior cT.
   * \param[in]          fInitialVariance The intial value for the variance of the
   *                                        Gaussians in the GMM.xNbComponents 
   */
  PixelDistributionGMM(uint32_t fMaxNbComponents = 5,
                       float fAlpha = 0.0075f,
                       float fThresholdModeBackground = 100.0f,
                       float fThresholdBackgroundModel = 0.9f,
                       float fThresholdModeUpdate = 9.0f,
                       float fCT = 0.05f,
                       float fInitialVariance = 11.0f);
  
  /**
   * Destructor. 
   */
  ~PixelDistributionGMM();

  /**
   * Main processing function:
   *
   * - perform background / foreground segementation,
   * - perfrom background model update.
   *
   * \note This version checks whether the pixel is part of an object.
   *
   * Here, some modes are not visited if a distance "close enough" has been
   *  found for mode updating:
   *
   * - Pixel is close enough (according to criterion 1) to mode with high
   *    weight -> it is a background pixel.
   * - Pixel is close enough (according to criterion 2) to the same
   *    mode -> the mode is updated with pixel's values.
   *
   * \note That this is done even in there is a mode (with smaller weight)
   *  that is closer to the pixel.
   *
   * - Hence, the "background" distribution is updated, but could be misleading.
   * - (i.e. There could be a better background distribution, modelled by that
   *   are modes not "considered" as they should be.)
   *
   * \param[in]      fPixelValues The pixel values (RGB or Y)
   * \param[in] fPixelsIsInObject Is the pixel an object pixel?
   *
   * \return 1 if the pixel is in the background, otherwise 0.
   */
  unsigned int process(float** const fPixelValues, bool fPixelsIsInObject);

  /// Array of Gaussians that makes the mixture.
  IsotropicGaussian **mGaussians;
  
  /// The number of currently used modes/gaussians/components.
  unsigned int mNbUsedModes;

  /**
   * Returns the probability that the provided pixel belongs to the background
   *
   * \param[in] fPixelValues The pixel value(s) in each channel 
   */
  float computeProbability(float* const fPixelValues);

  void modifyDistributionForFlushing(float** const fPixelValues);

private:

  /**
   * Normalize the weights of the gaussians in the mixtures such that they sum
   *  to one.
   */
  void normalizeWeights();

  /**
   * Sort the modes in descending orders according to their weights (i.e.
   *  highest weight goes first).
   */
  void sortModes();

  /// Used for debug, print the distribution for each gaussian in the mixture.
  void printDistribution();
  
  /// The distances to the mean for each channel (i.e. each dimension).
  float *mDistancesToMean;

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

#endif // PIXEL_DISTRIBUTION_GMM_HPP
