/** \file
 * Background model abstract base class.
 *
 * $Id: BackgroundModel.hpp 1923 2010-12-21 16:25:21Z kris $
 */

#ifndef BACKGROUND_MODEL_HPP
#define BACKGROUND_MODEL_HPP 

#include <ViNotion/Image.hpp>

namespace Vi
{
  /**
   * Background model abstract base class.
   */
  class EXPORT_TYPE BackgroundModel
  {
  public:

    /**
     * Virtual desctructor to allow for destruction using base pointers.
     */
    virtual ~BackgroundModel() {}

    /**
     * Main processing function with object level feedback.
     *
     * For each white pixel in the object mask, the background model is not
     *  updated. Provide a full black object mask if complete background
     *  model updating is needed.
     *
     * In other words, it is up to the higher level application to specify
     *  the object mask.
     *
     * \param[in]      fFrame Input frame.
     * \param[in] fObjectMask Input object mask (of input frame size).
     */
    virtual void process(const Vi::Image<> &fFrame, const Vi::Image<> &fObjectMask) = 0;

    /// The background / foreground segmentation mask in YUV.
    Vi::Image<> mBackgroundMaskYUV;
  };
}

#endif // BACKGROUND_MODEL_HPP
