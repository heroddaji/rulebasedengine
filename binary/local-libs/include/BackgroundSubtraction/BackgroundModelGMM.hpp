/** \file
 * GMM based background model prototype.
 *
 * $Id: BackgroundModelGMM.hpp 1949 2011-01-12 10:37:45Z kris $
 */

#ifndef BACKGROUND_MODEL_GMM_HPP
#define BACKGROUND_MODEL_GMM_HPP 

#include <ViNotion/Image.hpp>
#include <ViNotion/Compat.hpp>

#include <Settings/Settings.hpp>

#include "BackgroundModelGMMParams.hpp"
#include "BackgroundDistributionGMM.hpp"
#include "BackgroundModel.hpp"

namespace Vi
{
  /**
   * Gaussian Mixture Model based background model.
   */
  class EXPORT_TYPE BackgroundModelGMM : public Vi::BackgroundModel
  {
  public:

    /**
     * Constructor.
     */
    BackgroundModelGMM(unsigned int fFrameWidth, unsigned int fFrameHeight);
    
    /**
     * Destructor.
     */
    ~BackgroundModelGMM();

    /**
     * Main processing function with object level feedback.
     *
     * \note Convert the current frame to RGB if needed.
     * \note Call the background distribution processing function. Here, the
     *    background update takes segmentation results into account: the pixels
     *    belonging to a detected object won't be updated.
     * \note Build the background image for display purpose.
     *
     * \param[in]         fFrame Input frame.
     * \param[in] fObjectMaskYUV Input object mask (of input frame size).
     */
    void process(const Vi::Image<> &fFrame, const Vi::Image<> &fObjectMaskYUV);

    /**
     * Build the background image.
     *
     * \note The background image is built using the background distribution.
     * \note Each pixel is set to the mean of the gaussian in the mixture with the highest weight.
     * \note The background image is then converted from RGB to YUV (for display).
     */
    void buildBackgroundImage();

    /**
     * Run-time configuration function.
     */
    void configure(const Vi::Settings &fConfig);

    /// The background image .
    Vi::Image<> mBackgroundImage;

    /// The background / foreground segmentation mask in YUV.
    Vi::Image<> mBackgroundMaskYUV;
    
    /**
     * The probability distribution of the background.
     *
     * \note Has to be public to build the background image.
     */
    BackgroundDistributionGMM *mBackgroundDistribution;

  private:

#ifdef BACKGROUND_MODEL_GMM_USE_COLOR_INFO
    /// The current frame in RGB.
    Vi::Image<> mFrameRGB;

    /// The background model in RGB.
    Vi::Image<> mBackgroundImageRGB;
#endif // BACKGROUND_MODEL_GMM_USE_COLOR_INFO

    /// The width of the input frame.
    unsigned int mFrameWidth;

    /// The height of the input frame.
    unsigned int mFrameHeight;

    /// The total number of pixels in the image.
    unsigned int mSize;

    /// The previous frame (used in gain compensation)
    Vi::Image<> mPreviousFrame;
  };
}

#endif // BACKGROUND_MODEL_GMM_HPP
