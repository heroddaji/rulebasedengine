/** \file
 * Illumination Model Simple Parameters header file.
 *
 * $Id$
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
  namespace IlluminationModelParamsSimple
  {
    // ===========================
    // === ILLUMINATION MODEL  ===
    // ===========================

    /// the number of channels in the processed image (Y = 1, RGB = 3)
    const unsigned int nbChannels = 3;

    /// the number of components for the background in the mixture 
    const unsigned int nbModesBackground = 1;
    
    // the constant to cope with low intensity values in the ratio
    const unsigned int offset = 64;

    /// the threshold for including a pixel on the background
    const float backgroundThreshold = 0.0f;

    /// the update rate for the background model (the higher, the more rapid)
    const float updateRate = 0.0001f;

    /// the maximum allowed value for the variance (255*255)
    const float maxCovariance = 65025.0f;
    
    /// the minimum allowed value for the variance 
    const float minCovariance = 0.001f;
    
    /// the minimum allowed value for the mean 
    const float maxMean = 2.0f;
   
    /// the maximum allowed value for the mean 
    const float minMean = 0.0f;
    
    /// the maximum allowed value for the weight 
    const float maxWeight = 0.99999f;
    
    /// the minimum allowed value for the weight 
    const float minWeight = 0.00001f;
  }
}

#endif // ILLUMINATION_MODEL_PARAMETERS_HPP 
