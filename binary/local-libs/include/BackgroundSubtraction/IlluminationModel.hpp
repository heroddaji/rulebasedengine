/** \file
 * Illumination Model for background subtraction prototype.
 *
 *  \note This model is based on ideas found in the following paper:
 *        "Making Background Subtraction Robust To Sudden Illumination Changes",
 *        J. Pilet, C. Strecha and P. Fua, ECCV 2008.
 *
 * $Id: IlluminationModel.hpp 1902 2010-12-06 12:56:08Z guillaume $
 */

#ifndef ILLUMINATION_MODEL_HPP
#define ILLUMINATION_MODEL_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Image.hpp>

#include "IlluminationModelParams.hpp"
#include "IlluminationModelDistributionIsotropic.hpp"
#include "BackgroundModel.hpp"

namespace Vi
{
  /**
   * Illumination model
   */
  class EXPORT_TYPE IlluminationModel : public Vi::BackgroundModel
  {
  public:

    /**
     * Constructor.
     *
     * \param[in] fFrame The image usually corresponding to the first frame of the sequence. 
     */
    IlluminationModel(const Vi::Image<> &fFrame);
    
    /**
     * Destructor.
     */
    ~IlluminationModel();

    /**
     * Main processing function.
     * 
     * \note  In this function, the background model is updated. The ratio between the current
     *        frame and the background and then, the belief for each pixel is computed, 
     *        the distribution is updated, and finally the background probability is computed 
     *        to perform background / foreground segmentation.
     *
     * \param[in]      fFrame The current frame.
     * \param[in] fObjectMask The object mask for object level feedback.
     */
    void process(const Vi::Image<> &fFrame, const Vi::Image<> &fObjectMask); 
    
    /**
     * This function builds a background image out of the background model.
     */
    void buildBackgroundImage();
    
    Vi::Image<> mBackgroundImageYUV;  ///< The background image for display. 
 
  private:
   
    /**
     * Update the background model.
     *
     * \note  This function implements a background model update, which is
     *        based on exponential moving average with the background model and the current frame.
     *        Note also that the model is not updated where objects have been detected. 
     *
     * \param[in]      fFrame The current frame.
     * \param[in] fObjectMask The mask of detected objects.
     */
    void updateBackgroundModel(const Vi::Image<> &fFrame, const Vi::Image<> &fObjectMask);
    
    /**
     * Compute the illumination ratio.
     *
     * \note  Compute the illumination ratio between the pixels in the current frame and the background 
     *        model. Compute also the determinant of the Jacobian (see equation (3)).
     *
     * \param[in]      fFrame The current frame.
     */
    void computeIlluminationRatio(const Vi::Image<> &fFrame);

    /**
     * Compute pixel belief.
     *
     * \note  This is the complete implementation of equations (6) and (7).
     *
     * \param[in] fFrame The current frame.
     */
    void computeBeliefs(const Vi::Image<> &fFrame);

    /**
     * Compute background probability.
     *
     * \note  This is the implementation of equation (11). The segmentation is done by comparing the
     *        probability to a threshold, and then the background mask is updated.
     *
     * \param[in] fFrame The current frame.
     */
    void computeBackgroundProbability(const Vi::Image<> &fFrame);

    IlluminationModelDistributionIsotropic *illumDistrib; ///< The distribution.
    float ***mBackgroundModel;                            ///< The background model.

    Vi::Image<> mFrameRGB;              ///< The current frame.

    unsigned int mFrameWidth;           ///< The width of the current frame.
    unsigned int mFrameHeight;          ///< The height of the current frame.
    unsigned int mNbChannels;           ///< The number of channels in the video sequence.
   
    unsigned int mRatioOffset;          ///< The offset used to compute the pixel ratio (see the paper)
    float ***mIlluminationRatio;        ///< Store the ratio for each pixel.
    float *mPixelValues;                ///< Store the value(s) of a pixel.
    float **mDetJacobian;               ///< Store the determinant of the Jacobian for each pixel.
    float *mPixelBeliefs;               ///< Store the beliefs of a pixel.
    Vi::Image<> mBackgroundImageRGB;    ///< The background image.
  };
}
#endif //ILLUMINATION_MODEL_HPP
