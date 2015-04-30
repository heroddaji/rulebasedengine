/** \file
 * Object detector abstract base class definition.
 *
 * $Id$
 */

#ifndef OBJECT_DETECTOR_HPP
#define OBJECT_DETECTOR_HPP

#include <ViNotion/Compat.hpp>
#include <ViNotion/Image.hpp>
#include <ViNotion/Bbox.hpp>

namespace Vi
{
  /**
   * Object detector abstract base class.
   */
  class EXPORT_TYPE ObjectDetector
  {
  public:

    /**
     * Virtual desctructor to allow for destruction using base pointers.
     */
    virtual ~ObjectDetector() {}

    /**
     * Detect objects.
     *
     * \param[in]          fFrame Input frame.
     * \param[in,out] fDetections Vector of bounding boxes for detected objects.
     */
    virtual void detect(const Vi::Image<> &fFrame,
                        const Vi::Image<> &fObjectMask,
                        std::vector<Vi::Bbox<int> > &fDetections) = 0;
  };
}

#endif // OBJECT_DETECTOR_HPP
