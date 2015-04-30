/** \file
 * Header file for the video input interface class.
 *
 * $Id: VideoInput.hpp 1847 2010-11-12 13:54:35Z rick $
 */

#ifndef VIDEO_INPUT_HPP
#define VIDEO_INPUT_HPP

#include "Image.hpp"
#include "Compat.hpp"

namespace Vi
{
  /**
   * Video position indicator. This enumerator is used for seeking in video
   *  streams using an offset from a certain start position.
   */
  enum VideoPosId
  {
    VPI_SEEK_SET, ///< Seek position: begin.
    VPI_SEEK_CUR, ///< Seek position: current.
    VPI_SEEK_END  ///< Seek position: end.
  };

  /**
   * The video data input abstract base class. This class forms a
   *  generic video data source interface. Like a file stream, a video
   *  input has an internal cursor (a uint64_t, pointing at the
   *  current frame).
   *
   * \note During initialization, the internal frame index is set to
   *  the index of the first frame in the video source (i.e. value
   *  '0').
   */
  class EXPORT_TYPE VideoInput
  {
  public:

    /// Destructor.
    virtual ~VideoInput() {};

    /**
     * Read a frame from the video input. The read function is
     *  post-increment by definition, that is, the internal frame
     *  index is incremented (if possible) only _after_ the current
     *  frame was read. When the frame is reached that fulfills the
     *  End-Of-File (EOF) condition, the internal EOF flag is
     *  set. When a read operation is impossible (e.g. when the video
     *  source is at EOF) the function returns false.
     *
     * If a logic/runtime error occurs an exception is thrown from
     *  within the video input implementation.
     *
     *  \param[in] fImg The image returned by the read function (if
     *                  successful).
     *
     *  \return True when the read succeeded, false if not.
     */
    virtual bool read(Vi::Image<> &fImg) = 0;

    /**
     * Seek in the video input. The seek is executed from a start
     *  position in- dicated by the whence parameter. This function
     *  _always_ sets the internal frame index, the new internal frame
     *  index is returned. By checking whether the given index value
     *  corresponds to the returned value, one can check whether the
     *  seek operation succeeded. The seek offset from the end
     *  position (VPI_SEEK_END) is a backwards offset.
     *
     * If a logic/runtime error occurs an exception is thrown from
     *  within the video input implementation.
     *
     * \param[in] fOffset The offset position to seek (relative to
     *                    'whence').  
     * \param[in] fWhence The relative offset position for the seek
     *                    operation.
     *
     *  \return The newly set internal frame index.
     */
    virtual uint64_t seek(int64_t fOffset, VideoPosId fWhence = Vi::VPI_SEEK_CUR) = 0;

    /**
     * Get the internal frame index.
     *
     *  \return The current frame index.
     */
    virtual uint64_t tell() const = 0;

    /**
     * Get the End-Of-File (EOF) status. This function reads the
     *  internal EOF flag. The EOF flag will be reset if internally
     *  the EOF condition changed, or a seek operation is done to a
     *  non-EOF frame.
     *
     *  \return True if the video source is at EOF, otherwise false.
     */
    virtual bool eof() const = 0;

    /**
     * Get video source frame width.
     *
     * \return The frame height in pixels. Zero if the video source
     *         has not been initialized.
     */
    virtual unsigned int getWidth() const = 0;

    /**
     * Get video source frame height.
     *
     * \return The frame height in pixels. Zero if the video source has not been
     *    initialized.
     */
    virtual unsigned int getHeight() const = 0;

    /**
     * Get the length in frames.
     *
     * \return The length in frames. UINT64_MAX if not applicable.
     */
    virtual uint64_t getLength() const = 0;

    /**
     * Set loop mode. In loop mode, the read() function will seek to position
     *  VPI_SEEK_SET if the end of the video input data has been reached.
     *
     * \param[in] fState If true, the video input will loop, otherwise not.
     */
    virtual  VideoInput &setLoop(bool fState = true) { return *this; }
  };
}

#endif // VIDEO_INPUT_HPP
