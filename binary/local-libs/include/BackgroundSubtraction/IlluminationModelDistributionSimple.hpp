/** \file
 * Illumination Model Distribution prototype.
 *
 *  \note This model is based on ideas found in the following paper:
 *        "Making Background Subtraction Robust To Sudden Illumination Changes",
 *        J. Pilet, C. Strecha and P. Fua, ECCV 2008.
 *
 * $Id$
 */

#ifndef ILLUMINATION_MODEL_DISTRIBUTION_SIMPLE_HPP
#define ILLUMINATION_MODEL_DISTRIBUTION_SIMPLE_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Image.hpp>

namespace Vi
{
  /**
   * Illumination Model Distribution
   */
  class EXPORT_TYPE IlluminationModelDistributionSimple
  {
  public:

    /**
     * Constructor.
     */
    IlluminationModelDistributionSimple(unsigned int fDimension);
    
    /**
     * Destructor.
     */
    ~IlluminationModelDistributionSimple();

    /**
     * Initialization of the distribution.
     *
     * \note  Here the distribution consists of a mixture of 3 components: 1 gaussian for the background,
     *        1 gaussian for the foreground and 1 uniform for the foreground. The background gaussian is 
     *        is initialized with mean value(s) 1 (ratio bewteen current pixel value and background model),
     *        and an isotropic identity covariance matrix. The foreground gaussian is initialized with the
     *        the mean and the variance of the image corresponding to the first frame. The weights are 
     *        uniformely initialized.
     */
    void init();


    /**
     * Compute the (partial) belief of a pixel (E-step).
     *
     * \note  This function will compute the (partial probability) for a given pixel in the image.
     *        The pixel belief for each mode of the mixture are computed, this is actually the
     *        implemenation of equation (6) and (7) of the aforementioned paper. Note however that
     *        we don't consider the previously learned background and foreground distribution (the "h").
     *        We noticed that adding such a probability is not necessary to achieve satisfactory results.
     *        Hence, the algorithm does not require an "offline" step before being applied. 
     *
     * \param[in]       fPixelRatios The ratio(s) between the pixel value and the background model.
     * \param[in, out] fPixelBeliefs The partial belief computed in the function.
     */
    void computePixelBeliefs(float* const fPixelRatios, float *fPixelBeliefs);
   
    /**
     * Update the accumulators (to be used in the M-step).
     *
     * \note  This function update the sum for the M-step. It corresponds to equations (8), (9) and (10) 
     * 
     * \param[in]  fPixelRatios The ratio(s) between the pixel value and the background model.
     * \param[in] fPixelBeliefs The partial belief computed in the function.
     */
    void updateAccumulators(float* const fPixelRatios, float* const fPixelBeliefs); 
    
    /**
     * Update the distribution (M-step).
     *
     * \note Once all the pixels in the image have been processed, this function update the distribution.
     */ 
    void updateDistribution();

    /**
     * Print the distribution (debug purpose).
     */
    void printDistribution();
    
    /**
     * Print the accumulators (debug purpose).
     */
    void printAccumulators();

    unsigned int mNbModesBackground; ///< Number of modes for the background (i.e. K).

  private:

    float mMinWeight;                ///< The minimum weight allowed (prevent a degeneration).
    float mMaxWeight;                ///< The maximum weight allowed (prevent a degeneration).
    float mMinCovariance;            ///< The minimum covariance allowed.
    float mMaxCovariance;            ///< The maximum covariance allowed.
    float mMinMean;                  ///< The minimum mean allowed.
    float mMaxMean;                  ///< The maximum mean allowed.
    
    unsigned int mDimension;         ///< The dimension of the gaussian components.
    
    float *mWeights;                 ///< The weight for each component of the mixture.
    float *mWeightsAccumulator;      ///< The weights accumulator.
    float **mMeans;                  ///< The mean for each gaussian component.
    float **mMeansAccumulator;       ///< The means accumulator.
    float **mCovariances;            ///< The diagonal values of the covariances matrix.
    float **mCovariancesAccumulator; ///< The covariances accumulator.
  };
}
#endif // ILLUMINATION_MODEL_DISTRIBUTION_ISOTROPIC_SIMPLE_HPP
