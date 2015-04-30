/** \file
 * Illumination Model Parameters header file.
 *
 * $Id: IlluminationModelParams.hpp.example 1857 2010-11-17 10:56:57Z kris $
 */
#ifndef ILLUMINATION_MODEL_PARAMETERS_HPP
#define ILLUMINATION_MODEL_PARAMETERS_HPP

/**
 * Uncomment for using color information. Be sure the settings
 *  below correspond to the setting used here (i.e. nbChannels).
 */
#define ILLUMINATION_MODEL_USE_COLOR_INFO

namespace Vi
{
  /**
   * Compile-time settings for the Illumination Background Model
   */
  namespace IlluminationModelParams
  {
    // ===========================
    // === ILLUMINATION MODEL  ===
    // ===========================

    /// the number of channels in the processed image (Y = 1, RGB = 3)
    const unsigned int nbChannels = 3;

    /**
     * the number of components for the background in the mixture
     *
     * \note Also change the total number of modes if this one changes.
     */
    const unsigned int nbModesBackground = 1;
    
    /**
     * the number of components for the foreground in the mixture 
     *
     * \note Also change the total number of modes if this one changes.
     */
    const unsigned int nbModesForeground = 1;

    /// the total number of modes (i.e. background + foreground + 1)
    const unsigned int nbModesTotal = 3;

    // the constant to cope with low intensity values in the ratio
    const unsigned int offset = 64;

    /// the threshold for including a pixel on the background
    const float backgroundThreshold = 0.001f;

    /// the update rate for the background model (the higher, the more rapid)
    const float updateRate = 0.0001f;

    /// the maximum allowed value for the variance (255*255)
    const float maxCovariance = 65025.0f;
    
    /// the minimum allowed value for the variance 
    const float minCovariance = 0.00001f;
    
    /// the maximum allowed value for the variance 
    const float maxWeight = 0.99999f;
    
    /// the minimum allowed value for the variance 
    const float minWeight = 0.00001f;
    
    /// the maximum allowed value for the mean 
    const float minMean = 0.0f;
    
    /// the minimum allowed value for the mean 
    const float maxMean = 255.0f;
  }
}

#endif // ILLUMINATION_MODEL_PARAMETERS_HPP 
