/** \file
 *  Header file for the image file video source class.
 *
 *  $Id: VideoInputImageFile.hpp 1674 2010-09-10 14:00:17Z rick $
 */

#ifndef VIDEO_INPUT_IMAGEFILE_HPP
#define VIDEO_INPUT_IMAGEFILE_HPP

#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include "VideoInput.hpp"
#include "Compat.hpp"

namespace Vi
{
  /// Implementation of a video input based on image files.
  class EXPORT_TYPE VideoInputImageFile : public VideoInput
  {
  public:

    /**
     * Constructor. If a logic/runtime error occurs an exception is thrown.
     *
     *  \param[in] fPath The path to the image file directory.
     */
    VideoInputImageFile(const std::string fPath);

    bool read(Image<uint8_t> &fImg);
    uint64_t seek(int64_t fOffset, VideoPosId fWhence = Vi::VPI_SEEK_CUR);
    uint64_t tell() const;
    bool eof() const;
    uint64_t getLength() const;

    /**
     * Get video source frame width.
     *
     * \note Frame dimensions represent the first frame in the image
     *  directory.
     *
     * \return The frame height in pixels.
     */
    unsigned int getWidth() const;

    /**
     * Get video source frame height.
     *
     * \note Frame dimensions represent the first frame in the image
     *  directory.
     *
     * \return The frame height in pixels.
     */
    unsigned int getHeight() const;

  private:

    /// The internal image directory path.
    boost::filesystem::path mDirPath;

    /// The frame file name list.
    std::vector<std::string> mFrameNameList;

    /// The current frame index.
    uint64_t mCurFrameIndex;

    /// The maximum frame index value.
    uint64_t mMaxFrameIndex;

    /// End-Of-File flag.
    bool mEofFlag;

    /// Frame width.
    unsigned int mFrameWidth;

    /// Frame height.
    unsigned int mFrameHeight;
  };
}
#endif  // VIDEO_INPUT_IMAGEFILE_HPP
