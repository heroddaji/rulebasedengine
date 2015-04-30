/** \file
 * Running average based background model prototype.
 *
 * $Id: BackgroundModelRunAvg.hpp 1946 2011-01-11 13:38:34Z kris $
 */

#ifndef BACKGROUND_MODEL_RUN_AVG_HPP
#define BACKGROUND_MODEL_RUN_AVG_HPP 

#include <ViNotion/Image.hpp>
#include <ViNotion/Compat.hpp>

#include <Settings/Settings.hpp>

#include "BackgroundModel.hpp"

namespace Vi
{
  /**
   * Running average based background model.
   */
  class EXPORT_TYPE BackgroundModelRunAvg : public Vi::BackgroundModel
  {
  public:

    /**
     * Constructor.
     */
    BackgroundModelRunAvg(unsigned int fFrameWidth,
                          unsigned int fFrameHeight);
    
    void process(const Vi::Image<> &fFrame,
                 const Vi::Image<> &fObjectMaskYUV);

    /**
     * Build the background image.
     */
    void buildBackgroundImage();

    /**
     * Run-time configuration function.
     */
    void configure(const Vi::Settings &fConfig);

    /// The background image.
    Vi::Image<> mBackgroundImage;

  private:
   
    /// The background image (32-bit integer values).
    Vi::Image<int32_t> mBackgroundImageInt32;

    /// The width of the input frame.
    unsigned int mFrameWidth;

    /// The height of the input frame.
    unsigned int mFrameHeight;

    /// Internal frame counter.
    unsigned int mFrameCounter;

    // ------------------------------------------------------------------------
    /// \{ \name Class Run-time configurable parameters

    /**
     * The amount of frames used for learning the background. This number also
     *  determines the learning rate for later frames (i.e. 100 learning frames
     *  equals a learning rate of 1/100).
     */
    uint32_t mBgLearnFrames;

    /// Learning rate^(-1) for foreground pixels (as given in the object mask).
    uint32_t mFgLearnRate;

    /// The threshold for a pixel difference value to become an object pixel.
    uint8_t mPixDiffThreshold;
  };
}

#endif // BACKGROUND_MODEL_RUN_AVG_HPP
