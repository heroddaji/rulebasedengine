/** \file
 * Linux capture driver header file.
 *
 * $Id: VideoInputCaptureV4L.hpp 1722 2010-09-17 14:58:29Z rick $
 */

#ifndef VIDEO_INPUT_CAPTURE_V4L_HPP
#define VIDEO_INPUT_CAPTURE_V4L_HPP

#include "Image.hpp"
#include "VideoInputCaptureABC.hpp"

#include <linux/videodev2.h>

#include <string>
#include <vector>

namespace Vi
{
  /**
   * Video 4 Linux 2 capturing.
   */
  class VideoInputCaptureV4L : public VideoInputCaptureABC
  {
  public:
    /**
     * Constructor.
     */
    VideoInputCaptureV4L();

    /**
     * Destructor.
     */
    ~VideoInputCaptureV4L();

    std::vector<std::string> listDevs();
    VideoInputCaptureV4L& setBufferCount(unsigned int fCount);
    VideoInputCaptureV4L& setFieldOrder(Vi::FIELD_ORDER fFo);
    void init(unsigned int fId, unsigned int fW, unsigned int fH, unsigned int fFps);
    bool read(Vi::Image<> &fImage);
    void close();
  
    uint64_t seek(int64_t fOffset, VideoPosId fWhence = Vi::VPI_SEEK_CUR)
    { return mCurIndex; }

    uint64_t tell() const
    { return mCurIndex; }

    unsigned int getWidth() const
    { return mWidth; }

    unsigned int getHeight() const
    { return mHeight; }

    uint64_t getLength() const
    { return static_cast<uint64_t>(-1); } // UINT64_MAX

  private:
    /**
     * Enumerate and display all pixel formats.
     */
    void enumFmts();

    /**
     * Enumerate and display input formats.
     */
    void enumInputs();

    /**
     * Convert a pixel format to a human readable string.
     *
     * \param[in] fFmt format.
     *
     * \return Human readable string.
     */
    std::string fmt2string(__u32 fFmt);

    /**
     * Convert V4L2 video standard to a human readable string.
     *
     * \param[in] fStd V4L2 video standard id.   
     */
    std::string std2string(v4l2_std_id fStd);

    /**
     * Set pixel format.
     *
     * \param[in] fVf Partially filled V4L2 format structure to try.
     */
    void setFormat(v4l2_format fVf);

    int mCamFd;           ///< Camera device file descriptor.
    unsigned int mWidth;  ///< Width of captured frames [px].
    unsigned int mHeight; ///< Height of captured frames [px].
    bool mIsInit;         ///< True if a capture device is initialized.
    uint64_t mCurIndex;   ///< Current frame index.

    /// Only promoted to member variable to store the number of buffers (vrb.count).
    struct v4l2_requestbuffers mVrb;

    /// Requested field ordering.
    v4l2_field mFieldOrder;

    /// Frame buffer structure.
    struct Buffer
    {
      Image<> im;    ///< ViNotion image alias for mmap'ed memory.
      void *start;   ///< Start of mmap'ed image.
      size_t length; ///< Size of mmap'ed image.
    };
    Buffer *mBuffers; ///< Buffer list with vrb.count items.

    PIXEL_FORMAT mFmt; ///< Pixel format used by capture device.
  };

  /// Never use VideoInputCaptureV4L, use this typedef alias VideoInputCapture.
  typedef VideoInputCaptureV4L VideoInputCapture;
}

#endif // VIDEO_INPUT_CAPTURE_V4L_HPP
