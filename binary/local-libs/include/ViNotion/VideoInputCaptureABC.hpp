/** \file
 * Video input capture device base class.
 *
 * $Id: VideoInputCaptureABC.hpp 1722 2010-09-17 14:58:29Z rick $
 */

#ifndef VIDEO_INPUT_CAPTURE_ABC_HPP
#define VIDEO_INPUT_CAPTURE_ABC_HPP

#include "VideoInput.hpp"

#include <string>
#include <vector>

namespace Vi
{
  /// Field order enumeration.
  enum FIELD_ORDER
  {
    FO_ANY, ///< Driver chooses ordering.
    FO_TOP  ///< Only top fields.
  };

  /**
   * Video capturing ABC. Platform dependent capture code should implement this API.
   */
  class VideoInputCaptureABC : public VideoInput
  {
  public:
    /**
     * Initialize capture device for reading frames.
     *
     * \param[in] fId  Index in the vector returned by listDevs.
     * \param[in] fW   Requested width.
     * \param[in] fH   Requested height.
     * \param[in] fFps Requested frames per second.
     */
    virtual void init(unsigned int fId, unsigned int fW, unsigned int fH, unsigned int fFps) = 0;

    /**
     * List available capture devices.
     *
     * \return Vector of strings containing a descriptive string for each device.
     */
    virtual std::vector<std::string> listDevs() = 0;

    /**
     * Close capture device.
     */
    virtual void close() = 0;

    /**
     * Start capturing.
     */
    virtual void start() {}

    /**
     * Stop capturing.
     */
    virtual void stop() {}

    bool eof() const { return false; }
    
    /**
     * Set number of buffers for incoming frames.
     *
     * \param[in] fCount Number of frames.
     *
     * \return Reference to this object.
     */
    virtual VideoInputCaptureABC& setBufferCount(unsigned int fCount) { return *this; }
    
    /**
     * Set field order.
     *
     * \param[in] fFo Field order.
     *
     * \return Reference to this object.
     */
    virtual VideoInputCaptureABC& setFieldOrder(FIELD_ORDER fFo) = 0;// { return *this; }
  };
}

#endif // VIDEO_INPUT_CAPTURE_ABC_HPP
