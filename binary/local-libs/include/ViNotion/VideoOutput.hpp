/** \file
 * Header file for the video output interface class.
 *
 * $Id: VideoOutput.hpp 1656 2010-09-08 14:53:42Z rick $
 */

#ifndef VIDEO_OUTPUT_HPP
#define VIDEO_OUTPUT_HPP

#include "Image.hpp"
#include "Compat.hpp"

namespace Vi
{
  /// Abstract base class for video output devices.
  class EXPORT_TYPE VideoOutput
  {
  public:

    /**
     * Destructor.
     *
     * \note we need it here for being able to delete a derived class instance
     *  using a base class pointer.
     */
    virtual ~VideoOutput() {}

    /**
     * Write an image.
     *
     * \param[in] fImg The input image.
     *
     * \return True if success, false if otherwise.
     */
    virtual bool write(const Vi::Image<> &fImg) = 0;
  };
}

#endif // VIDEO_OUTPUT_HPP
