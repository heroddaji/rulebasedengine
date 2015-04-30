/** \file
 * Background Model GMM Parameters header file.
 *
 * $Id: BackgroundModelGMMParams.hpp.example 1949 2011-01-12 10:37:45Z kris $
 */
#ifndef BACKGROUND_MODEL_GMM_PARAMETERS_HPP
#define BACKGROUND_MODEL_GMM_PARAMETERS_HPP 

/**
 * Uncomment for using color information. Be sure the settings
 *  below correspond to the setting used here (i.e. nbChannels).
 */
#define BACKGROUND_MODEL_GMM_USE_COLOR_INFO

namespace Vi
{
  /**
   * Compile-time settings for the GMM Background Model
   */
  namespace BackgroundModelGMMParams
  {
    /// the number of channels in the processed image (Y = 1, RGB = 3)
    const unsigned int nbChannels = 3;
  }
}

#endif // BACKGROUND_MODEL_GMM_HPP
