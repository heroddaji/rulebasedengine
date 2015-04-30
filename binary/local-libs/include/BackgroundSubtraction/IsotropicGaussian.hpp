/** \file
 * Isotropic Gaussian prototype.
 *
 * $Id: IsotropicGaussian.hpp 1949 2011-01-12 10:37:45Z kris $
 */

#ifndef ISOTROPIC_GAUSSIAN_HPP
#define ISOTROPIC_GAUSSIAN_HPP

/**
 * Gaussian distribution with isotropic variance.
 */
class IsotropicGaussian
{
public:
    
  /**
   * Constructor.
   *
   * \param[in]       fDimension The dimensionality of the Gaussian.
   * \param[in] fInitialVariance The initial variance of the Gaussian.
   * \param[in]           fAlpha The initial weight of the Gaussian.
   */
  IsotropicGaussian(unsigned int fDimension,
                    float fInitialVariance,
                    float fAlpha);

  /**
   * Destructor.
   */
  ~IsotropicGaussian();

  /**
   * Update.
   *
   * \note This function updates the gaussian parameters (weight, mean, variance).
   * \note Implementation of equation (5), (6) and (14) of Zivkovic's paper.
   *
   * \param[in]            fAlpha The update factor.
   * \param[in]               fCT The prior.
   * \param[out] fDistancesToMean The distance between the current pixel's value and the mean.
   * \param[in]        fOwnership If the pixel "belongs" to the gaussian, it's true, false otherwise.
   */
  void update(float fAlpha, float fCT, float** fDistancesToMean, bool fOwnership);

   /**
    * Check whether the component is not significant enough anymore.
    * 
    * \note If the weight is below a certain threshold, discard the mode.
    *
    * \param[in] fAlpha The update factor.
    * \param[in]    fCT The prior.
    *
    * \return True if not significant enough, otherwise false.
    */
  bool checkPrune(float fAlpha, float fCT);

  /**
   * Set variables in case a new mode is created.
   *
   * \note Sets the weight and variance to their specified initial values.
   * \note Sets the mean vector to the current pixel's value.
   *
   * \param[in] fPixelValues The RGB value of the pixel.
   */ 
  void setVariables(float** const fPixelValues);

  unsigned int mDimension; ///< The dimension of the gaussian.
  float* mMeans;           ///< The means.
  float mInitialVariance;  ///< The initial variance.
  float mInitialWeight;    ///< The initial weight.
  float mVariance;         ///< The variance.
  float mWeight;           ///< The weight.

private:

   /**
    * Function to test the range of the updated mean.
    * 
    * \note make sure that the new value is within [0.0f-255.0f].
    *
    * \param[in] fValue The updated value to test.
    *
    * \return The mean value.
    */
  float testRangeMean(float fValue);

  /**
   * Function to test the range of the updated variance.
   * 
   * \note Make sure that the new value is within [4-5 * initialVariance].
   *
   * \param[in] fValue The updated value to test.
   *
   * \return The variance value.
   */
  float testRangeVariance(float fValue);

  float* mUpdatedMeans; ///< A temp array for the updated mean values.
};

#endif // ISOTROPIC_GAUSSIAN_HPP
